//
//  XYEditBox.cpp
//  DragonBattle
//
//  Created by zeng xiaolong on 13-6-21.
//
//

#include "XYEditBox.h"

XYEditBox* XYEditBox::create(const cocos2d::CCSize &size, cocos2d::extension::CCScale9Sprite *pNormal9SpriteBg, CCScale9Sprite* pPressed9SpriteBg, CCScale9Sprite* pDisabled9SpriteBg)
{
    XYEditBox* pRet = new XYEditBox();
    
    if (pRet != NULL && pRet->initWithSizeAndBackgroundSprite(size, pNormal9SpriteBg))
    {
        if (pPressed9SpriteBg != NULL)
        {
            pRet->setBackgroundSpriteForState(pPressed9SpriteBg, CCControlStateHighlighted);
        }
        
        if (pDisabled9SpriteBg != NULL)
        {
            pRet->setBackgroundSpriteForState(pDisabled9SpriteBg, CCControlStateDisabled);
        }
        pRet->setFont("Arial",24);
        pRet->setAnchorPoint(ccp(0.5, 0.5));
        pRet->setPlaceholderFontColor(ccGRAY);
        pRet->setInputMode(kEditBoxInputModeSingleLine);
        pRet->setReturnType(kKeyboardReturnTypeDone);
        pRet->setFontColor(ccWHITE);
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}


XYEditBox::XYEditBox(){
    
}
XYEditBox::~XYEditBox(){
    
}