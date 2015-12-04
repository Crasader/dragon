//
//  LogicManager.cpp
//  TestLogin
//
//  Created by 小敦 王 on 12-10-29.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
#include "LogicManager.h"
#include "UserData.h"
#include "OnlineManager.h"
#include "GameManager.h"
#include "LoadingLayer.h"
#include "NetworkManager.h"
#include "BattleLayer.h"

USING_NS_CC;

LogicManager* LogicManager::pLogicManager = NULL;

LogicManager::LogicManager()
{
    m_nPort = 0;
    m_nStopNetReason = E_NO_REASON;
}

LogicManager::~LogicManager()
{
}

bool LogicManager::init()
{
    srand(time(0));
    return true;
}

// game enter foregroud
void LogicManager::enterForeground()
{

    // 如果在主城中,不在断线处理中 或者在关卡中 奖励已领取
    if ((GameManager::sharedInstance()->getCommunityLayer() && m_nStopNetReason == E_NO_REASON) ||
         (GameManager::sharedInstance()->getBattleLayer() && GameManager::sharedInstance()->getBattleLayer()->m_bHasGetAward == true && m_nStopNetReason == E_NO_REASON)
        )
    {
        // 发送心跳包
        OnlineManager::sharedManager()->sendDataToOnline(KSEQ, PROTO_USER_KEEP_ALIVE_CMD, NULL);
        
        // 不管是否成功，都添加超时处理
        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(OnlineManager::onOnlineTimeOut), OnlineManager::sharedManager());
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(OnlineManager::onOnlineTimeOut),this,ONLINE_TIMEOUT_SEC, OnlineManager::sharedManager());
        
        XYTopLayer::getInstance()->ShowWaiting();
    }
    CCNotificationCenter::sharedNotificationCenter()->postNotification(ENTERFOREGROUND_NOTE);
}

// game enter backgroud
void LogicManager::enterBackground()
{
}


bool LogicManager::isOnlineConnected()
{
    return NetworkManager::shareNetworkManager()->isOnlineConnected();
}

// 开启网络
void LogicManager::startNetWork(const char* ip, uint16_t port)
{
    m_nStopNetReason = E_NO_REASON;
    NetworkManager::shareNetworkManager()->setOnlineIpAndPort(ip, port);
    NetworkManager::shareNetworkManager()->startThread();
}

void LogicManager::startNetWork()
{
    m_nStopNetReason = E_NO_REASON;
    if (NetworkManager::shareNetworkManager()->isGotOnline())
    {
        NetworkManager::shareNetworkManager()->startThread();
    }
}

void LogicManager::stopNetWork(int nReason)
{
    if (m_nStopNetReason != E_NO_REASON)
    {
        return ;
    }
    m_nStopNetReason = nReason;
    NetworkManager::shareNetworkManager()->closeConnect();
}

// tcp connec to online server success
void LogicManager::connectOnlineSuccess()
{
    OnlineManager::sharedManager()->userLoginToOnline();
}

// tcp socket closed
void LogicManager::onOnlineClosed()
{
    if (m_nStopNetReason == E_RETURN_LOGIN)
    {
        return ;
    }
    
    // 如果在战斗场景中
    if (GameManager::sharedInstance()->getBattleLayer())
    {
        LogicManager::shareLogicManager()->stopNetWork(E_SERVER_CLOSE);
        return ;
    }
    
    // 自动重连
    //OnlineManager::sharedManager()->onReconnectOnline(NULL);
    //直接重连好像有问题，在下一帧去重连
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(OnlineManager::onNetConnectTimeOut),OnlineManager::sharedManager(), 1, false);
}

// 成功登录到online
void LogicManager::loginToOnlineSucc()
{
    if (gUserData->getCurCommunityID() == WORLD_BOSS_MAP_ID)
    {
        GameManager::sharedInstance()->enterWorldBossCommunity();
    }
    else
    {
        GameManager::sharedInstance()->enterCommunity();
    }
}

// 得到抽牌奖励成功
void LogicManager::getCardPrizeToOnlineSucc(item_unit_t* pCard, bool bIsBagFull)
{
    BattleLayer* pbattle = (BattleLayer*)GameManager::sharedInstance()->getBattleLayer();
    pbattle->onGetCardPrize(pCard, bIsBagFull);
}

