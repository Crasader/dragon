//
//  XYDownButton.cpp
//  DragonBattle
//
//  Created by zeng xiaolong on 13-7-22.
//
//

#include "XYDownButton.h"

XYDownButton::XYDownButton()
{
    m_duration = 0.5;
}
XYDownButton::~XYDownButton()
{}
XYDownButton * XYDownButton::create(const char *pNormalFile,const char *pSelectFile, int nPriority)
{
    XYDownButton *pBtn = new XYDownButton();
    if (pBtn && pBtn->initWithFile(pNormalFile,pSelectFile,NULL,nPriority))
    {
        pBtn->autorelease();
        return pBtn;
    }
    CC_SAFE_DELETE(pBtn);
	return NULL;

}

bool XYDownButton::ccTouchBegan(CCTouch* pTouch, CCEvent* event)
{
    return XYBaseTouchSprite::ccTouchBegan(pTouch,event);
}

void XYDownButton::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    XYBaseTouchSprite::ccTouchMoved(pTouch,pEvent);
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
            }
            unscheduleAllSelectors();
        }
    }
}

void XYDownButton::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_pNormalFileFrame&&m_isEnabled)
    {
        this->setDisplayFrame(m_pNormalFileFrame);
    }
    unscheduleAllSelectors();
}

void XYDownButton::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_isEnabled)
    {
        if (m_pNormalFileFrame)
        {
            this->setDisplayFrame(m_pNormalFileFrame);
        }
    }
}

void XYDownButton::touchBegin()
{
    if(m_pTarget && m_pTouchBegan)
    {
        XYBaseTouchSprite::touchBegin();
        if (m_pSelectFileFrame)
        {
            this->setDisplayFrame(m_pSelectFileFrame);
        }
        else
        {
            CCSpriteFrame* pSelectFileFrame = CRUtils::getlightSpriteFrame(m_strPic.c_str());
            this->setDisplayFrame(pSelectFileFrame);
        }
        schedule(schedule_selector(XYDownButton::touchDown), m_duration);
    }
    
}

void XYDownButton::touchDown(float ftime)
{
    unscheduleAllSelectors();
    XYBaseTouchSprite::touchBegin();
    m_duration = m_duration*0.7;
    if (m_duration<0.1)
    {
        m_duration = 0.1;
    }
    schedule(schedule_selector(XYDownButton::touchDown), m_duration);
}
