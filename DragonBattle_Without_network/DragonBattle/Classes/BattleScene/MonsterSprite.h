//
//  MonsterSprite.h
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-19.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_MonsterSprite_h
#define DragonBattle_MonsterSprite_h

#include "cocos2d.h"
#include "BattleUnitSprite.h"

class MonsterSprite : public BattleUnitSprite
{
public:
    static MonsterSprite* create(UnitBaseConf *conf,MonsterUnitConf *info);
    
public:
    MonsterSprite();
    virtual ~MonsterSprite();
    
    virtual bool init(UnitBaseConf *conf,MonsterUnitConf *info);
    void attackBegin();
    // 计算每一帧单位移动的距离, 每一帧只计算一次，在runAIOnce中计算
    virtual void update();
    virtual void onDie(bool* pIsToRight);
    
    virtual bool skillAttack(int index);
    
    // 执行一次AI
    virtual void runAIOnce(BattleUnitSprite* pHero);
    
    CC_SYNTHESIZE(bool, m_ReLife, reLife);//判断随从怪是否可再生
    
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
};

#endif
