//
//  BattleUnitSprite.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-18.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "BattleUnitSprite.h"
#include "BattleLogic.h"
#include "GameScene.h"
#include "GameManager.h"
#include "BattleLayer.h"
#include "BattleMapUnits.h"
#include "MapCoord.h"
#include "GameData.h"
#include "ThrowEngine.h"
#include "AttackedEffectEngine.h"
#include "UserData.h"
#include "SimpleAudioEngine.h"
#include "AudioResource.h"
#include "CalculateDamage.h"
USING_NS_CC;
#define EFFECTSPRTAG 201
#define THEAIRMIN (0.1f)
static const ccColor3B ccColorOriginal = {255,255,255};
static const ccColor3B ccColorAngry = {255,130,255};

BattleUnitSprite::BattleUnitSprite()
{
    m_nAttackDistance = 0;
    m_UnitSprite = NULL;
    m_pStand = NULL;
    m_pGoForward = NULL;
    m_pRoll = NULL;
    m_pGetUp = NULL;
    m_pJumpUp = NULL;
    m_pJumpDown = NULL;
    m_pSkillOverDown = NULL;
    m_pJumping = NULL;
    m_pUnderAttacks = NULL;
    m_pComboBuff.clear();
    m_nComboIdx = 0;
    m_pCurHit.clear();
    
    m_pCurAttack = NULL;
    
    m_nCurrentBeHitCount = 0;
    nRecoverHpTime = 0;
    m_fSpeedX = 0;
    m_fSpeedY = 0;
    m_fAddSpeedY = 0;
    m_fAddSpeedX = 0;
    m_movePt.x = 0;
    m_movePt.y = 0;
    m_moveAddPt.x = 0;
    m_moveAddPt.y = JUMP_ACCELERATED_SPEED_Y;
    m_forceMoveBy = CCPointZero;
    m_nBeHitCount = 0;
    
    m_bAIActivated = false;
    m_fAngryTime = 0;
    m_nFlashTimes = 0;
    m_nBuffSkill = 0;
    m_nBuffTime = 0;
    m_nAttackSkill = 0;
    m_nCurDirection = kNone;
    m_nCurObstacleDirection = kObstacleNone;
    m_nHurtHp = 0;
}

BattleUnitSprite::~BattleUnitSprite()
{
    CC_SAFE_RELEASE_NULL(m_pGoForward);
    CC_SAFE_RELEASE_NULL(m_pGetUp);
    CC_SAFE_RELEASE_NULL(m_pSkillOverDown);
    CC_SAFE_RELEASE_NULL(m_pStand);
    CC_SAFE_RELEASE_NULL(m_pUnderAttacks);
    CC_SAFE_RELEASE_NULL(m_pRoll);
    CC_SAFE_RELEASE_NULL(m_pJumpUp);
    CC_SAFE_RELEASE_NULL(m_pJumpDown);
    CC_SAFE_RELEASE_NULL(m_pJumping);
    m_pComboBuff.clear();
    m_pCurHit.clear();
    m_pCurAttack = NULL;
}

bool BattleUnitSprite::init(UnitBaseConf *info)
{
    m_unitConf = *info;
    if (!info)
    {
        return false;
    }
    if (!CCNode::init())
    {
        return false;
    }
    m_pUintInfo = ResourcesManager::sharedInstance()->getUnitResConf(m_unitConf.nResourceID);
    m_UnitSprite = CCSprite::createWithSpriteFrameName(m_pUintInfo->strRoleFile.data());
    if (!m_UnitSprite)
    {
        return false;
    }
    m_UnitSprite->setScale(m_pUintInfo->fScale);
    setContentSize(ccp(500,500));
    setAnchorPoint(ccp(0.5f,0.5f));
    m_UnitSprite->setPosition(ccp(getContentSize().width/2, getContentSize().height/2));
    addChild(m_UnitSprite);
    
    m_pStand = CCRepeatForever::create(UnitResource::sharedInstance()->createAnimate(m_unitConf.nResourceID, kAnimStandBy));
    m_pStand->retain();
    
    m_pGoForward = CCRepeatForever::create(UnitResource::sharedInstance()->createAnimate(m_unitConf.nResourceID, kAnimRun));
	m_pGoForward->retain();
    
    m_pJumpUp = UnitResource::sharedInstance()->createAnimate(m_unitConf.nResourceID, kAnimJumpUp);
    m_pJumpUp->retain();
    m_pJumping = UnitResource::sharedInstance()->createAnimate(m_unitConf.nResourceID, kAnimJumping);
    m_pJumping->retain();
    m_pJumpDown = UnitResource::sharedInstance()->createAnimate(m_unitConf.nResourceID, kAnimJumpDwon);
    m_pJumpDown->retain();

    m_pGetUp = UnitResource::sharedInstance()->createAnimate(m_unitConf.nResourceID, kAnimGetUp);
    m_pGetUp->retain();

    m_pSkillOverDown = UnitResource::sharedInstance()->createAnimate(m_unitConf.nResourceID, kAnimJumping);
    m_pSkillOverDown->retain();
    
    m_pUnderAttacks = CCArray::create();
    for (int i = 0; i < m_pUintInfo->underAttacks.size(); i++)
    {
        m_pUnderAttacks->addObject(UnitResource::sharedInstance()->createAnimate(m_unitConf.nResourceID, m_pUintInfo->underAttacks[i]));
    }
    m_pUnderAttacks->retain();
    
    m_nCurAction = kStandby;
    m_UnitSprite->runAction(m_pStand);
    
    if (this->getUnitType() != kBoss && getUnitType() != kEnemyHero)
    {
        m_pSprShadow = CCSprite::create("BattleUI/shadow.png");
    }
    else
    {
        m_pSprShadow = CCSprite::create("BattleUI/boss_shadow.png");
        CCSprite *spr = CCSprite::create("BattleUI/boss_shadow.png");
        spr->setPosition(ccp(m_pSprShadow->getContentSize().width/2, m_pSprShadow->getContentSize().height/2));
        spr->runAction(CCRepeatForever::create(CCSequence::create(
        CCSpawn::create(CCScaleTo::create(0.3f, 1.2f),CCFadeTo::create(0.3f, 128),NULL),
        CCSpawn::create(CCScaleTo::create(0.3f, 1.0f),CCFadeTo::create(0.3f, 255),NULL),NULL)));
        m_pSprShadow->addChild(spr);
    }
    float a = getContentSize().height/2 +m_pUintInfo->offPos.y-getUnitRect().size.height/2;
    m_pSprShadow->setScale(1.0f);
    m_pSprShadow->setPosition(ccp(getContentSize().width/2,a));
    this->addChild(m_pSprShadow,-1);
    
    m_pBeHitRectLayer = CCLayerColor::create(ccc4(255, 0, 0, 128), 0, 0);
    m_pBeHitRectLayer->setAnchorPoint(ccp(0, 0));
    CCRect rect = getLocalUnitRect();
    m_pBeHitRectLayer->setContentSize(rect.size);
    m_pBeHitRectLayer->setPosition(ccp(rect.origin.x, rect.origin.y));
    
    this->addChild(m_pBeHitRectLayer);
    m_pBeHitRectLayer->setVisible(true);
    
    m_pAttackRectLayer = CCLayerColor::create(ccc4(64, 64, 64, 128), 0, 0);
    m_pAttackRectLayer->setAnchorPoint(ccp(0,0));
    m_pAttackRectLayer->setVisible(true);
    this->addChild(m_pAttackRectLayer, 10, 100);
    
    m_pSelfRectLayer = CCLayerColor::create(ccc4(0, 100, 100, 128), this->getContentSize().width, this->getContentSize().height);
    m_pSelfRectLayer->setPosition(ccp(0,0));
    m_pSelfRectLayer->setAnchorPoint(ccp(0,0));
    m_pSelfRectLayer->setVisible(false);
    this->addChild(m_pSelfRectLayer, 10, 101);
    
    setAttacRectVisible(false);
    schedule(schedule_selector(BattleUnitSprite::updateRecoverHp), 20);
    m_nAttackDistance = 1.2*getAttackDistance();
    return true;
}

