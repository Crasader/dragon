//
//  CellButton.cpp
//  DragonBattle
//
//  Created by 何 永翔 on 13-5-29.
//
//

#include "CellButton.h"
#include "ResourcesManager.h"

USING_NS_CC;

CellButton::CellButton()
{
    m_pTarget = NULL;
    m_pTouchBegan = NULL;
    m_pTouchEnded = NULL;
    
    m_pButtonSprite = NULL;
    m_pNomalFile = NULL;
    m_isEnabled = true;
    m_nItemID = 0;
}

CellButton::~CellButton()
{
    
}


bool CellButton::initWithFile(const char* pNormalFile, CCNode* target, SEL_CallFuncND touchBegan, SEL_CallFuncND touchEnded)
{
    if (!pNormalFile)
    {
        CCLOG("CellButton::initWithFile: file should not null!");
        return false;
    }
    
    m_pNomalFile = pNormalFile;
    
    m_pButtonSprite = CCSprite::create(m_pNomalFile);
    this->addChild(m_pButtonSprite);
    
    m_pTarget = target;
    m_pTouchBegan = touchBegan;
    m_pTouchEnded = touchEnded;
    
    this->setTouchEnabled(true);
    
    return true;
}

bool CellButton::appendItem(uint32_t nItemID)
{
    if (m_nItemID != 0)
    {
        CCLog("The cell button is not empty.");
        return false;
    }
    else
    {
        this->swapItem(nItemID);
        return true;
    }
}

uint32_t CellButton::removeItem()
{
    return this->swapItem(0);
}

uint32_t CellButton::swapItem(uint32_t nItemID)
{
    if (m_nItemID != nItemID)
    {
        if (nItemID == 0)
        {
            m_pButtonSprite->initWithFile(m_pNomalFile);
        }
        else
        {
            char fileName[64] = {0};
            sprintf(fileName, "item_%d.png", nItemID);
            //m_pButtonSprite->initWithFile(fileName);
            
            CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage(fileName);
            CCRect rect = CCRect(0, 0, pTexture->getContentSize().width, pTexture->getContentSize().height);
            CCSpriteFrame* pSpriteFrame = CCSpriteFrame::createWithTexture(pTexture, rect);
            m_pButtonSprite->setDisplayFrame(pSpriteFrame);
        }
        uint32_t tmp = m_nItemID;
        m_nItemID = nItemID;
        return tmp;
    }
    else
    {
        return nItemID;
    }
}

CellButton* CellButton::create(const char* pNormalFile, CCNode* target, SEL_CallFuncND touchBegan, SEL_CallFuncND touchEnded)
{
    CellButton* pRet = new CellButton();
    if (pRet && pRet->initWithFile(pNormalFile, target, touchBegan, touchEnded))
    {
        pRet->autorelease();
        return pRet;
    }
    
    CC_SAFE_DELETE(pRet);
    return NULL;
}

CellButton* CellButton::create(uint32_t normalID, CCNode* target, SEL_CallFuncND touchBegan, SEL_CallFuncND touchEnded)
{
    const char* pNormalFile = ResourcesManager::sharedInstance()->getUIResourceWithID(normalID)->strName.data();
    return CellButton::create(pNormalFile, target, touchBegan, touchEnded);
}

void CellButton::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -getTag(), true);
}

bool CellButton::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
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

void CellButton::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
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

void CellButton::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    this->touchEnded();
}

void CellButton::touchBegan()
{
    if(m_pTarget && m_pTouchBegan)
    {
        (m_pTarget->*m_pTouchBegan)(this, &m_nItemID);
        CCLog("%d", m_nItemID);
    }
}

void CellButton::touchEnded()
{
    if(m_pTarget && m_pTouchEnded)
    {
        (m_pTarget->*m_pTouchEnded)(this, &m_nItemID);
    }
}
