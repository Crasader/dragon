//
//  BattleMapUnits.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-22.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "BattleMapUnits.h"
#include "BattleUnitSprite.h"
#include "PartnerSprite.h"
#include "MonsterSprite.h"
#include "ThiefSprite.h"
#include "EnemyHeroSprite.h"
#include "MonsterContainer.h"
#include "GlobalDefine.h"
#include "MapCoord.h"
#include "BattleLogic.h"
#include "GameManager.h"
#include "UtilityAction.h"
#include "CommonUtility.h"
#include "GeometryUtility.h"
#include "MissionAttribute.h"
#include "DropItem.h"
#include "SimpleAudioEngine.h"
#include "AudioResource.h"
#include "EffectResource.h"

#include <algorithm>

#define BORN_MONSTER_FREQUENCY      30
using namespace std;
using namespace cocos2d;

BattleMapUnits::BattleMapUnits()
{
    m_pMapBg = NULL;
    m_pMissionConf = NULL;
    m_pSelf = NULL;
    m_pPartner = NULL;
    m_nCurPartnerIdx = 0;
    
    m_monsterContainer.init();
    m_throwSpriteVec.clear();
    m_mapObstacleVec.clear();
    m_deadMonsterVec.clear();
    
    m_fMapLength = 0;
    m_nCurMissionStageIdx = 0;
    m_pCurMissionStage = NULL;
    m_bGotBornPos = false;
    m_bClipBothSide = false;
    m_bPosOnRightClip = false;
    
    m_nKillCnt = 0;
    m_nCurWaveIdx = 0;
    m_nCurStageBornCnt = 0;
    m_nBattleState = kBattleing;
    m_nBornMonsterFrequency = 0;
    m_nBornFollowMonsterFrequency = 180;
    
    m_bHasBronFristMonster = false;
    m_bHasFristMonsterAIStart = false;
    m_bStopCreateMonster = false;
    
    m_pGoForwardsTip = NULL;
    m_bShowGoForwardsTip = false;
    m_pComboControl = NULL;
    m_fToY = 0;
    m_fFromY = 0;
    m_fDeltaY = 0;
    m_isStop = false;
    m_pMonsterCountTips = NULL;
    m_fightTime = 0;
    
    m_pDropItemArray = CCArray::create();
    CC_SAFE_RETAIN(m_pDropItemArray);
    m_dropItemMap.clear();
    m_thiefItemMap.clear();
    m_dieMonsterMap.clear();
    m_nGetExp = 0;
    m_nGetYxb = 0;
    m_nGetGold = 0;
    m_ItemLimitVec.clear();
    m_ThiefItemVec.clear();
}

BattleMapUnits::~BattleMapUnits()
{
    this->unscheduleAllSelectors();
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    CCDirector::sharedDirector()->setAnimationInterval(1.0 / 60);
    CC_SAFE_RELEASE_NULL(m_pMapBg);
    CC_SAFE_RELEASE_NULL(m_pDropItemArray);
    m_dropItemMap.clear();
    m_thiefItemMap.clear();
    m_dieMonsterMap.clear();
    m_ItemLimitVec.clear();
    m_ThiefItemVec.clear();
    m_pMissionConf = NULL;
    
}

BattleMapUnits* BattleMapUnits::create(MissionConf_t* pMissionConf)
{
    BattleMapUnits* pRet = new BattleMapUnits();
    if (pRet && pRet->init(pMissionConf))
    {
        pRet->autorelease();
        return pRet;
    }
    
    if (pRet)
    {
        delete pRet;
        pRet = NULL;
    }
    return NULL;
}

bool BattleMapUnits::init(MissionConf_t *pMissionConf)
{
    if (!CCLayer::init())
    {
        CCLOG("BattleMapUnits parent init error!");
        return false;
    }
    
    this->setAnchorPoint(ccp(0, 0));
    this->setPosition(ccp(0, 0));
    
    m_pMissionConf = pMissionConf;
    this->createBg();
    
    this->createObstacle();
    
    //定向坐标系原点
    MapCoord::setMainLayer(this);
    
    m_trapContainer.reset(this, m_pMissionConf->trapUnitVec);
    
    m_nCurMissionStageIdx = 0;
    m_pCurMissionStage = &(m_pMissionConf->missionStageVec.at(m_nCurMissionStageIdx));
    m_nCurStageBornCnt = 0;
    return true;
}

void BattleMapUnits::createObstacle()
{
    MapObstacle* mapObstacle = MapObstacle::create();
    this->addChild(mapObstacle, kBoxUnitOrder);
    const float gExpandLength = 3*SCREEN_WIDTH;//地图开始和结束各扩展一个屏幕长度,防止单位没有碰撞区域掉落
    mapObstacle->setCollideRect(CCRect(-gExpandLength, 0, m_fMapLength + gExpandLength * 2, m_pMissionConf->fHorizonHeight));
    mapObstacle->setObstacleDirection(kObstacleDown);
    m_mapObstacleVec.push_back(mapObstacle);
    
    for (int i = 0; i < m_pMissionConf->obstacleVec.size(); i++)
    {
        MapObstacle* mapObstacle1 = MapObstacle::create(&m_pMissionConf->obstacleVec.at(i));
        this->addChild(mapObstacle1, kBoxUnitOrder);
        m_mapObstacleVec.push_back(mapObstacle1);
    }
}

void BattleMapUnits::createBg()
{
    m_pMapBg = BattleMap::createWithID(m_pMissionConf->nScenesResourceID, this);
    CC_SAFE_RETAIN(m_pMapBg);
    m_fMapLength = m_pMapBg->getBattleMapLength();

}

void BattleMapUnits::addSelf(HeroSprite* pSprite)
{
    if (m_pSelf)
    {
        return;
    }
    m_friendContainer.addUnit(pSprite);
    m_pSelf = pSprite;
    this->addChild(pSprite, kRoleOrder);
    pSprite->setCenterPosition(ccp(m_pMissionConf->fRolePosX, m_pMissionConf->fRolePosY));
    CCNotificationCenter::sharedNotificationCenter()->postNotification(UNITHPCHANGE_NOTE, pSprite);
}

void BattleMapUnits::onExit()
{
    this->stopAllActions();
    this->removeAllChildrenWithCleanup(true);
    CCLayer::onExit();
}

// 重新定位地表层坐标 每一帧计算一次
void BattleMapUnits::battleMapUpdate(float dt)
{
    m_fightTime++;
    if (!m_pSelf)
    {
        return ;
    }
    
    // 英雄和地表层位置重设
    this->heroAndLandbgUndate(dt);
    
    // 背景层移动
    this->resetBackGroud();
    
    // 判断是否到达刷怪点
    this->reachStageBornPos();
    
    // 单位更新

    this->mapUnitsUpdate(dt);
}

