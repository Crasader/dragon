//
//  UserDataEx.h
//  DragonBattle
//
//  Created by 何 永翔 on 13-9-14.
//
//

#ifndef __DragonBattle__UserDataEx__
#define __DragonBattle__UserDataEx__

#include "online_cli_proto_cmd.h"
#include "GameData.h"
#include <cocos2d.h>
#include <vector>
#include "TaskData.h"
#include "GlobalDefine.h"

class UserDataEx : public cocos2d::CCObject
{
public:
    virtual ~UserDataEx();
    static UserDataEx* shareInstance();
    void caculateTodayFirstLogin(uint32_t lastlogin);
    CCArray *getNpcTasks(int npcID);//获取npc的任务状态
    int getTaskClipByMissionID(int missionID,bool isStart);//获取是否播放剧情 -1为无剧情 是否开始时剧情
    int getOptimalTask();
    
    bool hasOpenServerBonus();
    bool hasOpenTargetBonus();
    bool hasOpenVip6Bonus();
    bool hasOpenCountryBonus();
    bool hasOpenDoubleBonus();
    
    bool hasSealedBonus();
    bool hasSealedLogInBonus();
    bool hasSealedLvBonus();
    bool hasSealedBattleValueBonus();
    bool hasSealedTopLevelBonus();
    bool hasSealedTopBattleBonus();
    
    bool hasPlatformBonus();
    bool hasLoginBonus();
    bool hasLvBonus();
    bool hasInviteBonus();
    bool hasWelfareBonus();
    bool hasBookedBonus();
    bool hasFinishWantedTask();
    
    bool hasWorshipFairy();
    bool hasRechargeFirstBonus();
    bool hasRechargeTotalBonus();
    
    bool hasStartedActivity;
    
    int hasCanAcceptWantedTask();
    bool hasRechargeBonus();
    
    void execWantedTask(int type, int step = 1);
    static bool cmpTaskDialog(const CCObject* in_pCcObj0, const CCObject* in_pCcObj1);
    static bool cmpTask(const CCObject* pCcObj0, const CCObject* pCcObj1);
    bool hasHelpAward();
    int getActiveDegreeTotal();
    void setActiveDgreeValue(int type,int count = 1);
    void storeBattleInfo(int mission, std::map<int, int>* info)
    {
        m_missionID = mission;
        if (info) m_killMonster = *info;
    }
    
    void setMaxMainTask(int task_id)
    {
#if NOT_SKIP_FUNC_OPEN
        if (task_id < 700)
        {
            m_max_main_task = task_id;
        }
#endif
    }
    
    int getMaxMainTask() { return m_max_main_task; }
    
public:
    CC_SYNTHESIZE(int, m_nMaxMission, MaxMission);
    CC_SYNTHESIZE(int, m_nMaxHardMission, MaxHardMission);
    CC_SYNTHESIZE(int, m_nMaxElitMission, MaxEliteMission);
    CC_SYNTHESIZE(int, m_curWantedTask, WantedTask);
    CC_SYNTHESIZE(bool, m_isWantedTask, IsWantedTask);
    
    int m_missionID;//普通关卡
    std::map<int, int> m_killMonster;
    
    int m_curTask;
    int getCurTask();
    void setCurTask(int task);
	std::map<int, task_info_t> taskMap;	//主支线任务(已接 可接)
    int m_max_main_task;             //已完成的最大主线任务id
    
    // 悬赏任务
    uint32_t daily_count;
    long award_time_counter;
    uint32_t scroll_count[4];		//英雄榜数量
    std::map<int, award_task_info_t> awardTask;

    time_t login_time;   //记录登陆设备时间
    time_t server_time;    // 心跳返回的服务器时间
    
    // 寻宝锤子数
    uint32_t curHammerCnt;
	
	//奖励平台 周登录礼包与等级礼包物品配表需要预先确定，后期如不做版本变更，无法修改
	//周登录礼包
	uint32_t week_login_prize;		//登录次数 | 是否领取(用位表示)
	//等级奖励
	uint32_t level_prize;			//是否领取(用位表示)
	
	//活动奖励 需要配置
	std::vector<uint32_t> action_prize_vec;//活动ID
    
    // 个人礼包
    uint32_t self_gift;
    
	//签到
	uint32_t cur_date;		//当前日期
	uint32_t sign_value;	//位表示
	uint32_t sign_prize;	//位表示是否领取
	
	//福利：VIP每日领取礼包  离线经验  离线补偿
	uint32_t is_vip_gift;	//是否领取
    uint32_t vip_level_gift;//VIP成长奖励
    uint32_t totol_vip_gift;//vip充值礼包 totol_vip_gift>>16首冲礼包 totol_vip_gift&0x0000FFFF累计充值
    
	uint32_t offline_exp_hour;//离线经验小时数
	uint32_t offline_exp;
    uint32_t offline_cost_cold;//三倍离线经验领取消耗黄金
    
	uint32_t offline_days;//离线天数
	uint32_t offline_days_exp;//离线期内显示的经验
	uint32_t offline_days_silver;//离线期内显示的音量
	//离线补偿
	uint32_t offline_prize_id;	//离线补偿
    
	//剩余次数
	std::vector<uint32_t>	num_vec;	//剩余次数
	
	uint32_t shake_money;	//摇钱树 今日次数=shake_money>>16   双倍时间=shake_money&0xFFFF
    
	uint32_t login_day;     //创建账号开始的第几天登录
	uint32_t seven_day_prize;//头7日登录目标
	uint32_t prize_state_1; //(prize_state_1>>24)&0xFF谁与争锋领奖
    //(prize_state>>16)&0xFF表示最大弟子资质的值 (prize_state>>8)&0xFF表示领取资质奖励的状态
    //(prize_state&0xFF)侠义无双奖励 prize_state&(1<<7)表示是否领取了奖励 其他表示抽取到的位数
	uint32_t login_count;   //登录天数
	uint32_t prize_state_2;	//封测状态值
    uint32_t worship_fairy;	//参拜神仙姐姐 worship_fairy>>16 表示当前是连续第几天, worship_fairy&0x01表示今天是否参拜过
    uint32_t gift_info;//奖励及领奖信息,gift_info>>16 低0-4位对应5个奖品条件是否满足,gift_info 低0-4位对应5个奖品是否已经被领取
    
	uint32_t have_offline_msg;//是否有离线消息 0 无 1有
	uint32_t have_friend_req;//是否有好友请求 0 无 1有
    active_degree_info_t active_degree;//小助手 活跃度
    int user_lv_up_after_battle;
    uint32_t fake_vip_state;	//封测期间领取每日总在线时长奖励状态
    
    UnitBaseConf world_boss_conf;
    int boss_cur_hp;
    CC_SYNTHESIZE(bool, m_bTodayFirstLogin, TodayFirstLogin)//每日首次登陆
public:
    int lastMissionId;//最后打过的关卡ID，用来判定开服的七日目标
    
    std::vector<cli_user_chat_out> moveNoticeVec;//跑马灯信息
    std::vector<cli_user_chat_out> fixNoticeVec;//固定公告信息
private:
    UserDataEx();
    
    static UserDataEx* create();
    bool init();
    
    static UserDataEx* pShareInstance;
    
};

#endif
