//
//  UserDataEx.cpp
//  DragonBattle
//
//  Created by 何 永翔 on 13-9-14.
//
//

#include "UserData.h"
#include "UserDataEx.h"
#include "NotificeDefine.h"
#include "lib_formula.h"
#include <algorithm>
using namespace LIB_FORMULA;
using namespace std;
using namespace GlobalGameData;

UserDataEx* UserDataEx::pShareInstance = NULL;

UserDataEx::UserDataEx()
{
    m_bTodayFirstLogin = false;
    m_curTask = -1;
    m_max_main_task = 1000;
    m_curWantedTask = -1;
    m_isWantedTask = false;
    
    hasStartedActivity = false;
    
    user_lv_up_after_battle = 0;
    
    daily_count = 0;
    award_time_counter = 0;
    memset(scroll_count, 0, sizeof(uint32_t) * 4);
    awardTask.clear();
    
    curHammerCnt = 0;
    
    server_time = 0;
    
    m_missionID = 0;
    m_killMonster.clear();
    
    lastMissionId = 0;
    boss_cur_hp = 0;
    
    m_nMaxMission = 0;
    m_nMaxHardMission = 0;
    m_nMaxElitMission = 0;
}
UserDataEx::~UserDataEx()
{
    
}

UserDataEx* UserDataEx::shareInstance()
{
    if (pShareInstance == NULL)
    {
        pShareInstance = UserDataEx::create();
    }
    return pShareInstance;
}

UserDataEx* UserDataEx::create()
{
    UserDataEx* pRet = new UserDataEx();
    
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    
    CC_SAFE_DELETE(pRet);
    return NULL;
}
bool UserDataEx::init()
{
    return true;
}

int UserDataEx::getOptimalTask()
{
    int taskID = -1;
    for (std::map<int, task_info_t>::iterator it = taskMap.begin(); it != taskMap.end(); it++)
    {
        task_info_t* pTask = &it->second;
        int taskType = IS_MAIN_LINE_TASK(pTask->task_id) ? 1 : 2;
        if (taskID == -1)
        {
            taskID = pTask->task_id;
        }
        else
        {
            task_info_t* perTask = &taskMap[taskID];
            int perTaskType = IS_MAIN_LINE_TASK(perTask->task_id) ? 1 : 2;
            int optimal1 = 100*(pTask->cur_step == pTask->max_step?1:0) + 10*(2 - taskType) + pTask->task_state;//优先级 可交 主支线 可接 进行中
            int optimal2 = 100*(perTask->cur_step == perTask->max_step?1:0) + perTask->task_state*10 + (2 - perTaskType);
            if (optimal1 > optimal2)
            {
                taskID = pTask->task_id;
            }
        }
    }
    return taskID;
}

bool UserDataEx::hasOpenServerBonus()
{
    return gUserDataEx->hasOpenTargetBonus()
    || gUserDataEx->hasOpenVip6Bonus()
    || gUserDataEx->hasOpenCountryBonus()
    || gUserDataEx->hasOpenDoubleBonus();
}

bool UserDataEx::hasSealedBonus()
{
    return gUserDataEx->hasSealedLogInBonus()
    || gUserDataEx->hasSealedLvBonus()
    || gUserDataEx->hasSealedBattleValueBonus();
    //|| gUserDataEx->hasSealedTopLevelBonus()
    //|| gUserDataEx->hasSealedTopBattleBonus();
}

