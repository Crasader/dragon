//
//  PartnerSprite.h
//  DragonBattle
//
//  Created by 何 永翔 on 13-7-2.
//
//

#ifndef DragonBattle_PartnerSprite_h
#define DragonBattle_PartnerSprite_h

#include "cocos2d.h"
#include "BattleUnitSprite.h"

class PartnerSprite : public BattleUnitSprite
{
public:
    static PartnerSprite* create(UnitBaseConf *info,int lv);
    
public:
    PartnerSprite();
    virtual ~PartnerSprite();
    
    virtual bool init(UnitBaseConf *info,int lv);
    // 计算每一帧单位移动的距离, 每一帧只计算一次，在runAIOnce中计算
    virtual void update();
    virtual void onDie(bool* pIsToRight);
    virtual void onBeHit(bool bIsToRight,AttackInfo_t* pAttack);
    
    // 执行一次AI
    virtual void runAIOnce(BattleUnitSprite* pHero, BattleUnitSprite* pMonster);
    //virtual void throwObject();
private:
    void runAIAction(BattleUnitSprite* pHero, BattleUnitSprite* pMonster, kAIState nAIState, int nRunFrames);
    void resetBeHitCount(float dt);
    bool skillAttack(int index,bool bFlipx);
    void attackBegin();
private:
    int         m_nSkillLV;
    int         m_nLastNodeID;
    kAIState    m_nLastCalAI;       //上一次计算的AI
    kAIState    m_nLastAI;          //上一次执行成功的AI
    int         m_nRunFrames;       //上一次计算的RunFrames
    bool        m_bLastAIRunSucc;   //上次AI执行成功
    int         m_nBeHitCount;
    int         m_nAIRunFrames;     //每次AI执行的帧数量
};

#endif
