//
//  CCBPanelVictoryNew.cpp
//  DragonBattle
//
//  Created by 何 永翔 on 13-8-31.
//
//

#include "CCBPanelVictoryNew.h"
#include "CCBDefine.h"
#include "CCShake.h"
#include "GameManager.h"
#include "XYMessageBox.h"
#include "AudioResource.h"
#include "BattleLayer.h"
#include "lib_formula.h"
#include "CommonUtility.h"
#include "ItemSprite.h"

#define EQUIPMENT_ITEM_BACKGROUND_NORMAL ("public/public_frame_0.png")
#define NUM_COUNT (30)

USING_NS_CC_EXT;
using namespace std;
using namespace cocos2d;
using namespace LIB_FORMULA;

CCBPanelVictoryNew::CCBPanelVictoryNew()
{
    m_pCountDown = NULL;
    m_pBottomBlack = NULL;
    m_pBonus = NULL;
    m_pBottom = NULL;
    m_pCircle0 = NULL;
    m_pCircle1 = NULL;
    m_pStar[0] = NULL;
    m_pStar[1] = NULL;
    m_pStar[2] = NULL;
    
    m_pBtnRenew = NULL;
    m_pBtnReturn = NULL;
    
    m_nCurrentNum = 0;
    m_nMissionType = 1;
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CCBPanelVictoryNew::onShowDialog),GET_AND_BUY_ENERY_COUNT, NULL);
}

CCBPanelVictoryNew::~CCBPanelVictoryNew()
{
    CC_SAFE_RELEASE_NULL(m_pCountDown);
    CC_SAFE_RELEASE_NULL(m_pBottomBlack);
    CC_SAFE_RELEASE_NULL(m_pBonus);
    CC_SAFE_RELEASE_NULL(m_pBottom);
    CC_SAFE_RELEASE_NULL(m_pCircle0);
    CC_SAFE_RELEASE_NULL(m_pCircle1);
    CC_SAFE_RELEASE_NULL(m_pStar[0]);
    CC_SAFE_RELEASE_NULL(m_pStar[1]);
    CC_SAFE_RELEASE_NULL(m_pStar[2]);
    
    CC_SAFE_RELEASE_NULL(m_pBtnRenew);
    CC_SAFE_RELEASE_NULL(m_pBtnReturn);
    
    this->unscheduleAllSelectors();
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}

void CCBPanelVictoryNew::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader)
{
    loadLayer();
}

SEL_MenuHandler CCBPanelVictoryNew::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool CCBPanelVictoryNew::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "CountDown", CCSprite*, this->m_pCountDown);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BottomBlack", CCSprite*, this->m_pBottomBlack);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "Bonus", CCSprite*, this->m_pBonus);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "Bottom", CCSprite*, this->m_pBottom);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "Circle0", CCSprite*, this->m_pCircle0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "Circle1", CCSprite*, this->m_pCircle1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "Star0", CCSprite*, this->m_pStar[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "Star1", CCSprite*, this->m_pStar[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "Star2", CCSprite*, this->m_pStar[2]);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnRenew", CCControlButton*, this->m_pBtnRenew);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnReturn", CCControlButton*, this->m_pBtnReturn);
    
    return false;
}

SEL_CCControlHandler CCBPanelVictoryNew::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onReturn", CCBPanelVictoryNew::onReturn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onRenew", CCBPanelVictoryNew::onRenew);
    return NULL;
}

void CCBPanelVictoryNew::onReturn(CCObject* pTarget, cocos2d::extension::CCControlEvent event)
{
    this->unschedule(schedule_selector(CCBPanelVictoryNew::countDown));
    switch (m_nMissionType)
    {
        case 1://普通关卡
        case 6:
        {
            dynamic_cast<BattleLayer*>(GameManager::sharedInstance()->getBattleLayer())->onBattleFirstWin();
            this->removeFromParent();
        }
            break;
        case 2:
            GameManager::sharedInstance()->replaceScene(COMMUNITY_SCENCE,true);
            break;
        case 4: //爬塔
            GameManager::sharedInstance()->replaceScene(COMMUNITY_SCENCE,true);
            break;
        default:
            break;
    }
}
void CCBPanelVictoryNew::onRenew(CCObject* pTarget, cocos2d::extension::CCControlEvent event)
{
    if (gUserData->getEnery()>0)
    {
        GameManager::sharedInstance()->restartBattle();
        this->removeFromParent();
    }
    else
    {
        OnlineManager::sharedManager()->getDailyEvent(DAILY_BUY_ENERY_TYPE);
    }
}

