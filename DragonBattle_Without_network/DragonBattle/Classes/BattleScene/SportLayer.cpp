//
//  SportLayer.cpp
//  DragonBattle
//
//  Created by zeng xiaolong on 13-10-23.
//
//

#include "SportLayer.h"
#include "BattleMapUnits.h"
#include "ProgressLayerNew.h"
#include "UserData.h"
#include "ArenaResultLayer.h"
#include "OnlineManager.h"
#include "GameManager.h"
#include "CountdownLayer.h"
#include "LogicManager.h"

SportLayer::SportLayer()
{
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SportLayer::showUnitHp), UNITHPCHANGE_NOTE, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SportLayer::battleLose), BATTLELOST_NOTE, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SportLayer::battleWin), BATTLEWIN_NOTE, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SportLayer::onGetBattleResultComplete), ARENAFIGHTOVER_NOTE, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SportLayer::onGetKeapResultComplete), USER_END_KEAP_HORSE_NOTE, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SportLayer::onBattleEnd), BATTLEEND_NOTE, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SportLayer::onReConnect), BATTLERECONNECT_NOTE, this);
    m_pEnemyRole = NULL;
}

SportLayer::~SportLayer()
{
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}

void SportLayer::loadLayer(UnitBaseConf Conf,uint32_t *skills,int targetid,uint8_t *session)
{
    m_kBattleType = kArena;
    m_pMissionConf = GameData::getBattleMission(0);
    emenyHeroConf = Conf;
    memcpy(&m_nEnemykill,skills, sizeof(m_nEnemykill));
    m_targetID = targetid;
    loadResource();
}

void SportLayer::loadResource()
{
    CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
    std::vector<std::string> resourceVec;
    BattleCacheManager::sharedInstance()->loadResourcesBeforBattle(m_pMissionConf->nMissionId);
    BattleCacheManager::sharedInstance()->loadUnitResource(emenyHeroConf.nResourceID,m_nEnemykill);
    BattleCacheManager::sharedInstance()->getResourcesVec(&resourceVec);
    resourceVec.push_back(string("item/items.plist"));
    ProgressLayerNew* pLayer = ProgressLayerNew::create(&resourceVec, true, callfuncN_selector(SportLayer::LoadcreateBattle), this);
    this->addChild(pLayer, 130);
    pLayer->setPosition(ccp(SCREEN_WIDTH / 2, SCREEN_HEIGTH / 2));
}

void SportLayer::onBattleEnd(CCObject *obj)
{
    this->unschedule(schedule_selector(SportLayer::updatelastTime));
    CCLayerColor *layer = CCLayerColor::create(ccc4BFromccc4F(ccc4FFromccc3B(ccRED)));
    layer->setContentSize(CCSize(SCREEN_WIDTH*2, SCREEN_HEIGTH*2));
    CCPoint poi = m_pMapUnitsLayer->convertToNodeSpace(CCPointZero);
    layer->setPosition(ccp(poi.x-0.5*SCREEN_WIDTH,poi.y- 0.5*SCREEN_HEIGTH));
    m_pMapUnitsLayer->addChild(layer,kBlackMaskOrder);
    layer->setTag(1);
    CCRepeat *repeat = CCRepeat::create(CCSequence::create(CCCallFuncN::create(this, callfuncN_selector(SportLayer::onWinBgChange)),CCDelayTime::create(0.1),NULL),20);
    layer->runAction(CCSequence::create(repeat,CCCallFuncO::create(this, callfuncO_selector(SportLayer::battleResultEffect), obj),CCRemoveSelf::create(),NULL));
}

void SportLayer::onWinBgChange(CCNode*node)
{
    CCLayerColor *layer = (CCLayerColor*)node;
    if (layer->getTag() == 1)
    {
        layer->setColor(ccWHITE);
        layer->setTag(2);
    }
    else
    {
        layer->setColor(ccRED);
        layer->setTag(1);
    }
}

void SportLayer::battleResultEffect(CCObject *obj)
{
    CCString *str = (CCString*)obj;
    CCSprite *pic = NULL;
    if (str->boolValue())
    {
        pic = CCSprite::create("BattleUI/win.png");
    }
    else
    {
        pic = CCSprite::create("BattleUI/lose.png");
    }
    pic->setPosition(ccp(SCREEN_WIDTH/2, SCREEN_HEIGTH/2));
    pic->setScale(3);
    addChild(pic);
    pic->runAction(CCSequence::create(CCEaseExponentialIn::create(CCScaleTo::create(0.3, 1)),CCDelayTime::create(0.7),CCRemoveSelf::create(),NULL));
}

// create UI button
void SportLayer::createUI()
{
    m_pUIContainer = BattleUILayer::create();
    m_pUIContainer->setDelegate(this);
    this->addChild(m_pUIContainer, kUIOrder);
}