void BattleMapUnits::heroAndLandbgUndate(float dt)
{
    // 先更新英雄坐标点，在判断坐标点是否合理，然后重设
    m_pSelf->update();
    
    // 卡屏，并校验英雄坐标点是否正确
    CCPoint rolePt = m_pSelf->getPosition();
    CCPoint roleScreenPt = MapCoord::layerToScreen(rolePt);
    CCPoint pos = this->getPosition();
    float adjustX = pos.x;
    float adjustY = pos.y;
    if (m_bPosOnRightClip)
    {
        adjustX -= m_pSelf->getUnitResource()->fMoveSpeed;
        
        if (roleScreenPt.x <= ROLE_RUNNING_RIGHT_CLIP_POS_X)
        {
            m_bPosOnRightClip = false;
        }
        
        if (rolePt.x >= m_fRightClipX)
        {
            rolePt.x = m_fRightClipX;
        }

        if (adjustX < -(m_fRightClipX - SCREEN_WIDTH))
        {
            adjustX = -(m_fRightClipX - SCREEN_WIDTH);
        }        
    }
    else
    {
        if (roleScreenPt.x <= ROLE_RUNNING_LEFT_CLIP_POS_X)  //左边
        {
            if (rolePt.x - ROLE_RUNNING_LEFT_CLIP_POS_X / m_fScaleX <= m_fLeftClipX)
            {
                if (roleScreenPt.x <= ROLE_LEFT_CLIP_POS_X)
                {
                    rolePt.x = -(this->getPosition().x - ROLE_LEFT_CLIP_POS_X);
                }
                //rolePt.x = m_fLeftClipX + ROLE_LEFT_CLIP_POS_X;
                adjustX = -m_fLeftClipX;
            }
            else
            {
                adjustX = -(rolePt.x - ROLE_RUNNING_LEFT_CLIP_POS_X / m_fScaleX);
            }
        }
        else if (roleScreenPt.x > ROLE_RUNNING_RIGHT_CLIP_POS_X)
        {
            if (rolePt.x + (SCREEN_WIDTH - ROLE_RUNNING_RIGHT_CLIP_POS_X) / m_fScaleX >= m_fRightClipX)
            {
                adjustX = -(m_fRightClipX - SCREEN_WIDTH / m_fScaleX);
                if (roleScreenPt.x > ROLE_RIGHT_CLIP_POS_X)
                {
                    rolePt.x = m_fRightClipX - (SCREEN_WIDTH - ROLE_RIGHT_CLIP_POS_X) / m_fScaleX;
                }
            }
            else
            {
                adjustX = -rolePt.x + ROLE_RUNNING_RIGHT_CLIP_POS_X / m_fScaleX;
            }
        }
    }
    
    //horizonHeight表示角色的初始坐标高度，该高度计算过程为：
    //1.地面高度+碰撞区域高度/2=碰撞区域中心点高度
    //2.碰撞区域中心点高度-碰撞点与角色图片中心点的高度差=角色图片中心点高度
    const float horizonHeight = m_pMissionConf->fHorizonHeight + m_pSelf->getUnitRect().size.height / 2 - (m_pSelf->getCenterPosition().y - rolePt.y);
    //adjustParam为角色高度切换时，镜头在Y方面的切换距离，该距离为角度高度差乘以一个小于1的系数，
    //从而使镜头切换距离不至于过大。
    const float adjustParam = 2.0 / 3;
    const float duration = 1;
    CCAction* action = this->getActionByTag(MapSharkActionTag);
    if (action == NULL || action->isDone())
    {
        if (fabs(-(rolePt.y - horizonHeight) * adjustParam + m_fToY) > 1 &&
            (m_pSelf->getCurObstacleDirection() & kObstacleDown) == kObstacleDown)
        {
            m_fToY = (rolePt.y - horizonHeight) * adjustParam;
            m_fDeltaY = (m_fToY - m_fFromY) / (60 * duration);
        }
        m_fFromY += m_fDeltaY;
        if (fabs(m_fToY - m_fFromY) < 1)
        {
            m_fDeltaY = 0;
        }
        this->setPosition(ccp(adjustX, adjustY - m_fDeltaY));
    }
    else
    {
        this->setPosition(ccp(adjustX, adjustY));
    }
    m_pSelf->setPosition(rolePt);
}

void BattleMapUnits::resetBackGroud()
{
    m_pMapBg->resetBackGroud(m_fMapLength);
}

// 判断到达刷怪点
void BattleMapUnits::reachStageBornPos()
{
    //没卡屏，到达左边卡屏点，当前屏幕卡屏
    if (!m_bClipBothSide
        && m_pSelf->getCenterPosition().x >= m_pCurMissionStage->fClipScreenPosX - m_pCurMissionStage->fClipScreenLength)
    {
        m_bClipBothSide = true;
        this->resetClipStageBothSide();
    }
    
    // 刷出了第一波摆放怪，并且AI没有开始
    if (!m_bHasFristMonsterAIStart && m_bHasBronFristMonster)
    {
        if ((m_bGotBornPos && m_nCurMissionStageIdx == 0) || m_nCurMissionStageIdx > 0)
        {
            m_bHasFristMonsterAIStart = true;
            // 设置摆放怪AI开始
            BattleUnitSprite* pMonster = NULL;
            MONSTER_CONTAINER_FOREACH(&m_monsterContainer, pMonster)
            { 
                pMonster->setAIActivated(true);
            }
        }
    }
    
    //没到出怪点，到达出怪点后，开始出怪
    if (!m_bGotBornPos && m_pSelf->getCenterPosition().x >= m_pCurMissionStage->fClipScreenPosX)
    {
        m_bGotBornPos = true;
    }
}

void BattleMapUnits::mapUnitsUpdate(float dt)
{
    this->onGetDropItem();
    
    if (!m_isStop)
    {
        if (m_nBattleState == kBattleing)
        {
            BattleUnitSprite* pUnit = NULL;
            UNIT_CONTAINER_FOREACH(&m_friendContainer, pUnit)
            {
                this->onUnitAttack(pUnit);
                // 陷阱攻击及被攻击
                m_trapContainer.update(pUnit);
            }
            
            pUnit = NULL;
            MONSTER_CONTAINER_FOREACH(&m_monsterContainer, pUnit)
            {
                this->onUnitAttack(pUnit);
            }
            
            // 抛出物攻击
            this->onThrowAttack();
            
            // 判断刷怪点卡屏及刷怪点出怪
            this->judgeClipAndBornMonster();
        }
        this->updateMonsters(dt);
        this->updateGoForwardsTipPos();
        this->updateComboControlPos();
    }
    this->updateFriend(dt);
    // 抛出物更新
    this->updateThrowSpritePos();
    // 掉落物更新
    this->updateDropItemPos();
}

// 删除抛出物
void BattleMapUnits::removeThrowSprite(ThrowSprite* pThrow)
{
    ThrowSpriteVec::iterator itr = find(m_throwSpriteVec.begin(), m_throwSpriteVec.end(), pThrow);
    
    if (itr != m_throwSpriteVec.end())
    {
        ThrowSprite* ts = *itr;
        ts->destroy();
        m_throwSpriteVec.erase(itr);
        ts = NULL;
        return;
    }
}

void BattleMapUnits::addThrowSprite(AttackInfo_t* pAttackInfo,
                                    int pSkillConfID,
                                    UnitBaseConf* m_pUnitBaseConf,
                                    ThrowAttackConfig_t* throwAttackConfig,
                                    BattleUnitSprite* pUnitSprite, int nDir,CCPoint pt)
{
    
    ThrowSprite* ts = ThrowSprite::create(pUnitSprite, throwAttackConfig, pAttackInfo, pSkillConfID, m_pUnitBaseConf,(kThrowSpriteDirection)nDir,pt);
    this->addChild(ts, kThrowUnitOrder);
    m_throwSpriteVec.push_back(ts);
}

bool BattleMapUnits::isThrowSpriteCollideObstacle(ThrowSprite* pSprite, cocos2d::CCRect& explodeRect)
{
    MapObstacleVec::iterator itr = m_mapObstacleVec.begin();
    while (itr != m_mapObstacleVec.end())
    {
        MapObstacle* mo = *itr;
        CCRect obstacleRect = mo->getCollideRect();
        CCRect throwSpriteRect = pSprite->getAttackRect();
        if (obstacleRect.intersectsRect(throwSpriteRect))
        {
            explodeRect = GeometryUtility::GetIntersectRect(obstacleRect, throwSpriteRect);
            return true;
        }
        itr++;
    }
    explodeRect = CCRectZero;
    return false;
}

void BattleMapUnits::updateDropItemPos()
{
    for (int i = 0; i < m_pDropItemArray->count(); i++)
    {
        DropItem* pItem = dynamic_cast<DropItem*>(m_pDropItemArray->objectAtIndex(i));
        pItem->updatePos();
    }
}

void BattleMapUnits::showDropItemTip(DropItem* pItem)
{
    char str[128] = {0};
    sprintf(str, "X%d", pItem->GetItemCount());
    
    CCLabelTTF* pCount = CCLabelTTF::create(str, "Arial-BoldMT", 20);
    this->addChild(pCount, kHitTipOrder);
    pCount->setAnchorPoint(ccp(0, 0.5));
    pCount->setPosition(ccpAdd(pItem->getPosition(), ccp(0, 18)));
    pCount->setColor(ccWHITE);
    
    /*
    sprintf(str, "item_%d.png", pItem->GetItemID());
    CCSprite* pIcon = CCSprite::createWithSpriteFrameName(str);
    pCount->addChild(pIcon);
    pIcon->setAnchorPoint(ccp(0.5, 0.5));
    pIcon->setPosition(ccp(-22, 17));
    pIcon->setScale(0.6);
     */
    
    pCount->runAction(CCSequence::create(CCMoveBy::create(0.5, ccp(0, 50)), CCRemoveSelf::create(), NULL));
}

