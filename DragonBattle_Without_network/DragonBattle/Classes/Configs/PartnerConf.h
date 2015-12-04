//
//  PartnerConf.h
//  DragonBattle
//
//
//

#ifndef __DragonBattle__PartnerConf__
#define __DragonBattle__PartnerConf__

#include <map>
#include <vector>

#include "XmlEncrypt.h"

// 天赋类型
enum {
	TALENT_DECR_ENEMY_INATK = 1,		//减少敌人内攻百分比
	TALENT_DECR_ENEMY_OUTATK = 2,		//减少敌人外攻百分比
	TALENT_DECR_ENEMY_INDEF = 3,		//减少敌人内防百分比
	TALENT_DECR_ENEMY_OUTDEF = 4,		//减少敌人外防百分比
	TALENT_DECR_ENEMY_HPMAX = 5,		//减少敌人生命上限百分比
	TALENT_INCR_MISSION_EXP = 6,		//增加关卡中经验收益百分比
	TALENT_INCR_MISSION_YXB = 7,		//增加关卡中银两收益百分比
	TALENT_INCR_MASTER_ENERY_MAX = 8,	//增加掌门体力上限百分比
	TALENT_INCR_MASTER_ANGER_MAX = 9,	//增加掌门怒气上限百分比
	TALENT_INCR_MASTER_SECKILL_RATE = 10,//使掌门人有xx%概率秒杀敌人
};

// 缘
struct PartnerRelate_t
{
	int      nRelatePartner[4];	//同时上阵弟子
	uint32_t nPartnerSkill;		//佩戴技能
	uint8_t	 nEffectType;		//影响属性值类型
	float	 fEffectValue;      //百分比
	char	 desc[128];
};

// 弟子
struct PartnerConf_t
{
	uint32_t nPartnerID;
	uint32_t nResourceID;
    uint32_t nItemID;       //对应卡牌ID
	char	 strName[32];	//弟子名字
	uint32_t nFriendVal;		//招募所需要的友好度
	uint32_t nNeedYxb;		//招募所需要的银两
	uint32_t nNeedGold;		//招募所需要的黄金
	uint32_t nNeedPrestige;	//招募所需要的声望
	uint8_t	 nProfession;	//职业
	uint8_t	 nQuarty;		//品质
    uint8_t  nTalent;       //天赋
    float    fTalentValue;  //天赋值
	float	 fPartnerFac;	//弟子系数
	float	 fAptitudeFac;	//资质系数
	uint32_t nHp;			//血量
	uint32_t nInAtt;		//内攻
	uint32_t nOutAtt;		//外攻
	uint32_t nInDef;		//内防
	uint32_t nOutDef;		//外防
	uint32_t nSkillID;	//侠客带的技能
	std::vector<PartnerRelate_t*> partnerRelateVec;
};

typedef std::map<uint32_t, PartnerConf_t*> PartnerMap;
class PartnerConfig
{
public:
    PartnerConfig();
    ~PartnerConfig();
    
    PartnerConf_t* getPartnerConf(uint32_t nPartnerID);
    PartnerMap* getPartnerMap();
    
    bool loadFromFile(char* filePath, bool bEncrypt = false);
    void unload();

private:
	PartnerMap		m_PartnerMap;
    uint32_t    	m_nCurItemIdx;
};

#endif /* defined(__DragonBattle__PartnerConf__) */
