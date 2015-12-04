//
//  XYMessageBox.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-8-29.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "XYMessageBox.h"
#include "GlobalDefine.h"
#include "XYToggleButton.h"
#include "UserData.h"
#include "CommonUtility.h"
#include "OnlineManager.h"

BaseMessageBox::BaseMessageBox()
{
    m_pBkgSprite = NULL;
    m_pLabelTitle = NULL;
    m_pLabelMsg = NULL;
    m_pBtn1 = NULL;
    m_pBtn2 = NULL;
    
    m_pTarget = NULL;
    m_pSelFunc1 = NULL;
    m_pSelFunc2 = NULL;
    m_pColorMask = NULL;
    
}

BaseMessageBox::~BaseMessageBox()
{
    m_pTarget = NULL;
    m_pSelFunc1 = NULL;
    m_pSelFunc2 = NULL;
    
    m_pColorMask = NULL;
}
    
BaseMessageBox* BaseMessageBox::create(const char* title, const char* msg, bool hasTwoBtn, CCObject* pTarget, cocos2d::SEL_CallFuncN pSelFunc1, cocos2d::SEL_CallFuncN pSelFunc2)
{
    BaseMessageBox *pRet = new BaseMessageBox();
    if (pRet && pRet->init(title, msg, hasTwoBtn, pTarget, pSelFunc1, pSelFunc2))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
    }
    return pRet;
}

bool BaseMessageBox::init(const char* title, const char* msg, bool hasTwoBtn, CCObject* pTarget, cocos2d::SEL_CallFuncN pSelFunc1, cocos2d::SEL_CallFuncN pSelFunc2)
{
    if (!CCLayer::init())
    {
        CCLog("XYMessageBox::init error!");
        return false;
    }
    
    m_pColorMask = CCLayerColor::create(ccc4(0, 0, 0, 51));
    this->addChild(m_pColorMask);
    
    CCSprite *sprscale9 = CCSprite::create("public/public_window3.png");
    m_pBkgSprite = CCScale9Sprite::create("public/public_window3.png",CCRect(0, 0, sprscale9->getContentSize().width, sprscale9->getContentSize().height), CCRect(40, 40, sprscale9->boundingBox().size.width - 80,  sprscale9->boundingBox().size.height - 80));
    m_pBkgSprite->setAnchorPoint(ccp(0.5,0.5));
    m_pBkgSprite->setPosition(SCREEN_MID_POS);
    this->addChild(m_pBkgSprite);
    
    this->setPrority(MESSAGE_BOX_PRORITY);
    if (title)
    {
        this->setTitle(title);
    }
    
    if (msg)
    {
        this->setMsg(msg);
    }
    
    
    m_pTarget = pTarget;
    m_pSelFunc1 = pSelFunc1;
    //创建两个按钮
    m_pBtn1 = XYButton::createWithFileAndPriority("public/public_btn_red.png", "public/public_btn_red_light.png", "public/public_btn_gray.png", MESSAGE_BOX_PRORITY);
    m_pBtn1->setTouchEndEvent(this, callfuncN_selector(BaseMessageBox::onFunc1));
    m_pBtn1->setPosition(ccp(m_pBkgSprite->getContentSize().width / 2, m_pBkgSprite->getContentSize().height / 2 - 80));
    m_pBtn1->setLbl("  ", FONTNAME, 20);
    m_pBkgSprite->addChild(m_pBtn1);
    
    if (hasTwoBtn)
    {
        m_pBtn1->setPosition(ccp(m_pBkgSprite->getContentSize().width / 2 - 90, m_pBkgSprite->getContentSize().height / 2 - 80));
        
        m_pSelFunc2 = pSelFunc2;
        m_pBtn2 = XYButton::createWithFileAndPriority("public/public_btn_red.png", "public/public_btn_red_light.png", "public/public_btn_gray.png", MESSAGE_BOX_PRORITY);
        m_pBtn2->setTouchEndEvent(this, callfuncN_selector(BaseMessageBox::onFunc2));
        m_pBtn2->setPosition(ccp(m_pBkgSprite->getContentSize().width / 2 + 90, m_pBkgSprite->getContentSize().height / 2 - 80));
        m_pBtn2->setLbl("  ", FONTNAME, 20);
        m_pBkgSprite->addChild(m_pBtn2);
    }
    
    return true;
}
    
