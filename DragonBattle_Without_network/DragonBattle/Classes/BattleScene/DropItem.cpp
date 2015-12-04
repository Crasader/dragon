//
//  DropItem.cpp
//  DragonBattle
//
//  Created by 何 永翔 on 13-7-23.
//
//

#include "DropItem.h"
#include "BattleMapUnits.h"
#include "GameManager.h"

#define ANIMATE_TAG (250)
#define Y_OFFSET (20)

using namespace cocos2d;

DropItem::DropItem()
{
    m_nItemID = 0;
    m_nCount = 0;
    m_bIsThief = false;
    m_fInitSpeedY = 10;
}
DropItem::~DropItem()
{
    
}

DropItem* DropItem::create(int nItemID, int nCount, bool bIsThief)
{
    DropItem* pRet = new DropItem();
    if (pRet && pRet->initWithConfig(nItemID, nCount, bIsThief))
    {
        pRet->autorelease();
        return pRet;
    }
    
    CC_SAFE_DELETE(pRet);
    return NULL;
}
bool DropItem::initWithConfig(int nItemID, int nCount, bool bIsThief)
{
    char str[128] = {0};
    sprintf(str, "item_%d.png", nItemID);
    
    CCLOG("apple.....DropItem... item_id = %d", nItemID);
    if (!CCSprite::initWithSpriteFrameName(str))
    {
        return false;
    }
    /*不用数量显示
    if (nCount != 1)
    {
        sprintf(str, "%d", nCount);
        CCLabelTTF* pLabel = CCLabelTTF::create(str, "Arial", 20);
        this->addChild(pLabel);
        CCSize size = this->getContentSize();
        pLabel->setPosition(ccp(size.width / 2, size.height + 5));
    }
     */
    
    m_nItemID = nItemID;
    m_nCount = nCount;
    m_bIsThief = bIsThief;
    
    const float rate = 1;
    this->setScale(rate);
    CCSize size = this->getContentSize();
    size.width *= rate;
    size.height *= rate;
    this->setContentSize(size);
    
    CCRepeatForever* pRepeat = CCRepeatForever::create(CCRotateBy::create(0.35, 360));
    pRepeat->setTag(ANIMATE_TAG);
    this->runAction(pRepeat);
    
    return true;
}

CCRect DropItem::getUnitRect()
{
    CCPoint pos = this->getPosition();
    CCSize size = this->getContentSize();
    return CCRectMake(pos.x - 10, pos.y - size.height / 2 +Y_OFFSET, 5, size.height - Y_OFFSET);
}

kObstacleDirection DropItem::getObstacleDirection(float moveX, float moveY, float* pCorrectX, float* pCorrectY)
{
    BattleMapUnits* pMapUnitLayer = (BattleMapUnits*)GameManager::sharedInstance()->getBattleMapLayer();
    return pMapUnitLayer->checkDirection(getUnitRect(), moveX, moveY, pCorrectX, pCorrectY);
}

void DropItem::updatePos()
{
    //掉落速度：下面的调掉落物的下落速度（重力加速度的倍数）
    m_fInitSpeedY += JUMP_ACCELERATED_SPEED_Y;
    
    BattleMapUnits* pMapUnitLayer = (BattleMapUnits*)GameManager::sharedInstance()->getBattleMapLayer();
    if (pMapUnitLayer)
    {
        float correctX = 0;
        float correctY = 0;
        kObstacleDirection curObstacleDirection = this->getObstacleDirection(0, m_fInitSpeedY, &correctX, &correctY);
        correctY += this->getContentSize().height / 2 - Y_OFFSET;
        
        switch (curObstacleDirection)
        {
            case kObstacleNone:
                this->setPosition(ccp(getPosition().x, getPosition().y + m_fInitSpeedY));
                break;
            case kObstacleUp:
            case kObstacleDown:
                this->stopAnimation();
                m_fInitSpeedY = 0;
                this->setPosition(ccp(getPosition().x, correctY));
                break;
            default:
                this->stopAnimation();
                m_fInitSpeedY = 0;
                this->setPosition(ccp(correctX, correctY));
                break;
        }
    }

    
}

void DropItem::stopAnimation()
{
    CCAction* pAction = this->getActionByTag(ANIMATE_TAG);
    if (pAction != NULL && !pAction->isDone())
    {
        this->stopAction(pAction);
        this->runAction(CCRotateTo::create(0, 0));
    }
    
}

