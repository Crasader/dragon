//
//  OnlineManager.cpp
//  MyGame
//
//  Created by 小敦 王 on 12-10-19.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "OnlineManager.h"
#include "GameManager.h"
#include "LogicManager.h"
#include "NotificeDefine.h"
#include "UserData.h"
#include "NetworkManager.h"
#include "error_code.h"
#include "DealErrorCode.h"
#include "Itemunitformula.h"
#include "PartnersData.h"
#include "XYMessageBox.h"
#include "UserDataEx.h"
#include "CommonUtility.h"
#include "TimerManager.h"
#include <time.h>

static int sOut = 0;
static int sIn = 0;

OnlineManager* OnlineManager::pSharedManager = NULL;

OnlineManager::OnlineManager()
{
    this->init();
}

OnlineManager::~OnlineManager()
{
    this->unscheduleAllSelectors();
}

bool OnlineManager::init()
{
    m_bHasConnectedOnline = false;
    m_nWaitCmd = 0;
    m_nDealingCmd = 0;
    m_nConnectOnlineReason = 0;
    m_nTimeOutCnt = 0;
    memset(m_strOnlineSess, 0, sizeof(m_strOnlineSess));
    return true;
}

OnlineManager* OnlineManager::sharedManager()
{
    if (pSharedManager == NULL)
    {
        pSharedManager = new OnlineManager();
    }
    return pSharedManager;
}

void OnlineManager::purgeManager()
{
    if (pSharedManager)
    {
        CCDirector::sharedDirector()->getScheduler()->unscheduleUpdateForTarget(pSharedManager);
        delete pSharedManager;
        pSharedManager = NULL;
    } 
}

int OnlineManager::sendDataToOnline(int seqNum, int cmd, Cmessage* pMsg, uint32_t userid)
{
    //如果online没有登录 并且需要拉取其他的信息
    if (!NetworkManager::shareNetworkManager()->isOnlineConnected() && cmd != PROTO_USER_LOGIN_CMD)
    {
        return -1;
    }
    sOut += 4 + sizeof(pMsg);
    int ret = NetworkManager::shareNetworkManager()->sendDataToOnline(KSEQ, cmd, pMsg, userid);
    if (ret <= 0)   //发送不成功
    {
        m_nWaitCmd = 0;
    }
    else
    {
        if(cmd >= 506||
           cmd == PROTO_USER_GET_PACKAGE_CMD ||
           cmd == PROTO_USER_GET_ITEM_LIST_COUNT_CMD ||
           cmd == PROTO_USER_USE_ITEM_CMD)
        {
            XYTopLayer::getInstance()->ShowWaiting();
        }
        m_nWaitCmd = cmd;
        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(OnlineManager::onOnlineTimeOut), this);
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(OnlineManager::onOnlineTimeOut),this,ONLINE_TIMEOUT_SEC,false);
    }
    return ret;
}

bool OnlineManager::handleErrCmd(int cmd)
{
    bool ret = false;
    switch (cmd) {
        case PROTO_USER_ACTIVITY_DICE_CMD:
        case PROTO_USER_SHAKE_MONEY_CMD:
        case PROTO_USER_GET_PLATFORM_BONUS_CMD:
            ret = true;
            break;
            
        default:
            break;
    }
    return ret;
}

void OnlineManager::execWantedTask(int cmd)
{
    int type = 0;
    int step = 1;
    switch (cmd) {
        case PROTO_USER_UPGRADE_SKILL_CMD:
            type = TASK_SKILL_UP_TYPE;
            break;
        case PROTO_USER_TRAIN_BASE_ATTR_CMD:
            type = TASK_TRAIN_TYPE;
            break;
        case PROTO_USER_UPGRADE_NERVE_CMD:
            type = TASK_NERVER_UP_TYPE;
            break;
        case PROTO_USER_STRENGTHEN_EQUIPMENT_CMD:
            type = TASK_EQUIPMENT_UP_TYPE;
            break;
        case PROTO_USER_BREAK_JAR_CMD:
            type = TASK_BREAK_JAR_TYPE;
            break;
        case PROTO_USER_UPGRADE_FORCE_CMD:
            type = TASK_FORCE_UP_TYPE;
            break;
        case PROTO_USER_OVER_ARENA_BATTLE_CMD:
            type = TASK_SPORTS_TYPE;
            break;
        case PROTO_USER_SHOP_BUY_CMD:
            type = TASK_SHOPPING_MALL_TYPE;
            break;
        case PROTO_USER_AUTO_BOSS_BATTLE_CMD:
            type = TASK_ELITE_BOSS_TYPE;
            break;
        case PROTO_USER_PLANT_CMD:
            type = TASK_FARMLAND_TYPE;
            break;
        case PROTO_USER_SHAKE_MONEY_CMD:
            type = TASK_SHAKE_MONEY_TREE_TYPE;
            break;
        case PROTO_USER_ASTROLOGY_CMD:          //占星
            type = TASK_USER_ASTROLOGY_TYPE;
            break;
        case PROTO_USER_ADD_FRIEND_CMD:         //添加好友
            type = TASK_ADD_FRIEND_TYPE;
            break;
        case PROTO_USER_START_ESCORT_CMD:       //押镖
            type = TASK_BODYGUARD_TYPE;
            break;
        default:
            break;
    }
    
    gUserDataEx->execWantedTask(type, step);
}

// 处理online返回的信息
int OnlineManager::process_Online_CMD(char* data, int len)
{
    sIn += len;
    XYTopLayer::getInstance()->RemoveWaiting();
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(OnlineManager::onOnlineTimeOut), this);
    cli_proto_head_t* head = (cli_proto_head_t*)data;
    
    if (m_nWaitCmd != head->cmd)
    {
        //如果online返回的不与waitcmd相等，则判断是否为online发送的主动通知信息
        //处理完后直接return掉
        //return 0;
    }
    
    m_nWaitCmd = 0;//收到服务器的返回,这儿的协议都不是服务器主动发出的
    m_nTimeOutCnt = 0;  //超时数设置0
    
    // online 没有登录 弹出登录框，重新登录
    if (head->ret == cli_user_hadnot_login_to_online_err)
    {
        m_bHasConnectedOnline = false;
        GameManager::sharedInstance()->enterLoginLayer();
        return 0;
    }
    
    if (head->ret != 0)
    {
        CCLOG("online return error!--------------");
        DealErrorCode::dealErrorFunc(head->ret);
        
        if (!handleErrCmd(head->cmd))
        {
            return 0;
        }
    }
    
    static byte_array_t ba;
    ba.init_read_mode((const char*)(data + sizeof(cli_proto_head_t)), len);
//    ba.init_postion();
//    ba.write_buf((const char*)(data + sizeof(cli_proto_head_t)), len);
    
    execWantedTask(head->cmd);
    
    switch (head->cmd)
    {
        case PROTO_USER_LOGIN_CMD:  //用户登录包，校验session, cmdid 500
            //userLoginToOnlineCbk(head, (const uint8_t*)(data + sizeof(cli_proto_head_t)), len);
            userLoginToOnlineCbk(head, ba);
            break;
        case PROTO_KICK_USER_OFFLINE_CMD:
            kickUserOfflineCbk(head, ba);
            break;
        case PROTO_USER_LOGIN_EX_CMD: //用户登录后，紧跟着拉去任务、在线礼包等数据
            userLoginExToOnlineCbk(head, ba);
            break;
        case PROTO_USER_ACTION_CMD:
            userActionToOnlineCbk(ba);
            break;
        case PROTO_USER_CHANGE_EQUIPMENT_CMD:
            equipmentChangeToOnlineCbk(ba);
            break;
        case PROTO_USER_WEAR_EQUIPMENT_CMD:
            equipmentWearToOnlineCbk(ba);
            break;
        case PROTO_USER_PUNCH_EQUIPMENT_CMD:
            userPunchEquipmentCbk(ba);
            break;
        case PROTO_USER_INLAY_GEM_CMD:
            userInlayGemCbk(ba);
            break;
        case PROTO_USER_COMPOSE_GEM_CMD:
            userComposeGemCbk(ba);
            break;
        case PROTO_USER_TRAIN_BASE_ATTR_CMD:
            userTrainToOnlineCbk(ba);
            break;
        case PROTO_USER_CONFIRM_TRAIN_CMD:
            userTrainConfirmToOnlineCbk(ba);
            break;
        case PROTO_USER_CLEAR_COOLDOWN_CMD:
            cancelCoolDownToOnlineCbk(ba);
            break;
        case PROTO_USER_UPGRADE_NERVE_CMD:
            nerveUpgradeToOnlineCbk(ba);
            break;
        case PROTO_USER_CHAT_CMD:
            userChatToOnlineCbk(ba);
            break;
        case PROTO_USER_GET_COOLDOWN_CMD:
            getCDTimeToOnlineCbk(ba);
            break;
        case PROTO_USER_GET_TYPE_EVENT_CMD:
            getEventCountToOnlineCbk(ba);
            break;
        case PROTO_USER_GET_DAILY_EVENT_CMD:
            getDailyEventCbk(ba);
            break;
        case PROTO_USER_STRENGTHEN_EQUIPMENT_CMD:
            equipmentStrengthenToOnlineCbk(ba);
            break;
        case PROTO_USER_UPGRADE_EQUIPMENT_CMD:
            equipmentUpgradeToOnlineCbk(ba);
            break;
        case PROTO_USER_TRAIN_EQUIPMENT_CMD:
            equipmentWashToOnlineCbk(ba);
            break;
        case PROTO_USER_GET_PACKAGE_CMD:
            userGetPackageToOnlineCbk(ba);
            break;
        case PROTO_USER_BUY_ITEM_CMD:
            userBuyItemToOnlineCbk(ba);
            break;
        case PROTO_USER_KEEP_ALIVE_CMD://如果返回心跳包，暂不处理
            userKeepAliveCbk(head->ret, ba);
            break;
        case PROTO_USER_UPGRADE_FORCE_CMD:
            userUpgradeForceCbk(ba);
            break;
        case PROTO_USER_EXPAND_PACKAGE_CMD:
            userExpandPackageToOnlineCbk(ba);
            break;
        case PROTO_USER_SELL_ITEM_CMD:
            userSellItemToOnlineCbk(ba);
            break;
        case PROTO_USER_USE_PARTNER_CARD_CMD:
            userUsePartnerCardCbk(ba);
            break;
        case PROTO_USER_GET_SKILL_CMD:
            userGetSkillToOnlineCbk(ba);
            break;
        case PROTO_USER_UPGRADE_SKILL_CMD:
            userUpgradeSkillToOnlineCbk(ba);
            break;
        case PROTO_USER_CHANGE_SKILL_CMD:
            userChangeSkillToOnlineCbk(ba);
            break;
        case PROTO_USER_CHEAT_ITEM_CMD:
            userCheatItemCbk(ba);
            break;
        case PROTO_USER_BUY_ENERY_CMD:
            userBuyEneryToOnlineCbk(ba);
            break;
        case PROTO_USER_GET_STAR_LIST_CMD:
            userGetStarListCbk(ba);
            break;
        case PROTO_USER_ASTROLOGY_CMD:
            userAstrologyCbk(ba);
            break;
        case PROTO_USER_TRANSFORM_STAR_CMD:
            userTransformStarCbk(ba);
            break;
        case PROTO_USER_AUTO_TRANSFORM_STAR_CMD:
            userAutoTransformStarCbk(ba);
            break;
        case PROTO_USER_EQUIP_STAR_CMD:
            userEquipStarCbk(ba);
            break;
        case PROTO_USER_UPGRADE_STAR_CMD:
            userUpgradeStarCbk(ba);
            break;
        case PROTO_USER_CALL_ASTROLOGER_CMD:
            userCallAstrologerCbk(ba);
            break;
        case PROTO_USER_RECOVER_ENERY_CMD:
            userRecoverEneryToOnlineCbk(ba);
            break;
        case PROTO_USER_GET_MISSION_CMD:
            userGetMissionToOnlineCbk(ba);
            break;
        case PROTO_USER_LOGIN_SIGN_CMD:
            userLoginSignInToOnlineCbk(ba);
            break;
        case PROTO_USER_START_AUTO_BATTLE_CMD:
            userStartAutoBattleCbk(ba);
            break;
        case PROTO_USER_STOP_AUTO_BATTLE_CMD:
            userStopAutoBattleCbk();
            break;
        case PROTO_USER_SPEED_AUTO_BATTLE_CMD:
            userSpeedAutoBattleCbk(ba);
            break;
        case PROTO_USER_GET_ROUNDS_BATTLE_CMD:
            userGetRoundsBattleCbk(ba);
            break;
        case PROTO_USER_BATTLE_START_CMD:
            userBattleStartCbk(ba);
            break;
        case PROTO_USER_BATTLE_OVER_CMD:
            battleOverCbk(ba);
            break;
        case PROTO_USER_GET_BATTLE_PRIZE_CMD:
            getCardPrizeToOnlineCbk(ba);
            break;
        case PROTO_USER_GET_ATTR_ITEM_CMD:
            getUserAttrToOnlineCbk(ba);
            break;
        case PROTO_USER_GET_JAR_LIST_CMD:
            getPotListToOnlineCbk(ba);
            break;
        case PROTO_USER_UPGRADE_JAR_CMD:
            potUpgradeToOnlineCbk(ba);
            break;
        case PROTO_USER_DEL_FRIEND_CMD:
            userDelFriendToOnlineCbk(ba);
            break;
        case PROTO_USER_BREAK_JAR_CMD:
            potClickToOnlineCbk(ba);
            break;
        case PROTO_USER_ADD_FRIEND_CMD:
            userAddFriendToOnlineCbk(ba);
            break;
        case PROTO_USER_GET_ITEM_LIST_COUNT_CMD:
            userGetItemListCountToOnlineCbk(ba);
            break;
        case PROTO_USER_GET_MAGIC_STORE_CMD:
            userGetMagicStoreToOnlineCbk(ba);
            break;
        case PROTO_USER_BUY_MAGIC_ITEM_CMD:
            userBuyMagicItemToOnlineCbk(ba);
            break;
        case PROTO_USER_FLUSH_MAGIC_STORE_CMD:
            userFlushMagicStoreToOnlineCbk(ba);
            break;
        case PROTO_USER_SWAP_CMD:
            userSwapCbk(ba);
            break;
        case PROTO_USER_BALLOT_CMD:
            userBallotCbk(ba);
            break;
        case PROTO_USER_ENLIST_PARTNER_CMD:
            userEnlistPartnerCbk(ba);
        case PROTO_USER_GET_PARTNER_LIST_CMD:
            usergetPartnerListCbk(ba);
            break;
        case PROTO_USER_ONPLAY_PARTNER_CMD:
            userOnPlayerPartnerCbk(ba);
            break;
        case PROTO_USER_DROP_TASK_CMD:
            userDropTaskCbk(ba);
            break;
        case PROTO_USER_INHERIT_PARTNER_CMD:
            userInheritPartnerCbk(ba);
            break;
        case PROTO_USER_GET_PLATFORM_BONUS_CMD:
            userGetPlatformBonusToOnlineCbk(head->ret, ba);
            break;
        case PROTO_USER_RECALL_PARTNER_CMD:
            userRecallPartnerCbk(ba);
            break;
        case PROTO_USER_OFFPLAY_PARTNER_CMD:
            userOffPlayPartnerCbk(ba);
            break;
        case PROTO_USER_SWAP_PARTNER_SEAT_CMD:
            userSwapPartnerSeatCbk(ba);
            break;
        case PROTO_USER_GET_PRESTIGE_TITLE_CMD://	= 579,	//得到声望挑战信息
            userGetPrestigeTitleCbk(ba);
            break;
        case PROTO_USER_GET_PRESTIGE_PRIZE_CMD://	= 580,	//领取
            userGetPrestigePrizeCbk(ba);
            break;
        case PROTO_USER_PRESTIGE_LOTTERY_CMD:   //	= 581,	//声望摇奖
            userPrestigeLotteryCbk(ba);
            break;
        case PROTO_USER_PARTNER_EAT_EXP_CMD:
            userPartnerEatExpCbk(ba);
            break;
        case PROTO_USER_ACCEPT_TASK_CMD:
            userAcceptTaskCbk(ba);
            break;
        case PROTO_USER_FINISH_TASK_CMD:
            userFinishTaskCbk(ba);
            break;
        case PROTO_USER_SET_TASK_CMD:
            userSetTaskCbk(ba);
            break;
        case PROTO_USER_INVITE_INFO_CMD:
            userInviteInfoCbk(ba);
            break;
        case PROTO_USER_GET_TASK_LIST_CMD:
            userGetTaskListCbk(ba);
            break;
        case PROTO_USER_USE_ITEM_CMD:
            userUseItemCbk(head, ba);
            break;
        case PROTO_USER_BY_INVITE_CODE_CMD:
            userByInviteCodeCbk(ba);
            break;
        case PROTO_USER_GET_AWARD_TASK_CMD:
            userGetWantedTaskListCbk(ba);
            break;
        case PROTO_USER_ACCEPT_AWARD_TASK_CMD:
            userAcceptWantedTaskCbk(ba);
            break;
        case PROTO_USER_UPGRADE_AWARD_TASK_CMD:
            userUpgradeWantedTaskCbk(ba);
            break;
        case PROTO_USER_SET_AWARD_TASK_FINISH_CMD:
            userSetWantedTaskFinishCbk(ba);
            break;
        case PROTO_USER_FINISH_AWARD_TASK_CMD:
            userFinishWantedTaskCbk(ba);
            break;
        case PROTO_USER_REFRESH_AWARD_TASK_CMD:
            userRefreshWantedTaskCbk(ba);
            break;
        case PROTO_USER_GET_ARENA_INFO_CMD:
            userGetArenaInfoCbk(ba);
            break;
        case PROTO_USER_GET_ARENA_TOP_100_CMD:
            userGetArenaTop100Cbk(ba);
            break;
        case PROTO_USER_WORSHIP_TOP_USER_CMD:
            userWorshipTopCbk(ba);
            break;
        case PROTO_USER_GET_ALL_BATTLE_PRIZE_CMD:
            userGetAllBattlePrizeCbk(ba);
            break;
        case PROTO_USER_GET_ARENA_FILE_CMD:
            userGetArenaFileCbk(ba);
            break;
        case PROTO_USER_GET_ARENA_RANK_PRIZE_CMD:
            userGetArenaRankPrizeCbk(ba);
            break;
        case PROTO_USER_GET_WORSHIP_INFO_CMD:
            userGetWorshipInfoCbk(ba);
            break;
        case PROTO_USER_SHOP_INFO_CMD:
            userGetShopInfoCbk(ba);
            break;
        case PROTO_USER_SHOP_BUY_CMD:
            userBuyGoodsInShopCbk(ba);
            break;
        case PROTO_USER_RAND_INFO_CMD:
            userGetRankInfoCbk(ba);
            break;
        case PROTO_USER_START_ARENA_BATTLE_CMD:
            userStartArenaBattleCbk(ba);
            break;
        case PROTO_USER_OVER_ARENA_BATTLE_CMD:
            userOverArenaBattleCbk(ba);
            break;
        case PROTO_USER_BUY_HAMMER_CMD:
            userBuyHammerCbk(ba);
            break;
        case PROTO_USER_PRIVATE_GIFT_CMD:
            userGetGiftBoxInfoCbk(ba);
            break;
        case PROTO_USER_PRIVATE_GIFT_GET_CMD:
            userRecieveGiftBoxCbk(ba);
            break;
        case PROTO_USER_GET_BOSS_MISSION_CMD:
            userGetBossMissionInfoCbk(ba);
            break;
        case PROTO_USER_DEAL_FRIEND_REQUEST_CMD:
            userDealFriendRequestToOnlineCbk(ba);
            break;
        case PROTO_USER_AUTO_BOSS_BATTLE_CMD:
            userAutoBossBattleCbk(ba);
            break;
        case PROTO_USER_GET_USER_ONLINE_STATUS_CMD:
            userGetUsersOnlineStatusCbk(ba);
            break;
        case PROTO_USER_GET_FRIEND_LIST_CMD:
            userGetFriendListToOnlineCbk(ba);
            break;
        case PROTO_USER_GET_MAX_MISSION_CMD:
            userGetMaxMissionListCbk(ba);
            break;
        case PROTO_USER_RESET_BOSS_MISSION_CMD:
            userResetBossMissionCbk(ba);
            break;
        case PROTO_USER_GET_VILLA_INFO_CMD:
            userGetVillaInfoCbk(ba);
            break;
        case PROTO_USER_PLANT_CMD:
            userPlantCbk(ba);
            break;
        case PROTO_USER_SPEED_PLANT_CMD:
            userSpeedPlantCbk(ba);
            break;
        case PROTO_USER_HAVEST_PLANT_CMD:
            userHavestPlantCbk(ba);
            break;
        case PROTO_USER_EXPAND_LAND_CMD:
            userExpandLandCbk(ba);
            break;
        case PROTO_USER_FIND_USER_FROM_NICK_CMD:
            userFindUserFromNickToOnlineCbk(ba);
            break;
        case PROTO_USER_SHAKE_MONEY_CMD:
            userShakeMoneyTreeCbk(head->ret, ba);
            break;
        case PROTO_USER_ACTIVITY_DICE_CMD:
            userActivityDiceCbk(head->ret, ba);
            break;
        case PROTO_USER_TOWER_CMD:
            userGetTowerInfoCbk(ba);
            break;
        case PROTO_USER_AUTO_TOWERBATTLE_CMD:
            userAutoTowerBattleCbk(ba);
            break;
        case PROTO_USER_RESET_TOWER_CMD:
            userResetTowerCbk(ba);
            break;
        case PROTO_USER_GET_OTHER_USER_CMD:
            userGetOtherUserCbk(ba);
            break;
        case PROTO_USER_GET_VILLA_STATE_CMD:
            userGetVillaStateCbk(ba);
            break;
        case PROTO_USER_GET_ORDER_BILL_NO_CMD:
            userGetOrderBillNoCbk(ba);
            break;
        case PROTO_USER_NOTIFY_BUY_GOLD_CMD:
            userNotifyBuyGoldCbk(ba);
            break;
        case PROTO_USER_CHOOSE_HORSE_CMD:
            userChooseHorseCbk(ba);
            break;
        case PROTO_USER_GET_ESCORT_INFO_CMD:
            userGetEscortInfoCbk(ba);
            break;
        case PROTO_USER_REFRESH_HORSE_CMD:
            userRefreshHorseCbk(ba);
            break;
        case PROTO_USER_GET_ESCORT_USER_INFO_CMD:
            userGetEscortUserInfoCbk(ba);
            break;
        case PROTO_USER_ESCORT_COMBAT_INFO_CMD:
            userEscortCombatInfoCbk(ba);
            break;
        case PROTO_USER_START_KEAP_HORSE_CMD:
            userStartKeapHorseCbk(ba);
            break;
        case PROTO_USER_START_ESCORT_CMD:
            userStartEscortCbk(ba);
            break;
        case PROTO_USER_OVER_KEAP_HORSE_CMD:
            userOverKeapHorseCbk(ba);
            break;
        case PROTO_USER_ESCORT_GIFT_CMD:
            userEscortGiftCbk(ba);
            break;
        case PROTO_USER_ESCORT_EXISTS_GIFT_CMD:
            userEscortExistsGiftCbk(ba);
            break;
        case PROTO_USER_GET_WORLD_BOSS_INFO_CMD:
            userGetWorldBossInfoCbk(ba);
            break;
        case PROTO_USER_WORLD_BOSS_BTL_START_CMD:
            userWorldBossBTLStartCbk(ba);
            break;
        case PROTO_USER_WORLD_BOSS_BTL_OVER_CMD:
            userWorldBossBTLOverCbk(ba);
            break;
        case PROTO_USER_WORLD_BOSS_OVER_NOTI_CMD:
            userWorldBossOverNotiCbk(ba);
            break;
        case PROTO_USER_CHECK_WORLD_BOSS_CMD:
            userCheckWorldBossCbk(ba);
            break;
        case PROTO_USER_GET_ACTIVITY_INFO_CMD:
            userGetActivityInfoCbk(ba);
            break;
        case PROTO_USER_PARTNER_FRIENDVAL_TRAIN_CMD:
            userPartnerFriendvalTrainCbk(ba);
            break;
        case PROTO_USER_PARTNER_UNHAD_LIST_CMD:
            userPartnerUnhadListCbk(ba);
            break;
        case PROTO_USER_UPGRADE_PARTNER_SKILL_CMD:
            userUpgradePartnerSkillCbk(ba);
            break;
        case PROTO_USER_SYS_MSG_NOTI_CMD:
            userSysMsgNotiCbk(ba);
            break;
        case PROTO_USER_SET_READ_SYS_MSG_CMD:
            userSetReadSysMsgCbk(ba);
            break;
        case PROTO_USER_GET_HARD_MISSION_CMD:
            userGetHardMissionToOnlineCbk(ba);
            break;
        default:
            CCLOG("unkown online cmdid %d", head->cmd);
            break;
    }
    return 0;
}

