
//
//  BattleScene.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-16.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//
#include "BattleLayer.h"
#include "HeroSprite.h"
#include "Resources.h"
#include "BattleMapUnits.h"
#include "UiControlLayer.h"
#include "BattleUIComponent.h"
#include "CCBPanelVictoryNew.h"
#include "CCBPanelFailureNew.h"
#include "CCBDefine.h"
#include "OnlineManager.h"
#include "UserData.h"
#include "GameManager.h"
#include "ProgressLayerNew.h"
#include "XYMessageBox.h"
#include "AudioResource.h"
#include "CountdownLayer.h"
#include "LogicManager.h"

using namespace cocos2d;
BattleLayer::BattleLayer()
{
    m_pUIContainer = NULL;
    m_pPanelCardSelect = NULL;
    m_pPanelDialSelect = NULL;
    m_pPanelVictory = NULL;
    m_taskBar = NULL;
    m_bCollectTask = false;
    
    m_cardVec.clear();
    m_nTotalCostGold = 0;
    
    m_nCardCostGold = 0;
    m_nUseMedicineCount = 0;
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BattleLayer::setBattleStop), BATTLE_STOP_NOTE, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BattleLayer::showTutoria), BATTLETUTORIA_NOTE, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BattleLayer::battleRestart), BATTLESTART_NOTE, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BattleLayer::onKillCount), KILLMONSTER_NOTE, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BattleLayer::showUnitHp), UNITHPCHANGE_NOTE, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BattleLayer::onBattleEnd), BATTLEEND_NOTE, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BattleLayer::battleLose), BATTLELOST_NOTE, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BattleLayer::battleWin), BATTLEWIN_NOTE, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BattleLayer::onUIVisable), BATTLEUIVISABLE_NOTE, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BattleLayer::onReConnect), BATTLERECONNECT_NOTE, this);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BattleLayer::onWorldBossOverNotice), USER_WORLD_BOSS_OVER_NOTI_NOTE, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BattleLayer::onUseMedicine), AI_EAT_MED_NOTE, NULL);
}

BattleLayer::~BattleLayer()
{
    if (ISTUTORIAMISSION(m_pMissionConf->nMissionId))//新手技能重置
    {
        gUserData->m_heroData->skill[0] = 0;
        gUserData->m_heroData->skill[1] = 0;
        gUserData->m_heroData->skill[2] = 0;
        gUserData->m_heroData->skill[3] = 0;
    }
    m_cardVec.clear();
    
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    m_pMissionConf = NULL;
}

BattleLayer* BattleLayer::create(uint32_t nMissionID)
{
    BattleLayer *pRet = new BattleLayer();
    if (pRet && pRet->init(nMissionID))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool BattleLayer::init(uint32_t nMissionID)
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    m_kBattleType = kBattle;
    m_pMissionConf = GameData::getBattleMission(nMissionID);
    loadResource();
    return true;
}

void BattleLayer::LoadcreateBattle(CCNode* pNode)
{
    createbattle();
}

void BattleLayer::loadResource()
{
    CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
    std::vector<std::string> resourceVec;
    BattleCacheManager::sharedInstance()->loadResourcesBeforBattle(m_pMissionConf->nMissionId);
    BattleCacheManager::sharedInstance()->getResourcesVec(&resourceVec);
    ProgressLayerNew* pLayer = ProgressLayerNew::create(&resourceVec, true, callfuncN_selector(BattleLayer::LoadcreateBattle), this);
    this->addChild(pLayer, 130);
    pLayer->setPosition(ccp(SCREEN_WIDTH / 2, SCREEN_HEIGTH / 2));
}

void BattleLayer::battleRestart(CCObject *p)
{
    m_pMapNode = NULL;
    m_pMapUnitsLayer = NULL;
    
    m_pRole = NULL;
    
    m_pUIContainer = NULL;
    
    m_pPanelCardSelect = NULL;
    m_pPanelDialSelect = NULL;
    m_pPanelVictory = NULL;
    m_taskBar = NULL;
    
    m_nUseMedicineCount = 0;
    
    this->removeAllChildren();
    this->unscheduleAllSelectors();
    
    Cmessage* pMsg = dynamic_cast<NotificeObject*>(p)->msg;
    cli_user_battle_start_out* pOut = dynamic_cast<cli_user_battle_start_out*>(pMsg);
    setConfig(pOut->start_time, pOut->battle_key, &pOut->item_vec, &pOut->thief_item_vec);
    
    createbattle();
}

void BattleLayer::setConfig(uint32_t nStartTime, uint32_t nBattleKey, std::vector<item_unit_t>* pItemVec, std::vector<item_unit_t>* pThiefItemVec)
{
    m_nStartTime = nStartTime;
    m_nBattleKey = nBattleKey;
    m_ItemLimitVec.clear();
    m_ItemLimitVec.assign(pItemVec->begin(), pItemVec->end());
    m_ThiefItemVec.clear();
    if (pThiefItemVec != NULL)  m_ThiefItemVec.assign(pThiefItemVec->begin(), pThiefItemVec->end());
}