void BaseMessageBox::onFunc1(CCNode* pNode)
{
    if(m_pTarget && m_pSelFunc1)
    {
        (m_pTarget->*m_pSelFunc1)(pNode);
    }
    
    this->removeAllChildrenWithCleanup(true);
    this->removeFromParentAndCleanup(true);
}

void BaseMessageBox::onFunc2(CCNode* pNode)
{
    if(m_pTarget && m_pSelFunc2)
    {
        (m_pTarget->*m_pSelFunc2)(pNode);
    }
    
    this->removeAllChildrenWithCleanup(true);
    this->removeFromParentAndCleanup(true);
}

bool BaseMessageBox::showMessageBox()
{
    TOPADDCHILD(this);
    return true;
}

// 第几个btn设置名称
void BaseMessageBox::setBtnText(int btnIdx, const char* name)
{
    if (btnIdx == 1)
    {
        m_pBtn1->setLbl(name, FONTNAME, 20);
    }
    else
    {
        if (m_pBtn2)
        {
            m_pBtn2->setLbl(name, FONTNAME, 20);
        }
    }
}
    
// 设置标题
void BaseMessageBox::setTitle(const char* title, const char* fontName, float fontSize)
{
    if (!m_pLabelTitle)
    {
        m_pLabelTitle = CCLabelTTF::create(title, fontName, fontSize, CCSizeMake(500, 40), kCCTextAlignmentCenter);
        m_pLabelTitle->setAnchorPoint(ccp(0.5f, 0.5f));
        m_pLabelTitle->setColor(FONT_COLOR_GOLD);
        m_pLabelTitle->setPosition(ccp(m_pBkgSprite->getContentSize().width / 2, m_pBkgSprite->getContentSize().height / 2 + 105));
        m_pBkgSprite->addChild(m_pLabelTitle);
    }
    else
    {
        m_pLabelTitle->setString(title);
        m_pLabelTitle->setFontSize(fontSize);
        m_pLabelTitle->setFontName(fontName);
    }
}

// 设置内容
void BaseMessageBox::setMsg(const char* msg, const char* fontName, float fontSize)
{
    if (!m_pLabelMsg)
    {
        m_pLabelMsg = CCLabelTTF::create(msg, FONTNAME, 22.f, CCSizeMake(440, 100), kCCTextAlignmentLeft);
        m_pLabelMsg->setAnchorPoint(ccp(0.5f, 0.5f));
        m_pLabelMsg->setPosition(ccp(m_pBkgSprite->getContentSize().width / 2, m_pBkgSprite->getContentSize().height / 2));
        m_pBkgSprite->addChild(m_pLabelMsg);
    }
    else
    {
        m_pLabelMsg->setString(msg);
        m_pLabelMsg->setFontSize(fontSize);
        m_pLabelMsg->setFontName(fontName);
    }
}