// 断线后返回登录页面
void OnlineManager::onEnterLoginLayer(CCNode* pNode)
{
    GameManager::sharedInstance()->enterLoginLayer();
}

// 断线后重新连接，不退出游戏
void OnlineManager::onReconnectOnline(CCNode* pNode)
{
    //战斗中自动重连，不需要菊花
    if (!GameManager::sharedInstance()->getBattleLayer())
    {
        XYTopLayer::getInstance()->ShowWaiting();
    }
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(OnlineManager::onNetConnectTimeOut), this);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(OnlineManager::onNetConnectTimeOut), this,ONLINE_TIMEOUT_SEC, false);
    LogicManager::shareLogicManager()->startNetWork();
}

// 执行connect online超时
void OnlineManager::onNetConnectTimeOut(float t)
{
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(OnlineManager::onNetConnectTimeOut), this);
    LogicManager::shareLogicManager()->stopNetWork(E_CONNECT_FAILD);
    XYTopLayer::getInstance()->RemoveWaiting();
    
    // 如果是服务器socket断开了，主动重连
    if (LogicManager::shareLogicManager()->getStopNetReason() == E_SERVER_CLOSE)
    {
        onReconnectOnline(NULL);
        return ;
    }
    
    LogicManager::shareLogicManager()->changeStopNetReason(E_CONNECT_FAILD);
    XYMessageBox* pMessageBox = XYMessageBox::create(E_MB_OKCANCEL, "", "大侠，不要远离小女子，快去看看网络设置中是否有误。", this, callfuncN_selector(OnlineManager::onReconnectOnline), callfuncN_selector(OnlineManager::onEnterLoginLayer));
    pMessageBox->setMsgAlignment(kCCTextAlignmentCenter);
    pMessageBox->setTag(255);
    if (!XYTopLayer::getInstance()->getChildByTag(255))
    {
        pMessageBox->showMessageBox();
    }
}

/*
 * @brief 处理online协议超时
 */
int OnlineManager::onOnlineTimeOut(float dt)
{
    m_nTimeOutCnt++;
    //用户自动上传数据连续3次超时，则关掉online连接
    if (m_nTimeOutCnt == 3)
    {
        m_nTimeOutCnt = 0;
        //NetworkManager::shareNetworkManager()->disConnectOnline();
        m_bHasConnectedOnline = false;
    }
    LogicManager::shareLogicManager()->stopNetWork(E_SEND_TIME_OUT);
    XYTopLayer::getInstance()->RemoveWaiting();
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(OnlineManager::onOnlineTimeOut), this);
    
    // 战斗场景中
    if (GameManager::sharedInstance()->getBattleLayer())
    {
        return 0;
    }
    
    XYMessageBox* pMessageBox = XYMessageBox::create(E_MB_OKCANCEL, "", "大侠，不要远离小女子，快去看看网络设置中是否有误。", this, callfuncN_selector(OnlineManager::onReconnectOnline), callfuncN_selector(OnlineManager::onEnterLoginLayer));
    pMessageBox->setMsgAlignment(kCCTextAlignmentCenter);
    pMessageBox->setTag(255);
    if (!XYTopLayer::getInstance()->getChildByTag(255))
    {
        pMessageBox->showMessageBox();
    }
    
    
    switch (m_nWaitCmd)
    {
        case PROTO_USER_LOGIN_CMD://登录命令超时
        {
            /*
            m_bHasConnectedOnline = false;
            GameManager::sharedInstance()->enterLoginLayer();
            break;
             */
        }
            
        default:
            //do nothing
            break;
    }
    m_nWaitCmd = 0;
    return 0;
}

void OnlineManager::setOnlineSession(uint32_t svrTime, const char* session)
{
    *(uint32_t*)m_strOnlineSess = svrTime;
    for (int i = 0; i < 16; i++)
    {
        unsigned char c = (get_hex(session[i * 2]) << 4) + get_hex(session[i * 2 + 1]);
        m_strOnlineSess[4 + i] = c;
    }
}


//PROTO_KICK_USER_OFFLINE_CMD = 499 用户在其他设备上登录
void OnlineManager::kickUserOfflineCbk(cli_proto_head_t* head, byte_array_t& ba)
{
    XYTopLayer::getInstance()->RemoveWaiting();
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(OnlineManager::onOnlineTimeOut), this);
    
    NetworkManager::shareNetworkManager()->disConnectOnline();
    LogicManager::shareLogicManager()->stopNetWork(E_KICK_OFFLINE);
    m_nWaitCmd = 0;
    m_bHasConnectedOnline = false;
    
    XYMessageBox* pMessageBox = XYMessageBox::create(E_MB_OKCANCEL, "", "您的账号在其他设备上被使用，是否重新连接!", this, callfuncN_selector(OnlineManager::onReconnectOnline), callfuncN_selector(OnlineManager::onEnterLoginLayer));
    pMessageBox->setMsgAlignment(kCCTextAlignmentCenter);
    pMessageBox->setOKBtnText("重新连接");
    pMessageBox->setCancelBtnText("退出");
    pMessageBox->setTag(255);
    if (!XYTopLayer::getInstance()->getChildByTag(255))
    {
        pMessageBox->showMessageBox();
    }
}

messageBody_t& OnlineManager::loadMessage(const char* pPrefix, int nServerId, int nUserId, int nIndex, messageBody_t& body)
{
    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
    char str[128] = {0};
    sprintf(str, "%s%dMessage%dUserID%d", pPrefix, nServerId, nUserId, nIndex);
    body.nUserID = userDefault->getIntegerForKey(str, 0);
    sprintf(str, "%s%dMessage%dTime%d", pPrefix, nServerId, nUserId, nIndex);
    body.nTime = userDefault->getIntegerForKey(str, 0);
    sprintf(str, "%s%dMessage%dNick%d", pPrefix, nServerId, nUserId, nIndex);
    body.nick = userDefault->getStringForKey(str, "");
    sprintf(str, "%s%dMessage%dLevel%d", pPrefix, nServerId, nUserId, nIndex);
    body.nLevel = userDefault->getIntegerForKey(str, 0);
    sprintf(str, "%s%dMessage%dUnitID%d", pPrefix, nServerId, nUserId, nIndex);
    body.nUnitID = userDefault->getIntegerForKey(str, 1);
    sprintf(str, "%s%dMessage%dVipLevel%d", pPrefix, nServerId, nUserId, nIndex);
    body.nVipLevel = userDefault->getIntegerForKey(str, 0);
    sprintf(str, "%s%dMessage%dChannelID%d", pPrefix, nServerId, nUserId, nIndex);
    body.nChannelID = userDefault->getIntegerForKey(str, 0);
    sprintf(str, "%s%dMessage%dTargetID%d", pPrefix, nServerId, nUserId, nIndex);
    body.nTargetID = userDefault->getIntegerForKey(str, 0);
    sprintf(str, "%s%dMessage%dMsg%d", pPrefix, nServerId, nUserId, nIndex);
    body.msg = userDefault->getStringForKey(str, "");
    return body;
}


/*
 * @brief PROTO_USER_LOGIN_CMD: 用户登录包，校验session, cmdid 500
 * @return 负值和0表示发送不成功, 大于0为发送的字节数
 */
int OnlineManager::userLoginToOnline()
{
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(OnlineManager::onNetConnectTimeOut), this);
    cli_user_login_in in;
    memcpy(in.login_sess, m_strOnlineSess, sizeof(in.login_sess));
    return this->sendDataToOnline(KSEQ, PROTO_USER_LOGIN_CMD, &in);
}

void OnlineManager::userLoginToOnlineCbk(cli_proto_head_t* head, byte_array_t& ba)
{
    if (head->ret == 0 || head->ret == cli_user_had_login_to_online_err)
    {
        //设置已经登陆online
        m_bHasConnectedOnline = true;
    }
    else
    {
        //校验不通过
        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(OnlineManager::heartbeat), this);
        LogicManager::shareLogicManager()->stopNetWork();
        return;
    }
    cli_user_login_out out;
    ba.init_postion();
    out.read_from_buf(ba);
    int out_len = out.get_msg_len();
    CCLOG("%d %lu", out_len, sizeof(partner_info_t));
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }

    gUserData->resetData();

    gUserData->setyxb(out.yxb);
    gUserData->setPrestige(out.prestige_value);
    gUserData->setEnery(out.enery);
    gUserData->setMaxEnery(out.enery_max);
    gUserData->setEneryTime(out.enery_counter);
    gUserData->setCurCommunityID(out.last_community_id, out.spec_community != 0);
    
    gUserData->package_item_sum = out.package_cnt;
    gUserData->package_size = out.max_package_size;
    gUserData->is_exists_invite = out.is_exists_invite;
    gUserData->setGold(out.gold);
    gUserData->setPower(out.power_value);
    gUserData->resetBuyGold(out.buy_gold);
    gUserData->setVipLevel(GameData::getVipLvByCostGold(out.buy_gold));
    gUserData->setNerveValue(out.nerve_value);
    gUserData->setStarStoneExp(out.star_exp);
    
    memcpy(gUserData->nick, out.nick, sizeof(out.nick));
    gUserData->m_heroData->nUnitID = out.unit_id;
    gUserData->nPreExp = out.exp;
    gUserData->m_heroData->setExp(out.exp);
    
    gUserData->m_heroData->baseAttr.nUnitID = out.unit_id;
    gUserData->m_heroData->baseAttr.nResourceID = out.unit_id;
    memset(gUserData->m_heroData->baseAttr.unitName,0x0,64);
    memcpy(gUserData->m_heroData->baseAttr.unitName, out.nick, sizeof(out.nick));
    gUserData->m_heroData->baseAttr.nResourceID = out.unit_id;//主角ID与主角的资源ID一致

    gUserData->m_heroData->setInForceExp(out.force_exp);
    gUserData->m_heroData->setNerveLevel(out.nerve_level);
    gUserData->m_heroData->trainAttr.nToughness = out.train_toughness;
    gUserData->m_heroData->trainAttr.nInAttack = out.train_in_attack;
    gUserData->m_heroData->trainAttr.nInDef = out.train_in_def;
    gUserData->m_heroData->trainAttr.nOutAttack = out.train_out_attack;
    gUserData->m_heroData->trainAttr.nOutDef = out.train_out_def;
    gUserData->m_heroData->trainAttr.nLife = out.train_hp;
    gUserData->m_heroData->userEquip.clear();
    for (int i = 0; i<out.equipment_vec.size(); i++)
    {
        package_equipment_t unit = out.equipment_vec.at(i);
        gUserData->m_heroData->userEquip[GameData::getItem(unit.equipment_id)->u.equipment.nItemType] = unit;
    }
    for (int i = 0; i < 4; i++)
    {
        gUserData->m_heroData->star_array[i] = out.star_array[i];
    }
    memcpy(&gUserData->m_heroData->skill, &out.skills, sizeof(out.skills));
    memcpy(&gUserData->m_heroData->skillBuffLv, &out.passive_skills, sizeof(out.passive_skills));
    if (!GameManager::sharedInstance()->getBattleLayer() && !GameManager::sharedInstance()->getCommunityLayer())
    {
        gUserData->m_partnerDic->removeAllObjects();
        gUserData->m_arrPartner->removeAllObjects();
        for (int i = 0; i<out.partner_vec.size(); i++)
        {
            partner_info_t *info = &out.partner_vec.at(i);
            
            const PartnerConf_t* pPartnerConf = GameData::getPartnerConf(info->partner_id);
            if (!pPartnerConf)
            {
                CCLOG("---------------------弟子ID错误-----------------------!");
            }
            PartnerData *partnerData = PartnerData::create(info->partner_id);
            partnerData->setExp(info->exp);
            partnerData->skillLv = info->skill_level;
            partnerData->trainAttr.nLife = info->train_hp;
            partnerData->trainAttr.nInAttack = info->train_in_attack;
            partnerData->trainAttr.nOutAttack= info->train_out_attack;
            partnerData->trainAttr.nOutDef= info->train_out_def;
            partnerData->trainAttr.nInDef= info->train_in_def;
            partnerData->trainAttr.nToughness= info->train_toughness;
            partnerData->curseat= info->cur_seat;
            partnerData->inheritstate = info->inherit_state;
            for (int j = 0; j < 4; j++)
            {
                partnerData->star_array[j] = info->star_array[j];
            }
            for (int j = 0; j < 8; j++)
            {
                if (info->equipment[j].package_id == 0)
                {
                    continue;
                }
                partnerData->userEquip[j + 1] = info->equipment[j];
            }
            if (partnerData->curseat != 0)
            {
                gUserData->m_partnerDic->setObject(partnerData,partnerData->curseat);
            }
            else
            {
                gUserData->m_arrPartner->addObject(partnerData);
            }
        }
    }
    time_t timer;
    time(&timer);
    gUserData->m_dataEx->login_time = timer;
    //TODO:加入时间信息，先以本地时间存储，暂未加入五小时时限过滤
    gUserData->GlobalMessages.clear();
    for (int i = 0; i < GLOBAL_MESSAGE_LIMIT; i++)
    {
        messageBody_t t;
        t = this->loadMessage("Global", gUserData->server_id, gUserData->userid, i, t);
        if (t.nUserID != 0)
        {
            gUserData->GlobalMessages.insert(std::pair<int, messageBody_t>(i, t));
        }
    }
    
    gUserData->FriendMessages.clear();
    for (int i = 0; i < FRIEND_MESSAGE_LIMIT; i++)
    {
        messageBody_t t;
        t = this->loadMessage("Friend", gUserData->server_id, gUserData->userid, i, t);
        if (t.nUserID != 0)
        {
            gUserData->FriendMessages.insert(std::pair<int, messageBody_t>(i, t));
        }
    }
    
    gUserData->NewFriendMessageCount.clear();
    m_bFromLogin = true;
    this->userLoginExToOnline();
    if (out.auto_battling)
    {
        userGetRoundsBattle();
    }
    userGetMaxMissionList();
    // 登陆后及时取Sver时间
    heartbeat(0);
}

int OnlineManager::userLoginExToOnline()
{
    return this->sendDataToOnline(KSEQ, PROTO_USER_LOGIN_EX_CMD, NULL);
}

void  OnlineManager::userLoginExToOnlineCbk(cli_proto_head_t* head, byte_array_t& ba)
{
    cli_user_login_ex_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    
    UserDataEx* pDataEx = gUserData->m_dataEx;
    pDataEx->taskMap.clear();
    for (int i = 0; i<out.task_vec.size(); i++)
    {
        task_info_t taskinfo = out.task_vec.at(i);
        pDataEx->taskMap.insert(std::pair<int , task_info_t>(taskinfo.task_id,taskinfo));
    }
    pDataEx->m_curTask = -1;
    pDataEx->setCurTask(pDataEx->getOptimalTask());
    pDataEx->setMaxMainTask(out.max_main_task);
    
    pDataEx->award_time_counter = out.award_counter;
    pDataEx->awardTask.clear();
    for (int i = 0; i < 4; i ++)
    {
        award_task_info_t task = out.award_task[i];
        if (task.cur_step > task.max_step)
        {
            task.cur_step = task.max_step;
        }
        pDataEx->awardTask.insert(std::pair<int, award_task_info_t>(task.task_id, task));
    }

    pDataEx->action_prize_vec.clear();
    
    pDataEx->week_login_prize = out.week_login_prize;
    pDataEx->level_prize = out.level_prize;
    pDataEx->action_prize_vec = out.action_prize_vec;
    pDataEx->self_gift = out.self_gift & 0x1;
    pDataEx->cur_date = out.cur_date;
    pDataEx->sign_value = out.sign_value;
    pDataEx->sign_prize = out.sign_prize;
	pDataEx->is_vip_gift = out.is_vip_gift;
    pDataEx->vip_level_gift = out.vip_level_gift;
    pDataEx->totol_vip_gift = out.totol_vip_gift;
	pDataEx->offline_exp_hour = out.offline_exp_hour;//离线经验小时数
	pDataEx->offline_exp = out.offline_exp;
    pDataEx->offline_cost_cold = out.offline_cost_cold;
	pDataEx->offline_days = out.offline_days;//离线天数
	pDataEx->offline_days_exp = out.offline_days_exp;//离线期内显示的经验
	pDataEx->offline_days_silver = out.offline_days_yxb;//离线期内显示的音量
	pDataEx->offline_prize_id = out.offline_prize_id;	//离线补偿
    pDataEx->fake_vip_state = out.fake_vip_state;
    //剩余次数
    pDataEx->num_vec.clear();
    for (std::vector<uint32_t>::iterator it = out.num_vec.begin(); it != out.num_vec.end(); it++)
    {
        pDataEx->num_vec.push_back(*it);
    }
    
    pDataEx->shake_money = out.shake_money;	//摇钱树 今日次数=shake_money>>16   双倍时间=shake_money&0xFFFF
    pDataEx->login_day = out.login_day;     //创建账号开始的第几天登录
	pDataEx->seven_day_prize = out.seven_day_prize;//头7日登录目标
	pDataEx->prize_state_1 = out.prize_state_1; //(prize_state_1>>24)&0xFF谁与争锋领奖
    //(prize_state>>16)&0xFF表示最大弟子资质的值 (prize_state>>8)&0xFF表示领取资质奖励的状态
    //(prize_state&0xFF)侠义无双奖励 prize_state&(1<<7)表示是否领取了奖励 其他表示抽取到的位数
	pDataEx->login_count = out.login_count;   //登录天数
	pDataEx->prize_state_2 = out.prize_state_2;	//封测状态值
    pDataEx->worship_fairy = out.worship_fairy; //参拜神仙姐姐 worship_fairy>>16 表示当前是连续第几天, worship_fairy&0x01表示今天是否参拜过
    pDataEx->gift_info = out.gift_info;//奖励及领奖信息,gift_info>>16 低0-4位对应5个奖品条件是否满足,gift_info 低0-4位对应5个奖品是否已经被领取
	pDataEx->have_offline_msg = out.chat_info & 0x1;   //是否有离线消息 0 无 1有
	pDataEx->have_friend_req = out.chat_info & 0x2;   //是否有好友请求 0 无 1有
    if (out.num_vec.size() == 2)
    {
        pDataEx->daily_count = out.num_vec.at(0);
        pDataEx->curHammerCnt = out.num_vec.at(1);
    }
    
    pDataEx->active_degree = out.active_degree;
    pDataEx->caculateTodayFirstLogin(out.last_login_tm);
    TimerManager::getInstance()->synchTime(&out);

    //这儿用于12点后刷新领奖状态用
    CCNotificationCenter::sharedNotificationCenter()->postNotification(HAS_DAILY_NOTE);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(OnlineManager::heartbeat), this);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(OnlineManager::heartbeat), this, ONLINE_KEEP_ALIVE_SEC, false);
    
    if (!GameManager::sharedInstance()->getBattleLayer() && !GameManager::sharedInstance()->getCommunityLayer())//登录界面跳转社区
    {
        LogicManager::shareLogicManager()->loginToOnlineSucc();
    }
    else if(GameManager::sharedInstance()->getBattleLayer())//战斗界面继续战斗
    {
        if (GameManager::sharedInstance()->getBattleLayer()->m_bHasGetAward)
        {
            LogicManager::shareLogicManager()->loginToOnlineSucc();
        }
        else
        {
            CCNotificationCenter::sharedNotificationCenter()->postNotification(BATTLERECONNECT_NOTE);
        }
    }
    else//社区界面
    {
        if (m_bFromLogin == true)//在社区断线重连
        {
            LogicManager::shareLogicManager()->loginToOnlineSucc();
        }
        else//每日12点拉取loginex
        {
        
        }
    }
}