void BattleMapUnits::onGetDropItem()
{
    if (m_pSelf != NULL)
    {
        for (int i = 0; i < m_pDropItemArray->count(); i++)
        {
            DropItem* pItem = dynamic_cast<DropItem*>(m_pDropItemArray->objectAtIndex(i));
            if (pItem->getUnitRect().intersectsRect(m_pSelf->getUnitRect()))
            {
                CCSprite* effect = EffectResource::sharedInstance()->spriteWithEffect(30, false, true);
                effect->setPosition(pItem->getPosition());
                effect->setScale(1.2f);
                effect->setAnchorPoint(ccp(0.5f, 0.2f));
                
                if (pItem->getParent())
                {
                    pItem->getParent()->addChild(effect, kHitTipOrder);
                }
                
                int itemId = pItem->GetItemID();
                if (itemId == 0 || itemId == 2 || (itemId >= 100003 && itemId <= 100008))
                {
                    this->showDropItemTip(pItem);
                }
                dynamic_cast<BattleLayer*>(GameManager::sharedInstance()->getBattleLayer())->flyItemToPortrait(itemId, this->convertToWorldSpace(pItem->getPosition()));

                pItem->removeFromParent();
                m_pDropItemArray->removeObjectAtIndex(i);
                return;
            }
        }
    }
}

void BattleMapUnits::collectAllItems()
{
    for (int i = 0; i < m_pDropItemArray->count(); i++)
    {
        DropItem* pItem = dynamic_cast<DropItem*>(m_pDropItemArray->objectAtIndex(i));

        CCSprite* effect = EffectResource::sharedInstance()->spriteWithEffect(30, false, true);
        effect->setPosition(pItem->getPosition());
        effect->setScale(1.2f);
        effect->setAnchorPoint(ccp(0.5f, 0.2f));
        pItem->getParent()->addChild(effect, kHitTipOrder);
        
        int itemId = pItem->GetItemID();
        if (itemId == 0 || itemId == 2 || (itemId >= 100003 && itemId <= 100008))
        {
            this->showDropItemTip(pItem);
        }
        dynamic_cast<BattleLayer*>(GameManager::sharedInstance()->getBattleLayer())->flyItemToPortrait(itemId, this->convertToWorldSpace(pItem->getPosition()));
        
        pItem->removeFromParent();
    }
}

void BattleMapUnits::updateThrowSpritePos()
{
    ThrowSpriteVec::iterator itr = m_throwSpriteVec.begin();
    while (itr != m_throwSpriteVec.end())
    {
        ThrowSprite* ts = *itr;
        if (!ts->isExploding())
        {
            CCPoint point = ts->updatePos();
            if (ccpDistance(point, CCPointZero) == 0)
            {
                itr++;
            }
            else 
            {
                CCPoint screenPoint = MapCoord::layerToScreen(point);
                kThrowAttackType attackType = ts->getAttackType();
                CCRect explodeRect = CCRectZero;
                if ((attackType == kExplode || attackType == kCollide || attackType == kOutScreen) && (screenPoint.x  > SCREEN_WIDTH + 350 || screenPoint.x < 0 - 350))
                {
                    this->removeThrowSprite(ts);
                }
                //如果抛出物类型不为kFollow，则要判断抛出物是否与障碍物相互碰撞
                //如相互碰撞，则发生爆炸。
                else if (ts->getThrowType() != kFollow &&
                         this->isThrowSpriteCollideObstacle(ts, explodeRect))
                {
                    ts->explode(CCPointMake(explodeRect.origin.x + explodeRect.size.width / 2, explodeRect.origin.y + explodeRect.size.height / 2));
                    if (attackType == kExplode)
                    {
                        m_monsterContainer.underThrowAttack(ts, true);
                    }
                    //爆炸后会在播放特效后清除抛出物，故不需要在这清除
                }
                else
                {
                    itr++;
                }
            }
        }
        else
        {
            //CCLog("%f, %f", (*itr)->getPosition().x, (*itr)->getPosition().y);
            itr++;
        }
    }
}

void BattleMapUnits::onThrowAttack()
{
    ThrowSpriteVec::iterator itr = m_throwSpriteVec.begin();
    while (itr != m_throwSpriteVec.end())
    {
        ThrowSprite* ts = *itr;
            kThrowAttackType attackType = ts->getAttackType();
        
        if (!ts->isExploding())//没爆炸
        {
            CCRect rect = CCRectZero;
            BattleUnitSprite* pThrower = ts->getThrower();
            if (pThrower != NULL && (pThrower->getUnitType() == kPartnerType ||pThrower->getUnitType() == kHero ))
            {
                rect = m_monsterContainer.underThrowAttack(ts, false);
                if (pThrower->getUnitType() == kHero&&!rect.equals(CCRectZero) )
                {
                    m_pSelf->SetComboInfo();
                }
            }
            else
            {
                // 攻击到了
                rect = m_friendContainer.underThrowAttack(ts, false);
            }
            
            if (!rect.equals(CCRectZero) && (attackType == kCollide || attackType == kExplode))
            {
                if (attackType == kExplode)
                {
                    ts->explode(CCPointMake(rect.origin.x + rect.size.width / 2, rect.origin.y + rect.size.height / 2));
                    m_monsterContainer.underThrowAttack(ts, true);
                    if (pThrower->getUnitType() == kHero&&!rect.equals(CCRectZero) )
                    {
                        m_pSelf->SetComboInfo();
                    }
                }
                else
                {
                    this->removeThrowSprite(ts);
                }
            }
            else
            {
                itr++;
            }
        }
        else
        {
            itr++;
        }
    }
}

// 计算攻击
void BattleMapUnits::onUnitAttack(BattleUnitSprite* pUnit)
{
    if (!pUnit)
    {
        return;
    }
    AttackInfo_t* pAttack = pUnit->getCurAttack();
    if (!pAttack)
    {
        return;
    }
    // 主角攻击
    if (m_pSelf == pUnit || m_pPartner == pUnit)
    {
        if (m_pSelf == pUnit)
        {
            m_monsterContainer.underAttack(pUnit, pAttack);

        }
        if (m_pPartner == pUnit)
        {
            //加入同伴攻击
            m_monsterContainer.underAttack(pUnit,pAttack);
        }
    }
    else // 怪物攻击
    {
        BattleUnitSprite *sprUnit = NULL;
        UNIT_CONTAINER_FOREACH(&m_friendContainer, sprUnit)
        {
            pUnit->attack(sprUnit, pAttack);
        }
    }
}

// 判断刷怪点卡屏及刷怪点出怪
void BattleMapUnits::judgeClipAndBornMonster()
{
    if (m_nBattleState == kBattleWin || m_nBattleState == kBattleLose || m_bStopCreateMonster)
    {
        return;
    }
    if (m_nBornMonsterFrequency != 0)
    {
        m_nBornMonsterFrequency--;
        return;
    }
    
    //小偷逻辑
    if (m_nCurMissionStageIdx > 0 && m_nCurMissionStageIdx <= m_ThiefItemVec.size() && m_nCurWaveIdx == 0 && m_nCurStageBornCnt == 0 && m_bGotBornPos == true)//现在按出怪的波数来出每一个小偷
    {
        item_unit_t& unit = m_ThiefItemVec.at(m_nCurMissionStageIdx - 1);
        MonsterUnitConf* pUnitConf = GameData::getMonsterConf(90006);
        ThiefSprite* pMonster = ThiefSprite::create(pUnitConf, pUnitConf, &unit);
        this->addChild(pMonster,kThiefOrder);
        float x = -50;//小偷只从左边出现
        m_monsterContainer.addMonster(pMonster);
        CCPoint poi =  MapCoord::screenToLayer(ccp(x, m_pMissionConf->fRolePosY));
        pMonster->setPosition(poi);
        pMonster->setAIActivated(true);
        if (pMonster->getPositionX() > m_pSelf->getPosition().x)
        {
            pMonster->setFlipX(true);
        }
    }
    
    //判断是否刷出排放怪，在一屏完成后进行判断，
    if (!m_bGotBornPos && !m_bHasBronFristMonster && m_nCurStageBornCnt == 0)
    {
        m_bHasBronFristMonster = true;
        CCPoint pt = CCPoint(m_pCurMissionStage->fClipScreenPosX,300);
        for (int i = 0; m_nCurStageBornCnt < m_pCurMissionStage->monsterBorn[m_nCurWaveIdx].size(); ++i)
        {
            MonsterUnit *info = &(m_pCurMissionStage->monsterBorn[m_nCurWaveIdx][i]);
            if (info->nBornType == 0)//摆放怪
            {
                if (m_nCurMissionStageIdx == 0)
                {
                    pt.x = SCREEN_WIDTH + 100 + rand()%50;
                }
                else
                {
                    pt.x = info->fBasePosX;
                }
                MonsterUnitConf *pUnitConf = GameData::getMonsterConf(info->nMonsterID);
                UnitBaseConf conf = *pUnitConf;
                caculateMonsterAttr(&conf);
                BattleUnitSprite* pMonster = MonsterSprite::create(&conf,pUnitConf);
                if (pMonster == NULL)
                {
                    break;
                }
                pMonster->setFlipX(true);
                pMonster->setPosition(pt);
                this->addChild(pMonster, (pMonster->getUnitType() == kBoss) ? kBossOrder : kMonsterOrder);
                m_monsterContainer.addMonster(pMonster);
                if (pMonster->getUnitType() == kBoss)
                {
                    char strmusic [64]= {0};
                    sprintf(strmusic, "music/bossmusic.mp3");
                    AudioResource::shareAudioResource()->playBgSound(strmusic);
                    CCNotificationCenter::sharedNotificationCenter()->postNotification(UNITHPCHANGE_NOTE, pMonster);
                }

//                if (pMonster->getUnitType() == kNormalMonster)
                {
                    ++m_nCurStageBornCnt;
                }
            }
            else
            {
                break;
            }
        }
    }
    
    //判断是否刷新怪
    if (judgeCreateMonster())
    {
        this->createMonster();
    }
}