void BattleLayer::createbattle()
{
    this->unscheduleAllSelectors();
    this->createUnitHpBar();
    this->createUI();
    this->createUITools();
    this->createMapBkg();
    this->createRoleSprite();
    
    if (getBattleType() == kWorldBoss)
    {
        m_pBtnPause->setVisible(false);
        createWorldBoss();
        
    }
    
    if (getBattleType() == kEliteBoss || getBattleType() == kTowerBoss)
    {
        this->addChild(CountdownLayer::create(this, callfunc_selector(BattleLayer::onStart)));
    }
    else
    {
        onStart();
    }
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(BATTLETUTORIA_NOTE);
}

void BattleLayer::createWorldBoss()
{
    UnitBaseConf conf;
    if (gUserDataEx->boss_cur_hp != 0)
    {
        conf = gUserDataEx->world_boss_conf;
    }
    m_pMapUnitsLayer->createWorldBoss(&conf);
}

void BattleLayer::onStart()
{
    BattleLayerBase::onStart();
    if(m_timeBar)
    {
        m_timeBar->setDisplayFrame(CCSprite::create("BattleUI/ui_black_bg.png")->displayFrame());
        m_timeBar->setAnchorPoint(ccp(1.0f, 0.5f));
        m_timeBar->setPosition(ccp(SCREEN_WIDTH-10,SCREEN_HEIGTH-120));
        
        updatelastTime(0);
    }
    
    //任务对话
    int taskID = gUserDataEx->getTaskClipByMissionID(m_pMissionConf->nMissionId,true);
    if (taskID != -1)
    {
        MovieClip *clip = MovieClip::create(taskID, kFrontClip, m_pMapUnitsLayer);
        TOPADDCHILD(clip);
    }
    //任务完成情况
    taskID = gUserDataEx->getCurTask();
    if (taskID!= -1)
    {
        if(gUserDataEx->taskMap.find(taskID)!= gUserDataEx->taskMap.end())
        {
            task_info_t taskInfo = gUserDataEx->taskMap[taskID];
            const TaskConfig_t *taskConf = GameData::getTaskConf(taskInfo.task_id);
            if(taskConf->nFinishType == 1)
            {
                m_bCollectTask = true;
                onKillCount(NULL);
            }
        }
    }
}

void BattleLayer::setMedNum(int nNum)
{
    if (nNum == 0)
    {
        m_pBtnMed->setGayImage("BattleUI/ui_medicine.png", 1);
    }
    else
    {
        m_pBtnMed->setImage("BattleUI/ui_medicine.png", 1);
    }
    m_pBtnMed->setLbl(CCString::createWithFormat("%d", nNum)->getCString(), FONTNAME, 18);
}

