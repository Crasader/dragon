//
//  BattleLogic.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-19.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "GlobalDefine.h"
#include "BattleLogic.h"
#include "cocos2d.h"

bool BattleLogic::canChangeUnitAction(kRoleActionType oldAction, kRoleActionType newAction)
{
    bool ret = false;

//    CCLOG("Check Change Action oldAction:%d newAction:%d", oldAction, newAction);
    
    switch (oldAction)
    {
        //许翔：加入kStandby状态，待确认。
        case kRunning:
            if ((newAction == kJumpUp) || (newAction == kAttackBegin) || (newAction == kSkillBegin)
                || (newAction == kBeHit) || (newAction == kBeHitFly) || (newAction == kWin) || (newAction == kStandby)
                || (newAction == kRoll) || (newAction == kJumpInAir))
            {
                ret = true;
            }
            break;
            
        case kJumpUp:
            if ((newAction == kJumpInAir) || (newAction == kBeHit) || (newAction == kBeHitFly) || (newAction == kWin))
            {
                ret = true;
            }
            break;
            
        case kJumpInAir:
            if ((newAction == kJumpDown) || (newAction == kAirAttackBegin) || (newAction == kBeHit)
                || (newAction == kBeHitFly) || (newAction == kWin))
            {
                ret = true;
            }
            break;
        //许翔：加入kStandby状态，待确认。    
        case kJumpDown:
            if ((newAction == kAttackBegin) || (newAction == kBeHit) || (newAction == kBeHitFly) || (newAction == kWin) || (newAction == kStandby) || (newAction == kRoll))
            {
                ret = true;
            }
            break;
        //许翔：加入新kSkillOverDown选项。 
        case kSkillOverDown:
            if (newAction == kJumpDown || newAction == kBeHit)
            {
                ret = true;
            }
            break;
            
        case kAirAttackBegin:
            if ((newAction == kAirAttackValid) || (newAction == kBeHit) || (newAction == kBeHitFly) || (newAction == kWin))
            {
                ret = true;
            }
            break;
            
        case kAirAttackValid:
            if ((newAction == kAirAttackOver) || (newAction == kBeHit) || (newAction == kBeHitFly) || (newAction == kWin))
            {
                ret = true;
            }
            break;
        //许翔：加入kStandby状态，待确认。    
        case kAirAttackOver:
            if ((newAction == kBeHit) || (newAction == kBeHitFly) || (newAction == kWin) || (newAction == kJumpDown))
            {
                ret = true;
            }
            break;
            
        case kAttackBegin:
            if ((newAction == kAttackValid)
                || (newAction == kBeHit) || (newAction == kBeHitFly) || (newAction == kWin))
            {
                ret = true;
            }
            break;
        case kAttackValid:
            if ((newAction == kAttackOver)  || (newAction == kSkillBegin) || (newAction == kBeHitFly) || (newAction == kWin)|| (newAction == kBeHit))
            {
                ret = true;
            }
            break;
        //许翔：加入kStandby状态，待确认。
        case kAttackOver:
            if ((newAction == kAttackBegin) || (newAction == kSkillBegin) || (newAction == kBeHit) || 
                (newAction == kBeHitFly) || (newAction == kWin) || (newAction == kStandby || 
                (newAction == kRoll) || (newAction == kSkillOverDown)))
            {
                ret = true;
            }
            break;
            
        case kSkillBegin:
            if ((newAction == kSkillValid) || (newAction == kWin)||(newAction == kBeHit)||(newAction == kBeHitFly))
            {
                ret = true;
            }
            break;
        case kSkillValid:
            if ((newAction == kSkillOver) || (newAction == kWin)||(newAction == kBeHit)||(newAction == kBeHitFly))
            {
                ret = true;
            }
            break;
        //许翔：加入kStandby状态，待确认。    
        case kSkillOver:
            if ((newAction == kSkillBegin) || (newAction == kBeHit) || (newAction == kBeHitFly) || (newAction == kWin) || (newAction == kStandby) || (newAction == kSkillOverDown) || (newAction == kRoll))
            {
                ret = true;
            }
            break;
            
        case kStandby:
            if ((newAction == kRunning)
                || (newAction == kJumpUp) || (newAction == kAttackBegin) || (newAction == kSkillBegin)
                || (newAction == kBeHit) || (newAction == kBeHitFly) || (newAction == kWin) || (newAction == kRoll)
                )
            {
                ret = true;
            }
            break;
        //许翔：加入kStandby状态，待确认。    
        case kBeHit:
            if ((newAction == kSkillBegin) || (newAction == kBeHitFly) || (newAction == kWin) || (newAction == kBeHit) || (newAction == kStandby))
            {
                ret = true;
            }
            break;
            
        case kBeHitFly:
            if ((newAction == kBeHitDown) || (newAction == kWin))
            {
                ret = true;
            }
            break;
            
        case kBeHitDown:
            if (newAction == kWin || (newAction == kGetUp))
            {
                ret = true;
            }
            break;
        case kGetUp:
            if (newAction == kRoll || newAction == kStandby)
            {
                ret = true;
            }
            break;
        case kRoll:
            if (newAction == kStandby)
            {
                ret = true;
            }
        default:
            break;
    }
    
    return ret;
}

kRoleActionType BattleLogic::autoChangeUnitAction(kRoleActionType oldAction)
{
#define CASE_RETURN(condition, ret) case (condition): return (ret)
    switch (oldAction)
    {
        CASE_RETURN(kRunning, kStandby);
        CASE_RETURN(kJumpUp, kJumpInAir);
        CASE_RETURN(kJumpInAir, kJumpDown);
        CASE_RETURN(kJumpDown, kStandby);
        CASE_RETURN(kAirAttackBegin, kAirAttackValid);
        CASE_RETURN(kAirAttackValid, kAirAttackOver);
        CASE_RETURN(kAirAttackOver, kJumpInAir);
        CASE_RETURN(kAttackBegin, kAttackValid);
        CASE_RETURN(kAttackValid, kAttackOver);
        CASE_RETURN(kAttackOver, kStandby);
        CASE_RETURN(kSkillBegin, kSkillValid);
        CASE_RETURN(kSkillValid, kSkillOver);
        CASE_RETURN(kSkillOver, kStandby);
        CASE_RETURN(kStandby, kStandby);
        CASE_RETURN(kBeHit, kStandby);
        CASE_RETURN(kBeHitFly, kBeHitDown);
        CASE_RETURN(kBeHitDown, kGetUp);
        CASE_RETURN(kGetUp, kStandby);
        CASE_RETURN(kWin, kStandby);
        default:
            break;
    }
    return kStandby;
}