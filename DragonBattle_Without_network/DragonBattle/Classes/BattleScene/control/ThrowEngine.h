//
//  ThrowEngine.h
//  DragonBattle
//
//  Created by 小敦 王 on 13-5-16.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_ThrowEngine_h
#define DragonBattle_ThrowEngine_h

#include "cocos2d.h"
#include "Resources.h"
#include "SkillConf.h"
#include "BattleUnitConf.h"

class BattleUnitSprite;
class ThrowEngine : public cocos2d::CCNode
{
public:
    ThrowEngine();
    ~ThrowEngine();
    
    static ThrowEngine* create(BattleUnitSprite* unit);
    virtual bool init(BattleUnitSprite* unit);
    
    // 给外部的接口，生成抛出物
    void throwObject();
    void setThrowVec(std::vector<uint16_t>  *throwVec);//侠客抛出物需设置
    void setThrowMode(kThrowMode mode);
    // 多个抛出物间隔生成是的定时操作
    void engineThrowObject(float dt);
    
private:
    // 抛出配置表中的第index个object
    void throwObject(int index);
    
private:
    BattleUnitSprite*       m_pTargetUnit;
    AttackInfo_t*           m_pAttackInfo;       //当前出招
    UnitBaseConf            m_unitBaseConf;
    int                     m_nCurThrowIndex;
    
    CCPoint                 m_Point;
    int                     m_nDir;
    std::vector<uint16_t>   m_throwVec;//抛出物
    kThrowMode              m_kThorwMode;//抛出类型
    CC_SYNTHESIZE(int, m_pSkillConfID, SkillConfID);
    CC_SYNTHESIZE(int, m_nIntervarTime, IntervarTime);
};

#endif
