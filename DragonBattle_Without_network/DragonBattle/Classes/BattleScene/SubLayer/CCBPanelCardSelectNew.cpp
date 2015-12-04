//
//  CCBPanelCardSelectNew.cpp
//  DragonBattle
//
//  Created by 何 永翔 on 13-9-2.
//
//

#include "CCBPanelCardSelectNew.h"
#include "CCBDefine.h"
#include "OnlineManager.h"
#include "UserData.h"
#include "XYTopLayer.h"
#include "GameManager.h"
#include "XYMessageBox.h"
#include "CommonUtility.h"
#include "BattleLayer.h"

#define CARD_TAG (7216)
#define SLOT_TAG (9888)

USING_NS_CC_EXT;
using namespace std;

CCBPanelCardSelectNew::CCBPanelCardSelectNew()
{
    m_pGold = NULL;
    
    for (int i = 0; i < 5; i++)
    {
        m_pSlot[i] = NULL;
    }
    m_pBg = NULL;
    m_pCountDown = NULL;
    
    m_pBtnStart = NULL;
    m_pBtnBg = NULL;
    m_pBtnClose = NULL;
    m_pBtnRenew = NULL;
    m_pBtnReturn = NULL;
    
    m_CardMap.clear();
    m_PosMap.clear();
    m_pDelegate = NULL;
    m_bCanClick = false;
    m_nCardTag = 0;
    
    m_countDownHandler = NULL;
    
    m_pDelegate = NULL;
}

CCBPanelCardSelectNew::~CCBPanelCardSelectNew()
{
    CC_SAFE_RELEASE_NULL(m_pGold);
    
    for (int i = 0; i < 5; i++)
    {
        CC_SAFE_RELEASE_NULL(m_pSlot[i]);
    }
    CC_SAFE_RELEASE_NULL(m_pBg);
    CC_SAFE_RELEASE_NULL(m_pCountDown);
    
    CC_SAFE_RELEASE_NULL(m_pBtnStart);
    CC_SAFE_RELEASE_NULL(m_pBtnBg);
    CC_SAFE_RELEASE_NULL(m_pBtnClose);
    CC_SAFE_RELEASE_NULL(m_pBtnRenew);
    CC_SAFE_RELEASE_NULL(m_pBtnReturn);
    
    m_CardMap.clear();
    m_PosMap.clear();
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}

void CCBPanelCardSelectNew::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader)
{
    loadLayer();
}

SEL_MenuHandler CCBPanelCardSelectNew::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool CCBPanelCardSelectNew::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "Gold", CCLabelTTF*, this->m_pGold);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "Bg", CCSprite*, this->m_pBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "CountDown", CCSprite*, this->m_pCountDown);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "Slot0", CCControlButton*, this->m_pSlot[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "Slot1", CCControlButton*, this->m_pSlot[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "Slot2", CCControlButton*, this->m_pSlot[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "Slot3", CCControlButton*, this->m_pSlot[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "Slot4", CCControlButton*, this->m_pSlot[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnStart", CCControlButton*, this->m_pBtnStart);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnBg", CCControlButton*, this->m_pBtnBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnClose", CCControlButton*, this->m_pBtnClose);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnRenew", CCControlButton*, this->m_pBtnRenew);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnReturn", CCControlButton*, this->m_pBtnReturn);
    
    return false;
}

SEL_CCControlHandler CCBPanelCardSelectNew::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSlotClick", CCBPanelCardSelectNew::onSlotClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onStart", CCBPanelCardSelectNew::onStart);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClose", CCBPanelCardSelectNew::onClose);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onRenew", CCBPanelCardSelectNew::onRenew);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onReturn", CCBPanelCardSelectNew::onReturn);
    
    return NULL;
}

