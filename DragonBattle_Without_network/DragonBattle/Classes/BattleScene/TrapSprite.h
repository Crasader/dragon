//
//  TrapSprite.h
//  DragonBattle
//
//  Created by 何 永翔 on 13-6-4.
//
//

#ifndef DragonBattle_TrapSprite_h
#define DragonBattle_TrapSprite_h

#include "cocos2d.h"
#include "BattleUnitSprite.h"

class TrapSprite : public BattleUnitSprite
{
public:
    static TrapSprite* create(TrapUnit *info);
    
public:
    TrapSprite();
    virtual ~TrapSprite();
    
    virtual bool init(TrapUnit *info);
    
    inline bool judgeCollide() { return m_bJudgeCollide; }
    
    
    // 计算每一帧单位移动的距离, 每一帧只计算一次，在runAIOnce中计算
    virtual void update();
    virtual void onBeHit(bool bIsToRight);
    virtual void updateShadow(BattleMapUnits* pMapUnitLayer) {}
    CC_SYNTHESIZE(TrapUnit*, m_TrapUnit, TrapUnit);
protected:
    virtual int correctDamage(AttackInfo_t* pAttack, float fHurt);
    
private:
    void scheduleAttack(float dt);
    void attackValidEnd();
    bool m_bJudgeCollide;
};

#endif