CCPoint BattleUnitSprite::getUnitOffPos()
{
    return ccp(m_pUintInfo->fScale*m_pUintInfo->offPos.x,m_pUintInfo->fScale*m_pUintInfo->offPos.y);
}

CCSize BattleUnitSprite::getUnitSize()
{
    return CCSize(m_pUintInfo->fScale*m_pUintInfo->contentSize.width, m_pUintInfo->fScale*m_pUintInfo->contentSize.height);
}
// 受击区域，相对于sprite,(角色图片中心点与受击区域中心点)
CCRect BattleUnitSprite::getLocalUnitRect()
{
    return CCRect(this->getContentSize().width / 2 + getUnitOffPos().x - getUnitSize().width/ 2,
                  this->getContentSize().height / 2 + getUnitOffPos().y - getUnitSize().height/ 2,
                  getUnitSize().width,getUnitSize().height);
}

// 受击区域:对于地图层来说的区域
CCRect BattleUnitSprite::getUnitRect()
{
    CCRect rect = getLocalUnitRect();
    rect.origin.x += getPosition().x - this->getContentSize().width / 2;
    rect.origin.y += getPosition().y - this->getContentSize().height / 2;
    return rect;
}

// 得到图片中心点在屏幕上的坐标
CCPoint BattleUnitSprite::getScreenPosition()
{
    return MapCoord::layerToScreen(getPosition());
}

// 得到角色中心点的坐标,相对于layer
CCPoint BattleUnitSprite::getCenterPosition()
{
    return CCPoint(getPosition().x + getUnitOffPos().x, getPosition().y + getUnitOffPos().y);
}

CCPoint BattleUnitSprite::getFootPosition()
{
    return CCPoint(ccp(getContentSize().width/2 + getUnitOffPos().x,getContentSize().height/2+getUnitOffPos().y - getUnitSize().height/2));
}

// 根据给定的角色中心点设置图片的位置
void BattleUnitSprite::setCenterPosition(CCPoint pos)
{
    CCPoint pt;
    pt.x = pos.x - getUnitOffPos().x;
    pt.y = pos.y - getUnitOffPos().y + getUnitRect().size.height/2;
    this->setPosition(pt);
}

// 当前招式的攻击区域，相对sprite
CCRect BattleUnitSprite::getLocalAttackRect()
{
    CCPoint localPt;
    if (!m_UnitSprite->isFlipX())
    {
        localPt.x = this->getContentSize().width / 2 + getUnitOffPos().x + m_pUintInfo->fScale*m_pCurAttack->rcAttackRect.origin.x;
    }
    else
    {
        localPt.x = this->getContentSize().width / 2 + getUnitOffPos().x - m_pUintInfo->fScale*m_pCurAttack->rcAttackRect.origin.x - m_pUintInfo->fScale*m_pCurAttack->rcAttackRect.size.width;
    }
    localPt.y = this->getContentSize().height / 2 +  getUnitOffPos().y + m_pUintInfo->fScale*m_pCurAttack->rcAttackRect.origin.y;
    return CCRect(localPt.x, localPt.y, m_pUintInfo->fScale*m_pCurAttack->rcAttackRect.size.width, m_pUintInfo->fScale*m_pCurAttack->rcAttackRect.size.height);
}

// 当前招式的攻击区域，相对layer
CCRect BattleUnitSprite::getAttackRect()
{
    CCRect rect = this->getLocalAttackRect();
    rect.origin.x = rect.origin.x + getPosition().x - this->getContentSize().width / 2;
    rect.origin.y = rect.origin.y + getPosition().y - this->getContentSize().height / 2;
    return rect;
}

bool BattleUnitSprite::isFlipX()
{
    return m_UnitSprite->isFlipX();
}

void BattleUnitSprite::setFlipX(bool bFlipX)
{
    m_UnitSprite->setFlipX(bFlipX);
    CCRect rect = getLocalUnitRect();
    m_pBeHitRectLayer->setPosition(rect.origin);
}

// 是否被攻击
bool BattleUnitSprite::hasUnderAttacked(BattleUnitSprite* pUnit)
{
    
    return  m_pCurHit.find(pUnit) != m_pCurHit.end();
}

void BattleUnitSprite::clearUnserAttacked(BattleUnitSprite* pUnit)
{
    m_pCurHit.erase(pUnit);
}

bool BattleUnitSprite::changeAction(kRoleActionType action, kRoleDirectionType direction)
{
    if (direction == kNone)
        return changeAction(action);
    else
        return changeAction(action, direction == kRight);
}

bool BattleUnitSprite::changeAction(kRoleActionType action)
{
	return changeAction(action, !m_UnitSprite->isFlipX());
}

