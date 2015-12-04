//
//  ParticleResource.cpp
//  DragonBattle
//
//  Created by 白明江 on 11/20/13.
//
//

#include "ParticleResource.h"

static ParticleResource* g_sParticleResourceInstance = NULL;

ParticleResource::ParticleResource()
{
}

ParticleResource::~ParticleResource()
{
}

ParticleResource* ParticleResource::getInstance()
{
    if (g_sParticleResourceInstance == NULL)
    {
        g_sParticleResourceInstance = new ParticleResource;
    }
    
    return g_sParticleResourceInstance;
}

CCParticleSnow* ParticleResource::createPlantEffect(CCPoint pos)
{
    CCParticleSnow* particle = CCParticleSnow::createWithTotalParticles(10);
    particle->setPosition(pos);
    particle->setPosVar(ccp(40, 0));
    particle->setGravity(ccp(0, 1));
    particle->setAngle(90);
    particle->setLife(3);
    particle->setLifeVar(0.25);
    particle->setSpeed(40);
    particle->setSpeedVar(15);
    particle->setStartSize(6);
    particle->setStartSizeVar(3);
    particle->setPositionType(kCCPositionTypeGrouped);
    
    return particle;
}

CCParticleSystemQuad* ParticleResource::createDropMoneyEffect(CCPoint pos)
{
    CCParticleSystemQuad* particle = new CCParticleSystemQuad;
    
    
    particle->initWithTotalParticles(40);
    
    // duration
    particle->setDuration(kCCParticleDurationInfinity);
    // radius mode
    particle->setEmitterMode(kCCParticleModeGravity);
    
    const ccColor4F color = ccc4f(1, 1, 1, 1);
    const ccColor4F color_var = ccc4f(0, 0, 0, 1);
    particle->setStartColor(color);//设置粒子RGBA值
    particle->setStartColorVar(color_var);
    particle->setEndColor(color);
    particle->setEndColorVar(color_var);
    
    particle->setBlendAdditive(false);
    particle->setPosition(pos);//设置发射粒子的位置
    particle->setAutoRemoveOnFinish(true);
    
    particle->setPosVar(ccp(150, 20));
    particle->setGravity(ccp(0, 1));
    
    particle->setAngle(-90);
    particle->setAngleVar(0);
    
    particle->setLife(1);
    particle->setLifeVar(0.05);
    
    particle->setSpeed(200);
    particle->setSpeedVar(0);
    
    particle->setStartSize(30);
    particle->setStartSizeVar(0);
    
    particle->setEndSize(30);
    particle->setEndSizeVar(0);
    
    particle->setPositionType(kCCPositionTypeGrouped);
    
    particle->setEmissionRate(particle->getTotalParticles() / particle->getLife());
    

    particle->autorelease();
    
    return particle;
}

CCParticleSystemQuad* ParticleResource::createFireEffect()
{
    CCParticleSystemQuad* particle = CCParticleSystemQuad::create("effect/fire.plist");
    return particle;
}

CCParticleSystemQuad* ParticleResource::createFireEffect(CCNode* pBtn, CCRect rect, float factor/* = 0.5*/)
{
    CCParticleSystemQuad* btnEffect = createFireEffect();
    pBtn->addChild(btnEffect);
    
    float tag = factor;
    
    CCMoveBy* move1 = CCMoveBy::create(1.f * tag, ccp(rect.size.width, 0));
    CCMoveBy* move2 = CCMoveBy::create(0.5f * tag, ccp(0, rect.size.height));
    CCMoveBy* move3 = CCMoveBy::create(1.f * tag, ccp(-rect.size.width, 0));
    CCMoveBy* move4 = CCMoveBy::create(0.5f * tag, ccp(0, -rect.size.height));
    
    btnEffect->setAngle(180);
    btnEffect->setPosition(rect.origin);
    btnEffect->setPositionType(kCCPositionTypeRelative);
    
    CCSequence* sequence = CCSequence::create(
                                              move1,
                                              CCCallFuncO::create(this, callfuncO_selector(ParticleResource::resetParticleAngle),
                                                                  CCArray::create(btnEffect, CCInteger::create(270), NULL)),
                                              move2,
                                              CCCallFuncO::create(this, callfuncO_selector(ParticleResource::resetParticleAngle),
                                                                  CCArray::create(btnEffect, CCInteger::create(0), NULL)),
                                              move3,
                                              CCCallFuncO::create(this, callfuncO_selector(ParticleResource::resetParticleAngle),
                                                                  CCArray::create(btnEffect, CCInteger::create(90), NULL)),
                                              move4,
                                              CCCallFuncO::create(this, callfuncO_selector(ParticleResource::resetParticleAngle),
                                                                  CCArray::create(btnEffect, CCInteger::create(180), NULL)),
                                              NULL);
    
    btnEffect->runAction(CCRepeatForever::create(sequence));
    
    return btnEffect;
}

void ParticleResource::resetParticleAngle(CCObject* pObj)
{
    CCArray* pArr = dynamic_cast<CCArray*>(pObj);
    CCInteger* value = dynamic_cast<CCInteger*>(pArr->objectAtIndex(1));
    CCParticleSystemQuad* effect = dynamic_cast<CCParticleSystemQuad*>(pArr->objectAtIndex(0));
    if (value && effect)
    {
        //effect->setAngle(value->getValue());
        //effect->cocos2d::CCNode::setRotation(value->getValue());
    }
}

CCParticleSystemQuad* ParticleResource::createSynthesisAni()
{
    CCParticleSystemQuad* particle = CCParticleSystemQuad::create("effect/synthesis_effect_1.plist");
    return particle;
}

CCParticleSystemQuad* ParticleResource::createSynthesisNewItemAni()
{
    CCParticleSystemQuad* particle = CCParticleSystemQuad::create("effect/synthesis_effect_2.plist");
    return particle;
}

