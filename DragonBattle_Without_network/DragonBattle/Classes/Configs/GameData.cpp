//
//  GameData.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-16.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "GameData.h"
#include "BattleMissionConf.h"
#include "BattleUnitConf.h"
#include "MapUnitConf.h"

using namespace cocos2d;

BattleMissionConf* GameData::pBattleMissionConf = NULL;
AIConfig* GameData::pAIConfig = NULL;
ThrowAttackConfig* GameData::pThrowAttackConfig = NULL;
ItemConfig* GameData::pItemConfig = NULL;
PartnerUpgradeConf* GameData::pPartnerUpgradeConf = NULL;
ShopConf* GameData::pShopConf = NULL;
VipConf* GameData::pVipConf = NULL;
SwapItemConfig* GameData::pSwapItemConfig = NULL;
LVConf* GameData::pLVConf = NULL;
SkillConf* GameData::pSkillConf = NULL;
PartnerSkillConf* GameData::pPartnerSkillConf = NULL;
RoleAttrConf* GameData::pRoleAttrConf = NULL;
CommunitymapConf *GameData::pCommunitymapConf = NULL;
EquipUpgradeConfig *GameData::pEquipUpgradeConfig = NULL;
RoleNerveConfig *GameData::pRoleNerveConfig = NULL;
RoleForceConfig *GameData::pRoleForceConfig = NULL;
MonsterConf *GameData::pMonsterConf = NULL;
PartnerConfig *GameData::pPartnerConfig = NULL;
BattleSceneConf *GameData::pBattleSceneConf = NULL;
PrestigeChallengeConfig *GameData::pPrestigeChallengeConfig = NULL;
TaskConfig *GameData::pTaskConfig = NULL;
AwardTaskConfig *GameData::pAwardTaskConfig = NULL;
ActivityConf* GameData::pActivityConf = NULL;
HelpConf*   GameData::pHelpConf = NULL;
TutorialConfig* GameData::pTutorialConfig = NULL;
GiftItemConf *GameData::pGiftItemConf = NULL;

