//
//  EffectResource.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-9-3.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "EffectResource.h"
#include "GlobalDefine.h"
#include "GameManager.h"
#include "CRUtils.h"
USING_NS_CC;

#define EFFECT_FILE_STYLE   (const char*)"effect/%d/effect_%d_%02d.png"

#define EFFECT_KEY_FORMATE   (const char*)"effect_%d_%02d.png"
#define EFFECT_PLIST_FILE_FORMATE   (const char*)"effect/effect_%d.plist"
#define EFFECT_PNG_FILE_FORMATE   (const char*)"effect/effect_%d.png"
#define EFFECT_LIGHT_PNG_FILE_FORMATE   (const char*)"effect/effect_%d_0.png"

EffectResource* EffectResource::pSharedInstance = NULL;

EffectResource::EffectResource()
{
    m_pEffectResMap.clear();
    m_pEffectQueue = new CCArray;
}

EffectResource::~EffectResource()
{
    m_pEffectResMap.clear();
    if (m_pEffectQueue->count() > 0) m_pEffectQueue->removeAllObjects();
    CC_SAFE_RELEASE_NULL(m_pEffectQueue);
}

EffectResource* EffectResource::sharedInstance()
{
    if (!pSharedInstance)
    {
        pSharedInstance = new EffectResource();
        pSharedInstance->loadFromFile();        
    }
    return pSharedInstance;
}

bool EffectResource::loadFromFile()
{
    m_pEffectResMap[1] = 33;        //主城中点击地面效果
    m_pEffectResMap[2] = 20;        //关卡跳转点
    m_pEffectResMap[3] = 30;        //角色界面人物背后的闪光
    m_pEffectResMap[4] = 40;        //主城中升级
    m_pEffectResMap[5] = 24;        //烟花
    m_pEffectResMap[6] = 30;        //按钮旋转效果
    m_pEffectResMap[7] = 25;        //图谱特效
    m_pEffectResMap[8] = 20;        //弟子技能升级1
    m_pEffectResMap[9] = 16;        //弟子技能升级2
    m_pEffectResMap[10] = 16;       //资质变化火焰
    m_pEffectResMap[11] = 32;       //强化成功
    m_pEffectResMap[12] = 24;       //强化、进阶、洗练
    m_pEffectResMap[13] = 32;       //进阶成功
    m_pEffectResMap[14] = 15;       //关卡选中
    m_pEffectResMap[15] = 8;       //boss关卡选中
    m_pEffectResMap[16] = 25;       //进阶成功
    m_pEffectResMap[17] = 16;       //资质提升
    m_pEffectResMap[18] = 20;       //箭头动画
    m_pEffectResMap[19] = 20;       //在线领奖动画
    m_pEffectResMap[20] = 40;       //接受任务
    m_pEffectResMap[21] = 40;       //完成任务
    m_pEffectResMap[22] = 20;       //主城跳转点
    m_pEffectResMap[23] = 11;       //新功能开启
    m_pEffectResMap[24] = 16;       //任务提醒
    m_pEffectResMap[25] = 30;       //奖励平台小按钮特效
    m_pEffectResMap[26] = 30;       //奖励平台大按钮特效
    m_pEffectResMap[27] = 35;       //角色升级 身上
    m_pEffectResMap[28] = 24;       //绿色礼花
    m_pEffectResMap[29] = 30;       //封测扫光
    m_pEffectResMap[30] = 28;       //捡物品
    m_pEffectResMap[31] = 10;       //气脉
    m_pEffectResMap[32] = 22;       //复仇
    m_pEffectResMap[33] = 10;       //镶嵌
    m_pEffectResMap[34] = 14;       //充值扫光（社区）
    m_pEffectResMap[35] = 20;       //返回社区
    m_pEffectResMap[36] = 20;       //充值扫光2
    m_pEffectResMap[37] = 14;       //奖励Item扫光效果
    m_pEffectResMap[38] = 28;       //vip扫光效果
    m_pEffectResMap[39] = 14;       //引导箭头
    m_pEffectResMap[40] = 14;       //气脉闪电链
    m_pEffectResMap[41] = 22;       //获得物品特效
    return true;
}

