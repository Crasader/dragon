//
//  ItemConf.h
//  前后台共用 item表
//

#ifndef DragonBattle_ItemConf_h
#define DragonBattle_ItemConf_h

#include <map>
#include <vector>

#include "XmlEncrypt.h"

#pragma pack(push)
#pragma pack(1)

typedef enum PlayerAttrItemType_t
{
	ITEM_YXB = 0,//银两
	ITEM_EXP = 1,//经验
	ITEM_GOLD = 2,//黄金
	ITEM_POWER = 3,//内功
	ITEM_NERVE = 4,//真气(用于升级经脉等级)
	ITEM_ENERGY = 5,//体力
	ITEM_PRESTIGE = 6,//声望
	ITEM_FORCE = 7,//练气
	ITEM_STAR = 8, //星石经验
} PlayerAttrItemType;

typedef enum EquipAttributeType_t
{
    EQUIP_ATTR_NONE,
    HP 			= 1,
    OUT_ATTACK 	= 2,
    OUT_DEFENCE = 3,
    IN_ATTACK 	= 4,
    IN_DEFENCE 	= 5,
    TOUGHNESS 	= 6,
    CRIT 		= 7,
    RECOVER_HP 	= 8,
    REDUCE_HURT = 9,
    ADD_HURT 	= 10,
	EQUIP_ATTR_MAX  = 10,
} EquipAttributeType;

enum kEquipmentType {
	EQUIPMENT_WEAPON = 1,	//武器
	EQUIPMENT_HAT	 = 2,	//帽子
	EQUIPMENT_CLOTHES= 3,	//衣服
	EQUIPMENT_SHOES  = 4,	//鞋子
	EQUIPMENT_CUFF	 = 5,	//护腕
	EQUIPMENT_RING	 = 6,	//戒指
	EQUIPMENT_NECKLACE = 7,	//项链
	EQUIPMENT_AMULET = 8,	//护符
	MAX_EQUIPMENT_TYPE = 8,	//装备的最大类型
};

enum kEquipmentSeriesID {
	EQUIP_SERIES_NORMAL = 0,	//普通系
	EQUIP_SERIES_ICE 	= 1,	//冰系
	EQUIP_SERIES_FIRE 	= 2,	//火系
};

//一些限制数量的宏
enum {
	EQUIP_GEM_NUM = 3, //装备镶嵌宝石数量
};

// 道具类型
typedef struct useItem
{
	uint32_t	nItemType;		//物品子类型
	uint32_t	nEffect;		//子类型的值
	uint32_t	nLastValue;		//持续时间值
    uint8_t     nUsed;          //是否可以使用
} useItem_t;

// 装备
typedef struct equipmentItem
{
	uint32_t nSeriesID;//系列
    uint32_t nItemType;//类型
	uint32_t nEffect;
	float	 fEquipFac;//系数
	uint32_t nBaseHp;	//生命值
	float	 fBaseAtk;	//外攻击力
	float	 fBaseInnerAtk;	//内攻击力
	float	 fBaseDef;	//外防御力
	float	 fBaseInnerDef;//内防御力
	float	 fBaseCrit;	//暴击值
	float	 fBaseDefCrit;//韧性
} equipmentItem_t;

// 合成书卷轴
typedef struct composeBookItem
{
	uint32_t nEffect;
} composeBookItem_t;

// 材料
typedef struct goodItem
{
	uint32_t fromUnit;
	uint32_t nItemType;		//物品子类型
	uint32_t nEffect;
	int EquipTypes[2];	//宝石可以镶嵌到的装备类型
} goodItem_t;

//秘籍
typedef struct skillItem
{
	uint32_t nEffect;
} skillItem_t;

//弟子卡牌
typedef struct cardItem
{
	uint32_t nPartnerID;	//对应的弟子ID
	uint32_t nEffect;		//兑换弟子对应的swap_id
} cardItem_t;

typedef struct starItem
{
	uint32_t nItemType;		//物品子类型
	uint32_t nEffect;		//计算升级经验的系数
	uint32_t nBaseExp;		//基础经验
	uint32_t nBaseHp;	//生命值
	uint32_t nBaseAtk;	//外攻击力
	uint32_t nBaseInnerAtk;	//内攻击力
	uint32_t nBaseDef;	//外防御力
	uint32_t nBaseInnerDef;//内防御力
	uint32_t nBaseCrit;	//暴击值
	uint32_t nBaseDefCrit;//韧性
    uint32_t nAddHurt;//加伤
    uint32_t nReduceHurt;//减伤
} starItem_t;