//PROTO_USER_CHAT_CMD          = 630,//客户端发送聊天消息到服务器
int OnlineManager::userChatToOnline(int nChannelId, int nTargetId, const char* pMsg)
{
    cli_user_chat_in in;
    in.channel_id = nChannelId;
    in.target_id = nTargetId;
    sprintf(in.msg, "%s", pMsg);
    in.mlen = strlen(pMsg);
    return this->sendDataToOnline(KSEQ, PROTO_USER_CHAT_CMD, &in);
}
void OnlineManager::userChatToOnlineCbk(byte_array_t& ba)
{
    cli_user_chat_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    
    //21：收到好友请求。22：收到对方通过好友的消息。23：收到对方删除好友的消息
    if (out.channel_id == 21 || out.channel_id == 22 || out.channel_id == 23)
    {
        if (out.channel_id == 21)
        {
            gUserData->m_dataEx->have_friend_req += 1;
            CCNotificationCenter::sharedNotificationCenter()->postNotification(FRIEND_REQUEST_NOTE, NULL);
        }
        CCNotificationCenter::sharedNotificationCenter()->postNotification(REFRESH_FRIEND_LIST_NOTE, NULL);
        return;
    }
    else if (out.channel_id == 601 || out.channel_id == 602 || out.channel_id == 603 || out.channel_id == 605 || out.channel_id == 607 || out.channel_id == 608 || out.channel_id == 610)
    {
        gUserData->m_dataEx->moveNoticeVec.push_back(out);
    }
    else if (out.channel_id == 600 || out.channel_id == 604 || out.channel_id == 606 || out.channel_id == 609 || out.channel_id == 611 || out.channel_id == 613)
    {
        gUserData->m_dataEx->fixNoticeVec.push_back(out);
        //return;
    }
    
    messageBody_t t;
    t.nUserID = out.userid;
    t.nTime = out.chattime;
    t.nick = (char*)out.nick;
    t.nLevel = out.level;
    t.nUnitID = out.unit_id;
    t.nVipLevel = out.vip_level;
    t.nChannelID = out.channel_id;
    t.nTargetID = out.target_id;
    t.msg = out.msg;
    switch (out.channel_id)
    {
        case 1:
            CommonUtility::insertMessage(gUserData->GlobalMessages, "Global", gUserData->server_id, gUserData->userid, t, GLOBAL_MESSAGE_LIMIT);
            CCNotificationCenter::sharedNotificationCenter()->postNotification(GLOBAL_MESSAGE_UPDATE, NULL);
            break;
        case 601:
        {
            t.nUnitID = 0;
            notice_message_t* msg = (notice_message_t*)out.msg;
            if (msg->items.type == 1)
            {
                item_unit_t item = *msg->items.items;//掉落ITEM
                char str[512] = {0};
                const char* pName = GameData::getItem(item.item_id)->name;
                sprintf(str, "【%s】掌门在战斗中击败小偷，人品爆发获得%d%s", out.nick, item.item_cnt, pName);
                t.msg = str;
            }
            CommonUtility::insertMessage(gUserData->GlobalMessages, "Global", gUserData->server_id, gUserData->userid, t, GLOBAL_MESSAGE_LIMIT);
            CCNotificationCenter::sharedNotificationCenter()->postNotification(GLOBAL_MESSAGE_UPDATE, NULL);
        }
            break;
        case 602:
        {
            t.nUnitID = 0;
            notice_message_t* msg = (notice_message_t*)out.msg;
            if (msg->times.type == 1)
            {
                char str[512] = {0};
                sprintf(str, "【%s】掌门在竞技场大杀特杀达到【%d】连杀！", out.nick, msg->times.cnt);
                t.msg = str;
            }
            CommonUtility::insertMessage(gUserData->GlobalMessages, "Global", gUserData->server_id, gUserData->userid, t, GLOBAL_MESSAGE_LIMIT);
            CCNotificationCenter::sharedNotificationCenter()->postNotification(GLOBAL_MESSAGE_UPDATE, NULL);
        }
            break;
        case 603:
            CommonUtility::insertMessage(gUserData->GlobalMessages, "Global", gUserData->server_id, gUserData->userid, t, GLOBAL_MESSAGE_LIMIT);
            CCNotificationCenter::sharedNotificationCenter()->postNotification(GLOBAL_MESSAGE_UPDATE, NULL);
            break;
            
        case 604:
            gUserDataEx->hasStartedActivity = true;
            CCNotificationCenter::sharedNotificationCenter()->postNotification(HAS_DAILY_NOTE);
            break;
        case 605:
        {
            t.nUnitID = 0;
            notice_message_t* msg = (notice_message_t*)out.msg;
            if (msg->partner.type == 1)
            {
                char str[512] = {0};
                int partnerId = msg->partner.partner_id;
                sprintf(str, "恭喜【%s】掌门招募到弟子【%s】！", out.nick, GameData::getPartnerConf(partnerId)->strName);
                t.msg = str;
            }
            CommonUtility::insertMessage(gUserData->GlobalMessages, "Global", gUserData->server_id, gUserData->userid, t, GLOBAL_MESSAGE_LIMIT);
            CCNotificationCenter::sharedNotificationCenter()->postNotification(GLOBAL_MESSAGE_UPDATE, NULL);
        }
            break;
        case 606:
        {
            t.nUnitID = 0;
            notice_message_t* msg = (notice_message_t*)out.msg;
            
            char str[512] = {0};
            sprintf(str, "【%s】掌门人品爆发，从【%s】中发现了【%s】！", out.nick, GameData::getItem(msg->openbox.box_item_id)->name, GameData::getItem(msg->openbox.item_id)->name);
            t.msg = str;

            CommonUtility::insertMessage(gUserData->GlobalMessages, "Global", gUserData->server_id, gUserData->userid, t, GLOBAL_MESSAGE_LIMIT);
            CCNotificationCenter::sharedNotificationCenter()->postNotification(GLOBAL_MESSAGE_UPDATE, NULL);
        }
            break;
        case 608:
        {
            t.nUnitID = 0;
            notice_message_t* msg = (notice_message_t*)out.msg;
            
            char str[512] = {0};
            sprintf(str, "%s", msg->welcome.buf);
            t.msg = str;
            
            m_welcomeMsg = t;
            
            if (msg->welcome.frequency != 0)
            {
                this->showWelcomeMessage(0);
                CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(OnlineManager::showWelcomeMessage), this);
                CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(OnlineManager::showWelcomeMessage), this, 60 * msg->welcome.frequency, false);
            }
        }
            break;
        case 612:
        {
            t.nUnitID = 0;
            notice_message_t* msg = (notice_message_t*)out.msg;
            
            char str[512] = {0};
            sprintf(str, "【%s】掌门深得神仙姐姐喜爱，获得%s！", out.nick, GameData::getItem(msg->worship_fairy.item_id)->name);
            t.msg = str;
            
            CommonUtility::insertMessage(gUserData->GlobalMessages, "Global", gUserData->server_id, gUserData->userid, t, GLOBAL_MESSAGE_LIMIT);
            CCNotificationCenter::sharedNotificationCenter()->postNotification(GLOBAL_MESSAGE_UPDATE, NULL);
        }
            break;
        case 2:
        {
            CommonUtility::insertMessage(gUserData->FriendMessages, "Friend", gUserData->server_id, gUserData->userid, t, FRIEND_MESSAGE_LIMIT);
            if (t.nUserID != gUserData->userid)
            {
                std::map<int, int>::iterator it = gUserData->NewFriendMessageCount.find(t.nUserID);
                if (it != gUserData->NewFriendMessageCount.end())
                {
                    it->second += 1;
                }
                else
                {
                    gUserData->NewFriendMessageCount.insert(std::pair<int, int>(t.nUserID, 1));
                }
                gUserData->m_dataEx->have_offline_msg += 1;
                CCNotificationCenter::sharedNotificationCenter()->postNotification(MESSAGE_NUMBER_UPDATE, NULL);
            }
            CCNotificationCenter::sharedNotificationCenter()->postNotification(FRIEND_MESSAGE_UPDATE, NULL);
        }
            break;
        default:
            break;
    }
}

void OnlineManager::showWelcomeMessage(float dt)
{
    CommonUtility::insertMessage(gUserData->GlobalMessages, "Global", gUserData->server_id, gUserData->userid, m_welcomeMsg, GLOBAL_MESSAGE_LIMIT);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(GLOBAL_MESSAGE_UPDATE, NULL);
}

//PROTO_USER_USE_ITEM_CMD           = 503,  //使用道具
int OnlineManager::userUseItem(uint32_t packageId, uint32_t itemId)
{
    cli_user_use_item_in in;
    in.package_id = packageId;
    in.item_id = itemId;
    return this->sendDataToOnline(KSEQ, PROTO_USER_USE_ITEM_CMD, &in);
}

void OnlineManager::userUseItemCbk(cli_proto_head_t* head, byte_array_t& ba)
{
    cli_user_use_item_out out;
    out.read_from_buf(ba);
    
    int itemId = out.state;
    gUserData->removeBagItemByID(itemId);
    
    for (std::vector<package_item_t>::iterator it = out.item_vec.begin(); it != out.item_vec.end(); it ++) {
        
        switch ((*it).item_id)
        {
            case 0:
                gUserData->addYXB((*it).item_cnt);
                break;
            case 1:
                gUserData->addExp((*it).item_cnt);
                break;
            case 2:
                gUserData->addGold((*it).item_cnt);
                break;
            case 3:
                gUserData->addPower((*it).item_cnt);
                break;
            case 4:
                gUserData->addNerve((*it).item_cnt);
                break;
            case 5:
                gUserData->addEnery((*it).item_cnt);
                break;
            case 6:
                gUserData->addPrestige((*it).item_cnt);
                break;
            case 7://炼气值，只有英雄能增加炼气值？
                gUserData->m_heroData->addInForceExp((*it).item_cnt);
                break;
            case 8://星石经验
                gUserData->addStarStoneExp((*it).item_cnt);
                break;
            default:
                gUserData->increaseBag(*it);
                break;
        }
    }

    NotificeObject *object = NotificeObject::create();
    object->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(SWAPCOMPLETE_NOTE, object);
    userGetPackageToOnline();
}

//PROOT_USER_LOGIN_SIGN_CMD		= 621,		//签到,sign_type = 0是签当天的 1是补签
int OnlineManager::userLoginSignInToOnline(uint32_t nSignType)
{
    cli_user_login_sign_in in;
    in.sign_type = nSignType;
    return this->sendDataToOnline(KSEQ, PROTO_USER_LOGIN_SIGN_CMD, &in);
}

void OnlineManager::userLoginSignInToOnlineCbk(byte_array_t& ba)
{
    cli_user_login_sign_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    gUserDataEx->setActiveDgreeValue(2);
    gUserData->addGold(-out.cost_gold);
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(LOG_IN_SIGN_IN_NOTE, NULL);
}

int OnlineManager::userGetMissionToOnline(int communitID)
{
    cli_user_get_mission_in in;
    in.map_id = communitID;
    return this->sendDataToOnline(KSEQ, PROTO_USER_GET_MISSION_CMD, &in);
}

void OnlineManager::userGetMissionToOnlineCbk(byte_array_t& ba)
{
    cli_user_get_mission_out out;
    ba.init_postion();
    out.read_from_buf(ba);
    NotificeObject *object = NotificeObject::create();
    object->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(GETMISSIONCOMPLETE_NOTE, object);
}

int OnlineManager::userGetHardMissionToOnline(int communitID)
{
    cli_user_get_mission_in in;
    in.map_id = communitID;
    return this->sendDataToOnline(KSEQ, PROTO_USER_GET_HARD_MISSION_CMD, &in);
}

void OnlineManager::userGetHardMissionToOnlineCbk(byte_array_t& ba)
{
    cli_user_get_hard_mission_out out;
    ba.init_postion();
    out.read_from_buf(ba);
    NotificeObject *object = NotificeObject::create();
    object->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(GETMISSIONCOMPLETE_NOTE, object);
}

int OnlineManager::userRecoverEneryToOnline()
{
    return this->sendDataToOnline(KSEQ, PROTO_USER_RECOVER_ENERY_CMD, NULL);
}

void OnlineManager::userRecoverEneryToOnlineCbk(byte_array_t& ba)
{
    cli_user_recover_enery_out out;
    ba.init_postion();
    out.read_from_buf(ba);
    gUserData->setMaxEnery(out.enery_max);
    gUserData->setEneryTime(out.enery_counter);
    gUserData->setEnery(out.cur_enery);
}

int OnlineManager::userBuyEneryToOnline(int money)
{
    cli_user_buy_enery_in in;
    in.cost_gold = money;
    return this->sendDataToOnline(KSEQ, PROTO_USER_BUY_ENERY_CMD, &in);
}

void OnlineManager::userBuyEneryToOnlineCbk(byte_array_t& ba)
{
    cli_user_buy_enery_out out;
    ba.init_postion();
    out.read_from_buf(ba);
    gUserDataEx->setActiveDgreeValue(1);
    gUserData->addBuyEneryCount(-1);
    gUserData->setEnery(out.cur_enery);
    gUserData->addGold(-out.cost_gold);
}

int OnlineManager::userExpandPackageToOnline(int index)
{
    cli_user_expand_package_in in;
    in.package_index = index;
    m_nBuyBagCount = index;
    return this->sendDataToOnline(KSEQ, PROTO_USER_EXPAND_PACKAGE_CMD, &in);
}

void OnlineManager::userExpandPackageToOnlineCbk(byte_array_t& ba)
{
    cli_user_expand_package_out out;
    ba.init_postion();
    out.read_from_buf(ba);
    gUserData->addGold(-out.cost_gold);
    gUserData->package_size = m_nBuyBagCount;
    char str[64] = {0};
    sprintf(str, "背包空间+%d",m_nBuyBagCount);
    XYTopLayer::getInstance()->ShowTip(str);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(BAGSIZECHANGE_NOTE);
}

int OnlineManager::userGetPackageToOnline()
{
    return  this->sendDataToOnline(KSEQ, PROTO_USER_GET_PACKAGE_CMD, NULL);
}

void OnlineManager::userGetPackageToOnlineCbk(byte_array_t& ba)
{
    cli_user_get_package_out out;
    ba.init_postion();
    out.read_from_buf(ba);
    gUserData->package_item_sum = out.item_sum;
    gUserData->package_size =  out.package_size;
    gUserData->setEquips(out.equipment_vec);
    gUserData->setBagItems(out.item_vec);
    gUserData->setBagCards(out.card_vec);
    gUserData->setBagStuff(out.stuff_vec);

    CCNotificationCenter::sharedNotificationCenter()->postNotification(BAGCHANGE_NOTE);
}

void OnlineManager::heartbeat(float)
{
    this->sendDataToOnline(KSEQ, PROTO_USER_KEEP_ALIVE_CMD, NULL);
}

void OnlineManager::userKeepAliveCbk(int ret, byte_array_t& ba)
{
    if (ret == 0)
    {
        cli_user_keep_alive_out out;
        out.read_from_buf(ba);
        gUserDataEx->server_time = out.time;
        struct tm* pLocal = localtime(&gUserDataEx->server_time);
        //如果是0点0分，则重新拉一遍601协议
        if (pLocal->tm_hour == 0 && pLocal->tm_min == 0)
        {
            m_bFromLogin = false;
            this->userLoginExToOnline();
        }
        CCNotificationCenter::sharedNotificationCenter()->postNotification(KEEP_ALIVE_NOTE);
    }
}

//PROTO_USER_BUY_HAMMER_CMD = 567,  //购买用于砸罐子的锤子
int OnlineManager::userBuyHammer(int nCostGold)
{
    cli_user_buy_hammer_in in;
    in.cost_gold = nCostGold;
    return this->sendDataToOnline(KSEQ, PROTO_USER_BUY_HAMMER_CMD, &in);
}
void OnlineManager::userBuyHammerCbk(byte_array_t& ba)
{
    cli_user_buy_hammer_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    NotificeObject *object = NotificeObject::create();
    object->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(BUY_HAMMER_SUCC_NOTE, object);
}

//PROTO_USER_GET_ITEM_LIST_COUNT_CMD	= 502, 	//用户拉取指定itemid物品的数量
int OnlineManager::userGetItemListCountToOnline(std::vector<uint32_t>* pItemVec, int nReason)
{
    m_nGetItemReason = nReason;
    cli_user_get_item_list_count_in in;
    for (std::vector<uint32_t>::iterator it = pItemVec->begin(); it != pItemVec->end(); it++)
    {
        in.item_vec.push_back(*it);
    }
    return this->sendDataToOnline(KSEQ, PROTO_USER_GET_ITEM_LIST_COUNT_CMD, &in);
}

void OnlineManager::userGetItemListCountToOnlineCbk(byte_array_t& ba)
{
    cli_user_get_item_list_count_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    

    NotificeObject *object = NotificeObject::create();
    object->msg = &out;
    switch (m_nGetItemReason)
    {
        case GET_HAMMER_COUNT:
        {
            CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_HAMMER_COUNT_NOTE, object);
            break;
        }
        case GET_PILL_COUNT:
        {
            CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_PILL_COUNT_NOTE, object);
            break;
        }
        case GET_KEY_COUNT:
        {
            CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_KEY_COUNT_NOTE, object);
            break;
        }
        case GET_MEDICINE_COUNT:
        {
            CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_MEDICINE_COUNT_NOTE, object);
            break;
        }
        case GET_ITEM_FROM_SHOP:
        {
            CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_ITEM_COUNT_FROM_SHOP, object);
            break;
        }
        case GET_DIAMOND_COUNT:
        {
            CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_DIAMOND_COUNT_NOTE, object);
            break;
        }
        case GET_PARTNERSKILL_ITEM_COUNT:
        {
            CCNotificationCenter::sharedNotificationCenter()->postNotification(PARTNER_SKILL_ITEM_NOTE, object);
            break;
        }
        default:
            break;
    }
}

//PROTO_USER_GET_MAGIC_STORE_CMD	= 610,	//获得神秘商店
int OnlineManager::userGetMagicStoreToOnline()
{
    return this->sendDataToOnline(KSEQ, PROTO_USER_GET_MAGIC_STORE_CMD, NULL);
}
void OnlineManager::userGetMagicStoreToOnlineCbk(byte_array_t& ba)
{
    cli_user_get_magic_store_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    
    NotificeObject *object = NotificeObject::create();
    object->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_MAGIC_STORE_NOTE, object);
}

//PROTO_USER_BUY_MAGIC_ITEM_CMD	= 611,	//购买神秘商店中的物品
int OnlineManager::userBuyMagicItemToOnline(uint32_t nItemIndex, uint32_t nCostYxb)
{
    cli_user_buy_magic_item_in in;
    in.item_index = nItemIndex;
    in.cost_yxb = nCostYxb;
    return  this->sendDataToOnline(KSEQ, PROTO_USER_BUY_MAGIC_ITEM_CMD, &in);
}
void OnlineManager::userBuyMagicItemToOnlineCbk(byte_array_t& ba)
{
    cli_user_buy_magic_item_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    
    NotificeObject *object = NotificeObject::create();
    object->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(BUY_MAGIC_ITEM_NOTE, object);
}

//PROTO_USER_FLUSH_MAGIC_STORE_CMD= 612,	//刷新神秘商店
int OnlineManager::userFlushMagicStoreToOnline()
{
    return this->sendDataToOnline(KSEQ, PROTO_USER_FLUSH_MAGIC_STORE_CMD, NULL);
}
void OnlineManager::userFlushMagicStoreToOnlineCbk(byte_array_t& ba)
{
    cli_user_flush_magic_store_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    
    NotificeObject *object = NotificeObject::create();
    object->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(FLUSH_MAGIC_STORE, object);
}

int OnlineManager::userBuyItemToOnline(uint32_t itemid,uint32_t itemcnt)
{
    cli_user_buy_item_in in;
    in.item_id = itemid;
    in.item_cnt = itemcnt;
    return this->sendDataToOnline(KSEQ, PROTO_USER_BUY_ITEM_CMD, &in);
}

