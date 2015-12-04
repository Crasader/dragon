//
//  HelperConf.h
//  DragonBattle
//
//  Created by zeng xiaolong on 13-12-21.
//
//

#ifndef __DragonBattle__HelperConf__
#define __DragonBattle__HelperConf__

#ifndef WONPEE_SERVER
#include "online_cli_proto_cmd.h"
#else
#include "../../proto/online_cli_proto_cmd.h"
#endif
#include "XmlEncrypt.h"

#include <map>
#include <vector>

//活跃度任务类型
enum {
	LIVENESS_BUY_ENERGY 		= 1,       //购买体力
	LIVENESS_SIGN_UP 			= 2,       //签到
	LIVENESS_WORSHIP_FAIRY 		= 3,       //参拜神仙姐姐
	LIVENESS_PASS_MISSION 		= 4,       //通关任意普通关卡
	LIVENESS_STRENGTHEN_EQUIP 	= 5,       //强化装备
	LIVENESS_UPGRADE_FORCE 		= 6,       //练气
	LIVENESS_UPGRADE_NERVE 		= 7,       //经脉修炼
	LIVENESS_SHAKE_MONEY 		= 8,       //完成摇钱树
	LIVENESS_VILLA_PLANT 		= 9,       //去种植 
	LIVENESS_ELITE_BOSS 		= 10,      //通关精英副本
	LIVENESS_BREAK_JAR 			= 11,      //寻宝
	LIVENESS_TRAIN 				= 12,      //培养 
	LIVENESS_ARENA_BATTLE 		= 13,      //竞技场挑战
	LIVENESS_TRAIN_EQUIPMENT 	= 14,      //洗练
	LIVENESS_ASTROLOGY 			= 15,      //前往占星
	LIVENESS_TOWER_BATTLE 		= 16,      //通天塔战斗
	LIVENESS_AWARD_TASK 		= 17,      //完成悬赏任务 
	LIVENESS_USE_HP_BOTTLE 		= 18,      //战斗中使用血瓶
	LIVENESS_WORLD_BOSS_BTL 	= 19,      //挑战世界boss
	LIVENESS_ESCORT 			= 20,      //前往运镖
	LIVENESS_MAX_TYPE
};

struct DegreeItem_t
{
    char        name[64]; //名字
    uint32_t    type;//跳转类型
    uint32_t    nTaskID;//任务类型
    uint32_t    points;//每完成一次增加的活跃点数
    uint32_t    maxPoints;//最多可以获得的活跃点数
};

typedef std::vector<DegreeItem_t*> ActiveDegreeConfVec;//活跃度列表
typedef std::map<uint32_t/*nTaskID*/, DegreeItem_t*> ActiveDegreeConfMap;//活跃度

struct HelpItem_t
{
    char        name[64]; //名字
    char        desc[256]; //描述
    int         type;//类型
    int         nstar;
    int         nResourceID;
};

struct HelpConf_t
{
    int  nID;      //类别ID
    char name[64];//类别名
    char desc[64];
    std::vector<HelpItem_t>  HelpItemVec;
};

typedef std::map<uint32_t, HelpConf_t*> HelpConfMap;//小助手

class HelpConf
{
public:
    HelpConf();
    ~HelpConf();
    bool loadFromFile(const char* fileName, bool bEncrypt = false);
    void unload();
    const ActiveDegreeConfVec* getActiveDegreeItemVec() const
	{
		return &m_activeVec;
	}
    const DegreeItem_t* getActiveDegreeItem(uint32_t taskID) const
	{
		ActiveDegreeConfMap::const_iterator it = m_activeMap.find(taskID);
		if (it != m_activeMap.end()) {
			return it->second;
		}
		return NULL;
	}

    const HelpConf_t* getHelpItemVec(uint32_t nID)
   	{
        if (m_helpMap.find(nID)!=m_helpMap.end()) {
            return m_helpMap[nID];
        }
        return NULL;
    }
    HelpConfMap m_helpMap;
private:
	ActiveDegreeConfVec m_activeVec;
	ActiveDegreeConfMap m_activeMap;
};
#endif /* defined(__DragonBattle__HelperConf__) */
