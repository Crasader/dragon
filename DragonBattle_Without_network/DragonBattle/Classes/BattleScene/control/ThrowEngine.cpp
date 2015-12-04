//
//  ThrowEngine.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-5-16.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "ThrowEngine.h"
#include "BattleUnitSprite.h"
#include "GameManager.h"
#include "BattleMapUnits.h"

USING_NS_CC;

ThrowEngine::ThrowEngine()
{
    m_pAttackInfo = NULL;
    m_pTargetUnit = NULL;
    m_nCurThrowIndex = 0;
    m_nDir = 0;
}

ThrowEngine::~ThrowEngine()
{
    m_pAttackInfo = NULL;
    m_pTargetUnit = NULL;
}

ThrowEngine* ThrowEngine::create(BattleUnitSprite* pUnitSprite)
{
    ThrowEngine* pRet = new ThrowEngine();
    
    if (pRet && pRet->init(pUnitSprite))
    {
        pRet->autorelease();
        return pRet;
    }
    
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool ThrowEngine::init(BattleUnitSprite* pUnitSprite)
{
    m_pSkillConfID = pUnitSprite->m_nAttackSkill;
    m_pAttackInfo = pUnitSprite->getCurAttack();
    m_unitBaseConf = *(pUnitSprite->getUnitBaseConf());
    m_pTargetUnit = pUnitSprite;
    setThrowVec(&m_pAttackInfo->throwVec);
    setThrowMode(m_pAttackInfo->nThrowMode);
    setIntervarTime(m_pAttackInfo->nThrowintervalTime);
    m_nDir = m_pTargetUnit->isFlipX() ? 1 : 2;
    m_Point = m_pTargetUnit->getPosition();
    return true;
}

void ThrowEngine::setThrowVec(std::vector<uint16_t>  *throwVec)
{
    m_throwVec = *throwVec;
}

void ThrowEngine::setThrowMode(kThrowMode mode)
{
    m_kThorwMode = mode;
}
// 给外部的接口，生成抛出物
void ThrowEngine::throwObject()
{
    // 是否有抛储物
    if (m_pAttackInfo->nThrowMode != kThrowNull)
    {
        if (m_pAttackInfo->nThrowMode == kThrowSequence)
        {
            // 抛出第一个
            this->engineThrowObject(0);
            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(ThrowEngine::engineThrowObject), this, (float)m_nIntervarTime * ANIMAT_DELAY_PER_UNIT, false);
        }
        else if (m_pAttackInfo->nThrowMode == kThrowMultiOnceTime)
        {
            for (int i = 0; i < m_throwVec.size(); i++)
            {
                this->throwObject(i);
            }
        }
    }
}

// 多个抛出物间隔生成是的定时操作
void ThrowEngine::engineThrowObject(float dt)
{
    if (m_nCurThrowIndex >= m_throwVec.size())
    {
        m_nCurThrowIndex = 0;
        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(ThrowEngine::engineThrowObject), this);
        return;
    }
    this->throwObject(m_nCurThrowIndex);
    m_nCurThrowIndex++;
}

// 抛出配置表中的第index个object
void ThrowEngine::throwObject(int index)
{
    BattleMapUnits* pMapUnitLayer = (BattleMapUnits*)GameManager::sharedInstance()->getBattleMapLayer();
    if (index < m_throwVec.size() && m_pTargetUnit != NULL && pMapUnitLayer != NULL)
    {
        ThrowAttackConfig_t* pConf = GameData::getThrowAttackConf(m_throwVec[index]);
        if (pConf->nFixDirection == 1)
        {
            if (index == 0)
            {
                m_nDir = m_pTargetUnit->isFlipX() ? 1 : 2;
            }
        }
        else
        {
            m_nDir = 0;
        }
        pMapUnitLayer->addThrowSprite(m_pAttackInfo, m_pSkillConfID, &m_unitBaseConf, pConf, m_pTargetUnit,pConf->nFixDirection == 1?m_nDir:0,pConf->nFixPoi ==1?m_Point:m_pTargetUnit->getPosition());
    }
}