bool UserDataEx::hasOpenTargetBonus()
{
    if ((seven_day_prize & (1 << (login_day - 1)))) return false;
    if (seven_day_prize >> 16 == 7) return true;
    return false;
}
bool UserDataEx::hasOpenVip6Bonus()
{
    return prize_state_1 >> 24 == 0 && gUserData->getVipLevel() >= 6;
}
bool UserDataEx::hasOpenCountryBonus()
{
    int maxLv = (gUserDataEx->prize_state_1 >> 16) & 0xFF;
    int maxSkillLv = (gUserDataEx->prize_state_1 >> 12) & 0x0F;
    int bonusState = (gUserDataEx->prize_state_1 >> 8) & 0x0F;
    
    if (maxLv >= 30 && ((bonusState >> 0) & 1) == 0) return true;
    if (maxSkillLv >= 5 && ((bonusState >> 1) & 1) == 0) return true;
    if (maxSkillLv >= 10 && ((bonusState >> 2) & 1) == 0) return true;
    
    return false;
}
bool UserDataEx::hasOpenDoubleBonus()
{
    int state = prize_state_1 & 0xFF;
    if ((state & 1) && (state & (1 << 1)) && (state & (1 << 2)) && (state & (1 << 3)) && (state & (1 << 4)) && !(prize_state_1 & 0x80))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool UserDataEx::hasSealedLogInBonus()
{
    int loginDay = login_count >> 16;
    if (loginDay > 7) loginDay = 7;
    int getState = login_count & 0xFFFF;
    
    for (int i = 0; i < loginDay; i++)
    {
        if ((getState & (1 << i)) == 0)
        {
            return true;
        }
    }
    return false;
}

bool UserDataEx::hasWorshipFairy()
{
    return worship_fairy&0x01;
}

bool UserDataEx::hasRechargeFirstBonus()
{
    uint32_t totolVipGift = gUserData->m_dataEx->totol_vip_gift;
    return ((totolVipGift >> 16) & 1) == 0 && (gUserData->getBuyGold() > 0);
}

bool UserDataEx::hasRechargeTotalBonus()
{
    for (int i = 0; i < 12; i++)
    {
        //vip充值礼包 totol_vip_gift>>16首冲礼包 totol_vip_gift&0x0000FFFF累计充值
        if (((gUserData->m_dataEx->totol_vip_gift >> i) & 1) == 0)
        {
            if (gUserData->getBuyGold() >= GlobalGameData::gTotolVipGold[i])
            {
                return true;
            }
        }
    }
    return false;
}

bool UserDataEx::hasSealedLvBonus()
{
    int value = prize_state_2 >> 24;
    if ((value & (1 << 0)) == 0 && gUserData->m_heroData->getLV() >= 25) return true;
    if ((value & (1 << 1)) == 0 && gUserData->m_heroData->getLV() >= 40) return true;
    if ((value & (1 << 2)) == 0 && gUserData->m_heroData->getLV() >= 50) return true;
    if ((value & (1 << 3)) == 0 && gUserData->m_heroData->getLV() >= 60) return true;
    if ((value & (1 << 4)) == 0 && gUserData->m_heroData->getLV() >= 70) return true;
    if ((value & (1 << 5)) == 0 && gUserData->m_heroData->getLV() >= 80) return true;
    if ((value & (1 << 6)) == 0 && gUserData->m_heroData->getLV() >= 90) return true;
    return false;
}

bool UserDataEx::hasSealedBattleValueBonus()
{
    int value = prize_state_2 >> 16;
    if ((value & (1 << 0)) == 0 && gUserData->m_heroData->getFightPoint() >= 10000) return true;
    if ((value & (1 << 1)) == 0 && gUserData->m_heroData->getFightPoint() >= 50000) return true;
    if ((value & (1 << 2)) == 0 && gUserData->m_heroData->getFightPoint() >= 100000) return true;
    if ((value & (1 << 3)) == 0 && gUserData->m_heroData->getFightPoint() >= 200000) return true;
    if ((value & (1 << 4)) == 0 && gUserData->m_heroData->getFightPoint() >= 400000) return true;
    if ((value & (1 << 5)) == 0 && gUserData->m_heroData->getFightPoint() >= 600000) return true;
    if ((value & (1 << 6)) == 0 && gUserData->m_heroData->getFightPoint() >= 800000) return true;
    return false;
}

bool UserDataEx::hasSealedTopLevelBonus()
{
    return ((prize_state_2 >> 8) & 0xFF) == 1;
}

bool UserDataEx::hasSealedTopBattleBonus()
{
    return (prize_state_2 & 0xFF) == 1;
}

bool UserDataEx::hasPlatformBonus()
{
    if (this->hasLoginBonus())
    {
        return true;
    }
    if (this->hasLvBonus())
    {
        return true;
    }
    return false;
}
bool UserDataEx::hasLoginBonus()
{
    uint32_t prize = week_login_prize;
    int count = prize / 10000;
    for (int i = 0; i < 7; i++)
    {
        if (count >= i + 1)
        {
            if ((((prize % 10000) >> i) & 1) != 1)
            {
                return true;
            }
        }
    }
    return false;
}
bool UserDataEx::hasLvBonus()
{
    int lv = gUserData->m_heroData->getLV();
    uint32_t prize = level_prize;
    for (int i = 0; i < 10; i++)
    {
        if (lv >= (i + 1) * 10)
        {
            if (((prize >> i) & 1) != 1)
            {
                return true;
            }
        }
    }
    return false;
}
bool UserDataEx::hasInviteBonus()
{
    //奖励及领奖信息,gift_info>>16 低0-4位对应5个奖品条件是否满足,gift_info 低0-4位对应5个奖品是否已经被领取
    for (int i = 0; i < 8; i++)
    {
        if (((gift_info >> (16 + i)) & 1) && !((gift_info >> i) & 1)) return true;
    }
    return false;
}
bool UserDataEx::hasWelfareBonus()
{
    //if (gUserData->m_dataEx->is_vip_gift == 0) return true;
    if (gUserData->m_dataEx->offline_exp != 0) return true;
    //if (gUserData->m_dataEx->offline_days != 0) return true;
    return false;
}
bool UserDataEx::hasBookedBonus()
{
    uint32_t signValue = gUserData->m_dataEx->sign_value;
    int count = 0;
    for (int i = 0; i < 32; i++)
    {
        if (((signValue >> i) & 1) == 1)
        {
            count++;
        }
    }
    
    int prizeCount = 0;
    if (count >= 28)
    {
        prizeCount = 6;
    }
    else if (count >= 21)
    {
        prizeCount = 5;
    }
    else if (count >= 14)
    {
        prizeCount = 4;
    }
    else if (count >= 7)
    {
        prizeCount = 3;
    }
    else if (count >= 5)
    {
        prizeCount = 2;
    }
    else if (count >= 3)
    {
        prizeCount = 1;
    }
    else
    {
        prizeCount = 0;
    }
    
    return prizeCount > sign_prize;
}

bool UserDataEx::hasFinishWantedTask()
{
    bool ret = false;
    for (map<int, award_task_info_t>::iterator it = awardTask.begin(); it != awardTask.end(); it ++)
    {
        award_task_info_t task = it->second;
        if (task.task_state == 1 && task.cur_step == task.max_step)
        {
            ret = true;
            break;
        }
    }
    
    return ret;
}

int UserDataEx::hasCanAcceptWantedTask()
{
    int canAccept = gUserDataEx->daily_count >= 4 ? 4 : gUserDataEx->daily_count;
    std::map<int, award_task_info_t>::iterator it = gUserDataEx->awardTask.begin();
    for (; canAccept != 0 && it != gUserDataEx->awardTask.end(); it ++)
    {
        if (it->second.task_state == 1 || it->second.task_state == 2)
        {
            canAccept --;
        }
    }
    
    if (canAccept < 0)
    {
        canAccept = 0;
    }
    
    return canAccept;
}

CCArray *UserDataEx::getNpcTasks(int npcID)
{
    CCArray *arr = CCArray::create();
    for (std::map<int, task_info_t>::iterator it = taskMap.begin(); it != taskMap.end(); it++)
    {
        task_info_t* pTask = &it->second;
        const TaskConfig_t *conf = GameData::getTaskConf(pTask->task_id);
        if(pTask->task_state == 1)//任务已接
        {
            if(pTask->cur_step == pTask->max_step)//任务已完成
            {
                if (conf->nFinishNPC == npcID)//npc是交任务的人
                {
                    TaskState *state = TaskState::create(conf->nTaskID, kTaskCanGive);
                    arr->addObject(state);
                }
            }
            else//任务未完成
            {
                if (conf->stageTalk.nTalkTyep == 1 && conf->stageTalk.nNpcID == npcID &&pTask->max_step != pTask->cur_step)//npc是剧情对话的人并且还没对话
                {
                    TaskState *state = TaskState::create(conf->nTaskID, kTaskMovie);
                    arr->addObject(state);
                }
                else if (conf->nAcceptNPC == npcID)//npc是接任务的人
                {
                    TaskState *state = TaskState::create(conf->nTaskID, kTaskGot);
                    arr->addObject(state);
                }
            }
        }
        else//可接
        {
            if (conf->nAcceptNPC == npcID)
            {
                TaskState *state = TaskState::create(conf->nTaskID, kTaskCanGet);
                arr->addObject(state);
            }
        }
    }
    
    sort(arr->data->arr, arr->data->arr + arr->data->num, UserDataEx::cmpTask);
    
    return arr;
}
//圆圈任务排序
bool UserDataEx::cmpTask(const CCObject* in_pCcObj0, const CCObject* in_pCcObj1)
{
    TaskState *state1,*state2;
    state1 = (TaskState *)in_pCcObj0;
    state2 = (TaskState *)in_pCcObj1;
    const TaskConfig_t *conf1,*conf2;
    conf1 = GameData::getTaskConf(state1->taskID);
    conf2 = GameData::getTaskConf(state2->taskID);
    int quarty1 = state1->taskState*10 - conf1->nTaskType;
    int quarty2 = state2->taskState*10 - conf2->nTaskType;
    return quarty1 >= quarty2;
}
//dialog任务排序
bool UserDataEx::cmpTaskDialog(const CCObject* in_pCcObj0, const CCObject* in_pCcObj1)
{
    TaskState *state1,*state2;
    state1 = (TaskState *)in_pCcObj0;
    state2 = (TaskState *)in_pCcObj1;
    const TaskConfig_t *conf1,*conf2;
    conf1 = GameData::getTaskConf(state1->taskID);
    conf2 = GameData::getTaskConf(state2->taskID);
    int quarty1 = (10-conf1->nTaskType)*10 + state1->taskState;
    int quarty2 = (10-conf2->nTaskType)*10 + state2->taskState;
    return quarty1 >= quarty2;
}

int UserDataEx::getTaskClipByMissionID(int missionID,bool isStart)
{
    for (std::map<int, task_info_t>::iterator it = taskMap.begin(); it != taskMap.end(); it++)
    {
        task_info_t* pTask = &it->second;
        const TaskConfig_t *conf = GameData::getTaskConf(pTask->task_id);
        if(pTask->task_state == 1 && pTask->cur_step != pTask->max_step && conf->nFinishType == 2 && conf->stageTalk.nNpcID == missionID && conf->stageTalk.stageTalkVec.size() != 0)
        {
            if (conf->stageTalk.nOverIndex == conf->stageTalk.stageTalkVec.size())//剧情在前面
            {
                if (isStart)
                {
                    return pTask->task_id;
                }
                else
                {
                    break;
                }
            }
            else if(conf->stageTalk.nOverIndex == 0)//剧情在后面
            {
                if (!isStart)
                {
                    return pTask->task_id;
                }
                else
                {
                    break;
                }
            }
            else
            {
                return pTask->task_id;
            }
            
        }
    }
    return -1;
}

int UserDataEx::getCurTask()
{
    return m_curTask;
}

void UserDataEx::setCurTask(int task)
{
    m_curTask = task;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(CURRENT_TASK_NOTE);
}

void UserDataEx::execWantedTask(int type, int step)
{
    for (map<int, award_task_info_t>::iterator it = awardTask.begin(); it != awardTask.end(); it ++)
    {
        int task_id = it->first;
        award_task_info_t task = it->second;
        const AwardTaskConfig_t *taskConf  = GameData::getAwardTaskConfig(task_id);
        
        if (task.task_state == 1 && task.cur_step < task.max_step)
        {
            if (taskConf->nFinishType == type)
            {
                if (taskConf->nFinishType == 1)
                {
                    
                }
                else if (taskConf->nFinishType == TASK_PASS_MISSION_TYPE)
                {
                    if (taskConf->nNeedID == m_missionID)
                        awardTask[task_id].cur_step += step;
                }
                else
                {
                    awardTask[task_id].cur_step += step;
                }
                
                if (awardTask[task_id].cur_step >= awardTask[task_id].max_step)
                {
                    awardTask[task_id].cur_step = awardTask[task_id].max_step;
                    // notifi
                    CCNotificationCenter::sharedNotificationCenter()->postNotification(WANTED_TASK_FINISH_NODE);
                }
                
            }
        }
    }
}

bool UserDataEx::hasHelpAward()
{
    bool bHas = false;
    for (int i = 0; i<4; i++)
    {
        if (gActiveDegreeField[i] <= getActiveDegreeTotal())
        {
            if ((gUserDataEx->active_degree.prize_state&(0x01<<i)) == 0)
            {
                bHas = true;
                break;
            }
        }
    }
    return bHas;
}

int UserDataEx::getActiveDegreeTotal()
{
    return user_cal_active_degree(active_degree.active_state);
}

void UserDataEx::setActiveDgreeValue(int type,int count)
{
    add_user_active_degree(type, count, active_degree.active_state);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(HAS_DAILY_NOTE);
}

void UserDataEx::caculateTodayFirstLogin(uint32_t lastlogin)
{
    if (lastlogin == 0)
    {
        setTodayFirstLogin(false);
    }
    else
    {
        uint32_t curtime = cur_date;
        int day = curtime % 100;
        curtime /= 100;
        int month = curtime % 100;
        int year = curtime / 100;
        struct tm* tm_lastlogintime;
        time_t time_lastlogin = lastlogin;
        tm_lastlogintime = localtime(&time_lastlogin);
        if(tm_lastlogintime->tm_year + 1900 == year &&
           tm_lastlogintime->tm_mon+1 == month &&
           tm_lastlogintime->tm_mday == day)
        {
            setTodayFirstLogin(false);
        }
        else
        {
            setTodayFirstLogin(true);
        }
    }
}

bool UserDataEx::hasRechargeBonus()
{
    for (int i = 0; i < GlobalGameData::max_online_time_prize_num; i++)
    {

    }
    return false;
}