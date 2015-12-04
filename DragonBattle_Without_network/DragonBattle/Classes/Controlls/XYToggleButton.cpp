//
//  XYToggleButton.cpp
//  ChickRun
//
//  Created by long on 12-11-16.
//
//

#include "XYToggleButton.h"
#include "CRUtils.h"
XYToggleButton::XYToggleButton()
{
    m_isTurnOn = false;
}

XYToggleButton::~XYToggleButton()
{
    
}

XYToggleButton *XYToggleButton::create(const char *pNormalFile,const char *pSelectFile, int nPriority)
{
    XYToggleButton *pBtn = new XYToggleButton();
    if (pBtn && pBtn->initWithFile(pNormalFile,pSelectFile,NULL,nPriority))
    {
        pBtn->m_poi = ccp(pBtn->getContentSize().width/2, pBtn->getContentSize().height/2);
        pBtn->autorelease();
        return pBtn;
    }
    CC_SAFE_DELETE(pBtn);
	return NULL;
}

bool XYToggleButton::ccTouchBegan(CCTouch* pTouch, CCEvent* event)
{
    return XYBaseTouchSprite::ccTouchBegan(pTouch,event);
}

void XYToggleButton::setselect(bool select)
{
    m_isTurnOn = select;
    
    const int tag = 2007;
    CCNode* pNode = this->getChildByTag(tag);
    CCSprite* pSprite = NULL;
    if (pNode != NULL)
    {
        pSprite = dynamic_cast<CCSprite*>(pNode);
    }
    else
    {
        pSprite = CCSprite::createWithSpriteFrame(m_pSelectFileFrame);
        this->addChild(pSprite,5);
        pSprite->setTag(tag);
    }
    pSprite->setPosition(m_poi);
    pSprite->setVisible(m_isTurnOn);
}

void XYToggleButton::touchEnd()
{
    setselect(!m_isTurnOn);
    XYBaseButton::touchEnd();
}