void CCBPanelCardSelectNew::onSlotClick(CCObject* pTarget, cocos2d::extension::CCControlEvent event)
{
    m_nCardTag = m_PosMap.find(dynamic_cast<CCNode*>(pTarget)->getTag() - SLOT_TAG)->second;
    if (!m_bCanClick || m_nCardTag == 0)
    {
        return;
    }
    //m_bCanClick = false;
    
    int costGold = this->getTotalGold(m_CardMap.size());
    
    this->unschedule(schedule_selector(CCBPanelCardSelectNew::countDown));
    
    if (this->getTotalGold(m_CardMap.size()) > 0)
    {
        if (gUserData->getGold() < costGold)
        {
            XYTopLayer::getInstance()->ShowTip("您的黄金不足！");
            int num = 20;
            
            CCLabelAtlas* lbl = dynamic_cast<CCLabelAtlas*>(m_pCountDown->getChildren()->objectAtIndex(0));
            lbl->setString(CCString::createWithFormat("%d", num)->getCString());
            lbl->setTag(num);
            
            m_countDownHandler = cccontrol_selector(CCBPanelCardSelectNew::onClose);
            
            this->schedule(schedule_selector(CCBPanelCardSelectNew::countDown), 1);
            return;
        }
        /*
        char str[128] = {0};
        sprintf(str, "你确认花费%d黄金抽牌么？", costGold);
        
        RemindMessageBox* pRemind = RemindMessageBox::create(NO_GOLD_BOOKED_WARNING, str, this, callfuncN_selector(CCBPanelCardSelectNew::onConfirmSpend), callfuncN_selector(CCBPanelCardSelectNew::onCancelSpend));
        pRemind->showMessageBox();
         */
        this->onConfirmSpend(NULL);
    }
    else
    {
        this->onConfirmSpend(NULL);
    }
}

void CCBPanelCardSelectNew::onConfirmSpend(CCNode* pNode)
{
    if (m_pDelegate != NULL)
    {
        m_pDelegate->onPanelCardSelectNewSlotClick(this->getTotalGold(m_CardMap.size()), 6 - m_CardMap.size());
    }
}
void CCBPanelCardSelectNew::onCancelSpend(CCNode* pNode)
{
    int num = 20;
    
    CCLabelAtlas* lbl = dynamic_cast<CCLabelAtlas*>(m_pCountDown->getChildren()->objectAtIndex(0));
    lbl->setString(CCString::createWithFormat("%d", num)->getCString());
    lbl->setTag(num);
    
    m_countDownHandler = cccontrol_selector(CCBPanelCardSelectNew::onAutoSelectCard);
    
    this->schedule(schedule_selector(CCBPanelCardSelectNew::countDown), 1);
}

void CCBPanelCardSelectNew::onRenew(CCObject* pTarget, cocos2d::extension::CCControlEvent event)
{
    if (gUserData->getEnery()>0)
    {
        GameManager::sharedInstance()->restartBattle();
        this->removeFromParent();
    }
    else
    {
        OnlineManager::sharedManager()->getDailyEvent(DAILY_BUY_ENERY_TYPE,1);
    }
}

void CCBPanelCardSelectNew::setCardPrize(const char* pImg, const char* pName, int nId, int nCount)
{
    for (std::map<int, card_t>::iterator it = m_CardMap.begin(); it != m_CardMap.end(); it++)
    {
        card_t& card = it->second;
        if (strcmp(card.pImg.c_str(), pImg) == 0 && nCount == card.nNum)
        {
            m_CardMap.erase(it);
            break;
        }
    }
    
    int nextGold = this->getTotalGold(m_CardMap.size());
    for (std::map<int, int>::iterator it = m_PosMap.begin(); it != m_PosMap.end(); it++)
    {
        if (it->second == m_nCardTag)
        {
            dynamic_cast<CCLabelTTF*>(m_pBg->getChildByTag(it->second)->getUserObject())->setString("");
            it->second = 0;
        }
        else
        {
            if (m_pBg->getChildByTag(it->second))
            {
                dynamic_cast<CCLabelTTF*>(m_pBg->getChildByTag(it->second)->getUserObject())->setString(CCString::createWithFormat("%d黄金", nextGold)->getCString());
            }
        }
    }

    /*
    CCLabelTTF* pTitle = dynamic_cast<CCLabelTTF*>(m_pGold->getParent()->getChildByTag(250));
    int nextGold = this->getTotalGold(m_CardMap.size());
    if (nextGold > 0)
    {
        pTitle->setString("再次抽取将消耗黄金");
        m_pGold->setString(CCString::createWithFormat("%d", nextGold)->getCString());
    }
    else
    {
        pTitle->setString("");
        m_pGold->setString("");
    }
     */
    
    const int prizeTag = 10086;
    CCNode* pFace = m_pBg->getChildByTag(m_nCardTag);
    
    //翻牌时，牌的背面也需要发光
    CCSprite* pBackLight = CCSprite::create("BattleResultNew/card_back_light.png");
    m_pBg->addChild(pBackLight);
    pBackLight->setPosition(pFace->getPosition());
    pFace->removeFromParent();
    pFace = pBackLight;
    pFace->setTag(m_nCardTag);

    pFace->setZOrder(200);
    CCNode* pBack = this->createCard(pImg, pName, nCount, prizeTag, pFace->getPosition(), true);
    pBack->setZOrder(200);
    this->flipCard(pFace, pBack, 1);
    m_pBtnClose->setVisible(true);
    //CommonUtility::setBtnState(m_pBtnClose, CCRectMake(10, 22, 140, 50), true);
    m_pBtnRenew->setVisible(true);
    //m_pBtnReturn->setVisible(true);
    
    int num = 20;
    CCLabelAtlas* lbl = dynamic_cast<CCLabelAtlas*>(m_pCountDown->getChildren()->objectAtIndex(0));
    lbl->setString(CCString::createWithFormat("%d", num)->getCString());
    lbl->setTag(num);
    
    m_countDownHandler = cccontrol_selector(CCBPanelCardSelectNew::onClose);
    
    this->schedule(schedule_selector(CCBPanelCardSelectNew::countDown), 1);
}


