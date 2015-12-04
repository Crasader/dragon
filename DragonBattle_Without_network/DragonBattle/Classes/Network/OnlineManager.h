//
//  OnlineManager.h
//  MyGame
//
//  Created by 小敦 王 on 12-10-19.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef MyGame_OnlineManager_h
#define MyGame_OnlineManager_h

#include "cocos2d.h"
#include "CMDProcessor.h"
#include "online_cli_proto_cmd.h"
#include "btl_head.hpp"
#include "UserData.h"

enum
{
    ONLINE_TIMEOUT_SEC = 15,
    RECONNECT_TO_ONLINE_SEC = 30,
};

enum ENUM_CONNECT_ONLINE_REASON
{
    E_USER_FIRST_CONNECT_ONLINE = 1,
    E_REASON_CLOSE_RECONNECT,       //连接过online, 断开后重新连接
    E_REASON_DEAL_OHTER_PROTO,      //需要处理其他协议连接online
};

enum REQUEST_PROTO502_REASON {
    GET_HAMMER_COUNT,
    GET_PILL_COUNT,
    GET_KEY_COUNT,
    GET_MEDICINE_COUNT,
    GET_ITEM_FROM_SHOP,
    GET_PARTNERSKILL_ITEM_COUNT,
    GET_DIAMOND_COUNT
};

class MsgCache
{
    int cmd;
    Cmessage pMsg;
};

class OnlineManager : public cocos2d::CCNode
{
public:
    OnlineManager();
    virtual ~OnlineManager();
    
    static OnlineManager* sharedManager();
    static void purgeManager();
    virtual bool init();
    void heartbeat(float);//心跳
    //online命令超时
    int onOnlineTimeOut(float t);
    //connect网络超时
    void onNetConnectTimeOut(float t);
        
    void onEnterLoginLayer(cocos2d::CCNode* pNode);
    void onReconnectOnline(cocos2d::CCNode* p);
    
    bool handleErrCmd(int cmd);
    void execWantedTask(int cmd);
    int sendDataToOnline(int seqNum, int cmd, Cmessage* pMsg, uint32_t userid = 0);

    void setOnlineSession(uint32_t svrTime, const char* session);
    
    // 处理online返回的信息
    int process_Online_CMD(char* data, int len);
public:
    //PROTO_USER_LOGIN_CMD:
    //用户登录包，校验session, cmdid 500
    int userLoginToOnline();
    void userLoginToOnlineCbk(cli_proto_head_t* head, byte_array_t& ba);
    
    //PROTO_USER_LOGIN_EX_CMD			= 601,	//用户登录后，紧跟着拉去任务、在线礼包等数据
    int userLoginExToOnline();
    void userLoginExToOnlineCbk(cli_proto_head_t* head, byte_array_t& ba);
    
    //PROTO_KICK_USER_OFFLINE_CMD = 499 用户在其他设备上登录
    void kickUserOfflineCbk(cli_proto_head_t* head, byte_array_t& ba);
    
    //PROTO_USER_KEEP_ALIVE_CMD		= 505,	//前端心跳
    int userKeepAlive();
    void userKeepAliveCbk(int ret, byte_array_t& ba);
    
    //PROTO_USER_CHAT_CMD          = 630,//客户端发送聊天消息到服务器
    int userChatToOnline(int nChannelId, int nTargetId, const char* pMsg);
    void userChatToOnlineCbk(byte_array_t& ba);
    
    //PROTO_USER_USE_ITEM_CMD           = 503,  //使用道具
    int userUseItem(uint32_t packageId, uint32_t itemId);
    void userUseItemCbk(cli_proto_head_t* head, byte_array_t& ba);
    
    //PROOT_USER_LOGIN_SIGN_CMD		= 621,		//签到,sign_type = 0是签当天的 1是补签
    int userLoginSignInToOnline(uint32_t nSignType);
    void userLoginSignInToOnlineCbk(byte_array_t& ba);
    
    int userGetItemListCountToOnline(std::vector<uint32_t>* pItemVec, int nReason);
    void userGetItemListCountToOnlineCbk(byte_array_t& ba);
    
    //PROTO_USER_GET_MAGIC_STORE_CMD	= 610,	//获得神秘商店
    int userGetMagicStoreToOnline();
    void userGetMagicStoreToOnlineCbk(byte_array_t& ba);
    