void BaseMessageBox::setMsgAlignment(CCTextAlignment hAligment)
{
    if (m_pLabelMsg)
    {
        m_pLabelMsg->setHorizontalAlignment(hAligment);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


XYMessageBox::XYMessageBox()
{
    m_nBtnType = E_MB_OK;
}

XYMessageBox::~XYMessageBox()
{
    
}

XYMessageBox* XYMessageBox::create(int nMBType, const char* title, const char* msg, CCObject* target, cocos2d::SEL_CallFuncN selConfirm, cocos2d::SEL_CallFuncN selCancel)
{
    XYMessageBox *pRet = new XYMessageBox();
    if (pRet && pRet->init(nMBType, title, msg, target, selConfirm, selCancel))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
    }
    return pRet;
}

bool XYMessageBox::init(int nMBType, const char* title, const char* msg, CCObject* pTarget, cocos2d::SEL_CallFuncN pSelOK, cocos2d::SEL_CallFuncN pSelCancel)
{
    bool hasTwoBtn = false;
    if (nMBType == E_MB_OK || nMBType == E_MB_CANCEL)
    {
        hasTwoBtn = false;
    }
    else if (nMBType == E_MB_OKCANCEL)
    {
        hasTwoBtn = true;
    }

    if (!BaseMessageBox::init(title, msg, hasTwoBtn, pTarget, pSelOK, pSelCancel))
    {
        CCLog("XYMessageBox::init error!");
        return false;
    }
    
    m_nBtnType = nMBType;
    this->setOKBtnText("确定");
    this->setCancelBtnText("取消");
    
    return true;
}

void XYMessageBox::onOK(CCNode* pNode)
{
    BaseMessageBox::onFunc1(pNode);
}

void XYMessageBox::onCancel(CCNode* pNode)
{
    if (m_nBtnType == E_MB_CANCEL)
    {
        BaseMessageBox::onFunc1(pNode);
    }
    else if (m_nBtnType == E_MB_OKCANCEL)
    {
        BaseMessageBox::onFunc2(pNode);
    }
}

void XYMessageBox::setOKBtnText(const char* name)
{
    this->setBtnText(1, name);
}

void XYMessageBox::setCancelBtnText(const char* name)
{
    if (m_nBtnType == E_MB_CANCEL)
    {
        this->setBtnText(1, name);
    }
    else if (m_nBtnType == E_MB_OKCANCEL)
    {
        this->setBtnText(2, name);
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RemindMessageBox::RemindMessageBox()
{
    m_pToggleBtn = NULL;
}

RemindMessageBox::~RemindMessageBox()
{
    
}

RemindMessageBox* RemindMessageBox::create(int remindID, const char* msg, CCObject* target, cocos2d::SEL_CallFuncN selConfirm, cocos2d::SEL_CallFuncN selCancel)
{
    RemindMessageBox* pRet = new RemindMessageBox();
    
    if (pRet && pRet->init(remindID, msg, target, selConfirm, selCancel))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
    }
    
    return pRet;
}

bool RemindMessageBox::init(int remindID, const char* msg, CCObject* target, cocos2d::SEL_CallFuncN selConfirm, cocos2d::SEL_CallFuncN selCancel)
{
    if (!BaseMessageBox::init("", msg, true, target, selConfirm, selCancel))
    {
        CCLOG("RemindMessageBox::init error!");
        return false;
    }
    
    m_nRemindMsgID = remindID;
    
    this->setBtnText(1, "确定");
    this->setBtnText(2, "取消");
    
    if (m_pLabelMsg)
    {
        m_pLabelMsg->setPosition(ccp(m_pLabelMsg->getPosition().x, m_pLabelMsg->getPosition().y + 30));
        m_pLabelMsg->setHorizontalAlignment(kCCTextAlignmentCenter);
    }
    
    m_pToggleBtn = XYToggleButton::create("public/ticket_frame.png", "public/ticket_green.png", MESSAGE_BOX_PRORITY);
    m_pToggleBtn->setPosition(ccp(170, 124));
    m_pToggleBtn->setTouchRect(CCRect(0, 0, 260, 100));
    m_pToggleBtn->setselect(true);
    CCLabelTTF* pLabel = CCLabelTTF::create("本次登录不再提示", FONTNAME, 18);
    pLabel->setPosition(ccp(130, 43));
    m_pToggleBtn->addChild(pLabel);
    m_pToggleBtn->setTouchRect(CCRect(-10, -10, 200, 45));
    m_pBkgSprite->addChild(m_pToggleBtn);
    
    return true;
}

void RemindMessageBox::onOK(CCNode* pNode)
{
    this->onFunc1(pNode);
}

void RemindMessageBox::onCancel(CCNode* pNode)
{
    this->onFunc2(pNode);
}

void RemindMessageBox::onFunc1(cocos2d::CCNode *pNode)
{
    if (m_pToggleBtn && m_pToggleBtn->getTurnOn())
    {
        SET_ONCE_SET_VALUE(m_nRemindMsgID, 1);
    }
    BaseMessageBox::onFunc1(pNode);
}

bool RemindMessageBox::showMessageBox()
{
    if (IS_ONCE_SET_VALUE(m_nRemindMsgID))
    {
        BaseMessageBox::onFunc1(NULL);
        return false;
    }
    else
    {
        TOPADDCHILD(this);
        return true;
    }
}

void RemindMessageBox::setTipOff()
{
    m_pToggleBtn->setselect(false);
    m_pToggleBtn->setVisible(false);
}

BuyMessageBox::BuyMessageBox()
{
    
}

BuyMessageBox::~BuyMessageBox()
{
    
}

BuyMessageBox* BuyMessageBox::create(const char* msg, const char* icon, int nNum, int nCost, bool bIsGold, CCObject* target, cocos2d::SEL_CallFuncN selConfirm, cocos2d::SEL_CallFuncN selCancel)
{
    BuyMessageBox* pRet = new BuyMessageBox();
    if (pRet && pRet->init(msg, icon, nNum, nCost, bIsGold, target, selConfirm, selCancel))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
    }
    
    return pRet;
}

bool BuyMessageBox::init(const char* msg, const char* icon, int nNum, int nCost, bool bIsGold, CCObject* target, cocos2d::SEL_CallFuncN selConfirm, cocos2d::SEL_CallFuncN selCancel)
{
    if (!BaseMessageBox::init("", msg, true, target, selConfirm, selCancel))
    {
        CCLOG("BuyMessageBox::init error!");
        return false;
    }
    this->setBtnText(1, "购买");
    this->setBtnText(2, "取消");
    
    if (m_pLabelMsg)
    {
        m_pLabelMsg->setPosition(ccp(m_pLabelMsg->getPosition().x, m_pLabelMsg->getPosition().y + 40));
        m_pLabelMsg->setHorizontalAlignment(kCCTextAlignmentCenter);
    }
    
    CCSprite* pIcon = CCSprite::create(icon);
    m_pBkgSprite->addChild(pIcon);
    pIcon->setPosition(ccp(m_pBkgSprite->getContentSize().width / 2 - 50, m_pBkgSprite->getContentSize().height / 2 + 20));
    
    char str[128] = {0};
    sprintf(str, "X%d", nNum);
    CCLabelTTF* pNum = CCLabelTTF::create(str, FONTNAME, 20);
    m_pBkgSprite->addChild(pNum);
    pNum->setPosition(ccp(m_pBkgSprite->getContentSize().width / 2 + 10, m_pBkgSprite->getContentSize().height / 2 + 20));
    pNum->setAnchorPoint(ccp(0, 0.5));
    pNum->setColor(ccYELLOW);
    
    CCLabelTTF* pTitle = CCLabelTTF::create("本次消费：", FONTNAME, 20);
    m_pBkgSprite->addChild(pTitle);
    pTitle->setPosition(ccp(m_pBkgSprite->getContentSize().width / 2 - 50, m_pBkgSprite->getContentSize().height / 2 - 30));
    
    sprintf(str, "%d%s", nCost, bIsGold ? "黄金" : "银两");
    CCLabelTTF* pCost = CCLabelTTF::create(str, FONTNAME, 20);
    m_pBkgSprite->addChild(pCost);
    pCost->setPosition(ccp(m_pBkgSprite->getContentSize().width / 2 + 10, m_pBkgSprite->getContentSize().height / 2 - 30));
    pCost->setAnchorPoint(ccp(0, 0.5));
    pCost->setColor(ccYELLOW);
    
    return true;
}

void BuyMessageBox::onOK(CCNode* pNode)
{
    this->onFunc1(pNode);
}

void BuyMessageBox::onCancel(CCNode* pNode)
{
    this->onFunc2(pNode);
}

RankAwardMessageBox* RankAwardMessageBox::create(int nCopper,int nPower, CCObject* pTarget, cocos2d::SEL_CallFuncN pSelFunc1)
{
    RankAwardMessageBox* pRet = new RankAwardMessageBox();
    
    if (pRet && pRet->init(nCopper, nPower, pTarget,pSelFunc1))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
    }
    return pRet;
}

