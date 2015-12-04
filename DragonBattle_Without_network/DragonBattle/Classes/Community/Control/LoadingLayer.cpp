//
//  LoadingLayer.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-5-7.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "LoadingLayer.h"
#include "GlobalDefine.h"

USING_NS_CC;


LoadingLayer* LoadingLayer::create(cocos2d::CCObject* pObject, cocos2d::SEL_CallFunc pTarFunc)
{
    LoadingLayer *pRet = new LoadingLayer();
    if (pRet && pRet->init())
    {
        pRet->setTargetFunction(pObject, pTarFunc);
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

// on "init" you need to initialize your instance
bool LoadingLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    this->createBkg();
    
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
    
    m_bLoadingFinished = false;
    m_pTarObject = NULL;
    m_pTarFunc = NULL;
    m_fLastDt = 0;
    return true;
}

void LoadingLayer::createBkg()
{
    CCLayerColor* pMaskLayer = CCLayerColor::create(ccc4(0, 0, 0, 255));
    this->addChild(pMaskLayer);
    
    CCSprite* pBgSprite = CCSprite::create("CCBCommunityResource/login_loading_bg.jpg");
    this->addChild(pBgSprite);
    
    float fScaleX = SCREEN_WIDTH / pBgSprite->getContentSize().width;
    float fScaleY = SCREEN_HEIGTH / pBgSprite->getContentSize().height;
    pBgSprite->setScale(fScaleX > fScaleY ? fScaleX : fScaleY);
    pBgSprite->setPosition(SCREEN_MID_POS);
    
    CCSprite* pSlot = CCSprite::create("CCBCommunityResource/progress_slot.png");
    this->addChild(pSlot);
    pSlot->setPosition(ccp(SCREEN_MID_POS.x, 70));
    
    m_pProgressSprite = CCProgressTimer::create(CCSprite::create("CCBCommunityResource/progress_bar.png"));
    this->addChild(m_pProgressSprite);
    m_pProgressSprite->setPosition(ccp(SCREEN_MID_POS.x, 70));
    m_pProgressSprite->setType(kCCProgressTimerTypeBar);
    m_pProgressSprite->setMidpoint(ccp(0,0));
    m_pProgressSprite->setBarChangeRate(ccp(1, 0));
    
    CCLabelTTF* pLoadingWord = CCLabelTTF::create(RAND_STR_LOADING_WORDS, "Arial-BoldMT", 22);
    pLoadingWord->setColor(ccWHITE);
    pLoadingWord->setPosition(ccp(SCREEN_MID_POS.x, 30));
    this->addChild(pLoadingWord);
}

void LoadingLayer::close()
{
    this->removeAllChildrenWithCleanup(true);
    this->removeFromParentAndCleanup(true);
}

void LoadingLayer::setTargetFunction(CCObject* pObj, cocos2d::SEL_CallFunc pTarFunc)
{
    m_pTarObject = pObj;
    m_pTarFunc = pTarFunc;
}

void LoadingLayer::setLoadingFinished()
{
    m_bLoadingFinished = true;
}

void LoadingLayer::runOffsetPercent(float offsetPercent)
{
    m_fPercent += offsetPercent;
    if (m_fPercent >= 100.0)
    {
        m_fPercent = 99.9;
    }
    m_pProgressSprite->setPercentage(m_fPercent);
}

void LoadingLayer::update(float dt)
{
    if (m_fPercent >= 80.0 && !m_bLoadingFinished)
    {
        return ;
    }
    
    if (m_fPercent >= 99.0)
    {
        return ;
    }
    
    m_fLastDt += dt;
    
    if (m_fLastDt > 0.1)
    {
        m_fLastDt = 0;
        runOffsetPercent(5);
        if (m_fPercent >= 99.0)
        {
            if (m_pTarObject && m_pTarFunc)
            {
                close();
                (m_pTarObject->*m_pTarFunc)();
            }
        }
    }
}