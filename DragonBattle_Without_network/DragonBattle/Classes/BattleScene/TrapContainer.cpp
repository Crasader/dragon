//
//  TrapContainer.cpp
//  DragonBattle
//
//  Created by 何 永翔 on 13-6-4.
//
//

#include "TrapContainer.h"
#include "BattleMapUnits.h"
#include <algorithm>

using namespace std;


TrapContainer::TrapContainer()
{
    m_trapSpriteVec.clear();
}

TrapContainer::~TrapContainer()
{
    m_trapSpriteVec.clear();
}

TrapSpriteVec *TrapContainer::getTrapSpriteVec()
{
    return &m_trapSpriteVec;
}
void TrapContainer::reset(BattleMapUnits* pBattleMapUnits, TrapUnitVec& trapUnitVec)
{
    this->clearAllTrap();
    for (int i = 0; i < trapUnitVec.size(); i++)
    {
        TrapUnit* tu = &trapUnitVec.at(i);
        TrapSprite* sprite = TrapSprite::create(tu);
        pBattleMapUnits->addChild(sprite, kBoxUnitOrder);
        m_trapSpriteVec.push_back(sprite);
    }
}

bool TrapContainer::remove(BattleUnitSprite* pTrap)
{
    TrapSpriteVec::iterator it = find(m_trapSpriteVec.begin(), m_trapSpriteVec.end(), pTrap);
    if (it != m_trapSpriteVec.end())
    {
        m_trapSpriteVec.erase(it);
        return true;
    }
    else
    {
        return false;
    }
}

void TrapContainer::update(BattleUnitSprite* pHero)
{
    if (pHero != NULL)
    {
        TrapSpriteVec::iterator itr = m_trapSpriteVec.begin();
        while (itr != m_trapSpriteVec.end())
        {
            TrapSprite* sprite = *itr;
            if (sprite != NULL)
            {
                sprite->update();
                //陷阱攻击主角
                if (sprite->getUnitType() == kFire)
                {
                    sprite->attack(pHero, sprite->getCurAttack());
                }
                if (sprite->getUnitType() == kBombBox)
                {
                    sprite->attack(pHero, sprite->getCurAttack());
                }
                //陷阱被主角攻击
                if (sprite->getUnitType() == kBox)
                {
                    pHero->attack(sprite, pHero->getCurAttack());
                }
                //陷阱被主角攻击
                if (sprite->getUnitType() == kBombBox)
                {
                    pHero->attack(sprite, pHero->getCurAttack());
                }
            }
            itr++;
        }
    }
}

bool TrapContainer::isInterSect(cocos2d::CCRect unitRect, float moveX, float moveY, float* pCorrectX, float* pCorrectY)
{
    CCPoint point = unitRect.origin;
    CCSize size = unitRect.size;
    
    CCRect rect = CCRectMake(point.x + moveX, point.y + moveY, size.width, size.height);
    
    TrapSpriteVec::iterator itr = m_trapSpriteVec.begin();
    while (itr != m_trapSpriteVec.end())
    {
        TrapSprite* sprite = *itr;
        if (sprite != NULL && sprite->judgeCollide() && sprite->getHp() > 0)
        {
            CCRect rect1 = sprite->getUnitRect();
            if (!rect1.equals(CCRectZero) && !rect1.equals(unitRect) && rect1.intersectsRect(rect))
            {
                if (moveX > 0)
                {
                    *pCorrectX = rect1.getMinX() - 0.0001;
                }
                else if (moveX < 0)
                {
                    *pCorrectX = rect1.getMaxX() + 0.0001;
                }
                if (moveY > 0)
                {
                    *pCorrectY = rect1.getMinY() - 0.0001;
                }
                else if (moveY < 0)
                {
                    *pCorrectY = rect1.getMaxY() + 0.0001;
                }
                return true;
            }
        }
        itr++;
    }
    return false;
}

TrapSprite* TrapContainer::getOverlappingSpr(float poix,CCRect unitRect)
{
    TrapSprite * spr = NULL;
    TrapSpriteVec::iterator itr = m_trapSpriteVec.begin();
    while (itr != m_trapSpriteVec.end())
    {
        TrapSprite* sprite = *itr;
        if (sprite != NULL && sprite->judgeCollide() && sprite->getHp() > 0)
        {
            if (!sprite->getUnitRect().equals(CCRectZero))
            {
                float maxTrapX = sprite->getPositionX() + sprite->getUnitRect().size.width/2;
                float minTrapX = sprite->getPositionX() - sprite->getUnitRect().size.width/2;
                float maxSprX = poix + unitRect.size.width/2;
                float minSPrX = poix - unitRect.size.width/2;
                if (maxSprX <= minTrapX || minSPrX >= maxTrapX )
                {
                    itr++;
                    continue;
                }
                else
                {
                    spr = sprite;
                    break;
                }
            }
        }
        itr++;
    }
    return spr;
}

void TrapContainer::clearAllTrap()
{
    for (int i = 0; i < m_trapSpriteVec.size(); ++i)
    {
        TrapSprite* pSprite = m_trapSpriteVec[i];
        if (pSprite)
        {
            pSprite->removeAllChildrenWithCleanup(true);
            pSprite->removeFromParentAndCleanup(true);
        }
        pSprite = NULL;
    }
    m_trapSpriteVec.clear();
}