    //PROTO_USER_BUY_MAGIC_ITEM_CMD	= 611,	//购买神秘商店中的物品
    int userBuyMagicItemToOnline(uint32_t nItemIndex, uint32_t nCostYxb);
    void userBuyMagicItemToOnlineCbk(byte_array_t& ba);
    
    //PROTO_USER_FLUSH_MAGIC_STORE_CMD= 612,	//刷新神秘商店
    int userFlushMagicStoreToOnline();
    void userFlushMagicStoreToOnlineCbk(byte_array_t& ba);
    
    int userGetPackageToOnline();
    void userGetPackageToOnlineCbk(byte_array_t& ba);
    
    int userExpandPackageToOnline(int index);
    void userExpandPackageToOnlineCbk(byte_array_t& ba);
    
    int userGetMissionToOnline(int communitID);
    void userGetMissionToOnlineCbk(byte_array_t& ba);
    
    int userGetHardMissionToOnline(int communitID);
    void userGetHardMissionToOnlineCbk(byte_array_t& ba);
    
    int userBuyEneryToOnline(int money);
    void userBuyEneryToOnlineCbk(byte_array_t& ba);
    
    int userRecoverEneryToOnline();
    void userRecoverEneryToOnlineCbk(byte_array_t& ba);
    
    int userSellItemToOnline(uint32_t package_id,uint32_t item_id,uint32_t item_cnt);
    void userSellItemToOnlineCbk(byte_array_t& ba);
    
    int userGetSkillToOnline(uint32_t user_id);
    void userGetSkillToOnlineCbk(byte_array_t& ba);
    
    int userUpgradeSkillToOnline(uint32_t skill_id,uint32_t skill_level);
    void userUpgradeSkillToOnlineCbk(byte_array_t& ba);
    
    int userChangeSkillToOnline(uint32_t skill_id,uint32_t wear_seat);
    void userChangeSkillToOnlineCbk(byte_array_t& ba);
    
    int userBuyItemToOnline(uint32_t itemid,uint32_t itemcnt);
    void userBuyItemToOnlineCbk(byte_array_t& ba);
    
    int userTrainToOnline(uint32_t unitID,uint32_t traintype);
    void userTrainToOnlineCbk(byte_array_t& ba);
    
    int userTrainConfirmToOnline();
    void userTrainConfirmToOnlineCbk(byte_array_t& ba);
    
    int equipmentChangeToOnline(uint32_t unit_id, uint32_t packid,uint32_t itemid,uint32_t iswear);
    void equipmentChangeToOnlineCbk(byte_array_t& ba);
    
    int equipmentWearToOnline(uint32_t itemid);
    void equipmentWearToOnlineCbk(byte_array_t& ba);
    
    //PROTO_USER_PUNCH_EQUIPMENT_CMD	= 524,	//装备打孔
    int userPunchEquipment(int package_id, int item_id, int position);
    void userPunchEquipmentCbk(byte_array_t& ba);
    
    //PROTO_USER_INLAY_GEM_CMD		= 525,	//装备镶嵌宝石
    int userInlayGem(uint32_t op_type, uint32_t package_id, uint32_t item_id, uint32_t position, uint32_t gem_id);
    void userInlayGemCbk(byte_array_t& ba);
    
    //PROTO_USER_COMPOSE_GEM_CMD		= 526,	//宝石合成
    int userComposeGem(uint32_t gem_id);
    void userComposeGemCbk(byte_array_t& ba);
    
    //PROTO_USER_GET_ATTR_ITEM_CMD	= 600,	//得到itemid类型为0的属性值
    int getUserAttrToOnline();
    void getUserAttrToOnlineCbk(byte_array_t& ba);
    
    //PROTO_USER_GET_BATTLE_PRIZE_CMD   = 549,  //翻牌获得奖励
    int getCardPrizeToOnline(uint32_t nMapID, uint32_t nCostGold, uint32_t nCardState);
    void getCardPrizeToOnlineCbk(byte_array_t& ba);
    
    //PROTO_USER_BATTLE_OVER_CMD		= 548,	//关卡战斗结束
    int battleOverToOnline(uint32_t nMapID, uint32_t nOverReason, uint32_t* pKillMonster, uint32_t nAddYxb, uint32_t nAddExp, uint32_t nAddGold, uint32_t nMedicine, std::map<int, int>* pDieMonsterMap, std::map<int, int>* pDropItemMap, std::map<int, int>* pThiefItemMap, uint32_t nMaxCombo, uint32_t nBeHitCount, uint32_t hp_remain_rate);
    void battleOverCbk(byte_array_t& ba);
    
