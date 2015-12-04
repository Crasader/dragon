//
//  GameManager.h
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-16.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_GameManager_h
#define DragonBattle_GameManager_h

#include "cocos2d.h"
#include "GlobalDefine.h"
#include "GameScene.h"
#include "online_cli_proto_cmd.h"
#include "MovieClip.h"
#include "BattleLayerBase.h"

USING_NS_CC;

class CCBSelectMissionLayer;
class CommunityLayer;
class CCBRoleLayer;
class CCBEquipmentTitleLayer;
class CCBSelectMissionLayer;
class ServerStartJudgeNode;


class GameManager:public CCObject
{
public:
    GameManager();
    virtual ~GameManager();
    static GameManager* sharedInstance();
    static void purgeInstance();
public:
    cocos2d::CCLayer* getBattleMapLayer();
public:
    virtual bool init();
    void loadLayer();
    SceneType m_sceneType;
    int m_nMissionID;
    bool m_isFromMission;
    void replaceScene(SceneType sctype,bool isFromMission,int nMissionID = 0);
    // 进入登录页面
    void enterLoginLayer();
    //用户注销
    void backToLoginLayer();
    // 选择服务器
    void enterSelectServerLayer();
    
    bool getFinishTask();
    
    // 进入主城
    void enterCommunity();

    void showRoleLayer(int unitID,int typ, int bagType = 1);//type 1背包 2详细 3培养
    void showRechargeLayer();
    void showMovie(int taskID,kClipState clipState);

    // 进入战斗场景
    void enterBattleLayer(uint32_t nMissionID);
    // 战斗重来一次
    void restartBattle();
    // 退出战斗场景
    void exitBattleLayer();
    
    void enterWorldBossCommunity();
    void exitWorldBossCommunity(bool fromMission = false);
    
    void memoryDesc(CCNode *node);
    void replacemomory(CCNode *node);
    CC_SYNTHESIZE(CommunityLayer*, m_pCommunityLayer, CommunityLayer);
    CC_SYNTHESIZE(BattleLayerBase*, m_pBattleLayer, BattleLayer);
private:
    static GameManager* pSharedInstance;

};

#endif
