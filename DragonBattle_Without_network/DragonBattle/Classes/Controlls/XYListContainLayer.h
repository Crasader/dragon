//
//  XYListContainLayer.h
//  DragonBattle
//
//  Created by zeng xiaolong on 13-7-2.
//
//

#ifndef __DragonBattle__XYListContainLayer__
#define __DragonBattle__XYListContainLayer__
#include "XYContainLayer.h"

class XYListContainLayer:public XYContainLayer
{
public:
    XYListContainLayer(void);
    ~XYListContainLayer(void);
    static XYListContainLayer*create(int Priority);
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
};

#endif /* defined(__DragonBattle__XYListContainLayer__) */
