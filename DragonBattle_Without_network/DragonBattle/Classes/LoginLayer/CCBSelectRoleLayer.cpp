//
//  CCBSelectRoleLayer.cpp
//  DragonBattle
//
//  Created by zeng xiaolong on 13-6-22.
//
//

#include "CCBSelectRoleLayer.h"
#include "XYHttpLogin.h"
#include "UserData.h"
#include "LoginController.h"
#include "GameManager.h"
#include "LogicManager.h"
#include "OnlineManager.h"

CCBSelectRoleLayer::CCBSelectRoleLayer()
{
    BtnBack = NULL;
    //BtnCreate = NULL;
    BtnBegin = NULL;
    
    m_pRoleType = NULL;
    m_pRole = NULL;
    
    m_pLv = NULL;
    m_pName = NULL;
    m_pServerName = NULL;
}

CCBSelectRoleLayer::~CCBSelectRoleLayer()
{
    CC_SAFE_RELEASE_NULL(BtnBack);
    //CC_SAFE_RELEASE_NULL(BtnCreate);
    CC_SAFE_RELEASE_NULL(BtnBegin);
    
    CC_SAFE_RELEASE_NULL(m_pRoleType);
    CC_SAFE_RELEASE_NULL(m_pRole);
    
    CC_SAFE_RELEASE_NULL(m_pLv);
    CC_SAFE_RELEASE_NULL(m_pName);
    CC_SAFE_RELEASE_NULL(m_pServerName);
}

SEL_CCControlHandler CCBSelectRoleLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCreate", CCBSelectRoleLayer::onCreate);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBack", CCBSelectRoleLayer::onBack);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBegin", CCBSelectRoleLayer::onBegin);
    return NULL;
}

bool CCBSelectRoleLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnCreate", CCControlButton*, BtnCreate);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnBack", CCControlButton*, BtnBack);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnBegin", CCControlButton*, BtnBegin);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "RoleType", CCSprite*, m_pRoleType);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "Role", CCSprite*, m_pRole);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "LblLv", CCLabelTTF*, m_pLv);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "LblName", CCLabelTTF*, m_pName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "LblServerName", CCLabelTTF*, m_pServerName);
    
    return false;
}

void CCBSelectRoleLayer::onNodeLoaded(CCNode * pNode, CCNodeLoader * pNodeLoader)
{
    loadLayer();
}

void CCBSelectRoleLayer::loadLayer()
{
    this->setPosition(ccp((SCREEN_WIDTH - this->getContentSize().width) / 2, (SCREEN_HEIGTH - this->getContentSize().height) / 2));
    
    CCNode* pBack = this->getChildByTag(248);
    CCSize size = pBack->getContentSize();
    float scale = SCREEN_WIDTH / size.width > SCREEN_HEIGTH / size.height ? (SCREEN_WIDTH / size.width) : (SCREEN_HEIGTH / size.height);
    pBack->setScaleX(scale);
    pBack->setScaleY(scale);
    pBack->setPosition(pBack->getParent()->convertToNodeSpace(ccp(SCREEN_WIDTH / 2, SCREEN_HEIGTH / 2)));
    
    BtnBack->setPosition(BtnBack->getParent()->convertToNodeSpace(ccp(SCREEN_WIDTH - 150, SCREEN_HEIGTH - 50)));
    
    CCNode* pPlatfrom = this->getChildByTag(252);
    float deltaY = pPlatfrom->getPosition().y - this->convertToNodeSpace(CCPointZero).y;
    pPlatfrom->setPosition(ccpAdd(pPlatfrom->getPosition(), ccp(0, -deltaY)));
    m_pRole->setPosition(ccpAdd(m_pRole->getPosition(), ccp(0, -deltaY)));
    
    setPrority(-45);
    BtnBack->setTouchPriority(getPrority()-1);
    //BtnCreate->setTouchPriority(getPrority()-1);
    BtnBegin->setTouchPriority(getPrority()-1);
}

void CCBSelectRoleLayer::setUser(int nRoleType, int nLv, const char* pName)
{
    CCSpriteFrame* pFrame = NULL;
    CCSpriteFrame* pFrame1 = NULL;
    CCSize size = m_pRole->getContentSize();
    if (nRoleType == 1)
    {
        //m_pName->setColor(ccBLUE);
        pFrame = CCSpriteFrame::create("CCBLoginResource/role_flower_normal.png", CCRect(0, 0, size.width, size.height));
        pFrame1 = CCSpriteFrame::create("CCBLoginResource/label_flower.png", CCRectMake(0, 0, m_pRoleType->getContentSize().width, m_pRoleType->getContentSize().height));
    }
    else if (nRoleType == 2)
    {
        //m_pName->setColor(ccYELLOW);
        pFrame = CCSpriteFrame::create("CCBLoginResource/role_sword_normal.png", CCRect(0, 0, size.width, size.height));
        pFrame1 = CCSpriteFrame::create("CCBLoginResource/label_sword.png", CCRectMake(0, 0, m_pRoleType->getContentSize().width, m_pRoleType->getContentSize().height));
    }
    else if (nRoleType == 3)
    {
        //m_pName->setColor(ccc3(154, 149, 255));
        pFrame = CCSpriteFrame::create("CCBLoginResource/role_spear_normal.png", CCRect(0, 0, size.width, size.height));
        pFrame1 = CCSpriteFrame::create("CCBLoginResource/label_spear.png", CCRectMake(0, 0, m_pRoleType->getContentSize().width, m_pRoleType->getContentSize().height));
    }
    m_pRole->setTexture(pFrame->getTexture());
    m_pRoleType->setTexture(pFrame1->getTexture());
    m_pLv->setString(CCString::createWithFormat("LV:%d", nLv)->getCString());
    m_pName->setString(pName);
    m_pServerName->setString(CCUserDefault::sharedUserDefault()->getStringForKey("LastLogin", "").c_str());
}

void CCBSelectRoleLayer::onCreate(CCObject* pTarget, CCControlEvent event)
{
    LoginController::sharedInstance()->pushLayer(CREATEROLE);
}
void CCBSelectRoleLayer::onBack(CCObject* pTarget, CCControlEvent event)
{
    LoginController::sharedInstance()->popLayer();
}

void CCBSelectRoleLayer::onBegin(CCObject* pTarget, CCControlEvent event)
{
    XYTopLayer::getInstance()->ShowWaiting();
    LogicManager::shareLogicManager()->startNetWork(XYHttpLogin::getInstance()->Ip.c_str(), XYHttpLogin::getInstance()->Port);
}

