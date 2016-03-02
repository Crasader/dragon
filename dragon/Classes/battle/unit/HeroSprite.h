//
//  HeroSprite.h
//  dragon
//
//  Created by xingchong on 3/2/16.
//
//

#ifndef __dragon__HeroSprite__
#define __dragon__HeroSprite__

#include <stdio.h>
#include "BattleUnitSprite.h"

class HeroSprite : public BattleUnitSprite
{
public:
    HeroSprite();
    ~HeroSprite();
    
public:
    virtual bool init() override;
    virtual void initData(int guid, int configId) override;
    virtual void bornWithPos(Vec2 point) override;
    virtual void resetStandBy() override;
    virtual void update(float dt) override;
    virtual void updateMove(float dt) override;
    virtual void releaseSkill(int skillId) override;
    virtual void attackOver() override;
    virtual bool isBeHit() override;
    virtual void operateRun(int degree) override;
    virtual void operateStandBy() override;
    virtual void operateAttack(int skillid) override;
    virtual void operateAttackOver(int skillId) override;
    
    virtual bool isAttack(BattleUnitSprite *beHitUnit);
    virtual void attack(BattleUnitSprite *beHitUnit);
    virtual void underAttack(BattleUnitSprite *beHitUnit);
    
private:
    ValueMap _dataDic;
    int      _TouchAttackValue;
};

#endif /* defined(__dragon__HeroSprite__) */
