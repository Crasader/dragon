//
//  HeroSprite.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-19.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "HeroSprite.h"
#include "BattleUnitSprite.h"
#include "ResourcesManager.h"
#include "BattleLayer.h"
#include "GameScene.h"
#include "GameManager.h"
#include "BattleMapUnits.h"
#include "BattleLogic.h"
#include "AiManager.h"
#include "CommonUtility.h"
USING_NS_CC;
HeroSprite::HeroSprite()
{
    m_bAttackBtnDown = false;
    m_nBeHitIndex = 0;
    m_nBeHitCount = 0;
    m_MpTime = 0;
    m_bBeHitProtect = true;
    
    m_nLastCalAI = kAIZero;
    m_nLastNodeID = -1;
    m_nLastAI = kAIZero;
    m_bLastAIRunSucc = false;
    m_nAIRunFrames = 0;
    
    m_nMaxCombo = 0;
    m_nCombo = 0;
    m_lComboTime = 0;
}

HeroSprite::~HeroSprite()
{
    CC_SAFE_RELEASE_NULL(m_pRoll);
}

void HeroSprite::onDie(bool* pIsToRight)
{
    this->unscheduleAllSelectors();
}

void HeroSprite::resetBeHitCount(float dt)
{
    this->unschedule(schedule_selector(HeroSprite::resetBeHitCount));
    m_nBeHitCount = 0;
}

void HeroSprite::onBeHit(bool bIsToRight,AttackInfo_t* pAttack)
{
    if (getBeHitProtect())
    {
        m_nBeHitCount++;
        this->unschedule(schedule_selector(HeroSprite::resetBeHitCount));
        this->scheduleOnce(schedule_selector(HeroSprite::resetBeHitCount), 1);
        if (m_nBeHitCount == 5)
        {
            if (m_nCurAction != kBeHitFly)
            {
                this->changeAction(kBeHitFly, bIsToRight);
            }
        }
        else
        {
            BattleUnitSprite::onBeHit(bIsToRight);
        }
    }
    else
    {
        BattleUnitSprite::onBeHit(bIsToRight);
    }
}