CCAnimate* EffectResource::createEffectAnimate(uint32_t effectID)
{
    std::map<uint32_t, uint32_t>::iterator it = m_pEffectResMap.find(effectID);
    if (it == m_pEffectResMap.end())
    {
        return NULL;
    }
    
    char file[64] = {0};
    CCArray* pFrameArray = CCArray::create();
    for (int i = 1; i <= m_pEffectResMap[effectID]; i++)
    {
        sprintf(file, EFFECT_KEY_FORMATE, effectID, i);
        CCSpriteFrame* pSpriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(file);
        
        if (pSpriteFrame == NULL) {
            // try again.
            CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CCString::createWithFormat(EFFECT_PLIST_FILE_FORMATE, effectID)->getCString());
            
            pSpriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(file);
        }
        
        CCAssert(pSpriteFrame != NULL, "plz check to add plist file for spriteframe");
        
        pFrameArray->addObject(pSpriteFrame);
        
#if 0
        sprintf(file, EFFECT_FILE_STYLE, effectID, effectID, i);
        pFrameArray->addObject(CRUtils::initSpriteFrameWithName(file));
#endif
    }
    
    CCAnimation* pAnimation = CCAnimation::createWithSpriteFrames(pFrameArray, ANIMAT_DELAY_PER_UNIT);
    CCAnimate* pAnimate = CCAnimate::create(pAnimation);
    return pAnimate;
}

//序列动画，动画完后清除自己
CCSprite* EffectResource::spriteWithEffect(uint32_t effectID, bool isForever, bool cleanSelf, float delayPerUnit/* = 0.f*/)
{
    std::map<uint32_t, uint32_t>::iterator it = m_pEffectResMap.find(effectID);
    if (it == m_pEffectResMap.end())
    {
        return NULL;
    }
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CCString::createWithFormat(EFFECT_PLIST_FILE_FORMATE, effectID)->getCString());
    
    char file[64] = {0};
    sprintf(file, EFFECT_KEY_FORMATE, effectID, 1);
    CCSprite* pSprite = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(file));
    if (!pSprite)
    {
        return NULL;
    }
    
    CCArray* pFrameArray = CCArray::create();
    for (int i = 1; i <= m_pEffectResMap[effectID]; i++)
    {
        sprintf(file, EFFECT_KEY_FORMATE, effectID, i);
        CCSpriteFrame* pSpriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(file);
        CCAssert(pSpriteFrame != NULL, "plz check to add plist file for spriteframe");
        pFrameArray->addObject(pSpriteFrame);
        
#if 0
        sprintf(file, EFFECT_FILE_STYLE, effectID, effectID, i);
        pFrameArray->addObject(CRUtils::initSpriteFrameWithName(file));
#endif
        
    }
    
    CCAnimation* pAnimation = CCAnimation::createWithSpriteFrames(pFrameArray, ANIMAT_DELAY_PER_UNIT);
    if (effectID == 6 || effectID == 15)
    {
        pAnimation->setDelayPerUnit(ANIMAT_DELAY_PER_UNIT * 2);
    }
    else
    {
        pAnimation->setDelayPerUnit(ANIMAT_DELAY_PER_UNIT * delayPerUnit);
    }
    
    CCAnimate* pAnimate = CCAnimate::create(pAnimation);

    CCAction* pAction = NULL;
    if (isForever)
    {
        pAction = CCRepeatForever::create(pAnimate);
    }
    else
    {
        if (cleanSelf)
        {
            pAction = CCSequence::create(pAnimate, CCRemoveSelf::create(), CCCallFuncO::create(this, callfuncO_selector(EffectResource::removeSpriteFrame), CCString::createWithFormat(EFFECT_PLIST_FILE_FORMATE, effectID)), NULL);
        }
        else
        {
            pAction = pAnimate;
        }
    }
    pAction->setTag(kDefaultSpriteEffectTag);
    pSprite->runAction(pAction);
    return pSprite;
}

