//
//  GameData.h
//  DragonBattle
//  游戏配置数据统一的接口
//  Created by 小敦 王 on 13-4-16.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_GameData_h
#define DragonBattle_GameData_h

#include "BattleMissionConf.h"
#include "MapUnitConf.h"
#include "BattleUnitConf.h"
#include "SceneConf.h"

#include "UnitAIConf.h"
#include "ThrowAttackConf.h"
#include "ItemConf.h"
#include "ShopConf.h"
#include "PartnerUpgradeConf.h"
#include "VipConf.h"
#include "SwapItemConf.h"
#include "LVConf.h"
#include "SkillConf.h"
#include "RoleAttrConf.h"
#include "CommunityConf.h"
#include "EquipUpgradeConf.h"
#include "RoleNerveConf.h"
#include "RoleForceConf.h"
#include "MonsterConf.h"
#include "PartnerConf.h"
#include "PrestigeChallengeConf.h"
#include "TaskConf.h"
#include "ActivityConf.h"
#include "HelperConf.h"
#include "PriseDefine.h"
#include "TutorialConf.h"
#include "GiftItemConf.h"

#ifndef WONPEE_SERVER
#include "GlobalDefine.h"
#else
#include "MagicStoreConf.h"
#endif

class GameData
{
public:
    // 加载所有数据
    static bool loadAllConf();
    
    // 根据关卡ID得到关卡信息
    static MissionConf_t* getBattleMission(uint32_t missionId) {
		return (MissionConf_t*)pBattleMissionConf->getBattleMission(missionId);
	}
	//获取某主线关卡的下一个主线关卡
	static const MissionConf_t* getNextMainMission(uint32_t missionId) {
		return pBattleMissionConf->getNextMainMission(missionId);
	}
	//获取某精英关卡的下一个精英关卡
	static const MissionConf_t* getNextEliteMission(uint32_t missionId) {
		return pBattleMissionConf->getNextEliteMission(missionId);
	}
	//获取某困难关卡的下一个困难关卡
	static const MissionConf_t* getNextHardMission(uint32_t missionId) {
		return pBattleMissionConf->getNextHardMission(missionId);
	}
	//根据社区内某种类型(如主线、精英、困难)的关卡id列表
	static const IDVec& getCmtMissionIDVec(MissionType type, uint32_t nCmtMapID) {
		return pBattleMissionConf->getCmtMissionIDVec(type, nCmtMapID);
	}
    static const BattleMissionConf* getBattleMissionConf() { return pBattleMissionConf; }
    static void getMonsterCountInfo(uint32_t nMissionID, MonsterCountInfo_t* pMonsterArray, uint32_t* pArrayCount);
    static const ThiefConf_t* getBattleThiefConf() { return pBattleMissionConf->getBattleThiefConf(); }
 
    // 根据AI树ID得到AI
    static const AIJudgeTree* getAiJudgeTree(uint16_t treeID);
    
    // 根据抛出物ID得到抛出物
    static ThrowAttackConfig_t* getThrowAttackConf(uint16_t throwAttackID);
    
    // 通过itemID得到item
    static const item_t* getItem(uint32_t itemID);
	//找上一级或下一级宝石
    static const item_t* getPreLvGem(uint32_t GemId) { return pItemConfig->getPreLvGem(GemId); }
    static const item_t* getNextLvGem(uint32_t GemId) { return pItemConfig->getNextLvGem(GemId); }
    
    static const PartnerUpgradeItem_t* getPartnerUpgradeItem(uint32_t nLv);

    // 通过shopID得到item列表
    static const std::vector<ItemUnit_t>* getShopItem(uint32_t shopID);
    
    static const SwapItem_t* getSwapItem(uint32_t swapID);
    
    static int getLV(uint64_t nExp);
    static int getLVLimit();
    static int getExpLimit();
    static uint64_t getNextLvExp(uint64_t nExp);
    static LVConf* getLVConf(){return pLVConf;};
    static uint64_t getLVExp(int lv);
    static float getExpPercent(uint64_t nExp);
    
    static int getVipFunction(int viplv,int FunctionID);
	static const int getVipLvByCostGold(int gold) { return pVipConf->getVipLvByCostGold(gold); }
    
