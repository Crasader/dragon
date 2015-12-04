//
//  GeometryUtility.cpp
//  DragonBattle
//
//  Created by 永翔 何 on 13-5-15.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "GeometryUtility.h"

using namespace cocos2d;

CCRect GeometryUtility::GetIntersectRect(const CCRect& rectA, const CCRect& rectB)
{   
    float aMaxX = rectA.getMaxX();
    float aMinX = rectA.getMinX();
    float aMaxY = rectA.getMaxY();
    float aMinY = rectA.getMinY();
    float bMaxX = rectB.getMaxX();
    float bMinX = rectB.getMinX();
    float bMaxY = rectB.getMaxY();
    float bMinY = rectB.getMinY();
    
    float minX, maxX, minY, maxY;
    
    minX = aMinX >= bMinX ? aMinX : bMinX;
    maxX = aMaxX >= bMaxX ? bMaxX : aMaxX;
    minY = aMinY >= bMinY ? aMinY : bMinY;
    maxY = aMaxY >= bMaxY ? bMaxY : aMaxY;

    return CCRectMake(minX, minY, maxX - minX, maxY - minY);
}