void BattleLayer::createRoleSprite()
{
    //gUserData->m_heroData->refreshBaseAttr();
    if (ISTUTORIAMISSION(m_pMissionConf->nMissionId))//新手特殊处理
    {
        m_pBtnPause->setVisible(false);
        m_pBtnRenew->setVisible(false);
        m_pBtnClose->setVisible(false);
        m_pBtnMed->setVisible(false);
        
        gUserData->m_heroData->baseAttr.nUnitID = 1;
        gUserData->m_heroData->baseAttr.nResourceID = 1;
        memset(gUserData->m_heroData->baseAttr.unitName,0x0,64);
        memcpy(gUserData->m_heroData->baseAttr.unitName, "test", sizeof("test"));
        
        UnitBaseConf conf = gUserData->m_heroData->baseAttr;
        conf.fAddhurt = 0;
        conf.fCrit = 100;
        conf.fReducehurt = 1000;
        conf.fInAttack = 200;
        conf.fOutAttack = 200;
        conf.fInDef = 10000;
        conf.fOutDef = 100000;
        conf.fRecoverHp = 1000;
        conf.nLife = 10000;
        conf.fToughness = 1000;
        m_pRole = HeroSprite::create(&conf);
        m_pRole->setMp(10000);
        m_pRole->setMaxMp(10000);
    }
    else
    {
        gUserData->m_heroData->baseAttr.nUnitID = 1;
        gUserData->m_heroData->baseAttr.nResourceID = 1;
        memset(gUserData->m_heroData->baseAttr.unitName,0x0,64);
        memcpy(gUserData->m_heroData->baseAttr.unitName, "test", sizeof("test"));
        
        UnitBaseConf* pHeroConf = &(gUserData->m_heroData->baseAttr);
        
        m_pRole = HeroSprite::create(pHeroConf);
    }
    m_pMapUnitsLayer->addSelf(m_pRole);
    m_pMapUnitsLayer->createPartner(0);
}
// create UI button
void BattleLayer::createUI()
{
    m_pUIContainer = BattleUILayer::create();
    m_pUIContainer->setDelegate(this);
    this->addChild(m_pUIContainer, kUIOrder);

    // 如果是ip 4寸屏幕，方向按钮向右偏移40点
    float deltaX = 0;
    if (IS_IPHONE_5_INCH)
    {
        deltaX = 20.f;
    }
    
    m_pBtnPause = XYScaleButton::createWithFileAndPriority("BattleUI/btn_pause.png","BattleUI/btn_pause.png",NULL,-1);
    m_pBtnPause->setTouchEndEvent(this, callfuncN_selector(BattleLayer::onPause));
    m_pBtnPause->setPosition(ccp(368, SCREEN_HEIGTH - 34));
    m_pUIContainer->addChild(m_pBtnPause);
    
    m_pBtnRenew = XYScaleButton::createWithFileAndPriority("BattleUI/btn_renew.png","BattleUI/btn_renew_sele.png",NULL,-1);
    m_pBtnRenew->setTouchEndEvent(this, callfuncN_selector(BattleLayer::onRenew));
    m_pBtnRenew->setPosition(ccp(SCREEN_WIDTH - 150, SCREEN_HEIGTH - 50));
    m_pBtnRenew->setVisible(false);
    m_pUIContainer->addChild(m_pBtnRenew);
    
    m_pBtnClose = XYScaleButton::createWithFileAndPriority("BattleUI/btn_close.png","BattleUI/btn_close_sele.png",NULL,-1);
    m_pBtnClose->setTouchEndEvent(this, callfuncN_selector(BattleLayer::onClose));
    m_pBtnClose->setPosition(ccp(SCREEN_WIDTH - 50, SCREEN_HEIGTH - 50));
    m_pBtnClose->setVisible(false);
    CCSprite* pFlash = EffectResource::sharedInstance()->spriteWithEffect(35, true, true, 1);
    m_pBtnClose->addChild(pFlash);
    pFlash->setPosition(ccp(m_pBtnClose->getContentSize().width / 2.0f, m_pBtnClose->getContentSize().height / 2.0f));
    pFlash->setScale(m_pBtnClose->getContentSize().width / pFlash->getContentSize().width);
    m_pUIContainer->addChild(m_pBtnClose);
    
    m_pBtnMed = XYScaleButton::create("BattleUI/skill-null-auto.png",-1);
    m_pBtnMed->setTouchEndEvent(this, callfuncN_selector(BattleLayer::onMedicine));
    m_pBtnMed->addImage("BattleUI/skill_cd.png",-4, 22);
    m_pBtnMed->setPosition(ccp(50, 230));
    m_pBtnMed->setCD(10.0f, true);
    this->setMedNum(gUserData->nMedicine);
    m_pBtnMed->setLblPosition(0.75, 0.3);
    m_pUIContainer->addChild(m_pBtnMed);
    
    if (ISTUTORIAMISSION(m_pMissionConf->nMissionId))
    {
        m_pUIContainer->m_pBtnAutoFight->setVisible(false);
    }
}

void BattleLayer::cancelAutoFight(CCNode *node)
{
    m_pUIContainer->m_pBtnAutoFight->setselect(false);
    onAutoFightTouchBegan(m_pUIContainer->m_pBtnAutoFight);
}

void BattleLayer::onMedicine(CCNode *node)
{
    if (gUserData->nMedicine <=0)
    {
        XYTopLayer::getInstance()->ShowTip("药剂用完了，请到商城购买药剂！");
    }
    else
    {
        onUseMedicine(NULL);
    }
}

void BattleLayer::onUseMedicine(CCObject *obj)
{
    if (m_pMapUnitsLayer->getBattleState()== kBattleing && m_pRole->getHp()>0 && m_pMapUnitsLayer->m_bStopCreateMonster == false && gUserData->nMedicine > 0 )
    {
        m_pBtnMed->showProgressSprite();
        gUserData->nMedicine -= 1;
        m_pRole->addHp(m_pRole->getUnitBaseConf()->nLife/2);
        this->setMedNum(gUserData->nMedicine);
        m_nUseMedicineCount += 1;
    }
}

void BattleLayer::onRenew(CCNode *node)
{
    GameManager::sharedInstance()->restartBattle();
}
void BattleLayer::onClose(CCNode *node)
{
    GameManager::sharedInstance()->replaceScene(COMMUNITY_SCENCE, true);
}

