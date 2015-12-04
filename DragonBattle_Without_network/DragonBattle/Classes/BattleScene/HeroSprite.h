//
//  HeroSprite.h
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-19.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_HeroSprite_h
#define DragonBattle_HeroSprite_h

#include "cocos2d.h"
#include "BattleUnitSprite.h"
class HeroSprite : public BattleUnitSprite
{
public:
    HeroSprite();
    ~HeroSprite();
    
protected:
    bool            m_bAttackBtnDown;   //连续攻击
public:
    static HeroSprite* create(UnitBaseConf* pUnitConf);
    virtual bool init(UnitBaseConf *pUnitConf);
    
    // 计算每一帧单位的改变, 每一帧只计算一次，在场景每帧更新时计算
    virtual void statueAttackValid();
    virtual void update();
    virtual void addcombo();//主角连续攻击
    virtual void onDie(bool* pIsToRight);
    virtual void onBeHit(bool bIsToRight,AttackInfo_t* pAttack);
    virtual bool skillAttack(int index);
    
    void   setAttackBtnDown(bool bValue) { m_bAttackBtnDown = bValue; }
    void   resetStandby();
    void   attackValidRole();
    void   unAttackValidRole();
    int    getMp();
    void   setMp(float mp);
    void   addMp(float add);
    void   addHp(int hp);
    void   SetComboInfo();
    virtual void runAIOnce(BattleUnitSprite* pMonster);
private:
    bool aiSkillAttack(bool bFlipx);
    void aiAttackBegin();
    void resetBeHitCount(float dt);
    void recoverHP(float dt);
    int m_nBeHitIndex;
    int m_nBeHitCount;
    float m_nMp;
    int m_MpTime;
    CC_SYNTHESIZE(float,m_nMaxMp,MaxMp);
    CC_SYNTHESIZE(bool, m_bBeHitProtect, BeHitProtect);//受伤3次保护

    CC_SYNTHESIZE(float,m_nMaxCombo,MaxCombo);
    CC_SYNTHESIZE(int,m_nCombo,Combo);
    CC_SYNTHESIZE(long,m_lComboTime,ComboTime);
    
    uint32_t    m_nSkill[4];
    uint32_t    m_nSkillCD[4];
private:
    void heroUpdate(float dt);
    void runAIAction(BattleUnitSprite* pMonster, kAIState nAIState, int nRunFrames);
    
    int         m_nLastNodeID;
    kAIState    m_nLastCalAI;       //上一次计算的AI
    kAIState    m_nLastAI;          //上一次执行成功的AI
    int         m_nRunFrames;       //上一次计算的RunFrames
    bool        m_bLastAIRunSucc;   //上次AI执行成功
    int         m_nAIRunFrames;     //每次AI执行的帧数量
};

#endif
