//
//  ComboControl.cpp
//  DragonBattle
//
//  Created by 永翔 何 on 13-5-23.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "ComboControl.h"

using namespace cocos2d;

ComboControl::ComboControl()
{
    m_pCombo = NULL;
    m_pSpriteFrame = NULL;
}

ComboControl::~ComboControl()
{
}

ComboControl* ComboControl::create()
{
    ComboControl *pRet = new ComboControl();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool ComboControl::init()
{
    if (!CCLayer::init())
        return false;
    
    m_nCombo = 0;
    
    m_pSpriteFrame = CCSprite::create("BattleUI/combo.png");
    this->addChild(m_pSpriteFrame);
    m_pSpriteFrame->setAnchorPoint(CCPointMake(0.5, 0.5));
    m_pSpriteFrame->setPosition(CCPointMake(60, 0));
    
    m_pCombo = CCLabelAtlas::create("0", "BattleUI/attack_count_num.png", 49, 84, '0');
    this->addChild(m_pCombo);
    m_pCombo->setAnchorPoint(CCPointMake(0.5, 0.5));
    m_pCombo->setPosition(CCPointMake(20, 20));
    
    this->setVisible(false);
    
    return true;
}

void ComboControl::setCombo(int nCombo)
{
    if (m_nCombo != nCombo && nCombo != 1)
    {
        if (this->isVisible())
        {
            this->unschedule(schedule_selector(ComboControl::hide));
        }
        else 
        {
            this->setVisible(true);
        }
        this->schedule(schedule_selector(ComboControl::hide), 2.0f); 

        char a[8] = {0};
        sprintf(a, "%d", nCombo);
        
        m_pCombo->setString(a);
        m_pCombo->setScale(2);
        if (nCombo > 500)
        {
            //m_pCombo->setColor(ccc3(255, 0, 255));
        }
        else if (nCombo > 100)
        {
            //m_pCombo->setColor(ccRED);
        }
        else if (nCombo > 20)
        {
            //m_pCombo->setColor(ccYELLOW);
        }
        else 
        {
            //m_pCombo->setColor(ccBLUE);
        }
        m_pCombo->runAction(CCScaleTo::create(0.5, 1));
    }
    m_nCombo = nCombo;
}

void ComboControl::hide(float dt)
{
    this->setVisible(false);
}