bool RankAwardMessageBox::init(int nCopper,int nPower, CCObject* pTarget, cocos2d::SEL_CallFuncN pSelFunc1)
{
    if (!BaseMessageBox::init(NULL, NULL, false, pTarget, pSelFunc1))
    {
        CCLOG("BuyMessageBox::init error!");
        return false;
    }
    CCNode *graylayer = getChildByTag(GRAYLAYER);
    if (graylayer)
    {
        graylayer->removeFromParent();
    }
    m_pTarget = pTarget;
    m_pSelFunc1 = pSelFunc1;
    m_pBtn1->setVisible(false);
    
    
    CCSprite *spr = CCSprite::create("public/public_award.png");
    spr->setPosition(ccp(m_pBkgSprite->getContentSize().width / 2,m_pBkgSprite->getContentSize().height));
    m_pBkgSprite->addChild(spr);
    
    CCLabelTTF *lblDesc = CCLabelTTF::create("当前排名奖励", FONTNAME, 20);
    lblDesc->setColor(FONT_COLOR_GOLD);
    lblDesc->setAnchorPoint(ccp(0.5, 0.5));
    m_pBkgSprite->addChild(lblDesc);
    lblDesc->setPosition(ccp(m_pBkgSprite->getContentSize().width / 2, m_pBkgSprite->getContentSize().height - 60));
    

    
    XYButton *pClose = XYButton::createWithFileAndPriority("public/public_close.png", "public/public_close_light.png", NULL, MESSAGE_BOX_PRORITY);
    pClose->setTouchEndEvent(this, callfuncN_selector(RankAwardMessageBox::onClose));
    pClose->setPosition(ccp(m_pBkgSprite->getContentSize().width - 30,m_pBkgSprite->getContentSize().height - 30));
    m_pBkgSprite->addChild(pClose);
    
    XYButton *pGetAward = XYButton::createWithFileAndPriority("public/public_btn_red.png", "public/public_btn_red_light.png", "public/public_btn_gray.png", MESSAGE_BOX_PRORITY);
    pGetAward->setTouchEndEvent(this, callfuncN_selector(BaseMessageBox::onFunc1));
    pGetAward->setPosition(ccp(m_pBkgSprite->getContentSize().width / 2, m_pBkgSprite->getContentSize().height / 2 - 80));
    pGetAward->setLbl("领取奖励", FONTNAME, 20);
    m_pBkgSprite->addChild(pGetAward);
    return true;
}

