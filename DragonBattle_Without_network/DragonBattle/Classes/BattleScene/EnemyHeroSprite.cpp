//
//  EnemyHeroSprite.cpp
//  DragonBattle
//
//  Created by zeng xiaolong on 13-10-26.
//
//

#include "EnemyHeroSprite.h"
#include "GameData.h"
#include "BattleLogic.h"
#include "GameManager.h"
#include "BattleMapUnits.h"
#include "AiManager.h"
#define BUFFEFFECTTAG 508
USING_NS_CC;

EnemyHeroSprite::EnemyHeroSprite()
{
    m_bAIActivated = false;
    
    m_nLastNodeID = -1;
    m_nLastAI = kAIZero;
    m_bLastAIRunSucc = false;
    m_nAIRunFrames = 0;
    m_nContinueSkill = 0;
}

EnemyHeroSprite::~EnemyHeroSprite()
{
    
}

EnemyHeroSprite* EnemyHeroSprite::create(UnitBaseConf *info,uint32_t* skill)
{
    EnemyHeroSprite* pRet = new EnemyHeroSprite();
    if (pRet && pRet->init(info,skill))
    {
        pRet->autorelease();
        return pRet;
    }
    
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool EnemyHeroSprite::init(UnitBaseConf *info,uint32_t* skill)
{
    if (!BattleUnitSprite::init(info))
    {
        CCLOG("EnemyHeroSprite::init error!");
        return false;
    }
    
    memcpy(&m_nSkill,skill, sizeof(m_nSkill));
    m_pRoll = UnitResource::sharedInstance()->createAnimate(m_unitConf.nResourceID, kAnimRoll);
    m_pRoll->retain();

    m_nLV = info->getLV();
    m_nHp = info->getMaxHP();
    
    m_nAIRunFrames = 10;
    return true;
}

// 怪物的坐标点 只用判断Y坐标是否错误
void EnemyHeroSprite::update()
{
    BattleUnitSprite::update();
    if (m_nCurAction == kJumpInAir && m_movePt.y <= 0)
    {
        if (BattleLogic::canChangeUnitAction(m_nCurAction, kAirAttackBegin))
        {
            attackBegin();
        }
    }
}

void EnemyHeroSprite::runAIOnce(BattleUnitSprite* pHero)
{
    if (m_nHp <= 0)
    {
        return;
    }
    
    //AI是否激活
    if (!m_bAIActivated)
    {
        return;
    }
    
    //由于kBeHit是瞬时状态，所以，其不受m_nAIRunFrames影响，需立即判断
    if (this->getCurAction() == kBeHit)
    {
        m_nAIRunFrames = 0;
        
    }
    if (IS_ATTACK_ACTION(m_nCurAction)||IS_SKILL_ACTION(m_nCurAction))
    {
        if (getPosition().x < pHero->getPosition().x)
        {
            this->goForward();
        }
        else
        {
            this->goBack();
        }
    }
    // 当前行为是否完成
    if (IS_ATTACK_ACTION(m_nCurAction) || IS_AIRATTACK_ACTION(m_nCurAction) || IS_SKILL_ACTION(m_nCurAction) ||
        IS_JUMPING_ACTION(m_nCurAction) || m_nCurAction == kBeHit || m_nCurAction == kGetUp || m_nCurAction == kRoll)
    {
        m_nAIRunFrames = 0;
        return ;
    }
    
    BattleMapUnits* pMapUnitLayer = (BattleMapUnits*)GameManager::sharedInstance()->getBattleMapLayer();
    bool bLastAI = false;
    int nRunFrames = 0;
    kAIState aiAction = AiManager::getEnemyHeroAIPolicy(this, pHero, pMapUnitLayer, m_unitConf.nAIID,m_nAttackDistance,m_nContinueSkill, bLastAI, nRunFrames, m_nLastNodeID, m_nAIRunFrames);
    if (nRunFrames == 0)
    {
        m_nAIRunFrames--;
        if (m_nAIRunFrames < 0)
        {
            m_nAIRunFrames = 0;
        }
        return;
    }
    if (bLastAI)
    {
        return ;
    }
    this->runAIAction(pHero, aiAction, nRunFrames);
    if (m_bLastAIRunSucc)
    {
        m_nLastAI = aiAction;
    }
    else//执行ai失败 下次继续执行ai
    {
        m_nAIRunFrames = 0;
    }
}
void EnemyHeroSprite::runAIAction(BattleUnitSprite* pHero, kAIState aiAction, int nRunFrames)
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
        case kAISkillAttack1://BOSS使用技能1
        {
            if (this->aiSkillAttack(getPosition().x > pHero->getPosition().x))
            {
                m_nContinueSkill++;
                m_bLastAIRunSucc = true;
            }
            break;
        }
        case kAISkillAttack2://BOSS使用技能2
        {
            if (this->aiSkillAttack(getPosition().x > pHero->getPosition().x))
            {
                m_nContinueSkill++;
                m_bLastAIRunSucc = true;
            }
            break;
        }
        case kAIJumpForwardToHero://跳向英雄
        {
            /*if(this->moveJump())
            {
                m_nContinueSkill = 0;
                m_nAIRunFrames = nRunFrames;
                m_bLastAIRunSucc = true;
            }
            break;*/
        }
        case kAINormalAttack://普通攻击
        {
            if (BattleLogic::canChangeUnitAction(m_nCurAction, kAttackBegin))
            {
                m_nContinueSkill = 0;
                this->attackBegin();
                m_bLastAIRunSucc = true;
            }
            break;
        }
        case kAIFlashToHero:
        case kAIFlashToHeroTwice:
        {
            if (BattleLogic::canChangeUnitAction(m_nCurAction, kRoll)) //闪现
            {
                int direct = rand()%2;
                setFlipX(direct);
                if (getPosition().x < pHero->getPosition().x)
                {
                    this->setFlipX(false);
                }
                else
                {
                    this->setFlipX(true);
                }
                
                this->roll();
                m_nAIRunFrames = nRunFrames;
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

bool EnemyHeroSprite::aiSkillAttack(bool bFlipx)
{
    int count = m_nSkill[3]!=0?4:m_nSkill[2]!=0?3:m_nSkill[1]!=0?2:m_nSkill[0]!=0?1:0;
    if (count == 0)
    {
        return false;
    }
    uint32_t nSkill = m_nSkill[count%rand()];
    if (BattleLogic::canChangeUnitAction(m_nCurAction, kSkillBegin) && (m_nCurObstacleDirection & kObstacleDown) == kObstacleDown)
    {
        const SkillConf_t* pSkill = GameData::getSkillConf(GET_SKILL_ID(nSkill));
        if (pSkill != NULL && pSkill->skillBase.nSkillID%100<=7)//攻击技能
        {
            m_nAttackSkill = nSkill;
            const SkillInfo_t* skill = m_pUintInfo->skillMap.at(GET_SKILL_ID(m_nAttackSkill));
            if(addSkillCombo(skill))
            {
                setFlipX(bFlipx);
                return this->changeAction(kSkillBegin);
            }
        }
    }
    return false;
}

void EnemyHeroSprite::statueAttackBegin(bool isToRight)
{
    BattleUnitSprite::statueAttackBegin(isToRight);
    if (m_nCurAction == kAirAttackBegin)
    {
        if (!isFlipX())
        {
            this->goForward();
        }
        else
        {
            this->goBack();
        }
    }
}

void EnemyHeroSprite::statueAttackValid()
{
    if (m_nCurAction == kAirAttackValid)
    {
        if (!isFlipX())
        {
            this->goForward();
        }
        else
        {
            this->goBack();
        }
    }
}

void EnemyHeroSprite::resetStandby()
{
    if (m_nLastAI == kAIFlashToHeroTwice)
    {
        this->changeAction(kStandby);
        int direct = rand()%2;
        setFlipX(direct);
        this->roll();
        m_nAIRunFrames = 0;
        m_bLastAIRunSucc = true;
        m_nLastAI = kAIFlashToHero;
        return;
    }
    float moveAddPty = m_moveAddPt.y;
    bool fromBehit = false;
    if (m_nCurAction == kBeHit)
    {
        moveAddPty = m_moveAddPt.y;
    }

    this->changeAction(kStandby);
    if (fromBehit)
    {
        m_moveAddPt.y = moveAddPty;
    }
}

void EnemyHeroSprite::attackBegin()
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