bool BattleUnitSprite::changeAction(kRoleActionType action, bool isToRight)
{
    CCAssert(BattleLogic::canChangeUnitAction(m_nCurAction, action), "Change action error");
	m_UnitSprite->stopAllActions();
    m_UnitSprite->setPosition(ccp(getContentSize().width/2, getContentSize().height/2));
    //CCLOG("change Action: roleID[%d] curAction[%d] changeAction[%d]", m_unitConf->nUnitType, m_nCurAction, action);
    
    //kRoleActionType preAction = m_nCurAction;
    
	m_nCurAction = action;
	this->setFlipX(!isToRight);
    m_moveAddPt.y = JUMP_ACCELERATED_SPEED_Y;
	switch (action) 
	{
		case kRunning:
		{
            m_movePt.x = isToRight ? m_pUintInfo->fMoveSpeed : (-m_pUintInfo->fMoveSpeed);
            m_movePt.y = 0;
            m_moveAddPt.x = 0;
			m_UnitSprite->runAction(m_pGoForward);
		}
			break;
		case kStandby:
		{
			m_movePt.x = 0;
			m_movePt.y = 0;
			m_moveAddPt.x = 0;
			m_UnitSprite->runAction(m_pStand);
		}
			break;
        case kGetUp:
        {
			m_movePt.x = 0;
			m_movePt.y = 0;
			m_moveAddPt.x = 0;
            
            m_UnitSprite->runAction(CCSequence::create(m_pGetUp, CCCallFunc::create(this,callfunc_selector(BattleUnitSprite::getupEnd)), NULL));
        }
            break;
		case kJumpUp:
		{
			m_movePt.x = 0;
			m_movePt.y = m_pUintInfo->fJumpSpeed;
            m_moveAddPt.x = 0;
            
			m_UnitSprite->runAction(CCSequence::create(m_pJumpUp, CCCallFunc::create(this,callfunc_selector(BattleUnitSprite::jumpUpDone)), NULL));
		}
			break;
        case kSkillOverDown:
        {
			m_movePt.x = 0;
			m_movePt.y = 0;
            m_moveAddPt.x = 0;
            m_UnitSprite->runAction(m_pSkillOverDown);
        }
            break;
		case kJumpInAir:
		{
			m_UnitSprite->runAction(m_pJumping);
		}
			break;
		case kJumpDown:
		{
			m_movePt.x = 0;
			m_movePt.y = 0;
            m_moveAddPt.x = 0;
			m_UnitSprite->runAction(CCSequence::create(m_pJumpDown, CCCallFunc::create(this,callfunc_selector(BattleUnitSprite::resetStandby)), NULL));
		}
			break;
        case kRoll:
        {
            m_movePt.x = isToRight ? m_pUintInfo->fMoveSpeed * 2 : (-m_pUintInfo->fMoveSpeed * 2);
            m_movePt.y = 0;
            m_moveAddPt.x = 0;
			m_UnitSprite->runAction(CCSequence::create(m_pRoll, CCCallFunc::create(this,callfunc_selector(BattleUnitSprite::resetStandby)), NULL));
        }
            break;
        case kAttackBegin:
        case kSkillBegin:
        case kAirAttackBegin:
        {
            statueAttackBegin(isToRight);
            this->playAttackEffect();
        }
            break;
        case kAttackValid:
        case kAirAttackValid:
		case kSkillValid:
		{
            AnimationInfo_t* animationInfo = m_pCurAttack->attackAnimVec.at(1); //有效
            m_movePt.x = isToRight ? animationInfo->fSelfSpeedX : (-animationInfo->fSelfSpeedX);
            m_movePt.y = animationInfo->fSelfSpeedY;;
            m_moveAddPt.x = isToRight ? animationInfo->fSelfSpeedAddX : (-animationInfo->fSelfSpeedAddX);
            m_moveAddPt.y = animationInfo->fSelfSpeedAddY;
            
			CCAnimate* cAnimate2 = UnitResource::sharedInstance()->createAnimate(m_pUintInfo->ID, animationInfo);
            if (getChildByTag(EFFECTSPRTAG))
            {
                CCSprite *pEffectSprite = (CCSprite*)getChildByTag(EFFECTSPRTAG);
                pEffectSprite->setFlipX(m_UnitSprite->isFlipX());
            }
            if (action == kAirAttackValid)
            {
                m_UnitSprite->runAction(cAnimate2);
            }
            else
            {
                m_UnitSprite->runAction(CCSequence::create(cAnimate2,
                                               CCCallFunc::create(this, callfunc_selector(BattleUnitSprite::attackValidEnd)),NULL));
            }
            this->throwObject();
            statueAttackValid();//技能效果
            // 显示攻击区域
            CCRect rect = getLocalAttackRect();
            m_pAttackRectLayer->setPosition(rect.origin.x, rect.origin.y);
            m_pAttackRectLayer->setContentSize(rect.size);
		}
			break;
        case kAttackOver:
        case kAirAttackOver:
        case kSkillOver:
        {
            addcombo();
            
            //清楚缓存的招式
            if (m_pComboBuff.size() > 0)
            {
                m_pComboBuff.erase(m_pComboBuff.begin());
            }
            
            if (m_pComboBuff.size() > 0 && m_nCurAction == kAttackOver && (m_nCurObstacleDirection & kObstacleDown) == kObstacleDown) 
            {
                this->changeAction(kAttackBegin, m_nCurDirection);
            }
            else if (m_pComboBuff.size() > 0 && m_nCurAction == kSkillOver)
            {
                this->changeAction(kSkillBegin, m_nCurDirection);
            }
            else if (m_pCurAttack->attackAnimVec.size() > 2) //有收招
            {
                AnimationInfo_t* animationInfo = m_pCurAttack->attackAnimVec.at(2);
                
                m_movePt.x = isToRight ? animationInfo->fSelfSpeedX : (-animationInfo->fSelfSpeedX);
                m_movePt.y = animationInfo->fSelfSpeedY;;
                m_moveAddPt.x = isToRight ? animationInfo->fSelfSpeedAddX : (-animationInfo->fSelfSpeedAddX);
                m_moveAddPt.y = animationInfo->fSelfSpeedAddY;
                
                CCAnimate* cAnimate3 = UnitResource::sharedInstance()->createAnimate(m_unitConf.nResourceID, animationInfo);
                m_UnitSprite->runAction(CCSequence::create(cAnimate3, CCCallFunc::create(this, callfunc_selector(BattleUnitSprite::attackOverEnd)), NULL));
            }
            else
            {
                m_nAttackSkill = 0;
            }
        }
            break;
        case kBeHit:
        {
            if (getChildByTag(EFFECTSPRTAG))
            {
                getChildByTag(EFFECTSPRTAG)->removeFromParent();
            }
            m_pComboBuff.clear();
            int index = this->getUnderAttackIndex();
            if (m_pUnderAttacks->count() > index)
            {
                CCAnimate* animate = (CCAnimate*)(m_pUnderAttacks->objectAtIndex(index));
                animate->setDuration(m_fHitRecover);
                
                CCRepeat *repeat = CCRepeat::create(CCSequence::create(CCMoveBy::create(0.02, ccp(0, 0)),CCMoveBy::create(0.02, ccp(0, 0)),NULL),THEAIRMIN/0.04);
                m_UnitSprite->runAction(repeat);
                m_UnitSprite->runAction(CCSequence::create(CCDelayTime::create(THEAIRMIN),animate,
                CCCallFunc::create(this, callfunc_selector(BattleUnitSprite::resetStandby)), NULL));
            }
        }
            break;
        case kBeHitFly:
        {
            if (getChildByTag(EFFECTSPRTAG))
            {
                getChildByTag(EFFECTSPRTAG)->removeFromParent();
            }
            if (m_pUnderAttacks->count() > m_iUnderAttackIndex)
            {
                m_pComboBuff.clear();
                CCAnimate* animate = (CCAnimate*)(m_pUnderAttacks->objectAtIndex(m_iUnderAttackIndex));
                CCRepeat *repeat = CCRepeat::create(CCSequence::create(CCMoveBy::create(0.02, ccp(0, 0)),CCMoveBy::create(0.02, ccp(0, 0)),NULL),THEAIRMIN/0.04);
                m_UnitSprite->runAction(CCSequence::create(repeat,animate,NULL));
            }
        }
            break;
        case kBeHitDown:
        {
            m_movePt.x = 0;
            m_moveAddPt.x = 0;
            int nIndex = m_pUnderAttacks->count() > 4 ? 4 : (m_pUnderAttacks->count() - 1);

            if (m_pUnderAttacks->count() > nIndex)
            {
                m_UnitSprite->runAction(CCSequence::create((CCAnimate*)m_pUnderAttacks->objectAtIndex(nIndex),
                                                   CCCallFunc::create(this, callfunc_selector(BattleUnitSprite::resetFallOver)), NULL));
            }
        }
            break;
        default:
            break;
	}
    return true;
}

int BattleUnitSprite::getUnderAttackIndex()
{
    return m_iUnderAttackIndex;
}

void BattleUnitSprite::statueAttackBegin(bool isToRight)
{
    m_pCurAttack = m_pComboBuff.front();
    AnimationInfo_t* animationInfo = m_pCurAttack->attackAnimVec.at(0);
    m_movePt.x = isToRight ? animationInfo->fSelfSpeedX : (-animationInfo->fSelfSpeedX);
    m_movePt.y = animationInfo->fSelfSpeedY;
    m_moveAddPt.x = isToRight ? animationInfo->fSelfSpeedAddX : (-animationInfo->fSelfSpeedAddX);
    m_moveAddPt.y = animationInfo->fSelfSpeedAddY;
    this->playAttack(animationInfo, m_pCurAttack->nSharkType);
}

