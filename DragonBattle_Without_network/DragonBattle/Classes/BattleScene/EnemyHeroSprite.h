//
//  EnemyHeroSprite.h
//  DragonBattle
//
//  Created by zeng xiaolong on 13-10-26.
//
//

#ifndef __DragonBattle__EnemyHeroSprite__
#define __DragonBattle__EnemyHeroSprite__

#include "cocos2d.h"
#include "BattleUnitSprite.h"

class EnemyHeroSprite: public BattleUnitSprite
{
public:
    static EnemyHeroSprite* create(UnitBaseConf* pUnitConf,uint32_t* skill);
    virtual bool init(UnitBaseConf* pUnitConf,uint32_t* kill);
public:
    EnemyHeroSprite();
    virtual ~EnemyHeroSprite();
    
    void attackBegin();
    // 计算每一帧单位移动的距离, 每一帧只计算一次，在runAIOnce中计算
    virtual void update();
    void statueAttackBegin(bool isToRight);
    void statueAttackValid();
    virtual bool aiSkillAttack(bool bFlipx);
    void resetStandby();
    // 执行一次AI
    virtual void runAIOnce(BattleUnitSprite* pHero);
private:
    void runAIAction(BattleUnitSprite* pHero, kAIState nAIState, int nRunFrames);
private:
    int         m_nLastNodeID;
    kAIState    m_nLastAI;          //上一次执行成功的AI
    bool        m_bLastAIRunSucc;   //上次AI执行成功
    int         m_nAIRunFrames;     //每次AI执行的帧数量
    int         m_nContinueSkill;   //技能连续释放次数
    uint32_t    m_nSkill[4];
};

#endif