    //PROTO_USER_STRENGTHEN_EQUIPMENT_CMD:
    int equipmentStrengthenToOnline(uint32_t nPackageId, uint32_t nItemID, bool bIsTeen);
    void equipmentStrengthenToOnlineCbk(byte_array_t& ba);
    
    //PROTO_USER_CLEAR_COOLDOWN_CMD
    int cancelCoolDownToOnline(uint32_t nType, uint32_t nGold);
    void cancelCoolDownToOnlineCbk(byte_array_t& ba);
    
    //PROTO_USER_GET_COOLDOWN_CMD = 532, //得到某类型的CD时间
    int getCDTimeToOnline(uint32_t nType);
    void getCDTimeToOnlineCbk(byte_array_t& ba);
    
    //PROTO_USER_GET_TYPE_EVENT_CMD	= 536,	//得到某事件的完成次数
    int getEventCountToOnline(uint32_t nType);
    void getEventCountToOnlineCbk(byte_array_t& ba);
    
    //PROTO_USER_GET_TYPE_EVENT_CMD	= 536,	//得到某事件的完成次数    
    int getDailyEvent(uint32_t nType,uint32_t nReason = 0);
    void getDailyEventCbk(byte_array_t& ba);
    
    //PROTO_USER_BUY_HAMMER_CMD = 567,  //购买用于砸罐子的锤子
    int userBuyHammer(int nCostGold);
    void userBuyHammerCbk(byte_array_t& ba);
    
    //PROTO_USER_GET_USER_ONLINE_STATUS_CMD
    int userGetUsersOnlineStatus(std::vector<uint32_t>& uid_vec);
    void userGetUsersOnlineStatusCbk(byte_array_t& ba);
    
    //PROTO_USER_GET_FRIEND_LIST_CMD  = 670, //拉取好友信息
    int userGetFriendListToOnline(int nType);
    void userGetFriendListToOnlineCbk(byte_array_t& ba);
    
    //PROTO_USER_ADD_FRIEND_CMD       = 671, //用户添加好友
    int userAddFriendToOnline(int nServerId, int nId, bool bIsBlack);
    void userAddFriendToOnlineCbk(byte_array_t& ba);
    
    //PROTO_USER_DEAL_FRIEND_REQUEST_CMD = 672,//用户处理好友请求
    int userRefuseFriendRequest(std::vector<int>& vec);
    int userDealFriendRequestToOnline(int nFriendId, int nOpt);
    void userDealFriendRequestToOnlineCbk(byte_array_t& ba);
    
    //PROTO_USER_DEL_FRIEND_CMD       = 673, //用户删除好友
    int userDelFriendToOnline(int nServerId, int nFriendId, bool bIsBlack);
    void userDelFriendToOnlineCbk(byte_array_t& ba);
    
    //PROTO_USER_FIND_USER_FROM_NICK_CMD = 675,//通过nick找到用户基本信息
    int userFindUserFromNickToOnline(const char* pName);
    void userFindUserFromNickToOnlineCbk(byte_array_t& ba);
    
    //PROTO_USER_GET_PLATFORM_BONUS_CMD = 620,	//领取奖励平台的奖励
    int userGetPlatformBonusToOnline(uint32_t nBonusType, uint32_t nBonusIndex);
    void userGetPlatformBonusToOnlineCbk(int ret, byte_array_t& ba);
    
    int  userActionToOnline(float posx,float posy, int actionType, int communityID = 0);
    void userActionToOnlineCbk(byte_array_t& ba);
    
    //PROTO_USER_GET_JAR_LIST_CMD     = 563,  //得到寻宝的罐子的列表
    int getPotListToOnline();
    void getPotListToOnlineCbk(byte_array_t& ba);
    
    //PROTO_USER_BREAK_JAR_CMD        = 565,  //砸罐子
    int potClickToOnline(int nInfo);
    void potClickToOnlineCbk(byte_array_t& ba);
    
    //PROTO_USER_UPGRADE_JAR_CMD      = 564,  //罐子升级
    int potUpgradeToOnline(int nType);
    void potUpgradeToOnlineCbk(byte_array_t& ba);
    
