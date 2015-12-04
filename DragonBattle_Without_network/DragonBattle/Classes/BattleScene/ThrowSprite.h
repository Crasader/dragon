//
//  ThrowSprite.h
//  DragonBattle
//
//  Created by 永翔 何 on 13-5-6.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_ThrowSprite_h
#define DragonBattle_ThrowSprite_h

#include "cocos2d.h"
#include "BattleUnitSprite.h"
#include "Resources.h"
#include "MapObject.h"
#include "ThrowAttackConf.h"

enum kThrowSpriteDirection
{
    kThrowSpriteDirectionFollow = 0,
    kThrowSpriteDirectionLeft,
    kThrowSpriteDirectionRight
};

class ThrowSprite : public MapObject
{
public:
    ThrowSprite();
    ~ThrowSprite();

    static ThrowSprite* create(BattleUnitSprite* pUnitSprite,
                               ThrowAttackConfig_t* throwAttackConfig, 
                               AttackInfo_t* attackInfo,
                               int pSkillConfIDLV,
                               UnitBaseConf* m_pUnitBaseConf,
                               kThrowSpriteDirection nDir,CCPoint pt);

    cocos2d::CCPoint updatePos();
    inline cocos2d::CCPoint getSpeed() { return m_speed; }
    void setAttacRectVisible(bool bShow);
    cocos2d::CCRect getAttackRect();
    kThrowAttackType getAttackType();
    void explode(cocos2d::CCPoint point);
    void destroy();
    bool isFlipX();
    inline bool isExploding() { return m_bIsExploding; }
    void timeIsUp(float dt);
    
    AttackInfo_t* getAttackInfo() { return m_pAttackInfo; }
    ThrowResource_t* getThrowResource() const { return m_pThrowInfo; }
    UnitBaseConf getUnitBaseConf() { return m_UnitBaseConf; }
    const kThrowType getThrowType() { return m_pThrowAttackConf->nThrowType; }
    inline const bool judgeCollide() { return m_pThrowAttackConf->nJudgeCollide == 1; }
    inline const float getAttackInterval() { return m_pThrowAttackConf->fAttackInterval; }
    
    inline BattleUnitSprite* getThrower() { return m_pUnitSprite; }
    
    std::map<BattleUnitSprite*, clock_t> hitMap;

protected:
    virtual bool init(BattleUnitSprite* pUnitSprite, 
                      ThrowAttackConfig_t* throwAttackConfig, 
                      AttackInfo_t* attackInfo,
                      int pSkillConfIDLV,
                      UnitBaseConf* m_pUnitBaseConf,
                      kThrowSpriteDirection nDir,CCPoint pt);
    void removeFromParent();
    
private:
    cocos2d::CCSprite*      m_pAnimateSprite;//动画精灵
    cocos2d::CCLayerColor*  m_pAttackRectLayer;
    
    cocos2d::CCPoint        m_posStart;
    cocos2d::CCPoint        m_speed;
    float m_fHorizon;
    float m_fThrowSpeedX;
    float m_fThrowSpeedY;
    float m_fThrowSpeedAddX;
    float m_fThrowSpeedAddY;
    float m_fThrowFollowX;
    float m_fThrowFollowY;
    bool  m_bIsFlipX;
    bool  m_bIsExploding;
    kThrowSpriteDirection m_nDir;
    
    BattleUnitSprite* m_pUnitSprite;
    // 攻击信息
    AttackInfo_t*           m_pAttackInfo;
    // 抛出物配置
    ThrowAttackConfig_t*    m_pThrowAttackConf;
    
    UnitBaseConf m_UnitBaseConf;
    // 抛出物资源
    ThrowResource_t*        m_pThrowInfo;
    //技能配置
    CC_SYNTHESIZE(int, m_pSkillConfIDLV, skillConfIDLV);
};


#endif
