//
//  CCBStartLayer.cpp
//  DragonBattle
//
//  Created by 何 永翔 on 13-10-18.
//
//

#include "CCBStartLayer.h"
#include "XYHttpLogin.h"
#include "XYLblButton.h"
#include "LoginController.h"
#include "AudioResource.h"
#include "LogicManager.h"
#include "EffectResource.h"


CCBStartLayer::CCBStartLayer()
{
    m_pBtnStart = NULL;
    m_pBtnBack = NULL;
    m_pSprLastedSver = NULL;
    m_pSprGameIcon = NULL;
    
    m_pLayerTransit = NULL;
    
    m_nickName = "";
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CCBStartLayer::startGameCbk), "START_GAME_NOTE", NULL);
}

CCBStartLayer::~CCBStartLayer()
{
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    CC_SAFE_RELEASE_NULL(m_pBtnStart);
    CC_SAFE_RELEASE_NULL(m_pBtnBack);
    CC_SAFE_RELEASE_NULL(m_pSprLastedSver);
    CC_SAFE_RELEASE_NULL(m_pSprGameIcon);
    
    m_pLayerTransit = NULL;
}

SEL_CCControlHandler CCBStartLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onStart", CCBStartLayer::onStart);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBack", CCBStartLayer::onBack);
    return NULL;
}

bool CCBStartLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnStart", CCControlButton*, m_pBtnStart);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnBack", CCControlButton*, m_pBtnBack);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "spr_lasted_sver", CCSprite*, m_pSprLastedSver);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "spr_icon", CCSprite*, m_pSprGameIcon);
    
    return false;
}

void CCBStartLayer::onNodeLoaded(CCNode * pNode, CCNodeLoader * pNodeLoader)
{
    loadLayer();
}

void CCBStartLayer::loadLayer()
{
    this->setPosition(ccp((SCREEN_WIDTH - this->getContentSize().width) / 2, (SCREEN_HEIGTH - this->getContentSize().height) / 2));
    this->setContentSize(ccp(SCREEN_WIDTH, SCREEN_HEIGTH));
    
    setPrority(-10);
    m_pBtnStart->setTouchPriority(getPrority() - 1);
    m_pBtnBack->setTouchPriority(getPrority() - 1);
    
    CCSprite* pFlash = EffectResource::sharedInstance()->spriteWithEffect(35, true, true, 2);
    m_pBtnStart->addChild(pFlash);
    pFlash->setPosition(ccp(m_pBtnStart->getContentSize().width / 2.0f, m_pBtnStart->getContentSize().height / 2.0f));
    
    
    CCNode* pBack = this->getChildByTag(248);
    CCSize size = pBack->getContentSize();
    float scale = SCREEN_WIDTH / size.width > SCREEN_HEIGTH / size.height ? (SCREEN_WIDTH / size.width) : (SCREEN_HEIGTH / size.height);
    pBack->setScaleX(scale);
    pBack->setScaleY(scale);
    pBack->setPosition(pBack->getParent()->convertToNodeSpace(ccp(SCREEN_WIDTH / 2, SCREEN_HEIGTH / 2)));
    
    CCSprite* decoration_flower = CCSprite::create("CCBLoginResource/flower_decoration.png");
    this->addChild(decoration_flower);
    decoration_flower->setAnchorPoint(ccp(0.5f, 0.5f));
    decoration_flower->setPosition(this->convertToNodeSpace(ccp(SCREEN_WIDTH / 2, SCREEN_HEIGTH + 10)));
    decoration_flower->setScale(SCREEN_WIDTH / decoration_flower->getContentSize().width);
    
    
    m_pLayerTransit = CCLayerColor::create(ccc4(0, 0, 0, 0));
    m_pLayerTransit->setContentSize(CCSizeMake(SCREEN_WIDTH, SCREEN_HEIGTH));
    this->addChild(m_pLayerTransit);
    m_pLayerTransit->setAnchorPoint(CCPointZero);
    m_pLayerTransit->setPosition(this->convertToNodeSpace(CCPointZero));
    
    CCString* pLastName = NULL;
    if (LoginController::sharedInstance()->getLastSvrInfo())
    {
        CCString* pName = (CCString*)LoginController::sharedInstance()->getLastSvrInfo()->objectForKey("zone_name");
        pLastName = CCString::createWithFormat("%s  点击选服", pName->getCString());
    }

    XYLblButton* lastSvrBtn = XYLblButton::create(pLastName != NULL? pLastName->getCString() : "点击选择新服", FONTNAME, 24.f, getPrority() - 1);;
    m_pSprLastedSver->addChild(lastSvrBtn);
    lastSvrBtn->setAnchorPoint(ccp(0.5f, 0.5f));
    lastSvrBtn->setPosition(ccp(m_pSprLastedSver->getContentSize().width / 2, m_pSprLastedSver->getContentSize().height / 2));
    lastSvrBtn->setColor(ccYELLOW);
    lastSvrBtn->setLine(true);
    
    lastSvrBtn->setTouchEndEvent(this, callfuncN_selector(CCBStartLayer::onSelectSvr));
}

