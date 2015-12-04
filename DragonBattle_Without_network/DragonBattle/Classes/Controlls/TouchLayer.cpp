//
//  TouchLayer.cpp
//  DragonBattle
//
//  Created by zeng xiaolong on 13-6-7.
//
//

#include "TouchLayer.h"
#include "GameManager.h"
#include "TimerManager.h"

TouchLayer::TouchLayer()
{
    m_nPriority = 0;
}

TouchLayer::~TouchLayer()
{
    
}

void TouchLayer::setPrority(int priorty)
{
    setTouchEnabled(true);
    m_nPriority = priorty;
    setTouchPriority(m_nPriority);
}

int TouchLayer::getPrority()
{
    return m_nPriority;
}

void TouchLayer::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, m_nPriority, true);
}

bool TouchLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void TouchLayer::keyBackClicked()
{
    CCLayer::keyBackClicked();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //CCLayer::keyBackClicked();
    CCLOG("apple.....keyBackClicked");
#endif
}

void TouchLayer::onEnter()
{
    CCLayer::onEnter();
}

void TouchLayer::onExit()
{
    
    CCLayer::onExit();
}

void TouchLayer::removeFromParent()
{
    {
        CCLayer::removeFromParent();
    }
}
