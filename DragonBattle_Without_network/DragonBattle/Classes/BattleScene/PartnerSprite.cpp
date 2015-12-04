//
//  PartnerSprite.cpp
//  DragonBattle
//
//  Created by 何 永翔 on 13-7-2.
//
//

#include "PartnerSprite.h"
#include "GameData.h"
#include "BattleLogic.h"
#include "GameManager.h"
#include "BattleMapUnits.h"
#include "AiManager.h"
#include "ThrowEngine.h"
USING_NS_CC;

PartnerSprite::PartnerSprite()
{
    m_bAIActivated = false;
    
    m_nLastNodeID = -1;
    m_nLastCalAI = kAIZero;
    m_nLastAI = kAIZero;
    m_nRunFrames = 0;
    m_bLastAIRunSucc = false;
    m_nAIRunFrames = 0;
    m_nBeHitCount = 0;
}

PartnerSprite::~PartnerSprite()
{

}

PartnerSprite* PartnerSprite::create(UnitBaseConf *info,int lv)
{
    PartnerSprite* pRet = new PartnerSprite();
    if (pRet && pRet->init(info,lv))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool PartnerSprite::init(UnitBaseConf *info,int lv)
{
    if (!BattleUnitSprite::init(info))
    {
        CCLOG("PartnerSprite::init error!");
        return false;
    }
    m_nSkillLV = lv;
    m_nLV = info->getLV();
    m_nHp = info->getMaxHP();
    m_fAngryTime = 999999;//弟子没有硬直
    m_nAIRunFrames = 10;
    return true;
}

void PartnerSprite::onDie(bool* pIsToRight)
{

}

void PartnerSprite::resetBeHitCount(float dt)
{
    this->unschedule(schedule_selector(PartnerSprite::resetBeHitCount));
    m_nBeHitCount = 0;
}

void PartnerSprite::onBeHit(bool bIsToRight,AttackInfo_t* pAttack)
{
    m_nBeHitCount++;
    this->unschedule(schedule_selector(PartnerSprite::resetBeHitCount));
    this->scheduleOnce(schedule_selector(PartnerSprite::resetBeHitCount), 1);
    if (m_nBeHitCount == 5)
    {
        this->changeAction(kBeHitFly, bIsToRight);
    }
    else
    {
        BattleUnitSprite::onBeHit(bIsToRight);
    }
}

// 怪物的坐标点 只用判断Y坐标是否错误
void PartnerSprite::update()
{
    if (m_nCurAction == kJumpUp || m_nCurAction == kJumpInAir)
    {
        m_movePt.x = this->isFlipX() ? -m_pUintInfo->fMoveSpeed : m_pUintInfo->fMoveSpeed;
    }
    BattleUnitSprite::update();
}

void PartnerSprite::runAIOnce(BattleUnitSprite* pHero, BattleUnitSprite* pMonster)
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
        this->runAIAction(pHero, pMonster, m_nLastCalAI, m_nRunFrames);
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
        return ;
    }
    
    BattleMapUnits* pMapUnitLayer = (BattleMapUnits*)GameManager::sharedInstance()->getBattleMapLayer();
    
    bool bLastAI = false;
    int nRunFrames = 0;
    kAIState aiAction = AiManager::getPartnerAIPolicy(this, pHero, pMonster, pMapUnitLayer, 7, m_nAttackDistance, bLastAI, nRunFrames, m_nLastNodeID, m_nAIRunFrames);
    
    if (bLastAI)
    {
        m_nLastCalAI = aiAction;
        m_nRunFrames = nRunFrames;
        return;
    }

    this->runAIAction(pHero, pMonster, aiAction, nRunFrames);
    if (m_bLastAIRunSucc)
    {
        m_nLastAI = aiAction;
    }
}

