//
//  DragonBattleAppDelegate.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-15.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//
#include "AppDelegate.h"
#include "cocos2d.h"
#include "GlobalDefine.h"
#include "GameManager.h"
#include "LogicManager.h"
#include "SimpleAudioEngine.h"
#include "ServerConfig.h"
#include "TimerManager.h"
#include "ResourcesManager.h"
USING_NS_CC;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

#ifndef SERVER_RELEASE
    // turn on display FPS
    pDirector->setDisplayStats(true);
#endif
    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);
    
    pDirector->setProjection(kCCDirectorProjection2D);
    
    // OpenGL View
    CCEGLView *glView = CCEGLView::sharedOpenGLView();
    pDirector->setOpenGLView(glView);
    //-- DesignResolutionSize
    float designWidth = 960;
    float designHeight = 640;
    CCSize winSize = pDirector->getWinSize();
    if (winSize.width / designWidth > winSize.height / designHeight)
    {
        designWidth = designHeight * winSize.width / winSize.height;
    }
    else
    {
        designHeight = designWidth * winSize.height / winSize.width;
    }
    glView->setDesignResolutionSize(designWidth, designHeight, kResolutionShowAll);
    Global::initGlobalParams(designWidth,designHeight);
    
    // texture setting
    CCTexture2D::PVRImagesHavePremultipliedAlpha(true);
    GameManager::sharedInstance()->enterLoginLayer();
    
    ResourcesManager::sharedInstance()->uncompressRes();
/*
    char *archivePathPtr = (char *)(CCFileUtils::sharedFileUtils()->fullPathForFilename("action.7z").c_str());
    std::string downpath= CCFileUtils::sharedFileUtils()->getWritablePath() + "123.cache";
    std::string entryPathPtr = CCFileUtils::sharedFileUtils()->getWritablePath() + "pic/";
    char *path = (char*)downpath.c_str();
    char *path1 = (char*)entryPathPtr.c_str();
    if (!exitFile(entryPathPtr.c_str()))
    {
        int result = do7z_extract_entry(archivePathPtr,path, NULL,NULL,path1);
        if (result == 0)
        {
            
        }
    }*/
    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->pause();
    CCDirector::sharedDirector()->stopAnimation();
    // if you use SimpleAudioEngine, it must be pause
    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    
    TimerManager::getInstance()->recordLeaveTime();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->resume();
    CCDirector::sharedDirector()->startAnimation();
    LogicManager::shareLogicManager()->enterForeground();
    
    // if you use SimpleAudioEngine, it must resume here
    CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    
    // recalculate the time when game enter foreground.
    TimerManager::getInstance()->calculateTime();
}