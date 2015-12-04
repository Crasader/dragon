//
//  XYScaleButton.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-9-2.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "XYScaleButton.h"
#include "GlobalDefine.h"

enum
{
    kZoomActionTag = 50001,
};
#define CDIMAGE_TAG 22
#define DEFAULT_SCALE_VALUE 1.1

XYScaleButton::XYScaleButton()
{
    m_fScaleValue = DEFAULT_SCALE_VALUE;
    m_fCD = 0.f;
    m_fTimerCounter = 0.f;
    m_pProgressSprite = NULL;
    m_pLblTimerCounter = NULL;
    m_bWaitCD = false;
}

XYScaleButton::~XYScaleButton()
{
    m_pLblTimerCounter = NULL;
}

XYScaleButton* XYScaleButton::create(const char* pNormalFile, int nPriority)
{
    XYScaleButton* pRet = new XYScaleButton();
    if (pRet && pRet->initWithFile(pNormalFile, pNormalFile, NULL, nPriority))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return pRet;
}

XYScaleButton* XYScaleButton::createWithFileAndPriority(const char* pNormalFile, const char* pSelectFile, const char* pDisableFile, int nPriority)
{
    XYScaleButton* pRet = new XYScaleButton();
    if (pRet && pRet->initWithFile(pNormalFile, pSelectFile, pDisableFile, nPriority))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return pRet;
}

bool XYScaleButton::ccTouchBegan(CCTouch* pTouch, CCEvent* event)
{
    if (m_bWaitCD) {
        return false;
    }
    
    if (XYBaseTouchSprite::ccTouchBegan(pTouch,event))
    {
        if (m_pTarget != NULL && m_pTouchEnded != NULL)
        {
            CCAction *zoomAction = CCScaleTo::create(0.05f, m_fScaleValue);
            zoomAction->setTag(kZoomActionTag);
            this->runAction(zoomAction);
        }
        return true;
    }
    return false;
}

void XYScaleButton::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    XYBaseTouchSprite::ccTouchMoved(pTouch, pEvent);
    if (m_isEnabled)
    {
        CCPoint touchLocation = pTouch->getLocationInView();
        touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
        
        CCPoint local = this->convertToNodeSpace(touchLocation);
        CCRect r = m_TouchRect;
        r.setRect(0,0,r.size.width,r.size.height);
        
        if (!r.containsPoint(local))
        {
            if (m_pNormalFileFrame)
            {
                this->setDisplayFrame(m_pNormalFileFrame);
                CCAction *action = getActionByTag(kZoomActionTag);
                if (action)
                {
                    stopAction(action);        
                }
                this->setScale(1);
            }
            unscheduleAllSelectors();
        }
    }
}

void XYScaleButton::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_bWaitCD)
    {
        return;
    }
    
    if (m_pNormalFileFrame && m_isEnabled)
    {
        this->setDisplayFrame(m_pNormalFileFrame);
        
        CCAction *action = getActionByTag(kZoomActionTag);
        if (action)
        {
            stopAction(action);        
        }
        this->setScale(1);
    }
    unscheduleAllSelectors();
    XYBaseTouchSprite::ccTouchEnded(pTouch,pEvent);
}

void XYScaleButton::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_isEnabled)
    {
        if (m_pNormalFileFrame)
        {
            this->setDisplayFrame(m_pNormalFileFrame);
            
            CCAction *action = getActionByTag(kZoomActionTag);
            if (action)
            {
                stopAction(action);        
            }
            this->setScale(1);
        }
    }
}

void XYScaleButton::touchBegin()
{
    if (m_bWaitCD) {
        return;
    }
    
    CCAction* action = NULL;
    if (m_pProgressSprite)
        action = m_pProgressSprite->getActionByTag(10);
    
    if (action == NULL || action->isDone())
    {
        XYButton::touchBegin();
    }
}

void XYScaleButton::setEnabled(bool isEnabled)
{
    m_isEnabled = isEnabled;
    if (m_isEnabled)
    {
        setDisplayFrame(m_pNormalFileFrame);
        CCAction *action = getActionByTag(kZoomActionTag);
        if (action)
        {
            stopAction(action);        
        }
        this->setScale(1);
    }
    else
    {
        if(m_pDisableFileFrame)
        {
            setDisplayFrame(m_pDisableFileFrame);
        }
        else
        {
            CCSpriteFrame* pDisableFileFrame = CRUtils::getgaySpriteFrame(m_strPic.c_str());
            setDisplayFrame(pDisableFileFrame);
        }
    }
}

void XYScaleButton::setScaleValue(float fScale)
{
    m_fScaleValue = fScale;
}

void XYScaleButton::setCD(float var, bool showTime/* = false*/)
{
    m_fCD = var;
    if (getChildByTag(CDIMAGE_TAG))
    {
        m_pProgressSprite = CCProgressTimer::create((CCSprite*)getChildByTag(CDIMAGE_TAG));
        this->addChild(m_pProgressSprite, 5);
        m_pProgressSprite->setPosition(ccp(this->getContentSize().width / 2, this->getContentSize().height / 2));
        m_pProgressSprite->setType(kCCProgressTimerTypeRadial);
        m_pProgressSprite->setReverseDirection(true);
        
        if (showTime)
        {
            m_pLblTimerCounter = CCLabelAtlas::create("1", "CCBSportsResource/lblfightnum.png", 25, 32, '0');
            this->addChild(m_pLblTimerCounter, 6);
            m_pLblTimerCounter->setAnchorPoint(ccp(0.5f, 0.5f));
            m_pLblTimerCounter->setPosition(ccp(this->getContentSize().width / 2, this->getContentSize().height / 2));
            m_pLblTimerCounter->setVisible(false);
        }
    }
    else
    {
        m_pProgressSprite = NULL;
        m_pLblTimerCounter = NULL;
    }
    
}