void BattleUnitSprite::playAttack(AnimationInfo_t* animationInfo, int sharkType)
{
    CCAnimate* cAnimate1 = UnitResource::sharedInstance()->createAnimate(m_pUintInfo->ID, animationInfo);
    m_UnitSprite->runAction(CCSequence::create(cAnimate1, CCCallFunc::create(this, callfunc_selector(BattleUnitSprite::attackValid)), NULL));
}

void BattleUnitSprite::jumpUpDone()
{
	this->changeAction(kJumpInAir);
}

//sprite Action
// 前进
void BattleUnitSprite::goForward()
{
    this->m_nCurDirection = kRight;
    if ((m_nCurAction >= kJumpUp && m_nCurAction < kJumpDown)||m_nCurAction == kRunning)
    {
        this->setFlipX(false);
        m_movePt.x = m_pUintInfo->fMoveSpeed;
    }
    else if (m_nCurAction == kAirAttackBegin || m_nCurAction == kAirAttackValid)
    {
        m_movePt.x = m_pUintInfo->fMoveSpeed;
    }
    else if (m_nCurAction == kAttackBegin && m_UnitSprite->isFlipX())
    {
        this->setFlipX(false);
        m_movePt.x = -m_movePt.x;
        m_moveAddPt.x = -m_moveAddPt.x;
    }
    else if (BattleLogic::canChangeUnitAction(m_nCurAction, kRunning) && (m_nCurObstacleDirection & kObstacleDown) == kObstacleDown && m_nCurAction != kBeHit)//不在空中
    {
        changeAction(kRunning, true);
    }
}

// 后退
void BattleUnitSprite::goBack()
{
    this->m_nCurDirection = kLeft;
    if ((m_nCurAction >= kJumpUp && m_nCurAction < kJumpDown) || m_nCurAction == kRunning)
    {
        this->setFlipX(true);
        m_movePt.x = -m_pUintInfo->fMoveSpeed;
    }
    else if (m_nCurAction == kAirAttackBegin || m_nCurAction == kAirAttackValid)
    {
        m_movePt.x = -m_pUintInfo->fMoveSpeed;
    }
    else if (m_nCurAction == kAttackBegin && !m_UnitSprite->isFlipX())
    {
        this->setFlipX(true);
        m_movePt.x = -m_movePt.x;
        m_moveAddPt.x = -m_moveAddPt.x;
    }
    else if (BattleLogic::canChangeUnitAction(m_nCurAction, kRunning) && (m_nCurObstacleDirection & kObstacleDown) == kObstacleDown && m_nCurAction != kBeHit)
    {
        changeAction(kRunning, false);
    }
}


void BattleUnitSprite::stopRunning()
{
    this->m_nCurDirection = kNone;
    if (m_nCurAction >= kJumpUp && m_nCurAction <= kAirAttackOver)
    {
        m_movePt.x = 0;
    }
    else if (BattleLogic::canChangeUnitAction(m_nCurAction, kStandby) && (m_nCurObstacleDirection & kObstacleDown) == kObstacleDown && m_nCurAction != kBeHit)
    {
        this->changeAction(kStandby);
    }
}

bool BattleUnitSprite::moveJump()
{
    if (BattleLogic::canChangeUnitAction(m_nCurAction, kJumpUp)&& (m_nCurObstacleDirection & kObstacleDown) == kObstacleDown)
    {
        this->changeAction(kJumpUp);
        return true;
    }
    else
    {
        return false;
    }
}

bool BattleUnitSprite::roll()
{
    if (BattleLogic::canChangeUnitAction(m_nCurAction, kRoll) && (m_nCurObstacleDirection & kObstacleDown) == kObstacleDown)
    {
        AudioResource::shareAudioResource()->playEffect("music/shanbi.mp3");
        this->changeAction(kRoll);
        return true;
    }
    else
    {
        return false;
    }
}

bool BattleUnitSprite::skillAttack(int index)
{
    return true;
}

void BattleUnitSprite::throwObject()
{
    ThrowEngine* pEngine = ThrowEngine::create(this);
    pEngine->throwObject();
}

kObstacleDirection BattleUnitSprite::getObstacleDirection(float moveX, float moveY, float* pCorrectX, float* pCorrectY)
{
    BattleMapUnits* pMapUnitLayer = (BattleMapUnits*)GameManager::sharedInstance()->getBattleMapLayer();
    return pMapUnitLayer->checkDirection(getUnitRect(), moveX, moveY, pCorrectX, pCorrectY);
}

void BattleUnitSprite::updateRecoverHp(float dt)
{
    addHp(getUnitBaseConf()->getRecoverHP());
}

void BattleUnitSprite::update()
{
    this->updateMovePt();
    BattleMapUnits* pMapUnitLayer = (BattleMapUnits*)GameManager::sharedInstance()->getBattleMapLayer();
    if (pMapUnitLayer)
    {
        float correctX = 0;
        float correctY = 0;
        if (!m_forceMoveBy.equals(CCPointZero))
        {
            m_movePt = m_forceMoveBy;
            m_forceMoveBy = CCPointZero;
        }
        m_nCurObstacleDirection = this->getObstacleDirection(m_movePt.x, m_movePt.y, &correctX, &correctY);
        
        if (m_movePt.x > 0)
        {
            correctX -= getUnitSize().width / 2 + getUnitOffPos().x;
        }
        else
        {
            correctX += getUnitSize().width / 2 - getUnitOffPos().x;
        }
        if (m_movePt.y > 0)
        {
            correctY -= getUnitSize().height / 2 + getUnitOffPos().y;
        }
        else 
        {
            correctY += getUnitSize().height / 2 - getUnitOffPos().y;
        }
        
        switch (m_nCurObstacleDirection)
        {
            case kObstacleNone:
                this->setPosition(ccp(getPosition().x + m_movePt.x, getPosition().y + m_movePt.y));
                break;
            case kObstacleLeft:
            case kObstacleRight:
                m_movePt.x = 0;
                m_moveAddPt.x = 0;
                this->setPosition(ccp(correctX, getPosition().y + m_movePt.y));
                break;
            case kObstacleUp:
            case kObstacleDown:
                m_movePt.y = 0;
                m_moveAddPt.y = JUMP_ACCELERATED_SPEED_Y;
                this->setPosition(ccp(getPosition().x + m_movePt.x, correctY));
                
                break;
            default:
                m_movePt.x = 0;
                m_moveAddPt.x = 0;
                m_movePt.y = 0;
                this->setPosition(ccp(correctX, correctY));
                break;
        }
        //CCLog("x:%f, y:%f", this->getPosition().x, this->getPosition().y);
        //CCLog("m_nCurObstacleDirection:%d", m_nCurObstacleDirection);
        if ((m_nCurObstacleDirection & kObstacleDown) == kObstacleDown)
        {
            if (m_nCurAction == kJumpInAir || m_nCurAction == kSkillOverDown || m_nCurAction == kAirAttackOver )
            {
                //除了正常攻击切换到结束状态时，m_pCurSkillConf要清空外，
                //还需要在其它空中状态切换到kJumpDown时，m_pCurSkillConf也要清空。
                m_nAttackSkill = 0;
                this->changeAction(kJumpDown);
            }
            else if (m_nCurAction == kBeHitFly)
            {
                this->changeAction(kBeHitDown);
            }
            else if(m_nCurAction == kAirAttackValid)
            {
                this->changeAction(kAirAttackOver);
            }
        }
        else 
        {
            if (m_nCurAction == kRunning)
            {
                this->changeAction(kJumpInAir);
            }
            
        }
    }
    
    if (m_nCurAction == kBeHit)
    {
        // 修正被攻击后的速度
        if (this->m_movePt.x <= 0.0001 && this->m_movePt.x >= -0.0001)
        {
            this->m_moveAddPt.x = 0;
        }
    }
    this->updateShadow(pMapUnitLayer);
}