    //PROTO_USER_UPGRADE_EQUIPMENT_CMD:
    int equipmentUpgradeToOnline(uint32_t nServerItemID, uint32_t nItemId, uint32_t nCostGold);
    void equipmentUpgradeToOnlineCbk(byte_array_t& ba);
    //PROTO_USER_TRAIN_EQUIPMENT_CMD:
    int equipmentWashToOnline(uint32_t nTrainType, uint32_t nPackageId, uint32_t nEquipmentId);
    void equipmentWashToOnlineCbk(byte_array_t& ba);
    
    //PROTO_USER_UPGRADE_NERVE_CMD 1普通 2黄金
    int nerveUpgradeToOnline(uint32_t nType, uint32_t nLevel, bool bIsBatch);
	void nerveUpgradeToOnlineCbk(byte_array_t& ba);

    //PROTO_USER_UPGRADE_FORCE_CMD  	= 530,  //练气
    int userUpgradeForce(bool isMedicine, bool isMul, uint32_t nCount);
    void userUpgradeForceCbk(byte_array_t& ba);
    
    //PROTO_USER_START_AUTO_BATTLE_CMD= 543,	//开始扫图
    int userStartAutoBattle(uint32_t mapId, uint32_t battleTimes);
    void userStartAutoBattleCbk(byte_array_t& ba);
    
    int userStopAutoBattle();
    void userStopAutoBattleCbk();
    
    int userGetRoundsBattle();//扫图获得
    void userGetRoundsBattleCbk(byte_array_t& ba);
    
    int userSpeedAutoBattle(uint32_t costGold);//加速扫图
    void userSpeedAutoBattleCbk(byte_array_t& ba);
    
    //PROTO_USER_BATTLE_START_CMD		= 547,	//进入关卡
    int userBattleStart(uint32_t nMapID);
    void userBattleStartCbk(byte_array_t& ba);

    //PROTO_USER_CHEAT_ITEM_CMD = 1000
    int userCheatItem(uint32_t item_id, int32_t item_cnt);
    void userCheatItemCbk(byte_array_t& ba);
    
    //  PROTO_USER_SWAP_CMD             = 506,  //兑换或随机获取物品
    int userSwap(uint32_t swap_id);
    void userSwapCbk(byte_array_t& ba);
    
    //PROTO_USER_BALLOT_CMD			= 550,	//藏经阁抽签(确定或抽签)
    int userBallot(uint32_t ballot_type);
    void userBallotCbk(byte_array_t& ba);
    
    //PROTO_USER_GET_ALL_BATTLE_PRIZE_CMD	= 551,	//获得所有老虎机的奖励
    int userGetAllBattlePrize();
    void userGetAllBattlePrizeCbk(byte_array_t& ba);
    
    //PROTO_USER_ENLIST_PARTNER_CMD	= 570,	//招募侠客
    int userEnlistPartner(uint32_t nPartnerID);
    void userEnlistPartnerCbk(byte_array_t& ba);
    
	//PROTO_USER_GET_PARTNER_LIST_CMD	= 571,	//侠客列表
    int usergetPartnerList(uint32_t type);
    void usergetPartnerListCbk(byte_array_t& ba);
    
    int userOnPlayerPartner(uint32_t partner_id,uint32_t seat_index);
    void userOnPlayerPartnerCbk(byte_array_t& ba);
    
    int userSwapPartnerSeat(uint32_t seat_idx_1,uint32_t seat_idx_2);
    void userSwapPartnerSeatCbk(byte_array_t& ba);
    
    int userUpgradePartnerSkill(uint32_t partnerID);
    void userUpgradePartnerSkillCbk(byte_array_t& ba);
    
    //PROTO_USER_GET_PRESTIGE_TITLE_CMD	= 579,	//得到声望挑战信息
    int userGetPrestigeTitle();
    void userGetPrestigeTitleCbk(byte_array_t& ba);
    
	//PROTO_USER_GET_PRESTIGE_PRIZE_CMD	= 580,	//领取
    int userGetPrestigePrize();
    void userGetPrestigePrizeCbk(byte_array_t& ba);
    
	//PROTO_USER_PRESTIGE_LOTTERY_CMD	= 581,	//声望摇奖
    int userPrestigeLottery(uint32_t nLotteryType, bool isUseGold);
    void userPrestigeLotteryCbk(byte_array_t& ba);
    //PROTO_USER_PARTNER_EAT_EXP_CMD	= 577,//吃经验丹
    int userPartnerEatExp(uint32_t unit_id,uint32_t unit_cnt,uint32_t itme_id);
    void userPartnerEatExpCbk(byte_array_t& ba);
    //PROTO_USER_GET_TASK_LIST_CMD
    int userGetTaskList();
    void userGetTaskListCbk(byte_array_t& ba);
    