    static const SkillLevel_t* getSkillConf(uint32_t nSkillID, uint32_t nSkillLvl);
    static const SkillConf_t* getSkillConf(uint32_t nSkill);
    
    static const PartnerSkillConf_t* getPartnerSkillConf(uint32_t nSkill);
	static const PartnerSkillUpgradeConf_t* getPartnerSkillUpgradeConf(uint32_t nSkill, uint32_t nLv);
    
    static uint32_t getPartnerSkillHurt(uint32_t nSkill,uint32_t nLv);
    
    static const RoleAttrConf_t* getRoleAttrConf(int nLevel, int nRoleType);
    
    // 得到装备进阶配置
    static const EquipUpgradeConf_t* getEquipmentUpgradeConf(uint32_t nequipmentID);
    static bool  isEnableUnlockSkill(int selectSkill,std::map<int,user_skill_t> *skill_map);
    // 根据英雄ID得到改英雄的所有技能
    static const HeroSkillVec* getHeroSkillVec(uint32_t nUnitID);
    
	//得到主城地图信息
    static const CommunitymapConf_t* getCmdMapConf(uint32_t nCmtMapID);
	static void getCmdMapConfByLevel(uint32_t nLevel, CmtMapConfVec& cmtVec);
    static CommunityNPC_t *getNpcConf(int npcID);
    static CmtNPCResource_t *getNpcResourceConf(int resourceID);
    
    static const RoleNerveConf_t* getRoleNerve(uint32_t nNerveID);
    
    static int getRoleForceLevel(uint32_t nForceExp);
    static const RoleForceConf_t* getRoleForceSum(uint32_t nForceLevel);
    static RoleForceConf_t* getRoleForceConf(uint32_t nForceLevel, RoleForceConf_t* pConf);
    static MonsterUnitConf* getMonsterConf(uint32_t nMonsterID);
    
	static const PartnerConf_t* getPartnerConf(uint32_t nPartnerID);
    static PartnerMap* getPartnerMap();
	
	//得到弟子属性
    static const RoleAttrConf_t* getPartnerAttrConf(int nLevel, int nPartnerID, RoleAttrConf_t* pConf);
    
    //场景资源信息
    static SceneConf_t* getSceneConf(uint32_t nID);
    
    static int getPrestigeChallengeCount();
    static const PrestigeChallengeConf_t* getPrestigeChallenge(uint32_t nTitleID);
	static const PrestigeChallengeConf_t* getPrestigeChallengeByMissionID(uint32_t nMissionID);

	static const TaskConfig_t* getTaskConf(uint32_t nTaskID);
	static uint32_t startBranchTaskIDByLevel(uint32_t nLevel) { return pTaskConfig->startBranchTaskIDByLevel(nLevel); }
	static const AwardTaskConfig_t* getAwardTaskConfig(uint32_t nTaskID);
    static uint32_t startAwardTaskIDByLevel(uint32_t nLevel) { return pAwardTaskConfig->startAwardTaskIDByLevel(nLevel); }
    
    //ActivityConf
    static const ActivityConf_t* getActivityConf(uint32_t nID);
    static const ActivityMap getActivityMap();
    
    static const HelpConf_t* getHelpItemVec(uint32_t nID);
    static const HelpConf* getHelpConf();
    
	static const ActiveDegreeConfVec* getActiveDegreeItemVec() { return pHelpConf->getActiveDegreeItemVec(); }
	static const DegreeItem_t* getActiveDegreeItem(uint32_t taskID) { return pHelpConf->getActiveDegreeItem(taskID); }


    static const MissionGift_t* getMissionGift(uint32_t nID) { return pGiftItemConf->getMissionGift(nID); }
    
