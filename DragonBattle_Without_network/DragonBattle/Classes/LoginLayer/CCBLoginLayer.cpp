//
//  CCBLoginLayer.cpp
//  DragonBattle
//
//  Created by zeng xiaolong on 13-6-21.
//
//

#include "CCBLoginLayer.h"
#include "XYHttpLogin.h"
#include "UserData.h"
#include "XYToggleButton.h"
#include "LoginController.h"
#include "XYTopLayer.h"
#include "DealErrorCode.h"
#include "AudioResource.h"
#include "CCBStartLayer.h"
#include "XYHttpBridge.h"
#include "XYTopLayer.h"
#include "GameManager.h"


#ifdef TARGET_91VERSION
#include "NWStatistical.h"
#endif


CCBLoginLayer::CCBLoginLayer()
{
    m_pNomalNode = NULL;
    m_pBtnBegin = NULL;
    m_pBtnRegister = NULL;
    m_pTicket = NULL;
    m_pBtnTicket = NULL;
    
    m_pEditBoxUser = NULL;
    m_pEditBoxPwd = NULL;
    
    m_pTencentNode = NULL;
    m_pBtnLoginWithQQ = NULL;
    m_pBtnLoginWithWX = NULL;
    
    m_pLblVersion = NULL;
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CCBLoginLayer::logInError), LOG_IN_ERROR, NULL);
    

}

CCBLoginLayer::~CCBLoginLayer()
{
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    
    CC_SAFE_RELEASE_NULL(m_pNomalNode);
    CC_SAFE_RELEASE_NULL(m_pBtnBegin);
    CC_SAFE_RELEASE_NULL(m_pBtnRegister);
    CC_SAFE_RELEASE_NULL(m_pEditBoxUser);
    CC_SAFE_RELEASE_NULL(m_pEditBoxPwd);
    CC_SAFE_RELEASE_NULL(m_pTicket);
    
    CC_SAFE_RELEASE_NULL(m_pTencentNode);
    CC_SAFE_RELEASE_NULL(m_pBtnLoginWithQQ);
    CC_SAFE_RELEASE_NULL(m_pBtnLoginWithWX);
}

SEL_CCControlHandler CCBLoginLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBegin", CCBLoginLayer::onBegin);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onRegister", CCBLoginLayer::onRegister);
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onLoginWithQQ", CCBLoginLayer::onLoginWithQQ);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onLoginWithWX", CCBLoginLayer::onLoginWithWX);

    return NULL;
}

bool CCBLoginLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "normal_login", CCNode*, m_pNomalNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnBegin", CCControlButton*, m_pBtnBegin);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnRegister", CCControlButton*, m_pBtnRegister);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "Ticket", CCSprite*, m_pTicket);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "edit_box_user", CCEditBox*, m_pEditBoxUser);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "edit_box_pwd", CCEditBox*, m_pEditBoxPwd);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "tencent_login", CCNode*, m_pTencentNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_login_with_qq", CCControlButton*, m_pBtnLoginWithQQ);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_login_with_weixin", CCControlButton*, m_pBtnLoginWithWX);
    
    return false;
}

void CCBLoginLayer::onNodeLoaded(CCNode * pNode, CCNodeLoader * pNodeLoader)
{
    loadLayer();
}

