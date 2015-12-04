//
//  GameScene.h
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-16.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_GameScene_h
#define DragonBattle_GameScene_h

#include "cocos2d.h"

typedef enum
{
    LOGIN_SCENCE = 0,
    COMMUNITY_SCENCE,
    MISSION_SCENCE,
    NEW_SCENCE,
    LOADING_SCENCE,
    SPORT_SCENE
} SceneType;

enum kMainLayerTag
{
    kLoginSceneTag,     //登录页面
    kSelectServerLayerTag,  //选择服务器页面
    kCommunityLayerTag, //主城
    kBattleSceneTag,    //战斗场景
    kLoadingLayerTag,
    kShowMemoryLayerTag,
    
    kTutorialLayerTag = 127,
    kTopLayerTag = 131
};

class GameScene
{
public:
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
};


#endif