void OnlineManager::userBuyItemToOnlineCbk(byte_array_t& ba)
{
    cli_user_buy_item_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    gUserData->addYXB(-out.cost_yxb);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(BUY_NPC_ITEM_NOTE, NULL);
}

int OnlineManager::userGetSkillToOnline(uint32_t user_id)
{
    cli_user_get_skill_in in;
    in.userid = gUserData->userid;
    return this->sendDataToOnline(KSEQ, PROTO_USER_GET_SKILL_CMD, &in);
}

void OnlineManager::userGetSkillToOnlineCbk(byte_array_t& ba)
{
    cli_user_get_skill_out out;
    out.read_from_buf(ba);
    NotificeObject *object = NotificeObject::create();
    object->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(GETSKILLCOMPLETE_NOTE,object);
}

int OnlineManager::userUpgradeSkillToOnline(uint32_t skill_id,uint32_t skill_level)
{
    cli_user_upgrade_skill_in in;
    in.skill_id = skill_id;
    in.skill_level = skill_level;
    return this->sendDataToOnline(KSEQ, PROTO_USER_UPGRADE_SKILL_CMD, &in);
}

void OnlineManager::userUpgradeSkillToOnlineCbk(byte_array_t& ba)
{
    cli_user_upgrade_skill_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return;
    }
    
    gUserData->addYXB(-out.cost_yxb);
    gUserData->addPower(-out.cost_power);
    NotificeObject *object = NotificeObject::create();
    object->msg = &out;
    gUserData->m_heroData->skillLvChange(out.skill_id,out.skill_level);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(SKILLCHANGE_NOTE,object);
}
int OnlineManager::userChangeSkillToOnline(uint32_t skill_id,uint32_t wear_seat)
{
    cli_user_change_skill_in in;
    in.skill_id = skill_id;
    in.wear_seat = wear_seat;
    return this->sendDataToOnline(KSEQ, PROTO_USER_CHANGE_SKILL_CMD, &in);
}

void OnlineManager::userChangeSkillToOnlineCbk(byte_array_t& ba)
{
    cli_user_change_skill_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return;
    }
    memcpy(&gUserData->m_heroData->skill, &out.skills, sizeof(out.skills));
    CCNotificationCenter::sharedNotificationCenter()->postNotification(SKILLLOADCOMPLETE_NOTE);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(SKILL_NEW_OPEN_NOTE);
}

int OnlineManager::userActionToOnline(float posx, float posy, int actionType, int communityID)
{
    cli_user_action_in in;
    in.action_type = actionType;
    in.pos_x = posx;
    in.pos_y = posy;
    in.community_id = communityID;
    return this->sendDataToOnline(KSEQ, PROTO_USER_ACTION_CMD, &in);
}

void OnlineManager::userActionToOnlineCbk(byte_array_t& ba)
{
    cli_user_action_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }

    if (gUserData->getCurCommunityID() == out.community_id)
    {
        if (out.action_type == e_os_user_in_map) //    = 10,   //地图中所有用户的信息
        {
            gUserData->nTotalUserCount = out.total_user_cnt;
        }
        
        
        std::vector<user_pos_info_t>* pInfo = &(out.user_pos_vec);
        for (std::vector<user_pos_info_t>::iterator it = pInfo->begin(); it != pInfo->end(); it++)
        {
            if (it->unit_id == 0) //传的数据有误
            {
                CCLOGWARN("Server data error!");
            }
            else
            {
                user_pos_info_t info = (*it);
            }
        }
    }
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(POST_USER_CNT_NOTE);
}

int OnlineManager::userSellItemToOnline(uint32_t package_id,uint32_t item_id,uint32_t item_cnt)
{
    cli_user_sell_item_in in;
    in.package_id = package_id;
    in.item_id = item_id;
    in.item_cnt = item_cnt;
    return this->sendDataToOnline(KSEQ, PROTO_USER_SELL_ITEM_CMD, &in);
}
void OnlineManager::userSellItemToOnlineCbk(byte_array_t& ba)
{
    cli_user_sell_item_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    gUserData->reduceBag(out.package_id,out.item_id,out.item_cnt);
    gUserData->addYXB(out.add_yxb);
    
    gUserData->package_item_sum -= 1;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(BAGSIZECHANGE_NOTE);
    XYTopLayer::getInstance()->ShowSimpleTip(CCArray::create(CCString::createWithFormat("获得 %d 银两", out.add_yxb), NULL));
}

int OnlineManager::equipmentWearToOnline(uint32_t itemid)
{
    cli_user_wear_equipment_in in;
    in.equipment_id = itemid;
    
    return this->sendDataToOnline(KSEQ, PROTO_USER_WEAR_EQUIPMENT_CMD, &in);
}

void OnlineManager::equipmentWearToOnlineCbk(byte_array_t& ba)
{
    cli_user_wear_equipment_out out;
    out.read_from_buf(ba);
    
    RoleData *pRole = gUserData->m_heroData;
    
    pRole->addEquipItem(out.equipment);
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(WEAR_EQUIPMENT_NOTE);
}

int OnlineManager::userPunchEquipment(int package_id, int item_id, int position)
{
    cli_user_punch_equipment_in in;
    in.package_id = package_id;
    in.item_id = item_id;
    in.position = position;
    
    return this->sendDataToOnline(KSEQ, PROTO_USER_PUNCH_EQUIPMENT_CMD, &in);
}

void OnlineManager::userPunchEquipmentCbk(byte_array_t& ba)
{
    CCNotificationCenter::sharedNotificationCenter()->postNotification(PUNCH_EQUIPMENT_NOTE);
}

int OnlineManager::userInlayGem(uint32_t op_type, uint32_t package_id, uint32_t item_id, uint32_t position, uint32_t gem_id)
{
    cli_user_inlay_gem_in in;
    in.op_type = op_type;
    in.package_id = package_id;
    in.item_id = item_id;
    in.position = position;
    in.gem_id = gem_id;
    
    
    return this->sendDataToOnline(KSEQ, PROTO_USER_INLAY_GEM_CMD, &in);
}

void OnlineManager::userInlayGemCbk(byte_array_t& ba)
{
    cli_user_inlay_gem_out out;
    out.read_from_buf(ba);
    NotificeObject* obj = NotificeObject::create();
    obj->msg = &out;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(INLAY_GEM_SUCC_NOTE, obj);
}

//PROTO_USER_COMPOSE_GEM_CMD		= 526,	//宝石合成
int OnlineManager::userComposeGem(uint32_t gem_id)
{
    cli_user_compose_gem_in in;
    in.gem_id = gem_id;
    
    return this->sendDataToOnline(KSEQ, PROTO_USER_COMPOSE_GEM_CMD, &in);
}

void OnlineManager::userComposeGemCbk(byte_array_t& ba)
{
    cli_user_compose_gem_out out;
    out.read_from_buf(ba);
    NotificeObject* obj = NotificeObject::create();
    obj->msg = &out;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(COMPOSE_GEM_SUCC_NOTE, obj);
}

int OnlineManager::equipmentChangeToOnline(uint32_t unit_id, uint32_t packid,uint32_t itemid,uint32_t iswear)
{
    cli_user_change_equipment_in in;
    in.unit_id = unit_id;
    in.package_id = packid;
    in.equipment_id = itemid;
    in.is_wear = iswear;
    return this->sendDataToOnline(KSEQ, PROTO_USER_CHANGE_EQUIPMENT_CMD, &in);
}

void OnlineManager::equipmentChangeToOnlineCbk(byte_array_t& ba)
{
    cli_user_change_equipment_out out;
    out.read_from_buf(ba);
    RoleData *pRole = gUserData->getRoleByID(out.unit_id);
    if (pRole != NULL)
    {
        if (out.is_wear == 0)//脱下
        {
            int euiiptype = out.on_old_package_id%1000;
            package_equipment_t unit = pRole->userEquip[euiiptype];
            pRole->removeEquipItem(euiiptype);
            unit.package_id = out.on_package_id;
            gUserData->addBagEquipItem(unit);
        }
        else//穿上
        {
            package_equipment_t unit = gUserData->BagEquips[out.on_old_package_id];
            gUserData->removeBagEquipItem(out.on_old_package_id);
            unit.package_id = out.on_package_id;
            if (out.off_old_package_id != 0 && out.off_package_id != 0)//替换装备
            {
                if(pRole->userEquip.end()!= pRole->userEquip.find(out.off_old_package_id%1000))
                {
                    package_equipment_t unitold = pRole->userEquip[out.off_old_package_id%1000];
                    pRole->removeEquipItem(GameData::getItem(unitold.equipment_id)->u.equipment.nItemType);
                    unitold.package_id = out.off_package_id;
                    gUserData->addBagEquipItem(unitold);
                }
            }
            pRole->addEquipItem(unit);
        }
        CCNotificationCenter::sharedNotificationCenter()->postNotification(ATTRBITECHANGE_NOTE);
    }
}

int OnlineManager::userTrainToOnline(uint32_t unitID,uint32_t traintype)
{
    cli_user_train_base_attr_in in;
    in.unit_id = unitID;
    in.train_type = traintype;
    return this->sendDataToOnline(KSEQ, PROTO_USER_TRAIN_BASE_ATTR_CMD, &in);
}

void OnlineManager::userTrainToOnlineCbk(byte_array_t& ba)
{
    cli_user_train_base_attr_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    
    gUserDataEx->setActiveDgreeValue(12);

    CCDictionary *dicAddattribute = CCDictionary::create();
    dicAddattribute->setObject(CCString::createWithFormat("%d",out.train_hp), "hp");
    dicAddattribute->setObject(CCString::createWithFormat("%d",out.train_out_attack), "outattack");
    dicAddattribute->setObject(CCString::createWithFormat("%d",out.train_out_def), "outdef");
    dicAddattribute->setObject(CCString::createWithFormat("%d",out.train_in_attack), "inattack");
    dicAddattribute->setObject(CCString::createWithFormat("%d",out.train_in_def), "indef");
    dicAddattribute->setObject(CCString::createWithFormat("%d",out.train_toughness), "force");
    CCNotificationCenter::sharedNotificationCenter()->postNotification(HEROTRAIN_ADD_NOTE, dicAddattribute);
    getUserAttrToOnline();
}

int OnlineManager::userTrainConfirmToOnline()
{
    return this->sendDataToOnline(KSEQ, PROTO_USER_CONFIRM_TRAIN_CMD, NULL);
}

void OnlineManager::userTrainConfirmToOnlineCbk(byte_array_t& ba)
{
    cli_user_confirm_train_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    
    NotificeObject *object = NotificeObject::create();
    object->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(HEROTRAIN_CONFIRM_NOTE,object);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(ATTRBITECHANGE_NOTE);
}

int OnlineManager::getCDTimeToOnline(uint32_t nType)
{
    cli_user_get_cooldown_in in;
    in.cd_type = nType;
    return this->sendDataToOnline(KSEQ, PROTO_USER_GET_COOLDOWN_CMD, &in);
}

int OnlineManager::getEventCountToOnline(uint32_t nType)
{
    cli_user_get_type_event_in in;
    in.event_type = nType;
    return this->sendDataToOnline(KSEQ, PROTO_USER_GET_TYPE_EVENT_CMD, &in);
}

int OnlineManager::nerveUpgradeToOnline(uint32_t nType, uint32_t nLevel, bool bIsBatch)
{
    cli_user_upgrade_nerve_in in;
    in.is_batch = bIsBatch;
    in.nerve_type = nType;
    in.nerve_id = nLevel;
    return this->sendDataToOnline(KSEQ, PROTO_USER_UPGRADE_NERVE_CMD, &in);
}

void OnlineManager::nerveUpgradeToOnlineCbk(byte_array_t& ba)
{
    cli_user_upgrade_nerve_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    gUserDataEx->setActiveDgreeValue(7,out.inject_times);
    LogicManager::shareLogicManager()->nerveUpgradeToOnlineSucc(out.cd_type, out.cd_time, out.add_level, out.upgrade_succ, out.cost_air, out.cost_gold);
}

int OnlineManager::cancelCoolDownToOnline(uint32_t nType, uint32_t nGold)
{
    cli_user_clear_cooldown_in in;
    in.cd_type = nType;
    in.cost_gold = nGold;
    return this->sendDataToOnline(KSEQ, PROTO_USER_CLEAR_COOLDOWN_CMD, &in);
}

void OnlineManager::cancelCoolDownToOnlineCbk(byte_array_t& ba)
{
    cli_user_clear_cooldown_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    
    LogicManager::shareLogicManager()->cancelCoolDownToOnlineSucc(out.cd_type, out.cost_gold);
}

void OnlineManager::getCDTimeToOnlineCbk(byte_array_t& ba)
{
    cli_user_get_cooldown_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    
    LogicManager::shareLogicManager()->getCDTimeToOnlineSucc(out.cd_type, out.cd_state, out.cd_time);
}

void OnlineManager::getEventCountToOnlineCbk(byte_array_t& ba)
{
    cli_user_get_type_event_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    
    //NotificeObject *object = NotificeObject::create(&out);
    switch (out.event_type)
    {
        case ACTIVITY_DICE_TYPE:
            CCNotificationCenter::sharedNotificationCenter()->postNotification(ACTIVITY_DICE_COUNT_NOTE, CCInteger::create(out.event_count));
            break;
        case DAILY_MONEY_SHAKE_TYPE:
            CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_SHAKE_MONEY_COUNT_NOTE, CCInteger::create(out.event_count));
            break;
        default:
            break;
    }
    
}

int OnlineManager::getDailyEvent(uint32_t nType,uint32_t nReason)
{
    m_nBuyEneryReason = nReason;
    cli_user_get_daily_event_in in;
    in.event_type = nType;
    return this->sendDataToOnline(KSEQ, PROTO_USER_GET_DAILY_EVENT_CMD, &in);
}

void OnlineManager::getDailyEventCbk(byte_array_t& ba)
{
    cli_user_get_type_event_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    
    NotificeObject *object = NotificeObject::create(&out);
    switch (out.event_type)
    {
        case DAILY_PRESTIGE_LOTTERY_TYPE:
            CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_PRESTIGE_CHALLENGE_COUNT, object);
            break;
        case DAILY_BUY_ENERY_TYPE:
            gUserData->setBuyEneryCount(out.event_count);
            if (m_nBuyEneryReason == 0)
            {
                CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_ENERY_COUNT, CCString::createWithFormat("%d", out.event_count));
            }
            else
            {
                CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_AND_BUY_ENERY_COUNT, CCString::createWithFormat("%d", out.event_count));
            }
            break;
        case DAILY_BUY_HAMMER:
            CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_BUY_HAMMER_COUNT_TIMES_NOTE, CCInteger::create(out.event_count));
            break;
        default:
            break;
    }
}

void OnlineManager::addItem(int nItemID, int nItemCount)
{
    switch (nItemID)
    {
        case 0://银两
            gUserData->addYXB(nItemCount);
            break;
        case 1://经验值
            CCAssert(nItemCount >= 0, "Exp can't be negative number.");
            gUserData->addExp(nItemCount);
            break;
        case 2://黄金
            gUserData->addGold(nItemCount);
            break;
        case 3://内功值
            gUserData->addPower(nItemCount);
            break;
        case 4://真气
            gUserData->addNerve(nItemCount);
            break;
        case 5://体力
            gUserData->addEnery(nItemCount);
            break;
        case 6://声望
            gUserData->addPrestige(nItemCount);
            break;
        case 7://炼气值，只有英雄能增加炼气值？
            gUserData->m_heroData->addInForceExp(nItemCount);
            break;
        case 8://星石经验
            gUserData->addStarStoneExp(nItemCount);
            break;
        default:
            break;
    }
}

//PROTO_USER_GET_USER_ONLINE_STATUS_CMD
int OnlineManager::userGetUsersOnlineStatus(std::vector<uint32_t>& uid_vec)
{
    cli_user_get_user_online_status_in in;
    in.uid_vec = uid_vec;
    
    for (std::vector<uint32_t>::iterator it = uid_vec.begin(); it != uid_vec.end(); it++)
    {
        in.uid_vec.push_back(*it);
    }
    
    return this->sendDataToOnline(KSEQ, PROTO_USER_GET_USER_ONLINE_STATUS_CMD, &in);
}

void OnlineManager::userGetUsersOnlineStatusCbk(byte_array_t& ba)
{
    cli_user_get_user_online_status_out out;
    out.read_from_buf(ba);
    
    NotificeObject* obj = NotificeObject::create();
    obj->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_USERS_STATUS_NOTE, obj);
}

//PROTO_USER_GET_FRIEND_LIST_CMD  = 670, //拉取好友信息
int OnlineManager::userGetFriendListToOnline(int nType)
{
    cli_user_get_friend_list_in in;
    in.type = nType;
    return this->sendDataToOnline(KSEQ, PROTO_USER_GET_FRIEND_LIST_CMD, &in);
}
void OnlineManager::userGetFriendListToOnlineCbk(byte_array_t& ba)
{
    cli_user_get_friend_list_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    
    NotificeObject* obj = NotificeObject::create();
    obj->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_FRIEND_LIST_NOTE, obj);
}

//PROTO_USER_FIND_USER_FROM_NICK_CMD = 675,//通过nick找到用户基本信息
int OnlineManager::userFindUserFromNickToOnline(const char* pName)
{
    cli_user_find_user_from_nick_in in;
    sprintf(in.friend_nick, "%s", pName);
    return this->sendDataToOnline(KSEQ, PROTO_USER_FIND_USER_FROM_NICK_CMD, &in);
}
void OnlineManager::userFindUserFromNickToOnlineCbk(byte_array_t& ba)
{
    cli_user_find_user_from_nick_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    
    NotificeObject* obj = NotificeObject::create();
    obj->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(FIND_USER_FROM_NICK_NOTE, obj);
}

//PROTO_USER_ADD_FRIEND_CMD       = 671, //用户添加好友
int OnlineManager::userAddFriendToOnline(int nServerId, int nId, bool bIsBlack)
{
    m_bIsBlack = bIsBlack;
    cli_user_add_friend_in in;
    sprintf(in.friend_nick, "");
    in.svr_id = nServerId;
    in.friend_id = nId;
    in.is_black = bIsBlack;
    return this->sendDataToOnline(KSEQ, PROTO_USER_ADD_FRIEND_CMD, &in);
}

void OnlineManager::userAddFriendToOnlineCbk(byte_array_t& ba)
{
    CCNotificationCenter::sharedNotificationCenter()->postNotification(ADD_FRIEND_NOTE, CCBool::create(m_bIsBlack));
}

//PROTO_USER_DEAL_FRIEND_REQUEST_CMD = 672,//用户处理好友请求
int OnlineManager::userRefuseFriendRequest(std::vector<int>& vec)
{
    cli_user_deal_friend_reqeust_in in;
    for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); it++)
    {
        in.friend_vec.push_back(*it);
    }
    in.opt = 0;
    return this->sendDataToOnline(KSEQ, PROTO_USER_DEAL_FRIEND_REQUEST_CMD, &in);
}

int OnlineManager::userDealFriendRequestToOnline(int nFriendId, int nOpt)
{
    cli_user_deal_friend_reqeust_in in;
    in.friend_vec.push_back(nFriendId);
    in.opt = nOpt;
    return this->sendDataToOnline(KSEQ, PROTO_USER_DEAL_FRIEND_REQUEST_CMD, &in);
}
void OnlineManager::userDealFriendRequestToOnlineCbk(byte_array_t& ba)
{
    CCNotificationCenter::sharedNotificationCenter()->postNotification(DEAL_FRIEND_REQUEST_NOTE, NULL);
}

