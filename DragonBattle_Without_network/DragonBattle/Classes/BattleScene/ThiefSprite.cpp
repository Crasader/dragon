//
//  ThiefSprite.cpp
//  DragonBattle
//
//  Created by 何 永翔 on 13-11-25.
//
//

#include "ThiefSprite.h"
#include "GameData.h"
#include "BattleLogic.h"
#include "GameManager.h"
#include "BattleMapUnits.h"
#include "AiManager.h"

USING_NS_CC;

ThiefSprite::ThiefSprite()
{
    m_bAIActivated = false;
    m_ReLife = true;
    
    m_nLastNodeID = -1;
    m_nLastCalAI = kAIZero;
    m_nLastAI = kAIZero;
    m_nRunFrames = 0;
    m_bLastAIRunSucc = false;
    m_nAIRunFrames = 0;
}

ThiefSprite::~ThiefSprite()
{
    
}

ThiefSprite* ThiefSprite::create(UnitBaseConf *conf,MonsterUnitConf *info, item_unit_t* pUnit)
{
    ThiefSprite* pRet = new ThiefSprite();
    if (pRet && pRet->init(conf,info, pUnit))
    {
        pRet->autorelease();
        return pRet;
    }
    
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool ThiefSprite::init(UnitBaseConf *conf,MonsterUnitConf *info, item_unit_t* pUnit)
{
    conf->nUnitType = kThief;
    conf->nLife = 3;
    if (!BattleUnitSprite::init(conf))
    {
        CCLOG("ThiefSprite::init error!");
        return false;
    }

    m_MonsterConf = info;
    m_nLV = info->getLV();
    m_nHp = info->getMaxHP();
    m_nAIRunFrames = 10;
    
    m_dropItem.item_id = pUnit->item_id;
    m_dropItem.item_cnt = pUnit->item_cnt;
    
    this->setScale(1.05f);
    
    m_fAngryTime = 999999;//小偷没有硬直
    /*
     CCLabelTTF* pLabelTip = CCLabelTTF::create("", "Arial", 24);
     this->addChild(pLabelTip);
     char strMonsterName[64];
     // for test
     sprintf(strMonsterName, "%s%d", m_pUnitConf->unitName, m_pUnitConf->nResourceID);
     pLabelTip->setString(strMonsterName);
     pLabelTip->setColor(ccBLUE);
     pLabelTip->setPosition(ccp(this->getContentSize().width / 2, 50));
     */
    
    return true;
}

void ThiefSprite::onDie(bool* pIsToRight)
{
    BattleMapUnits* pMapUnitLayer = (BattleMapUnits*)GameManager::sharedInstance()->getBattleMapLayer();
    if (pMapUnitLayer)
    {
        pMapUnitLayer->addSelfExp(m_MonsterConf->nUnitID, m_MonsterConf->nExp);
    }
    CCNotificationCenter::sharedNotificationCenter()->postNotification(KILLMONSTER_NOTE);
}

// 怪物的坐标点 只用判断Y坐标是否错误
void ThiefSprite::update()
{
    BattleUnitSprite::update();
}

int ThiefSprite::correctDamage(AttackInfo_t* pAttack, float fHurt)
{
    //小偷每次伤1点血。
    return 1;
}

void ThiefSprite::onBeHit(bool bIsToRight)
{

}

void ThiefSprite::runAIOnce(BattleUnitSprite* pHero)
{
    if (m_nHp <= 0)
    {
        return ;
    }
    
    //AI是否激活
    if (!m_bAIActivated)
    {
        return ;
    }
    
    // 上一次计算的AI的结果
    if (m_nLastCalAI != kAIZero)
    {

        this->runAIAction(pHero, m_nLastCalAI, m_nRunFrames);
        if (m_bLastAIRunSucc)
        {
            m_nLastAI = m_nLastCalAI;
            m_nLastCalAI = kAIZero;
        }
    }
    
    BattleMapUnits* pMapUnitLayer = (BattleMapUnits*)GameManager::sharedInstance()->getBattleMapLayer();
    
    bool bLastAI = false;
    int nRunFrames = 0;
    kAIState aiAction = AiManager::getUnitAIPolicy(this, pHero, pMapUnitLayer, 28,0, bLastAI, nRunFrames, m_nLastNodeID, m_nAIRunFrames);
    if (nRunFrames == 0)
    {
        m_nAIRunFrames--;
        if (m_nAIRunFrames < 0)
        {
            m_nAIRunFrames = 0;
        }
        return ;
    }
    
    if (bLastAI)
    {
        //CCLog("LastCalAI:%d, %d", aiAction, nRunFrames);
        m_nLastCalAI = aiAction;
        m_nRunFrames = nRunFrames;
        return ;
    }

    this->runAIAction(pHero, aiAction, nRunFrames);
    if (m_bLastAIRunSucc)
    {
        m_nLastAI = aiAction;
    }
}

void ThiefSprite::runAIAction(BattleUnitSprite* pHero, kAIState aiAction, int nRunFrames)
{
    m_bLastAIRunSucc = false;
    switch (aiAction)
    {
        case kAIStandBy://待机
        {
            if (BattleLogic::canChangeUnitAction(m_nCurAction, kStandby))
            {
                this->changeAction(kStandby);
                m_nAIRunFrames = nRunFrames;
                m_bLastAIRunSucc = true;
            }
            else if (m_nCurAction == kStandby)//如果原本就是待机状态，只要延长待机状态即可
            {
                m_nAIRunFrames = nRunFrames;
                m_bLastAIRunSucc = true;
            }
            break;
        }
        case kAIMoveToHero://靠近英雄
        {
            if (BattleLogic::canChangeUnitAction(m_nCurAction, kRunning) || m_nCurAction == kRunning) //行走
            {
                if (getPosition().x < pHero->getPosition().x)
                {
                    this->goForward();
                }
                else
                {
                    this->goBack();
                }
                m_nAIRunFrames = nRunFrames;
                m_bLastAIRunSucc = true;
            }
            break;
        }
        case kAIFarAwayTarget://远离目标
        {
            if (BattleLogic::canChangeUnitAction(m_nCurAction, kRunning)) //行走
            {
                if (getPosition().x > pHero->getPosition().x)
                {
                    this->goForward();
                }
                else
                {
                    this->goBack();
                }
                m_nAIRunFrames = nRunFrames;
                m_bLastAIRunSucc = true;
            }
            break;
        }
        case kAIMoveForward:          //一直向前走（仅小偷有效）
        {
            if (BattleLogic::canChangeUnitAction(m_nCurAction, kRunning)) //行走
            {
                this->goForward();
                m_nAIRunFrames = nRunFrames;
                m_bLastAIRunSucc = true;
            }
            break;
        }
        case kAIFlashToOutScene:       //闪现出场景外（仅小偷有效）
        {
            if (BattleLogic::canChangeUnitAction(m_nCurAction, kStandby))
            {
                this->setPosition(ccp(-300, this->getPosition().y));
                this->changeAction(kStandby);
                this->setAIActivated(false);
            }
        }
        default:
        {
            return ;
        }
    }
}