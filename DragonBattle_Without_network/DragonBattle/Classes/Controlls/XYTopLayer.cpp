//
//  XYTopLayer.cpp
//  XY
//
//  Created by long on 12-12-25.
//
//

#include "XYTopLayer.h"
#include "CCScale9Sprite.h"
#include "GameData.h"
#include "HRichLabelTTF.h"
#include "EffectResource.h"
#include "UserData.h"

USING_NS_CC_EXT;

static XYTopLayer *s_TopLayer = NULL;
XYTopLayer::XYTopLayer(void) {
}

XYTopLayer::~XYTopLayer(void) {
}

XYTopLayer *XYTopLayer::getInstance()
{
    if (s_TopLayer == NULL) {
        s_TopLayer = new XYTopLayer();
        s_TopLayer->init();
    }
    return s_TopLayer;
}

bool XYTopLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}
void XYTopLayer::registerWithTouchDispatcher(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -200, false);
}
bool XYTopLayer::init() {
    if (!CCLayer::init()) {
        return false;
    }
    setTouchEnabled(true);
    return true;
}

void XYTopLayer::ShowWaiting() {
    if (this->getChildByTag(888)) {
        return;
    }
}

void XYTopLayer::RemoveWaiting() {
    if (this->getChildByTag(888)) {
        this->removeChildByTag(888, true);
    }
}

void XYTopLayer::ShowTip(const char *str)
{
    float x = CCDirector::sharedDirector()->getWinSize().width / 2;
    float y = CCDirector::sharedDirector()->getWinSize().height / 2;
    
    CCSprite *spr = CCSprite::create("public/tips_bg.png");
    spr->setPosition(ccp(x, y - 20));
    CCLabelTTF *lbl = CCLabelTTF::create(str, "Arial-BoldMT", 20);
    lbl->setTag(1);
    lbl->setPosition(ccp(spr->getContentSize().width/2, spr->getContentSize().height/2));
    spr->addChild(lbl);
    addChild(spr);
    spr->runAction(CCSequence::create(CCMoveBy::create(0.5, ccp(0,60)),CCDelayTime::create(1),CCCallFuncN::create(this,callfuncN_selector(XYTopLayer::fadeLbl)),CCFadeOut::create(0.5),CCRemoveSelf::create(),NULL));
}