// 成功登录到online
void LogicManager::battleOverToOnlineSucc(uint32_t map_type,uint32_t nFirstPass, uint32_t nScore, std::vector<item_unit_t>* pFirstPrize, std::vector<item_unit_t>* pCardVec, std::vector<item_unit_t>* pPassPrize, int nTotalCostGold)
{
    BattleLayer* pbattle = (BattleLayer*)GameManager::sharedInstance()->getBattleLayer();
    gUserDataEx->execWantedTask(1);
    switch (map_type)
    {
        case 1://普通
        case 6://困难
        {
            //一旦战斗结束数据被验证是有效的，则立即再向服务器请求用户属性信息，更新黄金、银两、体力等信息。
            OnlineManager::sharedManager()->getUserAttrToOnline();
            switch (nFirstPass)
            {
                case 0://未通关
                    if (pbattle)
                    {
                        pbattle->onBattleLose(map_type);
                    }
                    break;
                case 1://首次通关
                    //许翔：现在改成了胜利面板在前，抽卡面板在后
                    pbattle->onBattleWin(map_type, nScore, pFirstPrize, pPassPrize, pCardVec, nTotalCostGold);
                    gUserDataEx->execWantedTask(2);
                    gUserDataEx->setActiveDgreeValue(4);
                    break;
                case 2://非首次通关
                    pbattle->onBattleWin(map_type, nScore, pFirstPrize, pPassPrize, NULL, 0);
                    gUserDataEx->execWantedTask(2);
                    gUserDataEx->setActiveDgreeValue(4);
                    break;
                default:
                    break;
            }
            break;
        }
        case 2://精英
        {
            OnlineManager::sharedManager()->getUserAttrToOnline();
            switch (nFirstPass) {
                case 0://未通关
                    pbattle->onBattleLose(map_type);
                    break;
                case 2:
                {
                    gUserDataEx->execWantedTask(13);
                    gUserDataEx->setActiveDgreeValue(10);
                    pbattle->onBattleWin(map_type, nScore, pFirstPrize, pPassPrize, NULL, 0);
                    break;
                }
                    
                default:
                    break;
            }
            
            break;
        }
        case 4://爬塔
        {
            OnlineManager::sharedManager()->getUserAttrToOnline();
            switch (nFirstPass)
            {
                case 0://未通关
                    pbattle->onTowerBattleLose();
                    break;
                case 1://退出
                    //GameManager::sharedInstance()->getCommunityLayer()->setOpenFlag(kMenuTower);
                    break;
                case 2://通关
                    pbattle->onTowerBattleWin(pCardVec, nTotalCostGold);
                    gUserDataEx->execWantedTask(2);
                    gUserDataEx->setActiveDgreeValue(16);
                    break;
                default:
                    break;
            }
            break;
        }
        default:
            break;
    }
}

// 装备强化成功
void LogicManager::equipmentStrengthenToOnlineSucc(uint32_t nCost, uint32_t nCdState, uint32_t nCdTime)
{
    CCArray* pArray = CCArray::create();
    char str[128] = {0};
    sprintf(str, "%d", nCost);
    CCString* str1 = CCString::create(str);
    pArray->addObject(str1);
    sprintf(str, "%d", nCdState);
    CCString* str2 = CCString::create(str);
    pArray->addObject(str2);
    sprintf(str, "%d", nCdTime);
    CCString* str3 = CCString::create(str);
    pArray->addObject(str3);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EQUIPMENT_STRENGTHEN_SUCC_NOTE, pArray);
}

