//
//  CCBSelectServerLayer.cpp
//  DragonBattle
//
//  Created by zeng xiaolong on 13-6-22.
//
//

#include "CCBSelectServerLayer.h"
#include "XYContainLayer.h"
#include "XYSelectButton.h"
#include "XYHttpLogin.h"
#include "LoginController.h"
#include "HRichLabelTTF.h"
#include "LogicManager.h"

CCBSelectServerLayer::CCBSelectServerLayer()
{
    m_nIndex = -1;
    m_group = XYGroup::create();
    CC_SAFE_RETAIN(m_group);
    m_group->setTouchEndEvent(this, callfuncN_selector(CCBSelectServerLayer::onSelectServer));
    
    BtnEnter = NULL;
    m_pBtnBack = NULL;
    
    m_pLastLogin = NULL;
    m_pNodeParticle = NULL;
}

CCBSelectServerLayer::~CCBSelectServerLayer()
{
    CC_SAFE_RELEASE_NULL(BtnEnter);
    CC_SAFE_RELEASE_NULL(m_pBtnBack);
    CC_SAFE_RELEASE_NULL(m_group);
    
    CC_SAFE_RELEASE_NULL(m_pLastLogin);
    CC_SAFE_RELEASE_NULL(m_pNodeParticle);
}

SEL_CCControlHandler CCBSelectServerLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "OnEnter", CCBSelectServerLayer::onEnter);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBack", CCBSelectServerLayer::onBack);
    return NULL;
}

bool CCBSelectServerLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnEnter", CCControlButton*, BtnEnter);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnBack", CCControlButton*, m_pBtnBack);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_particle", CCNode*, m_pNodeParticle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "LastLogin", CCSprite*, m_pLastLogin);
    
    return false;
}

void CCBSelectServerLayer::onNodeLoaded(CCNode * pNode, CCNodeLoader * pNodeLoader)
{
    loadLayer();
}

void CCBSelectServerLayer::loadLayer()
{
    setPrority(-15);
    this->setPosition(ccp((SCREEN_WIDTH - this->getContentSize().width) / 2, (SCREEN_HEIGTH - this->getContentSize().height) / 2));
    
    CCParticleSystemQuad* particle = CCParticleSystemQuad::create("effect/fire_2.plist");
    m_pNodeParticle->addChild(particle);
    particle = CCParticleSystemQuad::create("effect/fire_1.plist");
    m_pNodeParticle->addChild(particle);
    
    CCNode* pBack = this->getChildByTag(248);
    CCSize size = pBack->getContentSize();
    float scale = SCREEN_WIDTH / size.width > SCREEN_HEIGTH / size.height ? (SCREEN_WIDTH / size.width) : (SCREEN_HEIGTH / size.height);
    pBack->setScaleX(scale);
    pBack->setScaleY(scale);
    pBack->setPosition(pBack->getParent()->convertToNodeSpace(ccp(SCREEN_WIDTH / 2, SCREEN_HEIGTH / 2)));
    
    m_pBtnBack->setPosition(m_pBtnBack->getParent()->convertToNodeSpace(ccp(SCREEN_WIDTH - 150, SCREEN_HEIGTH - 50)));
    
    CCNode* pPlatfrom = this->getChildByTag(252);
    float deltaY = pPlatfrom->getPosition().y - this->convertToNodeSpace(CCPointZero).y;
    pPlatfrom->setPosition(ccpAdd(pPlatfrom->getPosition(), ccp(0, -deltaY)));
    BtnEnter->setTouchPriority(getPrority()-5);
    m_pBtnBack->setTouchPriority(getPrority()-1);

    m_Scrollserver = XYScrollView::create(getPrority()-3, getChildByTag(102)->getContentSize());
    m_Scrollserver->setPosition(getChildByTag(102)->getPosition());
    m_Scrollserver->setDirection(kCCScrollViewDirectionVertical);
    this->addChild(m_Scrollserver);
    
    setServerList(LoginController::sharedInstance()->getSverList(), LoginController::sharedInstance()->getLastedSver());
}