void BattleUnitSprite::updateShadow(BattleMapUnits* pMapUnitLayer)
{
    float a = getContentSize().height/2 +m_pUintInfo->offPos.y-getUnitRect().size.height/2;
    float b = (getCenterPosition().y - getUnitRect().size.height/2 - pMapUnitLayer->m_pMissionConf->fHorizonHeight);
    m_pSprShadow->setScale(1.0f - 1.0f*b/640);
    m_pSprShadow->setPosition(ccp( getContentSize().width/2,a-b));
}

CCPoint BattleUnitSprite::updateMovePt()
{
    m_movePt.x += m_moveAddPt.x;
    m_movePt.y += m_moveAddPt.y;

    return m_movePt;
}

bool BattleUnitSprite::addSkillCombo(const SkillInfo_t* skill)
{
    //如果是技能攻击，一次性将所有技能信息全加入连击。
    m_pComboBuff.clear();
    const AttackInfoVec* attackVec = &(skill->attackVec);
    for (int i = 0; i < attackVec->size(); i++)
    {
        m_pComboBuff.push_back(attackVec->at(i));
    }
    return true;
}

// 添加攻击招式到招式队列中
void BattleUnitSprite::addCombo(const AttackInfoVec* pAttackVec, int nCombo)
{
    if (m_pComboBuff.size() > nCombo)
        return;
    if (m_nComboIdx >= pAttackVec->size())
        m_nComboIdx = 0;
    
    AttackInfo_t* pAttack = pAttackVec->at(m_nComboIdx);
    m_pComboBuff.push_back(pAttack);
    m_nComboIdx++;
}

void BattleUnitSprite::comboBegin(int nCombo)
{
    if (BattleLogic::canChangeUnitAction(m_nCurAction, kAttackBegin) && (m_nCurAction != kAttackOver))
    {
        m_nComboIdx = 0;
        m_pComboBuff.clear();
        for (int i = 0; i < nCombo; i++)
        {
            this->addCombo(&m_pUintInfo->attackVec, nCombo);
        }
        this->changeAction(kAttackBegin);
    }
}

void BattleUnitSprite::attackBegin()
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
        this->addCombo(&m_pUintInfo->attackVec);
        this->changeAction(kAttackBegin);
    }
    else if (m_nCurAction == kAttackBegin || m_nCurAction == kAttackValid || m_nCurAction == kAttackOver)
    {
        this->addCombo(&m_pUintInfo->attackVec);
    }
}

// 攻击起招动画结束，切换到攻击有效状态
void BattleUnitSprite::attackValid()
{
	kRoleActionType action;
	switch (m_nCurAction)
	{
		case kAttackBegin:
			action = kAttackValid;
			break;
		case kAirAttackBegin:
			action = kAirAttackValid;
			break;
		case kSkillBegin:
			action = kSkillValid;
			break;
        default:
            CCAssert(false, "attackValid invoke invalid.");
	}
	this->changeAction(action);
}

// 攻击有效动画结束，切换到下一状态
void BattleUnitSprite::attackValidEnd()
{
    kRoleActionType action;
    switch (m_nCurAction) 
    {
        case kAttackValid:
            action = kAttackOver;
            break;
        case kAirAttackValid:
            action = kAirAttackOver;
            break;
        case kSkillValid:
            action = kSkillOver;
            break;
        default:
            CCAssert(false, "BattleUnitSprite::attackValidEnd() invoke invalid.");
            break;
    }
    this->changeAction(action);
}

void BattleUnitSprite::attackOverEnd()
{
    m_nAttackSkill = 0;
    if ((m_nCurAction == kSkillOver || m_nCurAction == kAttackOver) && 
        (m_nCurObstacleDirection & kObstacleDown) != kObstacleDown)
    {
        this->changeAction(kSkillOverDown);
    }
    else if (m_nCurAction != kAirAttackOver) 
    {
        resetStandby();
    }
}

void BattleUnitSprite::getupEnd()
{
    this->resetStandby();
    if (this->getUnitType() == kBoss)
    {
        this->skillAttack(1);
    }
    if(this->getUnitType() == kHero ||
       this->getUnitType() == kEnemyHero ||
       this->getUnitType() == kBoss)
    {
        //起身无敌1秒
        m_fAngryTime = 0.5f;
        this->unschedule(schedule_selector(BattleUnitSprite::angryOver));
        this->schedule(schedule_selector(BattleUnitSprite::angryOver), 0.1f);
    }
}

void BattleUnitSprite::angryOver(float dt)
{
    if (m_fAngryTime >= 0)
    {
        m_fAngryTime = m_fAngryTime - dt;
    }
    else
    {
        this->unschedule(schedule_selector(BattleUnitSprite::angryOver));
    }
}

void BattleUnitSprite::resetStandby()
{
    m_moveAddPt.y = JUMP_ACCELERATED_SPEED_Y;
    m_nCurrentBeHitCount = 0;
    this->changeAction(kStandby);
}

// 攻击特效
void BattleUnitSprite::playAttackEffect()
{
    CCSize size = this->getContentSize();
    CCSprite* pEffectSprite = CCSprite::create();
    pEffectSprite->setTag(EFFECTSPRTAG);
    this->addChild(pEffectSprite, 1);
    pEffectSprite->setFlipX(m_UnitSprite->isFlipX());
    pEffectSprite->setPosition(ccp(size.width / 2, size.height / 2));
    if(m_pCurAttack->music.length()>0)
    {
        float posX = this->convertToWorldSpace(pEffectSprite->getPosition()).x;
        if (posX<SCREEN_WIDTH*1.5 && posX > -0.5*SCREEN_WIDTH)
        {
            char strmusic [64]= {0};
            sprintf(strmusic, "music/%s",m_pCurAttack->music.c_str());
            AudioResource::shareAudioResource()->playEffect(strmusic);
        }
    }
    
    CCArray* pArray = CCArray::create();
    bool isScale = false;
    for (int i = 0; i < m_pCurAttack->attactEffectVec.size(); i++)
    {
        AttackEffectInfo_t* pEffectInfo = (AttackEffectInfo_t*)m_pCurAttack->attactEffectVec.at(i);
        isScale = pEffectInfo->isScale;
        CCPoint pt = pEffectInfo->ptOffect;
        
        CCActionInterval *pAction = UnitResource::sharedInstance()->createAnimate(pEffectInfo);
        pArray->addObject(pAction);
    }
    
    if (isScale)
    {
        pEffectSprite->setScale(2);
    }
    
    if (pArray->count() > 0)
    {
        pEffectSprite->runAction(CCSequence::create(CCSequence::create(pArray),
                                                    CCRemoveSelf::create(),
                                                    NULL));
    }
}

int BattleUnitSprite::correctDamage(AttackInfo_t* pAttack, float fHurt)
{
    return (int)fHurt;
}