    int userAcceptTask(uint32_t task_id);
    void userAcceptTaskCbk(byte_array_t& ba);
    
    int userFinishTask(uint32_t task_id,uint32_t use_gold);
    void userFinishTaskCbk(byte_array_t& ba);
    
    int userSetTask(uint32_t task_id,uint32_t add_step);
    void userSetTaskCbk(byte_array_t& ba);
    
    int userDropTask(uint32_t task_id);
    void userDropTaskCbk(byte_array_t& ba);
    
//    PROTO_USER_GET_AWARD_TASK_CMD		= 591,	//拉取悬赏任务
    int userGetWantedTaskList();
    void userGetWantedTaskListCbk(byte_array_t& ba);
    
    //PROTO_USER_ACCEPT_AWARD_TASK_CMD 	= 592,	//接取悬赏任务
    int userAcceptWantedTask(uint32_t task_id);
    void userAcceptWantedTaskCbk(byte_array_t& ba);
    
    //PROTO_USER_UPGRADE_AWARD_TASK_CMD 	= 593,	//一件满星
    int userUpgradeWantedTask(uint32_t task_id);
    void userUpgradeWantedTaskCbk(byte_array_t& ba);
    
    //PROTO_USER_SET_AWARD_TASK_FINIS_CMD = 594,	//一键完成
    int userSetWantedTaskFinish(uint32_t task_id);
    void userSetWantedTaskFinishCbk(byte_array_t& ba);
    
    //PROTO_USER_FINISH_AWARD_TASK_CMD	= 595,	//完成后提交任务
    int userFinishWantedTask(uint32_t task_id, uint32_t item_id = 0);
    void userFinishWantedTaskCbk(byte_array_t& ba);
    
    //PROTO_USER_REFRESH_AWARD_TASK_CMD	= 596,	//刷新悬赏任务
    int userRefreshWantedTask();
    void userRefreshWantedTaskCbk(byte_array_t& ba);
    
    //PROTO_USER_GET_ANENA_INFO_CMD	= 641,	//得到竞技场信息
    int userGetArenaInfo();
    void userGetArenaInfoCbk(byte_array_t& ba);
    
    //PROTO_USER_GET_ARENA_TOP_100_CMD= 642,	//得到竞技场top100
    int userGetArenaTop100(uint32_t curpage);
    void userGetArenaTop100Cbk(byte_array_t& ba);
    
    //PROTO_USER_SWAP_PARTNER_CARD_CMD= 557,	//兑换弟子
    int userSwapPartnerCard(int nTargetId, std::vector<item_unit_t>* pCardVec);
    void userSwapPartnerCardCbk(byte_array_t& ba);
    
    //PROTO_USER_INHERTIT_PARTNER_CMD = 558,	//弟子传承
    int userInheritPartner(int nPartnerId, int nTargetId, int nUseGold);
    void userInheritPartnerCbk(byte_array_t& ba);
    //PROTO_USER_OFFPLAY_PARTNER_CMD	= 573,	//侠客下阵
    int userOffPlayPartner(uint32_t seatID);
    void userOffPlayPartnerCbk(byte_array_t& ba);
    
    int userWorshipTop(uint32_t rankid);
    void userWorshipTopCbk(byte_array_t& ba);
    
    //PROTO_USER_USE_PARTNER_CARD_CMD= 556,	//使用弟子卡牌
    int userUsePartnerCard(int nCardId, int nPackageId);
    void userUsePartnerCardCbk(byte_array_t& ba);
    //PROTO_USER_GET_ARENA_FILE_CMD	= 643,	//得到竞技场战报
    int userGetArenaFile();
    void userGetArenaFileCbk(byte_array_t& ba);
    
