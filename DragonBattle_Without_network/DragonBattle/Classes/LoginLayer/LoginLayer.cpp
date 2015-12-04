//
//  LoginLayer.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-16.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "LoginLayer.h"
#include "Resources.h"
#include "GameManager.h"
#include "GlobalDefine.h"
#include "UserData.h"
#include "LogicManager.h"
bool LoginLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    return true;
}