// 被打
void BattleUnitSprite::underAttack(BattleUnitSprite* pUnit, AttackInfo_t* pAttack, float fHurt, bool isCrit)
{
    m_pCurHit[pUnit] = pAttack;
    this->underAttack(this->getPosition().x < pUnit->getPosition().x, pAttack, fHurt, isCrit);
}

void BattleUnitSprite::underAttack(bool isToRight, AttackInfo_t* pAttack, float fHurt, bool isCrit)
{
    if(m_pUintInfo->strmusic.length()>0)
    {
        char strmusic [64]= {0};
        sprintf(strmusic, "music/%s",m_pUintInfo->strmusic.c_str());
        AudioResource::shareAudioResource()->playEffect(strmusic);
    }
    int damage = this->correctDamage(pAttack, fHurt);
    if (getHp() > 0)
    {
        addHp(-damage); 
        unAttackValidRole();
    }
    m_nBeHitCount ++;
    if (this->getUnitType() == kTree)
    {
        this->underAttackHarmAction(damage,isCrit, damage * ATTACK_TREE_BONUS_RATE);//伤害数值动画
    }
    else
    {
        this->underAttackHarmAction(damage,isCrit, 0);
    }
    
    this->playUnderAttackEffect(pAttack);
    if (this->getUnitType() != kTree)
    {
        m_UnitSprite->setColor(ccRED);
        scheduleOnce(schedule_selector(BattleUnitSprite::resetColor), 0.2f);
    }
    
    m_iUnderAttackIndex = pAttack->nUnderAttackIndex;
    m_fHitRecover = pAttack->hitRecover;
    m_fSpeedX = isToRight ? (-pAttack->fSpeedX) : (pAttack->fSpeedX);
    m_fSpeedY = pAttack->fSpeedY;
    m_fAddSpeedY = pAttack->fSpeedAddY;
    m_fAddSpeedX = 0;
    changeUnderAttackStatue(isToRight);
}

void BattleUnitSprite::changeUnderAttackStatue(bool isToRight)
{
    if (getHp() <= 0) //怪物死亡
    {
        this->onDie(&isToRight);
        m_UnitSprite->stopAllActions();
        m_nCurAction = kStandby;
        m_iUnderAttackIndex = 3;
        if (BattleLogic::canChangeUnitAction(m_nCurAction, kBeHitFly))
        {
            this->changeAction(kBeHitFly, isToRight);
            BattleMapUnits* pMapUnitLayer = (BattleMapUnits*)GameManager::sharedInstance()->getBattleMapLayer();
            if (pMapUnitLayer &&pMapUnitLayer->getBattleState() == kBattleing)
            {
                pMapUnitLayer->checkWin(this);
            }
        }
    }
    else
    {
        if ((m_pCurAttack != NULL && m_pCurAttack->isAngry == 1))//技能攻击时霸体
        {
            if (m_nCurAction == kSkillBegin||m_nCurAction == kSkillValid||m_nCurAction == kSkillOver)
            {
                return;
            }
            if ((m_nCurAction == kAttackBegin || m_nCurAction == kAttackValid
                || m_nCurAction == kAttackOver)&& (m_iUnderAttackIndex == 0 || m_iUnderAttackIndex == 1))//普通攻击时受招动画忽略 倒地动画会播
            {
                return;
            }
        }
        if (m_fAngryTime <= 0)
        {
            this->onBeHit(isToRight);
        }
    }
}

void BattleUnitSprite::resetColor(float dt)
{
    m_UnitSprite->setColor(ccColorOriginal);
}

void BattleUnitSprite::onBeHit(bool bIsToRight)
{
    if (getCurAction() == kBeHitFly)
    {
        m_nCurrentBeHitCount ++;
    }
    //0 1 受击  {2 、3、4 、5}倒地 6 根据攻击挑飞 7受击不播动画
    if ((m_iUnderAttackIndex == 2 || m_iUnderAttackIndex == 3 ||m_iUnderAttackIndex == 4 || m_iUnderAttackIndex == 5 || m_iUnderAttackIndex == 6) && this->getUnitType() != kTree)
    {
        if (BattleLogic::canChangeUnitAction(m_nCurAction, kBeHitFly))
        {
            m_movePt.x = 0;
            m_moveAddPt.x = 0;
            m_movePt.y = 0;
            m_moveAddPt.y = 0;
            if(m_iUnderAttackIndex != 5 && m_iUnderAttackIndex != 6)//特殊处理
            {
                AnimationInfo_t* t = m_pUintInfo->underAttacks.at(m_iUnderAttackIndex);
                m_fSpeedX = bIsToRight ? -t->fSelfSpeedX : t->fSelfSpeedX;
                m_fSpeedY = t->fSelfSpeedY;
                m_fAddSpeedX = bIsToRight ? t->fSelfSpeedAddX : -t->fSelfSpeedAddX;
                m_fAddSpeedY = t->fSelfSpeedAddY;
            }
            this->changeAction(kBeHitFly, bIsToRight);
            resetAddSpeed(0);
        }
        else if( m_nCurAction == kBeHitFly)
        {
            m_nCurAction = kBeHit;
            m_iUnderAttackIndex = 7;
            this->changeAction(kBeHitFly, bIsToRight);
            resetAddSpeed(0);
            //this->scheduleOnce(schedule_selector(BattleUnitSprite::resetAddSpeed), THEAIRMIN);
//            m_movePt.x = 0;
//            m_moveAddPt.x = 0;
//            m_movePt.y = 0;
//            m_moveAddPt.y = 0;
        }
    }
    else if(m_iUnderAttackIndex == 0|| m_iUnderAttackIndex == 1 || this->getUnitType() == kTree)
    {
        if (BattleLogic::canChangeUnitAction(m_nCurAction, kBeHit))
        {
            if(this->getUnitType() == kTree)//如果是摇钱树，被击打时无位移
            {
                m_fSpeedX = 0.0f;
                m_fAddSpeedX = 0;
                m_fSpeedY = 0.0f;
                m_fAddSpeedY = 0.0f;
            }
            this->changeAction(kBeHit);
            resetAddSpeed(0);
            //this->scheduleOnce(schedule_selector(BattleUnitSprite::resetAddSpeed), THEAIRMIN);
        }
        else if(m_nCurAction == kBeHitFly)//滞空
        {
            m_movePt.x = 0;
            m_moveAddPt.x = 0;
            m_movePt.y = 0;
            m_moveAddPt.y = 0;
            
            m_nCurAction = kBeHit;
            m_iUnderAttackIndex = 7;
            this->changeAction(kBeHitFly, bIsToRight);
            
            m_fAddSpeedX = 0;
            m_fSpeedY = 3.0f;
            m_fAddSpeedY = -0.2f;
            
            CCRepeat *repeat = CCRepeat::create(CCSequence::create(CCMoveBy::create(0.02, ccp(-10, 0)),CCMoveBy::create(0.02, ccp(10, 0)),NULL),THEAIRMIN/0.04);
            m_UnitSprite->runAction(repeat);
            resetAddSpeed(0);
            //this->scheduleOnce(schedule_selector(BattleUnitSprite::resetAddSpeed), THEAIRMIN);
        }
    }
    else if(m_iUnderAttackIndex == 7)//不播受击动画
    {
        return;
    }
}

