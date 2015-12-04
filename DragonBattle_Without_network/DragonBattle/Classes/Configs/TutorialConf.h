//
//  TutorialConf.h
//  DragonBattle
//
//  Created by 白明江 on 9/23/13.
//
//

#ifndef __DragonBattle__TutorialConf__
#define __DragonBattle__TutorialConf__

#include "XmlEncrypt.h"
#include <iostream>
#include <vector>
#include <map>
using namespace std;


typedef enum {
    func_id_hero = 1001,
    func_id_strengthen,
    func_id_skill,
    func_id_nerve,
    func_id_inforce,
    func_id_knight,
    func_id_prestige,
    func_id_pot,
    func_id_skill_mission,
    func_id_rank = 1011,
    func_id_farmland = 1012,
    func_id_elite_boss = 1013,
    func_id_equipment_upgrade = 1014,
    func_id_sport = 1015,
    func_id_activity = 1016,
    func_id_equipment_wash = 1017,
    func_id_tavern = 1018,
    func_id_wanted_task = 1019,
    func_id_tower = 1020,
    func_id_book = 2001,            //签到
    func_id_shake_tree = 2002,      //摇钱树
    func_id_fairy = 2005,           //神仙姐姐
    func_id_friend = 2006,
    func_id_bodyguard = 2008,       //押运
    func_id_diamond = 2009,         //宝石镶嵌
    func_id_star = 2010,            //占星
    func_id_hard_mission = 2011,    //困难关卡
    func_id_goal = 2012,            //活跃度
    
} func_id;

typedef enum {
    eLimit_Type_None = 0,
    eLimit_Type_Task = 1,
    eLimit_Type_Level = 2,

} Limit_Type;


struct OperateConfig_t {
    uint32_t nSubStep;
    uint32_t nCount;       //操作次数
    uint32_t nForce;		//强制引导 1为强制，0为不强制
    uint32_t nFuncID;
    char     desc[32];     //描述信息
};

struct TutorialConfig_t
{
	uint32_t            nStep;		    //第几个步骤
    uint32_t            nType;          //引导类型
	uint32_t            nNeedLevel;     //等级限制
	uint32_t            nTaskID;	    //完成任务
    uint32_t            nUIType;        //引导哪个界面的功能
    uint32_t            nFuncID;        //功能ID
    uint32_t            nDependStep;    //依赖步骤
#ifndef WONPEE_SERVER
    vector<uint32_t>    needItemVec;
    
    uint32_t            nAction;        //引导操作
    uint32_t            nNextStep;	    //接取任务时的NPC ID
	
	char                play_describe[32];		//引导描述信息
    std::string         assistant_say;
 
    vector<OperateConfig_t> subOperations;
#endif
};

typedef std::map<uint32_t, TutorialConfig_t*> TutorialMap;

class TutorialConfig {
public:
	TutorialMap m_tutorialMap;

public:
    TutorialConfig();
	~TutorialConfig();
	
	bool loadFromFile(const char* filePath, bool bEncrypt = false);
    void unload();

#ifndef WONPEE_SERVER
    const TutorialConfig_t* getTutorialConfWithTask(uint32_t nTaskID) const;
    const TutorialConfig_t* getTutorialConfWithLevel(uint32_t nLevelID) const;
    const TutorialConfig_t* getTutorialConfByStep(uint32_t step) const;
    const TutorialConfig_t* getTutorialConfByFuncID(uint32_t func) const;
#endif
    
    Limit_Type checkFuncOpened(uint32_t func_id, uint32_t task_id, uint32_t level);
};

#endif /* defined(__DragonBattle__TutorialConf__) */
