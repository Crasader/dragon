//
//  FlashButton.cpp
//  DragonBattle
//
//  Created by 何 永翔 on 13-7-15.
//
//

#include "FlashButton.h"

FlashButton::FlashButton()
{
}

FlashButton* FlashButton::create(const char* pBg, const char* pFlash1, const char* pFlash2, float nFlashSpeed, float nFlashInterval, int nPriority)
{
    FlashButton *pBtn = new FlashButton();
	if (pBtn && pBtn->initWithFile(pBg, pFlash1, pFlash2, nFlashSpeed, nFlashInterval, nPriority))
    {
	    pBtn->autorelease();
        return pBtn;
    }
    CC_SAFE_DELETE(pBtn);
	return NULL;
}

bool FlashButton::initWithFile(const char* pBg, const char* pFlash1, const char* pFlash2, float nFlashSpeed, float nFlashInterval, int nPriority)
{
    if (!XYBaseTouchSprite::initWithFile(pBg, nPriority))
    {
        return false;
    }
    
    CCSprite* pBgSprite1 = CCSprite::create(pFlash1);
    this->addChild(pBgSprite1);
    pBgSprite1->setOpacity(0);
    
    
    CCSprite* pBgSprite2 = CCSprite::create(pFlash2);
    this->addChild(pBgSprite2);
    pBgSprite2->setOpacity(0);
    
    CCSequence* pSeq1 = CCSequence::create(CCFadeIn::create(nFlashSpeed / 2), CCFadeOut::create(nFlashSpeed / 2), CCDelayTime::create(nFlashInterval), NULL);
    CCRepeatForever* pForever = CCRepeatForever::create(pSeq1);
    pBgSprite1->runAction(pForever);
    pBgSprite2->runAction(CCSequence::create(CCDelayTime::create((nFlashSpeed - nFlashInterval) / 2 + nFlashInterval), pForever->copy()->autorelease(), NULL));
    
    return true;
}
