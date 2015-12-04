//
//  XYMenuItemLabel.cpp
//  DragonBattle
//
//  Created by 何 永翔 on 13-11-20.
//
//

#include "XYMenuItemLabel.h"

using namespace cocos2d;

XYMenuItemLabel::XYMenuItemLabel()
{
    m_hasLine = false;
}

XYMenuItemLabel* XYMenuItemLabel::create(CCNode* label, CCObject* target, SEL_MenuHandler selector)
{
    XYMenuItemLabel* pRet = new XYMenuItemLabel();
    
    if (pRet && pRet->initWithLabel(label, target, selector))
    {
        pRet->autorelease();
        return pRet;
    }
    
    CC_SAFE_DELETE(pRet);
    return NULL;
}

void XYMenuItemLabel::setLine(bool hasline)
{
    m_hasLine = hasline;
}

void XYMenuItemLabel::draw()
{
    if (strcmp(dynamic_cast<CCLabelProtocol*>(m_pLabel)->getString(), "") != 0 && m_hasLine)
    {
        CHECK_GL_ERROR_DEBUG();
        glLineWidth(3.0f);
        ccColor3B color = dynamic_cast<CCRGBAProtocol*>(m_pLabel)->getColor();
        ccDrawColor4B(color.r, color.g, color.b, 255);
        ccDrawLine(ccp(m_pLabel->getPositionX(),0), ccp(m_pLabel->getContentSize().width,0));
        CHECK_GL_ERROR_DEBUG();
    }
    CCMenuItemLabel::draw();
}