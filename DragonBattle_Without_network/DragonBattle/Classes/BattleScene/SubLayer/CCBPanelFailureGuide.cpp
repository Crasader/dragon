//
//  CCBPanelFailureGuide.cpp
//  DragonBattle
//
//  Created by 何 永翔 on 14-3-6.
//
//

#include "CCBPanelFailureGuide.h"
#include "CCBDefine.h"
#include "OnlineManager.h"
#include "CCShake.h"
#include "GameManager.h"
#include "XYMessageBox.h"
#include "XYLinkLabel.h"
#include "lib_formula.h"
using namespace LIB_FORMULA;
USING_NS_CC_EXT;
using namespace std;

CCBPanelFailureGuide::CCBPanelFailureGuide()
{
    m_pBtnClose = NULL;
    m_pBtnPartner = NULL;
    m_pBtnStrengthen = NULL;
    m_pBtnInforce = NULL;
    m_pBtnSkill = NULL;
    m_nMissionType = 1;
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CCBPanelFailureGuide::onShowDialog),GET_AND_BUY_ENERY_COUNT, NULL);
}

CCBPanelFailureGuide::~CCBPanelFailureGuide()
{
    CC_SAFE_RELEASE_NULL(m_pBtnClose);
    CC_SAFE_RELEASE_NULL(m_pBtnPartner);
    CC_SAFE_RELEASE_NULL(m_pBtnStrengthen);
    CC_SAFE_RELEASE_NULL(m_pBtnInforce);
    CC_SAFE_RELEASE_NULL(m_pBtnSkill);
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}

void CCBPanelFailureGuide::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader)
{
    loadLayer();
}

SEL_MenuHandler CCBPanelFailureGuide::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool CCBPanelFailureGuide::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnClose", CCControlButton*, this->m_pBtnClose);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnPartner", CCControlButton*, this->m_pBtnPartner);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnStrengthen", CCControlButton*, this->m_pBtnStrengthen);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnInforce", CCControlButton*, this->m_pBtnInforce);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnSkill", CCControlButton*, this->m_pBtnSkill);
    
    return false;
}

SEL_CCControlHandler CCBPanelFailureGuide::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClose", CCBPanelFailureGuide::onClose);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onPartner", CCBPanelFailureGuide::onPartner);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onStrengthen", CCBPanelFailureGuide::onStrengthen);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onInforce", CCBPanelFailureGuide::onInforce);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSkill", CCBPanelFailureGuide::onSkill);
    
    return NULL;
}

void CCBPanelFailureGuide::onClose(CCObject* pTarget, cocos2d::extension::CCControlEvent event)
{
    this->removeFromParent();
}

void CCBPanelFailureGuide::onPartner(CCObject* pTarget, cocos2d::extension::CCControlEvent event)
{
}
void CCBPanelFailureGuide::onStrengthen(CCObject* pTarget, cocos2d::extension::CCControlEvent event)
{
}
void CCBPanelFailureGuide::onInforce(CCObject* pTarget, cocos2d::extension::CCControlEvent event)
{
}
void CCBPanelFailureGuide::onSkill(CCObject* pTarget, cocos2d::extension::CCControlEvent event)
{
}

void CCBPanelFailureGuide::onShowDialog(cocos2d::CCObject *p)
{
    CCString* count = dynamic_cast<CCString*>(p);
    if (count == NULL)
    {
        return;
    }
    
    if(GameData::getVipFunction(gUserData->getVipLevel(), DAYBUYENERY) < gUserData->getBuyEneryCount())
    {
        XYTopLayer::getInstance()->ShowTip("购买次数已满，升级VIP可增加购买次数！");
        return;
    }
    
    if (gUserData->getGold() <cal_buy_energy_cost_gold(gUserData->getBuyEneryCount()))
    {
        GoldGuideMessageBox::showGetGuideLayer("您的黄金不足，无法购买体力，是否充值？");
        return;
    }
    CCString* msg = CCString::createWithFormat("是否消耗%d黄金购买25点体力?",cal_buy_energy_cost_gold(gUserData->getBuyEneryCount()));
    XYMessageBox * pMsgBox = XYMessageBox::create(E_MB_OKCANCEL, "", msg->getCString(), this, callfuncN_selector(CCBPanelFailureGuide::onAddConfirm), callfuncN_selector(CCBPanelFailureGuide::onCancel));
    pMsgBox->setMsgAlignment(kCCTextAlignmentCenter);
    pMsgBox->showMessageBox();
}

void CCBPanelFailureGuide::onCancel(CCNode* p)
{
    onClose(NULL,NULL);
}
void CCBPanelFailureGuide::onAddConfirm(CCNode* p)
{
    OnlineManager::sharedManager()->userBuyEneryToOnline(cal_buy_energy_cost_gold(gUserData->getBuyEneryCount()));
}