void RankAwardMessageBox::onClose(CCNode* pNode)
{
    removeFromParent();
}

bool RechargeMessageBox::init()
{
    if (!BaseMessageBox::init(NULL, NULL, false, NULL, NULL))
    {
        return false;
    }
    m_pBtn1->setVisible(false);
    CCSprite *title = CCSprite::create("Recharge/recharge_title.png");
    title->setAnchorPoint(ccp(0.5, 1));
    title->setPosition(ccp(m_pBkgSprite->getContentSize().width / 2, m_pBkgSprite->getContentSize().height));
    m_pBkgSprite->addChild(title);
    
    CCSprite*sprGold = CCSprite::create("Recharge/recharge_4.png");
    sprGold->setAnchorPoint(ccp(0.5, 0));
    sprGold->setPosition(ccp(m_pBkgSprite->getContentSize().width / 2, m_pBkgSprite->getContentSize().height/2-10));
    m_pBkgSprite->addChild(sprGold);
    
    CCSprite *spr = CCSprite::create("Recharge/recharge_label_bottom.png");
    spr->setPosition(ccp(m_pBkgSprite->getContentSize().width / 2, m_pBkgSprite->getContentSize().height/2-20));
    m_pBkgSprite->addChild(spr);
    
    CCLabelTTF *lblDesc = CCLabelTTF::create("500黄金", FONTNAME, 20);
    lblDesc->setColor(FONT_COLOR_GOLD);
    lblDesc->setPosition(ccp(m_pBkgSprite->getContentSize().width / 2, m_pBkgSprite->getContentSize().height / 2-20));
    m_pBkgSprite->addChild(lblDesc);
    
    XYButton *pClose = XYButton::createWithFileAndPriority("public/public_close.png", "public/public_close_light.png", NULL, MESSAGE_BOX_PRORITY);
    pClose->setTouchEndEvent(this, callfuncN_selector(RankAwardMessageBox::onClose));
    pClose->setPosition(ccp(m_pBkgSprite->getContentSize().width - 30,m_pBkgSprite->getContentSize().height - 30));
    m_pBkgSprite->addChild(pClose);
    
    XYButton *pGetAward = XYButton::createWithFileAndPriority("public/public_btn_red.png", "public/public_btn_red_light.png", "public/public_btn_gray.png", MESSAGE_BOX_PRORITY);
    pGetAward->setTouchEndEvent(this, callfuncN_selector(RechargeMessageBox::onRecharge));
    pGetAward->setPosition(ccp(m_pBkgSprite->getContentSize().width / 2, m_pBkgSprite->getContentSize().height / 2 - 100));
    pGetAward->setLbl("领取", FONTNAME, 20);
    m_pBkgSprite->addChild(pGetAward);
    return true;
}

