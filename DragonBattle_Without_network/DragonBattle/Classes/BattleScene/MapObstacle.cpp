//
//  Obstacle.cpp
//  DragonBattle
//
//  Created by 永翔 何 on 13-5-19.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "MapObstacle.h"
#include "Resources.h"

using namespace cocos2d;

MapObstacle::MapObstacle()
{
    m_pSelfSprite = NULL;
    m_pRectColor = NULL;
    m_pConf = NULL;
    
    m_nObstacleDirection = kObstacleNone;
}

MapObstacle::~MapObstacle()
{
    m_pSelfSprite = NULL;
    m_pRectColor = NULL;
    m_pConf = NULL;    
}

MapObstacle*  MapObstacle::create()
{
    MapObstacle* pRet = new MapObstacle();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    
    CC_SAFE_DELETE(pRet);
    return NULL;
}

MapObstacle* MapObstacle::create(const Obstacle_t* pConf)
{
    MapObstacle* pRet = new MapObstacle();
    if (pRet && pRet->init(pConf))
    {
        pRet->autorelease();
        return pRet;
    }
    
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool MapObstacle::init(const Obstacle_t* pConf)
{
    m_pConf = pConf;
    
    if (pConf != NULL && pConf->nResID != 0)
    {
        //UIResourceInfo_t* pRes = ResourcesManager::sharedInstance()->getUIResourceWithID(pConf->nResID);
        //m_pSelfSprite = CCSprite::create(pRes->strName.c_str());
        m_pSelfSprite = CREATE_UI_SPRITE(pConf->nResID);
        this->addChild(m_pSelfSprite);
        m_pSelfSprite->setAnchorPoint(ccp(0, 0));
        //m_pSelfSprite->setPosition(ccp(0, 0));
    }

    m_pRectColor = CCLayerColor::create(ccc4(255, 0, 0, 0), 0, 0);
    this->addChild(m_pRectColor);
    m_pRectColor->setAnchorPoint(ccp(0, 0));
    m_pRectColor->setPosition(ccp(0, 0));
    
    if (pConf != NULL)
    {
        this->setCollideRect(CCRectMake(pConf->ptPosition.x, pConf->ptPosition.y, pConf->fWidth, pConf->fHeight));
    }

    return true;
}

void MapObstacle::setCollideRect(cocos2d::CCRect rc)
{
    m_rcCollide = rc;
    this->setAnchorPoint(ccp(0, 0));
    this->setPosition(rc.origin);
    this->setContentSize(rc.size);
    
    if (m_pSelfSprite)
    {
        //m_pSelfSprite->setScaleX(rc.size.width / m_pSelfSprite->getContentSize().width);
        //m_pSelfSprite->setScaleY(rc.size.height / m_pSelfSprite->getContentSize().height);
    }
    
    m_pRectColor->setContentSize(rc.size);
    //m_pRectColor->setPosition(ccp(rc.origin.x, rc.origin.y)); 
}

// 得到碰撞区域
CCRect MapObstacle::getCollideRect()
{
    return m_rcCollide;
}
    