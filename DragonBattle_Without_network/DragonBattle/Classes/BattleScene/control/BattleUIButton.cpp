//
//  BattleUIButton.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-18.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "BattleUIButton.h"
#include "ResourcesManager.h"

USING_NS_CC;

BattleUIButton::BattleUIButton()
{
    m_pTarget = NULL;
    m_pTouchBegan = NULL;
    m_pTouchEnded = NULL;

    m_pButtonSprite = NULL;
    m_pNomalFile = NULL;
    m_pSelectedFile = NULL;
    m_isEnabled = true;
}

BattleUIButton::~BattleUIButton()
{
    
}


bool BattleUIButton::initWithFile(const char* pNormalFile, const char* pSelFile, CCObject* target, cocos2d::SEL_CallFunc touchBegan, cocos2d::SEL_CallFunc touchEnded)
{
    if (!pNormalFile || !pSelFile)
    {
        CCLOG("BattleUIButton::initWithFile: file should not null!");
        return false;
    }
    
    m_pNomalFile = pNormalFile;
    m_pSelectedFile = pSelFile;

    m_pButtonSprite = CCSprite::create(m_pNomalFile);
    this->addChild(m_pButtonSprite);
    m_pTarget = target;
    m_pTouchBegan = touchBegan;
    m_pTouchEnded = touchEnded;
    
    this->setTouchEnabled(true);
    
    return true;
}

BattleUIButton* BattleUIButton::create(const char* pFile, CCObject* target, cocos2d::SEL_CallFunc touchBegan, cocos2d::SEL_CallFunc touchEnded)
{
    return BattleUIButton::create(pFile, pFile, target, touchBegan, touchEnded);
}

BattleUIButton* BattleUIButton::create(const char* pNormalFile, const char* pSelFile, CCObject* target, cocos2d::SEL_CallFunc touchBegan, cocos2d::SEL_CallFunc touchEnded)
{
    BattleUIButton* pRet = new BattleUIButton();
    if (pRet && pRet->initWithFile(pNormalFile, pSelFile, target, touchBegan, touchEnded))
    {
        pRet->autorelease();
        return pRet;
    }
    
    CC_SAFE_DELETE(pRet);
    return NULL;
}

BattleUIButton* BattleUIButton::create(uint32_t normalID, CCObject* target, cocos2d::SEL_CallFunc touchBegan, cocos2d::SEL_CallFunc touchEnded)
{
    return BattleUIButton::create(normalID, normalID, target, touchBegan, touchEnded);
}

BattleUIButton* BattleUIButton::create(uint32_t normalID, uint32_t selectedID, CCObject* target, cocos2d::SEL_CallFunc touchBegan, cocos2d::SEL_CallFunc touchEnded)
{
    const char* pNormalFile = ResourcesManager::sharedInstance()->getUIResourceWithID(normalID)->strName.data();
    const char* pSelFile = ResourcesManager::sharedInstance()->getUIResourceWithID(selectedID)->strName.data();
    return BattleUIButton::create(pNormalFile, pSelFile, target, touchBegan, touchEnded);
}

void BattleUIButton::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -getTag(), true);
}

void BattleUIButton::changeButtonSprite(bool bSel)
{
    CCSpriteFrame *pFrame = CCSpriteFrame::create(bSel ? m_pSelectedFile : m_pNomalFile, m_pButtonSprite->getTextureRect());
    m_pButtonSprite->initWithSpriteFrame(pFrame);
}

bool BattleUIButton::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!m_isEnabled)
    {
        return false;
    }
    
    CCPoint touchLocation = pTouch->getLocationInView();
    touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
    
    CCPoint local = m_pButtonSprite->convertToNodeSpace(touchLocation);
    CCRect r = ((CCMenuItem*)m_pButtonSprite)->rect();
    r.origin = CCPointZero;
    
    if (r.containsPoint(local))
    {
        this->changeButtonSprite(true);
        this->touchBegan();
        return true;
    }
    return false;
}

void BattleUIButton::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!m_isEnabled) 
    {
        return;
    }
    
    CCPoint touchLocation = pTouch->getLocationInView();
    touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
    
    CCPoint local = m_pButtonSprite->convertToNodeSpace(touchLocation);
    CCRect r = ((CCMenuItem*)m_pButtonSprite)->rect();
    r.origin = CCPointZero;
    if (!r.containsPoint(local))
    {
        this->changeButtonSprite(false);
        this->touchEnded();
    }
}

void BattleUIButton::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    this->changeButtonSprite(false);
    this->touchEnded();
}

void BattleUIButton::touchBegan()
{
    if(m_pTarget && m_pTouchBegan)
    {
        (m_pTarget->*m_pTouchBegan)();
    }
}

void BattleUIButton::touchEnded()
{
    if(m_pTarget && m_pTouchEnded)
    {
        (m_pTarget->*m_pTouchEnded)();
    }
}