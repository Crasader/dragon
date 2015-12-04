//
//  SkillConf.h
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-19.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_SkillConf_h
#define DragonBattle_SkillConf_h

#ifndef WONPEE_SERVER
#include "online_cli_proto_cmd.h"
#else
#include "../../proto/online_cli_proto_cmd.h"
#endif
#include "XmlEncrypt.h"

#include <map>
#include <vector>

#define MAX_SKILL_LEVEL 300
#define MAX_PRESKILL_NUM 2

#define SKILL_IDX_MOD_BASE 1000	//技能id的千位代表角色职业，末三位代表技能编号(实际只用到末两位)
#define PASSIVE_SKILL_IDX_BEG 8	//被动技能(心法)的起始编号
#define GET_SKILL_IDX(skill_id)	((skill_id) % SKILL_IDX_MOD_BASE)
#define MERGE_SKILL_ID(unit_id, skill_idx)	((unit_id)*SKILL_IDX_MOD_BASE + skill_idx)
#define isPassiveSkillID(skill_id) (GET_SKILL_IDX(skill_id) >= PASSIVE_SKILL_IDX_BEG)
 
#define SKILL_LV_MOD_BASE	1000 //获取技能等级时求余的基数
#define MERGE_SKILL_ID_LV(id, lv)	((id)*SKILL_LV_MOD_BASE + (lv))
#define GET_SKILL_ID(skill)     ((skill) / SKILL_LV_MOD_BASE)
#define GET_SKILL_LEVEL(skill)  ((skill) % SKILL_LV_MOD_BASE)

struct SkillLevelBase_t
{
    uint8_t     nSkillLevel;    //等级
    float       fCDTime;
    int         nCostEnergy;    //消耗内力
    int         nFightValue;    //战斗力
    bool        bBuff;          //是否buff技能
    char        skillDesc[256]; //描述
    int         nInAttackValue;
    float       fInAttackPrecent;
    int         nOutAttackValue;
    float       fOutAttackPrecent;
    int         nInDefValue;
    float       fInDefPrecent;
    int         nOutDefValue;
    float       fOutDefPrecent;
    int         nAddhurtValue;
    float       fAddhurtPrecent;
    int         nReducehurtValue;
    float       fReducehurtPrecent;
    int         nHPValue;
    int         nToughness;
    int        	nCrit;
};

struct SkillBaseConf_t
{
    uint32_t    nSkillID;       //技能id
    char        skillName[64];
    char        skillDesc[256]; //描述
    int         nHeroLevel;     //学习所需的英雄等级
    uint32_t	nTaskID;     	//学习所需完成的任务
};

struct SkillLevel_t
{
    int         nCostPower;     //学习或升级所需的功力数量
    int         nYxb;           //消耗银两
    SkillLevelBase_t skillLvBase;
};

struct SkillConf_t
{
    SkillBaseConf_t skillBase;
    uint32_t    nRoleType;      //绑定的英雄类型
	uint16_t	nMaxLevel;
    int         nPreSkill[MAX_PRESKILL_NUM];   //所需的前置等级
    SkillLevel_t skillLevel[MAX_SKILL_LEVEL];
};

struct PartnerSkillUpgradeConf_t
{
	uint32_t lv;
	uint32_t rate;
	uint32_t cost_yxb;
	uint32_t item_id;
	uint32_t item_cnt;
};

typedef std::map<uint32_t, PartnerSkillUpgradeConf_t*> PartnerSkillUpgradeMap;

struct PartnerSkillConf_t
{
    SkillBaseConf_t skillBase;
    int relateHurt;//缘伤害
    int addHurt;
    int quarty;
    int nThrowSequence;//1间隔抛出多个 2同时抛出多个
    float bescper;//技能触发概率
    
    std::vector<uint16_t>  throwVec;//抛出物
    std::vector<std::string> fileResourceVec;//资源文件
	PartnerSkillUpgradeMap upgrdateConfMap;//升级信息
};

typedef std::map<uint32_t, SkillConf_t*> SkillConfMap;//英雄技能
typedef std::map<uint32_t, PartnerSkillConf_t*> PartnerSkillConfMap;//伙伴技能

typedef std::vector<uint32_t> HeroSkillVec;

bool parseSkilllv(SkillLevelBase_t *pLevel,xmlNodePtr chl);
bool parseSkill(SkillBaseConf_t *pSkill,xmlNodePtr cur);

class SkillConf
{
public:
    SkillConf();
    ~SkillConf();
    
    bool loadFromFile(const char* fileName, bool bEncrypt = false);
    void unload();
    
    const SkillLevel_t* getSkillLevel(uint32_t nSkillID, uint32_t skillLevel);
    const SkillConf_t* getSkill(uint32_t nSkill);
    const HeroSkillVec* getHeroSkillVec(uint32_t nUnitID) { return &m_heroSkillVec[nUnitID - 1]; }
    bool isEnableUnlockSkill(int selectSkill,std::map<int,user_skill_t> *skill_map);
private:
    SkillConfMap m_skillMap;
    HeroSkillVec m_heroSkillVec[3]; //3个英雄
};

class PartnerSkillConf
{
public:
    PartnerSkillConf();
    ~PartnerSkillConf();
    
    bool loadFromFile(const char* fileName, bool bEncrypt = false);
    void unload();
    bool loadUnitSkillResource(xmlNodePtr chl,std::vector<std::string>* pFileVec);
	bool loadUnitSkillLevel(xmlNodePtr chl,std::map<uint32_t, PartnerSkillUpgradeConf_t*>* pupgrdateConfMap);
    const PartnerSkillConf_t* getPartnerSkill(uint32_t nSkill);
private:
    PartnerSkillConfMap m_skillMap;
};
#endif
