//
//  MapSprite.h
//  dragon
//
//  Created by xingchong on 2/26/16.
//
//

#ifndef __dragon__MapSprite__
#define __dragon__MapSprite__

#include <stdio.h>
#include "MapObject.h"

class MapSprite : public MapObject
{
public:
    MapSprite();
    ~MapSprite();
    
public:
    virtual bool init() override;
    virtual void destroy() override;
    virtual void reset();
    
    void showRectColor();
    void playRepeatForever(int actionId);
    void playOnce(int actionId);
    
    
private:
    int        _spriteId;
    float      _actionLastTime;
    float      _actionWaitTime;
    ValueMap   _curAniConfig;
    Node       _actionAni;
    Node       _actionEffect;
    LayerColor _RectColor;
    
    std::function<void()> _actionTimeOverCallback;
    std::function<void()> _finishAniCallback;
    std::function<void()> _breakAniCallback;
    
    Global::CollideDirection    _collideDirection;
    
};

#endif /* defined(__dragon__MapSprite__) */
