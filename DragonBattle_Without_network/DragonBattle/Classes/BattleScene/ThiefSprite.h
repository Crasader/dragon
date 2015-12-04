//
//  ThiefSprite.h
//  DragonBattle
//
//  Created by 何 永翔 on 13-11-25.
//
//

#ifndef __DragonBattle__ThiefSprite__
#define __DragonBattle__ThiefSprite__

#include "cocos2d.h"
#include "BattleUnitSprite.h"

class ThiefSprite : public BattleUnitSprite
{
public:
    static ThiefSprite* create(UnitBaseConf *conf,MonsterUnitConf *info, item_unit_t* pUnit);
    
public:
    ThiefSprite();
    virtual ~ThiefSprite();
    
    virtual bool init(UnitBaseConf *conf,MonsterUnitConf *info, item_unit_t* pUnit);
    // 计算每一帧单位移动的距离, 每一帧只计算一次，在runAIOnce中计算
    virtual void update();
    virtual void onBeHit(bool bIsToRight);
    virtual void onDie(bool* pIsToRight);
    
    // 执行一次AI
    virtual void runAIOnce(BattleUnitSprite* pHero);
    
    inline item_unit_t* getDropItem() { return &m_dropItem; }
    
    CC_SYNTHESIZE(bool, m_ReLife, reLife);//判断随从怪是否可再生
    
protected:
    virtual int correctDamage(AttackInfo_t* pAttack, float fHurt);
    
private:
    void runAIAction(BattleUnitSprite* pHero, kAIState nAIState, int nRunFrames);
    CC_SYNTHESIZE(MonsterUnitConf*, m_MonsterConf, MonsterConf);
private:
    int         m_nLastNodeID;
    kAIState    m_nLastCalAI;       //上一次计算的AI
    kAIState    m_nLastAI;          //上一次执行成功的AI
    int         m_nRunFrames;       //上一次计算的RunFrames
    bool        m_bLastAIRunSucc;   //上次AI执行成功
    
    int         m_nAIRunFrames;     //每次AI执行的帧数量
    
    item_unit_t m_dropItem;
};

#endif
