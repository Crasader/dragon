//
//  ArenaResultLayer.h
//  DragonBattle
//
//  Created by zeng xiaolong on 13-10-31.
//
//

#ifndef __DragonBattle__ArenaResultLayer__
#define __DragonBattle__ArenaResultLayer__
#include "XYMessageBox.h"
class ArenaResultLayer:public BaseMessageBox
{
public:
    static ArenaResultLayer* create(bool bWin,int nCopper,int nPre, CCObject* pTarget, cocos2d::SEL_CallFuncN pSelFunc1);
    bool init(bool bWin,int nCopper,int nPre, CCObject* pTarget, cocos2d::SEL_CallFuncN pSelFunc1);
};


#endif /* defined(__DragonBattle__ArenaResultLayer__) */