void CCBPanelVictoryNew::onShowDialog(cocos2d::CCObject *p)
{
    if(GameData::getVipFunction(gUserData->getVipLevel(), DAYBUYENERY) < gUserData->getBuyEneryCount())
    {
        XYTopLayer::getInstance()->ShowTip("购买次数已满，升级VIP可增加购买次数！");
        return;
    }
    if (gUserData->getGold()<cal_buy_energy_cost_gold(gUserData->getBuyEneryCount()))
    {
        GoldGuideMessageBox::showGetGuideLayer("您的黄金不足，无法购买体力，是否充值？");
        return;
    }
    CCString* msg = CCString::createWithFormat("是否消耗%d黄金购买25点体力?",cal_buy_energy_cost_gold(gUserData->getBuyEneryCount()));
    XYMessageBox * pMsgBox = XYMessageBox::create(E_MB_OKCANCEL, "", msg->getCString(), this, callfuncN_selector(CCBPanelVictoryNew::onAddConfirm), callfuncN_selector(CCBPanelVictoryNew::onCancel));
    pMsgBox->setMsgAlignment(kCCTextAlignmentCenter);
    pMsgBox->showMessageBox();
}

void CCBPanelVictoryNew::onCancel(CCNode* p)
{
    onReturn(NULL,NULL);
}

void CCBPanelVictoryNew::onAddConfirm(CCNode* p)
{
    OnlineManager::sharedManager()->userBuyEneryToOnline(cal_buy_energy_cost_gold(gUserData->getBuyEneryCount()));
}

void CCBPanelVictoryNew::loadLayer()
{
    m_pCircle0->setVisible(false);
    m_pCircle1->setVisible(false);
    
    char str[128] = {0};
    sprintf(str, "BattleResultNew/role%d.png",gUserData->m_heroData->nUnitID);
    m_pRole = CCSprite::create(str);
    m_pRole->setPosition(ccp(228, 336));
    m_pRole->setVisible(true);
    m_pRole->setScale(0);
    m_pBonus->setVisible(false);
    this->addChild(m_pRole);
    
    m_pBottom->setPosition(ccp(SCREEN_WIDTH + m_pBottom->getContentSize().width / 2, m_pBottom->getPosition().y));
    
    m_pBottomBlack->setOpacity(0);
    m_pBottomBlack->setScale(2);
    
    for (int i = 0; i < 3; i++)
    {
        m_pStar[i]->setOpacity(0);
        m_pStar[i]->setScale(15);
    }
    
    setPrority(-10);
    m_pBtnRenew->setTouchPriority(getPrority() - 1);
    m_pBtnReturn->setTouchPriority(getPrority() - 1);
    
    bool bFancy = GameManager::sharedInstance()->getBattleLayer()->m_pMissionConf->bFancy;
    if (GameManager::sharedInstance()->getBattleLayer()->getBattleType() == kEliteBoss || bFancy)
    {
        m_pBtnRenew->setVisible(false);
        m_pBtnRenew->setEnabled(false);
    }
}

void CCBPanelVictoryNew::setFirstWinButton(bool bIsFirst)
{
    if (!bIsFirst && GameManager::sharedInstance()->getBattleLayer()->getBattleType() != kEliteBoss)
    {
        if (!GameManager::sharedInstance()->getBattleLayer()->m_pMissionConf->bFancy)
        {
            m_pBtnRenew->setVisible(true);
            m_pBtnRenew->setEnabled(true);
        }
        m_pBtnReturn->setTitleForState(CCString::create("返回主城"), CCControlStateNormal);
        m_pBtnReturn->setTitleForState(CCString::create("返回主城"), CCControlStateHighlighted);
        m_pBtnReturn->setTitleForState(CCString::create("返回主城"), CCControlStateDisabled);
        m_pBtnReturn->setPosition(ccp(620, 36));
    }
}