CCSprite* CCBPanelCardSelectNew::createCard(const char* pImgUrl, const char* pName, int nNum, int nTag, CCPoint pos, bool bIsOutLight)
{
    CCSprite* pCard = CCSprite::create();
    m_pBg->addChild(pCard);
    pCard->setPosition(pos);
    pCard->setTag(nTag);
    
    CCSprite* pFace = CCSprite::create(bIsOutLight ? "BattleResultNew/card_face_light.png" : "BattleResultNew/card_face.png");
    pCard->addChild(pFace);
    
    CCSprite* pImg = CCSprite::createWithSpriteFrameName(pImgUrl);
    pCard->addChild(pImg);
    pImg->setPosition(ccp(0, 0));
    
    char str[128] = {0};
    sprintf(str, "%sX%d", pName, nNum);
    CCLabelTTF* pNum = CCLabelTTF::create(str, "Arial-BoldMT", 18);
    pCard->addChild(pNum);
    pNum->setPosition(ccp(0, -40));
    pNum->setColor(ccBLACK);
    
    return pCard;
}

void CCBPanelCardSelectNew::flipCard(CCNode* pFace, CCNode* pBack, float fDuration)
{
    pBack->setPosition(pFace->getPosition());
    
    CCScaleTo* pSpaOut = CCScaleTo::create(fDuration / 4, 1.5);
    CCScaleTo* pSpaIn = CCScaleTo::create(fDuration / 4, 1);
    CCDelayTime* pDelay = CCDelayTime::create(fDuration / 4);
    CCOrbitCamera* pFlip = CCOrbitCamera::create(fDuration / 4, 1, 0, 0, -90, 0, 0);
    CCSequence* pSeq = CCSequence::create(pSpaOut, pFlip, CCHide::create(), pDelay, pSpaIn, NULL);
    pFace->runAction(pSeq);
    
    pBack->setVisible(false);
    CCOrbitCamera* pFlip1 = CCOrbitCamera::create(fDuration / 4, 1, 0, -270, -90, 0, 0);
    
    CCSequence* pSeq1 = CCSequence::create(dynamic_cast<CCScaleTo*>(pSpaOut->copy()->autorelease()), dynamic_cast<CCDelayTime*>(pDelay->copy()->autorelease()), CCShow::create(), pFlip1, dynamic_cast<CCScaleTo*>(pSpaIn->copy()->autorelease()), NULL);
    pBack->runAction(pSeq1);
}

void CCBPanelCardSelectNew::onReturn(CCObject* pTarget, cocos2d::extension::CCControlEvent event)
{
    if (m_pDelegate != NULL)
    {
        m_pDelegate->onPanelCardSelectNewReturn();
    }
    this->removeFromParent();
}

void CCBPanelCardSelectNew::onClose(CCObject* pTarget, cocos2d::extension::CCControlEvent event)
{
    if (m_pDelegate != NULL)
    {
        m_pDelegate->onPanelCardSelectNewClose();
    }
    this->removeFromParent();
}

