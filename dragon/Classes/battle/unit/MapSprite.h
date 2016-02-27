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
    virtual void setActionMovePt(ValueMap param);
    virtual void setMoveSpeed(Vec2 movePt, Vec2 moveAddPt);
    virtual void playEffect();
    virtual void playBeHitEffect(int effectId);
    virtual void playEffectWithId(int effectId);
    virtual void update(float dt);
    virtual void setCollideRange(Size rect) override;
    
    void showRectColor();
    void playRepeatForever(int actionId);
    void playOnce(int actionId);
    void playAction(int actionId);
    void handlerActionData(ValueMap actionConfig);
    bool isPause();
    void psuseAction();
    void resumeAction();
    bool isToRight();
    void setToRight(bool toRight);
    bool getIsFlipX();
    void setFlipx(bool isFlip);
    bool isOnLand();
    void onFinishAniCallback(float actionId, std::string actionName);
    void onBreakAniCallback(ValueMap config);
    void onRemoveEffectAni();
    void resetAniCallback();
    
private:
    int        _spriteId;
    float      _actionLastTime;
    float      _actionWaitTime;
    ValueMap   _curActionConfig;
    Node       _actionAni;
    Node       _actionEffect;
    LayerColor _RectColor;
    
    std::function<void()> _actionTimeOverCallback;
    std::function<void()> _finishAniCallback;
    std::function<void()> _breakAniCallback;
    
    Global::CollideDirection    _collideDirection;
    
};

#endif /* defined(__dragon__MapSprite__) */
