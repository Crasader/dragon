//
//  XYSelectButton.cpp
//  XY
//
//  Created by long on 12-12-21.
//
//

#include "XYSelectButton.h"
#include "CRUtils.h"
#include "AudioResource.h"
#define SELEIMAGE_TAG 23
#define SELEANI_TAG 24
XYSelectButton::XYSelectButton()
{
    m_isTurnOn = false;
    m_isMove = false;
    m_bAnimate = false;
}
XYSelectButton::~XYSelectButton()
{
}
XYSelectButton *XYSelectButton::create(const char *pNormalFile, const char *pSelectFile, int nPriority,bool isChangePic)
{
    XYSelectButton *pBtn = new XYSelectButton();
    if (pBtn && pBtn->initWithFile(pNormalFile,pSelectFile,NULL,nPriority,isChangePic))
    {
        pBtn->autorelease();
        return pBtn;
    }
    CC_SAFE_DELETE(pBtn);
	return NULL;
}

XYSelectButton* XYSelectButton::create(const char *pNormalFile, const char *pSelectFile,const char *pDisableFile, int nPriority,bool isChangePic)
{
    XYSelectButton *pBtn = new XYSelectButton();
    if (pBtn && pBtn->initWithFile(pNormalFile,pSelectFile,pDisableFile,nPriority,isChangePic))
    {
        pBtn->autorelease();
        return pBtn;
    }
    CC_SAFE_DELETE(pBtn);
	return NULL;
}

bool XYSelectButton::initWithFile(const char * pNormalFile, const char *pSelectFile,const char *pDisableFile, int nPriority,bool isChangePic)
{
    XYBaseButton::initWithFile(pNormalFile,pSelectFile,pDisableFile,nPriority);
    m_bIsChangePic = isChangePic;
    m_poi = ccp(getContentSize().width/2,getContentSize().height/2);
    return true;
}

bool XYSelectButton::ccTouchBegan(CCTouch* pTouch, CCEvent* event)
{
    return XYBaseTouchSprite::ccTouchBegan(pTouch,event);
}

void XYSelectButton::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    return XYBaseTouchSprite::ccTouchMoved(pTouch,pEvent);
}

void XYSelectButton::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_isEnabled)
    {
        CCPoint touchLocation = pTouch->getLocationInView();
        touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
        
        CCPoint local = this->convertToNodeSpace(touchLocation);
        CCRect r = m_TouchRect;
        if (r.containsPoint(local)&&!m_isMove)
        {
            if(m_pTarget && m_pTouchEnded)
            {
                if (m_sound)
                {
                    AudioResource::shareAudioResource()->playEffect("music/click.mp3");
                }
            }
            touchEnd();
        }
    }
    m_isMove = false;
}

void XYSelectButton::touchEnd()
{
    if (!m_isTurnOn)
    {
        m_isTurnOn = true;
        XYBaseTouchSprite::touchEnd();
    }
    selet();
}

void XYSelectButton::selet()
{
    m_isTurnOn = true;
    if(m_bIsChangePic)
    {
        setDisplayFrame(m_pSelectFileFrame);
    }
    else
    {
        if(!getChildByTag(SELEIMAGE_TAG))
        {
            CCSprite*Sprselect = CCSprite::createWithSpriteFrame(m_pSelectFileFrame);
            Sprselect->setTag(SELEIMAGE_TAG);
            Sprselect->setPosition(m_poi);
            addChild(Sprselect,1);
            
            if (m_bAnimate)
            {
                CCSprite*SprAin = CCSprite::createWithSpriteFrame(m_pSelectFileFrame);
                SprAin->setTag(SELEANI_TAG);
                SprAin->setPosition(m_poi);
                SprAin->runAction(CCRepeatForever::create(CCSequence::create(
                    CCSpawn::create(CCScaleTo::create(1.f, 1.15f),CCFadeTo::create(1.f, 0),NULL),
                    CCSpawn::create(CCScaleTo::create(0.01f, 1.0f),CCFadeTo::create(0.01f, 255),NULL),
                                NULL)));
                addChild(SprAin,1);
            }
        }
    }
}

void XYSelectButton::unselet()
{
    m_isTurnOn = false;
    if(m_bIsChangePic)
    {
        setDisplayFrame(m_pNormalFileFrame);
    }
    else
    {
        if (getChildByTag(SELEIMAGE_TAG))
        {
            getChildByTag(SELEIMAGE_TAG)->removeFromParent();
        }
        if (getChildByTag(SELEANI_TAG))
        {
            getChildByTag(SELEANI_TAG)->removeFromParent();
        }
    }
}

XYSpecialSelectButton::XYSpecialSelectButton()
{
    m_isTurnOn = false;
    m_isMove = false;
    m_bAnimate = false;
}
XYSpecialSelectButton::~XYSpecialSelectButton()
{
    
}

XYSpecialSelectButton* XYSpecialSelectButton::create(const char *pNormalFile, const char *pSelectFile, int nPriority,bool isChangePic)
{
    XYSpecialSelectButton *pBtn = new XYSpecialSelectButton();
    if (pBtn && pBtn->initWithFile(pNormalFile,pSelectFile,NULL,nPriority,isChangePic))
    {
        pBtn->autorelease();
        return pBtn;
    }
    CC_SAFE_DELETE(pBtn);
	return NULL;
}

void XYSpecialSelectButton::touchEnd()
{
    selet();
    XYBaseButton::touchEnd();
}