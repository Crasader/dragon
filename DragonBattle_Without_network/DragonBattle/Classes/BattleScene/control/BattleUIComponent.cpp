
//
//  BattleUIComponent.cpp
//  DragonBattle
//  没有素材，现在坐标点都直接写在代码中，测试用
//  Created by 小敦 王 on 13-5-22.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "BattleUIComponent.h"
#include "BattleUnitSprite.h"
#include "UserData.h"
USING_NS_CC;

HpBarBase::HpBarBase()
{
    m_pAttachUnit = NULL;
    m_pHeadSprite = NULL;
}

HpBarBase::~HpBarBase()
{
    
}

void HpBarBase::resetHpBar()
{
    if (!m_pAttachUnit)
    {
        return;
    }
    
    int maxHp = m_pAttachUnit->getUnitBaseConf()->nLife;
    
    if (maxHp == 0)
    {
        return ;
    }
    
    int curHp = m_pAttachUnit->getHp();
    
    if (curHp <= 0)
    {
        curHp = 0;
    }
    if (curHp > maxHp)
    {
        curHp = maxHp;
    }
    
    setBar(curHp,maxHp);
    if (curHp == 0 && m_pAttachUnit->getUnitType() !=kHero && m_pAttachUnit->getUnitType() !=kEnemyHero)
    {
        m_pAttachUnit = NULL;
        this->scheduleOnce(schedule_selector(HpBarBase::hideSelf), 3.0);
    }
}

void HpBarBase::attachUnit(BattleUnitSprite *pUnit)
{
    this->unschedule(schedule_selector(HpBarBase::hideSelf));
    
    m_pAttachUnit = pUnit;
    CCSprite* pSprite = CCSprite::create(pUnit->getUnitResource()->headImage.data());
    CCRect rect = CCRect(0, 0, pSprite->getContentSize().width, pSprite->getContentSize().height);
    m_pHeadSprite->setScale(0.7);
    CCSpriteFrame* pSpriteFrame = CCSpriteFrame::createWithTexture(pSprite->getTexture(), rect);
    m_pHeadSprite->setDisplayFrame(pSpriteFrame);
    
    m_pUnitName->setString(pUnit->getUnitBaseConf()->unitName);
    char str[64] = {0};
    sprintf(str, "LV.%d",pUnit->getUnitBaseConf()->getLV());
    m_pUnitLv->setString(str);
    this->setVisible(true);
    
    if (m_pAttachUnit->getHp() == 0)
    {
        this->scheduleOnce(schedule_selector(HpBarBase::hideSelf), 2.0);
    }
}

void HpBarBase::detachUnit()
{
    m_pAttachUnit = NULL;
    this->setVisible(false);
}

void HpBarBase::hideSelf(float dt)
{
    this->setVisible(false);
}

HpBarLayer::HpBarLayer()
{
    m_pMpBar = NULL;
}

HpBarLayer::~HpBarLayer()
{

}

