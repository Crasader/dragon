//
//  MonsterContainer.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-5-8.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "MonsterContainer.h"
#include "BattleMapUnits.h"
#include "GlobalDefine.h"
#include "BattleLogic.h"
#include "GeometryUtility.h"
#include "CommonUtility.h"
#include <algorithm>

using namespace std;
USING_NS_CC;

MonsterContainer::MonsterContainer()
{
    m_AllMonsterVec.clear();
}

MonsterContainer::~MonsterContainer()
{
    
}

bool MonsterContainer::init()
{
    m_nBornBossSum = 0;             //boss刷新的总量
    m_nBornFollowMonsterSum = 0;    //随从怪刷的总量
    m_nBornNormalMonsterSum = 0;
    m_nBornNeutralMonsterSum = 0;
    m_nBornEnemyHeroSum = 0;
    m_nBornMonsterSum = 0;          //刷怪总量
    m_nBossHurtHpValue = 0;
    return true;
}

void MonsterContainer::addMonster(BattleUnitSprite* pMonster)
{
    if (pMonster == NULL)
    {
        return ;
    }
    
    m_nBornMonsterSum++;    //总量增加
    m_AllMonsterVec.push_back(pMonster);
}

void MonsterContainer::setFollowMonsterRelife(bool brelife)
{
    BattleMonsterVec followMonsterVec = getMonsterVecByType(kFollowMonster);
    BattleMonsterVec::iterator it;
    for (it = followMonsterVec.begin(); it != followMonsterVec.end(); )
    {
        MonsterSprite* pMonster = (MonsterSprite*)*it;
        ++it;
        if (!pMonster)
        {
            continue;
        }
        else
        {
            pMonster->setreLife(false);
        }
    }
}

int  MonsterContainer::getMonsterCountByType(int monsterType)
{
    BattleMonsterVec monsterVec = getMonsterVecByType(monsterType);
    int count = monsterVec.size();
    return count;
}

bool MonsterContainer::removeMonster(BattleMonsterVec& monsterVec, BattleUnitSprite* pMonster)
{
    BattleMonsterVec::iterator it = find(monsterVec.begin(), monsterVec.end(), pMonster);
    if (it != monsterVec.end())
    {
        monsterVec.erase(it);
        return true;
    }
    else
    {
        return false;
    }
    
}

bool MonsterContainer::removeMonster(BattleUnitSprite* pMonster)
{
    if (pMonster == NULL)
    {
        return false;
    }
    if (pMonster->getUnitType() == kBoss)
    {
        m_nBossHurtHpValue += pMonster->getHurtHp();
    }
    
    bool bFind = false;
    bFind = this->removeMonster(m_AllMonsterVec, pMonster);
    return bFind;
}

int MonsterContainer::getBossHurHp()
{
    BattleMonsterVec monsterVec = getMonsterVecByType(kBoss);
    int hurt = m_nBossHurtHpValue;
    for (int i = 0; i < monsterVec.size(); i ++)
    {
        hurt += monsterVec.at(i)->getHurtHp();
    }
    return hurt;
}

CCRect MonsterContainer::underThrowAttack(ThrowSprite* ts, bool isRangeAttack)
{
    CCRect rect = this->unitUnderThrowAttack(ts, isRangeAttack);
    if (!rect.equals(CCRectZero) && !isRangeAttack)
    {
        return rect;
    }
    else
    {
        return CCRectZero;
    }
}

CCRect MonsterContainer::unitUnderThrowAttack(ThrowSprite* ts, bool isRangeAttack)
{
    // foreach unit in monster vector
    BattleMonsterVec::iterator it;
    for (it = m_AllMonsterVec.begin(); it != m_AllMonsterVec.end(); )
    {
        BattleUnitSprite* pMonster = *it;
        if (!pMonster)
        {
            ++it;
            continue;
        }
        
        // 攻击到了
        if (ts->getAttackRect().intersectsRect(pMonster->getUnitRect()))
        {
            //CCRect rect = ts->getAttackRect();
            //CCLog("width:%f", rect.size.width);
            if (pMonster->getHp() > 0)
            {
                //pMonster->underAttack(ts->isFlipX(), ts->getAttackInfo());
                if (pMonster->underOjbectAttack(ts))
                {
                    if (ts->judgeCollide())
                    {
                        pMonster->forceMoveBy(ts->getSpeed());
                    }
                    if (!isRangeAttack)
                    {
                        return GeometryUtility::GetIntersectRect(ts->getAttackRect(), pMonster->getUnitRect());
                    }
                }
            }
        }

        ++it;
    }
    return CCRectZero;
}

void MonsterContainer::SetComboInfo(int* pCombo, long* pComboTime)
{
    if (pCombo != NULL && pComboTime != NULL)
    {
        long t = CommonUtility::getCurrentMillisecond();
        if (t - *pComboTime > 2000)
        {
            *pCombo = 0;
        }
        *pComboTime = t;
        *pCombo += 1;
    }
}

// 所有怪被攻击
void MonsterContainer::underAttack(BattleUnitSprite* pHero, AttackInfo_t* pAttack)
{
    if (!pHero)
    {
        return;
    }
    this->unitUnderAttack(pHero, pAttack);
}

// 怪攻击对方
void MonsterContainer::attack(BattleUnitSprite* pAttackRole,BattleUnitSprite* pUnderAttackRole, AttackInfo_t* pAttack)
{
    pAttackRole->attack(pUnderAttackRole, pAttack);
}

