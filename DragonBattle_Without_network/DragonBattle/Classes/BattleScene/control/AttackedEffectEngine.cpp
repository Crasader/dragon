//
//  ScheduleTimesEngine.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-5-16.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "AttackedEffectEngine.h"
#include "BattleUnitSprite.h"

USING_NS_CC;

ScheduleEngine::ScheduleEngine()
{
    m_nCurIndex = 0;
    m_nRunTimes = 0;
}

ScheduleEngine::~ScheduleEngine()
{
    
}

void ScheduleEngine::startWork(float delayPerUnit)
{
    this->scheduleUpdate(float(0));
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(ScheduleEngine::scheduleUpdate), this, delayPerUnit, false);
}

bool ScheduleEngine::scheduleUpdate(float dt)
{
    if (m_nCurIndex >= m_nRunTimes)
    {
        m_nCurIndex = 0;
        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(ScheduleEngine::scheduleUpdate), this);
        return false;
    }
    
    m_nCurIndex++;
    return true;
}


// ______________________________________________________________________________________________________________________
// ______________________________________________________________________________________________________________________
AttackedEffectEngine::AttackedEffectEngine()
{
    m_pTargetUnit = NULL;
    m_pAttack = NULL;
}

AttackedEffectEngine::~AttackedEffectEngine()
{
    m_pTargetUnit = NULL;
    m_pAttack = NULL;
}

AttackedEffectEngine* AttackedEffectEngine::create(BattleUnitSprite* unit, AttackInfo_t* pAttack, int pSkillConfIDLV, int runTimes, float fHurt)
{
    AttackedEffectEngine* pRet = new AttackedEffectEngine();
    
    if (pRet && pRet->init(unit, pAttack,pSkillConfIDLV, runTimes, fHurt))
    {
        pRet->autorelease();
        return pRet;
    }
    
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool AttackedEffectEngine::init(BattleUnitSprite* pUnitSprite, AttackInfo_t* pAttack, int pSkillConfIDLV, int runTimes, float fHurt)
{
    m_pAttack = pAttack;
    m_pSkillConfIDLV = pSkillConfIDLV,
    m_pTargetUnit = pUnitSprite;
    m_nRunTimes = runTimes;
    m_fHurt = fHurt;
    return true;
}

// 
bool AttackedEffectEngine::scheduleUpdate(float dt)
{
    if (ScheduleEngine::scheduleUpdate(dt))
    {
        if (m_nCurIndex <= m_nRunTimes)
        {
            //暂时先直接传入伤害值，具体的伤害值是多少要等公式出来后再确定
            m_pTargetUnit->underAttackHarmAction((int)(m_fHurt / m_nRunTimes));
            m_pTargetUnit->playUnderAttackEffect(m_pAttack);
        }
        return true;
    }
    else
    {
        return false;
    }
}