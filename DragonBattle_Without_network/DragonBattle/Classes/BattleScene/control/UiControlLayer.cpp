//
//  UiControlLayer.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-5-23.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "UiControlLayer.h"
#include "GlobalDefine.h"
#include "GameManager.h"
#include "BattleMapUnits.h"
USING_NS_CC;

UiControlLayer::UiControlLayer()
{
    m_bShowAttackRect = false;
    m_pShowLabel = NULL;
}

UiControlLayer::~UiControlLayer()
{
    
}

UiControlLayer* UiControlLayer::create()
{
    UiControlLayer* pRet = new UiControlLayer();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool UiControlLayer::init()
{
    if (!CCLayer::init())
    {
        CCLOG("UiControlLayer CCLayer::init() error!");
        return false;
    }
    
    CCLayerColor* pColorBg = CCLayerColor::create(ccc4(128, 128, 128, 128), SCREEN_WIDTH, SCREEN_HEIGTH);
    pColorBg->setPosition(CCPointZero);
    this->addChild(pColorBg);
    
    this->createBtns();
    
    this->createClose();
    
    this->setTouchEnabled(true);
    return true;
}

void UiControlLayer::createBtns()
{
    m_bShowAttackRect = GameManager::sharedInstance()->getBattleLayer()->isAttackRectVisible();
    m_pShowLabel = CCLabelTTF::create("显示攻击区域", "Arial", 40);
    m_pShowLabel->setColor(ccYELLOW);
    if (m_bShowAttackRect)
    {
        m_pShowLabel->setString("隐藏攻击区域");
        m_pShowLabel->setColor(ccBLUE);
    }
    
    CCMenuItemLabel* pBtn1 = CCMenuItemLabel::create(m_pShowLabel, this, menu_selector(UiControlLayer::onShowAttackRect));
    pBtn1->setAnchorPoint(ccp(0, 0));
    pBtn1->setPosition(ccp(40, 500));
    
    CCLabelTTF* pResetGame = CCLabelTTF::create("重新开始", "Arial", 40);
    pResetGame->setColor(ccYELLOW);
    CCMenuItemLabel* pBtn2 = CCMenuItemLabel::create(pResetGame, this, menu_selector(UiControlLayer::onResetGame));
    pBtn2->setAnchorPoint(ccp(0,0));
    pBtn2->setPosition(ccp(40 + pBtn1->getContentSize().width + 80, 500));
    
    CCLabelTTF* pVictory = CCLabelTTF::create("胜利", "Arial", 40);
    pVictory->setColor(ccYELLOW);
    CCMenuItemLabel* pBtn3 = CCMenuItemLabel::create(pVictory, this, menu_selector(UiControlLayer::onVictory));
    pBtn3->setAnchorPoint(ccp(0,0));
    pBtn3->setPosition(ccp(40 + pBtn1->getContentSize().width + 80 + pBtn2->getContentSize().width + 80, 500));
    
    CCLabelTTF* pFailure = CCLabelTTF::create("失败", "Arial", 40);
    pFailure->setColor(ccYELLOW);
    CCMenuItemLabel* pBtn4 = CCMenuItemLabel::create(pFailure, this, menu_selector(UiControlLayer::onFailure));
    pBtn4->setAnchorPoint(ccp(0,0));
    pBtn4->setPosition(ccp(40 + pBtn1->getContentSize().width + 80 + pBtn2->getContentSize().width + 80 + pBtn3->getContentSize().width + 80, 500));
    
    CCLabelTTF* pReturn = CCLabelTTF::create("返回", "Arial", 40);
    pReturn->setColor(ccYELLOW);
    CCMenuItemLabel* pBtn5 = CCMenuItemLabel::create(pReturn, this, menu_selector(UiControlLayer::onReturn));
    pBtn5->setAnchorPoint(ccp(0,0));
    pBtn5->setPosition(ccp(40, 450));
    
    CCMenu* pMenu = CCMenu::create(pBtn1, pBtn2, pBtn3, pBtn4, pBtn5, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu);
}

void UiControlLayer::createClose()
{
    CCLabelTTF* pCloseLabel = CCLabelTTF::create("关闭", "Arial", 40);
    pCloseLabel->setColor(ccBLUE);
    
    CCMenuItemLabel* pClose = CCMenuItemLabel::create(pCloseLabel, this, menu_selector(UiControlLayer::onClose));
    pClose->setAnchorPoint(ccp(1,1));
    pClose->setPosition(ccp(SCREEN_WIDTH - 20, SCREEN_HEIGTH - 20));
    
    CCMenu* pMenu = CCMenu::create(pClose, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu);
}

void UiControlLayer::onShowAttackRect(CCObject *pSender)
{
    m_bShowAttackRect = !m_bShowAttackRect;
    
    if (m_bShowAttackRect)
    {
        m_pShowLabel->setString("隐藏攻击区域");
        m_pShowLabel->setColor(ccBLUE);
    }
    else
    {
        m_pShowLabel->setString("显示攻击区域");
        m_pShowLabel->setColor(ccYELLOW);
    }
    
    ((BattleLayer*)(this->getParent()))->setAttackRectVisible(m_bShowAttackRect);
    BattleMapUnits* pMapLayer = (BattleMapUnits*)GameManager::sharedInstance()->getBattleMapLayer();
    pMapLayer->setAttacRectShow(m_bShowAttackRect);
}

void UiControlLayer::onResetGame(cocos2d::CCObject *pSender)
{
    return ;
    this->removeAllChildrenWithCleanup(true);
    this->removeFromParentAndCleanup(true);
    
    GameManager::sharedInstance()->restartBattle();
}

void UiControlLayer::onVictory(CCObject* pSender)
{
    return;
    uint32_t monster[4];
    for (int i = 0; i < 4; i++)
    {
        monster[i] = i;
    }
    CCNotificationCenter::sharedNotificationCenter()->postNotification(BATTLEWIN_NOTE);
    this->removeFromParent();
}
void UiControlLayer::onFailure(CCObject* pSender)
{
    return ;
    uint32_t monster[4];
    for (int i = 0; i < 4; i++)
    {
        monster[i] = i;
    }
    CCNotificationCenter::sharedNotificationCenter()->postNotification(BATTLELOST_NOTE);
    this->removeFromParent();
}

void UiControlLayer::onReturn(CCObject* pSender)
{
    this->removeAllChildrenWithCleanup(true);
    this->removeFromParentAndCleanup(true);
    return;
    GameManager::sharedInstance()->exitBattleLayer();
}

void UiControlLayer::onClose(CCObject *pSender)
{
    this->removeAllChildrenWithCleanup(true);
    this->removeFromParentAndCleanup(true);
}

// register touch dispatcher
void UiControlLayer::registerWithTouchDispatcher(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);
}

// default implements are used to call script callback if exist
bool UiControlLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}