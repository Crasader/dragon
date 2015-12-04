//
//  ProgressLayerNew.cpp
//  DragonBattle
//
//  Created by 何 永翔 on 14-2-24.
//
//

#include "ProgressLayerNew.h"
#include "GameData.h"
#include "UnitResource.h"
#include "UserData.h"

#define PROGRESS_TAG (1000)

using namespace cocos2d;

ProgressLayerNew::ProgressLayerNew()
{
    m_pSelector = NULL;
    
    m_fileVec.clear();
    
    m_pProgressSprite = NULL;
    
    m_nIndex = 0;
    m_nCount = 0;
}
ProgressLayerNew::~ProgressLayerNew()
{
    m_fileVec.clear();
}

ProgressLayerNew* ProgressLayerNew::create(const std::vector<std::string>* pFileVec, bool bHasTitle, SEL_CallFuncN selector, CCObject* pTarget)
{
    ProgressLayerNew* pRet = new ProgressLayerNew();
    
    if (pRet && pRet->initWithConf(pFileVec, bHasTitle, selector, pTarget))
    {
        pRet->autorelease();
        return pRet;
    }
    
    CC_SAFE_DELETE(pRet);
    return NULL;
}
bool ProgressLayerNew::initWithConf(const std::vector<std::string>* pFileVec, bool bHasTitle, SEL_CallFuncN selector, CCObject* pTarget)
{
    this->setTouchPriority(-129);
    
    m_pSelector = selector;
    m_pTarget = pTarget;
    
    for (std::vector<std::string>::const_iterator it = pFileVec->begin(); it != pFileVec->end(); it++)
    {
        m_fileVec.push_back(*it);
    }

    m_fileVec.push_back("item/items.plist");
    m_nCount = m_fileVec.size();
    
    char str[128] = {0};
    sprintf(str, "CCBCommunityResource/progress_bg_%d.jpg", rand() % 3);
    CCSprite* pBack = CCSprite::create(str);
    this->addChild(pBack);
    pBack->setPosition(ccp(0, - SCREEN_HEIGTH / 2 + SCREEN_HEIGTH * 0.443));
    
    if (bHasTitle)
    {
        CCSprite* pTitle = CCSprite::create("CCBLoginResource/game_icon.png");
        this->addChild(pTitle);
        pTitle->setPosition(ccp(0, - SCREEN_HEIGTH / 2 + SCREEN_HEIGTH * 0.703));
    }
    
    CCSprite* pSlot = CCSprite::create("CCBCommunityResource/progress_slot.png");
    this->addChild(pSlot);
    pSlot->setPosition(ccp(0, - SCREEN_HEIGTH / 2 + SCREEN_HEIGTH * 0.26));
    
    m_pProgressSprite = CCProgressTimer::create(CCSprite::create("CCBCommunityResource/progress_bar.png"));
    this->addChild(m_pProgressSprite);
    m_pProgressSprite->setTag(PROGRESS_TAG);
    m_pProgressSprite->setPosition(ccp(0, - SCREEN_HEIGTH / 2 + SCREEN_HEIGTH * 0.26));
    m_pProgressSprite->setType(kCCProgressTimerTypeBar);
    m_pProgressSprite->setMidpoint(ccp(0,0));
    m_pProgressSprite->setBarChangeRate(ccp(1, 0));
    
    /*
    CCParticleSystemQuad* particle = CCParticleSystemQuad::create("effect/loading1.plist");
    this->addChild(particle);
    particle->setPositionType(kCCPositionTypeFree);
    particle->setPosition(ccp(-m_pProgressSprite->getContentSize().width / 2.0f, - SCREEN_HEIGTH / 2 + 200));
    m_pProgressSprite->setUserObject(particle);
     */
    
    switch (gUserData->m_heroData->nUnitID)
    {
        case 1:
            this->setSprite(ccp(-135, - SCREEN_HEIGTH / 2 + SCREEN_HEIGTH * 0.299), 0.75f, ccp(-135, - SCREEN_HEIGTH / 2 + SCREEN_HEIGTH * 0.423), 0.8f, "hero0101.png", "action/player_1/Role_stay_1.plist", 1);
            break;
        case 2:
            this->setSprite(ccp(-135, - SCREEN_HEIGTH / 2 + SCREEN_HEIGTH * 0.299), 0.75f, ccp(-135, - SCREEN_HEIGTH / 2 + SCREEN_HEIGTH * 0.423), 0.8f, "hero2101.png", "action/player_1/Role_stay_2.plist", 2);
            break;
        case 3:
            this->setSprite(ccp(-135, - SCREEN_HEIGTH / 2 + SCREEN_HEIGTH * 0.299), 0.75f, ccp(-135, - SCREEN_HEIGTH / 2 + SCREEN_HEIGTH * 0.417), 0.8f, "hero4101.png", "action/player_1/Role_stay_3.plist", 3);
            break;
        default:
            break;
    }
    this->setSprite(ccp(135, - SCREEN_HEIGTH / 2 + SCREEN_HEIGTH * 0.299), 1, ccp(135, - SCREEN_HEIGTH / 2 + SCREEN_HEIGTH * 0.38), 0.75f, "monster_yz0701.png", "action/monster_1/Role_stay_44.plist", 44);
    
    CCLabelTTF* pLoadingWord = CCLabelTTF::create(RAND_STR_LOADING_WORDS, "Arial-BoldMT", 22);
    pLoadingWord->setColor(ccWHITE);
    pLoadingWord->setPosition(ccp(0, - SCREEN_HEIGTH / 2 + SCREEN_HEIGTH * 0.208));
    this->addChild(pLoadingWord);
    
    this->scheduleOnce(schedule_selector(ProgressLayerNew::load), 0);
    
    return true;
}

