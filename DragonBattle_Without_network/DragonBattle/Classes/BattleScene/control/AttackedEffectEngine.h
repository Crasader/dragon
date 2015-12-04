//
//  AttackedEffectEngine.h
//  DragonBattle
//
//  Created by 小敦 王 on 13-5-16.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_ScheduleTimesEngine_h
#define DragonBattle_ScheduleTimesEngine_h

#include "cocos2d.h"
#include "Resources.h"
#include "SkillConf.h"

class ScheduleEngine : public cocos2d::CCNode
{
public:
    ScheduleEngine();
    ~ScheduleEngine();
    virtual void startWork(float delayPerUnit);
    
protected:
    virtual bool scheduleUpdate(float dt);
    
protected:
    int     m_nCurIndex;
    int     m_nRunTimes;
};


class BattleUnitSprite;
class AttackedEffectEngine : public ScheduleEngine
{
public:
    AttackedEffectEngine();
    ~AttackedEffectEngine();
    
    static AttackedEffectEngine* create(BattleUnitSprite* unit, AttackInfo_t* pAttack, int pSkillConfIDLV, int runTimes, float fHurt);
    virtual bool init(BattleUnitSprite* unit, AttackInfo_t* pAttack, int pSkillConfIDLV, int runTimes, float fHurt);
    
protected:
    virtual bool scheduleUpdate(float dt);
    
private:
    //sprite死亡后，会有问题，不能这样用
    BattleUnitSprite*       m_pTargetUnit;
    AttackInfo_t*           m_pAttack;
    int            m_pSkillConfIDLV;
    float   m_fHurt;
};

#endif