void BattleLayer::onPause(CCNode *node)
{    
    CCDirector::sharedDirector()->setAnimationInterval(1.0 / 60);
    BaseMessageBox *box = BaseMessageBox::create("退出副本" ,"你确定退出副本?", true,this,callfuncN_selector(BattleLayer::onBack), callfuncN_selector(BattleLayer::onContune));
    box->m_pLabelTitle->setPositionY(250);
    box->m_pLabelMsg->setHorizontalAlignment(kCCTextAlignmentCenter);
    CCDirector::sharedDirector()->pause();
    box->setBtnText(1,"退出");
    box->setBtnText(2,"继续");
    TOPADDCHILD(box);
}

void BattleLayer::onBack(CCNode *node)
{
    CCDirector::sharedDirector()->resume();
    exitGame();
    GameManager::sharedInstance()->replaceScene(COMMUNITY_SCENCE, true);
}

void BattleLayer::setBattleStop(CCObject* obj)
{
    CCBool* var = dynamic_cast<CCBool*>(obj);
    if (var)
    {
        m_pMapUnitsLayer->setisStop(var->getValue());
    }
}

void BattleLayer::showTutoria(CCObject *obj)
{

}

void BattleLayer::onContune(CCNode *node)
{
    CCDirector::sharedDirector()->resume();
}

// create bkg 
void BattleLayer::createMapBkg()
{
    m_pMapNode = CCNode::create();
    m_pMapNode->setPosition(CCPointZero);
    m_pMapUnitsLayer = BattleMapUnits::create(m_pMissionConf);
    m_pMapNode->addChild(m_pMapUnitsLayer, kMapLayerOrder, kMapLayerOrder);
    addChild(m_pMapNode);
    m_pMapUnitsLayer->setPosition(ccp(0, 0));
    m_pMapUnitsLayer->setConfig(&m_ItemLimitVec, &m_ThiefItemVec);
}

void BattleLayer::flyItemToPortrait(int nItemId, CCPoint worldPosition)
{
    CCPoint pos = this->convertToNodeSpace(worldPosition);
    
    char str[128] = {0};
    sprintf(str, "item_%d.png", nItemId);
    CCSprite* pSprite = CCSprite::createWithSpriteFrameName(str);
    this->addChild(pSprite, kLayerMaxZOrder);
    pSprite->setPosition(pos);
    
    float duration = 1;
    CCPoint targetPos = ccpAdd(m_pHeroHpBar->getPosition(), ccp(40, -40));
    
    ccBezierConfig bezier;
    int radius = 200;
    int x = (rand() % (radius * 2)) - radius;
    float y = sqrtf(radius * radius - x * x);
    bezier.controlPoint_1 = ccpAdd(pos, ccp(x, y));
    bezier.controlPoint_2 = ccpAdd(pos, ccp(x, y));
    bezier.endPosition = targetPos;
    
    pSprite->runAction(CCSequence::create(CCSpawn::create(CCScaleTo::create(duration, 0.5f), CCBezierTo::create(duration, bezier), NULL), CCRemoveSelf::create(), NULL));
}

void BattleLayer::createUnitHpBar()
{
    m_pHeroHpBar = HpBarLayer::create(kHero);
    m_pHeroHpBar->setPosition(ccp(0, SCREEN_HEIGTH));
    this->addChild(m_pHeroHpBar, kUIOrder);
    
    m_pBossHpBar1 = BossHpBarLayer::create(5);
    m_pBossHpBar1->setPosition(ccp(SCREEN_WIDTH - m_pBossHpBar1->getContentSize().width - 15, SCREEN_HEIGTH));
    this->addChild(m_pBossHpBar1, kUIOrder);
    
    m_pPartnerHpBar = PartnerHpBarLayer::create(kPartnerType);
    m_pPartnerHpBar->setPosition(ccp(0, SCREEN_HEIGTH-100));
    this->addChild(m_pPartnerHpBar, kUIOrder);
}

void BattleLayer::onUIVisable(CCObject *obj)
{
    CCString *str = (CCString*)obj;
    bool visable = str->boolValue();
    m_pUIContainer->setVisible(visable);
}

void BattleLayer::battleLose(CCObject *obj)
{
    m_pMapUnitsLayer->setisStop(true);
    beforeSendBattleResult(0);
}

void BattleLayer::battleWin(CCObject *obj)
{
    
    gUserData->m_dataEx->lastMissionId = m_pMissionConf->nMissionId;
    if (ISTUTORIAMISSION(m_pMissionConf->nMissionId))//新手任务
    {
        GameManager::sharedInstance()->replaceScene(COMMUNITY_SCENCE, false);
    }
    else//正常流程
    {
        m_pMapUnitsLayer->setisStop(true);
        beforeSendBattleResult(1);
    }
}

void BattleLayer::exitGame()
{
    beforeSendBattleResult(2);
}

void BattleLayer::onReConnect(CCObject *obj)
{
    sendBattleResult(m_battleResult);
}