void CCBPanelFailureGuide::createLinkLabel(CCNode* pNode)
{
    XYLinkLabel* pTip = XYLinkLabel::create("点击前往", FONTNAME, 20);
    m_pBtnStrengthen->getParent()->addChild(pTip);
    pTip->setPosition(ccpAdd(pNode->getPosition(), ccp(0, -70)));
    pTip->setColor(ccYELLOW);
    pTip->enableStroke(ccYELLOW, 1);
    pNode->setUserObject(pTip);
}

void CCBPanelFailureGuide::loadLayer()
{
    setPrority(-10);
    m_pBtnClose->setTouchPriority(getPrority() - 1);
    m_pBtnPartner->setTouchPriority(getPrority() - 1);
    m_pBtnStrengthen->setTouchPriority(getPrority() - 1);
    m_pBtnInforce->setTouchPriority(getPrority() - 1);
    m_pBtnSkill->setTouchPriority(getPrority() - 1);
    
    CCSequence* pSeq = CCSequence::create(CCScaleTo::create(1, 1.2f), CCScaleTo::create(1, 1), NULL);
    CCRepeatForever* pRepeat = CCRepeatForever::create(pSeq);
    m_pBtnStrengthen->getParent()->getChildByTag(1)->runAction(pRepeat);
    m_pBtnStrengthen->getParent()->getChildByTag(3)->runAction(dynamic_cast<CCRepeatForever*>(pRepeat->copy()->autorelease()));
    m_pBtnStrengthen->getParent()->getChildByTag(5)->runAction(dynamic_cast<CCRepeatForever*>(pRepeat->copy()->autorelease()));
    m_pBtnStrengthen->getParent()->getChildByTag(7)->runAction(dynamic_cast<CCRepeatForever*>(pRepeat->copy()->autorelease()));
    
    this->createLinkLabel(m_pBtnStrengthen->getParent()->getChildByTag(1));
    this->createLinkLabel(m_pBtnStrengthen->getParent()->getChildByTag(3));
    this->createLinkLabel(m_pBtnStrengthen->getParent()->getChildByTag(5));
    this->createLinkLabel(m_pBtnStrengthen->getParent()->getChildByTag(7));
    
    CCArray* pArray = CCArray::create();
    if (GameData::checkFuncOpened(func_id_hero, gUserDataEx->getMaxMainTask(), gUserData->m_heroData->getLV()) != eLimit_Type_None)
    {
        m_pBtnPartner->setVisible(false);
        m_pBtnPartner->getParent()->getChildByTag(1)->setVisible(false);
        dynamic_cast<CCNode*>(m_pBtnPartner->getParent()->getChildByTag(1)->getUserObject())->setVisible(false);
    }
    else
    {
        pArray->addObject(m_pBtnPartner);
    }

    
    {
        pArray->addObject(m_pBtnSkill);
    }
    
    this->rearrangePosition(pArray);
}

void CCBPanelFailureGuide::rearrangePosition(CCArray* pArray)
{
    int count = pArray->count();
    CCPoint pos = ccp(376, 100);
    CCPoint delta = ccp(160, 0);
    if (count % 2 == 0)//偶数
    {
        pos = ccpAdd(pos, ccpMult(delta, -(count / 2.0f - 0.5f)));
    }
    else//奇数
    {
        pos = ccpAdd(pos, ccpMult(delta, -(count / 2)));
    }
    for (int i = 0; i < count; i++)
    {
        if (pArray->objectAtIndex(i) == m_pBtnPartner)
        {
            m_pBtnPartner->setPosition(pos);
            m_pBtnPartner->getParent()->getChildByTag(1)->setPosition(pos);
            dynamic_cast<CCNode*>(m_pBtnPartner->getParent()->getChildByTag(1)->getUserObject())->setPosition(ccpAdd(pos, ccp(0, -70)));
        }
        else if (pArray->objectAtIndex(i) == m_pBtnStrengthen)
        {
            m_pBtnStrengthen->setPosition(pos);
            m_pBtnStrengthen->getParent()->getChildByTag(3)->setPosition(pos);
            dynamic_cast<CCNode*>(m_pBtnStrengthen->getParent()->getChildByTag(3)->getUserObject())->setPosition(ccpAdd(pos, ccp(0, -70)));
        }
        else if (pArray->objectAtIndex(i) == m_pBtnInforce)
        {
            m_pBtnInforce->setPosition(pos);
            m_pBtnInforce->getParent()->getChildByTag(7)->setPosition(pos);
            dynamic_cast<CCNode*>(m_pBtnInforce->getParent()->getChildByTag(7)->getUserObject())->setPosition(ccpAdd(pos, ccp(0, -70)));
        }
        else if (pArray->objectAtIndex(i) == m_pBtnSkill)
        {
            m_pBtnSkill->setPosition(pos);
            m_pBtnSkill->getParent()->getChildByTag(5)->setPosition(pos);
            dynamic_cast<CCNode*>(m_pBtnSkill->getParent()->getChildByTag(5)->getUserObject())->setPosition(ccpAdd(pos, ccp(0, -70)));
        }
        pos = ccpAdd(pos, delta);
    }
    
}