    //PROTO_USER_GET_OTHER_USER_CMD	= 640,	//得到其他用户信息
    int userGetOtherUser(uint32_t userid);
    void userGetOtherUserCbk(byte_array_t& ba);
    //PROTO_USER_START_ARENA_BATTLE_CMD = 645,//开始竞技场战斗
    int userStartArenaBattle(uint32_t userid);
    void userStartArenaBattleCbk(byte_array_t& ba);
    //PROTO_USER_OVER_ARENA_BATTLE_CMD = 646,//结束竞技场战斗
    int userOverArenaBattle(uint32_t battle_result,uint32_t target_id,uint8_t *battle_session);
    void userOverArenaBattleCbk(byte_array_t& ba);
    //PROTO_USER_GET_ARENA_RANK_PRIZE_CMD	= 647,//竞技场领取奖励
    int userGetArenaRankPrize();
    void userGetArenaRankPrizeCbk(byte_array_t& ba);
    //PROTO_USER_GET_WORSHIP_INFO_CMD	= 648,	//得到膜拜信息
    int userGetWorshipInfo();
    void userGetWorshipInfoCbk(byte_array_t& ba);
    
    //PROTO_USER_SHOP_INFO			= 650,//拉取商城数据,参数 1代表是道具 2代表是礼包
    int userGetShopInfo(int type, int page);
    void userGetShopInfoCbk(byte_array_t& ba);
    
    //PROTO_USER_SHOP_BUY				= 651,//购买商城物品
    int userBuyGoodsInShop(int product_id, int count);
    void userBuyGoodsInShopCbk(byte_array_t& ba);
    
    //PROTO_USER_RAND_INFO_CMD		= 653,//排行榜数据
    int userGetRankInfo(int type);
    void userGetRankInfoCbk(byte_array_t& ba);

    //PROTO_USER_PRIVATE_GIFT			= 654,//个人礼包
    int userGetGiftBoxInfo(int type);
    void userGetGiftBoxInfoCbk(byte_array_t& ba);
    
    //PROTO_USER_PRIVATE_GIFT_GET_CMD		= 655,//个人礼包领取
    int userRecieveGiftBox(int type, int gift_id);
    void userRecieveGiftBoxCbk(byte_array_t& ba);
    
    //PROTO_USER_GET_BOSS_MISSION_CMD      = 656,  //拉去BOSS关卡信息 map_id为主城id
    int userGetBossMissionInfo(int map_id);
    void userGetBossMissionInfoCbk(byte_array_t& ba);
    
    //PROTO_USER_RESET_BOSS_MISSION_CMD=657,//重置BOSS关卡
    int userResetBossMission(int map_id);
    void userResetBossMissionCbk(byte_array_t& ba);
    
    //PROTO_USER_AUTO_BOSSBATTLE_CMD = 658, //BOSS扫图
    int userAutoBossBattle(int map_id);
    void userAutoBossBattleCbk(byte_array_t& ba);
    
    //PROTO_USER_GET_MAXMISSIONID_LIST_CMD = 659，//拉取开放的最大MISSIONID
    int userGetMaxMissionList();
    void userGetMaxMissionListCbk(byte_array_t& ba);
    
    //PROTO_USER_GET_VILLA_INFO_CMD	= 660,	//得到山庄信息
    int userGetVillaInfo(int user_id);
    void userGetVillaInfoCbk(byte_array_t& ba);
    
    //PROTO_USER_PLANT_CMD			= 661,	//种植种子
    int userPlant(int plantId, int landId);
    void userPlantCbk(byte_array_t& ba);
    
    //PROTO_USER_SPEED_PLANT_CMD		= 662,	//加速植物
    int userSpeedPlant(int plantId, int landId, int cost_gold);
    void userSpeedPlantCbk(byte_array_t& ba);
    
    //PROTO_USER_HAVEST_PLANT_CMD		= 663,	//收获果实
    int userHavestPlant(int plantId, int landId);
    void userHavestPlantCbk(byte_array_t& ba);
    
    //PROTO_USER_EXPAND_LAND_CMD		= 664,	//扩展土地
    int userExpandLand(int landId);
    void userExpandLandCbk(byte_array_t& ba);
    
    //PROTO_USER_SHAKE_MONEY_CMD		= 665,	//摇钱树
    int userShakeMoneyTree(uint32_t cost_gold);
    void userShakeMoneyTreeCbk(int ret, byte_array_t& ba);
    
    //PROTO_USER_ACTIVITY_DICE_CMD					= 666, //活动摇色子
    int userActivityDice(uint32_t is_cost_gold);
    void userActivityDiceCbk(int ret, byte_array_t& ba);
    
    //PROTO_USER_TOWER_CMD					= 667, //爬塔拉取信息
    int userGetTowerInfo();
    void userGetTowerInfoCbk(byte_array_t& ba);
    