bool GameData::loadAllConf()
{
    bool ret = true;
    
#define LOAD_CONFIG_FROM_FILE(_CONFIG_NAME_, _file_, _bEncrypt_)\
    if (ret && !p##_CONFIG_NAME_)\
    {\
        p##_CONFIG_NAME_ = new _CONFIG_NAME_();\
        if (!p##_CONFIG_NAME_->loadFromFile((_file_), (_bEncrypt_)))\
        {\
            DEBUG_LOG("init %s config error", _file_);\
            ret = false;\
        }\
    }

#ifdef WONPEE_SERVER
    LOAD_CONFIG_FROM_FILE(BattleMissionConf, ((char*)"./conf/mission_config.xml"), false);
    LOAD_CONFIG_FROM_FILE(ThrowAttackConfig, ((char*)"./conf/throw_attack_config.xml"), false);
    LOAD_CONFIG_FROM_FILE(AIConfig, ((char*)"./conf/ai_config.xml"), false);
    LOAD_CONFIG_FROM_FILE(LVConf, ((char*)"./conf/level_config.xml"), false);
    LOAD_CONFIG_FROM_FILE(CommunitymapConf, ((char*)"./conf/community_config.xml"), false);
	LOAD_CONFIG_FROM_FILE(SkillConf, ((char*)"./conf/skill.xml"), false);
    LOAD_CONFIG_FROM_FILE(PartnerSkillConf, ((char*)"./conf/partner_skill.xml"), false);
    LOAD_CONFIG_FROM_FILE(RoleAttrConf, ((char*)"./conf/role_attr_config.xml"), false)
    LOAD_CONFIG_FROM_FILE(ItemConfig, ((char*)"./conf/item.xml"), false);
    LOAD_CONFIG_FROM_FILE(ShopConf, ((char*)"./conf/shop.xml"), false);
    LOAD_CONFIG_FROM_FILE(VipConf, ((char*)"./conf/vip_config.xml"), false);
    LOAD_CONFIG_FROM_FILE(SwapItemConfig, ((char*)"./conf/swap.xml"), false);
    LOAD_CONFIG_FROM_FILE(EquipUpgradeConfig, ((char*)"./conf/equip_upgrade.xml"), false);
    LOAD_CONFIG_FROM_FILE(RoleNerveConfig, ((char*)"./conf/nerve_config.xml"), false);
    LOAD_CONFIG_FROM_FILE(RoleForceConfig, ((char*)"./conf/role_force_config.xml"), false);
    LOAD_CONFIG_FROM_FILE(MonsterConf, ((char*)"./conf/monster_config.xml"), false);
    LOAD_CONFIG_FROM_FILE(PartnerConfig, ((char*)"./conf/partner.xml"), false);
    LOAD_CONFIG_FROM_FILE(BattleSceneConf, ((char*)"./conf/scence_config.xml"), false);
	LOAD_CONFIG_FROM_FILE(PrestigeChallengeConfig, ((char*)"./conf/prestige_challenge.xml"), false);
    LOAD_CONFIG_FROM_FILE(TaskConfig, ((char*)"./conf/tasks.xml"), false);
    LOAD_CONFIG_FROM_FILE(TutorialConfig, ((char*)"./conf/tutorial.xml"), false);
    LOAD_CONFIG_FROM_FILE(AwardTaskConfig, ((char*)"./conf/award_tasks.xml"), false);
    LOAD_CONFIG_FROM_FILE(MagicStoreConf, ((char*)"./conf/magic_store.xml"), false);
    LOAD_CONFIG_FROM_FILE(VipConf, ((char*)"./conf/vip_config.xml"), false);
    LOAD_CONFIG_FROM_FILE(ActivityConf, ((char*)"./conf/activity_config.xml"), false);
    LOAD_CONFIG_FROM_FILE(HelpConf, ((char*)"./conf/helper_conf.xml"), false);
    LOAD_CONFIG_FROM_FILE(GiftItemConf, ((char*)"conf/gift_items.xml"), false);
#else
    LOAD_CONFIG_FROM_FILE(BattleMissionConf, ((char*)"config/mission_config.xml"), false);
    LOAD_CONFIG_FROM_FILE(ThrowAttackConfig, ((char*)"config/throw_attack_config.xml"), false);
    LOAD_CONFIG_FROM_FILE(AIConfig, ((char*)"config/ai_config.xml"), false);
    LOAD_CONFIG_FROM_FILE(LVConf, ((char*)"config/level_config.xml"), false);
    LOAD_CONFIG_FROM_FILE(CommunitymapConf, ((char*)"config/community_config.xml"), false);
    LOAD_CONFIG_FROM_FILE(SkillConf, ((char*)"config/skill.xml"), false);
    LOAD_CONFIG_FROM_FILE(PartnerSkillConf, ((char*)"config/partner_skill.xml"), false);
    LOAD_CONFIG_FROM_FILE(RoleAttrConf, ((char*)"config/role_attr_config.xml"), false)
    LOAD_CONFIG_FROM_FILE(ItemConfig, ((char*)"config/item.xml"), false);
    LOAD_CONFIG_FROM_FILE(ShopConf, ((char*)"config/shop.xml"), false);
    LOAD_CONFIG_FROM_FILE(SwapItemConfig, ((char*)"config/swap.xml"), false);
    LOAD_CONFIG_FROM_FILE(EquipUpgradeConfig, ((char*)"config/equip_upgrade.xml"), false);
    LOAD_CONFIG_FROM_FILE(RoleNerveConfig, ((char*)"config/nerve_config.xml"), false);
    LOAD_CONFIG_FROM_FILE(RoleForceConfig, ((char*)"config/role_force_config.xml"), false);    
    LOAD_CONFIG_FROM_FILE(MonsterConf, ((char*)"config/monster_config.xml"), false);
    LOAD_CONFIG_FROM_FILE(PartnerConfig, ((char*)"config/partner.xml"), false);
    LOAD_CONFIG_FROM_FILE(BattleSceneConf, ((char*)"config/scence_config.xml"), false);
    LOAD_CONFIG_FROM_FILE(PrestigeChallengeConfig, ((char*)"config/prestige_challenge.xml"), false);
    LOAD_CONFIG_FROM_FILE(TaskConfig, ((char*)"config/tasks.xml"), false);
    LOAD_CONFIG_FROM_FILE(TutorialConfig, ((char*)"config/tutorial.xml"), false);
    LOAD_CONFIG_FROM_FILE(AwardTaskConfig, ((char*)"config/award_tasks.xml"), false);
    LOAD_CONFIG_FROM_FILE(VipConf, ((char*)"config/vip_config.xml"), false);
    LOAD_CONFIG_FROM_FILE(ActivityConf, ((char*)"config/activity_config.xml"), false);
    LOAD_CONFIG_FROM_FILE(HelpConf, ((char*)"config/helper_conf.xml"), false);
    LOAD_CONFIG_FROM_FILE(GiftItemConf, ((char*)"config/gift_items.xml"), false);
#endif
    
#undef LOAD_CONFIG_FROM_FILE
    return ret;
}

int GameData::getLV(uint64_t nExp)
{
    return pLVConf->getLevelFromExp(nExp);
}

int GameData::getLVLimit()
{
    return pLVConf->getLVLimit();
}

int GameData::getExpLimit()
{
    return pLVConf->getExpLimit();
}

uint64_t GameData::getNextLvExp(uint64_t nExp)
{
    return pLVConf->getNextLvExp(nExp);
}

uint64_t GameData::getLVExp(int lv)
{
    return pLVConf->getLVExp(lv);
}

float GameData::getExpPercent(uint64_t nExp)
{
    return pLVConf->getExpPercent(nExp);
}

int GameData::getVipFunction(int viplv,int FunctionID)
{
    return pVipConf->getVipFunction(viplv, FunctionID);
}

const item_t* GameData::getItem(uint32_t itemID)
{
    return pItemConfig->getItem(itemID);
}

const PartnerUpgradeItem_t* GameData::getPartnerUpgradeItem(uint32_t nLv)
{
    return pPartnerUpgradeConf->getItem(nLv);
}

const std::vector<ItemUnit_t>* GameData::getShopItem(uint32_t shopID)
{
    return pShopConf->getItemVec(shopID);
}

const SwapItem_t* GameData::getSwapItem(uint32_t swapID)
{
    return pSwapItemConfig->getSwapItem(swapID);
}

void GameData::getMonsterCountInfo(uint32_t nMissionID, MonsterCountInfo_t* pMonsterArray, uint32_t* pArrayCount)
{
    return pBattleMissionConf->getMonsterCountInfo(nMissionID, pMonsterArray, pArrayCount);
}

const AIJudgeTree* GameData::getAiJudgeTree(uint16_t treeID)
{
    return pAIConfig->getAIFromID(treeID);
}

ThrowAttackConfig_t* GameData::getThrowAttackConf(uint16_t throwAttackID)
{
    return pThrowAttackConfig->getThrowAttackFromID(throwAttackID);
}

const RoleAttrConf_t* GameData::getRoleAttrConf(int nLevel, int nRoleType)
{
    return pRoleAttrConf->getRoleAttrConf(nLevel, nRoleType);
}

const SkillLevel_t* GameData::getSkillConf(uint32_t nSkillID, uint32_t nSkillLvl)
{
    return pSkillConf->getSkillLevel(nSkillID, nSkillLvl);
}

const SkillConf_t* GameData::getSkillConf(uint32_t nSkill)
{
    return pSkillConf->getSkill(nSkill);
}

const PartnerSkillConf_t* GameData::getPartnerSkillConf(uint32_t nSkill)
{
    return pPartnerSkillConf->getPartnerSkill(nSkill);
}

const PartnerSkillUpgradeConf_t* GameData::getPartnerSkillUpgradeConf(uint32_t nSkill, uint32_t nLv)
{
    const PartnerSkillConf_t* pConf = pPartnerSkillConf->getPartnerSkill(nSkill);
	if (!pConf) {
		return NULL;
	}
	PartnerSkillUpgradeMap::const_iterator it = pConf->upgrdateConfMap.find(nLv);
	if (it != pConf->upgrdateConfMap.end()) {
		return it->second;
	}
	return NULL;
}

uint32_t GameData::getPartnerSkillHurt(uint32_t nSkill,uint32_t nLv)
{
    const PartnerSkillConf_t* skillConf = GameData::getPartnerSkillConf(nSkill);
    return uint32_t(skillConf->addHurt*(nLv/4.0 + 0.75)*(nLv/4.0 + 0.75));
}

bool GameData::isEnableUnlockSkill(int selectSkill,std::map<int,user_skill_t> *skill_map)
{
    return pSkillConf->isEnableUnlockSkill(selectSkill,skill_map);
}

const HeroSkillVec* GameData::getHeroSkillVec(uint32_t nUnitID)
{
    return pSkillConf->getHeroSkillVec(nUnitID);
}

const CommunitymapConf_t* GameData::getCmdMapConf(uint32_t nCmtMapID)
{
	return pCommunitymapConf->getCmdMapConf(nCmtMapID);
}

void GameData::getCmdMapConfByLevel(uint32_t nLevel, CmtMapConfVec& cmtVec)
{
	pCommunitymapConf->getCmdMapConfByLevel(nLevel, cmtVec);
}

CommunityNPC_t *GameData::getNpcConf(int npcID)
{
    if (pCommunitymapConf->cmtNPCMap.end() != pCommunitymapConf->cmtNPCMap.find(npcID))
    {
        return pCommunitymapConf->cmtNPCMap[npcID];
    }
    return NULL;
}

CmtNPCResource_t *GameData::getNpcResourceConf(int resourceID)
{
    if (pCommunitymapConf->cmtNPCResourceMap.end() != pCommunitymapConf->cmtNPCResourceMap.find(resourceID))
    {
        return pCommunitymapConf->cmtNPCResourceMap[resourceID];
    }
    return NULL;
}

const EquipUpgradeConf_t* GameData::getEquipmentUpgradeConf(uint32_t nequipmentID)
{
    return pEquipUpgradeConfig->getEquipUpgrade(nequipmentID);
}

const RoleNerveConf_t* GameData::getRoleNerve(uint32_t nNerveID)
{
    return pRoleNerveConfig->getRoleNerve(nNerveID);
}

int GameData::getRoleForceLevel(uint32_t nForceExp)
{
    return pRoleForceConfig->getRoleForceLevel(nForceExp);
}

const RoleForceConf_t* GameData::getRoleForceSum(uint32_t nForceLevel)
{
    return pRoleForceConfig->getRoleForceSum(nForceLevel);
}

RoleForceConf_t* GameData::getRoleForceConf(uint32_t nForceLevel, RoleForceConf_t* pConf)
{
    return pRoleForceConfig->getRoleForceConf(nForceLevel, pConf);
}



MonsterUnitConf* GameData::getMonsterConf(uint32_t nMonsterID)
{
    return pMonsterConf->getMonster(nMonsterID);
}

const PartnerConf_t* GameData::getPartnerConf(uint32_t nPartnerID)
{
	return pPartnerConfig->getPartnerConf(nPartnerID);
}

PartnerMap* GameData::getPartnerMap()
{
    return pPartnerConfig->getPartnerMap();
}

//得到弟子属性
const RoleAttrConf_t* GameData::getPartnerAttrConf(int nLevel, int nPartnerID, RoleAttrConf_t* pConf)
{
	const PartnerConf_t* pPartnerConf = GameData::getPartnerConf(nPartnerID);
	if (!pPartnerConf || pPartnerConf->nProfession == 0 || pPartnerConf->nProfession > 3) {
		return NULL;
	}
	const RoleAttrConf_t* pRole = GameData::getRoleAttrConf(nLevel, pPartnerConf->nProfession);
	/*
	pConf->nLife = (int)(pRole->nLife * 0.6 * pPartnerConf->fPartnerFac) + pPartnerConf->nHp;
	pConf->nOutAttack = (int)(pRole->nOutAttack * 1.2 * pPartnerConf->fPartnerFac) + pPartnerConf->nOutAtt;
	pConf->nInAttack = (int)(pRole->nInAttack * 1.2 * pPartnerConf->fPartnerFac) + pPartnerConf->nInAtt;
	pConf->nOutDef = (int)(pRole->nOutDef * 0.8 * pPartnerConf->fPartnerFac) + pPartnerConf->nOutDef;
	pConf->nInDef = (int)(pRole->nInDef * 0.8 * pPartnerConf->fPartnerFac) + pPartnerConf->nInDef;
	pConf->nCrit = (int)(pRole->nCrit * 2 * pPartnerConf->fPartnerFac);
	pConf->nToughness = (int)(pRole->nToughness * 2 * pPartnerConf->fPartnerFac);
	pConf->nReduceHurt = pRole->nReduceHurt;
	pConf->nAddHurt = pRole->nAddHurt;
	pConf->nRecoverHP = pRole->nRecoverHP;
	*/
	pConf->nLife = (int)(pRole->nLife * pPartnerConf->fPartnerFac) + pPartnerConf->nHp;
	pConf->nOutAttack = (int)(pRole->nOutAttack * pPartnerConf->fPartnerFac) + pPartnerConf->nOutAtt;
	pConf->nInAttack = (int)(pRole->nInAttack * pPartnerConf->fPartnerFac) + pPartnerConf->nInAtt;
	pConf->nOutDef = (int)(pRole->nOutDef * pPartnerConf->fPartnerFac) + pPartnerConf->nOutDef;
	pConf->nInDef = (int)(pRole->nInDef * pPartnerConf->fPartnerFac) + pPartnerConf->nInDef;
	pConf->nCrit = (int)(pRole->nCrit * pPartnerConf->fPartnerFac);
	pConf->nToughness = (int)(pRole->nToughness * pPartnerConf->fPartnerFac);
	pConf->nReduceHurt = (int)(pRole->nReduceHurt * pPartnerConf->fPartnerFac);
	pConf->nAddHurt = (int)(pRole->nAddHurt * pPartnerConf->fPartnerFac);
	pConf->nRecoverHP = (int)(pRole->nRecoverHP * pPartnerConf->fPartnerFac);
	
	pConf->roleTrainAttr.fMaxLife = pRole->roleTrainAttr.fMaxLife * pPartnerConf->fPartnerFac;
	pConf->roleTrainAttr.fMaxOutAttack = pRole->roleTrainAttr.fMaxOutAttack * pPartnerConf->fPartnerFac;
	pConf->roleTrainAttr.fMaxInAttack = pRole->roleTrainAttr.fMaxInAttack * pPartnerConf->fPartnerFac;
	pConf->roleTrainAttr.fMaxOutDef = pRole->roleTrainAttr.fMaxOutDef * pPartnerConf->fPartnerFac;
	pConf->roleTrainAttr.fMaxInDef = pRole->roleTrainAttr.fMaxInDef * pPartnerConf->fPartnerFac;
	pConf->roleTrainAttr.fMaxToughness = pRole->roleTrainAttr.fMaxToughness * pPartnerConf->fPartnerFac;
	return pConf;
}

//const PartnerSkillUpgradeItem_t* GameData::PartnerSkillUpgradeItem(uint32_t skill_id, uint32_t lv)
//{
//	;//const PartnerSkillUpgradeItem_t* conf = pPartnerSkillUpgradeConf->getItem(skill_id, lv);
//	return conf;
//}

SceneConf_t* GameData::getSceneConf(uint32_t nID)
{
    return pBattleSceneConf->getSceneConf(nID);
}

int GameData::getPrestigeChallengeCount()
{
    return pPrestigeChallengeConfig->getPrestigeTitleCount();
}

const PrestigeChallengeConf_t* GameData::getPrestigeChallenge(uint32_t nTitleID)
{
    return pPrestigeChallengeConfig->getPrestigeChallenge(nTitleID);
}

const PrestigeChallengeConf_t* GameData::getPrestigeChallengeByMissionID(uint32_t nMissionID)
{
    return pPrestigeChallengeConfig->getPrestigeChallengeByMissionID(nMissionID);
}

const TaskConfig_t* GameData::getTaskConf(uint32_t nTaskID)
{
	return pTaskConfig->getTaskConfig(nTaskID);
}

const AwardTaskConfig_t* GameData::getAwardTaskConfig(uint32_t nTaskID)
{
	return pAwardTaskConfig->getAwardTaskConfig(nTaskID);
}

const ActivityConf_t* GameData::getActivityConf(uint32_t nID)
{
    return pActivityConf->getActivityByID(nID);
}

const ActivityMap GameData::getActivityMap()
{
    return pActivityConf->m_ActivityMap;
}

#ifdef WONPEE_SERVER
MagicStoreConf *GameData::pMagicStoreConf = NULL;
int GameData::getRandStoreArray(uint32_t curLevel, storeItemInfo_t* pItemArray, uint32_t* arrayCount)
{
	return pMagicStoreConf->getRandStoreArray(curLevel, pItemArray, arrayCount);
}
#else

const TutorialConfig_t* GameData::getTutorialConfWithTaskID(uint32_t task_id)
{
    return pTutorialConfig->getTutorialConfWithTask(task_id);
}

const TutorialConfig_t* GameData::getTutorialConfWithLevel(uint32_t level)
{
    return pTutorialConfig->getTutorialConfWithLevel(level);
}

const TutorialConfig_t* GameData::getTutorialConfByStep(uint32_t step)
{
    return pTutorialConfig->getTutorialConfByStep(step);
}

const HelpConf_t* GameData::getHelpItemVec(uint32_t nID)
{
    return pHelpConf->getHelpItemVec(nID);
}

const HelpConf* GameData::getHelpConf()
{
    return pHelpConf;
}
#endif

