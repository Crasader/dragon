
//
//  MonsterSprite.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-19.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "MonsterSprite.h"
#include "GameData.h"
#include "BattleLogic.h"
#include "GameManager.h"
#include "BattleMapUnits.h"
#include "AiManager.h"

USING_NS_CC;

MonsterSprite::MonsterSprite()
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

MonsterSprite::~MonsterSprite()
{
    
}

MonsterSprite* MonsterSprite::create(UnitBaseConf *conf,MonsterUnitConf *info)
{
    MonsterSprite* pRet = new MonsterSprite();
    if (pRet && pRet->init(conf,info))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool MonsterSprite::init(UnitBaseConf *conf,MonsterUnitConf *info)
{
    if (!BattleUnitSprite::init(conf))
    {
        CCLOG("MonsterSprite::init error!");
        return false;
    }
    m_MonsterConf = info;
    m_nLV = info->getLV();
    m_nHp = conf->getMaxHP();
    m_nAIRunFrames = 10;
    return true;
}

void MonsterSprite::onDie(bool* pIsToRight)
{
    BattleMapUnits* pMapUnitLayer = (BattleMapUnits*)GameManager::sharedInstance()->getBattleMapLayer();
    if (pMapUnitLayer)
    {
        pMapUnitLayer->addSelfExp(m_MonsterConf->nUnitID, m_MonsterConf->nExp);
    }
    CCNotificationCenter::sharedNotificationCenter()->postNotification(KILLMONSTER_NOTE);
}

// 怪物的坐标点 只用判断Y坐标是否错误
void MonsterSprite::update()
{
    BattleUnitSprite::update();
}

void MonsterSprite::runAIOnce(BattleUnitSprite* pHero)
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
        //如果怪物仍在被击打状态，则m_nLastCalAI延后执行
        if (this->getCurAction() == kBeHit)
        {
            return;
        }
        //两次攻击不能接在一起
        if ((m_nLastAI == kAISkillAttack1 || m_nLastAI == kAISkillAttack2 || m_nLastAI == kAINormalAttack) && m_nLastAI == m_nLastCalAI)
        {
            return;
        }
        this->runAIAction(pHero, m_nLastCalAI, m_nRunFrames);
        if (m_bLastAIRunSucc)
        {
            m_nLastAI = m_nLastCalAI;
            m_nLastCalAI = kAIZero;
        }
    }
    //由于kBeHit是瞬时状态，所以，其不受m_nAIRunFrames影响，需立即判断
    if (this->getCurAction() == kBeHit)
    {
        m_nAIRunFrames = 0;

    }
    // 当前行为是否完成
    if (IS_ATTACK_ACTION(m_nCurAction) || IS_AIRATTACK_ACTION(m_nCurAction) || IS_SKILL_ACTION(m_nCurAction) ||
        IS_JUMPING_ACTION(m_nCurAction) || m_nCurAction == kBeHit || m_nCurAction == kGetUp)
    {
        return;
    }
    
    BattleMapUnits* pMapUnitLayer = (BattleMapUnits*)GameManager::sharedInstance()->getBattleMapLayer();
    
    bool bLastAI = false;
    int nRunFrames = 0;
    kAIState aiAction = AiManager::getUnitAIPolicy(this, pHero, pMapUnitLayer, m_unitConf.nAIID,m_nAttackDistance, bLastAI, nRunFrames, m_nLastNodeID, m_nAIRunFrames);
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
    //两次攻击不能接在一起
    if ((m_nLastAI == kAISkillAttack1 || m_nLastAI == kAISkillAttack2 || m_nLastAI == kAINormalAttack) && m_nLastAI == aiAction)
    {
        return;
    }
    this->runAIAction(pHero, aiAction, nRunFrames);
    if (m_bLastAIRunSucc)
    {
        m_nLastAI = aiAction;
    }
}

bool MonsterSprite::skillAttack(int index)
{
    if (BattleLogic::canChangeUnitAction(m_nCurAction, kSkillBegin) && (m_nCurObstacleDirection & kObstacleDown) == kObstacleDown)
    {
        SkillInfoMap::const_iterator it = m_pUintInfo->skillMap.find(index);
            if (it != m_pUintInfo->skillMap.end())
            {
                const SkillInfo_t* skill = it->second;
                this->addSkillCombo(skill);
                return this->changeAction(kSkillBegin);
            }
    }
    return false;
}


void MonsterSprite::runAIAction(BattleUnitSprite* pHero, kAIState aiAction, int nRunFrames)
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
        case kAISkillAttack1://BOSS使用技能1
        {
            if (BattleLogic::canChangeUnitAction(m_nCurAction, kSkillBegin))
            {
                this->skillAttack(1);
                m_bLastAIRunSucc = true;
            }
            break;
        }
        case kAISkillAttack2://BOSS使用技能2
        {
            if (BattleLogic::canChangeUnitAction(m_nCurAction, kSkillBegin))
            {
                this->skillAttack(2);
                m_bLastAIRunSucc = true;
            }
            break;
        }
        case kAINormalAttack://普通攻击
        {
            if (BattleLogic::canChangeUnitAction(m_nCurAction, kAttackBegin))
            {
                this->attackBegin();
                m_bLastAIRunSucc = true;
            }
            break;
        }
        default:
        {
            return ;
        }
    }
}

void MonsterSprite::attackBegin()
{
    if (m_nCurAction >= kJumpUp && m_nCurAction <= kAirAttackOver)
    {
        if (BattleLogic::canChangeUnitAction(m_nCurAction, kAirAttackBegin))
        {
            m_nComboIdx = 0;
            m_pComboBuff.clear();
            this->addCombo(&m_pUintInfo->airAttack);
            this->changeAction(kAirAttackBegin);
        }
    }
    else if (BattleLogic::canChangeUnitAction(m_nCurAction, kAttackBegin) && (m_nCurAction != kAttackOver))
    {
        m_nComboIdx = 0;
        m_pComboBuff.clear();
        int attackindex = rand()%m_pUintInfo->attackVec.size();//小怪用第几个普攻攻击
        AttackInfo_t* pAttack = m_pUintInfo->attackVec.at(attackindex);
        m_pComboBuff.push_back(pAttack);
        m_nComboIdx++;
        this->changeAction(kAttackBegin);
    }
    else if (m_nCurAction == kAttackBegin || m_nCurAction == kAttackValid || m_nCurAction == kAttackOver)
    {
        this->addCombo(&m_pUintInfo->attackVec);
    }
}