void RechargeMessageBox::onRecharge(CCNode* pNode)
{
    OnlineManager::sharedManager()->userGetOrderBillNo(1);
    removeFromParent();
}

void RechargeMessageBox::onClose(CCNode* pNode)
{
    removeFromParent();
}



GoodsCountMessageBox::GoodsCountMessageBox()
{
    m_pDelegete = NULL;
    m_pLblTotal = NULL;
    m_iBuyCountLimit = -1;
    m_iBuyCount = 1;
    m_iCost = 0;
}

GoodsCountMessageBox::~GoodsCountMessageBox()
{
    m_pLblTotal = NULL;
}

GoodsCountMessageBox* GoodsCountMessageBox::create(const char* title, const char* msg, int nCost, int limitCount, CCObject* target, cocos2d::SEL_CallFuncN selConfirm, cocos2d::SEL_CallFuncN selCancel)
{
    GoodsCountMessageBox* pRet = new GoodsCountMessageBox;
    if (pRet->init(title, msg, nCost,limitCount, target, selConfirm, selCancel))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_RELEASE_NULL(pRet);
    }
    
    return pRet;
}

void GoodsCountMessageBox::onOK(CCNode* pNode)
{
    pNode->setTag(m_iBuyCount);
    this->onFunc1(pNode);
}

void GoodsCountMessageBox::onCancel(CCNode* pNode)
{
    this->onFunc2(pNode);
}