void ProgressLayerNew::setSprite(CCPoint shadowPosition, float fShadowScale, CCPoint rolePosition, float fRoleScale, const char* pRoleName, const char* pListName, int nRoleId)
{
    CCSprite* pShadow = CCSprite::create("BattleUI/shadow.png");
    this->addChild(pShadow);
    pShadow->setScale(fShadowScale);
    pShadow->setPosition(shadowPosition);
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(pListName);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(pListName);
    CCSprite* role = CCSprite::createWithSpriteFrameName(pRoleName);
    this->addChild(role);
    role->setScale(fRoleScale);
    role->setPosition(rolePosition);
    
    CCRepeatForever* pGoForward = CCRepeatForever::create(UnitResource::sharedInstance()->createAnimate(nRoleId, kAnimRun));
	role->runAction(pGoForward);
}

void ProgressLayerNew::load(float dt)
{
    this->unschedule(schedule_selector(ProgressLayerNew::load));
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(m_fileVec.at(m_nIndex++).c_str());
    
    const float totalTime = 0.1f;
    CCProgressTo* to = CCProgressTo::create(totalTime / m_nCount, m_nIndex * 100.0 / m_nCount);
    m_pProgressSprite->runAction(CCSequence::create(to, CCCallFunc::create(this, callfunc_selector(ProgressLayerNew::runProgressOver)), NULL));
    //dynamic_cast<CCNode*>(m_pProgressSprite->getUserObject())->runAction(CCMoveBy::create(totalTime / m_nCount, ccp(m_pProgressSprite->getContentSize().width / m_nCount, 0)));
}

void ProgressLayerNew::destroySelf()
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

void ProgressLayerNew::runProgressOver()
{
    if (m_nIndex != m_nCount)
    {
        this->scheduleOnce(schedule_selector(ProgressLayerNew::load), 0);
    }
    else
    {
        this->runAction(CCSequence::create(CCDelayTime::create(1), CCCallFunc::create(this, callfunc_selector(ProgressLayerNew::destroySelf)), NULL));
    }
}
