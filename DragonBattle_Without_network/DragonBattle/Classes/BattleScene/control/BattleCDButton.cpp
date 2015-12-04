//
//  BattleCDButton.cpp
//  DragonBattle
//
//  Created by 何 永翔 on 13-5-28.
//
//

#include "BattleCDButton.h"
#include "ResourcesManager.h"

USING_NS_CC;

BattleCDButton::BattleCDButton()
{
    m_pTarget = NULL;
    m_pTouchBegan = NULL;
    m_pTouchEnded = NULL;
    
    m_pButtonSprite = NULL;
    m_pProgressSprite = NULL;
    m_pNomalFile = NULL;
    m_pSelectedFile = NULL;
    m_isEnabled = true;
    m_nProgressTag = 10;
}

BattleCDButton::~BattleCDButton()
{
    
}


bool BattleCDButton::initWithFile(const char* pNormalFile, const char* pSelFile, float fCD, CCObject* target, SEL_Callback touchBegan, SEL_Callback touchEnded)
{
    if (!pNormalFile || !pSelFile)
    {
        CCLOG("BattleCDButton::initWithFile: file should not null!");
        return false;
    }
    
    m_pNomalFile = pNormalFile;
    m_pSelectedFile = pSelFile;
    
    m_fCD = fCD;
    
    m_pButtonSprite = CCSprite::create(m_pNomalFile);
    this->addChild(m_pButtonSprite);
    m_pProgressSprite = CCProgressTimer::create(CCSprite::create(m_pSelectedFile));
    this->addChild(m_pProgressSprite);
    m_pProgressSprite->setType(kCCProgressTimerTypeRadial);
    
    m_pTarget = target;
    m_pTouchBegan = touchBegan;
    m_pTouchEnded = touchEnded;
    
    this->setTouchEnabled(true);
    
    return true;
}

BattleCDButton* BattleCDButton::create(const char* pFile, float fCD, CCObject* target, SEL_Callback touchBegan, SEL_Callback touchEnded)
{
    return BattleCDButton::create(pFile, pFile, fCD, target, touchBegan, touchEnded);
}

BattleCDButton* BattleCDButton::create(const char* pNormalFile, const char* pSelFile, float fCD, CCObject* target, SEL_Callback touchBegan, SEL_Callback touchEnded)
{
    BattleCDButton* pRet = new BattleCDButton();
    if (pRet && pRet->initWithFile(pNormalFile, pSelFile, fCD, target, touchBegan, touchEnded))
    {
        pRet->autorelease();
        return pRet;
    }
    
    CC_SAFE_DELETE(pRet);
    return NULL;
}

BattleCDButton* BattleCDButton::create(uint32_t normalID, float fCD, CCObject* target, SEL_Callback touchBegan, SEL_Callback touchEnded)
{
    return BattleCDButton::create(normalID, normalID, fCD, target, touchBegan, touchEnded);
}

BattleCDButton* BattleCDButton::create(uint32_t normalID, uint32_t selectedID, float fCD, CCObject* target, SEL_Callback touchBegan, SEL_Callback touchEnded)
{
    const char* pNormalFile = ResourcesManager::sharedInstance()->getUIResourceWithID(normalID)->strName.data();
    const char* pSelFile = ResourcesManager::sharedInstance()->getUIResourceWithID(selectedID)->strName.data();
    return BattleCDButton::create(pNormalFile, pSelFile, fCD, target, touchBegan, touchEnded);
}

void BattleCDButton::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -getTag(), true);
}

void BattleCDButton::showProgressSprite()
{
    CCProgressTo* to = CCProgressTo::create(m_fCD, 100);
    to->setTag(m_nProgressTag);
    m_pProgressSprite->runAction(to);
}

bool BattleCDButton::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
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
        this->touchBegan();
        return true;
    }
    return false;
}

void BattleCDButton::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
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
        this->touchEnded();
    }
}

void BattleCDButton::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    this->touchEnded();
}

void BattleCDButton::touchBegan()
{
    CCAction* action = m_pProgressSprite->getActionByTag(m_nProgressTag);
    if (action == NULL || action->isDone())
    {
        if(m_pTarget && m_pTouchBegan)
        {
            if ((m_pTarget->*m_pTouchBegan)())
            {
                this->showProgressSprite();
            }
        }
    }
}

void BattleCDButton::touchEnded()
{
    if(m_pTarget && m_pTouchEnded)
    {
        (m_pTarget->*m_pTouchEnded)();
    }
}
