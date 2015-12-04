//
//  FlashSprite.cpp
//  DragonBattle
//
//  Created by 何 永翔 on 13-7-15.
//
//

#include "FlashSprite.h"

using namespace cocos2d;

FlashSprite::FlashSprite()
{
    m_pBgSprite2 = NULL;
    m_fFlashSpeed = 0;
    m_fFlashInterval = 0;
}
FlashSprite::~FlashSprite()
{
    this->unscheduleAllSelectors();
}

FlashSprite* FlashSprite::create(const char* pFlash1, const char* pFlash2, float fFlashSpeed, float fFlashInterval)
{
    FlashSprite *pBtn = new FlashSprite();
	if (pBtn && pBtn->initWithFile(pFlash1, pFlash2, fFlashSpeed, fFlashInterval))
    {
	    pBtn->autorelease();
        return pBtn;
    }
    CC_SAFE_DELETE(pBtn);
	return NULL;
}

CCRepeatForever* FlashSprite::createRepeatAction()
{
    CCSequence* pSeq1 = CCSequence::create(CCFadeIn::create(m_fFlashSpeed / 2), CCFadeOut::create(m_fFlashSpeed / 2), CCDelayTime::create(m_fFlashInterval), NULL);
    return CCRepeatForever::create(pSeq1);
}

void FlashSprite::runSprite2Action(float dt)
{
    if (m_pBgSprite2 != NULL)
    {
        m_pBgSprite2->runAction(this->createRepeatAction());
    }
}

bool FlashSprite::initWithFile(const char* pFlash1, const char* pFlash2, float fFlashSpeed, float fFlashInterval)
{
    if (!CCSprite::init())
    {
        return false;
    }
    
    m_fFlashSpeed = fFlashSpeed;
    m_fFlashInterval = fFlashInterval;
    
    CCSprite* pBgSprite1 = NULL;
    if (pFlash1 != NULL)
    {
        pBgSprite1 = CCSprite::create(pFlash1);
        this->addChild(pBgSprite1);
        pBgSprite1->setOpacity(0);
    }
    
    if (pFlash2 != NULL)
    {
        m_pBgSprite2 = CCSprite::create(pFlash2);
        this->addChild(m_pBgSprite2);
        m_pBgSprite2->setOpacity(0);
    }
    
    if (pBgSprite1 != NULL)
    {
        pBgSprite1->runAction(this->createRepeatAction());
    }
    this->scheduleOnce(schedule_selector(FlashSprite::runSprite2Action), (m_fFlashSpeed - m_fFlashInterval) / 2 + m_fFlashInterval);
    
    return true;
}
