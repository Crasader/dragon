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

public:
    virtual bool init() override;
    virtual void update(float dt) override;
    
    virtual void updateMovePt(float dt);
    virtual void destroy();
    virtual void setPauseTime(float pauseTime);
    virtual bool isHasDelay(float dt);
    virtual void pauseAction();
    virtual void resumeAction();
    
    virtual void setLogicAnchorPoint(Vec2 point){_curAnchorPoint = point;};
    virtual void setLogicPosition(Vec2 point);
    virtual void adjuestPosition(Vec2 point);
    
    virtual void setCollideRange(Size size);
    virtual Size getCollideRange(){return _collideRect.size;};
    virtual Rect getCollideRect(){return _collideRect;};
    
    
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