HeroSprite* HeroSprite::create(UnitBaseConf* pUnitConf)
{
    HeroSprite* pRet = new HeroSprite();
    
    if (pRet && pRet->init(pUnitConf))
    {
        pRet->autorelease();
        return pRet;
    }
    
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool HeroSprite::init(UnitBaseConf* pUnitConf)
{
    if (!BattleUnitSprite::init(pUnitConf))
    {
        CCLOG("HeroSprite::init error!");
        return false;
    }
    m_pRoll = UnitResource::sharedInstance()->createAnimate(m_unitConf.nResourceID, kAnimRoll);
    m_pRoll->retain();
    m_nHp = pUnitConf->nLife;
    memcpy(m_nSkill, &gUserData->m_heroData->skill, sizeof(m_nSkill));
    memset(m_nSkillCD, 0x0, sizeof(m_nSkillCD));
    m_nMaxMp = 100*(gUserData->getParnterTalentByID(9) + 1);
    m_nMp = m_nMaxMp;
    m_nAttackDistance = getAttackDistance()*1.2f;
    schedule(schedule_selector(HeroSprite::heroUpdate), 1.0f);
    return true;
}

void HeroSprite::heroUpdate(float dt)
{
    for (int i = 0; i<4; i++)
    {
        if (m_nSkillCD[i]>0)
        {
            m_nSkillCD[i] = m_nSkillCD[i] - 1;
        }
        else
        {
            m_nSkillCD[i] = 0;
        }
    }
    m_MpTime++;
    if (m_MpTime>= 10)//10秒
    {
        addMp(5);
        m_MpTime = 0;
    }
}

// 英雄每帧更新，先更新坐标点，是否合理在地图中设置
void HeroSprite::update()
{
    BattleUnitSprite::update();
}

void HeroSprite::resetStandby()
{
    this->changeAction(kStandby);
    if (m_bAttackBtnDown)
    {
        this->attackBegin();
    }
}

void HeroSprite::addcombo()
{
    if (m_bAttackBtnDown && m_nCurAction == kAttackOver && (m_nCurObstacleDirection & kObstacleDown) == kObstacleDown)
    {
        this->addCombo(&m_pUintInfo->attackVec);
    }
}

void HeroSprite::statueAttackValid( )
{
    if (m_pCurAttack->nSharkType == 0 && kAirAttackValid == m_nCurAction)
    {
        //屏幕震动
        BattleMapUnits* pMapUnitLayer = (BattleMapUnits*)GameManager::sharedInstance()->getBattleMapLayer();
        if (pMapUnitLayer)
        {
            pMapUnitLayer->mapShark();
        }
    }
    else if (kSkillValid == m_nCurAction)
    {
        CCRepeat *repeat = CCRepeat::create(CCSequence::create(CCScaleTo::create(0.05, 1),CCScaleTo::create(0.05, 1.01),NULL),3);
        BattleMapUnits* pLayer = (BattleMapUnits*)GameManager::sharedInstance()->getBattleMapLayer();
        CCPoint heropoi = pLayer->convertToWorldSpace(getPosition());
        pLayer->getParent()->runAction(CCSequence::create(
                                                         CCSpawn::create(CCMoveBy::create(0.05, ccp(-0.01*heropoi.x,0)),CCScaleTo::create(0.05, 1.01),NULL),repeat,
                                                         CCSpawn::create(CCMoveBy::create(0.05, ccp(0.01*heropoi.x,0)),CCScaleTo::create(0.05, 1),NULL),NULL));
        
    }
}

int  HeroSprite::getMp()
{
    return m_nMp;
}

void HeroSprite::attackValidRole()
{
    if(getCurAction() == kAttackValid)
    {
        addMp(2.0f);
    }
    SetComboInfo();
    if (m_nCombo > m_nMaxCombo)
    {
        m_nMaxCombo = m_nCombo;
    }
}

void HeroSprite::SetComboInfo()
{
    long t = CommonUtility::getCurrentMillisecond();
    if (t - m_lComboTime > 2000)
    {
        m_nCombo = 0;
    }
    m_lComboTime = t;
    m_nCombo += 1;
}

void HeroSprite::unAttackValidRole()
{
    addMp(4.0f);
}

void HeroSprite::setMp(float mp)
{
    m_nMp = mp;
}

void HeroSprite::addMp(float add)
{
    if (m_nMp == getMaxMp() && add>0)
    {
        return;
    }
    if(m_nMp + add>0)
    {
        if (m_nMp+add>getMaxMp())
        {
            m_nMp = getMaxMp();
        }
        else
        {
            m_nMp+= add;
        }
    }
    else
    {
        m_nMp = 0;
    }
    CCNotificationCenter::sharedNotificationCenter()->postNotification(MP_CHANGE_NOTE);
}

void HeroSprite::runAIOnce(BattleUnitSprite* pMonster)
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
        //如果仍在被击打状态，则m_nLastCalAI延后执行
        if (this->getCurAction() == kBeHit)
        {
            return;
        }
        //两次攻击不能接在一起
        if ((m_nLastAI == kAISkillAttack1 || m_nLastAI == kAISkillAttack2 || m_nLastAI == kAINormalAttack) && m_nLastAI == m_nLastCalAI)
        {
            return;
        }
        this->runAIAction(pMonster, m_nLastCalAI, m_nRunFrames);
        if (m_bLastAIRunSucc)
        {
            m_nLastAI = m_nLastCalAI;
            m_nLastCalAI = kAIZero;
        }
    }
    //由于kBeHit是瞬时状态，所以，其不受m_nAIRunFrames影响，需立即判断
    if (m_nAIRunFrames > 0 && this->getCurAction() != kBeHit)
    {
        m_nAIRunFrames--;
        return ;
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
    kAIState aiAction = AiManager::getHeroAIPolicy(this, pMonster, pMapUnitLayer,45, m_nAttackDistance, bLastAI, nRunFrames, m_nLastNodeID, m_nAIRunFrames);
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
        m_nLastCalAI = aiAction;
        m_nRunFrames = nRunFrames;
        return;
    }
    this->runAIAction(pMonster, aiAction, nRunFrames);
    if (m_bLastAIRunSucc)
    {
        m_nLastAI = aiAction;
    }
}