void CCBPanelCardSelectNew::onStart(CCObject* pTarget, cocos2d::extension::CCControlEvent event)
{
    this->unschedule(schedule_selector(CCBPanelCardSelectNew::countDown));
    
    m_pBtnStart->removeFromParent();
    m_pBtnBg->removeFromParent();
    
    for (std::map<int, card_t>::iterator it = m_CardMap.begin(); it != m_CardMap.end(); it++)
    {
        int tag = it->first;
        CCNode* pNode = m_pBg->getChildByTag(tag);
        if (pNode != NULL)
        {
            pNode->removeFromParent();
        }
        CCSprite* pBack = CCSprite::create("BattleResultNew/card_back.png");
        m_pBg->addChild(pBack);
        pBack->setTag(tag - CARD_TAG);
        float duration = 1;
        this->flipCard(m_pBg->getChildByTag(tag + CARD_TAG), pBack, duration);
        pBack->runAction(CCSequence::create(CCDelayTime::create(duration), CCMoveTo::create(0.5, m_pSlot[2]->getPosition()), NULL));
        duration += 1;
        int i = 0;
        do
        {
            i = rand() % 5;
        }
        while (m_PosMap.find(i)->second != 0);
        m_PosMap.find(i)->second = tag - CARD_TAG;
        pBack->runAction(CCSequence::create(CCDelayTime::create(duration), CCMoveTo::create(0.5, m_pSlot[i]->getPosition()), CCCallFuncN::create(this,callfuncN_selector(CCBPanelCardSelectNew::canClick)), NULL));
    }
}

void CCBPanelCardSelectNew::canClick(CCNode* pNode)
{
    m_bCanClick = true;
    
    m_pGold->setVisible(true);
    CCLabelTTF* pTitle = dynamic_cast<CCLabelTTF*>(m_pGold->getParent()->getChildByTag(250));
    pTitle->setVisible(false);
    //pTitle->setString("请抽取一张卡牌，本次抽取免费。");
    m_pGold->setString("");
    
    CCLabelTTF* pDesc = CCLabelTTF::create("", FONTNAME, 32);
    pNode->addChild(pDesc);
    pDesc->setPosition(ccp(pNode->getContentSize().width / 2.0f, pNode->getContentSize().height / 2.0f));
    pDesc->setColor(ccYELLOW);
    pNode->setUserObject(pDesc);
    
    int num = 20;
    
    CCLabelAtlas* lbl = dynamic_cast<CCLabelAtlas*>(m_pCountDown->getChildren()->objectAtIndex(0));
    lbl->setString(CCString::createWithFormat("%d", num)->getCString());
    lbl->setTag(num);
    
    m_countDownHandler = cccontrol_selector(CCBPanelCardSelectNew::onAutoSelectCard);
    
    this->schedule(schedule_selector(CCBPanelCardSelectNew::countDown), 1);
}

void CCBPanelCardSelectNew::onAutoSelectCard(CCObject* pTarget, cocos2d::extension::CCControlEvent event)
{
    for (int i = 0; i < 5; i++)
    {
        cocos2d::extension::CCControlButton* pSlot = this->m_pSlot[i];
        if (m_PosMap.find(dynamic_cast<CCNode*>(pSlot)->getTag() - SLOT_TAG)->second != 0)
        {
            this->onSlotClick(pSlot, 0);
            return;
        }
    }
}
/*
void CCBPanelCardSelectNew::onRemove(CCObject* pTarget, cocos2d::extension::CCControlEvent event)
{
    this->unschedule(schedule_selector(CCBPanelCardSelectNew::countDown));
    int num = 10;
    CCLabelAtlas* lbl = dynamic_cast<CCLabelAtlas*>(m_pCountDown->getChildren()->objectAtIndex(0));
    lbl->setString(CCString::createWithFormat("%d", num)->getCString());
    lbl->setTag(num);
    this->schedule(schedule_selector(CCBPanelCardSelectNew::countDown), 1);
    
    int gold = this->getGold();
    if (gold > UserData::sharedInstance()->getGold())
    {
        //this->unschedule(schedule_selector(CCBPanelCardSelectNew::countDown));
        //GoldGuideMessageBox::showGetGuideLayer("您的黄金不足，无法移除手牌，是否充值？");
        XYTopLayer::getInstance()->ShowTip("您的黄金不足！");
        return;
    }
    UserData::sharedInstance()->addGold(-gold);
    
    CCNode* pNode = dynamic_cast<CCNode*>(pTarget);
    int tag = pNode->getTag();
    pNode->removeFromParent();
    
    CCNode* pCard = m_pBg->getChildByTag(tag + CARD_TAG);
    pCard->removeFromParent();
    
    m_CardMap.erase(tag);
    this->setGold(m_CardMap.size());
    
    if (m_CardMap.size() == 1)
    {
        m_pBg->getChildByTag((*m_CardMap.begin()).first)->removeFromParent();
        m_pGold->setVisible(false);
        m_pGold->getParent()->getChildByTag(250)->setVisible(false);
    }
}
*/
void CCBPanelCardSelectNew::loadLayer()
{
    setPrority(-10);
    for (int i = 0; i < 5; i++)
    {
        m_pSlot[i]->setTouchPriority(getPrority() - 1);
    }
    m_pBtnStart->setTouchPriority(getPrority() - 3);
    m_pBtnBg->setTouchPriority(getPrority() - 2);
    CommonUtility::setBtnState(m_pBtnStart, CCRectMake(10, 22, 140, 50), true);
    m_pBtnClose->setTouchPriority(getPrority() - 3);
    m_pBtnReturn->setTouchPriority(getPrority() - 3);
    m_pBtnRenew->setTouchPriority(getPrority() - 3);
    m_pBtnClose->setVisible(false);
    m_pBtnReturn->setVisible(false);
    m_pBtnRenew->setVisible(false);
    
    m_pGold->setVisible(false);
    m_pGold->getParent()->getChildByTag(250)->setVisible(false);
}

