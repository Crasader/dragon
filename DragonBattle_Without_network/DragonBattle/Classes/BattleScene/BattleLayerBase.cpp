//
//  BattleLayerBase.cpp
//  DragonBattle
//
//  Created by zeng xiaolong on 13-10-22.
//
//

#include "BattleLayerBase.h"
#include "NotificeDefine.h"
#include "UserData.h"
#include "BattleMapUnits.h"
#include "XYLblButton.h"
#include "ProgressLayerNew.h"

USING_NS_CC;

#define AUTOFIGHT_LAYER (301)

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOSUtility.h"
#endif

BattleLayerBase::BattleLayerBase()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    setMulitTouch(true);
#else
    this->setTouchMode(kCCTouchesAllAtOnce);
#endif
    m_bHasGetAward =false;
    m_pMapNode = NULL;
    m_pMapUnitsLayer = NULL;
    m_pRole = NULL;
    m_pUIContainer = NULL;
    m_pMissionConf = NULL;
    m_lasttime = NULL;
    m_timeBar = NULL;
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BattleLayerBase::onMpChange), MP_CHANGE_NOTE, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BattleLayerBase::onAIPlaySkill), AI_SKILL_NOTE, NULL);
}

BattleLayerBase::~BattleLayerBase()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    setMulitTouch(false);
#else
    this->setTouchMode(kCCTouchesOneByOne);
#endif
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}