void BattleUnitSprite::resetAddSpeed(float dt)
{
    if (getCurAction() == kBeHitFly || getCurAction() == kBeHit)
    {
        m_movePt.x = m_fSpeedX;
        m_moveAddPt.x = m_fAddSpeedX;
        m_movePt.y = m_fSpeedY;
        m_moveAddPt.y = m_fAddSpeedY;
    }
    else
    {
        m_moveAddPt.y = JUMP_ACCELERATED_SPEED_Y;
    }
}

void BattleUnitSprite::updateAngryTime(float dt)
{
    if (m_fAngryTime < 1.8 && m_fAngryTime > 1.6)
    {
        m_UnitSprite->setColor(ccColorOriginal);
    }
    
    m_fAngryTime -= 0.1;
    if (m_fAngryTime <= 0)
    {
        m_fAngryTime = 0;
        this->unschedule(schedule_selector(BattleUnitSprite::updateAngryTime));
    }
}

// 抛出物造成的伤害
bool BattleUnitSprite::underOjbectAttack(MapObject* pObject)
{
    //因为抛出物没有攻击开始、攻击有效、攻击结束的状态，故根据被击打者的状态来清除抛出物缓存
    if (!canUnderAttack())
    {
        return false;
    }
    
    ThrowSprite* pThrow = (ThrowSprite*)pObject;
    std::map<BattleUnitSprite*, clock_t>* pHitMap = &(pThrow->hitMap);
    if (pHitMap->find(this) != pHitMap->end() && pHitMap->at(this) != 0 && (clock() - pHitMap->at(this)) * 1.0 / CLOCKS_PER_SEC < pThrow->getAttackInterval())
    {
        return false;
    }
    if(m_pUintInfo->strmusic.length()>0)
    {
        char strmusic [64]= {0};
        sprintf(strmusic, "music/%s",m_pUintInfo->strmusic.c_str());
        AudioResource::shareAudioResource()->playEffect(strmusic);
    }
    
    AttackInfo_t* pAttack = pThrow->getAttackInfo();
    ThrowResource_t* pThrowResource = pThrow->getThrowResource();
    if (pHitMap->find(this) != pHitMap->end())
    {
        pHitMap->at(this) = clock();
    }
    else
    {
        pHitMap->insert(std::pair<BattleUnitSprite*, clock_t>(this, clock()));
    }
    
    UnitBaseConf throwBaseConf = pThrow->getUnitBaseConf();
    int skillidlv = pThrow->getskillConfIDLV();
    bool bcrit;
    float hurt = CalculateDamage::getThrowAttackHurt(skillidlv,0,m_nBuffSkill,&throwBaseConf,&m_unitConf,pThrow->getAttackInfo()->fOutAttackCorrect,pThrow->getAttackInfo()->fInAttackCorrect,bcrit);
    addHp(-hurt);
    
    ScheduleEngine* pEngine = AttackedEffectEngine::create(this, pAttack, skillidlv, pThrowResource->nAttackCount, hurt);
    pEngine->startWork(2 * ANIMAT_DELAY_PER_UNIT);//怪物直接死亡了怎么办？
    
    bool isToRight = this->getPosition().x < pObject->getPosition().x;
    m_iUnderAttackIndex = pAttack->nUnderAttackIndex;
    m_fHitRecover = pAttack->hitRecover;
    m_fSpeedX = isToRight ? (-pAttack->fSpeedX) : (pAttack->fSpeedX);
    m_fSpeedY = pAttack->fSpeedY;
    m_fAddSpeedY = pAttack->fSpeedAddY;
    m_fAddSpeedX = 0;
    changeUnderAttackStatue(isToRight);
    return true;
}

// 伤害数值动画
void BattleUnitSprite::underAttackHarmAction(int nHarm, bool isCrit, int nBonus)
{
    char strNum[64] = "";
    sprintf(strNum, "%d", nHarm);
    CCLabelAtlas *pLabNum = NULL;
    CCSprite *reduce = NULL;
    if(kHero == getUnitBaseConf()->nUnitType)
    {
        pLabNum = CCLabelAtlas::create(strNum, "BattleUI/role_harm_num.png", 25, 33, '0');
        reduce = CCSprite::create("BattleUI/role_reduce.png");
        reduce->setAnchorPoint(ccp(1, 0.5));
        reduce->setPositionY(16);
        pLabNum->addChild(reduce);
    }
    else
    {
        pLabNum = CCLabelAtlas::create(strNum, "BattleUI/monster_harm_num.png", 24, 28, '0');
        reduce = CCSprite::create("BattleUI/monster_reduce.png");
        reduce->setAnchorPoint(ccp(1, 0.5));
        reduce->setPositionY(14);
        pLabNum->addChild(reduce);
    }
    
    if (isCrit)
    {
        CCSprite *sprcrite = CCSprite::create("BattleUI/crit.png");
        sprcrite->setAnchorPoint(ccp(1, 0.5));
        reduce->addChild(sprcrite);
    }
    
    int radius = 100;

    pLabNum->setAnchorPoint(ccp(0, 0.5));
    pLabNum->setPosition(ccp(this->getCenterPosition().x + rand() % (radius) - radius / 2, this->getCenterPosition().y + 150));
    
    pLabNum->setScale(3);
    pLabNum->setOpacity(0);
    
    float appearTime = 0.2;
    CCSpawn* pAppear = CCSpawn::create(CCScaleTo::create(appearTime, 1.2), CCFadeIn::create(appearTime), NULL);
    
    float adjustTime = 0.3;
    CCScaleTo* pAdjust = CCScaleTo::create(adjustTime, 1.5);

    int x = rand() % (radius * 2) - radius;
    float disappearTime = 0.3;
    CCMoveTo* pActionMove = CCMoveTo::create(disappearTime, ccp(pLabNum->getPosition().x + x, pLabNum->getPosition().y + radius));
    CCFadeOut* pFadeOut = CCFadeOut::create(disappearTime);
    CCSpawn* pDisappear = CCSpawn::create(pActionMove, pFadeOut, NULL);

    pLabNum->runAction(CCSequence::create(pAppear, pAdjust, pDisappear, CCRemoveSelf::create(), NULL));
    
    this->getParent()->addChild(pLabNum, kRoleOrder);
    
    if (nBonus != 0)
    {
        CCLabelTTF* pLabel = CCLabelTTF::create(CCString::createWithFormat("银两X%d", nBonus)->getCString(), FONTNAME, 28);
        this->getParent()->addChild(pLabel, kRoleOrder);
        pLabel->setColor(ccGREEN);
        pLabel->setPosition(ccp(this->getCenterPosition().x, this->getCenterPosition().y + 230));
        pLabel->runAction(CCSequence::create(CCMoveBy::create(appearTime + adjustTime + disappearTime, ccp(0, radius)), CCRemoveSelf::create(), NULL));
        BattleMapUnits* pMapUnitLayer = (BattleMapUnits*)GameManager::sharedInstance()->getBattleMapLayer();
        if (pMapUnitLayer)
        {
            pMapUnitLayer->addYxb(nBonus);
        }
    }
}

