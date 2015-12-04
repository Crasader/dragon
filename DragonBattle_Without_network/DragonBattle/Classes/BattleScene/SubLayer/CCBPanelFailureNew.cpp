//
//  CCBPanelFailureNew.cpp
//  DragonBattle
//
//  Created by 何 永翔 on 13-9-2.
//
//

#include "CCBPanelFailureNew.h"
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

CCBPanelFailureNew::CCBPanelFailureNew()
{
    m_pCountDown = NULL;
    
    m_pBtnRenew = NULL;
    m_pBtnReturn = NULL;

    m_nMissionType = 1;
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CCBPanelFailureNew::onShowDialog),GET_AND_BUY_ENERY_COUNT, NULL);
}

CCBPanelFailureNew::~CCBPanelFailureNew()
{
    CC_SAFE_RELEASE_NULL(m_pCountDown);
    CC_SAFE_RELEASE_NULL(m_pBtnRenew);
    CC_SAFE_RELEASE_NULL(m_pBtnReturn);

    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}

void CCBPanelFailureNew::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader)
{
    loadLayer();
}

SEL_MenuHandler CCBPanelFailureNew::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool CCBPanelFailureNew::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "CountDown", CCSprite*, this->m_pCountDown);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnRenew", CCControlButton*, this->m_pBtnRenew);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnReturn", CCControlButton*, this->m_pBtnReturn);
    
    return false;
}

SEL_CCControlHandler CCBPanelFailureNew::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onReturn", CCBPanelFailureNew::onReturn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onRenew", CCBPanelFailureNew::onRenew);
    
    return NULL;
}

bool CCBPanelFailureNew::isShowFailureGuide()
{
    if (GameData::checkFuncOpened(func_id_hero, gUserDataEx->getMaxMainTask(), gUserData->m_heroData->getLV()) == eLimit_Type_None)
    {
        return true;
    }
    if (GameData::checkFuncOpened(func_id_strengthen, gUserDataEx->getMaxMainTask(), gUserData->m_heroData->getLV()) == eLimit_Type_None)
    {
        return true;
    }
    if (GameData::checkFuncOpened(func_id_inforce, gUserDataEx->getMaxMainTask(), gUserData->m_heroData->getLV()) == eLimit_Type_None)
    {
        return true;
    }
    if (GameData::checkFuncOpened(func_id_skill, gUserDataEx->getMaxMainTask(), gUserData->m_heroData->getLV()) == eLimit_Type_None)
    {
        return true;
    }
    return false;
}

void CCBPanelFailureNew::onReturn(CCObject* pTarget, cocos2d::extension::CCControlEvent event)
{
    this->unschedule(schedule_selector(CCBPanelFailureNew::countDown));
    
    GameManager::sharedInstance()->replaceScene(COMMUNITY_SCENCE,true);
    switch (m_nMissionType)
    {
        case 1:
            break;
        case 4:     //声望挑战失败
            break;
        default:
            break;
    }
}

void CCBPanelFailureNew::onRenew(CCObject* pTarget, cocos2d::extension::CCControlEvent event)
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

void CCBPanelFailureNew::onShowDialog(cocos2d::CCObject *p)
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
    XYMessageBox * pMsgBox = XYMessageBox::create(E_MB_OKCANCEL, "", msg->getCString(), this, callfuncN_selector(CCBPanelFailureNew::onAddConfirm), callfuncN_selector(CCBPanelFailureNew::onCancel));
    pMsgBox->setMsgAlignment(kCCTextAlignmentCenter);
    pMsgBox->showMessageBox();
}

void CCBPanelFailureNew::onCancel(CCNode* p)
{
    onReturn(NULL,NULL);
}
void CCBPanelFailureNew::onAddConfirm(CCNode* p)
{
    OnlineManager::sharedManager()->userBuyEneryToOnline(cal_buy_energy_cost_gold(gUserData->getBuyEneryCount()));
}

void CCBPanelFailureNew::loadLayer()
{
    setPrority(-10);
    m_pBtnRenew->setTouchPriority(getPrority() - 1);
    m_pBtnReturn->setTouchPriority(getPrority() - 1);
    
    //倒计时，暂时不用咯
    int num = 10;
    CCLabelAtlas* lbl = CCLabelAtlas::create(CCString::createWithFormat("%d", num)->getCString(), "public/lblnum.png", 26, 32, '0');
    m_pCountDown->addChild(lbl);
    lbl->setAnchorPoint(CCPointZero);
    lbl->setPosition(ccp(m_pCountDown->getContentSize().width, 10));
    lbl->setTag(num);
    //this->schedule(schedule_selector(CCBPanelFailureNew::countDown), 1);
    m_pCountDown->setVisible(false);
}

void CCBPanelFailureNew::countDown(float dt)
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

void CCBPanelFailureNew::shakeSelf()
{
    CCShake* pAction = CCShake::create(0.1, 5);
    this->runAction(pAction);
}