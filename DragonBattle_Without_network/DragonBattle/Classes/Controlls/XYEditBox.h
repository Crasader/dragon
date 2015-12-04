//
//  XYEditBox.h
//  DragonBattle
//
//  Created by zeng xiaolong on 13-6-21.
//
//

#ifndef __DragonBattle__XYEditBox__
#define __DragonBattle__XYEditBox__
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC_EXT;
USING_NS_CC;

class XYEditBox:public cocos2d::extension::CCEditBox
{
public:
    XYEditBox(void);
    ~XYEditBox(void);
    static XYEditBox* create(const CCSize& size, CCScale9Sprite* pNormal9SpriteBg, CCScale9Sprite* pPressed9SpriteBg = NULL, CCScale9Sprite* pDisabled9SpriteBg = NULL);    

};


#endif /* defined(__DragonBattle__XYEditBox__) */
