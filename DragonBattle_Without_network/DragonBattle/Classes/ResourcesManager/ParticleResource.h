//
//  ParticleResource.h
//  DragonBattle
//
//  Created by 白明江 on 11/20/13.
//
//

#ifndef __DragonBattle__ParticleResource__
#define __DragonBattle__ParticleResource__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class ParticleResource : public CCObject
{

public:
    ParticleResource();
    virtual ~ParticleResource();
    
    static ParticleResource* getInstance();
    
    static CCParticleSnow* createPlantEffect(CCPoint pos);
    static CCParticleSystemQuad* createDropMoneyEffect(CCPoint pos);
    
    CCParticleSystemQuad* createFireEffect();
    CCParticleSystemQuad* createFireEffect(CCNode* pBtn, CCRect rect, float factor = 0.5);
    
    CCParticleSystemQuad* createSynthesisAni();
    CCParticleSystemQuad* createSynthesisNewItemAni();
    
private:
    void resetParticleAngle(CCObject* angle);
};

#endif /* defined(__DragonBattle__ParticleResource__) */
