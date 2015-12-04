//
//  ScheduleAction.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-8-31.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "ScheduleAction.h"
#include "ViewDefine.h"
USING_NS_CC;

ScheduleBase::ScheduleBase()
{
    m_nCurIndex = 0;
    m_nRunTimes = 0;
}

ScheduleBase::~ScheduleBase()
{
    
}

void ScheduleBase::startWork(float delayPerUnit)
{
    this->scheduleUpdate(float(0));
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(ScheduleBase::scheduleUpdate), this, delayPerUnit, false);
}

bool ScheduleBase::scheduleUpdate(float dt)
{
    if (m_nCurIndex >= m_nRunTimes)
    {
        m_nCurIndex = 0;
        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(ScheduleBase::scheduleUpdate), this);
        return false;
    }
    
    m_nCurIndex++;
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
TipsEffect::TipsEffect()
{
    m_pArray = CCArray::create();
    m_pArray->retain();
}

TipsEffect::~TipsEffect()
{
    m_pArray->release();
}

TipsEffect* TipsEffect::create()
{
    TipsEffect* pRet = new TipsEffect();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
    }
    return pRet;
}

bool TipsEffect::init()
{
    if (!CCNode::init())
    {
        return false;
    }
    
    return true;
}

void TipsEffect::addTips(const char* msg, const ccColor3B& clr)
{
    CCString* pString = CCString::create(msg);
    m_pArray->addObject(pString);
    m_nRunTimes++;
}

void TipsEffect::startWork(float delayPerUnit)
{
    //加到了top layer,也可以加到其他层
    TOPADDCHILD(this);
    
    ScheduleBase::startWork(delayPerUnit);
}

bool TipsEffect::scheduleUpdate(float dt)
{
    if (ScheduleBase::scheduleUpdate(dt))
    {
        if (m_pArray->count() == 0)
        {
            return false;
        }
        
        //取第一个
        CCString* pString = dynamic_cast<CCString*>(m_pArray->objectAtIndex(0));
        if (!pString)
        {
            return false;
        }
        
        XYTopLayer::getInstance()->ShowTip(pString->getCString());
        m_pArray->removeObjectAtIndex(0);
        return true;
    }
    else
    {
        this->removeAllChildren();
        this->removeFromParent();
        return false;
    }
}