//判断替换卡屏点 替换波数随从怪重生等
bool BattleMapUnits::judgeCreateMonster()
{
    // 没到出怪点，返回
    if (!m_bGotBornPos)
    {
        return false;
    }
    
    if (m_pCurMissionStage->monsterBorn.size()==0)
    {
        return false;
    }
    int monsterIndex = m_pMissionConf->getBornMonsterIndexInStage(m_pCurMissionStage, m_nCurWaveIdx, m_nCurStageBornCnt);//第n波的下个怪物生成
    int nextwaveIndex = m_pMissionConf->getWaveIndexInStage(m_pCurMissionStage,m_nCurWaveIdx);//下一波
    if (monsterIndex == -1)//所有怪物生成完毕
    {
        if (nextwaveIndex == -1)//当前卡屏波数结束 判断是否结束卡屏
        {
            if (m_monsterContainer.getMonsterCountByType(kNormalMonster) <= 0)//结束卡屏 重置
            {
                
                //没有到最后一屏,可以直接下一屏右边卡屏
                if (m_nCurMissionStageIdx < m_pMissionConf->missionStageVec.size() - 1)
                {
                    //解锁下一屏
                    if (m_pSelf->getScreenPosition().x > ROLE_RUNNING_RIGHT_CLIP_POS_X)
                    {
                        m_bPosOnRightClip = true;   //下一屏当前屏右边卡屏
                        m_bClipBothSide = false;    //是否两边都卡屏
                        m_nCurStageBornCnt = 0;     //当前屏出生怪物数量
                        m_nCurWaveIdx = 0;          //出怪波数重设
                        m_nCurMissionStageIdx++;    //进入下一屏
                        this->resetClipStageBothSide(false); // 只设置右边的卡屏点
                        
                        m_bGotBornPos = false;      //刷怪点
                        
                        // 第一波摆放怪
                        m_bHasBronFristMonster = false;
                        m_bHasFristMonsterAIStart = false;
                    }
                }
                else
                {
                    createFollowMonster();
                }
            }
            else//生成随从怪
            {
                createFollowMonster();
            }
        }
        else//当前卡屏波数未结束 准备加随从或下一波开始
        {
            if (m_monsterContainer.getMonsterCountByType(kNormalMonster) <= 1)
            {
                //下一波
                m_nCurWaveIdx++;
                m_nCurStageBornCnt = 0;
                m_deadMonsterVec.clear();
                m_monsterContainer.setFollowMonsterRelife(false);
            }
            else
            {
                createFollowMonster();
            }
        }
        return false;
    }
    return true;
}

BattleUnitSprite* BattleMapUnits::createEmeryHero(UnitBaseConf *info,uint32_t *skill)
{
    if (m_nBattleState != kBattleing)
    {
        return NULL;
    }
    EnemyHeroSprite* pMonster = EnemyHeroSprite::create(info,skill);
    if (pMonster == NULL)
    {
        return NULL;
    }
    this->setPosition(ccp((SCREEN_WIDTH-1900)/2,0));//设置地图位置
    CCPoint poi =  ccp(1300, 201);
    pMonster->setCenterPosition(poi);
    pMonster->setAIActivated(true);
    if (pMonster->getPositionX() > m_pSelf->getPosition().x)
    {
        pMonster->setFlipX(true);
    }
    this->addChild(pMonster,kMonsterOrder);
    m_monsterContainer.addMonster(pMonster);
    return pMonster;
}

void BattleMapUnits::caculateMonsterAttr(UnitBaseConf *conf)
{
    float talent[6] = {0};
    gUserData->getParnterTalent(talent);
    conf->fInAttack = conf->fInAttack*(1 - talent[1]);
    conf->fOutAttack = conf->fOutAttack*(1 - talent[2]);
    conf->fInDef = conf->fInDef*(1 - talent[3]);
    conf->fOutDef = conf->fOutDef*(1 - talent[4]);
    conf->nLife = conf->nLife*(1 - talent[5]);
}

BattleUnitSprite* BattleMapUnits::createWorldBoss(UnitBaseConf *info)
{
    if (m_nBattleState != kBattleing)
    {
        return NULL;
    }
    MonsterUnitConf *pUnitConf = GameData::getMonsterConf(90008);
    memcpy(info->unitName, pUnitConf->unitName, sizeof(pUnitConf->unitName));
    info->nResourceID = pUnitConf->nResourceID;
    info->nAIID = pUnitConf->nAIID;
    info->nUnitType = pUnitConf->nUnitType;
    info->nUnitID = pUnitConf->nUnitID;
    BattleUnitSprite* pMonster = MonsterSprite::create(info,pUnitConf);
    if (pMonster == NULL)
    {
        return NULL;
    }
    CCPoint poi =  ccp(1200, 201);
    pMonster->setCenterPosition(poi);
    pMonster->setAIActivated(true);
    if (pMonster->getPositionX() > m_pSelf->getPosition().x)
    {
        pMonster->setFlipX(true);
    }
    if (pMonster->getUnitType() == kBoss)
    {
        char strmusic [64]= {0};
        sprintf(strmusic, "music/bossmusic.mp3");
        AudioResource::shareAudioResource()->playBgSound(strmusic);
        CCNotificationCenter::sharedNotificationCenter()->postNotification(UNITHPCHANGE_NOTE, pMonster);
    }
    this->addChild(pMonster,kMonsterOrder);
    m_monsterContainer.addMonster(pMonster);
    return pMonster;
}

void BattleMapUnits::createMonster()
{
    if (m_nBattleState != kBattleing || GameManager::sharedInstance()->getBattleLayer()->getBattleType() == kWorldBoss)
    {
        return;
    }
    
    MonsterUnit *info = (&(m_pCurMissionStage->monsterBorn[m_nCurWaveIdx][m_nCurStageBornCnt]));
    MonsterUnitConf *pUnitConf = GameData::getMonsterConf(info->nMonsterID);
    UnitBaseConf conf = *pUnitConf;
    
    caculateMonsterAttr(&conf);
    
    BattleUnitSprite* pMonster = MonsterSprite::create(&conf,pUnitConf);
    if (pMonster == NULL)
    {
        return;
    }
    
    if (pMonster->getUnitType() == kBoss)
    {
        char strmusic [64]= {0};
        sprintf(strmusic, "music/bossmusic.mp3");
        AudioResource::shareAudioResource()->playBgSound(strmusic);
        CCNotificationCenter::sharedNotificationCenter()->postNotification(UNITHPCHANGE_NOTE, pMonster);
    }
    CCPoint poi;
    float x = 0;
    if (info->nBornType == 3)
    {
        x = info->pos.x;
        poi = info->pos;
        if (pMonster->getUnitType() != kTree) pMonster->setAngryTime(999999);
    }
    else if (info->nBornType == 4)
    {
        x = -100;
        poi =  ccp(x, m_pMissionConf->fRolePosY);
        pMonster->setAngryTime(999999);
    }
    else
    {
        x = info->isRight== 0?-50:info->isRight== 1?SCREEN_WIDTH + 50:rand()%2 == 0?-50:SCREEN_WIDTH + 50;//0左边 1右边 2随机屏幕两边出怪
        poi =  MapCoord::screenToLayer(ccp(x, m_pMissionConf->fRolePosY));
    }
    TrapSprite *trapSpr = m_trapContainer.getOverlappingSpr(poi.x,pMonster->getUnitRect());
    if (trapSpr)
    {
        if (x >0)
        {
            poi.x = trapSpr->getPositionX() + trapSpr->getUnitRect().size.width + pMonster->getUnitRect().size.width/2 + 50;
        }
        else
        {
            poi.x = trapSpr->getPositionX() - trapSpr->getUnitRect().size.width - pMonster->getUnitRect().size.width/2 - 50;
        }
        
    }
    pMonster->setPosition(poi);
    pMonster->setAIActivated(true);
    if (pMonster->getPositionX() > m_pSelf->getPosition().x)
    {
        pMonster->setFlipX(true);
    }
    
    if (info->nBornType == 2)   //跳跃刷怪
    {
        pMonster->changeAction(kRoll);
    }
    
    this->addChild(pMonster,kMonsterOrder);
    m_monsterContainer.addMonster(pMonster);
    m_nCurStageBornCnt++;
    
    m_nBornMonsterFrequency = 60*m_pCurMissionStage->fRefreshTime + rand() % 5;
}

