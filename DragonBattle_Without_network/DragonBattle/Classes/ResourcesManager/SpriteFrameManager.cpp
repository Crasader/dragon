//
//  SpriteFrameManager.cpp
//  DragonBattle
//
//  Created by long on 13-5-29.
//
//

#include "SpriteFrameManager.h"
static SpriteFrameManager* pSharedInstace = NULL;
SpriteFrameManager* SpriteFrameManager::sharedInstance()
{
    if (!pSharedInstace)
    {
        pSharedInstace = new SpriteFrameManager();
    }
    return pSharedInstace;
}

SpriteFrameManager::SpriteFrameManager()
{

}

SpriteFrameManager::~SpriteFrameManager()
{

}

void SpriteFrameManager::addSpriteFrameWithFile(const char *pszPlist,PathType ktype)
{
    switch(ktype)
    {
        case kCachePath:
        {
                std::string pszPath1 = CCFileUtils::sharedFileUtils()->getWritablePath() + "pic/" + pszPlist;
                CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(pszPath1.c_str());
            break;
        }
        case kRelativePath:
        {
            CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(pszPlist);
            break;
        }
    }
}