typedef struct item
{
    uint32_t    nID;			//物品ID
    char        name[64];		//名称
    char        desc[256];		//描述
	uint32_t	nProfession;	//使用职业
	uint32_t	nLevel;			//等级限制
	uint32_t	nQuality;		//品质（前端显示用）
	uint32_t 	nBuyPrice;		//购买价格
	uint32_t	nSellPrice;		//卖出价格
	uint32_t 	nBuyGold;		//金币购买价格
    uint32_t    nMax;			//叠加的最大值
    uint32_t    nMissionID;     //掉落关卡id
    union {
        useItem_t 			item;   	//道具
		goodItem_t			goods;		//材料
        composeBookItem_t 	book;		//合成卷制作书
		equipmentItem_t		equipment;	//装备
    	skillItem_t			partnerSkill;	//装备技能
		cardItem_t			card;		//弟子卡牌
		starItem_t			star;		//占星的宝石
	} u;
} item_t;

typedef std::map<uint32_t, item_t*> ItemMap;

struct ItemUnit_t
{
	uint32_t nItemID;
	uint32_t nItemCount;
};

struct itemDrop_t
{
    uint32_t nItemID;
    uint32_t nItemCnt;
    float    fRate;
};

#pragma pack(pop)

enum kItemKind
{
    kAttributeValue = 0,
    kBattleItemKind = 1,    //道具
    kGoodsKind = 2,         //材料
    kComposeBookKind = 3,   //合成卷轴
    kEquipmentKind = 4,     //装备
    kPartnerSkillKind = 5,	//侠客技能
	kPartnerCardKind  = 6,	//弟子卡牌
	kTemporaryItemKind = 10,//临时物品
	kStarItemKind = 11,		//占星石
	kItemKindMax,
};

enum kItemType{
	//宝石类型, 与EquipAttributeType_t对应
	kEquipmentGemTypeMin = 101, 	//装备镶嵌宝石类型最小值
	kEquipmentGemTypeHp = 101, 		//装备镶嵌生命宝石
	kEquipmentGemTypeOutAtk = 102, 	//装备镶嵌外攻宝石
	kEquipmentGemTypeOutDef = 103, 	//装备镶嵌外防宝石
	kEquipmentGemTypeInAtk = 104, 	//装备镶嵌内攻宝石
	kEquipmentGemTypeInDef = 105, 	//装备镶嵌内防宝石
	kEquipmentGemTypeTough = 106, 	//装备镶嵌韧性宝石
	kEquipmentGemTypeCrit = 107, 	//装备镶嵌奖暴击宝石
	kEquipmentGemTypeRecoverHp = 108, 	//装备镶嵌回血宝石(预留,暂无)
	kEquipmentGemTypeReduceHurt = 109, 	//装备镶嵌减伤宝石(预留,暂无)
	kEquipmentGemTypeAddHurt = 110, 	//装备镶嵌加伤宝石(预留,暂无)
	kEquipmentGemTypeMax = 110, 		//装备镶嵌宝石类型最大值

	kPlayerExpPillType = 202, //经验丹
	kPartnerExpPillType = 204, //弟子经验丹
	kHpRecoverBottleType = 304, //血瓶
	kExpSeedType = 305, //经验种子
	kNerveAirSeedType = 306, //真气种子
	kPowerSeedType = 307, //内功种子
    kEquipmentBook = 401, //卷轴
};

#define getKindOfItem(itemId) ((itemId) / 10000)
#define isKindOfItem(itemId, itemKind)  (getKindOfItem(itemId) == (itemKind))
#define ITEM_KIND_MAX   20
#define getItemOvrlaySum(item_id) \
		(isKindOfItem(item_id, kGoodsKind) ? 60 : 1)
#define getEquipmentState(equipment)	(equipment->nLevel / 10 + 1) //获取装备的阶级
//#define canInlayGemOnEquipment(equipConfig) (getEquipmentState(equipConfig) > 1) //进阶过才开启宝石镶嵌
#define canInlayGemOnEquipment(equipConfig) (1) //不需要进阶

#define isGemItemType(itemType) ((itemType) >= kEquipmentGemTypeMin && (itemType) <= kEquipmentGemTypeMax)
#define GEM_LEVEL_MAX 10	//宝石最大等级
#define GEM_TYPE_NUM 7
#define getEquipGemType(GemItemType) ((GemItemType) % 100) //取末两位上的数字


class ItemConfig
{
public:
    ItemConfig();
    ~ItemConfig();
    
    const item_t* getItem(uint32_t itemID) const;

	const item_t* getPreLvGem(uint32_t GemId);
	const item_t* getNextLvGem(uint32_t GemId);

#ifdef WONPEE_SERVER
	const item_t* getRandPartnerCard(uint32_t quarty);
#endif
    bool loadFromFile(char* filePath, bool bEncrypt = false);
    void unload();

private:
    bool loadItems(xmlNodePtr chl);
    
private:
	ItemMap		m_itemMap;
    uint32_t    m_nCurItemIdx;
	std::vector<uint32_t> GemIdVec;
};

#endif
