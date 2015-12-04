//
//  ResoucesManager.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-16.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "ResourcesManager.h"
#include "Resources.h"
#include "SpriteFrameManager.h"
#include "GameData.h"
#include "tools.h"
#include "UserData.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT) && (CC_TARGET_PLATFORM != CC_PLATFORM_WP8)
#include <curl/curl.h>
#include <curl/easy.h>

#include <stdio.h>
#include <vector>
#endif

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#endif

#include "support/zip_support/unzip.h"

#define BUFFER_SIZE    8192
#define MAX_FILENAME   512


USING_NS_CC;

ResourcesManager* ResourcesManager::pSharedInstance = NULL;
ResourcesManager* ResourcesManager::sharedInstance()
{
    if (!pSharedInstance)
    {
        pSharedInstance = new ResourcesManager();
    }
    return pSharedInstance;
}

void ResourcesManager::purgeInstance()
{
    
}

bool ResourcesManager::initResources()
{
    UIResources::sharedInstance()->initUIResources();
//    UnitResource::sharedInstance()->initAllUnitResouce();
    UnitResource::sharedInstance()->loadUnitResouceFromFile();
    ThrowResource::sharedInstance()->initAllThrowResource();
    return true;
}

CCSprite* ResourcesManager::createUISpriteWithID(uint32_t ID)
{
    return UIResources::sharedInstance()->createUISpriteWithID(ID);
}

UIResourceInfo_t* ResourcesManager::getUIResourceWithID(uint32_t ID)
{
    return UIResources::sharedInstance()->getUIResourceWithID(ID);
}

CCAnimation* ResourcesManager::createAnimation(uint32_t unitID, kAnimationType animType)
{
    return UnitResource::sharedInstance()->createAnimation(unitID, animType);
}

cocos2d::CCAnimate* ResourcesManager::createAnimate(uint32_t unitID, kAnimationType animType)
{
    return UnitResource::sharedInstance()->createAnimate(unitID, animType);
}

cocos2d::CCSprite* ResourcesManager::getUnitSprite(uint32_t unitID)
{
    return UnitResource::sharedInstance()->getUnitSprite(unitID);
}

const UnitResource_t* ResourcesManager::getUnitResConf(uint32_t nUnitType) const
{
    return UnitResource::sharedInstance()->getUnitResConf(nUnitType);
}

CCAnimation* ResourcesManager::createThrowAnimation(AttackEffectInfo_t* conf)
{
    return ThrowResource::sharedInstance()->createAnimation(conf);
}

CCAnimate* ResourcesManager::createThrowAnimate(AttackEffectInfo_t* conf)
{
    return ThrowResource::sharedInstance()->createAnimate(conf);
}

ThrowResource_t* ResourcesManager::getThrowConf(uint32_t nThrowID)
{
    return ThrowResource::sharedInstance()->getConf(nThrowID);
}

void ResourcesManager::uncompressRes()
{
    CCFileUtils::sharedFileUtils()->addSearchPath(CCFileUtils::sharedFileUtils()->getWritablePath().c_str());
    CCFileUtils::sharedFileUtils()->addSearchResolutionsOrder(CCFileUtils::sharedFileUtils()->getWritablePath().c_str());
    
    //uncompress("action.dat");
}

bool ResourcesManager::uncompress(const char* file)
{
    
    
    string outFileName = CCFileUtils::sharedFileUtils()->fullPathForFilename(file);
    unzFile zipfile = unzOpen(outFileName.c_str());
    if (! zipfile)
    {
        CCLOG("can not open downloaded zip file %s", outFileName.c_str());
        return false;
    }
    
    // Get info about the zip file
    unz_global_info global_info;
    if (unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK)
    {
        CCLOG("can not read file global info of %s", outFileName.c_str());
        unzClose(zipfile);
        return false;
    }
    
    char readBuffer[BUFFER_SIZE];
    
    CCLOG("start uncompressing");
    
    // Loop to extract all files.
    uLong i;
    for (i = 0; i < global_info.number_entry; ++i)
    {
        // Get info about current file.
        unz_file_info fileInfo;
        char fileName[MAX_FILENAME];
        if (unzGetCurrentFileInfo(zipfile,
                                  &fileInfo,
                                  fileName,
                                  MAX_FILENAME,
                                  NULL,
                                  0,
                                  NULL,
                                  0) != UNZ_OK)
        {
            CCLOG("can not read file info");
            unzClose(zipfile);
            return false;
        }
        
        string fullPath = fileName;
        
        int pos = fullPath.find("Resources");
        fullPath = CCFileUtils::sharedFileUtils()->getWritablePath() + fullPath.substr(pos + strlen("Resources") + 1, fullPath.size());
        
        // Check if this entry is a directory or a file.
        const size_t filenameLength = strlen(fileName);
        if (fileName[filenameLength-1] == '/')
        {
            // Entry is a direcotry, so create it.
            // If the directory exists, it will failed scilently.
            if (!createDirectory(fullPath.c_str()))
            {
                CCLOG("can not create directory %s", fullPath.c_str());
                unzClose(zipfile);
                return false;
            }
        }
        else
        {
            // Entry is a file, so extract it.
            
            // Open current file.
            if (unzOpenCurrentFile(zipfile) != UNZ_OK)
            {
                CCLOG("can not open file %s", fileName);
                unzClose(zipfile);
                return false;
            }
            
            // Create a file to store current file.
            FILE *out = fopen(fullPath.c_str(), "wb");
            if (! out)
            {
                CCLOG("can not open destination file %s", fullPath.c_str());
                unzCloseCurrentFile(zipfile);
                unzClose(zipfile);
                return false;
            }
            
            // Write current file content to destinate file.
            int error = UNZ_OK;
            do
            {
                error = unzReadCurrentFile(zipfile, readBuffer, BUFFER_SIZE);
                if (error < 0)
                {
                    CCLOG("can not read zip file %s, error code is %d", fileName, error);
                    unzCloseCurrentFile(zipfile);
                    unzClose(zipfile);
                    return false;
                }
                
                if (error > 0)
                {
                    fwrite(readBuffer, error, 1, out);
                }
            } while(error > 0);
            
            fclose(out);
        }
        
        unzCloseCurrentFile(zipfile);
        
        // Goto next entry listed in the zip file.
        if ((i+1) < global_info.number_entry)
        {
            if (unzGoToNextFile(zipfile) != UNZ_OK)
            {
                CCLOG("can not read next file");
                unzClose(zipfile);
                return false;
            }
        }
    }
    
    CCLOG("end uncompressing");
    
    return true;
}

bool ResourcesManager::createDirectory(const char *path)
{
    mode_t processMask = umask(0);
    int ret = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
    umask(processMask);
    if (ret != 0 && (errno != EEXIST))
    {
        return false;
    }
    
    return true;
}