BattleMonsterVec MonsterContainer::getMonsterVecByType(int monsterType)
{
    BattleMonsterVec vec;
    BattleMonsterVec::iterator it;
    for (it = m_AllMonsterVec.begin(); it != m_AllMonsterVec.end();)
    {
        BattleUnitSprite* pMonster = *it;
        if (!pMonster)
        {
            ++it;
            continue;
        }
        if (pMonster->getUnitType() == monsterType)
        {
            vec.push_back(pMonster);
        }
        ++it;
    }
    return vec;
}

void MonsterContainer::unitUnderAttack(BattleUnitSprite* pHero, AttackInfo_t* pAttack)
{
    // foreach unit in monster vector
    BattleMonsterVec::iterator it;
    for (it = m_AllMonsterVec.begin(); it != m_AllMonsterVec.end();)
    {
        BattleUnitSprite* pMonster = *it;
        if (!pMonster)
        {
            ++it;
            continue;
        }
        
        if (pMonster->getHp() > 0)
        {
            pHero->attack(pMonster, pAttack);
        }
        ++it;
    }
}

void MonsterContainer::runAIOnce(BattleUnitSprite* pHero)
{
    for (int i = 0; i < m_AllMonsterVec.size(); i++)
    {
        if (m_AllMonsterVec[i])
        {
            m_AllMonsterVec[i]->update();
            m_AllMonsterVec[i]->runAIOnce(pHero);
        }
    }
}

void MonsterContainer::setAttacRectShow(bool bShow)
{
    for (int i = 0; i < m_AllMonsterVec.size(); i++)
    {
        m_AllMonsterVec[i]->setAttacRectVisible(bShow);
    }
}

void MonsterContainer::setMonsterDeath()
{
    this->setMonsterDeath(kBoss);
    this->setMonsterDeath(kEnemyHero);
    this->setMonsterDeath(kNeutralMonster);
    this->setMonsterDeath(kNormalMonster);
    this->setMonsterDeath(kFollowMonster);
}

void MonsterContainer::setMonsterDeath(int monsterType)
{
    BattleMonsterVec monsterVec = getMonsterVecByType(monsterType);
    for (int i = 0; i < monsterVec.size(); i++)
    {
        BattleUnitSprite* pMonster = monsterVec.at(i);
        pMonster->setCurAction(kStandby);
        if (pMonster && BattleLogic::canChangeUnitAction(pMonster->getCurAction(), kBeHitFly))
        {
            //小怪死亡 被击飞状态
            pMonster->setUnderAttackIndex(3);
            pMonster->setHp(0);
            pMonster->changeAction(kBeHitFly);
        }
    }
}

void MonsterContainer::setMonsterBehitFly()
{
    this->setMonsterBehitFly(kBoss);
    this->setMonsterBehitFly(kEnemyHero);
    this->setMonsterBehitFly(kNeutralMonster);
    this->setMonsterBehitFly(kNormalMonster);
    this->setMonsterBehitFly(kFollowMonster);
}

void MonsterContainer::setMonsterBehitFly(int monsterType)
{
    BattleMonsterVec monsterVec = getMonsterVecByType(monsterType);

    for (int i = 0; i < monsterVec.size(); i++)
    {
        BattleUnitSprite* pMonster = monsterVec.at(i);
        if (pMonster && BattleLogic::canChangeUnitAction(pMonster->getCurAction(), kBeHitFly))
        {
            //小怪死亡 被击飞状态
            pMonster->setUnderAttackIndex(3);
            pMonster->changeAction(kBeHitFly);
        }
    }
}

void MonsterContainer::clearAllMonster()
{
    for (int i = 0; i < m_AllMonsterVec.size(); i++)
    {
        m_AllMonsterVec[i]->removeAllChildrenWithCleanup(true);
        m_AllMonsterVec[i]->removeFromParentAndCleanup(true);
        m_AllMonsterVec[i] = NULL;
    }
    m_AllMonsterVec.clear();
}

// 得到地图中当前非BOSS和随从怪物数量
int  MonsterContainer::getMissionMonster()
{
    BattleMonsterVec neutralMonsterVec = getMonsterVecByType(kNeutralMonster);
    BattleMonsterVec normalMonsterVec = getMonsterVecByType(kNormalMonster);
    return neutralMonsterVec.size() + normalMonsterVec.size();
}

int  MonsterContainer::getBornMonsterSum(int monsterType)
{
    switch (monsterType)
    {
        case kNormalMonster:
            return m_nBornNormalMonsterSum;
        case kNeutralMonster:
            return m_nBornNeutralMonsterSum;
        case kBoss:
            return m_nBornBossSum;
        case kFollowMonster:
            return m_nBornFollowMonsterSum;
        case kEnemyHero:
            return m_nBornEnemyHeroSum;
        default:
            return 0;
    }
}

int MonsterContainer::getKillMonsterSum(int monsterType)
{
    return getBornMonsterSum(monsterType) - getMonsterCountByType(monsterType);
}

int MonsterContainer::getContainerSize() const
{
    return m_AllMonsterVec.size();
}

BattleUnitSprite*  MonsterContainer::getMonsterByIndex(int index)
{
    if (index < m_AllMonsterVec.size())
    {
        return m_AllMonsterVec[index];
    }
    return NULL;
}