void CCBPanelCardSelectNew::setData(std::vector<std::string>* pImgVec, std::vector<std::string>* pNameVec, std::vector<int>* pNumVec)
{
    for (int i = 0; i < pImgVec->size(); i++)
    {
        card_t t;
        t.pImg = pImgVec->at(i).c_str();
        t.nNum = pNumVec->at(i);
        m_CardMap.insert(std::pair<int, card_t>(i, t));
        m_PosMap.insert(std::pair<int, int>(i, 0));
        
        this->createCard(t.pImg.c_str(), pNameVec->at(i).c_str(), t.nNum, i + CARD_TAG, m_pSlot[i]->getPosition());
        
        m_pSlot[i]->setTag(i + SLOT_TAG);
    }
    this->setGold(m_CardMap.size());
    
    int num = 20;
    CCLabelAtlas* lbl = CCLabelAtlas::create(CCString::createWithFormat("%d", num)->getCString(), "public/lblnum.png", 26, 32, '0');
    m_pCountDown->addChild(lbl);
    lbl->setAnchorPoint(CCPointZero);
    lbl->setPosition(ccp(m_pCountDown->getContentSize().width, 10));
    lbl->setTag(num);
    
    m_countDownHandler = cccontrol_selector(CCBPanelCardSelectNew::onStart);
    
    this->schedule(schedule_selector(CCBPanelCardSelectNew::countDown), 1);
}

void CCBPanelCardSelectNew::countDown(float dt)
{
    CCLabelAtlas* lbl = dynamic_cast<CCLabelAtlas*>(m_pCountDown->getChildren()->objectAtIndex(0));
    int num = lbl->getTag();
    if (num == 0)
    {
        (this->*m_countDownHandler)(NULL, 0);
    }
    else
    {
        num--;
        lbl->setString(CCString::createWithFormat("%d", num)->getCString());
        lbl->setTag(num);
    }
}

int CCBPanelCardSelectNew::getGold()
{
    const char* pNum = m_pGold->getString();
    return atoi(pNum);
}

int CCBPanelCardSelectNew::getTotalGold(int nTotal)
{
    switch (nTotal)
    {
        /*
        case 1:
            return 500 + 100 + 25 + 5;
            break;
        case 2:
            return 100 + 25 + 5;
            break;
        case 3:
            return 25 + 5;
            break;
        case 4:
            return 5;
            break;
        case 5:
            return 0;
            break;
        default:
            return 0;
            break;
         */
        case 1:
            return 500;
            break;
        case 2:
            return 100;
            break;
        case 3:
            return 25;
            break;
        case 4:
            return 5;
            break;
        case 5:
            return 0;
            break;
        default:
            return 0;
            break;
    }
}

void CCBPanelCardSelectNew::setGold(int nTotal)
{
    switch (nTotal)
    {
        case 2:
            m_pGold->setString("500");
            break;
        case 3:
            m_pGold->setString("100");
            break;
        case 4:
            m_pGold->setString("25");
            break;
        case 5:
            m_pGold->setString("5");
            break;
        default:
            m_pGold->setString("0");
            break;
    }
}