// 中招人的招式的攻击特效
void BattleUnitSprite::playUnderAttackEffect(AttackInfo_t* pAttack)
{
    CCSprite* pUnderAttackSprite = CCSprite::create();
    pUnderAttackSprite->setAnchorPoint(ccp(0.5, 0.5));
    pUnderAttackSprite->setTag(100);
    int nRandom = rand() % 360;
    pUnderAttackSprite->setRotation(nRandom);
    
    nRandom = rand() % 100 + 50;
    float fScale = ((float)nRandom) / 100.0;
    pUnderAttackSprite->setScale(fScale);
    
    pUnderAttackSprite->setPosition(ccp(this->getCenterPosition().x, this->getCenterPosition().y));
    this->getParent()->addChild(pUnderAttackSprite, kRoleOrder);
    
    CCSprite* pFlashSprite = CCSprite::create();
    CCAnimation * ani= CRUtils::createAni("flash", 1, 5);
    pFlashSprite->runAction(CCSequence::create(CCAnimate::create(ani),
                                               CCRemoveSelf::create(),
                                               NULL));
    int nflashRandom = rand()%60 - 30;
    pFlashSprite->setRotation(nflashRandom);
    pFlashSprite->setPosition(ccp(this->getCenterPosition().x, this->getCenterPosition().y));
    this->getParent()->addChild(pFlashSprite, kRoleOrder);
    
    CCArray* pArray = CCArray::create();
    for (int i = 0; i < pAttack->attackHitVec.size(); i++)
    {
        AttackEffectInfo_t* pEffectInfo = (AttackEffectInfo_t*)pAttack->attackHitVec.at(i);
        CCPoint pt = pEffectInfo->ptOffect;
        pUnderAttackSprite->setPosition(ccp(this->getCenterPosition().x + pt.x, this->getCenterPosition().y + pt.y));
        
        CCActionInterval *pAction = UnitResource::sharedInstance()->createAnimate(pEffectInfo);
        pArray->addObject(pAction);
    }
    
    if (pArray->count() > 0)
    {
        pUnderAttackSprite->runAction(CCSequence::create(CCSequence::create(pArray), 
                                                         CCRemoveSelf::create(),
                                                         NULL));
    }
}

// 被地图中其他物体攻击特效
void BattleUnitSprite::playOtherAttackedEffect(float dt)
{
    this->underAttackHarmAction(1, false, 0);//伤害数值动画
}

// 基类操作，根据对方执行AI
void BattleUnitSprite::runAIOnce(BattleUnitSprite* pOhter)
{

}

// 倒地动画结束
void BattleUnitSprite::resetFallOver()
{
    if (getHp() <= 0)
    {
        if(m_pUintInfo->strDiemusic.length()>0)
        {
            char strmusic [64]= {0};
            sprintf(strmusic, "music/%s",m_pUintInfo->strDiemusic.c_str());
            AudioResource::shareAudioResource()->playEffect(strmusic);
        }
        CCFadeTo *fadeto1 = CCFadeTo::create(0.1, 128);
        CCFadeTo *fadeto2 = CCFadeTo::create(0.1, 255);
        m_UnitSprite->runAction(CCSequence::create(fadeto1,fadeto2,fadeto1,fadeto2,fadeto1,CCCallFunc::create(this,callfunc_selector(BattleUnitSprite::unitDead)),NULL));
    }
    else 
    {
        this->changeAction(kGetUp);
    }
}

void BattleUnitSprite::unitDead()
{
    BattleMapUnits* pMapUnitLayer = (BattleMapUnits*)GameManager::sharedInstance()->getBattleMapLayer();
    if (pMapUnitLayer)
    {
        pMapUnitLayer->removeBattleUnit(this);
    }
}

// bool m_bAttackRectShow
void BattleUnitSprite::setAttacRectVisible(bool bShow)
{
//    m_pSelfRectLayer->setVisible(bShow);
    m_pAttackRectLayer->setVisible(bShow);
    m_pBeHitRectLayer->setVisible(bShow);
}

float BattleUnitSprite::calculateHurt(int attackBuffSkill,int underAttackBuffSkill,UnitBaseConf* pUnitConf,AttackInfo_t* pAttackInfo,int skillconfID,UnitBaseConf* pUnderAttackConf,bool &bCrit)
{
    float hurt = 0;
    float outAttackCorrect = pAttackInfo->fOutAttackCorrect;
    float inAttackCorrect = pAttackInfo->fInAttackCorrect;
    
    if (skillconfID != 0)
    {
        hurt = CalculateDamage::getSkillAttackHurt(skillconfID,attackBuffSkill,underAttackBuffSkill,pUnitConf,pUnderAttackConf, outAttackCorrect, inAttackCorrect,bCrit);

    }
    else //state == kSkillValid
    {
        hurt = CalculateDamage::getNormalAttackHurt(attackBuffSkill,underAttackBuffSkill,pUnitConf,pUnderAttackConf, outAttackCorrect, inAttackCorrect,bCrit);
    }
    return hurt;
}

void BattleUnitSprite::reset()
{
    m_UnitSprite->stopAllActions();
    this->resetHp();
    m_nCurAction =kJumpDown;
    m_UnitSprite->setColor(ccColorOriginal);
    this->resetStandby();
}

bool BattleUnitSprite::attack(BattleUnitSprite* pSprite, AttackInfo_t* pAttackInfo)
{
    if (!pSprite)
    {
        return false;
    }
    
    // 攻击有效
    kRoleActionType state = this->getCurAction();
    if (state == kAttackValid || state == kSkillValid || state == kAirAttackValid)
    {
        // 攻击到了
        if (!pSprite->hasUnderAttacked(this)
            && this->getAttackRect().intersectsRect(pSprite->getUnitRect())&&pSprite->canUnderAttack())
        {
            if (pSprite->getHp() > 0)
            {
                attackValidRole();
                bool  bCrit = false;
                float hurt = BattleUnitSprite::calculateHurt(m_nBuffSkill,pSprite->m_nBuffSkill,&m_unitConf,pAttackInfo,m_nAttackSkill,pSprite->getUnitBaseConf(),bCrit);
                pSprite->underAttack(this, pAttackInfo, hurt, bCrit);
                return true;
            }
        }
    }
    else
    {
        // 移出保存的攻击
        pSprite->clearUnserAttacked(this);

    }
    return false;
}

bool BattleUnitSprite::canUnderAttack()
{
    return (getCurAction()!= kBeHitDown&&
            getCurAction()!= kGetUp &&
            getCurAction()!= kRoll&&
            (getHp()>0)&&
            m_nFlashTimes <= 0);
}

void BattleUnitSprite::addHp(int hp)
{
    if ((m_nHp == getUnitBaseConf()->getMaxHP() && hp>=0)||m_nHp<=0)
    {
        return;
    }
    if(m_nHp + hp>0)
    {
        if (hp < 0)
        {
            m_nHurtHp -= hp;
        }
        
        if (m_nHp+hp>getUnitBaseConf()->getMaxHP())
        {
            m_nHp = getUnitBaseConf()->getMaxHP();
        }
        else
        {
            m_nHp+= hp;
        }
    }
    else
    {
        m_nHurtHp -= (m_nHp + hp);
        m_nHp = 0;

    }
    CCNotificationCenter::sharedNotificationCenter()->postNotification(UNITHPCHANGE_NOTE, this);
}

void BattleUnitSprite::setHp(int hp)
{
    m_nHp = hp;
}

int BattleUnitSprite::getHp()
{
    return m_nHp;
}

float BattleUnitSprite::getAttackDistance()
{    float fDistance = 0;
    const AttackInfoVec* pVec = &m_pUintInfo->attackVec;
    for (AttackInfoVec::const_iterator it = pVec->begin(); it != pVec->end(); it++)
    {
        AttackInfo_t* pInfo = *it;
        CCRect rect = pInfo->rcAttackRect;
        if (fDistance == 0 || rect.origin.x + rect.size.width < fDistance)
        {
            fDistance = rect.origin.x + rect.size.width;
        }
    }
    return fDistance;
}