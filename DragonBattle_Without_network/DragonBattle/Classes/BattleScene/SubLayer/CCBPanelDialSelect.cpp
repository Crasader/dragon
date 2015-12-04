//
//  CCBPanelDialSelect.cpp
//  DragonBattle
//
//  Created by 何 永翔 on 14-2-18.
//
//

#include "CCBPanelDialSelect.h"
#include "CCBDefine.h"
#include "UserData.h"
#include "OnlineManager.h"
#include "CommonUtility.h"
#include "XYMessageBox.h"
#include "AudioResource.h"
#include "CommonUtility.h"
#include "ItemSprite.h"
#include "CRUtils.h"

#define SELE_TAG (1999)
#define ITEM_COUNT (14)
#define FIRST_COST (5)
#define SECOND_COST (10)

USING_NS_CC_EXT;

CCBPanelDialSelect::CCBPanelDialSelect()
{
    for (int i = 0; i < ITEM_COUNT; i++)
    {
        m_pItems[i] = NULL;
        m_pItemSprites[i] = NULL;
    }
    
    m_pBtnReturn = NULL;
    m_pBtnGold = NULL;
    m_pBtnDial = NULL;
    
    m_pGoldTotal = NULL;
    m_pDesc = NULL;
    
    m_nFirstIndex = 0;
    m_nSecondIndex = 0;
    m_nDialIndex = 0;
    m_nTotalCostGold = 0;
    
    m_bIsBagFull = false;
    
    m_pDelegate = NULL;
    
    m_getMap.clear();
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CCBPanelDialSelect::getAllBattlePrize), USER_GET_ALL_BATTLE_PRIZE_NOTE, NULL);
}

CCBPanelDialSelect::~CCBPanelDialSelect()
{
    for (int i = 0; i < ITEM_COUNT; i++) CC_SAFE_RELEASE_NULL(m_pItems[i]);
    
    CC_SAFE_RELEASE_NULL(m_pBtnReturn);
    CC_SAFE_RELEASE_NULL(m_pBtnGold);
    CC_SAFE_RELEASE_NULL(m_pBtnDial);
    
    CC_SAFE_RELEASE_NULL(m_pGoldTotal);
    CC_SAFE_RELEASE_NULL(m_pDesc);
    
    m_getMap.clear();
    
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}

void CCBPanelDialSelect::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader)
{
    loadLayer();
}

SEL_MenuHandler CCBPanelDialSelect::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool CCBPanelDialSelect::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "Item0", CCSprite*, this->m_pItems[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "Item1", CCSprite*, this->m_pItems[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "Item2", CCSprite*, this->m_pItems[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "Item3", CCSprite*, this->m_pItems[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "Item4", CCSprite*, this->m_pItems[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "Item5", CCSprite*, this->m_pItems[5]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "Item6", CCSprite*, this->m_pItems[6]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "Item7", CCSprite*, this->m_pItems[7]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "Item8", CCSprite*, this->m_pItems[8]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "Item9", CCSprite*, this->m_pItems[9]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "Item10", CCSprite*, this->m_pItems[10]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "Item11", CCSprite*, this->m_pItems[11]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "Item12", CCSprite*, this->m_pItems[12]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "Item13", CCSprite*, this->m_pItems[13]);
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnDial", CCControlButton*, this->m_pBtnDial);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnGold", CCControlButton*, this->m_pBtnGold);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnReturn", CCControlButton*, this->m_pBtnReturn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "LblGoldTotal", CCLabelTTF*, this->m_pGoldTotal);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "LblDesc", CCLabelTTF*, this->m_pDesc);

    return false;
}

SEL_CCControlHandler CCBPanelDialSelect::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onDial", CCBPanelDialSelect::onDial);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGold", CCBPanelDialSelect::onGold);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onReturn", CCBPanelDialSelect::onReturn);
    
    return NULL;
}