    //Tutorial
    static TutorialConfig* pTutorialConfig;
    static const Limit_Type checkFuncOpened(uint32_t func_id, uint32_t task_id, uint32_t level) { return pTutorialConfig->checkFuncOpened(func_id, task_id, level); }

private:
    static BattleMissionConf *pBattleMissionConf; //关卡信息配置表
    static AIConfig *pAIConfig;
    static ThrowAttackConfig *pThrowAttackConfig;
    static ItemConfig *pItemConfig;
    static PartnerUpgradeConf* pPartnerUpgradeConf;
    static ShopConf* pShopConf;
    static VipConf* pVipConf;
    static SwapItemConfig *pSwapItemConfig;
    static LVConf *pLVConf;
    static SkillConf* pSkillConf;
    static PartnerSkillConf *pPartnerSkillConf;
    static RoleAttrConf* pRoleAttrConf;
    static CommunitymapConf *pCommunitymapConf;
	static EquipUpgradeConfig *pEquipUpgradeConfig;
    static RoleNerveConfig *pRoleNerveConfig;
    static RoleForceConfig *pRoleForceConfig;
    static MonsterConf *pMonsterConf;
    static PartnerConfig *pPartnerConfig;
    static BattleSceneConf *pBattleSceneConf;
    static PrestigeChallengeConfig *pPrestigeChallengeConfig;
	static TaskConfig *pTaskConfig;
	static AwardTaskConfig *pAwardTaskConfig;
	static ActivityConf* pActivityConf;
    static HelpConf*    pHelpConf;
    static GiftItemConf* pGiftItemConf;
    
#ifdef WONPEE_SERVER
	static MagicStoreConf *pMagicStoreConf;
	
public:
    static int getRandStoreArray(uint32_t curLevel, storeItemInfo_t* pItemArray, uint32_t* arrayCount);
	static const item_t* getRandPartnerCard(uint32_t quarty) { return pItemConfig->getRandPartnerCard(quarty);}
	static void getBattleGift(uint32_t nID, itemDrop_t* pItemList, uint32_t* pItemCnt) {
		return pGiftItemConf->getBattleGift(nID, pItemList, pItemCnt);
	}
#else
public:
    static const TutorialConfig_t* getTutorialConfWithTaskID(uint32_t task_id);
    static const TutorialConfig_t* getTutorialConfWithLevel(uint32_t level);
    static const TutorialConfig_t* getTutorialConfByStep(uint32_t step);
    static const char* getVipDes(int vipLv) { return pVipConf->getDes(vipLv); }
    static const TutorialConfig_t* getTutorialConfByFuncID(uint32_t func_id) { return pTutorialConfig->getTutorialConfByFuncID(func_id); }
#endif
};

namespace GlobalGameData {
//可领取奖励的活跃度的值
const uint8_t gActiveDegreeField[5] = {20, 40, 60, 80};

//弟子开启等级
const uint32_t gPartnerOpenLv[3] = {11,33,43};

//不同品质运镖声望
const uint32_t gBodyGuardPrestige[5] = {20,100,400,800,3000};

//累计充值礼包的区间
const uint16_t gTotolVipGold[12] = {150, 350, 650, 1000, 1550, 2150, 5000, 8000, 16000, 25000, 35000, 50000};
//单笔冲值礼包的区间
const uint32_t gOnceVipGold[10] = {100, 200, 500, 1000};

//声望增加值 固定值得,公式的另外走
enum {
	PRESTIGE_ESCORT_HORSE_1 = 20, //运镖1级马
	PRESTIGE_ESCORT_HORSE_2 = 100, //运镖2级马
	PRESTIGE_ESCORT_HORSE_3 = 400, //运镖3级马
	PRESTIGE_ESCORT_HORSE_4 = 800, //运镖4级马
	PRESTIGE_ESCORT_HORSE_5 = 3000, //运镖5级马

	PRESTIGE_ARENA_GIFT_FIGHT_SUCC = 10, //竞技场挑战成功
	PRESTIGE_ARENA_GIFT_FIGHT_FAIL = 5, //竞技场挑战失败
	};
const uint32_t gPartnerFriendValTrainCostYxb = 100000;

//在线总时长奖励
const uint32_t max_online_time_prize_num = 9;
const uint32_t online_time_prize_minutes[max_online_time_prize_num] = {
	5, 10, 20, 30, 45, 60, 80, 100, 120
};
const uint32_t online_time_prize_golds[max_online_time_prize_num] = {
	50, 100, 150, 200, 250, 300, 400, 500, 600
};

};
#endif
