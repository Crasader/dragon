//
//  ArenaResultLayer.cpp
//  DragonBattle
//
//  Created by zeng xiaolong on 13-10-31.
//
//

#include "ArenaResultLayer.h"
ArenaResultLayer* ArenaResultLayer::create(bool bWin,int nCopper,int nPower, CCObject* pTarget, cocos2d::SEL_CallFuncN pSelFunc1)
{
    ArenaResultLayer* pRet = new ArenaResultLayer();
    
    if (pRet && pRet->init(bWin,nCopper, nPower, pTarget,pSelFunc1))
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

bool ArenaResultLayer::init(bool bWin,int nCopper,int nPre, CCObject* pTarget, cocos2d::SEL_CallFuncN pSelFunc1)
{
    if (!BaseMessageBox::init(NULL, NULL, false, pTarget, pSelFunc1))
    {
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
    
    CCSprite *sprResult = NULL;
    if (bWin)
    {
        sprResult = CCSprite::create("CCBSportsResource/win.png");
    }
    else
    {
        sprResult = CCSprite::create("CCBSportsResource/lose.png");
    }
    sprResult->setPosition(ccp(m_pBkgSprite->getContentSize().width / 2 , m_pBkgSprite->getContentSize().height / 2 + 120));
    m_pBkgSprite->addChild(sprResult);
    
    CCLabelTTF* plblAward = CCLabelTTF::create("获得奖励", FONTNAME, 20);
    plblAward->setPosition(ccp(m_pBkgSprite->getContentSize().width / 2 , m_pBkgSprite->getContentSize().height / 2 + 50));
    m_pBkgSprite->addChild(plblAward);
    
    CCSprite* lblbg = CCSprite::create("public/public_lbl_bg.png");
    m_pBkgSprite->addChild(lblbg);
    lblbg->setPosition(ccp(m_pBkgSprite->getContentSize().width / 2 , m_pBkgSprite->getContentSize().height / 2 - 10));
    
    CCSprite *sprcellbg = CCSprite::create("public/public_num_bg.png");
    sprcellbg->setPosition(ccp(m_pBkgSprite->getContentSize().width / 2 -100, m_pBkgSprite->getContentSize().height / 2 - 10));
    m_pBkgSprite->addChild(sprcellbg);
    
    CCSpriteFrame *FileFrame = CRUtils::initSpriteFrameWithName(MONEY_ICON_RES);
    CCSprite *copperIcon = CCSprite::createWithSpriteFrame(FileFrame);
    copperIcon->setScale(0.5f);
    copperIcon->setAnchorPoint(CCPointZero);
    copperIcon->setPositionX(-10);
    sprcellbg->addChild(copperIcon);
    
    char str[128] = {0};
    sprintf(str, "%d", nCopper);
    CCLabelTTF* pCopper = CCLabelTTF::create(str, FONTNAME, 16);
    pCopper->setPosition(ccp(40, 5));
    pCopper->setAnchorPoint(ccp(0, 0));
    sprcellbg->addChild(pCopper);
    
    
    sprcellbg = CCSprite::create("public/public_num_bg.png");
    sprcellbg->setPosition(ccp(m_pBkgSprite->getContentSize().width / 2 + 100, m_pBkgSprite->getContentSize().height / 2 - 10));
    m_pBkgSprite->addChild(sprcellbg);
    
    CCSpriteFrame *iconFileFrame = CRUtils::initSpriteFrameWithName(PRESTIGE_ICON_RES);
    CCSprite *powerIcon= CCSprite::createWithSpriteFrame(iconFileFrame);
    powerIcon->setPositionX(-10);
    powerIcon->setAnchorPoint(CCPointZero);
    powerIcon->setScale(0.5f);
    sprcellbg->addChild(powerIcon);
    
    sprintf(str, "%d", nPre);
    CCLabelTTF* pPower = CCLabelTTF::create(str, FONTNAME, 16);
    pPower->setPosition(ccp(40,5));
    pPower->setAnchorPoint(ccp(0, 0));
    sprcellbg->addChild(pPower);
    
    XYButton *pGetAward = XYButton::createWithFileAndPriority("public/public_btn_red.png", "public/public_btn_red_light.png", "public/public_btn_gray.png", MESSAGE_BOX_PRORITY);
    pGetAward->setTouchEndEvent(this, callfuncN_selector(BaseMessageBox::onFunc1));
    pGetAward->setPosition(ccp(m_pBkgSprite->getContentSize().width / 2, m_pBkgSprite->getContentSize().height / 2 - 80));
    pGetAward->setLbl("确定", FONTNAME, 20);
    m_pBkgSprite->addChild(pGetAward);
    return true;
}