//PROTO_USER_DEL_FRIEND_CMD       = 673, //用户删除好友
int OnlineManager::userDelFriendToOnline(int nServerId, int nFriendId, bool bIsBlack)
{
    cli_user_del_friend_in in;
    in.svr_id = nServerId;
    in.friend_id = nFriendId;
    in.is_black = bIsBlack;
    return this->sendDataToOnline(KSEQ, PROTO_USER_DEL_FRIEND_CMD, &in);
}
void OnlineManager::userDelFriendToOnlineCbk(byte_array_t& ba)
{
    CCNotificationCenter::sharedNotificationCenter()->postNotification(DEL_FRIEND_NOTE, NULL);
}
//PROTO_USER_GET_PLATFORM_BONUS_CMD = 620,	//领取奖励平台的奖励
int OnlineManager::userGetPlatformBonusToOnline(uint32_t nBonusType, uint32_t nBonusIndex)
{
    cli_user_get_platform_bonus_in in;
    in.bonus_type = nBonusType;
    in.bonus_index = nBonusIndex;
    return this->sendDataToOnline(KSEQ, PROTO_USER_GET_PLATFORM_BONUS_CMD, &in);
}
void OnlineManager::userGetPlatformBonusToOnlineCbk(int ret, byte_array_t& ba)
{
    if (ret == 0)
    {
        cli_user_get_platform_bonus_out out;
        out.read_from_buf(ba);
        if (!ba.is_end())
        {
            CCLOG("报文过多，请检查！");
            return ;
        }
        
        if (out.bonus_type == BONUS_DAILY_WORSHIP_FAIRY)
        {
            gUserDataEx->setActiveDgreeValue(3);
        }
        
        NotificeObject* obj = NotificeObject::create();
        obj->msg = &out;
        if (out.bonus_type == BONUS_OFFLINE || out.bonus_type == BONUS_DAILY_WORSHIP_FAIRY || out.bonus_type == BONUS_INVITE_CODE)
        {
            std::vector<item_unit_t>* pUnit = &(out.add_bonus_vec);
            for (std::vector<item_unit_t>::iterator it = pUnit->begin(); it != pUnit->end(); it++)
            {
                this->addItem(it->item_id, it->item_cnt);
            }
        }
        else if (out.bonus_type == BONUS_TURN_TABLE_TYPE)
        {
            CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_PLATFORM_RAND_BONUS_NOTE, obj);
        }
        else if(out.bonus_type == BONUS_ACTIVE_DEGREE_PRIZE)
        {
            std::vector<item_unit_t>* pUnit = &(out.add_bonus_vec);
            for (std::vector<item_unit_t>::iterator it = pUnit->begin(); it != pUnit->end(); it++)
            {
                this->addItem(it->item_id, it->item_cnt);
            }
            CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_ACTIVE_DEGREE_NOTE, obj);
            
        }
        else if(out.bonus_type == BONUS_DAILY_VIP_FAKE)
        {
            std::vector<item_unit_t>* pUnit = &(out.add_bonus_vec);
            XYTopLayer::getInstance()->ShowTip(pUnit);
            CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_DAILY_VIP_FAKE_NOTE, obj);
            
        }
        else
        {
            const SwapItem_t* pSwap = GameData::getSwapItem(out.swap_id);
            const ItemUnitVec* pDel = &(pSwap->itemIn);
            for (ItemUnitVec::const_iterator it = pDel->begin(); it != pDel->end(); it++)
            {
                this->addItem(it->nItemID, -it->nItemCount);
            }
            
            const ItemUnitVec*  pAdd = &(pSwap->swapItemOut);
            for (ItemUnitVec::const_iterator it = pAdd->begin(); it != pAdd->end(); it++)
            {
                this->addItem(it->nItemID, it->nItemCount);
            }
            for (int i = 0;i < out.add_bonus_vec.size(); i ++)
            {
                item_unit_t item = out.add_bonus_vec.at(i);
                if (getKindOfItem(item.item_id) == kPartnerCardKind)
                {
                    const item_t* itemconf = GameData::getItem(item.item_id);
                    PartnerData *partnerData = PartnerData::create(itemconf->u.card.nPartnerID);
                    partnerData->setExp(0);
                    partnerData->skillLv = 1;
                    partnerData->trainAttr.nLife = 0;
                    partnerData->trainAttr.nInAttack = 0;
                    partnerData->trainAttr.nOutAttack = 0;
                    partnerData->trainAttr.nOutDef = 0;
                    partnerData->trainAttr.nInDef = 0;
                    partnerData->trainAttr.nToughness = 0;
                    partnerData->curseat = 0;
                    partnerData->inheritstate = 0;
                    memset(partnerData->star_array, 0, sizeof(partnerData->star_array));
                    partnerData->curseat = 0;
                    gUserData->m_arrPartner->addObject(partnerData);
                }
            }
        }
        
        if (out.bonus_type == BONUS_ACTIVE_DEGREE_PRIZE || out.bonus_type == BONUS_LOGIN_SIGN_TYPE)
        {
            XYTopLayer::getInstance()->ShowAwardLayer(out.add_bonus_vec);
        }
        
        CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_PLATFORM_ONLINE_BONUS_NOTE, obj);//刷新主界面
        CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_PLATFORM_BONUS_NOTE, obj);
    }
    else
    {
        CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_PLATFORM_RAND_BONUS_NOTE, NULL);
    }
    
}

int OnlineManager::equipmentStrengthenToOnline(uint32_t nPackageId, uint32_t nItemID, bool bIsTeen)
{
    cli_user_strengthen_equipment_in in;
    in.package_id = nPackageId;
    in.item_id = nItemID;
    in.is_teen = bIsTeen;
    return this->sendDataToOnline(KSEQ, PROTO_USER_STRENGTHEN_EQUIPMENT_CMD, &in);
}

void OnlineManager::equipmentStrengthenToOnlineCbk(byte_array_t& ba)
{
    cli_user_strengthen_equipment_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    LogicManager::shareLogicManager()->equipmentStrengthenToOnlineSucc(out.cost_yxb, out.cd_state, out.cd_time);
}

int OnlineManager::getUserAttrToOnline()
{
    return this->sendDataToOnline(KSEQ, PROTO_USER_GET_ATTR_ITEM_CMD, NULL);
}

void OnlineManager::getUserAttrToOnlineCbk(byte_array_t& ba)
{
    cli_user_get_attr_item_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    gUserData->setyxb(out.yxb);
    gUserData->m_heroData->setExp(out.exp);
    
    int delta_gold = out.gold - gUserData->getGold();
    gUserData->addGold(delta_gold);
    
    gUserData->setPower(out.power_value);
    gUserData->setNerveValue(out.nerve_value);
    gUserData->setEnery(out.enery);
    gUserData->setEneryTime(out.enery_counter);
    gUserData->package_item_sum = out.package_cnt;
    gUserData->package_size = out.max_package_size;
    NotificeObject *object = NotificeObject::create();
    object->msg = &out;
}

int OnlineManager::getCardPrizeToOnline(uint32_t nMapID, uint32_t nCostGold, uint32_t nCardState)
{
    cli_user_get_battle_prize_in in;
    in.map_id = nMapID;
    in.cost_gold = nCostGold;
    in.card_state = nCardState;
    return this->sendDataToOnline(KSEQ, PROTO_USER_GET_BATTLE_PRIZE_CMD, &in);
}

void OnlineManager::getCardPrizeToOnlineCbk(byte_array_t& ba)
{
    cli_user_get_battle_prize_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    
    this->addItem(out.card.item_id, out.card.item_cnt);
    
    LogicManager::shareLogicManager()->getCardPrizeToOnlineSucc(&out.card, out.gift_state);
}

int OnlineManager::battleOverToOnline(uint32_t nMapID, uint32_t nOverReason, uint32_t* pKillMonster, uint32_t nAddYxb, uint32_t nAddExp, uint32_t nAddGold, uint32_t nMedicine, std::map<int, int>* pDieMonsterMap, std::map<int, int>* pDropItemMap, std::map<int, int>* pThiefItemMap, uint32_t nMaxCombo, uint32_t nBeHitCount, uint32_t nHpRemainRate)
{
    m_battleBonus.over_reason = nOverReason;
    m_battleBonus.map_id = nMapID;
    for (int i = 0; i < 4; i++)
    {
        m_battleBonus.kill_monster[i] = pKillMonster[i];
    }
    m_battleBonus.add_yxb = nAddYxb;
    m_battleBonus.add_exp = nAddExp;
    m_battleBonus.add_gold = nAddGold;
    m_battleBonus.max_combo = nMaxCombo;
    m_battleBonus.be_hit_count = nBeHitCount;
    
    m_battleBonus.used_item[0] = 0;
    m_battleBonus.used_item[1] = 0;
    m_battleBonus.used_item[2] = 0;
    m_battleBonus.hp_item_cnt = nMedicine;
    m_battleBonus.kill_vec.clear();
    m_battleBonus.item_vec.clear();
    m_battleBonus.thief_item_vec.clear();
    if (pDieMonsterMap != NULL)
    {
        std::vector<kill_monster_info_t>* pVec = &m_battleBonus.kill_vec;
        pVec->clear();
        for (std::map<int, int>::iterator it = pDieMonsterMap->begin(); it != pDieMonsterMap->end(); it++)
        {
            kill_monster_info_t unit;
            unit.monster_id = it->first;
            unit.kill_cnt = it->second;
            pVec->push_back(unit);
        }
    }
    
    if (pDropItemMap != NULL)
    {
        std::vector<item_unit_t>* pVec = &m_battleBonus.item_vec;
        pVec->clear();
        for (std::map<int, int>::iterator it = pDropItemMap->begin(); it != pDropItemMap->end(); it++)
        {
            item_unit_t unit;
            unit.item_id = it->first;
            unit.item_cnt = it->second;
            pVec->push_back(unit);
        }
    }
    
    if (pThiefItemMap != NULL)
    {
        std::vector<item_unit_t>* pVec = &m_battleBonus.thief_item_vec;
        pVec->clear();
        for (std::map<int, int>::iterator it = pThiefItemMap->begin(); it != pThiefItemMap->end(); it++)
        {
            item_unit_t unit;
            unit.item_id = it->first;
            unit.item_cnt = it->second;
            pVec->push_back(unit);
        }
    }
    m_battleBonus.hp_remain_rate = nHpRemainRate;
    return this->sendDataToOnline(KSEQ, PROTO_USER_BATTLE_OVER_CMD, &m_battleBonus);
}

void OnlineManager::battleOverCbk(byte_array_t& ba)
{
    cli_user_battle_over_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    
    for (int i = 0; i < 3; i++)
    {
        if(m_battleBonus.used_item[i] == 10001)
        {
            gUserDataEx->setActiveDgreeValue(18);
        }
    }
    std::vector<item_unit_t>* pVec = &m_battleBonus.item_vec;
    for (std::vector<item_unit_t>::iterator it = pVec->begin(); it != pVec->end(); it++)
    {
        this->addItem(it->item_id, it->item_cnt);
    }
    
    if (out.map_type == 1)
    {
        if (m_battleBonus.map_id > gUserDataEx->getMaxMission())
        {
            gUserDataEx->setMaxMission(m_battleBonus.map_id);
        }
    }
    else if (out.map_type == 6)
    {
        if (m_battleBonus.map_id > gUserDataEx->getMaxHardMission())
        {
            gUserDataEx->setMaxHardMission(m_battleBonus.map_id);
        }
        
    }
    else if(out.map_type == 2)
    {
        if (m_battleBonus.map_id > gUserDataEx->getMaxEliteMission())
        {
            gUserDataEx->setMaxEliteMission(m_battleBonus.map_id);
        }
    }
    LogicManager::shareLogicManager()->battleOverToOnlineSucc(out.map_type,out.first_pass, out.score, &out.first_prize, &out.card_vec, &out.pass_prize, out.first_gold);
}

//train_type:0 确认培养 1普通培养 2指定培养
int OnlineManager::equipmentWashToOnline(uint32_t nTrainType, uint32_t nPackageId, uint32_t nEquipmentId)
{
    cli_user_train_equipment_in in;
    in.train_type = nTrainType;
    in.package_id = nPackageId;
    in.equipment_id = nEquipmentId;
    
    return this->sendDataToOnline(KSEQ, PROTO_USER_TRAIN_EQUIPMENT_CMD, &in);
}

void OnlineManager::equipmentWashToOnlineCbk(byte_array_t& ba)
{
    cli_user_train_equipment_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    gUserDataEx->setActiveDgreeValue(14);
    LogicManager::shareLogicManager()->equipmentWashToOnlineSucc(&out.equipment);
    
}

//PROTO_USER_GET_JAR_LIST_CMD     = 564,  //得到寻宝的罐子的列表
int OnlineManager::getPotListToOnline()
{
    return this->sendDataToOnline(KSEQ, PROTO_USER_GET_JAR_LIST_CMD, NULL);
}
void OnlineManager::getPotListToOnlineCbk(byte_array_t& ba)
{
    cli_user_get_jar_list_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    
    NotificeObject *object = NotificeObject::create();
    object->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_POT_LIST_NOTE, object);
}

//PROTO_USER_BREAK_JAR_CMD        = 565,  //砸罐子
int OnlineManager::potClickToOnline(int nInfo)
{
    cli_user_break_jar_in in;
    in.jar_info = nInfo;
    
    return this->sendDataToOnline(KSEQ, PROTO_USER_BREAK_JAR_CMD, &in);
}
void OnlineManager::potClickToOnlineCbk(byte_array_t& ba)
{
    cli_user_break_jar_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    
    gUserDataEx->setActiveDgreeValue(11);
    gUserDataEx->curHammerCnt --;
    
    NotificeObject *object = NotificeObject::create();
    object->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_POT_BONUS_NOTE, object);
}

int OnlineManager::potUpgradeToOnline(int nType)
{
    cli_user_upgrade_jar_in in;
    in.type = nType;
    
    return this->sendDataToOnline(KSEQ, PROTO_USER_UPGRADE_JAR_CMD, &in);
}
void OnlineManager::potUpgradeToOnlineCbk(byte_array_t& ba)
{
    cli_user_upgrade_jar_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    
    NotificeObject *object = NotificeObject::create();
    object->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(UPGRADE_POT_NOTE, object);
}



//PROTO_USER_USE_PARTNER_CARD_CMD= 556,	//使用弟子卡牌
int OnlineManager::userUsePartnerCard(int nCardId, int nPackageId)
{
    cli_user_use_partner_card_in in;
    in.card_id = nCardId;
    in.package_id = nPackageId;
    return this->sendDataToOnline(KSEQ, PROTO_USER_USE_PARTNER_CARD_CMD, &in);
}

void OnlineManager::userUsePartnerCardCbk(byte_array_t& ba)
{
    CCNotificationCenter::sharedNotificationCenter()->postNotification(USE_PARTNER_CARD_NOTE, NULL);
}

int OnlineManager::equipmentUpgradeToOnline(uint32_t nPackageId, uint32_t nItemId, uint32_t nCostGold)
{
    cli_user_upgrade_equipment_in in;
    in.package_id = nPackageId;
    in.item_id = nItemId;
    in.cost_gold = nCostGold;
    
    return this->sendDataToOnline(KSEQ, PROTO_USER_UPGRADE_EQUIPMENT_CMD, &in);
    /*测试用
    int i = rand() % 4;
    int ret = 0;
    switch (i)
    {
        case 0:
            ret = 0;
            break;
        case 1:
            ret = upgrade_role_grade_not_enough;
            break;
        case 2:
            ret = upgrade_money_not_enough;
            break;
        case 3:
            ret = upgrade_unknown_err;
            break;
    }
    LogicManager::shareLogicManager()->equipmentUpgradeToOnlineSucc(ret);
    return 0;
     */
}

void OnlineManager::equipmentUpgradeToOnlineCbk(byte_array_t& ba)
{
    cli_user_upgrade_equipment_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    LogicManager::shareLogicManager()->equipmentUpgradeToOnlineSucc(out.package_id, out.new_item_id, out.new_item_level, out.cost_gold);
}

int OnlineManager::userUpgradeForce(bool isMedicine, bool isMul, uint32_t nCount)
{
    cli_user_upgrade_force_in in;
    in.type = (isMedicine) ? 1 : 2;
    in.cost_value = nCount;
    return this->sendDataToOnline(KSEQ, PROTO_USER_UPGRADE_FORCE_CMD, &in);
}

void OnlineManager::userUpgradeForceCbk(byte_array_t& ba)
{
    cli_user_upgrade_force_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    
    gUserData->m_heroData->addInForceExp(out.force_exp);
    if (out.type == 2)
    {
        gUserData->addGold(-out.cost_value);
    }
    gUserDataEx->setActiveDgreeValue(6,out.cost_value);
    NotificeObject *object = NotificeObject::create(&out);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(UPGRADE_FORCE_NOTE, object);
}


//PROTO_USER_CHEAT_ITEM_CMD = 1000
int OnlineManager::userCheatItem(uint32_t item_id, int32_t item_cnt)
{
    cli_user_cheat_item_in in;
    in.item_id = item_id;
    in.item_cnt = item_cnt;
    return this->sendDataToOnline(KSEQ, PROTO_USER_CHEAT_ITEM_CMD, &in);
}

void OnlineManager::userCheatItemCbk(byte_array_t& ba)
{
    LogicManager::shareLogicManager()->userCheatItemSucc();
}

//PROTO_USER_START_AUTO_BATTLE_CMD= 543,	//开始扫图
int OnlineManager::userStartAutoBattle(uint32_t mapId, uint32_t battleTimes)
{
    cli_user_start_auto_battle_in in;
    in.map_id = mapId;
    gUserDataEx->storeBattleInfo(mapId, NULL);
    in.battle_times = battleTimes;
    return this->sendDataToOnline(KSEQ, PROTO_USER_START_AUTO_BATTLE_CMD, &in);
}

void OnlineManager::userStartAutoBattleCbk(byte_array_t& ba)
{
    cli_user_start_auto_battle_out out;
    out.read_from_buf(ba);
    
    gUserDataEx->execWantedTask(TASK_PASS_MISSION_TYPE, out.prize_vec.size());
    
    NotificeObject *object = NotificeObject::create();
    object->msg = &out;
    int yxb = getItemCount(&out.prize_vec,0);
    gUserData->addYXB(yxb);
    
    int exp = getItemCount(&out.prize_vec,1);
    gUserData->addExp(exp);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(AUTOBATTLESTART_NOTE, object);
    gUserDataEx->setActiveDgreeValue(4,out.prize_vec.size());
    getUserAttrToOnline();
}

int OnlineManager::userStopAutoBattle()
{
    return this->sendDataToOnline(KSEQ, PROTO_USER_STOP_AUTO_BATTLE_CMD, NULL);
}

void OnlineManager::userStopAutoBattleCbk()
{
    CCNotificationCenter::sharedNotificationCenter()->postNotification(AUTOBATTLEEND_NOTE,NULL);
}

int OnlineManager::userGetRoundsBattle()//扫图获得
{
    return this->sendDataToOnline(KSEQ, PROTO_USER_GET_ROUNDS_BATTLE_CMD, NULL);
}

void OnlineManager::userGetRoundsBattleCbk(byte_array_t& ba)
{
    cli_user_get_rounds_battle_out out;
    out.read_from_buf(ba);
    
    int yxb = getItemCount(&out.prize_vec,0);
    gUserData->addYXB(yxb);
    
    int exp = getItemCount(&out.prize_vec,1);
    gUserData->addExp(exp);
    
    NotificeObject *object = NotificeObject::create();
    object->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(GETROUNDSBATTLE_NOTE, object);
    if (gUserData->getEnery()>=gUserData->getMaxEnery())
    {
        getUserAttrToOnline();
    }
    gUserData->addEnery(-5*out.prize_vec.size());
}

int OnlineManager::userSpeedAutoBattle(uint32_t costGold)//加速扫图
{
    cli_user_speed_auto_battle_in in;
    in.cost_gold = costGold;
    return this->sendDataToOnline(KSEQ, PROTO_USER_SPEED_AUTO_BATTLE_CMD, &in);
}

void OnlineManager::userSpeedAutoBattleCbk(byte_array_t& ba)
{
    cli_user_speed_auto_battle_out out;
    if (!out.read_from_buf(ba))
    {
        CCLOG("OnlineManager::userSpeedAotoBattleCbk");
    }
    int yxb = getItemCount(&out.prize_vec,0);
    gUserData->addYXB(yxb);
    gUserData->addGold(-out.cost_gold);
    int exp = getItemCount(&out.prize_vec,1);
    gUserData->addExp(exp);
    
    NotificeObject *object = NotificeObject::create();
    object->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(SPEEDBATTLE_NOTE, object);
    gUserData->addEnery(-5*out.prize_vec.size());
}

//PROTO_USER_BATTLE_START_CMD		= 547,	//进入关卡

int OnlineManager::userBattleStart(uint32_t nMapID)
{
    cli_user_battle_start_in in;
    in.map_id = nMapID;
    return this->sendDataToOnline(KSEQ, PROTO_USER_BATTLE_START_CMD, &in);
}

void OnlineManager::userBattleStartCbk(byte_array_t& ba)
{
    cli_user_battle_start_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    
    NotificeObject *object = NotificeObject::create();
    object->msg = &out;
    gUserData->nMedicine = out.hp_item_cnt>50?50:out.hp_item_cnt;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(BATTLESTART_NOTE, object);
}

//PROTO_USER_INHERTIT_PARTNER_CMD = 558,	//弟子传承
int OnlineManager::userInheritPartner(int nPartnerId, int nTargetId, int nUseGold)
{
    cli_user_inherit_partner_in in;
    in.partner_id = nPartnerId;
    in.target_id = nTargetId;
    in.use_gold = nUseGold;
    return this->sendDataToOnline(KSEQ, PROTO_USER_INHERIT_PARTNER_CMD, &in);
}
void OnlineManager::userInheritPartnerCbk(byte_array_t& ba)
{
    cli_user_inherit_partner_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    NotificeObject *object = NotificeObject::create();
    object->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(INHERIT_PARTNER_NOTE, object);
}

int OnlineManager::userSwap(uint32_t swap_id)
{
    cli_user_swap_in in;
    in.swap_id = swap_id;
    return this->sendDataToOnline(KSEQ, PROTO_USER_SWAP_CMD, &in);
}

void OnlineManager::userSwapCbk(byte_array_t& ba)
{
    cli_user_swap_out out;
    out.read_from_buf(ba);
    
    for (int i = 0; i < out.item_vec.size(); i ++) {
        package_item_t item = out.item_vec.at(i);
        
        switch (item.item_id)
        {
            case 0:
                gUserData->addYXB(item.item_cnt);
                break;
            case 1:
                gUserData->addExp(item.item_cnt);
                break;
            case 2:
                gUserData->addGold(item.item_cnt);
                break;
            case 3:
                gUserData->addPower(item.item_cnt);
                break;
            case 4:
                gUserData->addNerve(item.item_cnt);
                break;
            case 5:
                gUserData->addEnery(item.item_cnt);
                break;
            case 6:
                gUserData->addPrestige(item.item_cnt);
                break;
            case 7://炼气值，只有英雄能增加炼气值？
                gUserData->m_heroData->addInForceExp(item.item_cnt);
                break;
            case 8://星石经验
                gUserData->addStarStoneExp(item.item_cnt);
                break;
            default:
                gUserData->increaseBag(item);
                break;
        }
    }
    
    NotificeObject *object = NotificeObject::create();
    object->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(SWAPCOMPLETE_NOTE, object);
}

int OnlineManager::userBallot(uint32_t ballot_type)
{
    cli_user_ballot_in in;
    in.ballot_type  = ballot_type;
    return this->sendDataToOnline(KSEQ, PROTO_USER_BALLOT_CMD, &in);
}