void CCBPanelVictoryNew::startRotate(float dt)
{
    this->unschedule(schedule_selector(CCBPanelVictoryNew::startRotate));

    m_pCircle0->setVisible(true);
    m_pCircle1->setVisible(true);
    
    m_pCircle0->runAction(CCRepeatForever::create(CCRotateBy::create(20, -360)));
    m_pCircle1->runAction(CCRepeatForever::create(CCRotateBy::create(10, 360)));
}

void CCBPanelVictoryNew::playStarSoundEffect()
{
    AudioResource::shareAudioResource()->playEffect("music/star.mp3");
}

void CCBPanelVictoryNew::setResult(int mission_type, int nStar, StrVec* pRoleNameVec, StrVec* pRoleImgVec, int nInForce, int nExp, int nMoney, int nGold, std::map<int, int>* pDropMapItem)
{
    m_nMissionType = mission_type;

    m_pRole->runAction(CCSequence::create(CCScaleTo::create(0.28, 1.2), CCScaleTo::create(0.02, 1), NULL));
    
    m_pBottom->runAction(CCMoveTo::create(0.3, ccp(585.3, 336.3)));
    
    CCSequence* pSeq = CCSequence::create(CCSpawn::create(CCFadeIn::create(0.3), CCEaseSineInOut::create(CCScaleTo::create(0.3, 1)), NULL), CCCallFunc::create(this,callfunc_selector(CCBPanelVictoryNew::shakeSelf)), NULL);
    m_pBottomBlack->runAction(pSeq);
    CCSequence* pSeqCopy = CCSequence::create(CCSpawn::create(CCFadeIn::create(0.3), CCEaseSineInOut::create(CCScaleTo::create(0.3, 0.6)), NULL), CCCallFunc::create(this,callfunc_selector(CCBPanelVictoryNew::shakeSelf)), NULL);
    
    switch (nStar)
    {
        case 1:
        {
            CCSequence* pSeq1 = CCSequence::create(CCDelayTime::create(0.3), pSeq, CCCallFunc::create(this, callfunc_selector(CCBPanelVictoryNew::playStarSoundEffect)), NULL);
            m_pStar[0]->runAction(pSeq1);
        }
            break;
        case 2:
        {
            CCSequence* pSeq1 = CCSequence::create(CCDelayTime::create(0.3), pSeqCopy, CCCallFunc::create(this, callfunc_selector(CCBPanelVictoryNew::playStarSoundEffect)), NULL);
            m_pStar[1]->runAction(pSeq1);
            CCSequence* pSeq2 = CCSequence::create(CCDelayTime::create(0.6), pSeqCopy, CCCallFunc::create(this, callfunc_selector(CCBPanelVictoryNew::playStarSoundEffect)), NULL);
            m_pStar[2]->runAction(pSeq2);
        }
            break;
        case 3:
        {
            CCSequence* pSeq1 = CCSequence::create(CCDelayTime::create(0.3), pSeq, CCCallFunc::create(this, callfunc_selector(CCBPanelVictoryNew::playStarSoundEffect)), NULL);
            m_pStar[0]->runAction(pSeq1);
            CCSequence* pSeq2 = CCSequence::create(CCDelayTime::create(0.6), pSeqCopy, CCCallFunc::create(this, callfunc_selector(CCBPanelVictoryNew::playStarSoundEffect)), NULL);
            m_pStar[1]->runAction(pSeq2);
            CCSequence* pSeq3 = CCSequence::create(CCDelayTime::create(0.9), pSeqCopy, CCCallFunc::create(this, callfunc_selector(CCBPanelVictoryNew::playStarSoundEffect)), NULL);
            m_pStar[2]->runAction(pSeq3);
        }
            break;
        default:
            break;
    }
    float totalTime = 0.3 + (nStar - 1) * 0.3 + 0.3 + 0.1;
    
    this->scheduleOnce(schedule_selector(CCBPanelVictoryNew::startRotate), totalTime);
    
    gUserData->addExp(nExp);
    gUserData->addYXB(nMoney);
    gUserData->addGold(nGold);
    
    this->addToVec(1, nExp, pDropMapItem);
    this->addToVec(0, nMoney, pDropMapItem);
    this->addToVec(3, nInForce, pDropMapItem);
    
    this->runAction(CCSequence::create(CCDelayTime::create(totalTime), CCCallFunc::create(this,callfunc_selector(CCBPanelVictoryNew::startCalculate)), NULL));
    
    const int xOffset = m_pBonus->getPosition().x;
    const int yItemOffset = m_pBonus->getPosition().y;
    const int nItemBtnOffsetX = 96;
    int nCount = pDropMapItem->size();
    if (nCount > 4)
    {
        nCount = 4;
    }
    int nStartX = xOffset;
    if (nCount % 2 == 0)//偶数
    {
        nStartX = nStartX - nCount / 2 * nItemBtnOffsetX + nItemBtnOffsetX / 2;
    }
    else//奇数
    {
        nStartX = nStartX - nCount / 2 * nItemBtnOffsetX;
    }
    int i = 0;
    for (std::map<int, int>::iterator it = pDropMapItem->begin(); it != pDropMapItem->end(); it++)
    {
        CCSprite* itembtn = CCSprite::create(CCString::createWithFormat("public/public_frame_%d.png", GameData::getItem(it->first)->nQuality)->getCString());
        ItemSprite* btn = CommonUtility::createButton(it->first, it->second, this, callfuncN_selector(CCBPanelVictoryNew::onItem), false, true, -89);
        itembtn->addChild(btn);
        btn->setPosition(ccp(itembtn->getContentSize().width / 2.0f, itembtn->getContentSize().height / 2.0f));
        m_pBonus->getParent()->addChild(itembtn);
        itembtn->setPosition(ccp(nStartX + i * nItemBtnOffsetX, yItemOffset));
        itembtn->setCascadeOpacityEnabled(true);
        itembtn->setOpacity(0);
        itembtn->setScale(15);
        itembtn->setVisible(false);

        itembtn->runAction(CCSequence::create(CCDelayTime::create(totalTime + i * 0.3f), CCShow::create(), dynamic_cast<CCSequence*>(pSeq->copy()->autorelease()), CCCallFunc::create(this, callfunc_selector(CCBPanelVictoryNew::playStarSoundEffect)), NULL));
        i++;
    }
}