void BattleLayer::onWorldBossOverNotice(CCObject* obj)
{
    if (m_kBattleType == kWorldBoss)
    {
        m_pMapUnitsLayer->setisStop(true);
        stopHpWarning();
        
        NotificeObject* pObj = dynamic_cast<NotificeObject*>(obj);
        if (pObj)
        {
            cli_user_world_boss_over_noti_out* out = (cli_user_world_boss_over_noti_out*)pObj->msg;
            
        }
    }
}

void BattleLayer::beforeSendBattleResult(int result)
{
    if (m_kBattleType == kWorldBoss)
    {
        gUserDataEx->boss_cur_hp = 0;
        int boss_hurt_hp = m_pMapUnitsLayer->m_monsterContainer.getBossHurHp();
        OnlineManager::sharedManager()->userWorldBossBTLOver(boss_hurt_hp);
        GameManager::sharedInstance()->replaceScene(COMMUNITY_SCENCE, true);
        return;
    }
    
    m_battleResult = result;
    if (LogicManager::shareLogicManager()->isOnlineConnected())
    {
        sendBattleResult(m_battleResult);
    }
    else
    {
        OnlineManager::sharedManager()->onReconnectOnline(NULL);
    }
}

void BattleLayer::sendBattleResult(int result)
{
    m_bHasGetAward = true;
    uint32_t monster[4];
    m_pMapUnitsLayer->getKillMonsters(monster);
    m_pMapUnitsLayer->limitExp();
    
    float maxHp = (float)m_pMapUnitsLayer->getHero()->getUnitBaseConf()->getMaxHP();
    float curHp = (float)m_pMapUnitsLayer->getHero()->getHp();
    uint32_t remainHp = (uint32_t)(curHp / maxHp * 100);
    
    OnlineManager::sharedManager()->battleOverToOnline(m_pMissionConf->nMissionId, result, monster, m_pMapUnitsLayer->getYxb(), m_pMapUnitsLayer->getExp(), m_pMapUnitsLayer->getGold(),m_nUseMedicineCount, &(m_pMapUnitsLayer->m_dieMonsterMap),&(m_pMapUnitsLayer->m_dropItemMap), &(m_pMapUnitsLayer->m_thiefItemMap),m_pRole->getMaxCombo(), m_pMapUnitsLayer->getBeHitCount(), remainHp);
    gUserDataEx->storeBattleInfo(m_pMissionConf->nMissionId, &m_pMapUnitsLayer->m_dieMonsterMap);
}

void BattleLayer::update(float dt)
{
    if (!m_pRole)
    {
        return;
    }
    
    // 行走动画
    if (m_pUIContainer->getRunLeft())
    {
        m_pRole->goBack();
    }
    else if (m_pUIContainer->getRunRight())
    {
        m_pRole->goForward();
    }

    m_pMapUnitsLayer->battleMapUpdate(dt);
}

void BattleLayer::onBattleEnd(CCObject *obj)
{
    stopHpWarning();
    cancelAutoFight(NULL);
    this->unschedule(schedule_selector(BattleLayerBase::updatelastTime));
    CCSprite *bg = CCSprite::create("BattleUI/kill_bg.png");
    bg->setScaleX(SCREEN_WIDTH/bg->getContentSize().width);
    bg->setScaleY(SCREEN_HEIGTH/bg->getContentSize().height);
    bg->setAnchorPoint(CCPointZero);
    this->addChild(bg,7);
    CCRepeat *repeat = CCRepeat::create(CCSequence::create(CCCallFuncN::create(this, callfuncN_selector(BattleLayer::onWinBgChange)),CCDelayTime::create(0.1),NULL),30);
    bg->runAction(CCSequence::create(repeat,CCRemoveSelf::create(),NULL));
}

void BattleLayer::onWinBgChange(CCNode*node)
{
    if(node->getChildByTag(1))
    {
        node->getChildByTag(1)->removeFromParent();
        CCSprite *bg = CCSprite::create("BattleUI/kill_bg1.png");
        bg->setAnchorPoint(CCPointZero);
        bg->setTag(2);
        node->addChild(bg);
    }
    else if(node->getChildByTag(2))
    {
        node->getChildByTag(2)->removeFromParent();
        CCSprite *bg = CCSprite::create("BattleUI/kill_bg2.png");
        bg->setAnchorPoint(CCPointZero);
        bg->setTag(1);
        node->addChild(bg);
    }
    else
    {
        CCSprite *bg = CCSprite::create("BattleUI/kill_bg2.png");
        bg->setAnchorPoint(CCPointZero);
        bg->setTag(1);
        node->addChild(bg);
    }
}