// 根据信息刷新随从怪
void BattleMapUnits::createFollowMonster()
{
    if (m_deadMonsterVec.size()>0)
    {
        m_nBornFollowMonsterFrequency ++;
        if (m_nBornFollowMonsterFrequency >= 180 && m_nKillCnt < m_pMissionConf->winCondition.nKillCnt)//3秒出一次随从
        {
            FollowMonsterInfoVec::iterator itr = m_deadMonsterVec.begin();
            MonsterUnitConf *pUnitConf = *itr;
            m_deadMonsterVec.erase(itr);
            m_nBornFollowMonsterFrequency = 0;
            UnitBaseConf conf = *pUnitConf;
            caculateMonsterAttr(&conf);
            BattleUnitSprite* pMonster = MonsterSprite::create(&conf,pUnitConf);
            if (!pMonster)
            {
                CCLOG("BattleMapUnits::createMonsterWithMonsterID error monsterID!");
                return ;
            }
            
            float x = rand()%2?SCREEN_WIDTH + 50: -50;
            
            CCPoint poi =  MapCoord::screenToLayer(ccp(x, m_pMissionConf->fRolePosY));
            TrapSprite *trapSpr = m_trapContainer.getOverlappingSpr(poi.x,pMonster->getUnitRect());
            if (trapSpr)
            {
                if (x >0)
                {
                    poi.x = trapSpr->getPositionX() + trapSpr->getUnitRect().size.width + pMonster->getUnitRect().size.width/2 + 50;
                }
                else
                {
                    poi.x = trapSpr->getPositionX() - trapSpr->getUnitRect().size.width - pMonster->getUnitRect().size.width/2 - 50;
                }
            }
            pMonster->setPosition(poi);
            
            if (pMonster->getPosition().x > m_pSelf->getPosition().x)
            {
                pMonster->setFlipX(true);
            }
            
            pMonster->setAIActivated(true);
            this->addChild(pMonster,kMonsterOrder);
            m_monsterContainer.addMonster(pMonster);
        }
    }
}

void BattleMapUnits::updateMonsters(float dt)
{
    BattleUnitSprite* pMonster = NULL;
    MONSTER_CONTAINER_FOREACH(&m_monsterContainer, pMonster)
    {

        pMonster->update();
        if (pMonster->getHp() > 0)
        {
            if (this->m_nBattleState == kBattleing)
            {
                    BattleUnitSprite* pSprite = NULL;
                    BattleUnitSprite* pFriendSprite = NULL;
                    int nMinDistance = 5000;
                    UNIT_CONTAINER_FOREACH(&m_friendContainer, pFriendSprite)
                    {
                        int nDistance = ccpDistance(pMonster->getPosition(), pFriendSprite->getPosition());
                        if(nMinDistance > nDistance)
                       {
                           pSprite = pFriendSprite;
                           nMinDistance = nDistance;
                       }
                    }
                    pMonster->runAIOnce(pSprite);
            }
            else
            {
                pMonster->stopRunning();
            }
        }
    }
}

void BattleMapUnits::updateFriend(float dt)
{
    BattleUnitSprite* pFriendSprite = NULL;
    UNIT_CONTAINER_FOREACH(&m_friendContainer, pFriendSprite)
    {
        if (pFriendSprite != m_pSelf)
        {
            // 更新友方坐标点及AI
            if (pFriendSprite != NULL)
            {
                pFriendSprite->update();
                if (this->m_nBattleState == kBattleing)
                {
                    ((PartnerSprite*)pFriendSprite)->runAIOnce(m_pSelf, this->getNearestMonster(pFriendSprite,0.9*SCREEN_WIDTH,true));
                }
                else
                {
                    pFriendSprite->stopRunning();
                }
            }
        }
        else
        {
            pFriendSprite->runAIOnce(getNearestMonster(m_pSelf,3*SCREEN_WIDTH,false));
        }
    }
}

void BattleMapUnits::createPartner(float dt)
{
    if (m_nCurPartnerIdx >= gUserData->m_partnerDic->count())
    {
        return;
    }
    PartnerData *pPartnerdata = (PartnerData*)(gUserData->m_partnerDic->objectForKey(m_nCurPartnerIdx + 1));
    if (!pPartnerdata)
    {
        return;
    }
    pPartnerdata->refreshBaseAttr();
    UnitBaseConf* pPartnerConf = &(pPartnerdata->baseAttr);
    int lv = pPartnerdata->skillLv;
    BattleCacheManager::sharedInstance()->loadPartnerResource(pPartnerConf->nResourceID);
    PartnerSprite *partner = PartnerSprite::create(pPartnerConf,lv);
    m_pPartner = partner;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(UNITHPCHANGE_NOTE, partner);
    if (m_nCurPartnerIdx!= 0)
    {
        CCAnimation *ani = CRUtils::createAni("born_", 1, 21);
        CCSprite *aniSpr = CCSprite::create();
        aniSpr->setScale(2);
        aniSpr->setAnchorPoint(ccp(0.5,0));
        aniSpr->setPosition(partner->getFootPosition());
        aniSpr->setPositionY(aniSpr->getPositionY()-100);
        aniSpr->runAction(CCSequence::create(CCAnimate::create(ani),CCRemoveSelf::create(),NULL));
        partner->addChild(aniSpr);
        m_monsterContainer.setMonsterBehitFly();
    }
    CCPoint poi = MapCoord::screenToLayer(ccp(200, 200));
    TrapSprite *trapSpr = m_trapContainer.getOverlappingSpr(poi.x,partner->getUnitRect());
    if (trapSpr)
    {
        poi.x = trapSpr->getPositionX() + trapSpr->getUnitRect().size.width + partner->getUnitRect().size.width/2 + 50;
    }
    partner->setCenterPosition(poi);
    partner->setAIActivated(true);
    this->addChild(partner, kPartnerOrder);
    m_friendContainer.addUnit(partner);
    m_nCurPartnerIdx ++;
}

void BattleMapUnits::addDropItem(int nItemId, int nItemCount, CCPoint position, bool bIsThief)
{
    DropItem* pItem = DropItem::create(nItemId, nItemCount, bIsThief);
    if (pItem)
    {
        this->addChild(pItem, kHitTipOrder);
        //掉落起始位置：下面的负数调掉落物的起始位置
        pItem->setPosition(ccpAdd(position, ccp(0, -80)));
        m_pDropItemArray->addObject(pItem);
        
        this->addToItemMap(pItem);
    }
}