void EffectResource::removeSpriteFrame(CCString* effect)
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(effect->getCString());
}

cocos2d::CCSprite* EffectResource::spriteWithEffect(uint32_t effectID, float time, bool cleanSelf)
{
    std::map<uint32_t, uint32_t>::iterator it = m_pEffectResMap.find(effectID);
    if (it == m_pEffectResMap.end())
    {
        return NULL;
    }
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CCString::createWithFormat(EFFECT_PLIST_FILE_FORMATE, effectID)->getCString());
    
    char file[64] = {0};
    sprintf(file, EFFECT_FILE_STYLE, effectID, effectID, 1);
    sprintf(file, EFFECT_KEY_FORMATE, effectID, 1);
    CCSprite* pSprite = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(file));

    if (!pSprite)
    {
        return NULL;
    }
    
    CCArray* pFrameArray = CCArray::create();
    for (int i = 1; i <= m_pEffectResMap[effectID]; i++)
    {
        sprintf(file, EFFECT_KEY_FORMATE, effectID, i);
        CCSpriteFrame* pSpriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(file);
        CCAssert(pSpriteFrame != NULL, "plz check to add plist file for spriteframe");
        pFrameArray->addObject(pSpriteFrame);
#if 0
        sprintf(file, EFFECT_FILE_STYLE, effectID, effectID, i);
        
        //CCSpriteFrame* pSpriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(file);
        //pFrameArray->addObject(pSpriteFrame);
        pFrameArray->addObject(CRUtils::initSpriteFrameWithName(file));
#endif
    }
    
    CCAnimation* pAnimation = CCAnimation::createWithSpriteFrames(pFrameArray, ANIMAT_DELAY_PER_UNIT);
    if (effectID == 6 || effectID == 15)
    {
        pAnimation->setDelayPerUnit(ANIMAT_DELAY_PER_UNIT * 2);
    }
    CCAnimate* pAnimate = CCAnimate::create(pAnimation);
    pAnimation->setDelayPerUnit(3.f);
    
    CCAction* pAction = NULL;
    
    pAction = CCRepeat::create(pAnimate, time / (ANIMAT_DELAY_PER_UNIT * m_pEffectResMap[effectID]));

    pAction->setTag(kDefaultSpriteEffectTag);
    pSprite->runAction(pAction);
    pSprite->runAction(CCSequence::create(CCDelayTime::create(time), CCRemoveSelf::create(), NULL));
    return pSprite;
}

// 向上飞出 淡入淡出
CCSprite* EffectResource::spriteWithActionFloat(const char* file, bool cleanSelf)
{
    CCSprite* pSprite = CCSprite::create(file);
    if (!pSprite)
    {
        return NULL;
    }
    
    CCFadeIn* pFadeIn = CCFadeIn::create(0.75);
    CCFadeOut* pFadeOut = CCFadeOut::create(0.75);
    CCAction* pAction = NULL;
    
    CCMoveBy* pMoveby = CCMoveBy::create(1.5, CCPoint(0, 180));
    if (cleanSelf)
    {
        pAction = CCSequence::create(CCSpawn::create(pMoveby, pFadeIn, NULL), CCDelayTime::create(1), pFadeOut, CCRemoveSelf::create(), NULL);
    }
    else
    {
        pAction = CCSequence::create(CCSpawn::create(pMoveby, pFadeIn, NULL), CCDelayTime::create(1), pFadeOut, NULL);
    }
    
    pAction->setTag(kDefaultSpriteEffectTag);
    pSprite->runAction(pAction);
    return pSprite;
}