void SportLayer::onPause(CCNode *node)
{    
    CCDirector::sharedDirector()->setAnimationInterval(1.0 / 60);
    BaseMessageBox *box = BaseMessageBox::create("竞技场" ,"你确定退出竞技场?", true,this,callfuncN_selector(SportLayer::onBack), callfuncN_selector(SportLayer::onContune));
    CCDirector::sharedDirector()->pause();
    box->m_pLabelTitle->setPositionY(260);
    box->m_pLabelMsg->setHorizontalAlignment(kCCTextAlignmentCenter);
    box->setBtnText(1,"退出");
    box->setBtnText(2,"继续");
    TOPADDCHILD(box);
}

void SportLayer::onBack(CCNode *node)
{
    CCDirector::sharedDirector()->resume();
    OnlineManager::sharedManager()->userOverArenaBattle(0,m_targetID,NULL);
    GameManager::sharedInstance()->replaceScene(COMMUNITY_SCENCE, true);
    if (getBattleType() == kArena)
    {
    }
    else if (getBattleType() == kKeap)
    {
    }
    
}

void SportLayer::onContune(CCNode *node)
{
    CCDirector::sharedDirector()->resume();
}

void SportLayer::createUnitHpBar()
{
    m_pHeroHpBar = HpBarLayer::createArena(kHero);
    m_pHeroHpBar->setPosition(ccp(0, SCREEN_HEIGTH));
    this->addChild(m_pHeroHpBar, kUIOrder);
    
    m_pEnemyHeroHpBar = HpBarLayer::createArena(kEnemyHero);
    m_pEnemyHeroHpBar->setPosition(ccp(SCREEN_WIDTH - m_pEnemyHeroHpBar->getContentSize().width,SCREEN_HEIGTH));
    m_pEnemyHeroHpBar->setBarFlipX(true);
    this->addChild(m_pEnemyHeroHpBar, kUIOrder);
}
void SportLayer::createbattle()
{
    this->unscheduleAllSelectors();
    this->createUI();
    this->createMapBkg();
    this->createUnitHpBar();
    this->createRoleSprite();
    
    this->createEmenySprite();
    m_pEnemyHeroHpBar->attachUnit(m_pEnemyRole);
    CountdownLayer *layer = CountdownLayer::create();
    m_pUIContainer->m_pBtnAutoFight->setPosition(ccp(50, SCREEN_HEIGTH - 110));
    layer->setBattleCbk(this, callfunc_selector(SportLayer::onStart));
    addChild(layer);
}

void SportLayer::onStart()
{
    BattleLayerBase::onStart();
    if (m_timeBar)
    {
        m_timeBar->setPosition(ccp(SCREEN_WIDTH/2,SCREEN_HEIGTH-40));
    }
}

void SportLayer::update(float dt)
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

void SportLayer::updatelastTime(float time)
{
    if (m_pMapUnitsLayer->getBattleState() != kBattleing)
    {
        this->unschedule(schedule_selector(SportLayer::updatelastTime));
        return;
    }
    m_lasttime --;
    CCLabelAtlas *plblTime = (CCLabelAtlas*)m_timeBar->getChildByTag(TIMELBLTAG);
    if (!plblTime)
    {
        plblTime = CCLabelAtlas::create("0", "public/public_lblatles_yellow.png", 72,89, '0');
        plblTime->setScale(0.8);
        plblTime->setAnchorPoint(ccp(0.5, 0.5));
        plblTime->setPosition(ccp(0,0));
        plblTime->setTag(TIMELBLTAG);
        m_timeBar->addChild(plblTime);
    }
    char str[64] = {0};
    sprintf(str,"%d",m_lasttime);
    plblTime->setString(str);
    if (m_lasttime == 0)//时间到 输了
    {
        this->unschedule(schedule_selector(SportLayer::updatelastTime));
        battleLose(NULL);
    }
}

void SportLayer::battleLose(CCObject *obj)
{
    m_pEnemyRole->setAIActivated(false);
    stopHpWarning();
    beforeSendBattleResult(0);
    m_pUIContainer->setVisible(false);
}

void SportLayer::battleWin(CCObject *obj)
{
    m_pMapUnitsLayer->setisStop(true);
    stopHpWarning();
    beforeSendBattleResult(1);
    m_pUIContainer->setVisible(false);
}

void SportLayer::onReConnect(CCObject *obj)
{
    sendBattleResult(m_battleResult);
    if (getBattleType() == kArena)
    {
        sendBattleResult(m_battleResult);
    }
    else if (getBattleType() == kKeap)
    {
        GameManager::sharedInstance()->replaceScene(COMMUNITY_SCENCE,true);
    }
}

void SportLayer::sendBattleResult(int result)
{
    if (getBattleType() == kArena)
    {
        OnlineManager::sharedManager()->userOverArenaBattle(result,m_targetID,NULL);
    }
    else if (getBattleType() == kKeap)
    {
        OnlineManager::sharedManager()->userOverKeapHorse(result, m_targetID, NULL);
    }
}