void BattleLayer::onPanelDialSelectDial(uint32_t nCostGold, int nTimes)
{
    m_nCardCostGold = nCostGold;
    OnlineManager::sharedManager()->getCardPrizeToOnline(m_pMissionConf->nMissionId, nCostGold, nTimes);
}
void BattleLayer::onPanelDialSelectReturn()
{
    if (this->getBattleType() == kTowerBoss)
    {
    }
    else if (this->getBattleType() == kEliteBoss)
    {
    }
    
    GameManager::sharedInstance()->replaceScene(COMMUNITY_SCENCE,true);
}

void BattleLayer::onPanelCardSelectNewSlotClick(uint32_t nCostGold, uint32_t nCardState)
{
    m_nCardCostGold = nCostGold;
    OnlineManager::sharedManager()->getCardPrizeToOnline(m_pMissionConf->nMissionId, nCostGold, nCardState);
}

void BattleLayer::onPanelCardSelectNewReturn()
{
    m_pUIContainer->m_pBtnAutoFight->setVisible(false);
    m_pBtnPause->setVisible(false);
    m_pBtnRenew->setVisible(false);
    m_pBtnClose->setVisible(false);
}

void BattleLayer::onPanelCardSelectNewClose()
{
    GameManager::sharedInstance()->replaceScene(COMMUNITY_SCENCE,true);
}

void BattleLayer::onGetCardPrize(item_unit_t* pCard, bool bIsBagFull)
{
    gUserData->addGold(-m_nCardCostGold);
    
    char str[128] = {0};
    sprintf(str, "item_%d.png", pCard->item_id);
    const item_t* pItem = GameData::getItem(pCard->item_id);
    //m_pPanelCardSelect->setCardPrize(str, pItem->name, pCard->item_id, pCard->item_cnt);
    m_pPanelDialSelect->setPrize(str, pItem->name, pCard->item_id, pCard->item_cnt, bIsBagFull);
}

void BattleLayer::onTowerBattleWin(std::vector<item_unit_t>* pCardVec, int nTotalCostGold)
{
    m_pPanelDialSelect = dynamic_cast<CCBPanelDialSelect*>(CREATE_CCB_NODE("CCBPanelDialSelect", CCBPanelDialSelectLoader::loader(), "BattleResultNew/PanelDialSelect.ccbi", this));
    this->addChild(m_pPanelDialSelect, kResultPanelOrder, kResultPanelOrder);
    CCSize size = m_pPanelDialSelect->getContentSize();
    m_pPanelDialSelect->setPosition(ccpAdd(SCREEN_MID_POS, ccp(-size.width / 2, -size.height / 2)));
    m_pPanelDialSelect->setDelegate(this);
    m_pPanelDialSelect->setData(pCardVec, nTotalCostGold);
}

/*
void BattleLayer::onTowerBattleWin(std::vector<item_unit_t>* pPassPrize)
{
    TowerBattleResult* node = TowerBattleResult::create();
    node->setStatus(true);
    node->loadPrize(m_pMissionConf->nMissionId, pPassPrize);
    
    CCSize size = node->getContentSize();
    this->addChild(node, kResultPanelOrder, kResultPanelOrder);
    node->setPosition(ccpAdd(SCREEN_MID_POS, ccp(-size.width / 2, -size.height / 2)));
}
 */

void BattleLayer::onTowerBattleLose()
{
    
}

void BattleLayer::onBattleLose(uint32_t map_type)
{
    CCNode *node = CREATE_CCB_NODE("CCBPanelFailureNew", CCBPanelFailureNewLoader::loader(), "BattleResultNew/PanelFailureNew.ccbi", this);
    ((CCBPanelFailureNew*)(node))->setMissionType(map_type);
    this->addChild(node, kResultPanelOrder, kResultPanelOrder);
    CCSize size = node->getContentSize();
    node->setPosition(ccpAdd(SCREEN_MID_POS, ccp(-size.width / 2, -size.height / 2)));
}