void XYTopLayer::flyItemToPortrait(int nItemId, int nItemCount, CCPoint worldPosition, int nIndex, bool bUpAndDownFirst, bool bIsOriginSize)
{
    CCPoint pos = this->convertToNodeSpace(worldPosition);
    
    CCSprite* pBg = CCSprite::create("public/tip_frame.png");
    this->addChild(pBg);
    pBg->setPosition(pos);
    pBg->setOpacity(0);
    pBg->setScale(bIsOriginSize ? 1 : 1.5f);
    
    CCSprite* pBottom = CCSprite::create("public/tip_bottom.png");
    pBg->addChild(pBottom);
    pBottom->setPosition(ccp(pBg->getContentSize().width / 2.0f, -10));
    pBottom->setOpacity(0);
    
    CCSprite* pBg1 = CCSprite::create("public/tip_frame.png");
    pBg->addChild(pBg1);
    pBg1->setPosition(ccp(pBg->getContentSize().width / 2.0f, pBg->getContentSize().height / 2.0f));
    pBg1->setOpacity(0);
    
    char str[128] = {0};
    char str1[128] = {0};

    const item_t*  pItem = GameData::getItem(nItemId);
    HRichLabelTTF* pNameLabel = HRichLabelTTF::create(pItem->name, "Arial-BoldMT", 16);
    pBg->addChild(pNameLabel);
    pNameLabel->setPosition(ccp(pBg->getContentSize().width / 2.0f, -10));
    pNameLabel->setStroke(ccBLACK);
    pNameLabel->setColor(ITEM_QUARTY_COLOR[pItem->nQuality]);
    pNameLabel->setOpacity(0);
    pNameLabel->setAnchorPoint(ccp(0.5f, 0.5f));
    CCSprite* pSprite = NULL;
    if (getKindOfItem(nItemId) == kPartnerCardKind)
    {
        sprintf(str, "headicon/headicon_%d.png",pItem->u.card.nPartnerID);
        pSprite = CCSprite::create(str);
    }
    else
    {
        sprintf(str, "item_%d.png", nItemId);
        pSprite = CCSprite::createWithSpriteFrameName(str);
    }
    pBg->addChild(pSprite);
    pSprite->setPosition(ccp(pBg->getContentSize().width / 2.0f, pBg->getContentSize().height / 2.0f));
    pSprite->setOpacity(0);
    
    sprintf(str1, "X%s", UserData::getShortNum(nItemCount, str));
    CCLabelTTF* pLabel = CCLabelTTF::create(str1, "Arial-BoldMT", 18);
    pBg->addChild(pLabel);
    pLabel->setAnchorPoint(ccp(1, 0));
    pLabel->setPosition(ccp(pBg->getContentSize().width - 5, 5));
    pLabel->setOpacity(0);
    
    float duration = 0.5;
    CCPoint targetPos = ccp(60, SCREEN_HEIGTH - 60);
    
    ccBezierConfig bezier;
    int radius = 200;
    int x = (rand() % (radius * 2)) - radius;
    float y = sqrtf(radius * radius - x * x);
    bezier.controlPoint_1 = ccpAdd(pos, ccp(x, y));
    bezier.controlPoint_2 = ccpAdd(pos, ccp(x, y));
    bezier.endPosition = targetPos;
    
    CCSequence* pSeq1 = NULL;
    if (bUpAndDownFirst)
    {
        pSeq1 = CCSequence::create(CCDelayTime::create(nIndex * 3.5f + 0.4f), CCFadeIn::create(0.2f), NULL);
        CCSequence* pSeq = CCSequence::create(CCMoveBy::create(0.5f, ccp(0, 20)), CCMoveBy::create(0.5f, ccp(0, -20)), NULL);
        CCSequence* pSeq2 = CCSequence::create(pSeq1, CCDelayTime::create(0.2f), pSeq, dynamic_cast<CCSequence*>(pSeq->copy()->autorelease()), CCSpawn::create(CCScaleTo::create(duration, 0.5f), CCBezierTo::create(duration, bezier), NULL), CCRemoveSelf::create(), NULL);
        pBg->runAction(pSeq2);
    }
    else
    {
        pSeq1 = CCSequence::create(CCDelayTime::create(nIndex * 0.5f + 0.4f), CCFadeIn::create(0.2f), NULL);
        CCSequence* pSeq2 = CCSequence::create(pSeq1, CCDelayTime::create(0.2f), CCSpawn::create(CCScaleTo::create(duration, 0.5f), CCBezierTo::create(duration, bezier), NULL), CCRemoveSelf::create(), NULL);
        pBg->runAction(pSeq2);
    }
    pBg1->runAction(dynamic_cast<CCSequence*>(pSeq1->copy()->autorelease()));
    pLabel->runAction(dynamic_cast<CCSequence*>(pSeq1->copy()->autorelease()));
    pNameLabel->runAction(dynamic_cast<CCSequence*>(pSeq1->copy()->autorelease()));
    pBottom->runAction(dynamic_cast<CCSequence*>(pSeq1->copy()->autorelease()));
    pSprite->runAction(dynamic_cast<CCSequence*>(pSeq1->copy()->autorelease()));
}


CCPoint XYTopLayer::getTipPosition()
{
    return ccp(SCREEN_WIDTH / 2.0f, SCREEN_HEIGTH / 2.0f);
}

void XYTopLayer::showTipEffect(CCPoint startPos)
{
    CCSprite* insetEffect = EffectResource::sharedInstance()->spriteWithEffect(41, false, true);
    this->addChild(insetEffect);
    insetEffect->setPosition(startPos.equals(CCPointZero) ? this->getTipPosition() : startPos);
}

