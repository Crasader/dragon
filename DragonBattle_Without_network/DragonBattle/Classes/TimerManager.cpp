//
//  TimerManager.cpp
//  DragonBattle
//
//  Created by 白明江 on 12/5/13.
//
//

#include "TimerManager.h"
#include "UserData.h"
#include "UserDataEx.h"

static TimerManager* g_sTimerManagerInstance = NULL;

TimerManager::TimerManager()
: m_tOlineBonusTimer(0.f)
, m_iOnlineBonusID(0)
, m_tLuckyTimer(0.f)
, m_iLuckyDialID(0.f)
, m_tFarmlandTimer(0.f)
, m_bHasHarvest(false)
, m_tLeaveTime(0)
, m_tDeltaTime(0)
, m_tActivityTime(0)
{
    m_pTimerNodeArr = CCArray::create();
    m_pTimerNodeArr->retain();
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TimerManager::synchFarmlandTimeCbk), GET_VILLA_INFO_NOTE, NULL);

}

TimerManager::~TimerManager()
{
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);

    CC_SAFE_RELEASE_NULL(m_pTimerNodeArr);
}

TimerManager* TimerManager::getInstance()
{
    if (g_sTimerManagerInstance == NULL)
    {
        g_sTimerManagerInstance = new TimerManager;
    }
    
    return g_sTimerManagerInstance;
}

void TimerManager::onEnter()
{
    CCNode::onEnter();
    // To do:
    calculateTime();
    schedule(schedule_selector(TimerManager::update), 1.f);
    
}

void TimerManager::onExit()
{
    // To do:
    unschedule(schedule_selector(TimerManager::update));
    recordLeaveTime();
    
    CCNode::onExit();
}

void TimerManager::recordLeaveTime()
{
    m_tLeaveTime = time(NULL);
}

void TimerManager::calculateTime()
{
    if (m_tLeaveTime != 0)
    {
        time_t back_time;
        back_time = time(NULL);
        
        m_tDeltaTime = back_time - m_tLeaveTime;
        
        time_t delta_time = m_tDeltaTime - 5; //延迟计算5秒
        
        delta_time = delta_time < 0 ? 0 : delta_time;
        
        m_tFarmlandTimer -= delta_time;
        m_tOlineBonusTimer -= delta_time;
        m_tLuckyTimer -= delta_time;
        
        if (m_tFarmlandTimer < 0) m_tFarmlandTimer = 0;
        if (m_tOlineBonusTimer < 0) m_tOlineBonusTimer = 0;
        if (m_tLuckyTimer < 0) m_tLuckyTimer = 0;
        
        CCObject* pObj = NULL;
        CCARRAY_FOREACH(m_pTimerNodeArr, pObj)
        {
            TimerNode* pNode = (TimerNode*)pObj;
            if (pNode)
            {
                int timer = pNode->getTimerCount();
                timer -= m_tDeltaTime;
                if (timer < 0) {
                    timer = 0;
                }
                
                pNode->setTimerCount(timer);
            }
        }
        
    }
    m_tLeaveTime = 0;
}

void TimerManager::synchTime(cli_user_login_ex_out* out)
{
    m_tOlineBonusTimer = out->online_prize_second;
    m_iOnlineBonusID = out->online_prize_id;
    
    m_tLuckyTimer = out->left_second;
    m_iLuckyDialID = out->turntable_id;
    
    //
    // 离开状态中更新服务器时间重新计算离开时间
    //
    if (m_tLeaveTime > 0)
    {
        m_tLeaveTime = time(NULL);
    }
    
}

void TimerManager::synchFarmlandTime()
{
    OnlineManager::sharedManager()->userGetVillaInfo(gUserData->userid);
}

