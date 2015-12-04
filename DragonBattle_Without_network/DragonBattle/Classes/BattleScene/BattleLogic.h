//
//  BattleLogic.h
//  DragonBattle
//  状态转变逻辑
//  Created by 小敦 王 on 13-4-19.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_BattleLogic_h
#define DragonBattle_BattleLogic_h

#include "GlobalDefine.h"

class BattleLogic
{
public:
    static bool canChangeUnitAction(kRoleActionType oldAction, kRoleActionType newAction);
    static kRoleActionType autoChangeUnitAction(kRoleActionType oldAction);
};

#endif
