//
//  EffectResource.h
//  DragonBattle
//
//  Created by 小敦 王 on 13-9-3.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_EffectResource_h
#define DragonBattle_EffectResource_h

#include "cocos2d.h"
#include <string>
#include <map>

#define EFFECT_TIP_TAG (10000)
#define EFFECT_19_TAG (10019)

USING_NS_CC;

enum
{
    kDefaultSpriteEffectTag = 0xCCCD0001,
};

class EffectResource : public CCNode
{
public:
    EffectResource();
    ~EffectResource();
    static EffectResource* sharedInstance();
    
    bool loadFromFile();
    
    cocos2d::CCAnimate* createEffectAnimate(uint32_t effectID);
        
    //序列动画，动画完后清除自己
    cocos2d::CCSprite* spriteWithEffect(uint32_t effectID, bool isForever, bool cleanSelf, float delayPerUnit = 1.f);
    cocos2d::CCSprite* spriteWithEffect(uint32_t effectID, float time, bool cleanSelf);

    // 向上飞出 淡入淡出
    cocos2d::CCSprite* spriteWithActionFloat(const char* file, bool cleanSelf);
    
    cocos2d::CCNode* spriteWithFrameAnim(uint32_t effectID, bool cleanSelf = true);
    
    cocos2d::CCSprite* spriteWithActionFadeInOut(const char* file, bool isForever = true);
    
    void updateEffectInQueue(CCNode* parent);
    
    void showEffectInQueue(uint32_t effectID, bool cleanSelf = true);
    void showEffectWithFloatInQueue(const char* file, bool cleanSelf = true);
    
    void removeSpriteFrame(CCString* effect);
    
    void update(float dt);
    virtual void onEnter();
    virtual void onExit();

private:
    std::map<uint32_t, uint32_t> m_pEffectResMap;
    
    static EffectResource* pSharedInstance;
    
    CCArray* m_pEffectQueue;
    float m_ftimer;
};

#endif
