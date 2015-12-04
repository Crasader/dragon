//
//  UserData.h
//  DragonBattle
//  缓存的用户数据，从后台拉取的
//  Created by 小敦 王 on 13-4-16.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_UserData_h
#define DragonBattle_UserData_h

#include "cocos2d.h"
#include <vector>

#include "GameData.h"
#include "OnceSetDefine.h"
#include "online_cli_proto_cmd.h"
#include "RoleData.h"
USING_NS_CC;
#define PACKAGE_CAPACITY (64)

#include "UserDataEx.h"

typedef struct messageBody {
	uint32_t nUserID;//发送者ID
    uint32_t nTime;//发送时间
    std::string nick;//昵称
	uint32_t nLevel;//等级
	uint32_t nUnitID;//3种职业，只能是1 2 3
	uint32_t nVipLevel;//VIP等级
	uint32_t nChannelID;//频道ID 1世界 2好友 3帮派 4群组 5地图
	uint32_t nTargetID;//接收对象根据频道进行区分 1是全部玩家 2是好友USERID  3帮派ID 4群组ID 5是地图ID<具体根据需要进行修改>
    
    std::string msg;
} messageBody_t;

class FindTarget:public CCObject //寻路
{
public:
    FindTarget(){};
    ~FindTarget(){};
    static FindTarget *create()
    {
        FindTarget *target = new FindTarget();
        target->nCommunityID = -1;
        target->nNPCID = -1;
        target->nMissionID = -1;
        target->autorelease();
        return target;
    }
    int nCommunityID;//目标主城
    int nNPCID;
    int nMissionID;
};

class UserData:public CCObject
{
public:
    UserData();
    ~UserData();
    static UserData* sharedInstance();
    bool resetData();
    bool initFromServer();
    
    static char* getShortNum(uint32_t nNum, char* str);
    CC_SYNTHESIZE(uint32_t, nPower, Power);  //功力值
    void addPower(int addPower);
    CC_SYNTHESIZE(uint32_t,yxb,yxb);//银两
    void addYXB(int addYXB);
    CC_SYNTHESIZE(uint32_t, nGold,Gold);//黄金数量
    void addGold(int addgold);
    CC_SYNTHESIZE(uint32_t, nPrestige, Prestige);//声望
    void addPrestige(int nAddPrestige);
    CC_SYNTHESIZE(uint32_t, nNerveValue, NerveValue);//真气数量
    void addNerve(int addNerve);
    CC_SYNTHESIZE(uint32_t, nStarStoneExp, StarStoneExp);//星石经验
    void addStarStoneExp(int starStoneexp);
    CC_PROPERTY(uint32_t,nEnery, Enery);//体力
    void addEnery(uint32_t addenery);
    CC_PROPERTY(uint32_t, nMaxEnery, MaxEnery);
    CC_PROPERTY(int,nEneryTime,EneryTime);//体力刷新时间
    void addExp(uint64_t addExp);
    CC_PROPERTY(uint32_t, nBuyEneryCount, BuyEneryCount);
    void addBuyEneryCount(int addCount);
    void refreshEnery(float ftime);
    CCArray *getAllRoleArray();
    CCArray *getBattleRoleArray();
    RoleData* getBattleRoleByID(uint32_t nRoleID);//获取上阵侠客
    RoleData* getRoleByID(uint32_t nRoleID);
    CCArray* getAllPartner(int unlessID  = -1);
    CCArray* getUnbattlePartner();
    void removePartnerByID(int unitID);
    void resetFindTarget();//重设寻路系统
    bool hasTarget();
    void getParnterTalent(float *talent);
    float getParnterTalentByID(int talentID);
public:
    FindTarget *m_findTarget;
    HeroData *m_heroData;
    UserDataEx* m_dataEx;
    CCDictionary *m_partnerDic;//上阵
    CCArray* m_arrPartner;//未上阵
    
    uint64_t nPreExp;            //用于显示之前的经验
    uint32_t server_id;
    uint32_t userid;            //用户ID
    char     nick[32];          //角色名
    CC_PROPERTY(uint32_t, nCostGold, CostGold);//已花费的黄金（用来算VIP等级）
    void resetBuyGold(uint32_t nGold);//当切换帐号时，不能用setBuyGold，而要用resetBuyGold
    CC_PROPERTY(uint32_t, nBuyGold, BuyGold);
    CC_SYNTHESIZE(uint16_t, nVipLevel, VipLevel);//vip等级
    
    uint32_t nTotalUserCount;
    uint32_t nLastCommunityID;      //上一次的主城ID
    uint32_t nWorldBossCommunityID; //世界Boss社区ID

    uint32_t package_item_sum;	//当前背包中物品数量
    uint32_t package_size;//背包格数
    uint32_t is_exists_invite;//是否填过了邀请码
    std::map<int, messageBody> GlobalMessages;
    std::map<int, messageBody> FriendMessages;
    std::map<int, int> NewFriendMessageCount;
    std::map<int,package_equipment_t> BagEquips;
    std::map<int,package_item_t> BagItems;
    std::map<int,package_item_t> BagCards;
    std::map<int,package_item_t> Bagstuff;
    
    std::map<int, int> OnceSetMap;
    
    int nMedicine;//带入药剂
    bool hiddenOtherUser;
    void setHiddenPlayer(bool hidden)
    {
        hiddenOtherUser = hidden;
        CCNotificationCenter::sharedNotificationCenter()->postNotification(CHANGEHIDDENPLAYER_NOTE);
    }
public:
    uint32_t getCurCommunityID();
    void setCurCommunityID(uint32_t curID, bool in_boss_community = false);
    
    bool strengthenEquipment(int nIndex, uint32_t nCost, bool bIsTeen);
    bool nerveUpgrade(int nAir, int nGold, bool bIsSuccess, int nAddLv);
    bool upgradeEquipment(int nIndex, uint32_t nPackageID, uint32_t nNewItemID, uint32_t nNewItemLevel);
    void setEquips(std::vector<package_equipment_t> equipment_vec);
    void setBagItems(std::vector<package_item_t> item_vec);
    void setBagCards(std::vector<package_item_t> card_vec);
    void setBagStuff(std::vector<package_item_t> stuff_vec);
    
    void reduce(std::map<int,package_item_t> &map,uint32_t package_id,uint32_t item_id,uint32_t item_cnt);
    void reduceBag(uint32_t package_id,uint32_t item_id,uint32_t item_cnt);
    
    void increase(std::map<int,package_item_t> &map, package_item_t item);
    void increaseBag(package_item_t itm);
    
    void addBagEquipItem(package_equipment_t bagItem);
    void removeBagEquipItem(int nindex);
    int getItemCount(uint32_t nItemID);
    bool existMedicine(int medID);
    
    void removeBagItemByID(int nItemID);
    int getLV()
    {
        return m_heroData->getLV();
    }
    
private:
    static UserData* pSharedInstace;
};

#define gUserData (UserData::sharedInstance())
#define gUserDataEx (UserData::sharedInstance()->m_dataEx)
#define SET_ONCE_SET_VALUE(msgid, value)    (gUserData->OnceSetMap[msgid] = value)
#define IS_ONCE_SET_VALUE(msgid)            (gUserData->OnceSetMap[msgid] == 1)

#endif