void CCBLoginLayer::loadLayer()
{
    setPrority(-5);
    this->setPosition(ccp((SCREEN_WIDTH - this->getContentSize().width) / 2, (SCREEN_HEIGTH - this->getContentSize().height) / 2));
    
    CCNode* pBack = this->getChildByTag(248);
    CCSize size = pBack->getContentSize();
    float scale = SCREEN_WIDTH / size.width > SCREEN_HEIGTH / size.height ? (SCREEN_WIDTH / size.width) : (SCREEN_HEIGTH / size.height);
    pBack->setScaleX(scale);
    pBack->setScaleY(scale);
    pBack->setPosition(pBack->getParent()->convertToNodeSpace(ccp(SCREEN_WIDTH / 2, SCREEN_HEIGTH / 2)));
    AudioResource::shareAudioResource()->playBgSound("music/taoyuan.mp3");
    m_pLblVersion = CCLabelTTF::create(CCString::createWithFormat("Version: %s", APP_VERSION)->getCString(), FONTNAME, 20);

    this->addChild(m_pLblVersion);
    m_pLblVersion->setPosition(m_pLblVersion->getParent()->convertToNodeSpace(ccp(SCREEN_WIDTH - 200, 50)));

    m_pNomalNode->setVisible(true);
    m_pTencentNode->setVisible(false);

#ifdef TARGET_91VERSION
    m_pNomalNode->setVisible(false);
    m_pTencentNode->setVisible(false);
#endif
    
#ifdef TARGET_TENCENT
    m_pNomalNode->setVisible(false);
    m_pTencentNode->setVisible(true);
    m_pBtnLoginWithQQ->setTouchPriority(getPrority()-1);
    m_pBtnLoginWithWX->setTouchPriority(getPrority()-1);
#endif
    
#ifdef TARGET_SELFVERSION
    
    m_pBtnBegin->setTouchPriority(getPrority()-1);
    m_pBtnRegister->setTouchPriority(getPrority()-1);
    
    m_pEditBoxUser->setTouchPriority(getPrority()-1);
    m_pEditBoxUser->setPlaceHolder("name@example.com");
    m_pEditBoxUser->setReturnType(kKeyboardReturnTypeNext);
    
    m_pEditBoxUser->setFont(FONTNAME, 24.f);
    m_pEditBoxUser->setFontColor(ccWHITE);
    m_pEditBoxUser->setPlaceholderFontColor(ccGRAY);
    
    m_pEditBoxUser->setDelegate(this);
    
    m_pEditBoxPwd->setInputFlag(kEditBoxInputFlagPassword);
    m_pEditBoxPwd->setTouchPriority(getPrority()-1);
    m_pEditBoxPwd->setPlaceHolder("密码最长6-12位");
    m_pEditBoxPwd->setFont(FONTNAME, 24.f);
    m_pEditBoxPwd->setFontColor(ccWHITE);
    m_pEditBoxPwd->setPlaceholderFontColor(ccGRAY);
    
    m_pEditBoxPwd->setReturnType(kKeyboardReturnTypeDone);
    m_pEditBoxPwd->setDelegate(this);
    
    
    m_pBtnTicket = XYToggleButton::create("CCBLoginResource/ticket_bottom.png", "public/ticket_green.png", getPrority()-1);
    m_pTicket->getParent()->addChild(m_pBtnTicket);
    m_pBtnTicket->setPosition(m_pTicket->getPosition());
    
    std::string userName = CCUserDefault::sharedUserDefault()->getStringForKey("UserName", "");
    m_pEditBoxUser->setText(userName.c_str());
    bool isPasswordSave = CCUserDefault::sharedUserDefault()->getBoolForKey("IsPasswordSave", true);
    m_pBtnTicket->setselect(isPasswordSave);
    std::string password = CCUserDefault::sharedUserDefault()->getStringForKey("Password", "");
    m_pEditBoxPwd->setText(password.c_str());
    
    
    CCSprite* decoration_flower = CCSprite::create("CCBLoginResource/flower_decoration.png");
    this->addChild(decoration_flower);
    decoration_flower->setAnchorPoint(ccp(0.5f, 0.5f));
    decoration_flower->setPosition(this->convertToNodeSpace(ccp(SCREEN_WIDTH / 2, SCREEN_HEIGTH + 10)));
    decoration_flower->setScale(SCREEN_WIDTH / decoration_flower->getContentSize().width);
    
#endif
    
    return;
    XYTopLayer::getInstance()->ShowWaiting();
    XYHttpBridge::getInstance()->checkHaveBulletin(this, httpresponse_selector(CCBLoginLayer::checkBulletinOver));
    
}

void CCBLoginLayer::checkBulletinOver(CCNode* pNode, void* pData)
{
    XYTopLayer::getInstance()->RemoveWaiting();
    CCHttpResponse* response = (CCHttpResponse*)pData;
    
    if (response && response->isSucceed())
    {
        CSJson::Value json_object;
        
        std::string strContents;
        strContents.assign(response->getResponseData()->begin(), response->getResponseData()->end());
        CCLOG("%s",strContents.c_str());
        
        CSJson::Reader reader;
        if (reader.parse(strContents, json_object))
        {
            if (json_object["success"].asBool())
            {
                CSJson::Value i = json_object["inform"].get("cnt", 0);
                if (i.asInt() != 0)
                {
                    CSJson::Value j = json_object["inform"]["url"];
                    const char* str = j.asCString();
                    if (strlen(str) != 0)
                    {
                        

                        
                    }
                }
            }
        }
    }
}


void CCBLoginLayer::logInError(CCObject* pTarget)
{
    //char str[128] = {0};
    //XYTopLayer::getInstance()->ShowTip(DealErrorCode::getServerErrorMsg(dynamic_cast<CCString*>(pTarget)->intValue(), str));
}

void CCBLoginLayer::onBegin(CCObject* pTarget, CCControlEvent event)
{
    GameManager::sharedInstance()->enterBattleLayer(TUTORIAL_MISSION);
    
    return;
    if ( *(m_pEditBoxUser->getText()) == '\0'&&*(m_pEditBoxPwd->getText()) == '\0')
    {
        XYTopLayer::getInstance()->ShowTip("请输入帐号、密码！");
        return;
    }
    else if ( *(m_pEditBoxUser->getText()) != '\0'&&*(m_pEditBoxPwd->getText()) == '\0')
    {
        XYTopLayer::getInstance()->ShowTip("请输入密码！");
        return;
    }
    else if ( *(m_pEditBoxUser->getText()) == '\0'&&*(m_pEditBoxPwd->getText()) != '\0')
    {
        XYTopLayer::getInstance()->ShowTip("请输入帐号！");
        return;
    }
    
    CCUserDefault::sharedUserDefault()->setStringForKey("UserName", m_pEditBoxUser->getText());
    CCUserDefault::sharedUserDefault()->setBoolForKey("IsPasswordSave", m_pBtnTicket->getTurnOn());
    
    if (m_pBtnTicket->getTurnOn())
    {
        CCUserDefault::sharedUserDefault()->setStringForKey("Password", m_pEditBoxPwd->getText());
    }
    else
    {
        CCUserDefault::sharedUserDefault()->setStringForKey("Password", "");
    }
    
    XYHttpLogin::getInstance()->login(m_pEditBoxUser->getText(), m_pEditBoxPwd->getText());
}


void CCBLoginLayer::onRegister(CCObject* pTarget, CCControlEvent event)
{

}


void CCBLoginLayer::onLoginWithQQ(cocos2d::CCObject *pTarget, CCControlEvent event)
{
    CCLOG("apple...LoginWithQQ");
}

void CCBLoginLayer::onLoginWithWX(cocos2d::CCObject *pTarget, CCControlEvent event)
{
    CCLOG("apple...LoginWithWX");
}