void BattleLayer::onBattleFirstWin()
{
    if (m_cardVec.size() > 0)//如果是第一次通关
    {
        //CCNode *node = CREATE_CCB_NODE("CCBPanelCardSelectNew", CCBPanelCardSelectNewLoader::loader(), "BattleResultNew/PanelCardSelectNew.ccbi", this);
        //this->addChild(node, kResultPanelOrder, kResultPanelOrder);
        //CCSize size = node->getContentSize();
        //node->setPosition(ccpAdd(SCREEN_MID_POS, ccp(-size.width / 2, -size.height / 2)));
        //m_pPanelCardSelect = dynamic_cast<CCBPanelCardSelectNew*>(node);
        //m_pPanelCardSelect->setDelegate(this);
        
        m_pPanelDialSelect = dynamic_cast<CCBPanelDialSelect*>(CREATE_CCB_NODE("CCBPanelDialSelect", CCBPanelDialSelectLoader::loader(), "BattleResultNew/PanelDialSelect.ccbi", this));
        this->addChild(m_pPanelDialSelect, kResultPanelOrder, kResultPanelOrder);
        CCSize size = m_pPanelDialSelect->getContentSize();
        m_pPanelDialSelect->setPosition(ccpAdd(SCREEN_MID_POS, ccp(-size.width / 2, -size.height / 2)));
        m_pPanelDialSelect->setDelegate(this);
        
        std::vector<std::string> imgVec;
        std::vector<std::string> nameVec;
        std::vector<int> numVec;
        
        char str[128] = {0};
        int i = 0;
        for (std::vector<item_unit_t>::iterator it = m_cardVec.begin(); it != m_cardVec.end(); it++)
        {
            if (getKindOfItem(it->item_id) == 10) continue;
            
            sprintf(str, "item_%d.png", it->item_id);
            imgVec.push_back(str);
            nameVec.push_back(GameData::getItem(it->item_id)->name);
            numVec.push_back(it->item_cnt);
            i++;
        }
        
        //m_pPanelCardSelect->setData(&imgVec, &nameVec, &numVec);
        m_pPanelDialSelect->setData(&m_cardVec, m_nTotalCostGold);
    }
    else
    {
        GameManager::sharedInstance()->replaceScene(COMMUNITY_SCENCE,true);
    }
}

CCBPanelVictoryNew* BattleLayer::onBattleWin(uint32_t map_type, uint32_t nScore, std::vector<item_unit_t>* pFirstPrize, std::vector<item_unit_t>* pPassPrize, std::vector<item_unit_t>* pCardVec, int nTotalCostGold)
{
    m_pMapUnitsLayer->collectAllItems();
    
    CCNode *node = CREATE_CCB_NODE("CCBPanelVictoryNew", CCBPanelVictoryNewLoader::loader(), "BattleResultNew/PanelVictoryNew.ccbi", this);
    this->addChild(node, kResultPanelOrder, kResultPanelOrder);
    CCSize size = node->getContentSize();
    node->setPosition(ccpAdd(SCREEN_MID_POS, ccp(-size.width / 2, -size.height / 2)));
    
    StrVec roleNameVec;
    roleNameVec.push_back(UserData::sharedInstance()->nick);
    
    StrVec roleImgVec;
    roleImgVec.push_back("BattleResultNew/role.png");//角色图片信息还没连通
    
    int inforce = 0;
    for (std::vector<item_unit_t>::iterator it = pFirstPrize->begin(); it != pFirstPrize->end(); it++)
    {
        switch (it->item_id)
        {
            case 3:
                inforce = it->item_cnt;
                break;
            default:
                break;
        }
    }

    int exp = m_pMapUnitsLayer->getExp();
    int money = m_pMapUnitsLayer->getYxb();
    int gold = m_pMapUnitsLayer->getGold();
    std::map<int, int>* pDropMapItem = m_pMapUnitsLayer->getDropItemMap();
    
    for (std::vector<item_unit_t>::iterator it = pPassPrize->begin(); it != pPassPrize->end(); it++)
    {
        switch (it->item_id)
        {
            case 0:
                money += it->item_cnt;
                break;
            case 1:
                exp += it->item_cnt;
                break;
            default:
            {
                if (pDropMapItem->find(it->item_id) != pDropMapItem->end())
                {
                    pDropMapItem->find(it->item_id)->second += it->item_cnt;
                }
                else
                {
                    pDropMapItem->insert(std::pair<int, int>(it->item_id, it->item_cnt));
                }
            }
                break;
        }
    }
    
    m_cardVec.clear();
    if (pCardVec != NULL)
    {
        for (std::vector<item_unit_t>::iterator it = pCardVec->begin(); it != pCardVec->end(); it++)
        {
            m_cardVec.push_back(*it);
        }
    }
    m_nTotalCostGold = nTotalCostGold;
    
    CCBPanelVictoryNew* pPanel = dynamic_cast<CCBPanelVictoryNew*>(node);
    pPanel->setResult(map_type, nScore, &roleNameVec, &roleImgVec, inforce, exp, money, gold, pDropMapItem);
    pPanel->setFirstWinButton(m_cardVec.size() > 0);
    return pPanel;
}

void BattleLayer::networkBattleRestart()
{
    //许翔：加入联网功能后，不能直接重新开始战斗，而要告诉服务器重新开始，由回调方法通知重新开始战斗
    OnlineManager::sharedManager()->userBattleStart(m_pMissionConf->nMissionId);
}

void BattleLayer::createUITools()
{
#ifndef SERVER_RELEASE
    m_pShowLabel = CCLabelTTF::create("设置", "Arial", 32);
    m_pShowLabel->setColor(ccRED);
    m_bShowAttackRect = false;
    
    CCMenuItemLabel* pBtn = CCMenuItemLabel::create(m_pShowLabel, this, menu_selector(BattleLayer::onShowAttackRect));
    pBtn->setAnchorPoint(ccp(1, 1));
    pBtn->setPosition(ccp(SCREEN_WIDTH - 60, SCREEN_HEIGTH - 50));
    
    CCMenu* pMenu = CCMenu::create(pBtn, NULL);
    pMenu->setPosition(CCPointZero);
    pMenu->setTouchPriority(-5);
    this->addChild(pMenu, kUIOrder);
#endif
}