void TimerManager::synchFarmlandTimeCbk(CCObject* pObj)
{
    NotificeObject* obj = (NotificeObject*)pObj;
    cli_user_get_villa_info_out* out = (cli_user_get_villa_info_out*)obj->msg;
    
    m_bHasHarvest = false;
    
    for (int i = 0; i < out->land_vec.size(); i ++)
    {
        villa_land_info_t info = out->land_vec.at(i);
        if (info.plant_id != 0)
        {
            setFarmlandTimer(info.left_time);
            if (info.left_time == 0)
            {
                m_bHasHarvest = true;
            }
        }
    }
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(HAS_DAILY_NOTE);
}

void TimerManager::update(float dt)
{
    if (m_tDeltaTime >= 2)
    {
        dt = m_tDeltaTime;
        m_tDeltaTime = 0;
    }
    
    updateOnlineBonusTimer(dt);
    updateLuckyTimer(dt);
    updateCountDownTimer(dt);
    
    gUserDataEx->server_time += (int)dt;
    
     //活动结束
    if (gUserDataEx->server_time >= m_tActivityTime)
    {
        m_tActivityTime = 0;
        gUserDataEx->hasStartedActivity = false;
        CCNotificationCenter::sharedNotificationCenter()->postNotification(HAS_DAILY_NOTE);
    }
    
    if (m_tFarmlandTimer > 0)
    {
        m_tFarmlandTimer --;
        
        if (m_tFarmlandTimer == 1)
        {
            m_bHasHarvest = true;
            CCNotificationCenter::sharedNotificationCenter()->postNotification(HAS_DAILY_NOTE);
        }
    }
    
    for (int i = 0; i < m_vecTimeDelegates.size(); i ++)
    {
        TimerDelegate* delegate = m_vecTimeDelegates.at(i);
        if (delegate && delegate->getRun())
        {
            delegate->updateTimer(dt);
        }
    }
    
//    for (std::vector<TimerDelegate*>::iterator it = m_vecTimeDelegates.begin(); it != m_vecTimeDelegates.end(); )
//    {
//        TimerDelegate* delegate = (*it);
//        if (delegate && delegate->getRun())
//        {
//            delegate->updateTimer(dt);
//        }
//        
//        it ++;
//    }
}

void TimerManager::updateOnlineBonusTimer(float dt)
{
    if (m_tOlineBonusTimer > 0)
    {
        m_tOlineBonusTimer --;
    }
    else
    {
        m_tOlineBonusTimer = 0;
    }

}

void TimerManager::updateLuckyTimer(float dt)
{
    if (m_tLuckyTimer > 0)
    {
        m_tLuckyTimer --;
    }
    
    if (m_tLuckyTimer == 0)
    {
        m_iLuckyDialID ++;
        if (m_iLuckyDialID > 4)
        {
            m_iLuckyDialID = 0; // 结束
            m_tLuckyTimer = 0;
        }
        
        if (m_iLuckyDialID > 0)
        {
            m_tLuckyTimer = 24 * 60 * 60 + m_tLuckyTimer;
        }
    }

}

void TimerManager::updateCountDownTimer(float dt)
{
    CCObject* pObj = NULL;
    
    CCARRAY_FOREACH(m_pTimerNodeArr, pObj)
    {
        TimerNode* pNode = (TimerNode*)pObj;
        if (pNode->getTimerParent() != NULL && pNode->getTimerLabel() != NULL)
        {
            int countDownTimer = pNode->getTimerCount();
            
            if (countDownTimer > 0)
            {
                countDownTimer --;
                pNode->setTimerCount(countDownTimer);
                
                showTimeWithFormat(pNode->getTimerLabel(), pNode->getTimerFormat(), pNode->getTimerFormatType(), countDownTimer);
            }
            else if (countDownTimer == 0)
            {
                countDownTimer --;
                pNode->setTimerCount(countDownTimer);
                showTimeWithFormat(pNode->getTimerLabel(), pNode->getTimerFormat(), pNode->getTimerFormatType(), countDownTimer);
                
                // 通知UI
                noticeUI(pNode->getTimerParent(), pNode->getTimerLabel());
            }
            else
            {
                // To Do 计时结束
                if (pNode->getRemoveSelf())
                {
                    m_pTimerNodeArr->removeObject(pNode);
                }
            }
        }
    }
}

