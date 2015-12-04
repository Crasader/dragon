//
//  XYLblButton.cpp
//  DragonBattle
//
//  Created by zeng xiaolong on 13-8-27.
//
//

#include "XYLblButton.h"
#include "AudioResource.h"
#define LABEL_TAG 100

XYLblButton::XYLblButton()
{
    m_hasLine = false;
}

XYLblButton::~XYLblButton()
{

}

XYLblButton * XYLblButton::create(const char *string, const char *fontName, float fontSize,int nPriority)
{
    XYLblButton *pBtn = new XYLblButton();
	if (pBtn && pBtn->initWithLbl(string,fontName,fontSize,nPriority))
    {
	    pBtn->autorelease();
        return pBtn;
    }
    CC_SAFE_DELETE(pBtn);
	return NULL;
}

bool XYLblButton::initWithLbl(const char *string, const char *fontName, float fontSize,int nPriority)
{
    m_iPriority = nPriority;
    if (CCSprite::init())
    {
        CCLabelTTF *lbl = CCLabelTTF::create(string, fontName, fontSize);
        lbl->setAnchorPoint(CCPointZero);
        lbl->setTag(LABEL_TAG);
        addChild(lbl);
        this->setTouchRect(CCRect(-10, -10, lbl->getContentSize().width+20, lbl->getContentSize().height+20));
        this->setContentSize(lbl->getContentSize());
        return true;
    }
    else
    {
        return false;
    }
}

void XYLblButton::setColor(ccColor3B color)
{
    if (getChildByTag(LABEL_TAG))
    {
        CCLabelTTF* lbl = (CCLabelTTF*)getChildByTag(LABEL_TAG);
        lbl->setColor(color);
    }
}

void XYLblButton::setString(const char* str)
{
    if (getChildByTag(LABEL_TAG))
    {
        CCLabelTTF* lbl = (CCLabelTTF*)getChildByTag(LABEL_TAG);
        lbl->setString(str);
        this->setTouchRect(CCRect(-10, -10, lbl->getContentSize().width+20, lbl->getContentSize().height+20));
    }
}

void XYLblButton::setLine(bool hasline)
{
    m_hasLine = hasline;
}

void XYLblButton::draw()
{
    CCLabelTTF* lbl = (CCLabelTTF*)getChildByTag(LABEL_TAG);
    if (m_hasLine && lbl)
    {
        CHECK_GL_ERROR_DEBUG();
        glLineWidth(3.0f);
        ccColor3B color = lbl->getColor();
        ccDrawColor4B(color.r, color.g, color.b, 255);
        ccDrawLine(ccp(lbl->getPositionX(),0), ccp(lbl->getContentSize().width,0));
        CHECK_GL_ERROR_DEBUG();
    }
    XYBaseTouchSprite::draw();
}

void XYLblButton::setFontName(const char* fontname)
{
    if (getChildByTag(LABEL_TAG))
    {
        CCLabelTTF* lbl = (CCLabelTTF*)getChildByTag(LABEL_TAG);
        lbl->setFontName(fontname);
    }
}

void XYLblButton::setFontSize(float fontSize)
{
    if (getChildByTag(LABEL_TAG))
    {
        CCLabelTTF* lbl = (CCLabelTTF*)getChildByTag(LABEL_TAG);
        lbl->setFontSize(fontSize);
    }
}

void XYLblButton::touchBegin()
{
    if(m_pTarget && m_pTouchBegan)
    {
        AudioResource::shareAudioResource()->playEffect("music/click.mp3");
        
        XYBaseTouchSprite::touchBegin();
    }
}
void XYLblButton::touchEnd()
{
    if(m_pTarget && m_pTouchEnded)
    {
        AudioResource::shareAudioResource()->playEffect("music/click.mp3");
        
        XYBaseTouchSprite::touchEnd();
    }
}