void BattleLayer::onShowAttackRect(CCObject* pSender)
{
    UiControlLayer* pLayer = UiControlLayer::create();
    this->addChild(pLayer, kResultPanelOrder, kResultPanelOrder);
}

void BattleLayer::showUnitHp(CCObject *obj)
{
    BattleUnitSprite* pSprite = (BattleUnitSprite*)obj;
    HpBarBase* pHpBar = NULL;
    if (pSprite->getUnitType() == kHero)
    {
        pHpBar = m_pHeroHpBar;
        if (m_pMapUnitsLayer->getBattleState() == kBattleing &&
            (float)m_pRole->getHp()/(float)m_pRole->getUnitBaseConf()->getMaxHP() * 100<=25 &&
            (float)m_pRole->getHp()/(float)m_pRole->getUnitBaseConf()->getMaxHP()>0 && m_pMapUnitsLayer->getBattleState() == kBattleing)
        {
            hpWarning();
        }
        else
        {
            stopHpWarning();
        }
    }
    else if (pSprite->getUnitType() == kBoss || pSprite->getUnitType() == kTree)
    {
        pHpBar = m_pBossHpBar1;
    }
    else if (pSprite->getUnitType() == kPartnerType)
    {
        pHpBar = m_pPartnerHpBar;
    }
    else
    {
        return;
    }
    if (pHpBar->getAttackUnit() != pSprite)
    {
        pHpBar->attachUnit(pSprite);
    }
    pHpBar->resetHpBar();
}

void BattleLayer::hpWarning()
{
    BattleLayerBase::hpWarning();
    CCSprite* pFlash = (CCSprite*)m_pBtnMed->getChildByTag(501);
    if (!pFlash && gUserData->nMedicine>0 && m_nUseMedicineCount<50)
    {
        CCSprite* pFlash = EffectResource::sharedInstance()->spriteWithEffect(35, true, true, 2);
        m_pBtnMed->addChild(pFlash);
        pFlash->setTag(501);
        pFlash->setPosition(MIDDLE_POI(m_pBtnMed));
    }
}

void BattleLayer::stopHpWarning()
{
    BattleLayerBase::stopHpWarning();
    CCSprite* pFlash = (CCSprite*)m_pBtnMed->getChildByTag(501);
    if (pFlash)
    {
        pFlash->removeFromParent();
    }
}

void BattleLayer::recoverUIState()
{
    m_pHeroHpBar->detachUnit();
    m_pBossHpBar1->detachUnit();
    m_pPartnerHpBar->detachUnit();
    if (!m_pRole)
    {
        this->createRoleSprite();
        this->scheduleUpdate();
    }
}

void BattleLayer::onKillCount(CCObject *pSender)
{
    
    if (m_bCollectTask)
    {
        CCLabelTTF *plblTask = (CCLabelTTF*)this->getChildByTag(TASKLBLTAG);
        if(!plblTask)
        {
            plblTask = CCLabelTTF::create("",FONTNAME, 18);
            plblTask->setAnchorPoint(ccp(1.0f, 0.5f));
            plblTask->setPosition(ccp(SCREEN_WIDTH-10,SCREEN_HEIGTH-145));
            plblTask->setTag(TASKLBLTAG);
            this->addChild(plblTask);
        }
        int taskID = gUserDataEx->getCurTask();
        if (taskID != -1 && gUserDataEx->taskMap.find(taskID) != gUserDataEx->taskMap.end())
        {
            task_info_t taskInfo = gUserDataEx->taskMap[taskID];
            const TaskConfig_t *taskConf = GameData::getTaskConf(taskInfo.task_id);
            MonsterUnitConf *unitConf = GameData::getMonsterConf(taskConf->nNeedID);
            int step = taskInfo.cur_step;
            if (m_pMapUnitsLayer->m_dieMonsterMap.find(taskConf->nNeedID)!= m_pMapUnitsLayer->m_dieMonsterMap.end())
            {
                step = m_pMapUnitsLayer->m_dieMonsterMap[taskConf->nNeedID]+ taskInfo.cur_step;
                step = step > taskInfo.max_step?taskInfo.max_step:step;
            }
            
            plblTask->setString(CCString::createWithFormat("%s:%d/%d",unitConf->unitName,step,taskInfo.max_step)->getCString());
            if (step == taskInfo.max_step)
            {
                plblTask->setColor(FONT_COLOR_GREEN);
            }
        }
    }
}