void BattleMapUnits::addDropItem(std::vector<itemDrop_t>* pDropItemVec, CCPoint position, bool bIsThief)
{
    int rate = rand() % 100;
    if (pDropItemVec != NULL && pDropItemVec->size() != 0)
    {
        for (std::vector<itemDrop_t>::iterator it = pDropItemVec->begin(); it != pDropItemVec->end(); it++)
        {
            if (rate < it->fRate || bIsThief)
            {
                switch (it->nItemID)
                {
                    case -1://啥都不掉
                        return;
                        break;
                    default://其它的都算物品
                    {
                        int itemCount = rand() % it->nItemCnt + 1;
                        itemCount = this->getDropItemCount(it->nItemID, itemCount);
                        if (itemCount > 0 || bIsThief)
                        {
                            CCPoint screenPos = MapCoord::layerToScreen(position);
                            if (screenPos.x > ROLE_RIGHT_CLIP_POS_X)
                            {
                                position = ccpAdd(position, ccp(ROLE_RIGHT_CLIP_POS_X - screenPos.x, 0));
                            }
                            else if (screenPos.x <= ROLE_LEFT_CLIP_POS_X)
                            {
                                position = ccpAdd(position, ccp(ROLE_LEFT_CLIP_POS_X - screenPos.x, 0));
                            }
                            if (m_pMissionConf->bFancy)//如果是奖励关卡，拆分为5个Item掉落，增强视觉效果
                            {
                                int count = 5;
                                for (int i = 0; i < count; i++)
                                {
                                    this->addDropItem(it->nItemID, (bIsThief ? it->nItemCnt : itemCount) / count, ccpAdd(position, ccp((i - count / 2) * 20, 0)), bIsThief);
                                }
                            }
                            else
                            {
                                this->addDropItem(it->nItemID, bIsThief ? it->nItemCnt : itemCount, position, bIsThief);
                            }
                        }
                        return;
                    }
                        break;
                }
            }
            else
            {
                rate -= it->fRate;
            }
        }
    }
}

void BattleMapUnits::addToItemMap(DropItem* pItem)
{
    int itemId = pItem->GetItemID();
    int itemCount = pItem->GetItemCount();
    if (pItem->IsThief())
    {
        if (m_thiefItemMap.find(itemId) != m_thiefItemMap.end())
        {
            m_thiefItemMap[itemId] += itemCount;
        }
        else
        {
            m_thiefItemMap.insert(std::pair<int, int>(itemId, itemCount));
        }
        if (itemId == 0)//如果是银两
        {
            m_nGetYxb += itemCount;
        }
        else if (itemId == 2)//如果是黄金
        {
            m_nGetGold += itemCount;
        }
    }
    else
    {
        if (itemId == 0)//如果是银两
        {
            m_nGetYxb += itemCount;
        }
        else
        {
            if (m_dropItemMap.find(itemId) != m_dropItemMap.end())
            {
                m_dropItemMap[itemId] += itemCount;
            }
            else
            {
                m_dropItemMap.insert(std::pair<int, int>(itemId, itemCount));
            }
        }
    }
}

void BattleMapUnits::addSelfExp(int nID, int nDropExp)
{
    m_nGetExp += nDropExp;
    if (m_dieMonsterMap.find(nID) != m_dieMonsterMap.end())
    {
        m_dieMonsterMap[nID]++;
    }
    else
    {
        m_dieMonsterMap[nID] = 1;
    }
}

// 怪死亡
void BattleMapUnits::removeBattleUnit(BattleUnitSprite* pUnit)
{
    if (pUnit == NULL)
    {
        return;
    }
    BattleUnitSprite* pOtherUnit;
    MONSTER_CONTAINER_FOREACH(&m_monsterContainer, pOtherUnit)
    {
        pOtherUnit->clearUnserAttacked(pUnit);
    }
    
    pOtherUnit = NULL;
    UNIT_CONTAINER_FOREACH(&m_friendContainer, pOtherUnit)
    {
        pOtherUnit->clearUnserAttacked(pUnit);
    }
    
    if (pUnit == m_pSelf ||pUnit->getUnitType() == kEnemyHero)
    {
        
    }
    else if (pUnit->getUnitType() == kPartnerType)
    {
        m_friendContainer.removeUnit(pUnit);
        pUnit->removeFromParentAndCleanup(true);
        pUnit = NULL;
        scheduleOnce(schedule_selector(BattleMapUnits::createPartner), 2);
    }
    else
    {
        if (pUnit->getUnitType() == kFollowMonster)//随从怪 保存配置
        {
            MonsterSprite *pFollowMoster = (MonsterSprite*)pUnit;
            if (pFollowMoster->getreLife())
            {
                MonsterUnitConf *pinfo = pFollowMoster->getMonsterConf();
                m_deadMonsterVec.push_back(pinfo);
            }
            m_trapContainer.remove(pUnit);
            m_monsterContainer.removeMonster(pUnit);
        }
        else if (pUnit->getUnitType() == kBox || pUnit->getUnitType() == kBombBox)
        {
            TrapUnit* pConf = dynamic_cast<TrapSprite*>(pUnit)->getTrapUnit();
            this->addDropItem(&pConf->dropItemVec, pUnit->getPosition(), false);
            m_trapContainer.remove(pUnit);
        }
        else if (pUnit->getUnitType() == kThief)
        {
            item_unit_t* pConf = dynamic_cast<ThiefSprite*>(pUnit)->getDropItem();
            itemDrop_t t;
            t.nItemID = pConf->item_id;
            t.nItemCnt = pConf->item_cnt;
            t.fRate = 100;
            std::vector<itemDrop_t> dropItemVec;
            dropItemVec.clear();
            dropItemVec.push_back(t);
            this->addDropItem(&dropItemVec, pUnit->getPosition(), true);
            m_monsterContainer.removeMonster(pUnit);
        }
        else
        {
            if (pUnit->getUnitType() == kNormalMonster ||pUnit->getUnitType() ==kBoss)
            {
                MonsterUnitConf* pConf = dynamic_cast<MonsterSprite*>(pUnit)->getMonsterConf();
                if (pConf)
                {
                    this->addDropItem(&pConf->dropItemVec, pUnit->getPosition(), false);
                }
            }
            m_trapContainer.remove(pUnit);
            m_monsterContainer.removeMonster(pUnit);
        }
        pUnit->removeFromParentAndCleanup(true);
        pUnit = NULL;
    }
}

const static int s_fps = 15;
void BattleMapUnits::checkWin(BattleUnitSprite* pSprite)
{
    if (pSprite != m_pSelf)
    {
        // 减1是因为还么有从容器中remove掉
        if(m_pMissionConf->winCondition.nWinType == pSprite->getUnitType())
        {
            m_nKillCnt++;
        }
        if (m_nKillCnt == m_pMissionConf->winCondition.nKillCnt)
        {
            m_monsterContainer.setMonsterDeath();
            CCNotificationCenter::sharedNotificationCenter()->postNotification(BATTLEEND_NOTE,CCString::create("1"));
            CCDirector::sharedDirector()->setAnimationInterval(1.0 / s_fps);
            this->scheduleOnce(schedule_selector(BattleMapUnits::battleWin), 3);
            m_bStopCreateMonster = true;
        }
    }
    else
    {
        CCNotificationCenter::sharedNotificationCenter()->postNotification(BATTLEEND_NOTE,CCString::create("0"));
        CCDirector::sharedDirector()->setAnimationInterval(1.0 / s_fps);
        this->scheduleOnce(schedule_selector(BattleMapUnits::showLosePanel),3);
        m_bStopCreateMonster = true;
    }
}

void BattleMapUnits::battleWin(float dt)
{
    if (m_nBattleState == kBattleing)
    {
        setBattleState(kBattleWin);
        
        BattleUnitSprite* pOtherUnit = NULL;
        UNIT_CONTAINER_FOREACH(&m_friendContainer, pOtherUnit)
        {
            pOtherUnit->stopRunning();
            pOtherUnit->setAIActivated(false);
        }

        CCDirector::sharedDirector()->setAnimationInterval(1.0 / 60);
            int taskID = gUserDataEx->getTaskClipByMissionID(m_pMissionConf->nMissionId,false);
            if (taskID == -1)
            {
                showWinPanel();
            }
            else
            {
                MovieClip *clip = MovieClip::create(taskID, kBehindClip, this);
                TOPADDCHILD(clip);
            }
    }
}

void BattleMapUnits::onMovieOver(bool isOver)
{
    CCString *str = CCString::createWithFormat("%d",isOver);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(BATTLEUIVISABLE_NOTE,str);
    if (m_nBattleState == kBattleWin && isOver)
    {
        showWinPanel();
    }
    else if(m_nBattleState == kBattleing)
    {
        if (isOver)
        {
            m_isStop = false;
        }
        else
        {
            m_isStop = true;
        }
    }
}

