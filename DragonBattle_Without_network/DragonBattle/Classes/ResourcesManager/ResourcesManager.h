
//
//  ResoucesManager.h
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-16.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_ResoucesManager_h
#define DragonBattle_ResoucesManager_h

#include "UiResources.h"
#include "UnitResource.h"
#include "EffectResource.h"

// 图片资源
class ResourcesManager
{
public:
    bool initResources();
    
    static ResourcesManager* sharedInstance();
    static void purgeInstance();
    
    cocos2d::CCSprite* createUISpriteWithID(uint32_t ID);
    UIResourceInfo_t* getUIResourceWithID(uint32_t ID);
    
    cocos2d::CCAnimation*   createAnimation(uint32_t unitID, kAnimationType animType);
    cocos2d::CCAnimate*     createAnimate(uint32_t unitID, kAnimationType animType);
    cocos2d::CCSprite*      getUnitSprite(uint32_t unitID);
    const UnitResource_t*   getUnitResConf(uint32_t nUnitType) const;
    
    cocos2d::CCAnimation* createThrowAnimation(AttackEffectInfo_t* conf);
    cocos2d::CCAnimate* createThrowAnimate(AttackEffectInfo_t* conf);    
    ThrowResource_t* getThrowConf(uint32_t nThrowID);
    
    void uncompressRes();
private:
    bool uncompress(const char* zipfile);
    bool createDirectory(const char *path);
private:
    static ResourcesManager* pSharedInstance;
};

#define  CREATE_UI_SPRITE  ResourcesManager::sharedInstance()->createUISpriteWithID

#endif
