//
//  FlashSprite.h
//  DragonBattle
//
//  Created by 何 永翔 on 13-7-15.
//
//

#ifndef DragonBattle_FlashSprite_h
#define DragonBattle_FlashSprite_h

#include "cocos2d.h"

class FlashSprite : public cocos2d::CCSprite
{
public:
    FlashSprite();
    virtual ~FlashSprite();
    
    static FlashSprite* create(const char* pFlash1, const char* pFlash2, float fFlashSpeed, float fFlashInterval);
    virtual bool initWithFile(const char* pFlash1, const char* pFlash2, float fFlashSpeed, float fFlashInterval);
    
private:
    cocos2d::CCRepeatForever* createRepeatAction();
    void runSprite2Action(float dt);
    
    cocos2d::CCSprite* m_pBgSprite2;
    float m_fFlashSpeed;
    float m_fFlashInterval;
};

#endif
