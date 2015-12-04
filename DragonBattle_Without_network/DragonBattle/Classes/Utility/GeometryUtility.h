//
//  GeometryUtility.h
//  DragonBattle
//
//  Created by 永翔 何 on 13-5-15.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_GeometryUtility_h
#define DragonBattle_GeometryUtility_h

#include "cocos2d.h"

class GeometryUtility
{
public:
    static cocos2d::CCRect GetIntersectRect(const cocos2d::CCRect& rectA, const cocos2d::CCRect& rectB);
};

#endif