    //PROTO_USER_AUTO_TOWERBATTLE_CMD = 668,//爬塔自动挑战
    int userAutoTowerBattle();
    void userAutoTowerBattleCbk(byte_array_t& ba);
    
    //PROTO_USER_RESET_TOWER_CMD		= 669, //重置爬塔次数
    int userResetTower();
    void userResetTowerCbk(byte_array_t& ba);
    
    //PROTO_USER_GET_VILLA_STATE_CMD	= 676,	//得到山庄状态
    int userGetVillaState();
    void userGetVillaStateCbk(byte_array_t& ba);
    
    //PROTO_USER_GET_STAR_LIST_CMD    = 677,  //得到占星列表
    int userGetStarList();
    void userGetStarListCbk(byte_array_t& ba);
    
    //PROTO_USER_ASTROLOGY_CMD        = 678,  //占星
    int userAstrology(uint32_t nType, uint32_t nId);
    void userAstrologyCbk(byte_array_t& ba);
    
    //PROTO_USER_TRANSFORM_STAR_CMD   = 679,  //转化单个星石
    int userTransformStar(uint32_t nId);
    void userTransformStarCbk(byte_array_t& ba);
    
    //PROTO_USER_AUTO_TRANSFORM_STAR_CMD  = 680,  //一键转化星石
    int userAutoTransformStar(uint32_t nState);
    void userAutoTransformStarCbk(byte_array_t& ba);
    
    //PROTO_USER_EQUIP_STAR_CMD       = 681,  //装备或卸下星石
    int userEquipStar(uint32_t nEquipType, uint32_t nId, uint32_t nSeatId);
    void userEquipStarCbk(byte_array_t& ba);
    
    //PROTO_USER_UPGRADE_STAR_CMD     = 682,  //升级星石
    int userUpgradeStar(uint32_t nId);
    void userUpgradeStarCbk(byte_array_t& ba);
    
    //PROTO_USER_CALL_ASTROLOGER_CMD	= 683,	//召唤占星师
    int userCallAstrologer();
    void userCallAstrologerCbk(byte_array_t& ba);
    
    //PROTO_USER_GET_ORDER_BILL_NO_CMD	= 800,	//获取充值订单号
    int userGetOrderBillNo(uint32_t producttype);
    void userGetOrderBillNoCbk(byte_array_t& ba);
    //PROTO_USER_NOTIFY_BUY_GOLD_CMD		= 801,	//充值结果通知
    int userNotifyBuyGold();
    void userNotifyBuyGoldCbk(byte_array_t& ba);
    
    //PROTO_USER_CHOOSE_HORSE_CMD = 684, //选马了
    int userChooseHorse();
    void userChooseHorseCbk(byte_array_t& ba);
    //PROTO_USER_GET_ESCORT_INFO_CMD = 686, //押镖运信息
    int userGetEscortInfo(uint32_t isnew);
    void userGetEscortInfoCbk(byte_array_t& ba);
    //PROTO_USER_REFRESH_HORSE_CMD = 685, //刷马
    int userRefreshHorse(uint32_t isgold);
    void userRefreshHorseCbk(byte_array_t& ba);
    //PROTO_USER_GET_ESCORT_USER_INFO_CMD = 690, //拉取押镖玩家信息
    int userGetEscortUserInfo(uint32_t userid);
    void userGetEscortUserInfoCbk(byte_array_t& ba);
    //PROTO_USER_ESCORT_COMBAT_INFO_CMD = 687, //战报信息
    int userEscortCombatInfo();
    void userEscortCombatInfoCbk(byte_array_t& ba);
    //PROTO_USER_START_KEAP_HORSE_CMD = 688, //开始劫持白龙马
    int userStartKeapHorse(uint32_t keaped_userid);
    void userStartKeapHorseCbk(byte_array_t& ba);
    //PROTO_USER_OVER_KEAP_HORSE_CMD = 689, //结束劫持白龙马
    int userOverKeapHorse(uint32_t battle_result,uint32_t target_id,uint8_t *battle_session);
    void userOverKeapHorseCbk(byte_array_t& ba);
    //PROTO_USER_START_ESCORT_CMD = 691, //开始押镖
    int userStartEscort();
    void userStartEscortCbk(byte_array_t& ba);
    //PROTO_USER_ESCORT_GIFT_CMD = 692, //领押镖奖励
    int userEscortGift();
    void userEscortGiftCbk(byte_array_t& ba);
    //PROTO_USER_ESCORT_EXISTS_GIFT_CMD = 693, //是否存在押镖奖励
    int userEscortExistsGift();
    void userEscortExistsGiftCbk(byte_array_t& ba);