void CCBStartLayer::onStart(CCObject* pTarget, CCControlEvent event)
{
    char strmusic [64]= {0};
    sprintf(strmusic, "music/start_click.mp3");
    AudioResource::shareAudioResource()->playEffect(strmusic);
    if (LoginController::sharedInstance()->getLastedSver() <=0)
    {
        LoginController::sharedInstance()->pushLayer(SELECTSERVER);
    }
    else
    {
        startGame();
    }
}

void CCBStartLayer::startNetWork()
{
    XYTopLayer::getInstance()->ShowWaiting();
    LogicManager::shareLogicManager()->startNetWork(XYHttpLogin::getInstance()->Ip.c_str(), XYHttpLogin::getInstance()->Port);
}

void CCBStartLayer::startGameCbk(CCObject*)
{
    if (m_pLayerTransit && this->getParent())
    {
        m_pLayerTransit->runAction(CCSequence::create(CCDelayTime::create(0.6f),
                                                      CCFadeTo::create(0.4f, 255),
                                                      CCDelayTime::create(0.1f),
                                                      CCCallFunc::create(this, callfunc_selector(CCBStartLayer::startNetWork)),
                                                      NULL));
        m_pSprGameIcon->setOpacity(255 * 60.f);
        m_pSprGameIcon->runAction(CCSpawn::create(
                                                  CCFadeTo::create(1.f, 30),
                                                  CCScaleTo::create(1.f, 1.2f),
                                                  NULL));
    }
    else
    {
        startNetWork();
    }
}

void CCBStartLayer::startGame()
{
    CCDictionary *serverinfo = LoginController::sharedInstance()->getLastSvrInfo();
    
    if (serverinfo)
    {
        CCString *strID = (CCString*)serverinfo->objectForKey("zone_id");
        CCString *strname = (CCString*)serverinfo->objectForKey("zone_name");
        CCUserDefault::sharedUserDefault()->setStringForKey("LastLogin", strname->getCString());
        CCString *strIp = (CCString*)serverinfo->objectForKey("zone_ip");
        CCString *strPort = (CCString*)serverinfo->objectForKey("zone_port");
        XYHttpLogin::getInstance()->selectServer(strID->intValue(), strIp->getCString(), strPort->intValue());
    }
    else
    {
        CCMessageBox("请选择服务器开始游戏！", "提示");
    }
    
}

void CCBStartLayer::onBack(CCObject* pTarget, CCControlEvent event)
{
    LoginController::sharedInstance()->popLayer();
}

void CCBStartLayer::onSelectSvr(CCNode*)
{
    LoginController::sharedInstance()->pushLayer(SELECTSERVER);
}