XYSelectButton* CCBSelectServerLayer::createServerButton(CCDictionary *serverinfo, bool isLasted/* = false*/)
{
    CCString* pName = (CCString*)serverinfo->objectForKey("zone_name");
    int nServerState = serverinfo->valueForKey("zone_state")->intValue();
    if (isLasted) {
        nServerState = kServerStateNormal;
    }
    
    int role_type = serverinfo->valueForKey("role_type")->intValue();
    int role_level = serverinfo->valueForKey("role_level")->intValue();
    
    XYSelectButton* btn = XYSelectButton::create("CCBLoginResource/button_server_normal.png", "CCBLoginResource/button_server_select.png", getPrority() - 1);
    btn->setLbl(pName->getCString(), "Arial-BoldMT", 22);
    
    if (role_level != 0 && role_type != 0)
    {
        char str[64] = {0};
        sprintf(str, "headicon/headicon_%d.png", role_type);
        CCSprite* headIcon = CCSprite::create(str);
        btn->addChild(headIcon);
        headIcon->setScale(0.4f);
        headIcon->setPosition(ccp(btn->getContentSize().width - 35, btn->getContentSize().height / 2 + 7));
        
        sprintf(str, "LV%d", role_level);
        HRichLabelTTF* lblLevel = HRichLabelTTF::create(str, FONTNAME, 18.f);
        btn->addChild(lblLevel);
        lblLevel->setColor(ccYELLOW);
        lblLevel->setStroke(FONT_COLOR_BROWN_DARK);
        lblLevel->setPosition(ccp(btn->getContentSize().width - 35, 18));
    }
    
    if (nServerState == kServerStateRecommend)
    {
        CCSprite* pFlag = CCSprite::create("CCBLoginResource/icon_recommend.png");
        btn->addChild(pFlag);
        pFlag->setPosition(ccp(30, btn->getContentSize().height / 2 - 5));
    }
    else if (nServerState == kServerStateBusy)
    {
        CCSprite* pFlag = CCSprite::create("CCBLoginResource/icon_busy.png");
        btn->addChild(pFlag);
        pFlag->setPosition(ccp(30, btn->getContentSize().height / 2 - 5));
    }
    
    return btn;
}

void CCBSelectServerLayer::setServerList(CCArray*arr, int nLastLoginId)
{
    float height = m_Scrollserver->getVisableRect().size.height > (arr->count() + 2) / 3 * 60?m_Scrollserver->getVisableRect().size.height:(arr->count() + 2) / 3 * 60;
    XYContainLayer *color =XYContainLayer::create(getPrority()-4);
    color->setContentSize(CCSize(m_Scrollserver->getVisableRect().size.width,height));
    m_Scrollserver->setContentSize(CCSize(m_Scrollserver->getContentSize().width,height));
    
    CCString* pRecommendName = NULL;
    int recommendIndex = 0;
    CCString* pLastLoginName = NULL;
    int lastLoginIndex = -1;
    for (int i = 0; i < arr->count(); i++)
    {
        CCDictionary *serverinfo = (CCDictionary*)arr->objectAtIndex(i);
        CCString *strname = (CCString*)serverinfo->objectForKey("zone_name");
        
        XYSelectButton *btn = this->createServerButton(serverinfo);
        
        
        btn->setAnchorPoint(ccp(0, 0.5));
        btn->setPosition(ccp(i % 3 * (286 - 20), height - (i / 3) * 60 - 30));
        btn->setTag(i);
        color->addChild(btn);
        m_group->addBtn(btn);
        
        if (pRecommendName == NULL && true)
        {
            pRecommendName = strname;
            recommendIndex = i;
        }
        CCString *strid = (CCString*)serverinfo->objectForKey("zone_id");
        if (nLastLoginId == strid->intValue())
        {
            pLastLoginName = strname;
            lastLoginIndex = i;
        }
    }
    m_Scrollserver->setContainer(color);
    m_Scrollserver->setContentOffset(ccp(0, -(height - m_Scrollserver->getVisableRect().size.height)));
    
    if (0 == nLastLoginId || lastLoginIndex == -1)
    {
        m_pLastLogin->removeFromParent();
        m_group->setSelectButtonByIndex(0);
        m_group->getSelectBtn()->touchEnd();
        m_nIndex = 0;
    }
    else
    {
        XYSelectButton* pLastLogin = this->createServerButton(LoginController::sharedInstance()->getLastSvrInfo(), true);
        this->addChild(pLastLogin);
        pLastLogin->setPosition(m_pLastLogin->getPosition());
        pLastLogin->setTag(lastLoginIndex);
        m_group->addBtn(pLastLogin);
        pLastLogin->touchEnd();
        
        m_nIndex = lastLoginIndex;
    }

}

void CCBSelectServerLayer::onSelectServer(CCNode*pSender)
{
    m_nIndex = pSender->getTag();
    CCLog("%d",m_nIndex);
}

void CCBSelectServerLayer::onEnter(CCObject* pTarget, CCControlEvent event)
{
    if (m_nIndex == -1)
    {
        CCMessageBox("选择服务器", NULL);
        return;
    }
    
    CCDictionary *serverinfo = (CCDictionary *)LoginController::sharedInstance()->getSverList()->objectAtIndex(m_nIndex);
    CCString *strID = (CCString*)serverinfo->objectForKey("zone_id");
    CCString *strname = (CCString*)serverinfo->objectForKey("zone_name");
    CCUserDefault::sharedUserDefault()->setStringForKey("LastLogin", strname->getCString());
    CCString *strIp = (CCString*)serverinfo->objectForKey("zone_ip");
    CCString *strPort = (CCString*)serverinfo->objectForKey("zone_port"); 
    XYHttpLogin::getInstance()->selectServer(strID->intValue(), strIp->getCString(), strPort->intValue());
}

void CCBSelectServerLayer::onBack(CCObject* pTarget, CCControlEvent event)
{
    LoginController::sharedInstance()->popLayer();
}