void CCBPanelVictoryNew::onItem(CCNode* pNode)
{
    ItemSprite* pBtn = dynamic_cast<ItemSprite*>(pNode);
}

void CCBPanelVictoryNew::addToVec(int nItemId, int nItemCount, std::map<int, int>* pDropMapItem)
{
    if (nItemCount > 0)
    {
        pDropMapItem->insert(std::pair<int, int>(nItemId, nItemCount));
    }
}

void CCBPanelVictoryNew::countDown(float dt)
{
    CCLabelAtlas* lbl = dynamic_cast<CCLabelAtlas*>(m_pCountDown->getChildren()->objectAtIndex(0));
    int num = lbl->getTag();
    if (num == 0)
    {
        onReturn(NULL, 0);
    }
    else
    {
        num--;
        lbl->setString(CCString::createWithFormat("%d", num)->getCString());
        lbl->setTag(num);
    }
}

void CCBPanelVictoryNew::startCalculate()
{
    //this->schedule(schedule_selector(CCBPanelVictoryNew::update), 0.1, NUM_COUNT - 1, 0);
    
    int num = 20;
    CCLabelAtlas* lbl = CCLabelAtlas::create(CCString::createWithFormat("%d", num)->getCString(), "public/lblnum.png", 26, 32, '0');
    m_pCountDown->addChild(lbl);
    lbl->setAnchorPoint(CCPointZero);
    lbl->setPosition(ccp(m_pCountDown->getContentSize().width, 10));
    lbl->setTag(num);
    this->schedule(schedule_selector(CCBPanelVictoryNew::countDown), 1);
}

void CCBPanelVictoryNew::update(float dt)
{
    m_nCurrentNum += 1;
}

void CCBPanelVictoryNew::shakeSelf()
{
    CCShake* pAction = CCShake::create(0.1, 5);
    this->runAction(pAction);
}