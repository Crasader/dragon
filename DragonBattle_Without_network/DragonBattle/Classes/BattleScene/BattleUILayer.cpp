//
//  BattleUILayer.cpp
//  DragonBattle
//
//  Created by zeng xiaolong on 13-10-22.
//
//

#include "BattleUILayer.h"
#include "GlobalDefine.h"
#include "UserData.h"

#define SILL_KEY        GET_KEY_WITH_USER(gUserData->userid, "skill")


static CCPoint s_SkillPoi[4] = {ccp(- 222, 143),ccp(- 147, 216),ccp(-45,250),ccp(-355,45)};

BattleUILayer::BattleUILayer()
{
    m_bRunLeft = false;
    m_bRunRight = false;
    m_pLeftTouch = NULL;
    m_pRightTouch = NULL;

    m_pAttackBtn = NULL;
    m_pRollBtn = NULL;
    m_pSkillBtn[0] = NULL;
    m_pSkillBtn[1] = NULL;
    m_pSkillBtn[2] = NULL;
    m_pSkillBtn[3] = NULL;
    m_pBtnAutoFight = NULL;

}

BattleUILayer::~BattleUILayer()
{

}

bool BattleUILayer::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    float deltaX = 0;
    if (IS_IPHONE_5_INCH)
    {
        deltaX = 20.f;
    }
    setTouchEnabled(true);
    setTouchPriority(-10);
    setTouchEnabled(false);
    setTouchEnabled(true);
    
    m_pLeftTouch = CCSprite::create("BattleUI/left-auto.png");
    m_pLeftTouch->setPosition(ccp(90 + deltaX, 100));
    addChild(m_pLeftTouch);
    
    m_pRightTouch = CCSprite::create("BattleUI/right-auto.png");
    m_pRightTouch->setPosition(ccp(280 + deltaX, 100));
    m_pRightTouch->setFlipX(true);
    addChild(m_pRightTouch);
    
    m_pAttackBtn = XYBattleButton::createWithFileAndPriority("BattleUI/attack-auto.png", "BattleUI/attack-hover.png", "BattleUI/attack-auto.png", -1);
    m_pAttackBtn->setSound(false);
    m_pAttackBtn->setTouchBeganEvent(this, callfuncN_selector(BattleUILayer::onAttackTouchBegan));
    m_pAttackBtn->setTouchEndEvent(this, callfuncN_selector(BattleUILayer::onAttackTouchEnded));
    m_pAttackBtn->setPosition(ccp(SCREEN_WIDTH - 90, 90));
    addChild(m_pAttackBtn);
    
    m_pRollBtn = XYRollButton::createWithFileAndPriority("BattleUI/rush-auto.png", "BattleUI/rush-auto_sele.png", NULL, -1);
    m_pRollBtn->setTouchRect(CCRectMake(-10, -10, m_pRollBtn->getContentSize().width+20,  m_pRollBtn->getContentSize().height+20));
    m_pRollBtn->setSound(false);
    m_pRollBtn->setTouchBeganEvent(this, callfuncN_selector(BattleUILayer::onRollTouchBegan));
    m_pRollBtn->setPosition(ccp(SCREEN_WIDTH - 250, 45));
    addChild(m_pRollBtn);
    
    m_pBtnAutoFight = XYToggleButton::create("BattleUI/btn_autofight_sele.png","BattleUI/btn_autofight_unsele.png", -11);
    m_pBtnAutoFight->setTouchEndEvent(this, callfuncN_selector(BattleUILayer::onAutoFightTouchBegan));
    m_pBtnAutoFight->setPosition(ccp(295, SCREEN_HEIGTH - 34));
    addChild(m_pBtnAutoFight);
    
    for (int i = 0; i<4;i++)
    {
        if (gUserData->m_heroData->skill[i] != 0)
        {
            const SkillConf_t* pSkill = GameData::getSkillConf(GET_SKILL_ID(gUserData->m_heroData->skill[i]));
            if (pSkill)
            {
                m_pSkillBtn[i] = XYRollButton::createWithFileAndPriority("BattleUI/skill-null-auto.png", "BattleUI/skill-null-auto.png", NULL, -1);
                m_pSkillBtn[i]->setSound(false);
                m_pSkillBtn[i]->setImage(CCString::createWithFormat("skillicon/skillicon_%d.png",pSkill->skillBase.nSkillID)->getCString(), 1);
                m_pSkillBtn[i]->setTag(i);
                m_pSkillBtn[i]->addImage("BattleUI/skill_cd.png",-4, 22);
                m_pSkillBtn[i]->setTouchBeganEvent(this, callfuncN_selector(BattleUILayer::onSkillTouchBegan));
                m_pSkillBtn[i]->setPosition(ccp(SCREEN_WIDTH + s_SkillPoi[i].x,s_SkillPoi[i].y));
                const SkillLevel_t *pSkilllv = GameData::getSkillConf(GET_SKILL_ID(gUserData->m_heroData->skill[i]), GET_SKILL_LEVEL(gUserData->m_heroData->skill[i]));
                m_pSkillBtn[i]->setCD(pSkilllv->skillLvBase.fCDTime);
                addChild(m_pSkillBtn[i]);
            }
        }
    }
    
    return true;
}