void XYTopLayer::ShowTip(const std::vector<item_unit_t>* pItemVec)
{
    int n = 0;
    for (std::vector<item_unit_t>::const_iterator it = pItemVec->begin(); it != pItemVec->end(); it++)
    {
        this->flyItemToPortrait(it->item_id, it->item_cnt, this->getTipPosition(), n, false, true);
        n++;
    }
    this->showTipEffect(CCPointZero);
}

void XYTopLayer::ShowTip(const std::vector<package_item_t>* pItemVec)
{
    int n = 0;
    for (std::vector<package_item_t>::const_iterator it = pItemVec->begin(); it != pItemVec->end(); it++)
    {
        this->flyItemToPortrait(it->item_id, it->item_cnt, this->getTipPosition(), n, false, true);
        n++;
    }
    this->showTipEffect(CCPointZero);
}

void XYTopLayer::ShowBoxTip(const std::vector<package_item_t>* pItemVec, CCPoint startPos)
{
    int n = 0;
    for (std::vector<package_item_t>::const_iterator it = pItemVec->begin(); it != pItemVec->end(); it++)
    {
        this->flyItemToPortrait(it->item_id, it->item_cnt, startPos, n, true, true);
        n++;
    }
    this->showTipEffect(startPos);
}

void XYTopLayer::ShowSingleTip(int nItemId, int nItemCount)
{
    this->flyItemToPortrait(nItemId, nItemCount, this->getTipPosition(), 0, false, true);

    this->showTipEffect(CCPointZero);
}

void XYTopLayer::ShowSimpleTip(CCArray* content, ccColor3B color/* = ccGREEN*/)
{
    if (content == NULL || content->count() <= 0) {
        return;
    }
    
    CCNode* nodeContainer = CCNode::create();
    
    int width = 368;
    int height = 130;
    
    int i = 0;
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(content, pObj)
    {
        CCString* str = (CCString*)pObj;
        HRichLabelTTF* lbl = HRichLabelTTF::create(str->getCString(), FONTNAME, 24);
        lbl->setAnchorPoint(ccp(0.5f, 0.f));
        lbl->setColor(color);
        lbl->setStroke(ccBLACK);
        
        lbl->setPosition(ccp(width / 2, 65 + lbl->getContentSize().height * i));
        nodeContainer->addChild(lbl);
        
        height += lbl->getContentSize().height;
        i ++;
    }
    
    float x = CCDirector::sharedDirector()->getWinSize().width / 2;
    float y = CCDirector::sharedDirector()->getWinSize().height / 2;
    
    nodeContainer->setTag(1);
    nodeContainer->setContentSize(CCSizeMake(width, height));
    nodeContainer->setPosition(ccp(x, y));
    nodeContainer->setAnchorPoint(ccp(0.5, 0.5f));
    
    nodeContainer->runAction(CCSequence::create(CCMoveBy::create(0.5, ccp(0,60)),CCDelayTime::create(1),CCCallFuncN::create(this,callfuncN_selector(XYTopLayer::fadeLbl)),CCFadeOut::create(0.5),CCRemoveSelf::create(),NULL));
    
    addChild(nodeContainer);
}

void XYTopLayer::ShowTip(const std::vector<ItemUnit_t>* pItemVec)
{
    int n = 0;
    for (std::vector<ItemUnit_t>::const_iterator it = pItemVec->begin(); it != pItemVec->end(); it++)
    {
        this->flyItemToPortrait(it->nItemID, it->nItemCount, this->getTipPosition(), n, false, true);
        n++;
    }
    this->showTipEffect(CCPointZero);
}

void XYTopLayer::ShowTip(int swapID)
{
    this->ShowTip(&(GameData::getSwapItem(swapID)->swapItemOut));
}

void XYTopLayer::ShowAwardLayer( std::vector<ItemUnit_t>itemVec)
{
   
}

void XYTopLayer::ShowAwardLayer(std::vector<item_unit_t> itemVec)
{
    
}

void XYTopLayer::fadeLbl(CCNode *p)
{
    if (p->getChildByTag(1)) {
        p->getChildByTag(1)->runAction(CCFadeOut::create(0.4));
    } ;
}