CCNode* EffectResource::spriteWithFrameAnim(uint32_t effectID, bool cleanSelf/* = true*/)
{
    CCNode* effectNode = CCNode::create();
    char str[64] = {0};
    sprintf(str, EFFECT_PNG_FILE_FORMATE, effectID);
    
    CCSprite* worldSpr = CCSprite::create(str);
    
    sprintf(str, EFFECT_LIGHT_PNG_FILE_FORMATE, effectID);
    CCSprite* lightSpr1 = CCSprite::create(str);
    CCSprite* lightSpr2 = CCSprite::create(str);
    lightSpr2->setFlipX(true);
    
    worldSpr->addChild(lightSpr1);
    worldSpr->addChild(lightSpr2);
    lightSpr1->setPosition(ccp(worldSpr->getContentSize().width, worldSpr->getContentSize().height));
    lightSpr2->setPosition(ccp(0, 0));
    
    lightSpr1->runAction(CCSpawn::create(
                                         CCMoveBy::create(1.0f, ccp(-worldSpr->getContentSize().width, 0)),
                                         CCFadeOut::create(1.f),
                                         NULL));
    
    lightSpr2->runAction(CCSpawn::create(
                                         CCMoveBy::create(1.0f, ccp(worldSpr->getContentSize().width, 0)),
                                         CCFadeOut::create(1.f),
                                         NULL));
    
    worldSpr->setOpacity(0);
    worldSpr->runAction(CCSequence::create(CCFadeIn::create(1.5f),
                                           CCFadeOut::create(1.f),
                                           NULL));
    
    
    effectNode->addChild(worldSpr);
    effectNode->setPosition(SCREEN_MID_POS);
    XYTopLayer::getInstance()->addChild(effectNode, 128);
    
    return effectNode;
}

CCSprite* EffectResource::spriteWithActionFadeInOut(const char* file, bool isForever)
{
    CCSprite* pSprite = CCSprite::create(file);
    
    
    
    if (isForever)
    {
        CCSequence* act = CCSequence::create(CCFadeTo::create(0.3f, 50), CCFadeTo::create(0.3f, 255), NULL);
        pSprite->runAction(CCRepeatForever::create(act));
    }
    else
    {
        CCSequence* act = CCSequence::create(CCFadeTo::create(0.3f, 50), CCFadeTo::create(0.3f, 255), CCRemoveSelf::create(), NULL);
        pSprite->runAction(CCRepeatForever::create(act));
    }
    
    return pSprite;
}

void EffectResource::updateEffectInQueue(CCNode* parent) {
    if (this->getParent() && this->getParent() != this) {
        EffectResource::sharedInstance()->removeFromParent();
    }
    if (this->getParent() == NULL) {
        parent->addChild(this);
    }
}

void EffectResource::update(float dt) {
    
    if (m_ftimer >= 1.f) {
        m_ftimer = 0.f;
         
        if (m_pEffectQueue->count() > 0) {
            CCSprite* pEffSprite = (CCSprite*)m_pEffectQueue->objectAtIndex(0);
            
            if (pEffSprite->getTag() == 27) // 角色升级，身上效果
            {

            }
            else
            {
                pEffSprite->setPosition(SCREEN_MID_POS);
                XYTopLayer::getInstance()->addChild(pEffSprite, 128);
            }
            
            m_pEffectQueue->removeObjectAtIndex(0);
        }
        
    } else {
        m_ftimer += dt;
    }    
}

void EffectResource::onEnter() {
    CCNode::onEnter();
    m_ftimer = 1.f;
    schedule(schedule_selector(EffectResource::update), ANIMAT_DELAY_PER_UNIT);
}

void EffectResource::onExit() {
    m_pEffectQueue->removeAllObjects();
    unschedule(schedule_selector(EffectResource::update));
    CCNode::onExit();
}

void EffectResource::showEffectInQueue(uint32_t effectID, bool cleanSelf) {
    CCSprite* effect = spriteWithEffect(effectID, false, cleanSelf);
    effect->setTag(effectID);
    m_pEffectQueue->addObject(effect);
}

void EffectResource::showEffectWithFloatInQueue(const char* file, bool cleanSelf) {
    CCSprite* effect = spriteWithActionFloat(file, cleanSelf);
    m_pEffectQueue->addObject(effect);
}
