//
//  XYLinkLabel.cpp
//  DragonBattle
//
//  Created by 何 永翔 on 14-1-21.
//
//

#include "XYLinkLabel.h"

using namespace cocos2d;


XYLinkLabel::XYLinkLabel()
{
    
}
XYLinkLabel::~XYLinkLabel()
{
    
}

XYLinkLabel* XYLinkLabel::create(const char *string, const char *fontName, float fontSize)
{
    XYLinkLabel *pRet = new XYLinkLabel();
    if(pRet && pRet->initWithString(string, fontName, fontSize, CCSizeZero, kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool XYLinkLabel::initWithString(const char *string, const char *fontName, float fontSize,
                                const cocos2d::CCSize &dimensions, CCTextAlignment hAlignment,
                                CCVerticalTextAlignment vAlignment)
{
    return CCLabelTTF::initWithString(string, fontName, fontSize, dimensions, hAlignment, vAlignment);
}

void XYLinkLabel::draw()
{
    CCLabelTTF::draw();
    
    ccDrawColor4B(_displayedColor.r,_displayedColor.g,_displayedColor.b,_displayedOpacity);
    ccPointSize(2);
    
    ccDrawLine(ccp(0,0), ccp(this->getContentSize().width, 0));
}