void CCBPanelDialSelect::dialToIndex(int nIndex, int nIndex1)
{
    CCSprite* pSele = dynamic_cast<CCSprite*>(this->getChildByTag(SELE_TAG));
    if (pSele == NULL)
    {
        pSele = CCSprite::create("public/public_bag_frame_sele.png");
        this->addChild(pSele);
        pSele->setTag(SELE_TAG);
    }
    
    CCArray* pArray = CCArray::create();
    m_nFirstIndex = m_nDialIndex + (ITEM_COUNT - m_nDialIndex % ITEM_COUNT)  + ITEM_COUNT * 3 + (nIndex > nIndex1 ? nIndex : nIndex1);
    m_nSecondIndex = nIndex1 == 0 ? 0 : m_nFirstIndex - abs(nIndex - nIndex1);
    
    float interval = 0.4f;
    float rate = 1.5f;
    int gradualNum = 6;
    for (int i = m_nDialIndex; i < m_nDialIndex + gradualNum; i++)
    {
        CCCallFunc* pFunc = CCCallFunc::create(this, callfunc_selector(CCBPanelDialSelect::setDialPosition));
        pArray->addObject(pFunc);
        CCDelayTime* pDelay = CCDelayTime::create(interval);
        pArray->addObject(pDelay);
        interval /= rate;
    }
    for (int i = m_nDialIndex + gradualNum; i < m_nFirstIndex - gradualNum; i++)
    {
        CCCallFunc* pFunc = CCCallFunc::create(this, callfunc_selector(CCBPanelDialSelect::setDialPosition));
        pArray->addObject(pFunc);
        CCDelayTime* pDelay = CCDelayTime::create(interval);
        pArray->addObject(pDelay);
    }
    for (int i = m_nFirstIndex - gradualNum; i < m_nFirstIndex + 1; i++)
    {
        interval *= rate;
        CCCallFunc* pFunc = CCCallFunc::create(this, callfunc_selector(CCBPanelDialSelect::setDialPosition));
        pArray->addObject(pFunc);
        CCDelayTime* pDelay = CCDelayTime::create(interval);
        pArray->addObject(pDelay);
    }
    pSele->stopAllActions();
    pSele->setOpacity(255);
    pSele->setScale(1);
    pSele->runAction(CCSequence::create(pArray));
}

void CCBPanelDialSelect::selectItem(int nIndex)
{
    /*
    CCSprite* pToken = CCSprite::create("public/public_bag_frame_sele.png");
    this->addChild(pToken);
    pToken->setPosition(m_pItems[nIndex % ITEM_COUNT]->getPosition());
     */
    char str[128] = {0};
    sprintf(str, "item_%d.png", m_pItemSprites[nIndex % ITEM_COUNT]->itemID);
    CCSpriteFrame* pFrame = CRUtils::getgaySpriteFrame(str);
    CCSprite* pImg = CCSprite::createWithSpriteFrame(pFrame);
    m_pItemSprites[nIndex % ITEM_COUNT]->addChild(pImg);
    CCSize size = m_pItemSprites[nIndex % ITEM_COUNT]->getContentSize();
    pImg->setPosition(ccp(size.width / 2.0f, size.height / 2.0f));
    //m_pItemSprites[nIndex % ITEM_COUNT]->setTexture(pFrame->getTexture());
    
    XYTopLayer::getInstance()->ShowSingleTip(m_pItemSprites[nIndex % ITEM_COUNT]->itemID, m_pItemSprites[nIndex % ITEM_COUNT]->itemNum);
}

void CCBPanelDialSelect::setDialPosition()
{
    if (m_nSecondIndex != 0 && m_nSecondIndex == m_nDialIndex)
    {
        this->selectItem(m_nDialIndex);
    }
    if (m_nFirstIndex == m_nDialIndex)
    {
        this->selectItem(m_nDialIndex);
        
        if (atoi(m_pGoldTotal->getString()) != m_nTotalCostGold - FIRST_COST - SECOND_COST) m_pBtnDial->setEnabled(true);
        m_pBtnReturn->setEnabled(true);
        m_pBtnGold->setEnabled(true);
        CCSequence* pSeq = CCSequence::create(CCSpawn::create(CCScaleTo::create(1.0f, 1.05f), CCFadeOut::create(1.0f), NULL), CCScaleTo::create(0, 1), CCFadeIn::create(0), NULL);
        this->getChildByTag(SELE_TAG)->runAction(CCRepeatForever::create(pSeq));
        
        if (m_bIsBagFull) XYTopLayer::getInstance()->ShowTip("背包已满，获得的奖励已放入个人礼包！");
        
        
    }
    this->getChildByTag(SELE_TAG)->setPosition(m_pItems[m_nDialIndex % ITEM_COUNT]->getPosition());
    m_nDialIndex++;
}

void CCBPanelDialSelect::getAllBattlePrize(cocos2d::CCObject *p)
{
    m_pBtnGold->setVisible(false);
    this->getChildByTag(999)->setVisible(false);
    m_pBtnDial->setVisible(false);
    m_pDesc->setVisible(false);
    m_pBtnReturn->setEnabled(true);
    
    NotificeObject* obj = (NotificeObject*)p;
    cli_user_get_all_battle_prize_out* out = (cli_user_get_all_battle_prize_out*)obj->msg;
    //out->gift_state//0放到背包中了 1放到个人礼包中了
    gUserData->addGold(-out->cost_gold);
    std::vector<item_unit_t> vec;
    vec.clear();
    for (int i = 0; i < ITEM_COUNT; i++)
    {
        if (m_getMap.find(i) == m_getMap.end())
        {
            item_unit_t unit;
            unit.item_id = m_pItemSprites[i]->itemID;
            unit.item_cnt = m_pItemSprites[i]->itemNum;
            vec.push_back(unit);
        }
            
    }
    XYTopLayer::getInstance()->ShowTip(&vec);
    if (out->gift_state == 1) XYTopLayer::getInstance()->ShowTip("背包已满，获得的奖励已放入个人礼包！");
}