    //PROTO_USER_INVITE_INFO_CMD = 694, //好友邀请信息
    int userInviteInfo();
    void userInviteInfoCbk(byte_array_t& ba);

    //PROTO_USER_GET_WORLD_BOSS_INFO_CMD = 695, //拉取世界boss信息
    int userGetWorldBossInfo();
    void userGetWorldBossInfoCbk(byte_array_t& ba);
    
    //PROTO_USER_WORLD_BOSS_BTL_START_CMD
    int userWorldBossBTLStart(int cost_gold);
    void userWorldBossBTLStartCbk(byte_array_t& ba);
    
    //PROTO_USER_WORLD_BOSS_BTL_OVER_CMD
    int userWorldBossBTLOver(int hurt_hp);
    void userWorldBossBTLOverCbk(byte_array_t& ba);
    
    //PROTO_USER_WORLD_BOSS_OVER_NOTI_CMD = 699
    int userWorldBossOverNoti();
    void userWorldBossOverNotiCbk(byte_array_t& ba);
    
    //PROTO_USER_CHECK_WORLD_BOSS_CMD
    int userCheckWorldBoss();
    void userCheckWorldBossCbk(byte_array_t& ba);
    
    //PROTO_USER_GET_ACTIVITY_INFO_CMD = 701, //获取活动信息
    int userGetActivityInfo(std::vector<uint32_t>& id_vec);
    void userGetActivityInfoCbk(byte_array_t& ba);
    
    //PROTO_USER_PARTNER_FRIENDVAL_TRAIN_CMD = 702, //弟子好友度培养
    int userPartnerFriendvalTrain(int nPartnerID, int nTrainType);
    void userPartnerFriendvalTrainCbk(byte_array_t& ba);
    
    //PROTO_USER_PARTNER_UNHAD_LIST_CMD = 703, //拉取没有获得的弟子列表及友好度
    int userPartnerUnhadList();
    void userPartnerUnhadListCbk(byte_array_t& ba);
    
    //PROTO_USER_BY_INVITE_CODE_CMD = 705, //设置邀请我进来的好友码
    int userByInviteCode(int nInviteCode);
    void userByInviteCodeCbk(byte_array_t& ba);
    
    //PROTO_USER_RECALL_PARTNER_CMD //放逐侠客
    int  userRecallPartner(uint32_t partner_id,uint32_t recall);
    void userRecallPartnerCbk(byte_array_t& ba);
    
	//PROTO_USER_SYS_MSG_NOTI_CMD = 710, //系统消息通知
    int userSysMsgNoti();
    void userSysMsgNotiCbk(byte_array_t& ba);
	//PROTO_USER_SET_READ_SYS_MSG_CMD = 711, //设置系统消息为已读
    int userSetReadSysMsg(int msgType);
    void userSetReadSysMsgCbk(byte_array_t& ba);
    
private:
    cli_user_battle_over_in m_battleBonus;
    
    messageBody_t& loadMessage(const char* pPrefix, int nServerId, int nUserId, int nIndex, messageBody_t& body);
    void addItem(int nItemID, int nItemCount);
    void showWelcomeMessage(float dt);
    
    static OnlineManager* pSharedManager;
    bool        m_bIsBlack;//是否加入黑名单
    bool        m_bHasConnectedOnline;//ONLINE已经连接上
    uint32_t    m_nWaitCmd;         //需要返回的online cmd
    uint32_t    m_nDealingCmd;      //需要处理的消息，当online没连接上时，先连接online，然后在处理这个
    uint32_t    m_nTimeOutCnt;      //超时的次数
    uint32_t m_nBuyEneryReason;     //0获得次数 1购买体力
    uint32_t    m_nConnectOnlineReason; //连接Online的原因
    uint32_t    m_nBuyBagCount;//购买格子数
    
    messageBody_t m_welcomeMsg;
    int m_nGetItemReason;
    int m_nPartnerType;
    bool m_bFromLogin;              //拉去logineX是登陆拉取或者是每天12点拉取
    unsigned char m_strOnlineSess[20];
};

#endif