void BattleUILayer::onAttackTouchBegan(CCNode *node)
{
    m_delegate->onAttackTouchBegan();
}

void BattleUILayer::onAutoFightTouchBegan(CCNode *node)
{
    m_delegate->onAutoFightTouchBegan(node);
}

void BattleUILayer::onAttackTouchEnded(CCNode *node)
{
    m_delegate->onAttackTouchEnded();
}

void BattleUILayer::onRollTouchBegan(CCNode *node)
{
    m_delegate->onRollTouchBegan(node);
}

void BattleUILayer::onSkillTouchBegan(CCNode *node)
{
    m_delegate->onSkillTouchBegan(node);
}

void BattleUILayer::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    CCTouch *touch = (CCTouch*)pTouches->anyObject();
	CCPoint curPoint = touch->getLocationInView();
	curPoint = CCDirector::sharedDirector()->convertToGL(curPoint);
    
    CCRect rectLeft = m_pLeftTouch->boundingBox();
    CCRect rectRight = m_pRightTouch->boundingBox();
    if (rectLeft.containsPoint(curPoint))
    {
        m_bRunLeft = true;
        m_bRunRight = false;
    }
    else if (rectRight.containsPoint(curPoint))
    {
        m_bRunLeft = false;
        m_bRunRight = true;
    }
        
    m_pLeftTouch->stopAllActions();
    m_pLeftTouch->runAction(CCScaleTo::create(0.05f, m_bRunLeft ? 1.1 : 1));
    m_pRightTouch->stopAllActions();
    m_pRightTouch->runAction(CCScaleTo::create(0.05f, m_bRunRight ? 1.1 : 1));
}

void BattleUILayer::ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    CCTouch *touch = (CCTouch*)pTouches->anyObject();
	CCPoint curPoint = touch->getLocationInView();
	curPoint = CCDirector::sharedDirector()->convertToGL(curPoint);
    
    CCRect rectLeft = m_pLeftTouch->boundingBox();
    CCRect rectRight = m_pRightTouch->boundingBox();
    
    if (rectLeft.containsPoint(curPoint))
    {
        m_bRunLeft = true;
        m_bRunRight = false;
    }
    else if (rectRight.containsPoint(curPoint))
    {
        m_bRunLeft = false;
        m_bRunRight = true;
    }
    else
    {
        if (m_bRunLeft || m_bRunRight)
        {
            m_bRunLeft = false;
            m_bRunRight = false;
            
            if (m_delegate)
            {
                m_delegate->directMoveOutside();
            }
        }
    }
    m_pLeftTouch->stopAllActions();
    m_pLeftTouch->runAction(CCScaleTo::create(0.05f, m_bRunLeft ? 1.1 : 1));
    m_pRightTouch->stopAllActions();
    m_pRightTouch->runAction(CCScaleTo::create(0.05f, m_bRunRight ? 1.1 : 1));
}

void BattleUILayer::ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    CCTouch *touch = (CCTouch*)pTouches->anyObject();
	CCPoint curPoint = touch->getLocationInView();
	curPoint = CCDirector::sharedDirector()->convertToGL(curPoint);
    
    CCRect rectLeft = m_pLeftTouch->boundingBox();
    CCRect rectRight = m_pRightTouch->boundingBox();
    
    if (rectLeft.containsPoint(curPoint)||rectRight.containsPoint(curPoint))
    {
        m_bRunLeft = false;
        m_bRunRight = false;
        if (m_delegate)
        {
            m_delegate->directTouchEnd();
        }
    }
    m_pLeftTouch->stopAllActions();
    m_pLeftTouch->runAction(CCScaleTo::create(0.05f, m_bRunLeft ? 1.1 : 1));
    m_pRightTouch->stopAllActions();
    m_pRightTouch->runAction(CCScaleTo::create(0.05f, m_bRunRight ? 1.1 : 1));
}