void OnlineManager::userBallotCbk(byte_array_t& ba)
{
    cli_user_ballot_out out;
    out.read_from_buf(ba);
    gUserData->addGold(-out.cost_gold);
    gUserData->addYXB(-out.cost_yxb);
    NotificeObject *object = NotificeObject::create();
    object->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(BALLOTCOMPLETE_NOTE, object);
}

//PROTO_USER_GET_ALL_BATTLE_PRIZE_CMD	= 551,	//获得所有老虎机的奖励
int OnlineManager::userGetAllBattlePrize()
{
    return this->sendDataToOnline(KSEQ, PROTO_USER_GET_ALL_BATTLE_PRIZE_CMD, NULL);
}
void OnlineManager::userGetAllBattlePrizeCbk(byte_array_t& ba)
{
    cli_user_get_all_battle_prize_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }

    NotificeObject *object = NotificeObject::create(&out);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(USER_GET_ALL_BATTLE_PRIZE_NOTE, object);
}

//PROTO_USER_ENLIST_PARTNER_CMD	= 570,	//招募侠客
int OnlineManager::userEnlistPartner(uint32_t nPartnerID)
{
    cli_user_enlist_partner_in in;
    in.partner_id = nPartnerID;
    return this->sendDataToOnline(KSEQ, PROTO_USER_ENLIST_PARTNER_CMD, &in);
}

void OnlineManager::userEnlistPartnerCbk(byte_array_t& ba)
{
    cli_user_enlist_partner_out out;
    out.read_from_buf(ba);
    
    const PartnerConf_t* pPartnerConf = GameData::getPartnerConf(out.partner_id);
    if (!pPartnerConf)
    {
        CCLOG("---------------------弟子ID错误-----------------------!");
    }
    PartnerData *partnerData = PartnerData::create(out.partner_id);
    partnerData->setExp(0);
    partnerData->skillLv = 1;
    partnerData->trainAttr.nLife = 0;
    partnerData->trainAttr.nInAttack = 0;
    partnerData->trainAttr.nOutAttack = 0;
    partnerData->trainAttr.nOutDef = 0;
    partnerData->trainAttr.nInDef = 0;
    partnerData->trainAttr.nToughness = 0;
    partnerData->curseat = 0;
    partnerData->inheritstate = 0;
    memset(partnerData->star_array, 0, sizeof(partnerData->star_array));
    partnerData->curseat = 0;
    gUserData->m_arrPartner->addObject(partnerData);
    
    NotificeObject *object = NotificeObject::create();
    object->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(USER_ENLIST_PARTNER_NOTE, object);
}

//PROTO_USER_GET_PARTNER_LIST_CMD = 571,  //侠客列表
int OnlineManager::usergetPartnerList(uint32_t type)
{
    m_nPartnerType = type;
    cli_user_get_partner_list_in in;
    in.type = type;
    return this->sendDataToOnline(KSEQ, PROTO_USER_GET_PARTNER_LIST_CMD, &in);
}

void OnlineManager::usergetPartnerListCbk(byte_array_t& ba)
{
    cli_user_get_partner_list_out out;
    out.read_from_buf(ba);
    
    //0所有弟子 1已上阵弟子 2未上阵弟子 3放逐的弟子
    switch (m_nPartnerType)
    {
        case 0:
        {
            
        }
            break;
        case 2:
        {
            gUserData->m_arrPartner->removeAllObjects();
            for (int i = 0; i<out.partner_vec.size(); i++)
            {
                partner_info_t *info= &out.partner_vec.at(i);
                PartnerData *partnerData = PartnerData::create(info->partner_id);
                partnerData->setExp(info->exp);
                partnerData->skillLv = info->skill_level;
                partnerData->trainAttr.nLife = info->train_hp;
                partnerData->trainAttr.nInAttack = info->train_in_attack;
                partnerData->trainAttr.nOutAttack= info->train_out_attack;
                partnerData->trainAttr.nOutDef= info->train_out_def;
                partnerData->trainAttr.nInDef= info->train_in_def;
                partnerData->trainAttr.nToughness= info->train_toughness;
                partnerData->curseat= info->cur_seat;
                partnerData->inheritstate = info->inherit_state;
                for (int j = 0; j < 4; j++)
                {
                    partnerData->star_array[j] = info->star_array[j];
                }
                for (int i = 0; i<8; i++)
                {
                    if (info->equipment[i].package_id == 0)
                    {
                        continue;
                    }
                    partnerData->userEquip[i+1] = info->equipment[i];
                }
                gUserData->m_arrPartner->addObject(partnerData);
            }
        }
            break;
        case 3:
        {
            NotificeObject *object = NotificeObject::create();
            object->msg = &out;
            CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_EXILE_PARTNERLIST_NOTE, object);
        }
            break;
        default:
            break;
    }
}

int OnlineManager::userUpgradePartnerSkill(uint32_t partnerID)
{
    cli_user_upgrade_partner_skill_in in;
    in.partner_id = partnerID;
    return this->sendDataToOnline(KSEQ, PROTO_USER_UPGRADE_PARTNER_SKILL_CMD, &in);
}

void OnlineManager::userUpgradePartnerSkillCbk(byte_array_t& ba)
{
    cli_user_upgrade_partner_skill_out out;
    out.read_from_buf(ba);
    NotificeObject *object = NotificeObject::create();
    object->msg = &out;
    for (std::vector<item_unit_t>::iterator it = out.cost_vec.begin(); it != out.cost_vec.end(); it++)
    {
        this->addItem(it->item_id, -it->item_cnt);
    }
    CCNotificationCenter::sharedNotificationCenter()->postNotification(PARTNER_SKILLUPGRADE_NOTE, object);
}

//PROTO_USER_ONPLAY_PARTNER_CMD	= 572,	//侠客上阵
int OnlineManager::userOnPlayerPartner(uint32_t partner_id,uint32_t seat_index)
{
    cli_user_onplay_partner_in in;
    in.partner_id = partner_id;
    in.seat_index = seat_index;
    return this->sendDataToOnline(KSEQ, PROTO_USER_ONPLAY_PARTNER_CMD, &in);

}

void OnlineManager::userOnPlayerPartnerCbk(byte_array_t& ba)
{
    cli_user_onplay_partner_out out;
    out.read_from_buf(ba);
    PartnerData *battleMate = NULL;
    battleMate = (PartnerData*)gUserData->getRoleByID(out.on_partner_id);
    
    if (battleMate)
    {
        //把上阵的移到未上阵的数组中
        PartnerData *pData = (PartnerData*)gUserData->m_partnerDic->objectForKey(out.on_seat_index);
        if (pData)
        {
            pData->retain();
            gUserData->m_partnerDic->removeObjectForKey(out.on_seat_index);
            pData->curseat = 0;
            gUserData->m_arrPartner->addObject(pData);
            pData->release();
        }
        //上阵弟子
        battleMate->retain();
        gUserData->removePartnerByID(out.on_partner_id);
        battleMate->curseat = out.on_seat_index;
        gUserData->m_partnerDic->setObject(battleMate, out.on_seat_index);
        battleMate->release();
    }
    CCNotificationCenter::sharedNotificationCenter()->postNotification(PARTNER_ONPLAY_NOTE, NULL);
    userRecoverEneryToOnline();
}

int OnlineManager::userSwapPartnerSeat(uint32_t seat_idx_1,uint32_t seat_idx_2)
{
    cli_user_swap_partner_seat_in in;
    in.seat_idx_1 = seat_idx_1;
    in.seat_idx_2 = seat_idx_2;
    return this->sendDataToOnline(KSEQ, PROTO_USER_SWAP_PARTNER_SEAT_CMD, &in);
}

void OnlineManager::userSwapPartnerSeatCbk(byte_array_t& ba)
{
    cli_user_swap_partner_seat_out out;
    out.read_from_buf(ba);
    PartnerData *obj1 = (PartnerData*)gUserData->m_partnerDic->objectForKey(out.seat_idx_1);
    obj1->curseat = out.seat_idx_2;
    PartnerData *obj2 = (PartnerData*)gUserData->m_partnerDic->objectForKey(out.seat_idx_2);
    obj2->curseat = out.seat_idx_1;
    obj1->retain();
    obj2->retain();
    gUserData->m_partnerDic->removeObjectForKey(out.seat_idx_1);
    gUserData->m_partnerDic->removeObjectForKey(out.seat_idx_2);
    gUserData->m_partnerDic->setObject(obj1, out.seat_idx_2);
    gUserData->m_partnerDic->setObject(obj2, out.seat_idx_1);
    obj1->release();
    obj2->release();
    CCNotificationCenter::sharedNotificationCenter()->postNotification(PARTNER_ONPLAY_NOTE, NULL);
    userRecoverEneryToOnline();
} 

int OnlineManager::userOffPlayPartner(uint32_t seatID)
{
    cli_user_offplay_partner_in in;
    in.seat_id = seatID;
    return this->sendDataToOnline(KSEQ, PROTO_USER_OFFPLAY_PARTNER_CMD, &in);
}

void OnlineManager::userOffPlayPartnerCbk(byte_array_t & ba)
{
    CCNotificationCenter::sharedNotificationCenter()->postNotification(PARTNER_OFFPLAYER_NOTE);
}

int  OnlineManager::userRecallPartner(uint32_t partner_id,uint32_t recall)
{
    cli_user_recall_partner_in in;
    in.partner_id = partner_id;
    in.recall = recall;
    return this->sendDataToOnline(KSEQ,PROTO_USER_RECALL_PARTNER_CMD, &in);
}

void OnlineManager::userRecallPartnerCbk(byte_array_t& ba)
{
    cli_user_recall_partner_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    
    if (out.is_recall == 1)
    {
        partner_info_t *info= &(out.partner);
        PartnerData *partnerData = PartnerData::create(info->partner_id);
        partnerData->setExp(info->exp);
        partnerData->skillLv = info->skill_level;
        partnerData->trainAttr.nLife = info->train_hp;
        partnerData->trainAttr.nInAttack = info->train_in_attack;
        partnerData->trainAttr.nOutAttack= info->train_out_attack;
        partnerData->trainAttr.nOutDef= info->train_out_def;
        partnerData->trainAttr.nInDef= info->train_in_def;
        partnerData->trainAttr.nToughness= info->train_toughness;
        partnerData->curseat= info->cur_seat;
        partnerData->inheritstate = info->inherit_state;
        for (int j = 0; j < 4; j++)
        {
            partnerData->star_array[j] = info->star_array[j];
        }
        for (int i = 0; i < 8; i++)
        {
            if (info->equipment[i].package_id == 0)
            {
                continue;
            }
            partnerData->userEquip[i+1] = info->equipment[i];
        }
        gUserData->m_arrPartner->addObject(partnerData);
    }
    NotificeObject *object = NotificeObject::create(&out);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(PARTNER_DROP_NOTE, object);
}

//PROTO_USER_GET_PRESTIGE_TITLE_CMD	= 579,	//得到声望挑战信息
int OnlineManager::userGetPrestigeTitle()
{
    return this->sendDataToOnline(KSEQ, PROTO_USER_GET_PRESTIGE_TITLE_CMD, NULL);
}

void OnlineManager::userGetPrestigeTitleCbk(byte_array_t& ba)
{
    cli_user_get_prestige_title_out out;
    out.read_from_buf(ba);
    NotificeObject *object = NotificeObject::create(&out);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_PRESTIGE_TITLE, object);
}

//PROTO_USER_GET_PRESTIGE_PRIZE_CMD	= 580,	//领取
int OnlineManager::userGetPrestigePrize()
{
    return this->sendDataToOnline(KSEQ, PROTO_USER_GET_PRESTIGE_PRIZE_CMD, NULL);
}

void OnlineManager::userGetPrestigePrizeCbk(byte_array_t& ba)
{
    cli_user_get_prestige_prize_out out;
    out.read_from_buf(ba);
    NotificeObject *object = NotificeObject::create(&out);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_PRESTIGE_PRIZE, object);
}

//PROTO_USER_PRESTIGE_LOTTERY_CMD	= 581,	//声望摇奖
int OnlineManager::userPrestigeLottery(uint32_t nLotteryType, bool isUseGold)
{
    cli_user_prestige_lottery_in in;
    in.lottery_type = nLotteryType - 1;
    in.is_gold = isUseGold ? 1 : 0;
    return this->sendDataToOnline(KSEQ, PROTO_USER_PRESTIGE_LOTTERY_CMD, &in);
}

void OnlineManager::userPrestigeLotteryCbk(byte_array_t& ba)
{
    cli_user_prestige_lottery_out out;
    out.read_from_buf(ba);
    
    if (out.cost_yxb != 0)
    {
        gUserData->addYXB(-out.cost_yxb);
    }
    if (out.cost_gold != 0)
    {
        gUserData->addGold(-out.cost_gold);
    }
    gUserData->addPrestige(out.add_prestige);
    
    NotificeObject *object = NotificeObject::create(&out);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(PRESTIGE_LOTTERY, object);
}

int OnlineManager::userPartnerEatExp(uint32_t unit_id,uint32_t unit_cnt,uint32_t itme_id)
{
    cli_user_partner_eat_exp_in in;
    in.unit_id = unit_id;
    in.item.item_id = itme_id;
    in.item.item_cnt = unit_cnt;
    return this->sendDataToOnline(KSEQ, PROTO_USER_PARTNER_EAT_EXP_CMD, &in);
}
 
void OnlineManager::userPartnerEatExpCbk(byte_array_t& ba)
{
    cli_user_partner_eat_exp_out out;
    out.read_from_buf(ba);
    
    int add_exp = out.add_exp;
    RoleData* role = gUserData->getRoleByID(out.unit_id);
    if (role)
    {
        role->addExp(out.add_exp);
        if (role->getExp() > gUserData->m_heroData->getExp())
        {
            add_exp = role->getExp() - gUserData->m_heroData->getExp();
            role->setExp(gUserData->m_heroData->getExp());
        }
    }
    
    CCArray *arr = CCArray::create();
    arr->addObject(CCString::createWithFormat("经验值 X %d", add_exp));
    XYTopLayer::getInstance()->ShowSimpleTip(arr);
    
    userGetPackageToOnline();
    CCNotificationCenter::sharedNotificationCenter()->postNotification(PARTNEREATEXP_NOTE, NULL);
}

int OnlineManager::userGetTaskList()
{
    return this->sendDataToOnline(KSEQ, PROTO_USER_GET_TASK_LIST_CMD, NULL);
}

void OnlineManager::userGetTaskListCbk(byte_array_t& ba)
{
    cli_user_get_task_list_out out;
    out.read_from_buf(ba);
    UserDataEx* pDataEx = gUserData->m_dataEx;
    pDataEx->taskMap.clear();
    for (int i = 0; i<out.task_vec.size(); i++)
    {
        task_info_t taskinfo = out.task_vec.at(i);
        pDataEx->taskMap.insert(std::pair<int , task_info_t>(taskinfo.task_id,taskinfo));
    }
    pDataEx->setCurTask(pDataEx->getOptimalTask());
    CCNotificationCenter::sharedNotificationCenter()->postNotification(UPDATE_TASK_LIST_NOTE, NULL);
}
int OnlineManager::userAcceptTask(uint32_t task_id)
{
    cli_user_accept_task_in in;
    in.task_id = task_id;
    return this->sendDataToOnline(KSEQ, PROTO_USER_ACCEPT_TASK_CMD,&in);
}

void OnlineManager::userAcceptTaskCbk(byte_array_t& ba)
{
    cli_user_accept_task_out out;
    out.read_from_buf(ba);
    NotificeObject *object = NotificeObject::create(&out);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(ACCEPT_TASK_COMPLETE_NOTE, object);
}

int OnlineManager::userDropTask(uint32_t task_id)
{
    cli_user_drop_task_in in;
    in.task_id = task_id;
    return this->sendDataToOnline(KSEQ, PROTO_USER_DROP_TASK_CMD,&in);
}

void OnlineManager::userDropTaskCbk(byte_array_t& ba)
{
    CCNotificationCenter::sharedNotificationCenter()->postNotification(DROP_TASK_COMPLETE_NOTE, NULL);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(UPDATE_TASK_LIST_NOTE, NULL);
}

int OnlineManager::userFinishTask(uint32_t task_id,uint32_t use_gold)
{
    cli_user_finish_task_in in;
    in.task_id = task_id;
    in.use_gold = use_gold;
    return this->sendDataToOnline(KSEQ, PROTO_USER_FINISH_TASK_CMD,&in);
}

void OnlineManager::userFinishTaskCbk(byte_array_t& ba)
{
    cli_user_finish_task_out out;
    out.read_from_buf(ba);
    
    gUserDataEx->setMaxMainTask(out.task_id);
    
    for (int i = 0; i<out.prize_vec.size(); i++)
    {
        item_unit_t item = out.prize_vec.at(i);
        if ( getKindOfItem(item.item_id) == 0)
        {
            switch (item.item_id)
            {
                case 0:
                    gUserData->addYXB(item.item_cnt);
                    break;
                case 1:
                    gUserData->m_heroData->addExp(item.item_cnt);
                    break;
                case 2:
                    gUserData->addGold(item.item_cnt);
                    break;
                case 3:
                    gUserData->addPower(item.item_cnt);
                    break;
                case 4:
                    gUserData->addNerve(item.item_cnt);
                    break;
                case 5:
                    gUserData->addEnery(item.item_cnt);
                    break;
                case 6:
                    gUserData->addPrestige(item.item_cnt);
                    break;
                case 7://炼气值，只有英雄能增加炼气值？
                    gUserData->m_heroData->addInForceExp(item.item_cnt);
                    break;
                case 8://星石经验
                    gUserData->addStarStoneExp(item.item_cnt);
                    break;
                default:
                    break;
            }
        }
    }
    XYTopLayer::getInstance()->ShowTip(&(out.prize_vec));
    gUserDataEx->taskMap.erase(out.task_id);
    gUserDataEx->m_curTask = -1;
    const TaskConfig_t *curTaskConf = GameData::getTaskConf(out.task_id);
    const TaskConfig_t *afterTaskConf = GameData::getTaskConf(curTaskConf->nAfterTaskID);
    if (afterTaskConf && curTaskConf->nTaskType == 1 && gUserData->getLV() >= afterTaskConf->nNeedLevel)//主线任务直接插入
    {
        task_info_t taskInfo;
        taskInfo.task_id = afterTaskConf->nTaskID;
        taskInfo.task_type = afterTaskConf->nTaskType;
        taskInfo.task_state = 2;
        taskInfo.max_step = afterTaskConf->nNeedCount;
        taskInfo.cur_step = 0;
        gUserDataEx->taskMap.insert(std::pair<int , task_info_t>(taskInfo.task_id,taskInfo));
    }
    
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(UPDATE_TASK_LIST_NOTE, NULL);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_TASK_AWARD_NOTE, NULL);
    
    // 完成任务通知引导
    CCNotificationCenter::sharedNotificationCenter()->postNotification(POST_TASK_DATA_TUTORIAL, CCString::createWithFormat("%d", out.task_id));
}

int OnlineManager::userSetTask(uint32_t task_id,uint32_t add_step)
{
    cli_user_set_task_in in;
    in.task_id = task_id;
    in.add_step = add_step;
    return this->sendDataToOnline(KSEQ, PROTO_USER_SET_TASK_CMD,&in);
}

void OnlineManager::userSetTaskCbk(byte_array_t& ba)
{
    cli_user_get_task_list_out out;
    out.read_from_buf(ba);
    for (int i = 0; i<out.task_vec.size(); i++)
    {
        task_info_t taskInfochange = out.task_vec.at(i);
        if (gUserDataEx->taskMap.end() != gUserDataEx->taskMap.find(taskInfochange.task_id))
        {
            task_info_t *taskInfo = &gUserDataEx->taskMap.at(taskInfochange.task_id);
            taskInfo->task_state = taskInfochange.task_state;
            taskInfo->cur_step = taskInfochange.cur_step;
        }
        else
        {
            gUserDataEx->taskMap.insert(std::pair<int , task_info_t>(taskInfochange.task_id,taskInfochange));
        }
    }
    if (gUserDataEx->getCurTask() != -1)
    {
        if(gUserDataEx->taskMap.find(gUserDataEx->getCurTask())!= gUserDataEx->taskMap.end())
        {
            task_info_t *info = &gUserDataEx->taskMap[gUserDataEx->getCurTask()];
            const TaskConfig_t* conf = GameData::getTaskConf(gUserDataEx->getCurTask());

            
            if(info->cur_step == info->max_step && (conf->nFinishType == 1 ||conf->nFinishType == 2||conf->nFinishType == 3))
            {
                CCNotificationCenter::sharedNotificationCenter()->postNotification(RAIDTASK_COMPLETELNOTE);
            }
        }
    }
    else
    {
        gUserDataEx->setCurTask(gUserDataEx->getOptimalTask());
    }
    CCNotificationCenter::sharedNotificationCenter()->postNotification(UPDATE_TASK_LIST_NOTE, NULL);
}

int OnlineManager::userGetWantedTaskList()
{
    return this->sendDataToOnline(KSEQ, PROTO_USER_GET_AWARD_TASK_CMD, NULL);
}

