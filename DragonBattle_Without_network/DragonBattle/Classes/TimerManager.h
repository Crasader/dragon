//
//  TimerManager.h
//  DragonBattle
//
//  Created by 白明江 on 12/5/13.
//
//

#ifndef __DragonBattle__TimerManager__
#define __DragonBattle__TimerManager__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include <ctime>
#include "Lock.h"
#include "OnlineManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

typedef enum
{
    Time_Format_hh_mm_ss = 0x7,
    Time_Format_mm_ss = 0x3,
    Time_Format_ss = 0x1,
    
} kTime_Format;

class TimerNode : public CCObject
{
private:
    CC_SYNTHESIZE(CCNode*, m_pTimerParent, TimerParent);
    CC_SYNTHESIZE(CCLabelTTF*, m_pTimerLabel, TimerLabel);
    CC_SYNTHESIZE_RETAIN(CCString*, m_pTimerFormat, TimerFormat);
    CC_SYNTHESIZE(kTime_Format, m_eTimerFormatType, TimerFormatType);
    CC_SYNTHESIZE(int, m_iTimerCount, TimerCount);
    CC_SYNTHESIZE(bool, m_bRemoveSelf, RemoveSelf);
    
public:
    TimerNode()
    {
        m_pTimerParent = NULL;
        m_pTimerLabel = NULL;
        m_pTimerFormat = NULL;
        m_eTimerFormatType = Time_Format_hh_mm_ss;
        m_iTimerCount = -1;
        m_bRemoveSelf = false;
    }
    virtual ~TimerNode()
    {
        m_pTimerParent = NULL;
        m_pTimerLabel = NULL;
        CC_SAFE_RELEASE_NULL(m_pTimerFormat);
        m_iTimerCount = -1;
    }
    
    static TimerNode* create()
    {
        TimerNode* pRet = new TimerNode;
        pRet->autorelease();
        return pRet;
    }
};


class TimerDelegate
{
private:
    CC_SYNTHESIZE(CCNode*, m_pTimerParent, TimerParent);
    CC_SYNTHESIZE(bool,m_bRun,Run);
public:
    TimerDelegate() { m_pTimerParent = NULL;m_bRun = true; }
    virtual ~TimerDelegate() { m_pTimerParent = NULL; }
    
    virtual void updateTimer(float) {}
    virtual void overTimer(CCNode* pParent, CCLabelTTF* pTimerLabel) {}
    
};

class TimerManager : public CCNode
{
protected:
    CC_SYNTHESIZE(time_t, m_tOlineBonusTimer, OlineBonusTimer); //在线礼包
    CC_SYNTHESIZE(time_t, m_tLuckyTimer, LuckyTimer);           //幸运大转盘时间
    CC_SYNTHESIZE(int, m_iOnlineBonusID, OnlineBonusID);
    CC_SYNTHESIZE(int, m_iLuckyDialID, LuckyDialID);
    
    CC_PROPERTY(time_t, m_tFarmlandTimer, FarmlandTimer);
    CC_SYNTHESIZE(bool, m_bHasHarvest, HasHarvest);
    CC_SYNTHESIZE(time_t, m_tActivityTime, ActivityTime);
    
private:
    XYLock m_oLock;
    std::vector<TimerDelegate*> m_vecTimeDelegates;
    
    CCArray* m_pTimerNodeArr;
    
    time_t m_tLeaveTime;
    time_t m_tDeltaTime;
    
private:
    TimerManager();
    
    void update(float dt);
    void updateLuckyTimer(float dt);
    void updateOnlineBonusTimer(float dt);
    void updateCountDownTimer(float dt);
    
    void showTimeWithFormat(CCLabelTTF* lblTime, CCString* format, kTime_Format format_type, int counter);
    
    void noticeUI(CCNode* pParent, CCLabelTTF* pLabel);
    
public:
    virtual ~TimerManager();
    
    static TimerManager* getInstance();
    
    virtual void onEnter();
    virtual void onExit();
    
    
    
    void addTimerDelegate(TimerDelegate*, CCNode* pParent = NULL);
    void removeTimerDelegate(TimerDelegate*);
    
    void setCountDownLabel(CCNode* pParent, CCLabelTTF* pLabel, const char* format, int countDown);
    void setTimerNode(TimerNode* pNode);
    void removeCountDownLabel(CCNode* pParent);
    
    void recordLeaveTime();
    void calculateTime();
    
    void synchTime(cli_user_login_ex_out* out);
    void synchFarmlandTime();
    void synchFarmlandTimeCbk(CCObject* pObj);
    
public:
    long getOnlineTimerByID(int nid);
};

#endif /* defined(__DragonBattle__TimerManager__) */
