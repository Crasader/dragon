//
//  BattleSubBGNode.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-17.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "BattleSubBGNode.h"
USING_NS_CC;


BattleSubBGNode::BattleSubBGNode()
{
    m_fWidth = 0;
}
BattleSubBGNode::~BattleSubBGNode()
{
    
}

// create近景层，根据地图长度来随机计算各个图片的坐标
BattleSubBGNode* BattleSubBGNode::createNearScene(const MissionBGVec* pBgConf, float fMapLength)
{
    BattleSubBGNode* pRet = new BattleSubBGNode();
    if (pRet && pRet->initNearScene(pBgConf, fMapLength))
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

bool BattleSubBGNode::initNearScene(const MissionBGVec *pBgConf, float fMapLenght)
{
    if (pBgConf->size() == 0)
    {
        return false;
    }
    
    m_fWidth = fMapLenght;
    for (int i = 0; i < pBgConf->size(); i++)
    {
        CCSprite* pBg = NULL;
        pBg = CCSprite::createWithSpriteFrameName((*pBgConf)[i]->strBgName.data());
        pBg->setPosition(ccp((*pBgConf)[i]->fBgPosX, (*pBgConf)[i]->fBgPosY));
        pBg->setAnchorPoint(ccp(0,0));
        pBg->setFlipX((*pBgConf)[i]->isFlipX);
        this->addChild(pBg);
    }
    
    this->setAnchorPoint(ccp(0, 0));
    return true;
}