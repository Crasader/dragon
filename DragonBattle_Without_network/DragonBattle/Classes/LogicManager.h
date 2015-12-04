//
//  LogicManager.h
//  
//
//  Created by alexhe on 12-10-29.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef LogicManager_h
#define LogicManager_h

#include "GlobalDefine.h"
#include "cocos2d.h"
#include "UserData.h"

#include <map>
#include <string>

enum ENUM_GAME_OVER_REASON
{
    E_NO_NET_CONNECT,                   //
    E_LOING_DATA_ERR,
    E_LOGIN_NO_CONNECT,
    E_CHECK_LOGIN_ERR,
    E_USE_SERVER_DATA_ERR,              //使用服务器数据错误
};

enum ENUM_CLOSE_NET_REASON
{
    E_NO_REASON  = 0,
    E_RETURN_LOGIN = 1,     //退出到登录页面
    E_LOGIN_PLATFORM_FAILD, //登陆91失败
    E_CONNECT_FAILD,        //net connect连接失败
    E_SEND_FAILD,           //net send发送失败
    E_SEND_TIME_OUT,        //超时
    E_KICK_OFFLINE,         //踢用户下线
    E_SERVER_CLOSE,         //服务器关闭
};

class LogicManager : public cocos2d::CCNode
{
public:
    LogicManager();
    virtual ~LogicManager();
    virtual bool init();
    C_SHARE_INSTANCE(LogicManager);
    
    // game enter foregroud
    void enterBackground();
    // game enter backgroud
    void enterForeground();
    
    // 开启网络
    void startNetWork(const char* ip, uint16_t port);
    void startNetWork();//已经得到online ip port的情况下
    
    // 关闭网络线程
    void stopNetWork(int nReason = 0);
    int  getStopNetReason() { return m_nStopNetReason; }
    void setStopNetReason(int nReason) { if (m_nStopNetReason == E_NO_REASON) m_nStopNetReason = nReason; }
    void changeStopNetReason(int nReason) { m_nStopNetReason = nReason; }
    
    // tcp connec to online server success
    void connectOnlineSuccess();
    
    // 登录online成功
    void loginToOnlineSucc();
    
    void onOnlineClosed();
    
    bool isOnlineConnected();
public:
    // 得到抽牌奖励成功
    void getCardPrizeToOnlineSucc(item_unit_t* pCard, bool bIsBagFull);
    
    // 战斗结束，返回相应奖励
    void battleOverToOnlineSucc(uint32_t map_type,uint32_t nFirstPass, uint32_t nScore, std::vector<item_unit_t>* pFirstPrize, std::vector<item_unit_t>* pCardVec, std::vector<item_unit_t>* pPassPrize, int nTotalCostGold);
    
    // 装备强化成功
    void equipmentStrengthenToOnlineSucc(uint32_t nCost, uint32_t nCdState, uint32_t nCdTime);
    
    // 经脉升级成功
    void nerveUpgradeToOnlineSucc(uint32_t nCdType, uint32_t nCdTime, uint32_t nCdState, uint32_t nUpgradeSucc, uint32_t nCostAir, uint32_t nCostGold);
    
    // 取消装备强化CD成功
    void cancelCoolDownToOnlineSucc(uint32_t nCdType, uint32_t nGold);
    
    // 取得装备强化CD成功
    void getCDTimeToOnlineSucc(uint32_t nCdType, uint32_t nCdState, uint32_t nCdTime);
    
    // 装备进阶成功
    void equipmentUpgradeToOnlineSucc(uint32_t nPackageID, uint32_t nNewItemID, uint32_t nNewItemLevel, uint32_t nCostGold);
    
    //装备洗练成功
    void equipmentWashToOnlineSucc(package_equipment_t* pEquip);
    
    void userCheatItemSucc();
    
    void userGetDailyBuyEnery(uint32_t type);//1请求可挑战次数 2购买体力
private:
    //bool m_bGameStarted;    //是否是游戏状态
    std::string m_ip;
    uint16_t m_nPort;
    uint32_t m_nStopNetReason;
};

#endif
