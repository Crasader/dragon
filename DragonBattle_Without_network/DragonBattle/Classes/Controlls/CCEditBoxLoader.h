//
//  CCEditBoxLoader.h
//  DragonBattle
//
//  Created by 白明江 on 1/22/14.
//
//

#ifndef __DragonBattle__CCEditBoxLoader__
#define __DragonBattle__CCEditBoxLoader__

#include "cocos2d.h"
#include "cocos-ext.h"


USING_NS_CC;
USING_NS_CC_EXT;

class CCEditBoxLoader : public CCControlButtonLoader
{
public:
    virtual ~CCEditBoxLoader() {};
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CCEditBoxLoader, loader);
    
protected:
    virtual CCEditBox *createCCNode(cocos2d::CCNode* pParent, cocos2d::extension::CCBReader * pCCBReader)
    {
        CCEditBox* editBox = new CCEditBox();
        editBox->setZoomOnTouchDown(false);
        editBox->autorelease();
        editBox->initWithSizeAndBackgroundSprite(CCSize(280, 41), CCScale9Sprite::create("public/onePixels.png"));
        return editBox;
    }
};

#endif /* defined(__DragonBattle__CCEditBoxLoader__) */
