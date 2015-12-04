//
//  ScheduleAction.h
//  DragonBattle
//
//  Created by 小敦 王 on 13-8-31.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_ScheduleAction_h
#define DragonBattle_ScheduleAction_h

#include "cocos2d.h"
USING_NS_CC;

class ScheduleBase : public cocos2d::CCNode
{
public:
    ScheduleBase();
    ~ScheduleBase();
    virtual void startWork(float delayPerUnit);
    
protected:
    virtual bool scheduleUpdate(float dt);
    
protected:
    int     m_nCurIndex;
    int     m_nRunTimes;
};

class TipsEffect : public ScheduleBase
{
public:
    TipsEffect();
    ~TipsEffect();
    
    static TipsEffect* create();
    virtual bool init();
    
    virtual void startWork(float delayPerUnit);
    
    //必须得先添加完了之后才能startWork, 颜色还没有用到
    void addTips(const char* msg, const ccColor3B& clr);

protected:
    virtual bool scheduleUpdate(float dt);

private:
    cocos2d::CCArray*    m_pArray;
};

#endif