void SportLayer::beforeSendBattleResult(int result)
{
    m_bHasGetAward = true;
    m_battleResult = result;
    if (LogicManager::shareLogicManager()->isOnlineConnected())
    {
        sendBattleResult(m_battleResult);
    }
    else
    {
        m_battleResult = 0;
        OnlineManager::sharedManager()->onReconnectOnline(NULL);
    }
}

void SportLayer::onGetKeapResultComplete(CCObject *p)
{
    NotificeObject *obj = (NotificeObject*)p;
    cli_user_over_keap_horse_out *out = (cli_user_over_keap_horse_out*)obj->msg;
    if (out->battle_result == 0)
    {
       
    }
    else
    {

    }
}

void SportLayer::onGetBattleResultComplete(CCObject *p)
{
    NotificeObject *obj = (NotificeObject*)p;
    cli_user_over_arena_battle_out *out = (cli_user_over_arena_battle_out*)obj->msg;
    int copper = 0;
    int prestige = 0;
    for(int i = 0;i<out->prize_vec.size();i++)
    {
        item_unit_t item = out->prize_vec.at(i);
        if (item.item_id == 0)
        {
            copper = item.item_cnt;
            gUserData->addYXB(copper);
        }
        
        if (item.item_id == 6)
        {
            prestige = item.item_cnt;
            gUserData->addPrestige(prestige);
        }
    }
    /*
    ArenaResultLayer * arena = ArenaResultLayer::create(out->battle_result, copper, prestige, this, callfuncN_selector(SportLayer::onReturnCmtLayer));
    TOPADDCHILD(arena);
     */
    item_unit_t items[2] = {0};
    item_unit_t item0;
    item0.item_id = 0;
    item0.item_cnt = copper;
    items[0] = item0;
    item_unit_t item1;
    item1.item_id = 6;
    item1.item_cnt = prestige;
    items[1] = item1;
    
}

void SportLayer::onBodyguardCompleteLayerGetBonus()
{
    GameManager::sharedInstance()->replaceScene(COMMUNITY_SCENCE,true);
}

void SportLayer::onReturnCmtLayer(CCNode *node)
{
    GameManager::sharedInstance()->replaceScene(COMMUNITY_SCENCE,true);
}

void SportLayer::createRoleSprite()
{
    gUserData->m_heroData->refreshBaseAttr();
    UnitBaseConf pHeroConf = gUserData->m_heroData->baseAttr;
    if (pHeroConf.getLV()>50)
    {
        pHeroConf.nLife = pHeroConf.nLife*(1.0f +1.0f/20);
    }
    else
    {
        pHeroConf.nLife = pHeroConf.nLife*(1.0f + (51.0f - pHeroConf.getLV())/20);
    }
    float s = 1 + (pHeroConf.getLV() - emenyHeroConf.getLV())*0.005f;
    if (s>1.5f)
    {
        s = 1.5f;
    }
    
    if (s<0.5f)
    {
        s = 0.5f;
    }
    pHeroConf.nLife = pHeroConf.nLife*s;
    pHeroConf.fInAttack = pHeroConf.fInAttack*s;
    pHeroConf.fOutAttack = pHeroConf.fOutAttack*s;
    if (pHeroConf.getLV() == emenyHeroConf.getLV() && (pHeroConf.getFightPoint() - emenyHeroConf.getFightPoint()>200||pHeroConf.getFightPoint() - emenyHeroConf.getFightPoint()<-200 ))
    {
        s = 1 + 0.01f*(pHeroConf.getFightPoint() - emenyHeroConf.getFightPoint())/10000;
        pHeroConf.fInAttack = pHeroConf.fInAttack*s;
        pHeroConf.fOutAttack = pHeroConf.fOutAttack*s;
    }
    m_pRole = HeroSprite::create(&pHeroConf);
    m_pRole->setBeHitProtect(false);
    m_pMapUnitsLayer->addSelf(m_pRole);
}

void SportLayer::createEmenySprite()
{
    UnitBaseConf pEmenyConf = emenyHeroConf;
    pEmenyConf.nAIID = 27;
    pEmenyConf.nUnitType = kEnemyHero;
    if (pEmenyConf.getLV()>50)
    {
        pEmenyConf.nLife = pEmenyConf.nLife*(1.0f +1.0f/20);
    }
    else
    {
         pEmenyConf.nLife = pEmenyConf.nLife*(1.0f +(51.0f - pEmenyConf.getLV())/20);
    }
    m_pEnemyRole = m_pMapUnitsLayer->createEmeryHero(&pEmenyConf,m_nEnemykill);
}

void SportLayer::LoadcreateBattle(CCNode* pNode)
{
    createbattle();
}

void SportLayer::showUnitHp(CCObject *obj)
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
    else if(pSprite->getUnitType() == kEnemyHero)
    {
        pHpBar = m_pEnemyHeroHpBar;
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