float XYScaleButton::getCD()
{
    return m_fCD;
}

void XYScaleButton::updateTimer(float dt)
{
    char str[64] = {0};
    sprintf(str, "%d", (int)m_fTimerCounter);
    
    if (m_pLblTimerCounter)
        m_pLblTimerCounter->setString(str);

    
    if (m_fTimerCounter == 0)
    {
        unschedule(schedule_selector(XYScaleButton::updateTimer));
    }
    
    m_fTimerCounter --;
}

void XYScaleButton::showProgressSprite()
{
    if (m_fCD != 0.f)
    {
        m_pProgressSprite->setVisible(true);
        m_bWaitCD = true;

        CCProgressFromTo* to = CCProgressFromTo::create(m_fCD,100.0f,0.0f);
        to->setTag(10);
        m_pProgressSprite->runAction(CCSequence::create(to, CCCallFunc::create(this, callfunc_selector(XYScaleButton::removeCDImg)), NULL));
        
        if (m_pLblTimerCounter)
        {
            m_fTimerCounter = m_fCD;
            
            m_pLblTimerCounter->setVisible(true);
            this->updateTimer(0);
            
            schedule(schedule_selector(XYScaleButton::updateTimer), 1.f);
        }
    }
}

void XYScaleButton::removeCDImg()
{
    this->runAction(CCSequence::create(CCScaleTo::create(0.2, 1.1),CCScaleTo::create(0.2, 1.0f),NULL));
    this->removeChildByTag(201);
    m_pProgressSprite->setVisible(false);
    m_bWaitCD = false;
    
    if (m_pLblTimerCounter)
    {
        m_pLblTimerCounter->setVisible(false);
        unschedule(schedule_selector(XYScaleButton::updateTimer));
    }
    
}

XYBattleButton * XYBattleButton::create(const char * pNormalFile, int nPriority)
{
    XYBattleButton* pRet = new XYBattleButton();
    if (pRet && pRet->initWithFile(pNormalFile, pNormalFile, NULL, nPriority))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return pRet;

}

XYBattleButton * XYBattleButton::createWithFileAndPriority(const char * pNormalFile, const char *pSelectFile, const char *pDisableFile, int nPriority)
{
    XYBattleButton* pRet = new XYBattleButton();
    if (pRet && pRet->initWithFile(pNormalFile, pSelectFile, pDisableFile, nPriority))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return pRet;
}

void XYBattleButton::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_isEnabled)
    {
        CCPoint touchLocation = pTouch->getLocationInView();
        touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
        
        CCPoint local = this->convertToNodeSpace(touchLocation);
        CCRect r = m_TouchRect;
        r.setRect(0,0,r.size.width,r.size.height);
        
        if (!r.containsPoint(local))
        {
            if (m_pNormalFileFrame)
            {
                this->setDisplayFrame(m_pNormalFileFrame);
                CCAction *action = getActionByTag(kZoomActionTag);
                if (action)
                {
                    stopAction(action);
                }
                this->setScale(1);
            }
            touchEnd();
            unscheduleAllSelectors();
        }
    }
}

void XYBattleButton::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_pNormalFileFrame && m_isEnabled)
    {
        this->setDisplayFrame(m_pNormalFileFrame);
        
        CCAction *action = getActionByTag(kZoomActionTag);
        if (action)
        {
            stopAction(action);
        }
        this->setScale(1);
    }
    unscheduleAllSelectors();
    touchEnd();
}

void XYBattleButton::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    XYScaleButton::ccTouchCancelled(pTouch,pEvent);
    this->setScale(1);
    touchEnd();
}

XYRollButton * XYRollButton::createWithFileAndPriority(const char * pNormalFile, const char *pSelectFile, const char *pDisableFile, int nPriority)
{
    XYRollButton* pRet = new XYRollButton();
    if (pRet && pRet->initWithFile(pNormalFile, pSelectFile, pDisableFile, nPriority))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return pRet;
}

bool XYRollButton::ccTouchBegan(CCTouch* pTouch, CCEvent* event)
{
    for (CCNode *c = this; c != NULL; c = c->getParent())
    {
        if (c->isVisible() == false)
        {
            return false;
        }
    }
    m_isMove =false;
    CCPoint touchLocation = pTouch->getLocation();
    m_startPoi = touchLocation;
    touchLocation = this->convertToNodeSpace(touchLocation);
    CCRect r = m_TouchRect;
    if (r.containsPoint(touchLocation))
    {
//        CCLayerColor *layer = CCLayerColor::create(ccc4(0, 0, 0, 128), m_TouchRect.size.width, m_TouchRect.size.height);
//        layer->setPosition(ccp(m_TouchRect.origin.x, m_TouchRect.origin.y));
//        addChild(layer);
        touchBegin();
        if (m_pTarget != NULL && m_pTouchEnded != NULL)
        {
            CCAction *zoomAction = CCScaleTo::create(0.05f, m_fScaleValue);
            zoomAction->setTag(kZoomActionTag);
            this->runAction(zoomAction);
        }
        return true;
    }
    return false;
}
void XYRollButton::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCAction *action = getActionByTag(kZoomActionTag);
    if (action)
    {
        stopAction(action);
    }
    this->setScale(1);
}

void XYRollButton::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    CCAction *action = getActionByTag(kZoomActionTag);
    if (action)
    {
        stopAction(action);
    }
    this->setScale(1);
}

void XYRollButton::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{

}

void XYRollButton::touchBegin()
{
    if (m_bWaitCD)
    {
        return;
    }
    XYBaseTouchSprite::touchBegin();
}