void TimerManager::showTimeWithFormat(CCLabelTTF* pLblTime, CCString* pFormat, kTime_Format format_type, int countDownTimer)
{
    int hour = countDownTimer / 3600;
    int minute = (countDownTimer % 3600) / 60;
    int second = (countDownTimer % 3600) % 60;
    
    if (pFormat)
    {
        const char* format = pFormat->getCString();
        
        if (format_type == Time_Format_hh_mm_ss)
        {
            pLblTime->setString(CCString::createWithFormat(format, hour, minute, second)->getCString());
        }
        else if (format_type == Time_Format_mm_ss)
        {
            pLblTime->setString(CCString::createWithFormat(format, minute, second)->getCString());
        }
    }
    else
    {
        pLblTime->setString(CCString::createWithFormat("%02d:%02d:%02d", hour, minute, second)->getCString());
    }
}


void TimerManager::noticeUI(CCNode* pParent, CCLabelTTF* pLabel)
{
    for (std::vector<TimerDelegate*>::iterator it = m_vecTimeDelegates.begin(); it != m_vecTimeDelegates.end(); it ++)
    {
        if ((*it)->getTimerParent() == pParent)
        {
            (*it)->overTimer(pParent, pLabel);
            break;
        }
    }
}

void TimerManager::addTimerDelegate(TimerDelegate* delegate, CCNode* pParent/* = NULL*/)
{
    if (delegate)
    {
        delegate->setTimerParent(pParent);
        m_vecTimeDelegates.push_back(delegate);
    }
    else
    {
        CCLOGWARN("Time Delegate is Null!");
    }
}


void TimerManager::removeTimerDelegate(TimerDelegate* delegate)
{
    XYSmartLock sl(&m_oLock);
    
    for (std::vector<TimerDelegate*>::iterator it = m_vecTimeDelegates.begin(); it != m_vecTimeDelegates.end(); )
    {
        if ((*it) == delegate)
        {
            m_vecTimeDelegates.erase(it);
            break;
        }
        else
        {
            it ++;
        }
    }
}

void TimerManager::setCountDownLabel(CCNode* pParent, CCLabelTTF* pLabel, const char* format, int countDown)
{
    TimerNode* pNode = new TimerNode;
    pNode->setTimerParent(pParent);
    pNode->setTimerLabel(pLabel);
    pNode->setTimerFormat(CCString::create(format));
    pNode->setTimerCount(countDown);
    
    m_pTimerNodeArr->addObject(pNode);
    
    CC_SAFE_RELEASE_NULL(pNode);
}

void TimerManager::setTimerNode(TimerNode* pNode)
{
    m_pTimerNodeArr->addObject(pNode);
}

void TimerManager::removeCountDownLabel(CCNode* pParent)
{
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(m_pTimerNodeArr, pObj)
    {
        TimerNode* pNode = (TimerNode*)pObj;
        if (pNode->getTimerParent() == pParent)
        {
            m_pTimerNodeArr->removeObject(pNode);
        }
    }
    
}

time_t TimerManager::getFarmlandTimer()
{
    return m_tFarmlandTimer;
}

void TimerManager::setFarmlandTimer(time_t var)
{
    XYSmartLock sl(&m_oLock);
    
    if (var < m_tFarmlandTimer)
    {
        m_tFarmlandTimer = var;
    }
}

long TimerManager::getOnlineTimerByID(int nid)
{
    long ret = 0;
    switch (nid) {
        case 1: break;
        case 2: ret = 5 * 60; break;
        case 3: ret = 10 * 60; break;
        case 4: ret = 15 * 60; break;
        case 5: ret = 15 * 60; break;
        case 6: ret = 15 * 60; break;
        case 7: ret = 30 * 60; break;
        default:
            break;
    }
    
    return ret;
}





