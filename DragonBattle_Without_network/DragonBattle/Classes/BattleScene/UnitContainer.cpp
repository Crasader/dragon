//
//  UnitContainer.cpp
//  DragonBattle
//
//  Created by zeng xiaolong on 14-2-15.
//
//

#include "UnitContainer.h"
#include "BattleMapUnits.h"
#include "GlobalDefine.h"
#include "BattleLogic.h"
#include "GeometryUtility.h"
#include "CommonUtility.h"
#include <algorithm>
using namespace std;
USING_NS_CC;

UnitContainer::UnitContainer()
{
    m_AllUnitVec.clear();
}

UnitContainer::~UnitContainer()
{
    
}

bool UnitContainer::init()
{
    m_nBornUnitSum = 0;
    return true;
}

void UnitContainer::addUnit(BattleUnitSprite* pUnit)
{
    if (pUnit == NULL)
    {
        return ;
    }
    
    m_nBornUnitSum++;    //总量增加
    m_AllUnitVec.push_back(pUnit);
}

bool UnitContainer::removeUnit(BattleUnitSprite*pMonster)
{
    if (pMonster == NULL)
    {
        return false;
    }
    bool bFind = false;
    bFind = this->removeMonster(m_AllUnitVec, pMonster);
    return bFind;
}

bool UnitContainer::removeMonster(BattleMonsterVec& unitVec, BattleUnitSprite* pUnit)
{
    BattleMonsterVec::iterator it = find(unitVec.begin(), unitVec.end(), pUnit);
    if (it != unitVec.end())
    {
        unitVec.erase(it);
        return true;
    }
    else
    {
        return false;
    }
    
}

// 受到攻击
void UnitContainer::underAttack(BattleUnitSprite* pAttackUnit, AttackInfo_t* pAttack, int* pCombo, long* pComboTime)
{
    if (!pAttackUnit)
    {
        return;
    }
    this->unitUnderAttack(pAttackUnit, pAttack, pCombo, pComboTime);
}

void UnitContainer::unitUnderAttack(BattleUnitSprite* pAttackUnit, AttackInfo_t* pAttack,int* pCombo, long* pComboTime)
{
    // foreach unit in monster vector
    BattleMonsterVec::iterator it;
    for (it = m_AllUnitVec.begin(); it != m_AllUnitVec.end();)
    {
        BattleUnitSprite* pMonster = *it;
        if (!pMonster)
        {
            ++it;
            continue;
        }
        
        if (pMonster->getHp() > 0)
        {
            pAttackUnit->attack(pMonster, pAttack);
        }
        ++it;
    }
}

cocos2d::CCRect UnitContainer::underThrowAttack(ThrowSprite* ts, bool isRangeAttack)
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

CCRect UnitContainer::unitUnderThrowAttack(ThrowSprite* ts, bool isRangeAttack)
{
    // foreach unit in monster vector
    BattleMonsterVec::iterator it;
    for (it = m_AllUnitVec.begin(); it != m_AllUnitVec.end(); )
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
            if (pMonster->getHp() > 0)
            {
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

BattleUnitSprite*  UnitContainer::getMonsterByIndex(int index)
{
    if (index < m_AllUnitVec.size())
    {
        return m_AllUnitVec[index];
    }
    return NULL;
}