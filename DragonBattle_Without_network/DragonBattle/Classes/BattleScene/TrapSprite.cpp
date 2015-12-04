//
//  TrapSprite.cpp
//  DragonBattle
//
//  Created by 何 永翔 on 13-6-4.
//
//

#include "TrapSprite.h"
#include "GameData.h"
#include "BattleLogic.h"
#include "GameManager.h"
#include "BattleMapUnits.h"

USING_NS_CC;

TrapSprite::TrapSprite()
{
    m_bAIActivated = false;
    
    m_bJudgeCollide = false;
}

TrapSprite::~TrapSprite()
{
    this->unscheduleAllSelectors();
}

TrapSprite* TrapSprite::create(TrapUnit *info)
{
    TrapSprite* pRet = new TrapSprite();
    if (pRet && pRet->init(info))
    {
        pRet->autorelease();
        return pRet;
    }
    
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool TrapSprite::init(TrapUnit *info)
{
    if (!BattleUnitSprite::init(info))
    {
        CCLOG("TrapSprite::init error!");
        return false;
    }
    m_TrapUnit = info;
    m_nLV = info->getLV();
    m_nHp = info->getMaxHP();
    
    this->setPosition(info->pos);
    if (!info->size.equals(CCSizeZero))
    {
        this->setContentSize(info->size);
        m_UnitSprite->setPosition(ccp(getContentSize().width/2, getContentSize().height/2));
    }
    m_fAngryTime = 999999;//宝箱、陷阱没有硬直
    if (m_unitConf.nUnitType == kFire)
    {
        this->schedule(schedule_selector(TrapSprite::scheduleAttack), info->fAttackInterval);
    }

    m_bJudgeCollide = info->nJudgeCollide == 1;
    return true;
}

void TrapSprite::onBeHit(bool bIsToRight)
{
    m_fSpeedX = 0;
    m_fSpeedY = 0;
    m_fAddSpeedX = 0;
    if (getHp() <= 1 && m_unitConf.nUnitType == kBombBox)//木桶爆炸
    {
        m_nCurAction = kStandby;
        attackBegin();
    }
    else
    {
        this->changeAction(kBeHit);
    }
}

void TrapSprite::attackValidEnd()
{
    if (m_unitConf.nUnitType == kBombBox)//木桶爆炸
    {
        m_nCurAction = kStandby;
        setHp(0);
        changeUnderAttackStatue(true);
    }
    else
    {
        BattleUnitSprite::attackValidEnd();
    }
}

void TrapSprite::scheduleAttack(float dt)
{
    this->attackBegin();
}

int TrapSprite::correctDamage(AttackInfo_t* pAttack, float fHurt)
{
    if (m_unitConf.nUnitType == kBox)
    {
        //如果是命运宝箱，受到伤害时，无论攻击者是谁，固定受到1点伤害。
        return 1;
    }
    else if (m_unitConf.nUnitType == kFire)
    {
        //如果是火焰陷阱，不受到任何伤害。
        return 0;
    }
    else if (m_unitConf.nUnitType == kBombBox)
    {
        //爆炸木桶
        return 1;
    }
    else
    {
        CCAssert(false, "TrapSprite's type is unknown.");
    }
    return 0;
}

// 怪物的坐标点 只用判断Y坐标是否错误
void TrapSprite::update()
{
    BattleUnitSprite::update();
}


