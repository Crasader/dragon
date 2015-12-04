//
//  ShowMemoryLayer.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-5-3.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "ShowMemoryLayer.h"
#include "GlobalDefine.h"
#include "tools.h"

USING_NS_CC;

bool ShowMemoryLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    m_pMemoryUsed = NULL;
    this->scheduleUpdate();
    return true;
}

void ShowMemoryLayer::update(float dt)
{
    this->showUserdMemory();
}

void ShowMemoryLayer::showUserdMemory()
{
    char buff[128] = {0};
    sprintf(buff, "used memory: %fM / %fM", usedMemory(), availableMemory());
    
    if (!m_pMemoryUsed)
    {
        m_pMemoryUsed = CCLabelTTF::create(buff, "Arial", 32);
        this->addChild(m_pMemoryUsed);
        m_pMemoryUsed->setAnchorPoint(ccp(0, 0));
        m_pMemoryUsed->setPosition(ccp(20, /*SCREEN_HEIGTH - */20));
        m_pMemoryUsed->setColor(ccRED);
    }
    m_pMemoryUsed->setString(buff);
}