//
//  XYLinkLabel.h
//  DragonBattle
//
//  Created by 何 永翔 on 14-1-21.
//
//

#ifndef __DragonBattle__XYLinkLabel__
#define __DragonBattle__XYLinkLabel__

#include "cocos2d.h"

class XYLinkLabel : public cocos2d::CCLabelTTF
{
public:
    XYLinkLabel();
    virtual ~XYLinkLabel();
    
    static XYLinkLabel * create(const char *string, const char *fontName, float fontSize);
    bool initWithString(const char *string, const char *fontName, float fontSize,
                        const cocos2d::CCSize& dimensions, cocos2d::CCTextAlignment hAlignment,
                        cocos2d::CCVerticalTextAlignment vAlignment);
    
    void draw();
};

#endif /* defined(__DragonBattle__XYLinkLabel__) */