void OnlineManager::userGetWantedTaskListCbk(byte_array_t& ba)
{
    cli_user_get_award_task_out out;
    out.read_from_buf(ba);
    
    gUserDataEx->daily_count = out.daily_count;
    gUserDataEx->award_time_counter = out.award_counter;
    memcpy(gUserDataEx->scroll_count, out.scroll_count, sizeof(uint32_t) * 4);
    
    gUserDataEx->awardTask.clear();
    for (int i = 0; i < 4; i ++) {
        award_task_info_t task = out.award_task[i];
        if (task.cur_step > task.max_step)
        {
            task.cur_step = task.max_step;
        }
        gUserDataEx->awardTask.insert(std::pair<int, award_task_info_t>(task.task_id, task));
    }
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_WANTED_TASK_LIST, NULL);
}

int OnlineManager::userAcceptWantedTask(uint32_t task_id)
{
    cli_user_accept_award_task_in in;
    in.task_id = task_id;
    
    return this->sendDataToOnline(KSEQ, PROTO_USER_ACCEPT_AWARD_TASK_CMD, &in);
}

void OnlineManager::userAcceptWantedTaskCbk(byte_array_t& ba)
{
    CCNotificationCenter::sharedNotificationCenter()->postNotification(ACCEPT_WANTED_TASK_COMPLETE_NOTE, NULL);
}

int OnlineManager::userUpgradeWantedTask(uint32_t task_id)
{
    cli_user_upgrade_award_task_in in;
    in.task_id = task_id;
    
    return this->sendDataToOnline(KSEQ, PROTO_USER_UPGRADE_AWARD_TASK_CMD, &in);
}

void OnlineManager::userUpgradeWantedTaskCbk(byte_array_t& ba)
{
    cli_user_upgrade_award_task_out out;
    out.read_from_buf(ba);
    
    gUserData->addGold(-out.cost_gold);
    
    int task_id = out.task_id;
    
    gUserDataEx->awardTask[task_id].task_star = 5;

    CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_WANTED_TASK_LIST, NULL);
}

int OnlineManager::userSetWantedTaskFinish(uint32_t task_id)
{
    cli_user_set_award_task_finish_in in;
    in.task_id = task_id;
    return this->sendDataToOnline(KSEQ, PROTO_USER_SET_AWARD_TASK_FINISH_CMD, &in);
}

void OnlineManager::userSetWantedTaskFinishCbk(byte_array_t& ba)
{
    cli_user_upgrade_award_task_out out;
    out.read_from_buf(ba);
    
    gUserData->addGold(-out.cost_gold);
    
    int task_id = out.task_id;
    
    gUserDataEx->awardTask[task_id].cur_step = gUserDataEx->awardTask[task_id].max_step;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_WANTED_TASK_LIST, NULL);
}

int OnlineManager::userFinishWantedTask(uint32_t task_id, uint32_t item_id)
{
    cli_user_finish_award_task_in in;
    in.task_id = task_id;
    in.item_id = item_id;
    return this->sendDataToOnline(KSEQ, PROTO_USER_FINISH_AWARD_TASK_CMD, &in);
}

void OnlineManager::userFinishWantedTaskCbk(byte_array_t& ba)
{
    cli_user_finish_award_task_out out;
    out.read_from_buf(ba);
    
    gUserDataEx->setActiveDgreeValue(17);
    gUserDataEx->daily_count --;
    gUserDataEx->awardTask[out.task_id].task_state = 2;
    
    for (int i = 0; i < out.prize_vec.size(); i ++) {
        item_unit_t item = out.prize_vec.at(i);
        if (getKindOfItem(item.item_id) == 0)
        {
            switch (item.item_id)
            {
                case 0:
                    gUserData->addYXB(item.item_cnt);
                    break;
                case 1:
                    gUserData->m_heroData->addExp(item.item_cnt);
                    break;
                case 2:
                    gUserData->addGold(item.item_cnt);
                    break;
                case 3:
                    gUserData->addPower(item.item_cnt);
                    break;
                case 4:
                    gUserData->addNerve(item.item_cnt);
                    break;
                case 5:
                    gUserData->addEnery(item.item_cnt);
                    break;
                case 6:
                    gUserData->addPrestige(item.item_cnt);
                    break;
                case 7://炼气值，只有英雄能增加炼气值？
                    gUserData->m_heroData->addInForceExp(item.item_cnt);
                    break;
                case 8://星石经验
                    gUserData->addStarStoneExp(item.item_cnt);
                    break;
                default:
                    break;
            }
        }
    }
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(FINISH_WANTED_TASK_NODE, NULL);
    XYTopLayer::getInstance()->ShowTip(&(out.prize_vec));
    CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_WANTED_TASK_LIST);
}


int OnlineManager::userRefreshWantedTask()
{
    return this->sendDataToOnline(KSEQ, PROTO_USER_REFRESH_AWARD_TASK_CMD, NULL);
}

void OnlineManager::userRefreshWantedTaskCbk(byte_array_t& ba)
{
    cli_user_refresh_award_task_out out;
    out.read_from_buf(ba);
    
    gUserData->addYXB(-out.cost_yxb);
    gUserDataEx->award_time_counter = out.award_counter;
    
    gUserDataEx->awardTask.clear();
    for (int i = 0; i < 4; i ++) {
        award_task_info_t task = out.award_task[i];
        if (task.cur_step > task.max_step)
        {
            task.cur_step = task.max_step;
        }
        gUserDataEx->awardTask.insert(std::pair<int, award_task_info_t>(task.task_id, task));
    }
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_WANTED_TASK_LIST, NULL);
}

int OnlineManager::userGetArenaInfo()
{
    return sendDataToOnline(KSEQ, PROTO_USER_GET_ARENA_INFO_CMD, NULL);
}

void OnlineManager::userGetArenaInfoCbk(byte_array_t& ba)
{
    cli_user_get_arena_info_out out;
    out.read_from_buf(ba);
    NotificeObject *object = NotificeObject::create(&out);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(ANENAINFO_NOTE, object);

}

int OnlineManager::userGetArenaTop100(uint32_t curpage)
{
    cli_user_get_arena_top_100_in in;
    in.cur_page = curpage;
    return sendDataToOnline(KSEQ, PROTO_USER_GET_ARENA_TOP_100_CMD, &in);
}

void OnlineManager::userGetArenaTop100Cbk(byte_array_t& ba)
{
    cli_user_get_arena_top_100_out out;
    out.read_from_buf(ba);
    NotificeObject *object = NotificeObject::create(&out);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(ARENATOP100_NOTE, object);
}

int OnlineManager::userGetArenaFile()
{
    return sendDataToOnline(KSEQ, PROTO_USER_GET_ARENA_FILE_CMD, NULL);
}

void OnlineManager::userGetArenaFileCbk(byte_array_t& ba)
{
    cli_user_get_arena_file_out out;
    out.read_from_buf(ba);
    NotificeObject *object = NotificeObject::create(&out);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(ARENAFILE_NOTE, object);
}

int OnlineManager::userGetArenaRankPrize()
{
    return sendDataToOnline(KSEQ, PROTO_USER_GET_ARENA_RANK_PRIZE_CMD, NULL);
}

void OnlineManager::userGetArenaRankPrizeCbk(byte_array_t& ba)
{
    cli_user_get_arena_rank_prize_out out;
    out.read_from_buf(ba);
    gUserData->addYXB(out.prize_yxb);
    gUserData->addPrestige(out.prize_prestige);
    std::vector<item_unit_t> pItemVec;
    item_unit_t item1;
    item1.item_id = 0;
    item1.item_cnt = out.prize_yxb;
    
    item_unit_t item2;
    item2.item_id = 6;
    item2.item_cnt = out.prize_prestige;
    
    pItemVec.push_back(item1);
    pItemVec.push_back(item2);
    XYTopLayer::getInstance()->ShowTip(&pItemVec);
    NotificeObject *object = NotificeObject::create(&out);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(ARENAPRIZE_NOTE, object);
}

int OnlineManager::userStartArenaBattle(uint32_t userid)
{
    cli_user_start_arena_battle_in in;
    in.target_userid = userid;
    return sendDataToOnline(KSEQ, PROTO_USER_START_ARENA_BATTLE_CMD, &in);
}

void OnlineManager::userStartArenaBattleCbk(byte_array_t& ba)
{
    cli_user_start_arena_battle_out out;
    out.read_from_buf(ba);
    gUserDataEx->setActiveDgreeValue(13);
    
    NotificeObject *object = NotificeObject::create(&out);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(ARENAFIGHT_NOTE, object);
}

int OnlineManager::userOverArenaBattle(uint32_t battle_result,uint32_t target_id,uint8_t *battle_session)
{
    cli_user_over_arena_battle_in in;
    in.battle_result = battle_result;
    in.target_id = target_id;
    return sendDataToOnline(KSEQ, PROTO_USER_OVER_ARENA_BATTLE_CMD, &in);
}

void OnlineManager::userOverArenaBattleCbk(byte_array_t& ba)
{
    cli_user_over_arena_battle_out out;
    out.read_from_buf(ba);
    
    NotificeObject *object = NotificeObject::create(&out);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(ARENAFIGHTOVER_NOTE, object);
}

int OnlineManager::userGetWorshipInfo()
{
    return sendDataToOnline(KSEQ, PROTO_USER_GET_WORSHIP_INFO_CMD, NULL);
}

void OnlineManager::userGetWorshipInfoCbk(byte_array_t& ba)
{
    cli_user_get_worship_info_out out;
    out.read_from_buf(ba);
    NotificeObject *object = NotificeObject::create(&out);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(WORSHIPINFO_NOTE, object);
}

int OnlineManager::userWorshipTop(uint32_t rankid)
{
    cli_user_worship_top_user_in in;
    in.rank_id = rankid;
    return sendDataToOnline(KSEQ, PROTO_USER_WORSHIP_TOP_USER_CMD, &in);
}

void OnlineManager::userWorshipTopCbk(byte_array_t& ba)
{
    cli_user_worship_top_user_out out;
    out.read_from_buf(ba);
    gUserData->addYXB(-out.cost_yxb);
    gUserData->addGold(-out.cost_gold);
    gUserData->addPrestige(out.add_prestige);
    
    std::vector<item_unit_t> vec;
    item_unit_t item2;
    item2.item_id = 6;
    item2.item_cnt = out.add_prestige;
    vec.push_back(item2);
    XYTopLayer::getInstance()->ShowTip(&vec);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(WORSHIP_NOTE);
}

//PROTO_USER_SHOP_INFO			= 650,//拉取商城数据,参数 1代表是道具 2代表是礼包
int OnlineManager::userGetShopInfo(int type, int page)
{
    cli_user_shop_info_in in;
    in.shop_type = type;
    in.page_no = page;
    return sendDataToOnline(KSEQ, PROTO_USER_SHOP_INFO_CMD, &in);
}

void OnlineManager::userGetShopInfoCbk(byte_array_t& ba)
{
    cli_user_shop_info_out out;
    out.read_from_buf(ba);
    
    NotificeObject* obj = NotificeObject::create();
    obj->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_MALL_GOODS_INFO, obj);
}

int OnlineManager::userBuyGoodsInShop(int product_id, int count)
{
    cli_user_shop_buy_in in;
    in.product_id = product_id;
    in.product_cnt = count;
    
    return sendDataToOnline(KSEQ, PROTO_USER_SHOP_BUY_CMD, &in);
}

void OnlineManager::userBuyGoodsInShopCbk(byte_array_t& ba)
{
    cli_user_shop_buy_out out;
    out.read_from_buf(ba);
    gUserData->addGold(- out.gold_cnt);
    
    for (int i = 0; i < out.item_vec.size(); i ++)
    {
        item_unit_t item = out.item_vec.at(i);
        if (getKindOfItem(item.item_id) == 0)
        {
            switch (item.item_id)
            {
                case 0:
                    gUserData->addYXB(item.item_cnt * out.product_cnt);
                    break;
                case 1:
                    gUserData->addExp(item.item_cnt * out.product_cnt);
                    break;
                case 2:
                    gUserData->addGold(item.item_cnt * out.product_cnt);
                    break;
                case 3:
                    gUserData->addPower(item.item_cnt * out.product_cnt);
                    break;
                case 4:
                    gUserData->addNerve(item.item_cnt * out.product_cnt);
                    break;
                case 5:
                    gUserData->addEnery(item.item_cnt * out.product_cnt);
                    break;
                case 6:
                    gUserData->addPrestige(item.item_cnt * out.product_cnt);
                    break;
                case 7://炼气值，只有英雄能增加炼气值？
                    gUserData->m_heroData->addInForceExp(item.item_cnt * out.product_cnt);
                    break;
                case 8://星石经验
                    gUserData->addStarStoneExp(item.item_cnt * out.product_cnt);
                    break;
                default:
                    break;
            }
        }
    }
    
    
    NotificeObject* obj = NotificeObject::create();
    obj->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(BUY_ITEM_FROM_SHOP_SUCCESS, obj);
}

int OnlineManager::userGetRankInfo(int type)
{
    cli_user_rank_info_in in;
    in.rank_type = type;
    return sendDataToOnline(KSEQ, PROTO_USER_RAND_INFO_CMD, &in);
}

void OnlineManager::userGetRankInfoCbk(byte_array_t& ba)
{
    cli_user_rank_info_out out;
    out.read_from_buf(ba);
    
    NotificeObject* obj = NotificeObject::create();
    obj->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_USER_RANK_NOTE, obj);
}

//PROTO_USER_PRIVATE_GIFT			= 654,//个人礼包
int OnlineManager::userGetGiftBoxInfo(int type)
{
    cli_user_gift_box_info_in in;
    in.type = type;
    return sendDataToOnline(KSEQ, PROTO_USER_PRIVATE_GIFT_CMD, &in);
}

void OnlineManager::userGetGiftBoxInfoCbk(byte_array_t& ba)
{
    cli_user_gift_box_info_out out;
    out.read_from_buf(ba);

    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    
    NotificeObject* obj = NotificeObject::create();
    obj->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_GIFT_BOX_NOTE, obj);
}

int OnlineManager::userRecieveGiftBox(int type, int id)
{
    cli_user_gift_box_get_in in;
    in.id = id;
    in.type = type;
    return sendDataToOnline(KSEQ, PROTO_USER_PRIVATE_GIFT_GET_CMD, &in);
}

void OnlineManager::userRecieveGiftBoxCbk(byte_array_t& ba)
{
    cli_user_gift_box_get_out out;
    out.read_from_buf(ba);

    NotificeObject* obj = NotificeObject::create();
    obj->msg = &out;
    
    std::vector<item_unit_t>* pUnit = &(out.items_vec);
    for (std::vector<item_unit_t>::iterator it = pUnit->begin(); it != pUnit->end(); it++)
    {
        this->addItem(it->item_id, it->item_cnt);
    }
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(RECIEVE_GIFT_BOX_NOTE, obj);
}

int OnlineManager::userGetBossMissionInfo(int map_id)
{
    cli_user_get_mission_in in;
    in.map_id = map_id;
    return sendDataToOnline(KSEQ, PROTO_USER_GET_BOSS_MISSION_CMD, &in);
}

void OnlineManager::userGetBossMissionInfoCbk(byte_array_t& ba)
{
    cli_user_get_mission_out out;
    out.read_from_buf(ba);
        
    NotificeObject* obj = NotificeObject::create();
    obj->msg = &out;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_BOSS_MISSION_NOTE, obj);
}

int OnlineManager::userResetBossMission(int map_id)
{
    cli_user_reset_boss_mission_in in;
    in.map_id = map_id;
    return sendDataToOnline(KSEQ, PROTO_USER_RESET_BOSS_MISSION_CMD, &in);
}

void OnlineManager::userResetBossMissionCbk(byte_array_t& ba)
{
    cli_user_reset_boss_mission_out out;
    out.read_from_buf(ba);
    
    gUserData->addGold(- out.cost_gold);
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(RESET_BOSS_MISSION_NOTE);
}

int OnlineManager::userAutoBossBattle(int map_id)
{
    cli_user_auto_boss_battle_in in;
    in.map_id = map_id;
    
    return sendDataToOnline(KSEQ, PROTO_USER_AUTO_BOSS_BATTLE_CMD, &in);
}

void OnlineManager::userAutoBossBattleCbk(byte_array_t& ba)
{
    cli_user_auto_boss_battle_out out;
    out.read_from_buf(ba);
    
    // check wanted task
    for (int i = 0; i < out.boss_vec.size(); i ++)
    {
        int mission_id = out.boss_vec.at(i).map_id;
        gUserDataEx->storeBattleInfo(mission_id, NULL);
        gUserDataEx->execWantedTask(TASK_ELITE_BOSS_TYPE);
        
    }
    
    int yxb = getItemCount(&out.boss_vec, 0);
    gUserData->addYXB(yxb);
    
    int exp = getItemCount(&out.boss_vec, 1);
    gUserData->addExp(exp);
    
    gUserDataEx->setActiveDgreeValue(10,out.boss_vec.size());
    NotificeObject* obj = NotificeObject::create();
    obj->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(AUTO_BATTLE_BOSS_NOTE, obj);
}

int OnlineManager::userGetMaxMissionList()
{
    return sendDataToOnline(KSEQ, PROTO_USER_GET_MAX_MISSION_CMD, NULL);
}

void OnlineManager::userGetMaxMissionListCbk(byte_array_t& ba)
{
    cli_user_get_max_mission_out out;
    out.read_from_buf(ba);
    
    NotificeObject* obj = NotificeObject::create();
    obj->msg = &out;
    gUserDataEx->setMaxMission(out.maxid_vec.at(0));
    gUserDataEx->setMaxEliteMission(out.maxid_vec.at(1));
    gUserDataEx->setMaxHardMission(out.maxid_vec.at(2));
    CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_MAXMISSIONID_NOTE, obj);
}

int OnlineManager::userGetVillaInfo(int user_id)
{
    cli_user_get_villa_info_in in;
    in.userid = user_id;
    return sendDataToOnline(KSEQ, PROTO_USER_GET_VILLA_INFO_CMD, &in);
}

void OnlineManager::userGetVillaInfoCbk(byte_array_t& ba)
{
    cli_user_get_villa_info_out out;
    out.read_from_buf(ba);
    
    NotificeObject* obj = NotificeObject::create();
    obj->msg = &out;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_VILLA_INFO_NOTE, obj);
}


int OnlineManager::userPlant(int plantId, int landId)
{
    cli_user_plant_in in;
    in.plant_id = plantId;
    in.land_id = landId;
    return sendDataToOnline(KSEQ, PROTO_USER_PLANT_CMD, &in);
    
}

void OnlineManager::userPlantCbk(byte_array_t& ba)
{
    cli_user_plant_out out;
    out.read_from_buf(ba);
    
    gUserDataEx->setActiveDgreeValue(9);
    NotificeObject* obj = NotificeObject::create();
    obj->msg = &out;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(PLANT_SUCCESS_NOTE, obj);
}

int OnlineManager::userSpeedPlant(int plantId, int landId, int cost_gold)
{
    cli_user_speed_plant_in in;
    in.land_id = landId;
    in.plant_id = plantId;
    in.cost_gold = cost_gold;
    return sendDataToOnline(KSEQ, PROTO_USER_SPEED_PLANT_CMD, &in);
}

void OnlineManager::userSpeedPlantCbk(byte_array_t& ba)
{
    cli_user_speed_plant_out out;
    out.read_from_buf(ba);
    
    gUserData->addGold(-out.cost_gold);
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(SPEED_PLANT_NOTE, CCString::createWithFormat("%d", out.land_id));
}

int OnlineManager::userHavestPlant(int plantId, int landId)
{
    cli_user_havest_plant_in in;
    in.land_id = landId;
    in.plant_id = plantId;
    
    return sendDataToOnline(KSEQ, PROTO_USER_HAVEST_PLANT_CMD, &in);
}

void OnlineManager::userHavestPlantCbk(byte_array_t& ba)
{
    cli_user_havest_plant_out out;
    out.read_from_buf(ba);
    
    switch (out.prize.item_id) {
        case 2:
            gUserData->addGold(out.prize.item_cnt);
            break;
        case 0:
            gUserData->addYXB(out.prize.item_cnt);
            break;
        case 4:
            gUserData->addNerve(out.prize.item_cnt);
            break;
        case 3:
            gUserData->addPower(out.prize.item_cnt);
            break;
        default:
            break;
    }
    
    NotificeObject* obj = NotificeObject::create();
    obj->msg = &out;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(PLANT_HAVEST_NOTE, obj);
}

int OnlineManager::userExpandLand(int landId)
{
    cli_user_expand_land_in in;
    in.land_id = landId;
    return sendDataToOnline(KSEQ, PROTO_USER_EXPAND_LAND_CMD, &in);
}

void OnlineManager::userExpandLandCbk(byte_array_t& ba)
{
    cli_user_expand_land_out out;
    out.read_from_buf(ba);
    
    gUserData->addYXB(-out.cost_yxb);
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(LAND_EXPAND_SUCCESS_NOTE, CCString::createWithFormat("%d", out.land_id));
}

int OnlineManager::userShakeMoneyTree(uint32_t cost_gold)
{
    cli_user_shake_money_in in;
    in.cost_gold = cost_gold;
    
    return sendDataToOnline(KSEQ, PROTO_USER_SHAKE_MONEY_CMD, &in);
}

void OnlineManager::userShakeMoneyTreeCbk(int ret, byte_array_t& ba)
{
    if (ret == 0)
    {
        cli_user_shake_money_out out;
        out.read_from_buf(ba);
        
        gUserData->addGold(- out.cost_gold);
        gUserData->addYXB(out.add_yxb);
        
        gUserDataEx->setActiveDgreeValue(8);
        CCNotificationCenter::sharedNotificationCenter()->postNotification(SHAKE_MONEY_TREE_NODE, CCString::createWithFormat("%d", out.add_yxb));
    }
    else
    {
        CCNotificationCenter::sharedNotificationCenter()->postNotification(SHAKE_MONEY_TREE_NODE, NULL);
    }
    
}

