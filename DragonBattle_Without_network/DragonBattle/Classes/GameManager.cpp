//
//  GameManager.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-16.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "GameManager.h"
#include "BattleLayer.h"
#include "SportLayer.h"
#include "ShowMemoryLayer.h"
#include "ResourcesManager.h"
#include "GameData.h"
#include "XYTopLayer.h"
#include "LoginController.h"
#include "tools.h"
#include "LoadingLayer.h"
#include "TimerManager.h"
#include "LocalLanguage.h"

#include "XYMessageBox.h"
#include "LogicManager.h"
using namespace cocos2d;

GameManager* GameManager::pSharedInstance = NULL;

GameManager::GameManager()
{
    m_pBattleLayer = NULL;
    m_pCommunityLayer = NULL;
    m_nMissionID = 0;
}

GameManager::~GameManager()
{
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    LocalLanguage::unLoadLocalString();
}

GameManager* GameManager::sharedInstance()
{
    if (!pSharedInstance)
    {
        pSharedInstance = new GameManager();
        pSharedInstance->init();
    }
    
    return pSharedInstance;
}

void GameManager::purgeInstance()
{
    if(pSharedInstance)
    {
        delete pSharedInstance;
        pSharedInstance = NULL;
    }
}

bool GameManager::init()
{
    srand(time(0));
    ResourcesManager::sharedInstance()->initResources();
    double curMemory = usedMemory();
    
    LocalLanguage::loadLocalString();
    
    CCLOG("before GameManager::init: %f", curMemory);
    GameData::loadAllConf();
    
    double confMemory = usedMemory() - curMemory;
    CCLOG("end GameManager::init %f (used)%f", usedMemory(), confMemory);
    return true;
}

void GameManager::enterLoginLayer()
{
    replaceScene(LOGIN_SCENCE,false);
}

void GameManager::backToLoginLayer()
{
    replaceScene(LOGIN_SCENCE,false);
    LogicManager::shareLogicManager()->stopNetWork(E_RETURN_LOGIN);

    gUserData->resetData();
}

// 选择服务器
void GameManager::enterSelectServerLayer()
{
    CCLOG("socket 连接成功");
}

// 进入主城
void GameManager::enterCommunity()
{
    LoginController::sharedInstance()->removeLoginLayer();
    replaceScene(COMMUNITY_SCENCE,false);
}

bool GameManager::getFinishTask()
{

}



void GameManager::showRoleLayer(int unitID, int type, int bagType/* = 0*/)
{
    if (m_pCommunityLayer)
    {
        gUserData->resetFindTarget();
    }
}

void GameManager::showRechargeLayer()
{
}

void GameManager::showMovie(int taskID,kClipState clipState)
{
    MovieOver *movieover;

    MovieClip *clip = MovieClip::create(taskID,clipState,movieover);
    clip->setTag(10001);
    TOPADDCHILD(clip);
}


// 根据关卡ID进入关卡战斗场景
void GameManager::enterBattleLayer(uint32_t nMissionID)
{
    replaceScene(MISSION_SCENCE,false, nMissionID);
}

// 退出战斗场景
void GameManager::exitBattleLayer()
{
    replaceScene(COMMUNITY_SCENCE,true,0);
}

// 战斗场景中的地图层
CCLayer* GameManager::getBattleMapLayer()
{
    if (m_pBattleLayer)
    {
        return (CCLayer*)m_pBattleLayer->m_pMapUnitsLayer;
    }
    
    return NULL;
}


void GameManager::replaceScene(SceneType sctype,bool isFromMission,int nMissionID)
{
    m_sceneType = sctype;
    m_nMissionID = nMissionID;
    m_pBattleLayer = NULL;
    m_pCommunityLayer = NULL;
    m_isFromMission = isFromMission;
    
    if (CCDirector::sharedDirector()->getRunningScene())
    {
        CCDirector::sharedDirector()->getRunningScene()->removeAllChildren();
    }
    
    CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
    
    XYTopLayer::getInstance()->removeAllChildren();
    XYTopLayer::getInstance()->removeFromParentAndCleanup(false);
    
    
    CCScene *pScene = CCScene::create();
    switch (m_sceneType)
    {
        case LOGIN_SCENCE:
        {
            pScene->addChild(LoginController::sharedInstance()->creatLayer());
            break;
        }
        case NEW_SCENCE:
        {
            XYButton *btn = XYButton::create("public/public_btn_smallred_light.png");
            btn->setPosition(ccp(480,320));
            btn->setTouchEndEvent(this, callfuncN_selector(GameManager::memoryDesc));
            pScene->addChild(btn);
            
            XYButton *btn1 = XYButton::create("public/public_btn_smallred_light.png");
            btn1->setPosition(ccp(480,160));
            btn1->setTouchEndEvent(this, callfuncN_selector(GameManager::replacemomory));
            pScene->addChild(btn1);
            break;
        }
        case COMMUNITY_SCENCE:
        {
            CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("item/items.plist");
            for (int i = 1; i<=3; i++)
            {
                char str[64] = {0};
                sprintf(str, "action/player_1/Role_stay_%d.plist",i);
                CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(str);
                sprintf(str, "action/player_1/Role_stay_%d_a.plist",i);
                CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(str);
                sprintf(str, "action/player_1/Role_stay_%d_b.plist",i);
                CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(str);
            }
 
            break;
        }
        case MISSION_SCENCE:
        {
            m_pBattleLayer = BattleLayer::create(m_nMissionID);
            pScene->addChild(m_pBattleLayer);
            break;
        }
        case LOADING_SCENCE:
        {
            if (m_nMissionID == 1)
            {
                LoadingLayer* pLoadLayer = LoadingLayer::create();
                pScene->addChild(pLoadLayer, kLoadingLayerTag, kLoadingLayerTag);
            }
            break;
        }
        case SPORT_SCENE:
        {
            m_pBattleLayer = SportLayer::create();
            pScene->addChild(m_pBattleLayer);
        }
        default:
            break;
    }
#ifndef SERVER_RELEASE
    ShowMemoryLayer* pShowMemoryLayer = ShowMemoryLayer::create();
    pScene->addChild(pShowMemoryLayer, kShowMemoryLayerTag, kShowMemoryLayerTag);
#endif

    pScene->addChild(XYTopLayer::getInstance(),kTopLayerTag);
    
    if (CCDirector::sharedDirector()->getRunningScene())
    {
        CCDirector::sharedDirector()->getRunningScene()->removeAllChildren();
        CCDirector::sharedDirector()->replaceScene(pScene);
    }
    else
    {
        CCDirector::sharedDirector()->runWithScene(pScene);
    }
}

void GameManager::enterWorldBossCommunity()
{
    gUserData->setCurCommunityID(gUserData->nLastCommunityID, true);
    OnlineManager::sharedManager()->userActionToOnline(0, 0, 3, gUserData->getCurCommunityID());
    GameManager::sharedInstance()->replaceScene(COMMUNITY_SCENCE, false);
}

void GameManager::exitWorldBossCommunity(bool fromMission/* = false*/)
{
    gUserData->setCurCommunityID(gUserData->nLastCommunityID);
    OnlineManager::sharedManager()->userActionToOnline(0, 0, 3, gUserData->getCurCommunityID());
    GameManager::sharedInstance()->replaceScene(COMMUNITY_SCENCE, fromMission);
}

void GameManager::replacemomory(CCNode *node)
{
    enterCommunity();
}

void GameManager::memoryDesc(CCNode *node)
{
    CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
    CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
}

void GameManager::restartBattle()
{
    m_pBattleLayer->networkBattleRestart();
}
