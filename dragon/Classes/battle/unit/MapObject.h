//
//  MapObject.h
//  dragon
//
//  Created by xingchong on 2/25/16.
//
//

#ifndef __dragon__MapObject__
#define __dragon__MapObject__

#include <stdio.h>
#include "cocos2d.h"
#include "Global.H"

USING_NS_CC;

class MapObject : public Node
{
public:
    MapObject();
    ~MapObject();
    
    virtual bool init();
    virtual void destroy();
    virtual void update(float dt);
    virtual void setPauseTime(float pauseTime);
    
private:
    int      _id;
    float    _defaultY;
    Vec2     _curAnchorPoint;
    Vec2     _curPosition;
    Rect     _collideRect;
    Vec2     _movePt;
    Vec2     _moveAddPt;
    float    _pauseTime;
    
    Global::CollideDirection _collideDirection;
    
};

#endif /* defined(__dragon__MapObject__) */