int OnlineManager::userActivityDice(uint32_t is_cost_gold)
{
    cli_user_activity_dice_in in;
    in.is_cost_gold = is_cost_gold;
    
    return sendDataToOnline(KSEQ, PROTO_USER_ACTIVITY_DICE_CMD, &in);
}

void OnlineManager::userActivityDiceCbk(int ret, byte_array_t& ba)
{
    if (ret == 0)
    {
        cli_user_activity_dice_out out;
        out.read_from_buf(ba);
        
        gUserData->addGold(-out.cost_gold);
        gUserData->addYXB(-out.cost_yxb);
        gUserData->addEnery(out.enery);
        
        NotificeObject* obj = NotificeObject::create();
        obj->msg = &out;
        CCNotificationCenter::sharedNotificationCenter()->postNotification(ACTIVITY_DICE_NOTE, obj);
    }
    else
    {
        CCNotificationCenter::sharedNotificationCenter()->postNotification(ACTIVITY_DICE_NOTE, NULL);
    }
    
}

int OnlineManager::userGetTowerInfo()
{
    return sendDataToOnline(KSEQ, PROTO_USER_TOWER_CMD, NULL);
}

void OnlineManager::userGetTowerInfoCbk(byte_array_t& ba)
{
    cli_user_tower_out out;
    out.read_from_buf(ba);
    
    NotificeObject* obj = NotificeObject::create();
    obj->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_TOWER_INFO_NOTE, obj);
}

int OnlineManager::userAutoTowerBattle()
{
    return sendDataToOnline(KSEQ, PROTO_USER_AUTO_TOWERBATTLE_CMD, NULL);
}

void OnlineManager::userAutoTowerBattleCbk(byte_array_t& ba)
{
    cli_user_auto_towerbattle_out out;
    out.read_from_buf(ba);
    
    gUserDataEx->setActiveDgreeValue(16,out.boss_vec.size());
    NotificeObject* obj = NotificeObject::create();
    obj->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(AUTO_TOWER_BATTLE_NOTE, obj);
}

int OnlineManager::userResetTower()
{
    return sendDataToOnline(KSEQ, PROTO_USER_RESET_TOWER_CMD, NULL);
}

void OnlineManager::userResetTowerCbk(byte_array_t& ba)
{
    CCNotificationCenter::sharedNotificationCenter()->postNotification(RESET_TOWER_INFO_NOTE);
}

int OnlineManager::userGetOtherUser(uint32_t userid)
{
    cli_user_get_other_user_in in;
    in.userid = userid;
    return sendDataToOnline(KSEQ, PROTO_USER_GET_OTHER_USER_CMD, &in);
}

void OnlineManager::userGetOtherUserCbk(byte_array_t& ba)
{
    cli_user_get_other_user_out out;
    out.read_from_buf(ba);
    NotificeObject* obj = NotificeObject::create();
    obj->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_OTHER_USER_NOTE, obj);
}

int OnlineManager::userGetVillaState()
{
    return sendDataToOnline(KSEQ, PROTO_USER_GET_VILLA_STATE_CMD, NULL);
}

void OnlineManager::userGetVillaStateCbk(byte_array_t& ba)
{
    cli_user_get_villa_state_out out;
    out.read_from_buf(ba);
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_VILLA_STATE, CCInteger::create(out.plant_state));
}

//PROTO_USER_GET_STAR_LIST_CMD    = 677,  //得到占星列表
int OnlineManager::userGetStarList()
{
    return sendDataToOnline(KSEQ, PROTO_USER_GET_STAR_LIST_CMD, NULL);
}
void OnlineManager::userGetStarListCbk(byte_array_t& ba)
{
    cli_user_get_star_list_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    
    NotificeObject* obj = NotificeObject::create();
    obj->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_STAR_LIST_NOTE, obj);
}

//PROTO_USER_ASTROLOGY_CMD        = 678,  //占星
int OnlineManager::userAstrology(uint32_t nType, uint32_t nId)
{
    cli_user_astrology_in in;
    in.astrology_type = nType;
    in.astrologer_id = nId;
    
    return sendDataToOnline(KSEQ, PROTO_USER_ASTROLOGY_CMD, &in);
}
void OnlineManager::userAstrologyCbk(byte_array_t& ba)
{
    cli_user_astrology_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return;
    }
    gUserDataEx->setActiveDgreeValue(15,out.star_vec.size());
    NotificeObject* obj = NotificeObject::create();
    obj->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(ASTROLOGY_NOTE, obj);
}

//PROTO_USER_TRANSFORM_STAR_CMD   = 679,  //转化单个星石
int OnlineManager::userTransformStar(uint32_t nId)
{
    cli_user_transform_star_in in;
    in.unique_id = nId;
    
    return sendDataToOnline(KSEQ, PROTO_USER_TRANSFORM_STAR_CMD, &in);
}
void OnlineManager::userTransformStarCbk(byte_array_t& ba)
{
    cli_user_transform_star_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    
    NotificeObject* obj = NotificeObject::create();
    obj->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(TRANSFORM_STAR_NOTE, obj);
}

//PROTO_USER_AUTO_TRANSFORM_STAR_CMD  = 680,  //一键转化星石
int OnlineManager::userAutoTransformStar(uint32_t nState)
{
    cli_user_auto_transform_star_in in;
    in.star_state = nState;
    
    return sendDataToOnline(KSEQ, PROTO_USER_AUTO_TRANSFORM_STAR_CMD, &in);
}
void OnlineManager::userAutoTransformStarCbk(byte_array_t& ba)
{
    cli_user_auto_transform_star_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    
    NotificeObject* obj = NotificeObject::create();
    obj->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(AUTO_TRANSFORM_STAR_NOTE, obj);
    
}

//PROTO_USER_EQUIP_STAR_CMD       = 681,  //装备或卸下星石
int OnlineManager::userEquipStar(uint32_t nEquipType, uint32_t nId, uint32_t nSeatId)
{
    cli_user_equip_star_in in;
    in.equip_type = nEquipType;
    in.unique_id = nId;
    in.seat_id = nSeatId;
    
    return sendDataToOnline(KSEQ, PROTO_USER_EQUIP_STAR_CMD, &in);
}
void OnlineManager::userEquipStarCbk(byte_array_t& ba)
{
    cli_user_equip_star_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    
    NotificeObject* obj = NotificeObject::create();
    obj->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EQUIP_STAR_NOTE, obj);
}

//PROTO_USER_UPGRADE_STAR_CMD     = 682,  //升级星石
int OnlineManager::userUpgradeStar(uint32_t nId)
{
    cli_user_upgrade_star_in in;
    in.unique_id = nId;
    
    return sendDataToOnline(KSEQ, PROTO_USER_UPGRADE_STAR_CMD, &in);
}
void OnlineManager::userUpgradeStarCbk(byte_array_t& ba)
{
    cli_user_upgrade_star_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    
    NotificeObject* obj = NotificeObject::create();
    obj->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(UPGRADE_STAR_NOTE, obj);
}

//PROTO_USER_CALL_ASTROLOGER_CMD	= 683,	//召唤占星师
int OnlineManager::userCallAstrologer()
{
    return sendDataToOnline(KSEQ, PROTO_USER_CALL_ASTROLOGER_CMD, NULL);
}
void OnlineManager::userCallAstrologerCbk(byte_array_t& ba)
{
    cli_user_call_astrologer_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    
    NotificeObject* obj = NotificeObject::create();
    obj->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(CALL_ASTROLOGER_NOTE, obj);
}

//PROTO_USER_INVITE_INFO_CMD = 694, //好友邀请信息
int OnlineManager::userInviteInfo()
{
    return sendDataToOnline(KSEQ, PROTO_USER_INVITE_INFO_CMD, NULL);
}
void OnlineManager::userInviteInfoCbk(byte_array_t& ba)
{
    cli_user_invite_info_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    
    NotificeObject* obj = NotificeObject::create();
    obj->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(INVITE_INFO_NOTE, obj);
}

int OnlineManager::userGetOrderBillNo(uint32_t producttype)
{
    cli_user_get_order_bill_no_in in;
    in.product_type = producttype;
    return sendDataToOnline(KSEQ, PROTO_USER_GET_ORDER_BILL_NO_CMD, &in);
}

void OnlineManager::userGetOrderBillNoCbk(byte_array_t& ba)
{
    cli_user_get_order_bill_no_out out;
    out.read_from_buf(ba);
    NotificeObject* obj = NotificeObject::create();
    obj->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_ORDER_BILL_NOTE, obj);
}

int OnlineManager::userNotifyBuyGold()
{
    return sendDataToOnline(KSEQ, PROTO_USER_NOTIFY_BUY_GOLD_CMD, NULL);
}

void OnlineManager::userNotifyBuyGoldCbk(byte_array_t& ba)
{
    cli_user_notify_buy_gold_out out;
    out.read_from_buf(ba);
    gUserData->setBuyGold(out.buy_gold_total);
    gUserData->addGold(out.buy_gold);
}

int OnlineManager::userGetEscortInfo(uint32_t isnew)
{
    cli_user_get_escort_info_in in;
    in.is_new = isnew;
    return sendDataToOnline(KSEQ, PROTO_USER_GET_ESCORT_INFO_CMD, &in);
}

void OnlineManager::userGetEscortInfoCbk(byte_array_t& ba)
{

}

int OnlineManager::userChooseHorse()
{
    return sendDataToOnline(KSEQ, PROTO_USER_CHOOSE_HORSE_CMD, NULL);
}

void OnlineManager::userChooseHorseCbk(byte_array_t& ba)
{
    cli_user_choose_horse_out out;
    out.read_from_buf(ba);
    NotificeObject* obj = NotificeObject::create();
    obj->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(USER_CHOOSE_HORSE_NOTE, obj);
}

int OnlineManager::userRefreshHorse(uint32_t isgold)
{
    cli_user_refresh_horse_in in;
    in.is_gold = isgold;
    return sendDataToOnline(KSEQ, PROTO_USER_REFRESH_HORSE_CMD, &in);
}

void OnlineManager::userRefreshHorseCbk(byte_array_t& ba)
{
    cli_user_refresh_horse_out out;
    out.read_from_buf(ba);
    if (out.cost_gold>0)
    {
        gUserData->addGold(-out.cost_gold);
    }
    NotificeObject* obj = NotificeObject::create();
    obj->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(USER_REFRESH_HORSE_NOTE, obj);
}

int OnlineManager::userGetEscortUserInfo(uint32_t userid)
{
    cli_user_get_escort_user_info_in in;
    in.userid = userid;
    return sendDataToOnline(KSEQ, PROTO_USER_GET_ESCORT_USER_INFO_CMD, &in);
}

void OnlineManager::userGetEscortUserInfoCbk(byte_array_t& ba)
{
    cli_user_get_escort_user_info_out out;
    out.read_from_buf(ba);
    NotificeObject* obj = NotificeObject::create();
    obj->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(USER_GET_ESCORT_USERINFO_NOTE, obj);
}

int OnlineManager::userEscortCombatInfo()
{
    return sendDataToOnline(KSEQ, PROTO_USER_ESCORT_COMBAT_INFO_CMD,NULL);
}

void OnlineManager::userEscortCombatInfoCbk(byte_array_t& ba)
{
    cli_user_escort_combat_info_out out;
    out.read_from_buf(ba);
    NotificeObject* obj = NotificeObject::create();
    obj->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(USER_GET_ESCORT_COMBATINFO_NOTE, obj);
}

int OnlineManager::userStartKeapHorse(uint32_t keaped_userid)
{
    cli_user_start_keap_horse_in in;
    in.keaped_userid = keaped_userid;
    return sendDataToOnline(KSEQ, PROTO_USER_START_KEAP_HORSE_CMD,&in);
}

void OnlineManager::userStartKeapHorseCbk(byte_array_t& ba)
{
    cli_user_start_keap_horse_out out;
    out.read_from_buf(ba);
    NotificeObject *object = NotificeObject::create(&out);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(USER_START_KEAP_HORSE_NOTE, object);
}

int OnlineManager::userStartEscort()
{
    return sendDataToOnline(KSEQ, PROTO_USER_START_ESCORT_CMD,NULL);
}

void OnlineManager::userStartEscortCbk(byte_array_t& ba)
{
    gUserDataEx->setActiveDgreeValue(20);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(USER_START_ESCORT_NOTE, NULL);
}

int OnlineManager::userOverKeapHorse(uint32_t battle_result,uint32_t target_id,uint8_t *battle_session)
{
    cli_user_over_keap_horse_in in;
    in.battle_result = battle_result;
    in.target_id = target_id;
    return sendDataToOnline(KSEQ, PROTO_USER_OVER_KEAP_HORSE_CMD, &in);
}

void OnlineManager::userOverKeapHorseCbk(byte_array_t& ba)
{
    cli_user_over_keap_horse_out out;
    out.read_from_buf(ba);
    NotificeObject *object = NotificeObject::create(&out);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(USER_END_KEAP_HORSE_NOTE, object);
}

int OnlineManager::userEscortGift()
{
    return sendDataToOnline(KSEQ, PROTO_USER_ESCORT_GIFT_CMD,NULL);
}

void OnlineManager::userEscortGiftCbk(byte_array_t& ba)
{
    cli_user_escort_gift_out out;
    out.read_from_buf(ba);
    for (int i = 0; i<2; i++)
    {
        item_unit_t item = out.gift[i];
        if (item.item_id == 0)
        {
            gUserData->addYXB(item.item_cnt);
        }
    }
    if (out.is_moved == 1)
    {
        XYTopLayer::getInstance()->ShowTip("背包已满，奖励已寄存到个人礼包中，请查收！");
    }
}

int OnlineManager::userEscortExistsGift()
{
    return sendDataToOnline(KSEQ, PROTO_USER_ESCORT_EXISTS_GIFT_CMD,NULL);
}

void OnlineManager::userEscortExistsGiftCbk(byte_array_t& ba)
{
    cli_user_escort_exists_gift_out out;
    out.read_from_buf(ba);
    NotificeObject *object = NotificeObject::create(&out);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(USER_ESCORT_EXISTS_GIFT_NOTE, object);
}

//PROTO_USER_GET_WORLD_BOSS_INFO_CMD = 695, //拉取世界boss信息
int OnlineManager::userGetWorldBossInfo()
{
    return sendDataToOnline(KSEQ, PROTO_USER_GET_WORLD_BOSS_INFO_CMD, NULL);
}

void OnlineManager::userGetWorldBossInfoCbk(byte_array_t& ba)
{
    cli_user_get_world_boss_info_out out;
    out.read_from_buf(ba);
    
    NotificeObject* pObj = NotificeObject::create();
    pObj->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(USER_GET_WORLD_BOSS_INFO_NOTE, pObj);
    
}

int OnlineManager::userWorldBossBTLStart(int cost_gold)
{
    cli_user_world_boss_btl_start_in in;
    in.cost_gold = cost_gold;
    return sendDataToOnline(KSEQ, PROTO_USER_WORLD_BOSS_BTL_START_CMD, &in);
}

void OnlineManager::userWorldBossBTLStartCbk(byte_array_t& ba)
{
    cli_user_world_boss_btl_start_out out;
    out.read_from_buf(ba);
    
    gUserDataEx->setActiveDgreeValue(19);
    gUserData->addYXB(out.reward_yxb);
    gUserData->addNerve(out.reward_nerve);
    
    NotificeObject* pObj = NotificeObject::create();
    pObj->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(USER_WORLD_BOSS_BTL_START_NOTE, pObj);
}

int OnlineManager::userWorldBossBTLOver(int hurt_hp)
{
    cli_user_world_boss_btl_over_in in;
    in.hurt_hp = hurt_hp;
    return sendDataToOnline(KSEQ, PROTO_USER_WORLD_BOSS_BTL_OVER_CMD, &in);
}

void OnlineManager::userWorldBossBTLOverCbk(byte_array_t& ba)
{
    cli_user_world_boss_btl_over_out out;
    out.read_from_buf(ba);
    
    //CCNotificationCenter::sharedNotificationCenter()->postNotification(USER_WORLD_BOSS_BTL_OVER_NOTE);
}

int OnlineManager::userWorldBossOverNoti()
{
    return sendDataToOnline(KSEQ, PROTO_USER_WORLD_BOSS_OVER_NOTI_CMD, NULL);
}

void OnlineManager::userWorldBossOverNotiCbk(byte_array_t& ba)
{
    cli_user_world_boss_over_noti_out out;
    out.read_from_buf(ba);
    
    
    NotificeObject* pObj = NotificeObject::create();
    pObj->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(USER_WORLD_BOSS_OVER_NOTI_NOTE, pObj);

}

int OnlineManager::userCheckWorldBoss()
{
    return sendDataToOnline(KSEQ, PROTO_USER_CHECK_WORLD_BOSS_CMD, NULL);
}

void OnlineManager::userCheckWorldBossCbk(byte_array_t& ba)
{
    cli_user_check_world_boss_out out;
    out.read_from_buf(ba);
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(USER_CHECK_WORLD_BOSS_NOTE, CCInteger::create(out.status));
}

//PROTO_USER_GET_ACTIVITY_INFO_CMD = 701, //获取活动信息
int OnlineManager::userGetActivityInfo(std::vector<uint32_t>& id_vec)
{
    cli_user_get_activity_info_in in;
    for (int i = 0; i < id_vec.size(); i ++)
    {
        int activity_id = id_vec.at(i);
        in.id_vec.push_back(activity_id);
    }
    
    return sendDataToOnline(KSEQ, PROTO_USER_GET_ACTIVITY_INFO_CMD, &in);
}

void OnlineManager::userGetActivityInfoCbk(byte_array_t& ba)
{
    cli_user_get_activity_info_out out;
    out.read_from_buf(ba);
    
    NotificeObject* pObj = NotificeObject::create();
    pObj->msg = &out;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(USER_GET_ACTIVITY_INFO_NOTE, pObj);
}

//PROTO_USER_PARTNER_FRIENDVAL_TRAIN_CMD = 702, //弟子好友度培养
int OnlineManager::userPartnerFriendvalTrain(int nPartnerID, int nTrainType)
{
    cli_user_partner_friendval_train_in in;
    in.partner_id = nPartnerID;
    in.type = nTrainType;
    return sendDataToOnline(KSEQ, PROTO_USER_PARTNER_FRIENDVAL_TRAIN_CMD, &in);
}

void OnlineManager::userPartnerFriendvalTrainCbk(byte_array_t& ba)
{
    cli_user_partner_friendval_train_out out;
    out.read_from_buf(ba);
    
    gUserData->addYXB(-out.cost_yxb);
    NotificeObject* pObj = NotificeObject::create();
    pObj->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(USER_TRAIN_PARTNER_FRIENDLY_NOTE, pObj);
}

//PROTO_USER_PARTNER_UNHAD_LIST_CMD = 703, //拉取没有获得的弟子列表及友好度
int OnlineManager::userPartnerUnhadList()
{
    return sendDataToOnline(KSEQ, PROTO_USER_PARTNER_UNHAD_LIST_CMD, NULL);
}

void OnlineManager::userPartnerUnhadListCbk(byte_array_t& ba)
{
    cli_user_partner_unhad_list_out out;
    out.read_from_buf(ba);
    
    NotificeObject* pObj = NotificeObject::create();
    pObj->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(USER_GET_ENLIST_PARTNER_NOTE, pObj);
}

//PROTO_USER_BY_INVITE_CODE_CMD = 705, //设置邀请我进来的好友码
int OnlineManager::userByInviteCode(int nInviteCode)
{
    cli_user_by_invite_code_in in;
    in.invite_code = nInviteCode;
    return sendDataToOnline(KSEQ, PROTO_USER_BY_INVITE_CODE_CMD, &in);
}
void OnlineManager::userByInviteCodeCbk(byte_array_t& ba)
{
    cli_user_by_invite_code_out out;
    out.read_from_buf(ba);
    if (!ba.is_end())
    {
        CCLOG("报文过多，请检查！");
        return ;
    }
    
    NotificeObject *object = NotificeObject::create();
    object->msg = &out;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(USER_BY_INVITE_CODE_NOTE,object);
}

int OnlineManager::userSysMsgNoti()
{
    return sendDataToOnline(KSEQ, PROTO_USER_SYS_MSG_NOTI_CMD, NULL);
}

void OnlineManager::userSysMsgNotiCbk(byte_array_t& ba)
{
    cli_user_sys_msg_noti_out out;
    out.read_from_buf(ba);
    
    for (int i = 0; i < out.msg_list.size(); i ++)
    {
        sys_noti_info_t info = out.msg_list.at(i);
        
        char str[64] = {0};
        sprintf(str, "msg_type_%d", info.msg_type);
        
        int msg_num = CCUserDefault::sharedUserDefault()->getIntegerForKey(GET_KEY_WITH_USER(gUserData->userid, str));
        msg_num += info.num;
        
        CCUserDefault::sharedUserDefault()->setIntegerForKey(GET_KEY_WITH_USER(gUserData->userid, str), msg_num);
        CCUserDefault::sharedUserDefault()->flush();
    }
    
    NotificeObject* pObj = NotificeObject::create();
    pObj->msg = &out;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(USER_SYS_MSG_NOTE, pObj);
}

int OnlineManager::userSetReadSysMsg(int msgType)
{
    cli_user_set_read_sys_msg_in in;
    in.msg_type = msgType;
    return sendDataToOnline(KSEQ, PROTO_USER_SET_READ_SYS_MSG_CMD, &in);
}

void OnlineManager::userSetReadSysMsgCbk(byte_array_t& ba)
{
    
}

