#ifndef TASK_CONF_H__
#define TASK_CONF_H__

#include <map>
#include <vector>
#include <string>
#include "XmlEncrypt.h"

// finishType:1杀怪   2通关(可有剧情)   3收集  4与NPC交谈（可有剧情）   5强化   6经脉   7练气   8砸罐 9技能升级
enum eTaskFinishType {
	TASK_KILL_MONSTER_TYPE	= 1,	//杀怪多少只
	TASK_PASS_MISSION_TYPE	= 2,	//通过关卡
	TASK_COLLECT_ITEM_TYPE	= 3,	//收集item
	TASK_NPC_TALK_TYPE		= 4,	//NPC交谈(送信 给NPC东西都都可看做与NPC交谈,由前端主动发送)
	TASK_EQUIPMENT_UP_TYPE	= 5,	//装备强化类
	TASK_NERVER_UP_TYPE		= 6,	//经脉
	TASK_FORCE_UP_TYPE		= 7,	//练气
	TASK_BREAK_JAR_TYPE		= 8,	//砸罐
	TASK_SKILL_UP_TYPE		= 9,	//角色技能升级
	TASK_WEAR_EQUIPMENT_TYPE	= 10, //穿戴装备
    TASK_ENLIST_PARTNER_TYPE    = 11, //招募弟子
    TASK_SHAKE_MONEY_TREE_TYPE  = 12, //摇钱树
    TASK_ELITE_BOSS_TYPE        = 13, //精英怪
    TASK_SPORTS_TYPE            = 14, //竞技场
    TASK_FARMLAND_TYPE          = 15, //山庄种植
    TASK_SHOPPING_MALL_TYPE     = 16, //商城购买
    TASK_SKILL_MISSION_TYPE     = 17, //藏经阁
    TASK_TRAIN_TYPE             = 18, //培养
	TASK_CHANGE_EQUIPMENT_TYPE	= 19, //装备进阶(TODO悬赏暂没实现)
	TASK_TRAIN_PARTNER_TYPE		= 20, //培养弟子(TODO悬赏暂没实现)
	TASK_PARTNER_INHERIT_TYPE	= 21, //弟子传承(TODO悬赏暂没实现)
	TASK_SUCCINCT_EQUIPMENT_TYPE = 22,//装备洗练(TODO悬赏暂没实现)
	TASK_USER_ASTROLOGY_TYPE	= 23, //占星(悬赏任务已实现)
	TASK_TOWER_BATTLE_TYPE		= 24, //通天塔战斗(TODO悬赏暂没实现)
	TASK_PUNCH_EQUIPMENT_TYPE	= 25, //装备打孔(TODO悬赏暂没实现)
	TASK_INLAY_EQUIPMENT_TYPE	= 26, //装备镶嵌(TODO悬赏暂没实现)
	TASK_COMPOSE_GEM_TYPE		= 27, //合成宝石(TODO悬赏暂没实现)
    TASK_ADD_FRIEND_TYPE        = 28, //添加好友(悬赏任务已实现)
    TASK_BODYGUARD_TYPE         = 29, //运镖(悬赏任务已实现)
	TASK_MAX_TYPE,
};

struct RoleTalk_t
{
	uint32_t nRoleID;		//0表示自己，其他：在场景中表示NPC ID，在关卡中表示怪ID
	std::string roleTalk;	//剧情对话
};

struct StageTalk_t
{
	uint32_t nTalkTyep;		//1表示剧情在场景中 2表示在关卡中
	uint32_t nNpcID;		//场景中对话的NPC ID
	uint32_t nOverIndex;	//nOverIndex之前的对话是进入关卡播放的 之后的对话时战胜boss之后播放的(没战胜不放)
	std::vector<RoleTalk_t> stageTalkVec;
};

struct TaskConfig_t
{
	uint32_t nTaskID;		//任务ID
	uint32_t nTaskType;		//任务类型
	uint32_t nPreTaskID;	//前置任务
	uint32_t nAfterTaskID;	//后置任务
	uint32_t nAcceptNPC;	//接取任务时的NPC ID
	uint32_t nFinishNPC;	//提交任务时的NPC ID
	uint32_t nNeedLevel;	//接取所需要的等级
	uint32_t nMaxLevel;		//接取所需的最大等级
	uint32_t nFinishType;	//完成任务所需的条件类型
	uint32_t nNeedID;		//完成任务所需条件的ID
	uint32_t nNeedCount;	//完成任务所需条件的数量
	uint32_t nSkillIndex;	//完成任务后得到主动技能的编号(与角色职业类型组合成技能id)
	uint32_t nPrizeSwap[4];	//任务的奖励，对应到swap表中的ID，如果0有值，则共用
	char	 name[32];		//任务的名称
    char	 tips[32];		//任务的进行中提示
	char	 desc[128];		//任务的描述,在任务面板中显示的描述
	char	 npcTalk[3][512];	//接任务 已接不可完成 可完成时的NPC说的话
	StageTalk_t	stageTalk;
    
    uint32_t nBossID;
};

#define IS_MAIN_LINE_TASK(nTaskID)		((nTaskID) < 700)
#define IS_BRANCH_LINE_TASK(nTaskID)	((nTaskID) >= 700)
#define MAX_AWARD_TASK_ID  (1200)

typedef std::map<uint32_t, TaskConfig_t*> TaskMap;

class TaskConfig
{
public:
	TaskConfig();
	~TaskConfig();
	
	bool loadFromFile(const char* filePath, bool bEncrypt = false);
    void unload();
    
    const TaskConfig_t* getTaskConfig(uint32_t nTaskID) const;
	uint32_t startBranchTaskIDByLevel(uint32_t nLevel) { return m_nTaskLevelLimit[nLevel]; }
private:
	TaskMap m_taskMap;
	uint32_t m_nTaskLevelLimit[201];		//每个等级支线任务的开始ID，从等级开始
};

struct AwardTaskConfig_t
{
	uint32_t nTaskID;		//悬赏任务ID
	uint32_t nNeedLevel;	//接取所需要的等级
	uint32_t nFinishType;	//完成任务所需的条件类型
	uint32_t nNeedID;		//完成任务所需条件的ID
	uint32_t nNeedCount;	//完成任务所需条件的数量
	uint32_t nPrizeSwap;	//任务的奖励，对应到swap表中的ID
	char	 name[32];		//任务的名称
	char	 desc[128];		//任务的描述,在任务面板中显示的描述
};

class AwardTaskConfig
{
public:
	AwardTaskConfig();
	~AwardTaskConfig();
	bool loadFromFile(const char* filePath, bool bEncrypt = false);
	void unload();

	const AwardTaskConfig_t* getAwardTaskConfig(uint32_t nTaskID) const;
	uint32_t startAwardTaskIDByLevel(uint32_t nLevel) { return m_nTaskLevelLimit[nLevel]; }

private:
	AwardTaskConfig_t m_awardTaskArray[MAX_AWARD_TASK_ID + 1];
	uint32_t m_nTaskLevelLimit[201];		//每个等级悬赏任务的开始ID，从等级开始
};

#endif