HpBarLayer* HpBarLayer::create(int unitType)
{
    HpBarLayer* pRet = new HpBarLayer();
    if (pRet && pRet->init(unitType))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool HpBarLayer::init(int unitType)
{
    if (!CCSprite::initWithFile("BattleUI/ui_hp_frame.png"))
    {
        return false;
    }
    this->setVisible(false);
    m_pHpBar = CCProgressTimer::create(CCSprite::create("BattleUI/ui_hp.png"));
    loadLayer();
    return true;
}

HpBarLayer* HpBarLayer::createArena(int unitType)
{
    HpBarLayer* pRet = new HpBarLayer();
    if (pRet && pRet->initArena(unitType))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool HpBarLayer::initArena(int unitType)
{
    if(unitType == kHero)
    {
        if (!CCSprite::initWithFile("BattleUI/ui_hp_frame3.png"))
        {
            return false;
        }
        CCSprite * mpspr = CCSprite::create("BattleUI/ui_mp_frame.png");
        mpspr->setAnchorPoint(CCPointZero);
        mpspr->setPosition(ccp(70,7));
        addChild(mpspr);
    }
    else
    {
        if (!CCSprite::initWithFile("BattleUI/ui_hp_frame3.png"))
        {
            return false;
        }
    }
    this->setVisible(false);
    m_pHpBar = CCProgressTimer::create(CCSprite::create("BattleUI/ui_arenahp.png"));
    loadLayer();
    m_pUnitLv->setPosition(ccp(80,58));
    m_pUnitName->setPosition(ccp(140,58));
    m_pHpBar->setPosition(ccp(82,30));
    m_pMpBar->setPosition(88,11);
    m_pMpAddBar->setPosition(88,11);
    return true;
}

void HpBarLayer::setBarFlipX(bool bflipx)
{
    setFlipX(true);
    m_pUnitLv->setAnchorPoint(ccp(1, 0));
    m_pUnitLv->setPosition(ccp(getContentSize().width - m_pUnitLv->getPositionX(),m_pUnitLv->getPositionY()));
    
    m_pUnitName->setAnchorPoint(ccp(1, 0));
    m_pUnitName->setPosition(ccp(getContentSize().width - m_pUnitName->getPositionX(),m_pUnitName->getPositionY()));
    m_pHeadSprite->setPosition(ccp(getContentSize().width - m_pHeadSprite->getPositionX(), m_pHeadSprite->getPositionY()));
    
    m_pMpBar->setPosition(getContentSize().width - m_pMpBar->getPositionX(), m_pMpBar->getPositionY());
    m_pMpBar->setAnchorPoint(ccp(1, 0));
    m_pMpBar->setMidpoint(ccp(1, 0));
    m_pMpBar->setVisible(false);
    
    m_pHpBar->setPosition(getContentSize().width-m_pHpBar->getPositionX(),m_pHpBar->getPositionY());
    m_pHpBar->setAnchorPoint(ccp(1, 0));
    m_pHpBar->setMidpoint(ccp(1, 0));
    
    
    m_pMpAddBar->setPosition(getContentSize().width-m_pMpAddBar->getPositionX(), m_pMpAddBar->getPositionY());
    m_pMpAddBar->setAnchorPoint(ccp(1, 0));
    m_pMpAddBar->setMidpoint(ccp(1, 0));
    m_pMpAddBar->setVisible(false);
}

void HpBarLayer::loadLayer()
{
    setAnchorPoint(ccp(0,1));
    m_pUnitLv = CCLabelTTF::create("",FONTNAME, 18);
    m_pUnitLv->setColor(FONT_COLOR_GOLD);
    m_pUnitLv->setAnchorPoint(ccp(0, 0));
    m_pUnitLv->setPosition(ccp(80,10));
    this->addChild(m_pUnitLv,1);
    
    m_pUnitName = CCLabelTTF::create("",FONTNAME, 18);
    m_pUnitName->setAnchorPoint(ccp(0, 0));
    m_pUnitName->setPosition(ccp(130,10));
    this->addChild(m_pUnitName,1);
    
    m_pHpBar->setPosition(80, 54);
    m_pHpBar->setAnchorPoint(ccp(0, 0));
    m_pHpBar->setBarChangeRate(ccp(1, 0));
    m_pHpBar->setType(kCCProgressTimerTypeBar);
    m_pHpBar->setMidpoint(ccp(0, 0));
    m_pHpBar->setPercentage(99.9);
    this->addChild(m_pHpBar);
    
    m_pMpAddBar = CCProgressTimer::create(CCSprite::create("BattleUI/ui_mp_red.png"));
    m_pMpAddBar->setPosition(80, 35);
    m_pMpAddBar->setAnchorPoint(ccp(0, 0));
    m_pMpAddBar->setType(kCCProgressTimerTypeBar);
    m_pMpAddBar->setMidpoint(ccp(0, 0));
    m_pMpAddBar->setBarChangeRate(ccp(1, 0));
    m_pMpAddBar->setPercentage(99.9);
    this->addChild(m_pMpAddBar,1);
    
    m_pMpBar = CCProgressTimer::create(CCSprite::create("BattleUI/ui_mp.png"));
    m_pMpBar->setPosition(80, 35);
    m_pMpBar->setAnchorPoint(ccp(0, 0));
    m_pMpBar->setType(kCCProgressTimerTypeBar);
    m_pMpBar->setMidpoint(ccp(0, 0));
    m_pMpBar->setBarChangeRate(ccp(1, 0));
    m_pMpBar->setPercentage(99.9);
    this->addChild(m_pMpBar,1);
    
    m_pHeadSprite = CCSprite::create();
    m_pHeadSprite->setPosition(ccp(40, 36));
    this->addChild(m_pHeadSprite);
}

void HpBarLayer::attachUnit(BattleUnitSprite* pUnit)
{
    m_pAttachUnit = pUnit;
    CCSprite* pSprite = CCSprite::create(pUnit->getUnitResource()->headImage.data());
    CCRect rect = CCRect(0, 0, pSprite->getContentSize().width, pSprite->getContentSize().height);
    m_pHeadSprite->setScale(0.7);
    CCSpriteFrame* pSpriteFrame = CCSpriteFrame::createWithTexture(pSprite->getTexture(), rect);
    m_pHeadSprite->setDisplayFrame(pSpriteFrame);

    m_pUnitName->setString(pUnit->getUnitBaseConf()->unitName);
    char str[64] = {0};
    sprintf(str, "LV.%d",pUnit->getUnitBaseConf()->getLV());
    m_pUnitLv->setString(str);
    this->setVisible(true);
}

void HpBarLayer::setBar(int curHp,int maxHp)
{
    
    float fPercent = curHp * 100.0 / maxHp;
    if (fPercent > 99.9)
    {
        fPercent = 99.9;
    }
    m_pHpBar->stopAllActions();
    CCProgressTo* to = CCProgressTo::create(1 * fabsf(m_pHpBar->getPercentage() - fPercent) / 100, fPercent);
    m_pHpBar->runAction(to);
    
}

void HpBarLayer::setMp(float mp,int maxMp)
{
    if (m_pMpBar)
    {
        float ftime = 0.2;
        float mpPrecent = mp/maxMp;
        if (mpPrecent*100 > m_pMpBar->getPercentage())
        {
            ftime = 5 * fabsf(m_pMpBar->getPercentage() - mpPrecent*100) / 100;
        }
        CCProgressFromTo* to = CCProgressFromTo::create(ftime,m_pMpBar->getPercentage(),mpPrecent*100);
        m_pMpBar->runAction(to);
    }
    if (m_pMpAddBar)
    {
        m_pMpAddBar->setPercentage(mp/maxMp*100);
    }
}

PartnerHpBarLayer* PartnerHpBarLayer::create(int unitType)
{
    PartnerHpBarLayer* pRet = new PartnerHpBarLayer();
    if (pRet && pRet->init(unitType))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

void PartnerHpBarLayer::attachUnit(BattleUnitSprite *pUnit)
{
    this->unschedule(schedule_selector(HpBarBase::hideSelf));
    
    m_pAttachUnit = pUnit;
    char str[64] = {0};
    sprintf(str,"headicon/headicon_%d.png", pUnit->getUnitBaseConf()->nUnitID);
    CCSprite* pSprite = CCSprite::create(str);
    CCRect rect = CCRect(0, 0, pSprite->getContentSize().width, pSprite->getContentSize().height);
    m_pHeadSprite->setScale(0.7);
    CCSpriteFrame* pSpriteFrame = CCSpriteFrame::createWithTexture(pSprite->getTexture(), rect);
    m_pHeadSprite->setDisplayFrame(pSpriteFrame);
    
    m_pUnitName->setString(pUnit->getUnitBaseConf()->unitName);
    sprintf(str, "LV.%d",pUnit->getUnitBaseConf()->getLV());
    m_pUnitLv->setString(str);
    this->setVisible(true);
    
    if (m_pAttachUnit->getHp() == 0)
    {
        this->scheduleOnce(schedule_selector(HpBarBase::hideSelf), 2.0);
    }
}

bool PartnerHpBarLayer::init(int unitType)
{
    if (!CCSprite::initWithFile("BattleUI/ui_hp_frame_partner.png"))
    {
        return false;
    }
    setAnchorPoint(ccp(0,1));
    m_pUnitLv = CCLabelTTF::create("",FONTNAME, 18);
    m_pUnitLv->setColor(FONT_COLOR_GOLD);
    m_pUnitLv->setAnchorPoint(ccp(0, 0));
    m_pUnitLv->setPosition(ccp(82,20));
    this->addChild(m_pUnitLv,1);
    
    m_pUnitName = CCLabelTTF::create("",FONTNAME, 18);
    m_pUnitName->setAnchorPoint(ccp(0, 0));
    m_pUnitName->setPosition(ccp(130,20));
    this->addChild(m_pUnitName,1);
    
    m_pHpBar = CCProgressTimer::create(CCSprite::create("BattleUI/ui_hp_partner.png"));
    m_pHpBar->setPosition(75, 45);
    m_pHpBar->setAnchorPoint(ccp(0, 0));
    m_pHpBar->setBarChangeRate(ccp(1, 0));
    m_pHpBar->setType(kCCProgressTimerTypeBar);
    m_pHpBar->setMidpoint(ccp(0, 0));
    m_pHpBar->setPercentage(99.9);
    this->addChild(m_pHpBar);
    
    m_pHeadSprite = CCSprite::create();
    m_pHeadSprite->setPosition(ccp(34, 34));
    this->addChild(m_pHeadSprite);
    this->setVisible(false);
    return true;
}

void PartnerHpBarLayer::setBar(int curHp,int maxHp)
{
    float fPercent = curHp * 100.0 / maxHp;
    if (fPercent > 99.9)
    {
        fPercent = 99.9;
    }
    m_pHpBar->stopAllActions();
    CCProgressTo* to = CCProgressTo::create(1 * fabsf(m_pHpBar->getPercentage() - fPercent) / 100, fPercent);
    m_pHpBar->runAction(to);
}

BossHpBarLayer::BossHpBarLayer()
{
    m_nBarnum = 1;
    m_arrBar = CCArray::create();
    CC_SAFE_RETAIN(m_arrBar);
}

BossHpBarLayer::~BossHpBarLayer()
{
    CC_SAFE_RELEASE_NULL(m_arrBar);
}

BossHpBarLayer* BossHpBarLayer::create(int barnum)
{
    BossHpBarLayer* pRet = new BossHpBarLayer();
    if (pRet && pRet->init(barnum))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;

}

bool BossHpBarLayer::init(int barnum)
{
    if (!CCSprite::initWithFile("BattleUI/ui_hp_frame1.png"))
    {
        return false;
    }
    setAnchorPoint(ccp(0,1));
    m_pUnitLv = CCLabelTTF::create("",FONTNAME, 18);
    m_pUnitLv->setColor(FONT_COLOR_GOLD);
    m_pUnitLv->setAnchorPoint(ccp(0, 0));
    m_pUnitLv->setPosition(ccp(90,5));
    m_pUnitLv->setVisible(false);
    this->addChild(m_pUnitLv,1);
    
    m_pUnitName = CCLabelTTF::create("",FONTNAME, 18);
    m_pUnitName->setAnchorPoint(ccp(1, 0));
    m_pUnitName->setPosition(ccp(300,5));
    this->addChild(m_pUnitName,1);
    
    m_nBarnum = barnum;
    for (int i = 1; i<=m_nBarnum; i++)
    {
        CCProgressTimer *HpBar = CCProgressTimer::create(CCSprite::create(CCString::createWithFormat("BattleUI/ui_hp_boss_%d.png",i)->getCString()));
        HpBar->setPosition(10, 33);
        HpBar->setAnchorPoint(ccp(0, 0));
        HpBar->setType(kCCProgressTimerTypeBar);
        HpBar->setMidpoint(ccp(0, 0));
        HpBar->setBarChangeRate(ccp(1, 0));
        HpBar->setPercentage(99.9);
        this->addChild(HpBar);
        m_arrBar->addObject(HpBar);
    }
        
    m_pHeadSprite = CCSprite::create();
    m_pHeadSprite->setPosition(ccp(338, 36));
    this->addChild(m_pHeadSprite);
    
    lbl = CCLabelTTF::create("0", FONTNAME,24);
    lbl->setAnchorPoint(CCPointZero);
    lbl->setPosition(ccp(90, -10));
    lbl->setVisible(false);
    addChild(lbl);
    
    this->setVisible(false);
    return true;
}

void BossHpBarLayer::setBar(int curHp,int maxHp)
{
    char str[64] = {0};
    sprintf(str, "%d/%d",curHp,maxHp);
    lbl->setString(str);
    for (int i = 0; i<m_arrBar->count(); i++)
    {
        CCNode *node = (CCNode*)m_arrBar->objectAtIndex(i);
        node->stopAllActions();
    }
    float time = 0;
    for (int i = m_arrBar->count()-1; i>=0;i--)
    {
        CCProgressTimer *progress = (CCProgressTimer*)m_arrBar->objectAtIndex(i);
        float unit = ((float)maxHp)/m_arrBar->count();
        int fPercent =  100*(curHp - unit*i)/unit;
        fPercent = fPercent<0?0:fPercent;
        if (fPercent>=100 || fPercent<0 ||fPercent == progress->getPercentage())
        {
            continue;
        }
        CCProgressTo* to = CCProgressTo::create(0.5 * fabsf(progress->getPercentage() - fPercent) / 100, fPercent);
        progress->runAction(CCSequence::create(CCDelayTime::create(time),to,NULL));
        time += 0.5 * fabsf(progress->getPercentage() - fPercent) / 100;
    }
}

void BossHpBarLayer::attachUnit(BattleUnitSprite *pUnit)
{
    HpBarBase::attachUnit(pUnit);
    float maxHp = pUnit->getUnitBaseConf()->getMaxHP();
    float curHp = pUnit->getHp();
    for (int i = m_arrBar->count()-1; i>=0;i--)
    {
        CCProgressTimer *progress = (CCProgressTimer*)m_arrBar->objectAtIndex(i);
        float unit = ((float)maxHp)/m_arrBar->count();
        int fPercent =  100*(curHp - unit*i)/unit;
        fPercent = fPercent<0?0:fPercent;
        fPercent = fPercent>=100?100:fPercent;
        progress->setPercentage(fPercent);
    }
}