void BattleMapUnits::showWinPanel()
{
    //CCNotificationCenter::sharedNotificationCenter()->postNotification(BATTLEWIN_NOTE);
}

void BattleMapUnits::showLosePanel(float dt)
{
    if (getBattleState() == kBattleing)
    {
        AudioResource::shareAudioResource()->stopBgSound();
        CCNotificationCenter::sharedNotificationCenter()->postNotification(BATTLELOST_NOTE);
        setBattleState(kBattleLose);
        CCDirector::sharedDirector()->setAnimationInterval(1.0 / 60);
    }
}

void BattleMapUnits::limitExp()
{
    for (std::vector<item_unit_t>::iterator it = m_ItemLimitVec.begin(); it != m_ItemLimitVec.end(); it++)
    {
        if (it->item_id == 1 && m_nGetExp > it->item_cnt)
        {
            m_nGetExp = it->item_cnt;
            break;
        }
    }
}

int BattleMapUnits::getDropItemCount(int nItemID, int nItemCount)
{
    for (std::vector<item_unit_t>::iterator it = m_ItemLimitVec.begin(); it != m_ItemLimitVec.end(); it++)
    {
        if (it->item_id == nItemID)
        {
            if (it->item_cnt < nItemCount)
            {
                nItemCount = it->item_cnt;
            }
            it->item_cnt -= nItemCount;
            return nItemCount;
        }
    }
    return nItemCount;
}

void BattleMapUnits::getKillMonsters(uint32_t* pMonster)
{
    //kNeutralMonster = 1,    //中立怪，杂碎，不用杀死即可过关
    //kNormalMonster = 2,         //普通怪物，
    //kFollowMonster = 3,         //随从怪物
    //kBoss = 4,              //boss
    for (int i = 0; i < 4; i++)
    {
        pMonster[i] = m_monsterContainer.getKillMonsterSum(i + 1);//由于怪类型的范围是从1到4，故这儿需要加1
    }
}

void BattleMapUnits::setConfig(std::vector<item_unit_t>* pItemVec, std::vector<item_unit_t>* pThiefItemVec)
{
    m_ItemLimitVec.clear();
    m_ItemLimitVec.assign(pItemVec->begin(), pItemVec->end());
    m_ThiefItemVec.clear();
    if (pThiefItemVec != NULL)  m_ThiefItemVec.assign(pThiefItemVec->begin(), pThiefItemVec->end());
}

void BattleMapUnits::updateComboControlPos()
{
    if (m_pComboControl == NULL)
    {
        m_pComboControl = ComboControl::create();
        this->addChild(m_pComboControl, kLandBgOrderMax);
        m_pComboControl->setAnchorPoint(ccp(0.5, 0.5));
    }
    m_pComboControl->setCombo(m_pSelf->getCombo());
    if (m_pComboControl->isVisible())
    {
        m_pComboControl->setPosition(MapCoord::screenToLayer(ccp(SCREEN_WIDTH - 150, 360)));
    }
}

void BattleMapUnits::updateGoForwardsTipPos()
{
    if (getBattleState() != kBattleing)
    {
        this->unschedule(schedule_selector(BattleMapUnits::showGoForwardsTip));
        m_bShowGoForwardsTip = false;
        if (m_pGoForwardsTip != NULL)
        {
            m_pGoForwardsTip->stopAllActions();
            m_pGoForwardsTip->setVisible(false);
        }
    }
    
    if (m_pCurMissionStage->monsterBorn.size()>0 &&getBattleState() == kBattleing)
    {
        int monsterIndex = m_pMissionConf->getBornMonsterIndexInStage(m_pCurMissionStage, m_nCurWaveIdx, m_nCurStageBornCnt);//第n波的下个怪物生成
        int nextwaveIndex = m_pMissionConf->getWaveIndexInStage(m_pCurMissionStage,m_nCurWaveIdx);//下一波
        if (((monsterIndex == -1&&
              nextwaveIndex == -1&&
              m_monsterContainer.getMonsterCountByType(kNormalMonster) <= 0&&
              m_nCurMissionStageIdx+1!= m_pMissionConf->missionStageVec.size())||
             (m_bGotBornPos==false &&m_nCurWaveIdx == 0))&&
            0<m_pMissionConf->missionStageVec.size())//当前卡屏结束  或者下屏开启 但还未到刷怪点
        {
            if (!m_bShowGoForwardsTip)
            {
                this->schedule(schedule_selector(BattleMapUnits::showGoForwardsTip), 3.0f);
                m_bShowGoForwardsTip = true;
            }
        }
        else
        {
            if (m_bShowGoForwardsTip)
            {
                this->unschedule(schedule_selector(BattleMapUnits::showGoForwardsTip));
                m_bShowGoForwardsTip = false;
                if (m_pGoForwardsTip != NULL)
                {
                    m_pGoForwardsTip->stopAllActions();
                    m_pGoForwardsTip->setVisible(false);
                }
            }
        }
    }
    else
    {
        if (m_bShowGoForwardsTip)
        {
            this->unschedule(schedule_selector(BattleMapUnits::showGoForwardsTip));
            m_bShowGoForwardsTip = false;
            if (m_pGoForwardsTip != NULL)
            {
                m_pGoForwardsTip->stopAllActions();
                m_pGoForwardsTip->setVisible(false);
            }
        }
    }
    
    if (m_pGoForwardsTip != NULL && m_pGoForwardsTip->isVisible())
    {
        m_pGoForwardsTip->setPosition(MapCoord::screenToLayer(ccp(SCREEN_WIDTH - 80, SCREEN_HEIGTH * 2 / 3)));
    }
}

void BattleMapUnits::showGoForwardsTip(float dt)
{
    if (m_pGoForwardsTip == NULL)
    {
        m_pGoForwardsTip = CCSprite::create("BattleUI/go_forwards.png");
        this->addChild(m_pGoForwardsTip, kLandBgOrderMax);
        m_pGoForwardsTip->setAnchorPoint(ccp(0.5, 0.5));
        m_pGoForwardsTip->setVisible(false);
    }
    m_pGoForwardsTip->setVisible(true);
    CCFiniteTimeAction* action = CCSequence::create(CCBlink::create(2, 5), CCHide::create(), NULL);
    m_pGoForwardsTip->runAction(action);
}

// 地图震动
void BattleMapUnits::mapShark()
{
    CCShark * pShark = CCShark::create(0.5, this->getPosition(), 10, 2, false);
    pShark->setTag(MapSharkActionTag);
    this->runAction(pShark);
}

bool BattleMapUnits::isInterSect(cocos2d::CCRect unitRect, float moveX, float moveY, float* pCorrectX, float* pCorrectY, kObstacleDirection* pObstacleDirection)
{
    if (m_trapContainer.isInterSect(unitRect, moveX, moveY, pCorrectX, pCorrectY)) return true;
    
    CCPoint point = unitRect.origin;
    CCSize size = unitRect.size;
    
    CCRect rect = CCRectMake(point.x + moveX, point.y + moveY, size.width, size.height);
    for (int i = 0; i < m_mapObstacleVec.size(); i++)
    {
        MapObstacle* obstacle = m_mapObstacleVec.at(i);
        
        CCRect rect1 = obstacle->getCollideRect();
        if (rect1.intersectsRect(rect))
        {
            kObstacleDirection obstacleDirection = obstacle->getObstacleDirection();
            if (obstacleDirection != kObstacleNone)
            {
                switch (obstacleDirection)
                {
                    case kObstacleUp:
                        *pCorrectY = rect1.getMinY() - 0.0001;
                        break;
                    case kObstacleDown:
                        *pCorrectY = rect1.getMaxY() + 0.0001;
                        break;
                    case kObstacleLeft:
                        *pCorrectX = rect1.getMaxX() + 0.0001;
                        break;
                    case kObstacleRight:
                        *pCorrectX = rect1.getMinX() - 0.0001;
                        break;
                    default:
                        break;
                }
                *pObstacleDirection = obstacleDirection;
            }
            else
            {
                if (moveX > 0)
                {
                    *pCorrectX = rect1.getMinX() - 0.0001;
                }
                else if (moveX < 0)
                {
                    *pCorrectX = rect1.getMaxX() + 0.0001;
                }
                if (moveY > 0)
                {
                    *pCorrectY = rect1.getMinY() - 0.0001;
                }
                else if (moveY < 0)
                {
                    *pCorrectY = rect1.getMaxY() + 0.0001;
                }
            }
            return true;
        }
    }
    return false;
    
}