BattleLayerBase* BattleLayerBase::createWithID(uint16_t nMissionID)
{
    BattleLayerBase *pRet = new BattleLayerBase();
    if (pRet && pRet->initWithID(nMissionID))
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

bool BattleLayerBase::initWithID(uint16_t nMissionID)
{
    if (!CCLayer::init())
    {
        return false;
    }
    m_pMissionConf = GameData::getBattleMission(nMissionID);
    loadResource();
    return true;
}

void BattleLayerBase::createUI()
{
    m_pUIContainer = BattleUILayer::create();
    m_pUIContainer->setDelegate(this);
    this->addChild(m_pUIContainer, kUIOrder);
}
//
void BattleLayerBase::update(float dt)
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

void BattleLayerBase::setSkillBtnStatue(XYRollButton *btn,int skill)
{
    if (skill != 0)
    {
        const SkillConf_t* pSkill = GameData::getSkillConf(GET_SKILL_ID(skill));
        if (pSkill)
        {
            const SkillLevel_t * pskilllv= GameData::getSkillConf(GET_SKILL_ID(skill),GET_SKILL_LEVEL(skill));
            if(m_pRole->getMp()>=pskilllv->skillLvBase.nCostEnergy)
            {
                if (btn)
                {
                    if (btn->getEnable() == false)
                    {
                        btn->setEnable(true);
                        btn->setImage(CCString::createWithFormat("skillicon/skillicon_%d.png",pSkill->skillBase.nSkillID)->getCString(), 1);
                    }
                }
            }
            else
            {
                if (btn)
                {
                    if (btn->getEnable() == true)
                    {
                        btn->setEnable(false);
                        btn->setGayImage(CCString::createWithFormat("skillicon/skillicon_%d.png",pSkill->skillBase.nSkillID)->getCString(), 1);
                    }
                }
            }
        }
    }
}

void BattleLayerBase::onMpChange(CCObject *pSender)
{
    m_pHeroHpBar->setMp(m_pRole->getMp(),m_pRole->getMaxMp());
    for (int i = 0; i<4; i++)
    {
        setSkillBtnStatue(m_pUIContainer->m_pSkillBtn[i],gUserData->m_heroData->skill[i]);
    }
    
    if(m_pRole->getMp()<10)
    {
        m_pUIContainer->m_pRollBtn->setEnable(false);
        m_pUIContainer->m_pRollBtn->setScale(1);
    }
    else
    {
        m_pUIContainer->m_pRollBtn->setEnable(true);
        m_pUIContainer->m_pRollBtn->setScale(1);
    }
}

void BattleLayerBase::createMapBkg()
{
    m_pMapNode = CCNode::create();
    m_pMapNode->setPosition(CCPointZero);
    m_pMapUnitsLayer = BattleMapUnits::create(m_pMissionConf);
    m_pMapNode->addChild(m_pMapUnitsLayer, kMapLayerOrder, kMapLayerOrder);
    addChild(m_pMapNode);
    m_pMapUnitsLayer->setPosition(ccp(0, 0));
}

void BattleLayerBase::createbattle()
{
    this->unscheduleAllSelectors();
    this->createUnitHpBar();
    this->createUI();

    this->createMapBkg();
    this->createRoleSprite();
    
    onStart();
}

void BattleLayerBase::onStart()
{
    this->setTouchEnabled(true);
    this->scheduleUpdate();
    m_lasttime = m_pMissionConf->fPassTime;
    if(m_lasttime>0)
    {
        m_timeBar = CCSprite::create();
        m_timeBar->setPosition(ccp(SCREEN_WIDTH/2,SCREEN_HEIGTH-40));
        m_timeBar->setAnchorPoint(ccp(0.5,0.5));
        addChild(m_timeBar,kUIOrder);
        this->schedule(schedule_selector(BattleLayerBase::updatelastTime), 1.0f);
    }
}

void BattleLayerBase::updatelastTime(float time)
{
    if (m_pMapUnitsLayer->getBattleState() != kBattleing)
    {
        this->unschedule(schedule_selector(BattleLayer::updatelastTime));
        return;
    }
    if(m_pMapUnitsLayer->getisStop())
    {
        return;
    }
    m_lasttime --;
    CCLabelTTF *plblTime = NULL;
    if (!m_timeBar->getChildByTag(TIMELBLTAG))
    {
        plblTime = CCLabelTTF::create("",FONTNAME, 18);
        plblTime->setColor(FONT_COLOR_GOLD);
        plblTime->setPosition(ccp(m_timeBar->getContentSize().width/2,m_timeBar->getContentSize().height/2));
        plblTime->setTag(TIMELBLTAG);
        m_timeBar->addChild(plblTime);
    }
    else
    {
        plblTime = (CCLabelTTF*)m_timeBar->getChildByTag(TIMELBLTAG);
    }
    char str[64] = {0};
    int fen = m_lasttime/60%60;
    int miao = m_lasttime%60;
    sprintf(str,"%02d:%02d",fen,miao);
    plblTime->setString(str);
    
    if (m_lasttime > 0 && m_lasttime <= 10)
    {
        CCLabelAtlas* pLabNum = CCLabelAtlas::create(CCString::createWithFormat("%d", m_lasttime)->getCString(), "BattleUI/role_harm_num.png", 25, 33, '0');
        this->addChild(pLabNum);
        pLabNum->setPosition(SCREEN_MID_POS);
        pLabNum->setAnchorPoint(ccp(0.5f, 0.5f));
        pLabNum->setScale(8);
        pLabNum->runAction(CCSequence::create(CCScaleTo::create(0.3f, 4), CCDelayTime::create(0.7f), CCRemoveSelf::create(), NULL));
    }
    
    if (m_lasttime == 0)//时间到 输了
    {
        this->unschedule(schedule_selector(BattleLayer::updatelastTime));
        if (m_pMissionConf->bFancy)
        {
            battleWin(NULL);
        }
        else
        {
            battleLose(NULL);
        }
    }
}

void BattleLayerBase::loadResource()
{
    CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
    std::vector<std::string> resourceVec;
    BattleCacheManager::sharedInstance()->loadResourcesBeforBattle(m_pMissionConf->nMissionId);
    BattleCacheManager::sharedInstance()->getResourcesVec(&resourceVec);
    ProgressLayerNew* pLayer = ProgressLayerNew::create(&resourceVec, true, callfuncN_selector(BattleLayerBase::LoadcreateBattle), this);
    this->addChild(pLayer, 130);
    pLayer->setPosition(ccp(SCREEN_WIDTH / 2, SCREEN_HEIGTH / 2));
}

void BattleLayerBase::LoadcreateBattle(CCNode* pNode)
{
    createbattle();
}

void BattleLayerBase::directMoveOutside()
{
    if (m_pRole && m_pRole->getCurAction() == kRunning)
    {
        m_pRole->stopRunning();
    }
}

void BattleLayerBase::directTouchEnd()
{
    m_pRole->stopRunning();
}

void BattleLayerBase::onAttackTouchBegan()
{
    if (!m_pRole) {
        return;
    }
    m_pRole->setAttackBtnDown(true);
    m_pRole->attackBegin();
}

void BattleLayerBase::onAttackTouchEnded()
{
    if (!m_pRole) {
        return;
    }
    m_pRole->setAttackBtnDown(false);
}

void BattleLayerBase::onSkillTouchBegan(CCNode *node)
{
    XYRollButton *btn = (XYRollButton*)node;
    if (!btn->getEnable())
    {
        XYTopLayer::getInstance()->ShowTip("怒气值不足");
        return;
    }
    if (!m_pRole)
    {
        return;
    }
    m_pRole->skillAttack(btn->getTag());
}

void BattleLayerBase::onAutoFightTouchBegan(CCNode *node)
{
    XYToggleButton *btn = (XYToggleButton*)node;
    if(btn->getTurnOn())
    {
        TouchLayer *layer = TouchLayer::create();
        layer->setPrority(-10);
        CCSprite *spr = CCSprite::create("BattleUI/ui_autofighting.png");
        spr->setPosition(SCREEN_MID_POS);
        spr->setPositionY(SCREEN_HEIGTH*2/3);
        layer->addChild(spr);
        
        XYLblButton *btn = XYLblButton::create("点击取消", FONTNAME, 24,-11);
        btn->setTouchRect(CCRect(-30, 0, btn->getContentSize().width+60, btn->getContentSize().height+80));
        btn->setTouchEndEvent(this, callfuncN_selector(BattleLayer::cancelAutoFight));
        btn->setPosition(SCREEN_MID_POS);
        btn->setPositionY(SCREEN_HEIGTH*2/3-30);
        btn->setColor(FONT_COLOR_GOLD);
        layer->addChild(btn);
        
        layer->setTag(AUTOFIGHT_LAYER);
        addChild(layer);
        m_pRole->setAIActivated(true);
    }
    else
    {
        CCLayer *layer = (CCLayer*)getChildByTag(AUTOFIGHT_LAYER);
        if (layer)
        {
            layer->removeFromParent();
        }
        m_pRole->setAIActivated(false);
    }
}

void BattleLayerBase::onAIPlaySkill(CCObject *pSender)
{
    CCInteger *interer = (CCInteger*)pSender;
    m_pUIContainer->m_pSkillBtn[interer->getValue()]->showProgressSprite();
}

void BattleLayerBase::onRollTouchBegan(CCNode *node)
{
    XYRollButton *btn = (XYRollButton*)node;
    if (!btn->getEnable())
    {
        XYTopLayer::getInstance()->ShowTip("怒气值不足");
    }
    if (!m_pRole)
    {
        return;
    }
    if(m_pRole->getMp()>=5)
    {
        if(m_pRole->roll())
        {
            m_pRole->addMp(-5);
        }
    }
}

void BattleLayerBase::createUnitHpBar()
{
    m_pHeroHpBar = HpBarLayer::create(kHero);
    m_pHeroHpBar->setPosition(ccp(0, SCREEN_HEIGTH));
    m_pHeroHpBar->attachUnit(m_pRole);
    this->addChild(m_pHeroHpBar, kUIOrder);
}

void BattleLayerBase::createRoleSprite()
{
    gUserData->m_heroData->refreshBaseAttr();
    UnitBaseConf* pHeroConf = &(gUserData->m_heroData->baseAttr);
    m_pRole = HeroSprite::create(pHeroConf);
    m_pMapUnitsLayer->addSelf(m_pRole);
    m_pMapUnitsLayer->createPartner(0);
}

void BattleLayerBase::hpWarning()
{
    CCSprite *spBlood = (CCSprite *)this->getChildByTag(BOOLDTAG);
    if (spBlood == NULL)
    {
        spBlood = CCSprite::create("BattleUI/alert_hp.png");
        float scale = SCREEN_HEIGTH/spBlood->getContentSize().height>SCREEN_WIDTH/spBlood->getContentSize().width?SCREEN_HEIGTH/spBlood->getContentSize().height:SCREEN_WIDTH/spBlood->getContentSize().width;
        spBlood->setScale(scale);
        spBlood->setPosition(ccp(SCREEN_WIDTH/2,SCREEN_HEIGTH/2));
        spBlood->setTag(BOOLDTAG);
        this->addChild(spBlood,50);
    }
    if (spBlood->numberOfRunningActions()== 0)
    {
        spBlood->setVisible(true);
        spBlood->setOpacity(0);
        float scale = SCREEN_HEIGTH/spBlood->getContentSize().height>SCREEN_WIDTH/spBlood->getContentSize().width?SCREEN_HEIGTH/spBlood->getContentSize().height:SCREEN_WIDTH/spBlood->getContentSize().width;
        CCRepeatForever *pRepeat = CCRepeatForever::create(CCSequence::create(
                                                                              CCSpawn::create(CCScaleTo::create(0.3, scale*1.05),CCFadeTo::create(0.3,30.0*255/100),NULL),
                                                                              CCSpawn::create(CCScaleTo::create(0.5, scale*1),CCFadeTo::create(0.5,80.0*255/100),NULL),NULL));
        spBlood->runAction(pRepeat);
    }
}

void BattleLayerBase::stopHpWarning()
{
    CCSprite *spBlood = (CCSprite *)this->getChildByTag(BOOLDTAG);
    if (spBlood)
    {
        spBlood->stopAllActions();
        spBlood->setVisible(false);
    }
}

void BattleLayerBase::networkBattleRestart()
{
    
}