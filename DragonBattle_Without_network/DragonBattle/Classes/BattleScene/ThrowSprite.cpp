//
//  ThrowSprite.cpp
//  DragonBattle
//
//  Created by 永翔 何 on 13-5-6.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "ThrowSprite.h"
#include "BattleMapUnits.h"
#include "GameManager.h"

#include <math.h>

USING_NS_CC;

ThrowSprite::ThrowSprite()
{
    m_speed = CCPointZero;
    
    m_nDir = kThrowSpriteDirectionFollow;
    
    m_fThrowSpeedX = 0;
    m_fThrowSpeedY = 0;
    m_fThrowSpeedAddX = 0;
    m_fThrowSpeedAddY = 0;
    m_pAttackInfo = NULL;
    
    m_bIsExploding = false;
    
    hitMap.clear();
}

ThrowSprite::~ThrowSprite()
{
    m_pAttackInfo = NULL;
    m_pThrowInfo = NULL;
    
    hitMap.clear();
}

ThrowSprite* ThrowSprite::create(BattleUnitSprite* pUnitSprite, ThrowAttackConfig_t* pThrowAttackConfig, AttackInfo_t* attackInfo, int pSkillConfIDLV, UnitBaseConf* pUnitBaseConf,kThrowSpriteDirection nDir,CCPoint pt)
{
    ThrowSprite* pRet = new ThrowSprite();
    
    if (pRet && pRet->init(pUnitSprite, pThrowAttackConfig, attackInfo,pSkillConfIDLV, pUnitBaseConf,nDir,pt))
    {
        pRet->autorelease();
        return pRet;
    }
    
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool ThrowSprite::isFlipX()
{
    return m_bIsFlipX;
}

bool ThrowSprite::init(BattleUnitSprite* pUnitSprite, ThrowAttackConfig_t* pThrowAttackConfig, AttackInfo_t* attackInfo, int pSkillConfIDLV, UnitBaseConf* pUnitBaseConf, kThrowSpriteDirection nDir,CCPoint pt)
{
    m_pUnitSprite = pUnitSprite;
    m_nDir = nDir;
    m_pThrowInfo = ResourcesManager::sharedInstance()->getThrowConf(pThrowAttackConfig->nResID);
    m_pThrowAttackConf = pThrowAttackConfig;

    m_pAttackInfo = attackInfo;
    m_pSkillConfIDLV = pSkillConfIDLV;
    m_UnitBaseConf = *pUnitBaseConf;
    
    m_ID = m_pThrowInfo->nID;
    m_pAnimateSprite = CCSprite::create();
    m_pAnimateSprite->setScale(m_pThrowInfo->fScale);
    this->addChild(m_pAnimateSprite);

    m_pAnimateSprite->runAction(CCRepeatForever::create(ResourcesManager::sharedInstance()->createThrowAnimate(m_pThrowInfo->pMoveAction)));
    
    CCPoint point;
    CCPoint attackPoint;
    if (m_nDir == kThrowSpriteDirectionFollow)
    {
        m_bIsFlipX = m_pUnitSprite->isFlipX();
    }
    else if (m_nDir == kThrowSpriteDirectionLeft)
    {
        m_bIsFlipX = true;
    }
    else if (m_nDir == kThrowSpriteDirectionRight)
    {
        m_bIsFlipX = false;
    }
    CCPoint collideStart = m_pThrowInfo->ptCollideStart;
    CCPoint throwStart = pThrowAttackConfig->ptThrowStart;
    CCPoint spritePos = pt;
    if (m_bIsFlipX)
    {
        point = CCPointMake(spritePos.x - throwStart.x, spritePos.y + throwStart.y);
        attackPoint = CCPointMake(-collideStart.x, collideStart.y);
    }
    else 
    {
        point = CCPointMake(spritePos.x + throwStart.x, spritePos.y + throwStart.y);
        attackPoint = CCPointMake(collideStart.x, collideStart.y);
    }
    this->setPosition(point);
    
    if (m_pThrowAttackConf->nAttackType == kDistance)
    {
        m_posStart = point;
    }
    else if (m_pThrowAttackConf->nAttackType == kTime)
    {
        this->schedule(schedule_selector(ThrowSprite::timeIsUp), m_pThrowAttackConf->fAttackValue);
    }
    
    switch (m_pThrowAttackConf->nThrowType)
    {
        case kSpeed:
        {
            ThrowAttackConfig_t::ThrowUnion_t::ThrowSpeed_t ts = pThrowAttackConfig->throwUnion.throwSpeed;
            m_fThrowSpeedX = m_bIsFlipX ? (-ts.fThrowSpeedX) : ts.fThrowSpeedX;
            m_fThrowSpeedY = ts.fThrowSpeedY;
            m_fThrowSpeedAddX = m_bIsFlipX ? (-ts.fThrowSpeedAddX) : ts.fThrowSpeedAddX;
            m_fThrowSpeedAddY = ts.fThrowSpeedAddY;
            m_pAnimateSprite->setFlipX(m_bIsFlipX);
            break;
        }
        case kFollow:
        {
            ThrowAttackConfig_t::ThrowUnion_t::ThrowFollow_t tf = pThrowAttackConfig->throwUnion.throwFollow;
            m_fThrowFollowX = tf.fThrowFollowX;
            m_fThrowFollowY = tf.fThrowFollowY;
            break;
        }
        case kBezier:
        {
            int fTargetX = 300;
            int fTargetY = 160;
            ThrowAttackConfig_t::ThrowUnion_t::ThrowBezier_t tb = pThrowAttackConfig->throwUnion.throwBezier;
            ccBezierConfig bezier;
            bezier.controlPoint_1 = point;
            bezier.controlPoint_2 = CCPointMake((fTargetX - point.x) / 2 + point.x, tb.fThrowHeight);
            bezier.endPosition = CCPointMake(fTargetX, fTargetY);
            CCBezierTo* actionMove = CCBezierTo::create((fTargetX - point.x) / tb.fThrowSpeed, bezier);
            this->runAction(actionMove);
            break;
        }
        case kTrace:
        {
            
            break;
        }
        default:
            break;
    }
    
    m_pAttackRectLayer = CCLayerColor::create(ccc4(64, 64, 64, 128), 0, 0);
    this->addChild(m_pAttackRectLayer, 10, 100);
    CCSize size = m_pThrowInfo->contentSize;
    m_pAttackRectLayer->setContentSize(size);
    m_pAttackRectLayer->setAnchorPoint(CCPointMake(0, 0));
    m_pAttackRectLayer->setPosition(CCPointMake(attackPoint.x - size.width / 2, attackPoint.y - size.height / 2));
    this->setAttacRectVisible(false);
    return true;
}

void ThrowSprite::timeIsUp(float dt)
{
    this->unschedule(schedule_selector(ThrowSprite::timeIsUp));
    this->removeFromParent();
}

void ThrowSprite::removeFromParent()
{
    BattleMapUnits* pMapUnitLayer = (BattleMapUnits*)GameManager::sharedInstance()->getBattleMapLayer();
    if (pMapUnitLayer)
    {
        pMapUnitLayer->removeThrowSprite(this);
    }
}

// 抛出物爆炸
void ThrowSprite::explode(CCPoint point)
{
    m_pAnimateSprite->stopAllActions();
    
    //CCPoint oriPoint = this->getPosition();
    this->setPosition(ccpAdd(point, m_pThrowInfo->ptExplodeStart));
    //CCLog("origin: %f, %f now %f, %f", oriPoint.x, oriPoint.y, point.x, point.y);
    m_pAnimateSprite->runAction(CCSequence::create(ResourcesManager::sharedInstance()->createThrowAnimate(this->m_pThrowInfo->pAttackAction), CCCallFunc::create(this, callfunc_selector(ThrowSprite::removeFromParent)), NULL));
    CCPoint originPoint = m_pAttackRectLayer->getPosition();
    CCSize contentSize = m_pThrowInfo->contentSize;
    CCSize explodeSize = m_pThrowInfo->explodeSize;
    m_pAttackRectLayer->setPosition(CCPointMake(originPoint.x + contentSize.width / 2 - explodeSize.width / 2, originPoint.y + contentSize.height / 2 - explodeSize.height / 2));
    m_pAttackRectLayer->setContentSize(explodeSize);
    m_bIsExploding = true;
}

void ThrowSprite::destroy()
{
    this->removeAllChildrenWithCleanup(true);
    this->removeFromParentAndCleanup(true);
}

kThrowAttackType ThrowSprite::getAttackType()
{
    return m_pThrowAttackConf->nAttackType;
}

CCPoint ThrowSprite::updatePos()
{
    m_speed = CCPointZero;
    CCPoint p = this->getPosition();
    CCPoint ret;
    if (m_pThrowAttackConf->nThrowType == kSpeed)
    {
        m_fThrowSpeedX += m_fThrowSpeedAddX;
        m_fThrowSpeedY += m_fThrowSpeedAddY;
        ret = CCPointMake(p.x + m_fThrowSpeedX, p.y + m_fThrowSpeedY);
    }
    else if (m_pThrowAttackConf->nThrowType == kFollow)
    {
        if (m_pUnitSprite == NULL)
        {
            removeFromParent();
            return CCPointZero;
        }
        else 
        {
            if (m_pUnitSprite->getHp() <= 0)
            {
                removeFromParent();
                return CCPointZero;
            }
            else 
            {
                CCPoint targetPoint = m_pUnitSprite->getPosition();
                if (m_pUnitSprite->isFlipX())
                {
                    targetPoint = CCPointMake(targetPoint.x - m_pThrowAttackConf->ptThrowStart.x, targetPoint.y + m_pThrowAttackConf->ptThrowStart.y);
                }
                else 
                {
                    targetPoint = CCPointMake(targetPoint.x + m_pThrowAttackConf->ptThrowStart.x, targetPoint.y + m_pThrowAttackConf->ptThrowStart.y);
                }
                float targetX, targetY;
                if (targetPoint.x > p.x)
                {
                    targetX = fabs(m_fThrowFollowX);
                }
                else if (targetPoint.x < p.x)
                {
                    targetX = -fabs(m_fThrowFollowX);
                }
                else 
                {
                    targetX = 0;
                }
                if (targetPoint.y > p.y)
                {
                    targetY = fabs(m_fThrowFollowY);
                }
                else if (targetPoint.y < p.y)
                {
                    targetY = -fabs(m_fThrowFollowY);
                }
                else 
                {
                    targetY = 0;
                }
                targetX += p.x;
                targetY += p.y;
                if (fabs(targetX - targetPoint.x) < fabs(m_fThrowFollowX))
                    targetX = targetPoint.x;
                if (fabs(targetY - targetPoint.y) < fabs(m_fThrowFollowY))
                    targetY = targetPoint.y;            
                ret = CCPointMake(targetX, targetY);
            }
        }
    }
    
    //根据飞行速度调整角度
    m_speed = ccpSub(ret, p);
    float angle = atan2f(m_speed.x, m_speed.y) / M_PI * 180;
    this->setRotation(this->isFlipX() ? (angle + 90) : (angle - 90));
    
    this->setPosition(ret);

    if (m_pThrowAttackConf->nAttackType == kDistance &&
        ccpDistance(m_posStart, ret) >= m_pThrowAttackConf->fAttackValue)
    {
        this->removeFromParent();
        return CCPointZero;
    }

    return ret;
}

void ThrowSprite::setAttacRectVisible(bool bShow)
{
    m_pAttackRectLayer->setVisible(bShow);
}

// 攻击区域
CCRect ThrowSprite::getAttackRect()
{
    return CCRectMake(m_pAttackRectLayer->getPosition().x + getPosition().x, 
                      m_pAttackRectLayer->getPosition().y + getPosition().y, 
                      m_pAttackRectLayer->getContentSize().width, 
                      m_pAttackRectLayer->getContentSize().height);
}