void CCBPanelDialSelect::setPrize(const char* pImg, const char* pName, int nId, int nCount, bool bIsBagFull)
{
    for (int i = 0; i < ITEM_COUNT; i++)
    {
        if (m_pItemSprites[i]->itemID == nId && m_pItemSprites[i]->itemNum == nCount)
        {
            m_getMap.insert(std::pair<int, int>(i, i));
            if (atoi(m_pGoldTotal->getString()) == m_nTotalCostGold - FIRST_COST)//第三次抽奖
            {
                this->dialToIndex(i, 0);
                m_pGoldTotal->setString(CCString::createWithFormat("%d", m_nTotalCostGold - FIRST_COST-SECOND_COST)->getCString());
                m_pBtnDial->setEnabled(false);
                m_pDesc->setColor(ccGRAY);
                m_pDesc->setString("已领取");
            }
            else if (m_nFirstIndex > 0)//第二次抽奖
            {
                this->dialToIndex(i, 0);
                m_pGoldTotal->setString(CCString::createWithFormat("%d", m_nTotalCostGold - FIRST_COST)->getCString());
                m_pBtnDial->setEnabled(false);
                m_pDesc->setString(CCString::createWithFormat("%d\n黄金", SECOND_COST)->getCString());
            }
            else//第一次抽奖
            {
                this->dialToIndex(i, 0);
                m_pBtnDial->setEnabled(false);
                m_pDesc->setString(CCString::createWithFormat("%d\n黄金", FIRST_COST)->getCString());
                m_pBtnReturn->setEnabled(true);
            }
            m_pBtnReturn->setEnabled(false);
            m_pBtnGold->setEnabled(false);
            break;
        }
    }
    m_bIsBagFull = bIsBagFull;
}

void CCBPanelDialSelect::onDial(CCObject* pTarget, CCControlEvent event)
{
    if (atoi(m_pGoldTotal->getString()) == m_nTotalCostGold - FIRST_COST)//第三次抽奖
    {
        if (m_pDelegate != NULL) m_pDelegate->onPanelDialSelectDial(SECOND_COST, 3);
    }
    else if (m_nFirstIndex > 0)//第二次抽奖
    {
        if (m_pDelegate != NULL) m_pDelegate->onPanelDialSelectDial(FIRST_COST, 2);
    }
    else//第一次抽奖
    {
        if (m_pDelegate != NULL) m_pDelegate->onPanelDialSelectDial(0, 1);
    }
}
void CCBPanelDialSelect::onGold(CCObject* pTarget, CCControlEvent event)
{
    OnlineManager::sharedManager()->userGetAllBattlePrize();
}
void CCBPanelDialSelect::onReturn(CCObject* pTarget, CCControlEvent event)
{
    if (m_pDelegate != NULL) m_pDelegate->onPanelDialSelectReturn();
    this->removeFromParent();
}

void CCBPanelDialSelect::loadLayer()
{
    /*
    cli_user_chat_out out;
    out.userid = 10000;
    sprintf((char*)out.nick, "名字");
    out.level = 10;
    out.unit_id = 0;
	out.chattime = 1392725190;
	out.vip_level = 1;
    out.target_id = 10000;
    out.mlen = 255;
    out.channel_id = 602;
    message msg;
    msg.times.type = 1;
    msg.times.cnt = 5;
    memcpy(out.msg, &msg, 255);
    
    gUserData->m_dataEx->fixNoticeVec.push_back(out);
     */
}

void CCBPanelDialSelect::onItem(CCNode* pNode)
{
    ItemSprite* pBtn = dynamic_cast<ItemSprite*>(pNode);
}

void CCBPanelDialSelect::setData(std::vector<item_unit_t>* pItemVec, int nTotalCostGold)
{
    m_nTotalCostGold = nTotalCostGold;
    m_pGoldTotal->setString(CCString::createWithFormat("%d", nTotalCostGold)->getCString());
    
    this->setPrority(-10);
    m_pBtnDial->setTouchPriority(this->getPrority() - 1);
    m_pBtnGold->setTouchPriority(this->getPrority() - 1);
    m_pBtnReturn->setTouchPriority(this->getPrority() - 1);
    m_pBtnReturn->setEnabled(false);
    
    int i = 0;
    for (std::vector<item_unit_t>::iterator it = pItemVec->begin(); it != pItemVec->end(); it++)
    {
        m_pItemSprites[i] = CommonUtility::createButton(it->item_id, it->item_cnt, this, callfuncN_selector(CCBPanelDialSelect::onItem), false, false, this->getPrority() - 1);
        this->addChild(m_pItemSprites[i]);
        m_pItemSprites[i]->setPosition(m_pItems[i]->getPosition());
        i++;
    }
}