void PartnerSprite::runAIAction(BattleUnitSprite* pHero, BattleUnitSprite* pMonster, kAIState aiAction, int nRunFrames)
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
            if (pHero == NULL)
            {
                m_bLastAIRunSucc = true;
                return;
            }
            if (BattleLogic::canChangeUnitAction(m_nCurAction, kRunning)||m_nCurAction == kRunning) //行走
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
            if (BattleLogic::canChangeUnitAction(m_nCurAction, kRunning)||m_nCurAction == kRunning) //行走
            {
                if (pHero == NULL)
                {
                    m_bLastAIRunSucc = true;
                    return;
                }
                if (this->getPosition().x >= pHero->getPosition().x)
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
        case kAIMoveToScreen://向屏幕中间走
        {
            if (BattleLogic::canChangeUnitAction(m_nCurAction, kRunning)||m_nCurAction == kRunning) //行走
            {
                if (getScreenPosition().x>SCREEN_WIDTH/2)
                {
                    this->goBack();
                }
                else
                {
                    this->goForward();
                }
                m_nAIRunFrames = nRunFrames;
                m_bLastAIRunSucc = true;
            }
            break;
        }
        case kAINormalAttack://普通攻击技能攻击
        {
            if (fabs(getCenterPosition().x - pMonster->getCenterPosition().x)>m_nAttackDistance)//距离远 
            {
                if (this->skillAttack(1,getPosition().x > pMonster->getPosition().x))
                {
                    m_bLastAIRunSucc = true;
                }
            }
            else
            {
//           float fate = ((PartnerData*)gUserData->getRoleByID(m_unitConf.nUnitID))->relatePrecent(9);
                const PartnerConf_t *partnerConf = GameData::getPartnerConf(m_unitConf.nUnitID);
                const PartnerSkillConf_t* skillConf = GameData::getPartnerSkillConf(partnerConf->nSkillID);
                float fate = skillConf->bescper;
                int random = rand()%100;
                if (random <= fate*100)//释放技能
                {
                    if (this->skillAttack(1,getPosition().x > pMonster->getPosition().x))
                    {
                        m_bLastAIRunSucc = true;
                    }
                }
                else
                {
                    if (BattleLogic::canChangeUnitAction(m_nCurAction, kAttackBegin))
                    {
                        this->attackBegin();
                        m_bLastAIRunSucc = true;
                    }
                }
            }
            break;
        }
        case kAIFlashToHero://闪现到英雄身边
        {
            if (BattleLogic::canChangeUnitAction(m_nCurAction, kStandby) || m_nCurAction == kStandby)
            {
                if (pHero == NULL)
                {
                    m_bLastAIRunSucc = true;
                    return;
                }
                if (BattleLogic::canChangeUnitAction(m_nCurAction, kStandby))
                {
                    changeAction(kStandby);
                }
                CCPoint poi;
                poi.x = pHero->getPositionX();
                poi.y = getPositionY();
                BattleMapUnits* pMapUnitLayer = (BattleMapUnits*)GameManager::sharedInstance()->getBattleMapLayer();
                TrapSprite *trapSpr = pMapUnitLayer->m_trapContainer.getOverlappingSpr(poi.x,getUnitRect());
                if (trapSpr)
                {
                    if (!isFlipX())
                    {
                        poi.x = trapSpr->getPositionX() + trapSpr->getUnitRect().size.width + getUnitRect().size.width/2 + 50;
                    }
                    else
                    {
                        poi.x = trapSpr->getPositionX() - trapSpr->getUnitRect().size.width - getUnitRect().size.width/2 - 50;
                    }
                }
                setPosition(poi);
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

bool PartnerSprite::skillAttack(int index,bool bFlipx)
{
    if (BattleLogic::canChangeUnitAction(m_nCurAction, kSkillBegin) && (m_nCurObstacleDirection & kObstacleDown) == kObstacleDown)
    {        
        const SkillInfo_t* skill = m_pUintInfo->skillMap.at(index);
        if(addSkillCombo(skill))
        {
            const PartnerConf_t *partnerConf = GameData::getPartnerConf(m_unitConf.nUnitID);
            m_nAttackSkill = MERGE_SKILL_ID_LV(partnerConf->nSkillID,m_nSkillLV);
            setFlipX(bFlipx);
            return this->changeAction(kSkillBegin);
        }
    }
    return false;
}

void PartnerSprite::attackBegin()
{
    if (BattleLogic::canChangeUnitAction(m_nCurAction, kAttackBegin) && (m_nCurAction != kAttackOver))
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
/*
void PartnerSprite::throwObject()
{
    if(getCurAction() == kSkillValid && m_nSkill!=0)
    {
        ThrowEngine* pEngine = ThrowEngine::create(this);
        const PartnerSkillConf_t* skillConf = GameData::getPartnerSkillConf(GET_SKILL_ID(m_nSkill));
        std::vector<uint16_t>  throwVec = skillConf->throwVec;
        pEngine->setThrowVec(&throwVec);
        pEngine->setThrowMode((kThrowMode)skillConf->nThrowSequence);
        pEngine->setSkillConfID(m_nSkill);
        pEngine->throwObject();
    }
}
*/