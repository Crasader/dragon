//
//  ThirdLayer.cpp
//  DragonBattle
//
//  Created by zeng xiaolong on 13-6-17.
//
//

#include "ThirdLayer.h"

ThirdLayer::ThirdLayer()
{
    m_nPriority = 0;
}

ThirdLayer::~ThirdLayer()
{
}

void ThirdLayer::setPrority(int priorty)
{
    setTouchEnabled(true);
    m_nPriority = priorty;
    setTouchPriority(m_nPriority);
}

int ThirdLayer::getPrority()
{
    return m_nPriority;
}

void ThirdLayer::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, m_nPriority, true);
}

bool ThirdLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}