kObstacleDirection BattleMapUnits::checkDirection(CCRect unitRect, float moveX, float moveY, float* pCorrectX, float* pCorrectY)
{
    if (IS_FLOAT_ZERO(moveX) && IS_FLOAT_ZERO(moveY))
    {
        return kObstacleNone;
    }
    kObstacleDirection obstacleDirection = kObstacleNone;
    if (!this->isInterSect(unitRect, moveX, moveY, pCorrectX, pCorrectY, &obstacleDirection))
    {
        return kObstacleNone;
    }
    else
    {
        if (obstacleDirection != kObstacleNone)
        {
            return obstacleDirection;
        }
        if (IS_FLOAT_ZERO(moveX))
        {
            if (moveY > 0)
            {
                return kObstacleUp;
            }
            else if (moveY < 0)
            {
                return kObstacleDown;
            }
        }
        else if (IS_FLOAT_ZERO(moveY))
        {
            if (moveX > 0)
            {
                return kObstacleRight;
            }
            else if (moveX < 0)
            {
                return kObstacleLeft;
            }
        }
        else
        {
            if (!this->isInterSect(unitRect, 0, moveY, pCorrectX, pCorrectY, &obstacleDirection))
            {
                if (moveX > 0)
                {
                    return kObstacleRight;
                }
                else if (moveX < 0)
                {
                    return kObstacleLeft;
                }
            }
            else if (!this->isInterSect(unitRect, moveX, 0, pCorrectX, pCorrectY, &obstacleDirection))
            {
                if (moveY > 0)
                {
                    return kObstacleUp;
                }
                else if (moveY < 0)
                {
                    return kObstacleDown;
                }
            }
            else
            {
                kObstacleDirection dir1;
                if (moveX > 0)
                {
                    dir1 = kObstacleRight;
                }
                else if (moveX < 0)
                {
                    dir1 = kObstacleLeft;
                }
                kObstacleDirection dir2;
                if (moveY > 0)
                {
                    dir2 = kObstacleUp;
                }
                else if (moveY < 0)
                {
                    dir2 = kObstacleDown;
                }
                return (kObstacleDirection)(dir1 | dir2);
            }
        }
    }
    return kObstacleNone;
}

// 重定为卡屏点
void BattleMapUnits::resetClipStageBothSide(bool bBothSide)
{
    m_pCurMissionStage = &(m_pMissionConf->missionStageVec.at(m_nCurMissionStageIdx));
    if (bBothSide)
    {
        m_fLeftClipX = m_pCurMissionStage->fClipScreenPosX - m_pCurMissionStage->fClipScreenLength;
        if (m_fLeftClipX < 0)
        {
            m_fLeftClipX = 0;
        }
    }
    
    m_fRightClipX = m_pCurMissionStage->fClipScreenPosX + m_pCurMissionStage->fClipScreenLength;
    if (m_fRightClipX > m_fMapLength)
    {
        m_fRightClipX = m_fMapLength;
    }
}

BattleUnitSprite* BattleMapUnits::getNearestMonster(BattleUnitSprite* unitSprite,float fdistance,bool bInScreen)
{
    float x = unitSprite->getCenterPosition().x;
    BattleUnitSprite* pNear = NULL;
    BattleUnitSprite* pMonster = NULL;
    MONSTER_CONTAINER_FOREACH(&m_monsterContainer, pMonster)
    {
        CCPoint centerPos = pMonster->getCenterPosition();
        CCPoint screenPoint = pMonster->getScreenPosition();
        if (pMonster->getHp())
        {
            if (bInScreen == true)
            {
                if (screenPoint.x < SCREEN_WIDTH && screenPoint.x > 0)
                {
                    float dis = fabsf(centerPos.x - x);
                    if (dis < fdistance)
                    {
                        pNear = pMonster;
                        fdistance = dis;
                        if (unitSprite->isFlipX() == (pNear->getPosition().x < unitSprite->getPosition().x)&&
                            unitSprite->getAttackDis() > dis &&
                            dis>10)//如果方向是敌人 并且距离小于攻击范围 直接选取该敌人
                        {
                            break;
                        }
                    }
                }
            }
            else
            {
                if (pNear)
                {
                    CCPoint screenPoint1 = pNear->getScreenPosition();
                    if ((screenPoint1.x > SCREEN_WIDTH || screenPoint1.x < 0) && screenPoint.x < SCREEN_WIDTH && screenPoint.x > 0)
                    {
                        pNear = pMonster;
                    }
                    else if((screenPoint.x > SCREEN_WIDTH || screenPoint.x < 0) && screenPoint1.x < SCREEN_WIDTH && screenPoint1.x > 0)
                    {
                        continue;
                    }
                    else
                    {
                        float dis = fabsf(centerPos.x - x);
                        if (dis < fdistance)
                        {
                            pNear = pMonster;
                            fdistance = dis;
                            if (unitSprite->isFlipX() == (pNear->getPosition().x < unitSprite->getPosition().x)&&
                                unitSprite->getAttackDis() > dis &&
                                dis>10)//如果方向是敌人 并且距离小于攻击范围 直接选取该敌人
                            {
                                break;
                            }
                        }
                    }
                }
                else
                {
                    pNear = pMonster;
                }
            }
        }
    }
    
    TrapSpriteVec *pTrapVec = m_trapContainer.getTrapSpriteVec();
    for (int i = 0; i<pTrapVec->size(); i++)
    {
        TrapSprite *pTrapSprite = pTrapVec->at(i);
        CCPoint centerPos = pTrapSprite->getCenterPosition();
        CCPoint screenPoint = pTrapSprite->getScreenPosition();
        if (pTrapSprite->getHp() > 0 && screenPoint.x < SCREEN_WIDTH && screenPoint.x > 0 && pTrapSprite->getUnitBaseConf()->nUnitType == kBox)
        {
            float dis = fabsf(centerPos.x - x);
            if (dis < fdistance)
            {
                pNear = pTrapSprite;
                fdistance = dis;
            }
        }
    }
    return pNear;
}

void BattleMapUnits::setAttacRectShow(bool bShow)
{
    if (m_pSelf)
    {
        m_pSelf->setAttacRectVisible(bShow);
    }
    
    m_monsterContainer.setAttacRectShow(bShow);
    
    ThrowSpriteVec::iterator itr = m_throwSpriteVec.begin();
    while (itr != m_throwSpriteVec.end())
    {
        ThrowSprite* ts = *itr;
        ts->setAttacRectVisible(bShow);
        itr++;
    }
}

void BattleMapUnits::setMonsterCountTips()
{
    if (m_pMonsterCountTips == NULL)
    {
        m_pMonsterCountTips = CCLabelTTF::create("", "Arial", 30);
        m_pMonsterCountTips->setColor(ccYELLOW);
        m_pMonsterCountTips->setAnchorPoint(ccp(0, 0.5));
        m_pMonsterCountTips->setPosition(MapCoord::screenToLayer(ccp(10, /*SCREEN_HEIGTH - */10)));
        m_pMonsterCountTips->setVisible(false);
        this->addChild(m_pMonsterCountTips, kLandBgOrderMax);
    }
    char tips[128] = {0};
    sprintf(tips, "普通怪物总数:%d,刷怪数:%d,杀死数:%d | 杂碎(%d/%d) 普通(%d/%d) 随从(%d/%d) boss(%d/%d)", 10,
            m_monsterContainer.getBornMonsterSum(kNormalMonster) + m_monsterContainer.getBornMonsterSum(kNeutralMonster),
            m_monsterContainer.getBornMonsterSum(kNormalMonster) + m_monsterContainer.getBornMonsterSum(kNeutralMonster) - m_monsterContainer.getMissionMonster(),
            m_monsterContainer.getBornMonsterSum(kNeutralMonster), m_monsterContainer.getMonsterCountByType(kNeutralMonster),
            m_monsterContainer.getBornMonsterSum(kNormalMonster), m_monsterContainer.getMonsterCountByType(kNormalMonster),
            m_monsterContainer.getBornMonsterSum(kFollowMonster), m_monsterContainer.getMonsterCountByType(kFollowMonster),
            m_monsterContainer.getBornMonsterSum(kBoss), m_monsterContainer.getMonsterCountByType(kBoss)
            );
    m_pMonsterCountTips->setPosition(MapCoord::screenToLayer(ccp(10, /*SCREEN_HEIGTH - */10)));
    m_pMonsterCountTips->setString(tips);
}