// 经脉升级成功
void LogicManager::nerveUpgradeToOnlineSucc(uint32_t nCdType, uint32_t nCdTime, uint32_t nCdState, uint32_t nUpgradeSucc, uint32_t nCostAir, uint32_t nCostGold)
{
    CCArray* pArray = CCArray::create();
    char str[128] = {0};
    sprintf(str, "%d", nCostAir);
    CCString* str1 = CCString::create(str);
    pArray->addObject(str1);
    sprintf(str, "%d", nCostGold);
    CCString* str2 = CCString::create(str);
    pArray->addObject(str2);
    sprintf(str, "%d", nCdState);
    CCString* str3 = CCString::create(str);
    pArray->addObject(str3);
    sprintf(str, "%d", nCdTime);
    CCString* str4 = CCString::create(str);
    pArray->addObject(str4);
    sprintf(str, "%d", nUpgradeSucc);
    CCString* str5 = CCString::create(str);
    pArray->addObject(str5);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(NERVE_UPGRADE_SUCC_NOTE, pArray);
}

// 取消装备强化CD成功
void LogicManager::cancelCoolDownToOnlineSucc(uint32_t nCdType, uint32_t nGold)
{
    gUserData->addGold(-nGold);
    switch (nCdType)
    {
        case STRENGTHEN_EQUIPMENT_CD:
            CCNotificationCenter::sharedNotificationCenter()->postNotification(EQUIPMENT_STRENGTHEN_COOL_DOWN_SUCC_NOTE, NULL);
            break;
        case NERVE_UNGRADE_CD:
            CCNotificationCenter::sharedNotificationCenter()->postNotification(NERVE_UPGRADE_COOL_DOWN_SUCC_NOTE, NULL);
            break;
        case BONUS_ARENA_DAILY_TYPE:
            CCNotificationCenter::sharedNotificationCenter()->postNotification(ARENA_CD_SUCC_NOTE, NULL);
        default:
            break;
    }
}

// 取得装备强化CD成功
void LogicManager::getCDTimeToOnlineSucc(uint32_t nCdType, uint32_t nCdState, uint32_t nCdTime)
{
    CCArray* pArray = CCArray::create();
    char str[128] = {0};
    sprintf(str, "%d", nCdState);
    CCString* str1 = CCString::create(str);
    pArray->addObject(str1);
    sprintf(str, "%d", nCdTime);
    CCString* str2 = CCString::create(str);
    pArray->addObject(str2);
    switch (nCdType)
    {
        case STRENGTHEN_EQUIPMENT_CD:
            CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_EQUIPMENT_STRENGTHEN_CD_SUCC_NOTE, pArray);
            break;
        case NERVE_UNGRADE_CD:
            CCNotificationCenter::sharedNotificationCenter()->postNotification(GET_NERVE_UNGRADE_CD_SUCC_NOTE, pArray);
            break;
        default:
            break;
    }
}

// 装备进阶成功
void LogicManager::equipmentUpgradeToOnlineSucc(uint32_t nPackageID, uint32_t nNewItemID, uint32_t nNewItemLevel, uint32_t nCostGold)
{
    gUserData->addGold(-nCostGold);
    CCArray* pArray = CCArray::create();
    char str[128] = {0};
    sprintf(str, "%d", nPackageID);
    CCString* str1 = CCString::create(str);
    pArray->addObject(str1);
    sprintf(str, "%d", nNewItemID);
    CCString* str2 = CCString::create(str);
    pArray->addObject(str2);
    sprintf(str, "%d", nNewItemLevel);
    CCString* str3 = CCString::create(str);
    pArray->addObject(str3);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EQUIPMENT_UPGRADE_SUCC_NOTE, pArray);
}

//装备洗练成功
void LogicManager::equipmentWashToOnlineSucc(package_equipment_t* pEquip)
{
    char str[128] = {0};
    CCArray* pArray = CCArray::create();
    
    for (int i = 0; i < 4; i++)
    {
        sprintf(str, "%d", pEquip->attach_attr[i].attr_type);
        CCString* pType = CCString::create(str);
        pArray->addObject(pType);
        sprintf(str, "%d", pEquip->attach_attr[i].attr_value);
        CCString* pValue = CCString::create(str);
        pArray->addObject(pValue);
    }
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EQUIPMENT_WASH_SUCC_NOTE, pArray);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(ATTRBITECHANGE_NOTE);
}

void LogicManager::userCheatItemSucc()
{
    CCNotificationCenter::sharedNotificationCenter()->postNotification(CHEAT_ITEM_NOTE, NULL);
}