bool GoodsCountMessageBox::init(const char* title, const char* msg, int nCost, int limitCount , CCObject* target, cocos2d::SEL_CallFuncN selConfirm, cocos2d::SEL_CallFuncN selCancel)
{
    if (!BaseMessageBox::init(title, msg, true, target, selConfirm, selCancel))
    {
        CCLOG("BuyMessageBox::init error!");
        return false;
    }
    m_pBkgSprite->setContentSize(CCSize(740, 380));
    CCScale9Sprite *bg = CCScale9Sprite::create("public/frame_blue.png");
    bg->setInsetBottom(20);
    bg->setInsetLeft(20);
    bg->setInsetRight(20);
    bg->setInsetTop(20);
    bg->setContentSize(CCSize(625, 210));
    bg->setAnchorPoint(CCPoint(0.5, 0.5));
    bg->setPosition(MIDDLE_POI(m_pBkgSprite));
    m_pBkgSprite->addChild(bg);
    
    char str[128] = {0};
    this->setBtnText(1, "购买");
    this->setBtnText(2, "取消");
    m_pBtn1->setPosition(ccp(m_pBkgSprite->getContentSize().width / 2-100, 50));
    m_pBtn2->setPosition(ccp(m_pBkgSprite->getContentSize().width / 2+100, 50));
    
    this->m_pBtn1->setTouchEndEvent(this, callfuncN_selector(GoodsCountMessageBox::onOK));
    this->m_pBtn2->setTouchEndEvent(this, callfuncN_selector(GoodsCountMessageBox::onCancel));
    
    if (m_pLabelMsg)
    {
        m_pLabelMsg->setPosition(ccp(m_pBkgSprite->getContentSize().width / 2,270));
        m_pLabelMsg->setHorizontalAlignment(kCCTextAlignmentCenter);
        sprintf(str, "您购买\"%s\"的数量", msg);
        m_pLabelMsg->setString(str);
    }
    
    if (m_pLabelTitle)
    {
        m_pLabelTitle->setAnchorPoint(ccp(0.5f, 1.f));
        m_pLabelTitle->setPosition(ccp(m_pBkgSprite->getContentSize().width / 2, m_pBkgSprite->getContentSize().height - 10));
    }
    /*
    XYDownButton* reduceBtn = XYDownButton::create("public/public_btn_red_reduce.png", "public/public_btn_red_reduce_light.png", getPrority() - 1);
    numBg->addChild(reduceBtn);
    reduceBtn->setPosition(ccp(- 30, numBg->getContentSize().height / 2));
    reduceBtn->setTouchBeganEvent(this, callfuncN_selector(GoodsCountMessageBox::onReduceClick));
    
    XYDownButton* addBtn = XYDownButton::create("public/public_btn_red_add.png", "public/public_btn_red_add_light.png", getPrority() - 1);
    numBg->addChild(addBtn);
    addBtn->setPosition(ccp(numBg->getContentSize().width + 30, numBg->getContentSize().height / 2));
    addBtn->setTouchBeganEvent(this, callfuncN_selector(GoodsCountMessageBox::onAddClick));
    */
    setBuyCountLimit(limitCount);
    m_iCost = nCost;
    
    CCSprite* pSprTotalPrice = CCSprite::create("Shop/num_bg.png");
    pSprTotalPrice->setPosition(ccp(m_pBkgSprite->getContentSize().width / 2, 100));
    m_pBkgSprite->addChild(pSprTotalPrice);
    
    CCLabelTTF* pTitle = CCLabelTTF::create("总价：", FONTNAME, 20);
    pSprTotalPrice->addChild(pTitle);
    pTitle->setColor(FONT_COLOR_BROWN);
    pTitle->setAnchorPoint(ccp(0.f, 0.5f));
    pTitle->setPosition(ccp(20.f, pSprTotalPrice->getContentSize().height / 2));
    
    sprintf(str, "%d 黄金", m_iCost * m_iBuyCount);
    m_pLblTotal = CCLabelTTF::create(str, FONTNAME, 20);
    pSprTotalPrice->addChild(m_pLblTotal);
    m_pLblTotal->setColor(ccYELLOW);
    m_pLblTotal->setAnchorPoint(ccp(0.f, 0.5));
    CCPoint pos = pTitle->getPosition();
    m_pLblTotal->setPosition(ccpAdd(pos, ccp(pTitle->getContentSize().width, 0)));
    
    numberView = XYNumberView::create(limitCount,getPrority()-1);
    numberView->setPosition(MIDDLE_POI(m_pBkgSprite));
    numberView->setDelegate(this);
    numberView->setTimes(m_iBuyCount);
    m_pBkgSprite->addChild(numberView);
    
    return true;
}

void GoodsCountMessageBox::numberChange(int index)
{
    if (m_pDelegete)
    {
        m_pDelegete->numberChange(index);
    }
}

void GoodsCountMessageBox::onReduceClick(CCNode* pNode)
{
    if (m_iBuyCount > 1)
    {
        m_iBuyCount --;
    }
    
    char str[128] = {0};
    sprintf(str, "%d 黄金", m_iCost * m_iBuyCount);
    m_pLblTotal->setString(str);
}

void GoodsCountMessageBox::onAddClick(CCNode* pNode)
{
    if (m_iBuyCountLimit > 0)
    {
        if (m_iBuyCount < m_iBuyCountLimit)
        {
            m_iBuyCount ++;
        }
        else
        {
            char str[128] = {0};
            sprintf(str, "最多只能购买%d个该商品!", m_iBuyCountLimit);
            XYTopLayer::getInstance()->ShowTip(str);
        }
    }
    else
    {
        m_iBuyCount ++;
    }
    
    char str[128] = {0};
    sprintf(str, "%d 黄金", m_iCost * m_iBuyCount);
    m_pLblTotal->setString(str);
}