void HeroSprite::runAIAction(BattleUnitSprite* pMonster, kAIState aiAction, int nRunFrames)
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
        case kAIMoveToMonster://靠近怪物
        {
            if (BattleLogic::canChangeUnitAction(m_nCurAction, kRunning)||m_nCurAction == kRunning) //行走
            {
                if (pMonster == NULL)
                {
                    m_bLastAIRunSucc = true;
                    return;
                }
                if (getPosition().x < pMonster->getPosition().x)
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
        case kAIMoveForward://向前走
        {
            if (BattleLogic::canChangeUnitAction(m_nCurAction, kRunning)) //行走
            {
                this->goForward();
                m_nAIRunFrames = nRunFrames;
                m_bLastAIRunSucc = true;
            }
            break;
        }
        case kAINormalAttack://普通攻击技能攻击
        {
            int nrand = rand()%2;
            if (nrand == 0)
            {
                if (!aiSkillAttack(getPosition().x > pMonster->getPosition().x))
                {
                    if (getPosition().x <= pMonster->getPosition().x)
                    {
                        this->setFlipX(false);
                    }
                    else
                    {
                        this->setFlipX(true);
                    }
                    aiAttackBegin();
                }
            }
            else
            {
                if (getPosition().x <= pMonster->getPosition().x)
                {
                    this->setFlipX(false);
                }
                else
                {
                    this->setFlipX(true);
                }
                aiAttackBegin();
            }
            m_bLastAIRunSucc = true;
            break;
        }
        default:
        {
            return;
        }
    }
}

void HeroSprite::aiAttackBegin()
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
        for (int i = 0; i<m_pUintInfo->attackVec.size(); i++)
        {
            AttackInfo_t* pAttack = m_pUintInfo->attackVec.at(i);
            m_pComboBuff.push_back(pAttack);
            m_nComboIdx++;
        }
        this->changeAction(kAttackBegin);
    }
    else if (m_nCurAction == kAttackBegin || m_nCurAction == kAttackValid || m_nCurAction == kAttackOver)
    {
        this->addCombo(&m_pUintInfo->attackVec);
    }
}

bool HeroSprite::skillAttack(int index)
{
    if (BattleLogic::canChangeUnitAction(m_nCurAction, kSkillBegin) && (m_nCurObstacleDirection & kObstacleDown) == kObstacleDown)
    {
        uint32_t nSkill = m_nSkill[index];
        const SkillLevel_t * pskilllv= GameData::getSkillConf(GET_SKILL_ID(nSkill),GET_SKILL_LEVEL(nSkill));
        m_nAttackSkill = nSkill;
        const SkillInfo_t* skill = m_pUintInfo->skillMap.at(GET_SKILL_ID(m_nAttackSkill));
        if(m_nSkillCD[index]<=0 && addSkillCombo(skill) && pskilllv->skillLvBase.nCostEnergy<=getMp())
        {
            m_nSkillCD[index] = pskilllv->skillLvBase.fCDTime;
            CCNotificationCenter::sharedNotificationCenter()->postNotification(AI_SKILL_NOTE, CCInteger::create((index)));
            addMp(-pskilllv->skillLvBase.nCostEnergy);
            return this->changeAction(kSkillBegin);
        }
    }
    return false;
}

bool HeroSprite::aiSkillAttack(bool bFlipx)
{
    vector<int> skillvec;
    for (int i = 0; i<4; i++)
    {
        if (m_nSkill[i]!= 0 && m_nSkillCD[i] == 0)
        {
            skillvec.push_back(i);
        }
    }

    int count = skillvec.size();
    if (count == 0)
    {
        return false;
    }
    return skillAttack(skillvec.at(rand()%count));
}

void HeroSprite::addHp(int hp)
{
    BattleUnitSprite::addHp(hp);
    if ( (((float)getHp())/getUnitBaseConf()->getMaxHP()) < 0.25 &&m_bAIActivated)//托管喝血
    {
        CCNotificationCenter::sharedNotificationCenter()->postNotification(AI_EAT_MED_NOTE);
    }
}