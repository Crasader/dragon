//
//  BattleUnitSprite.h
//  dragon
//
//  Created by xingchong on 2/18/16.
//
//

#ifndef __dragon__BattleUnitSprite__
#define __dragon__BattleUnitSprite__

#include <stdio.h>
#include "MapSprite.h"

class BattleUnitSprite : public MapSprite
{
public:
    BattleUnitSprite();
    ~BattleUnitSprite();
    
public:
    virtual bool init() override;
    virtual void destroy() override;
    virtual void initData(int guid, int configId);
    virtual void initView();
    virtual void onFinishAniCallback(float actionId, std::string actionName) override;
    virtual void releaseSkill(int skillId);
    virtual void attackOver();
    virtual void resetStandBy();
    virtual void update(float dt);
    virtual void adjuestPosition(Vec2 point) override;
    virtual void updateMove(float dt);
    virtual void handlerActionData(ValueMap actionData);
    virtual void bornWithPos(Vec2 point);
    virtual bool isBeHit();
    virtual void operateRun(int degree);
    virtual void operateStandBy();
    virtual void operateAttack(int skillId);
    virtual void operateAttackOver(int skillId);
    
    void die();
    Global::RoleState getNextAction();
    void resetNextAction();
    bool autoChangeAction(Global::RoleState roleState);
    bool changeBehitAction();
    bool operateChangeAction(Global::RoleState roleState);
    Vec2 getBeHitPoint();
    void operateJump();
    
    
private:
    ValueMap _dataDic;
    std::function<void()> _onLandCallback;
    std::function<void()> _offLandCallback;
};

#endif /* defined(__dragon__BattleUnitSprite__) */
