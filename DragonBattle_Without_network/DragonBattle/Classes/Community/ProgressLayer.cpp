//
//  ProgressLayer.cpp
//  DragonBattle
//
//  Created by 何 永翔 on 13-9-25.
//
//

#include "ProgressLayer.h"

#define PROGRESS_TAG (1000)

ProgressLayer::ProgressLayer()
{
    m_pSelector = NULL;
    
    m_fileVec.clear();
    
    m_pProgressSprite = NULL;
    
    m_nIndex = 0;
    m_nCount = 0;
}
ProgressLayer::~ProgressLayer()
{
    m_fileVec.clear();
}

ProgressLayer* ProgressLayer::create(const char* pBg, const std::vector<std::string>* pFileVec, bool bHasTitle, SEL_CallFuncN selector, CCObject* pTarget)
{
    ProgressLayer* pRet = new ProgressLayer();
    
    if (pRet && pRet->initWithConf(pBg, pFileVec, bHasTitle, selector, pTarget))
    {
        pRet->autorelease();
        return pRet;
    }
    
    CC_SAFE_DELETE(pRet);
    return NULL;
}
bool ProgressLayer::initWithConf(const char* pBg, const std::vector<std::string>* pFileVec, bool bHasTitle, SEL_CallFuncN selector, CCObject* pTarget)
{
    this->setTouchPriority(-129);
    
    m_pSelector = selector;
    m_pTarget = pTarget;
    
    for (std::vector<std::string>::const_iterator it = pFileVec->begin(); it != pFileVec->end(); it++)
    {
        m_fileVec.push_back(*it);
    }
    m_nCount = m_fileVec.size();
    
    CCSprite* pBack = CCSprite::create(pBg);
    this->addChild(pBack);
    CCSize size = pBack->getContentSize();
    float scale = SCREEN_WIDTH / size.width > SCREEN_HEIGTH / size.height ? (SCREEN_WIDTH / size.width) : (SCREEN_HEIGTH / size.height);
    pBack->setScaleX(scale);
    pBack->setScaleY(scale);
    
    if (bHasTitle)
    {
        CCSprite* pTitle = CCSprite::create("CCBLoginResource/game_icon.png");
        this->addChild(pTitle);
        pTitle->setPosition(ccp(SCREEN_WIDTH / 2 - 200, SCREEN_HEIGTH / 2 - 100));
    }
    
    CCSprite* pSlot = CCSprite::create("CCBCommunityResource/progress_slot.png");
    this->addChild(pSlot);
    pSlot->setPosition(ccp(0, - SCREEN_HEIGTH / 2 + 100));
    
    m_pProgressSprite = CCProgressTimer::create(CCSprite::create("CCBCommunityResource/progress_bar.png"));
    this->addChild(m_pProgressSprite);
    m_pProgressSprite->setTag(PROGRESS_TAG);
    m_pProgressSprite->setPosition(ccp(0, - SCREEN_HEIGTH / 2 + 100));
    m_pProgressSprite->setType(kCCProgressTimerTypeBar);
    m_pProgressSprite->setMidpoint(ccp(0,0));
    m_pProgressSprite->setBarChangeRate(ccp(1, 0));
    
    CCLabelTTF* pLoadingWord = CCLabelTTF::create(RAND_STR_LOADING_WORDS, "Arial-BoldMT", 22);
    pLoadingWord->setColor(ccWHITE);
    pLoadingWord->setPosition(ccp(0, - SCREEN_HEIGTH / 2 + 50));
    this->addChild(pLoadingWord);
    
    this->scheduleOnce(schedule_selector(ProgressLayer::load), 0);
    
    return true;
}

void ProgressLayer::load(float dt)
{
    this->unschedule(schedule_selector(ProgressLayer::load));
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(m_fileVec.at(m_nIndex++).c_str());
    
    const float totalTime = 0.1;
    CCProgressTo* to = CCProgressTo::create(totalTime / m_nCount, m_nIndex * 100.0 / m_nCount);
    m_pProgressSprite->runAction(CCSequence::create(to, CCCallFunc::create(this, callfunc_selector(ProgressLayer::runProgressOver)), NULL));
}

void ProgressLayer::destroySelf()
{
    if (m_pSelector != NULL && m_pTarget != NULL)
    {
        (m_pTarget->*m_pSelector)(this);
    }
    if (getParent())
    {
        this->removeFromParent();
    }
}

void ProgressLayer::runProgressOver()
{
    if (m_nIndex != m_nCount)
    {
        this->scheduleOnce(schedule_selector(ProgressLayer::load), 0);
    }
    else
    {
        this->runAction(CCSequence::create(CCDelayTime::create(1), CCCallFunc::create(this, callfunc_selector(ProgressLayer::destroySelf)), NULL));
    }
}
