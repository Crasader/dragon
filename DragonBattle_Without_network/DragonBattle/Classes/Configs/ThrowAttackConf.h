//
//  ThrowAttackConf.h
//  DragonBattle
//
//  Created by 永翔 何 on 13-5-17.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_ThrowAttackConf_h
#define DragonBattle_ThrowAttackConf_h

#ifdef WONPEE_SERVER
#include "cocos2d_diff.h"
#else
#include "cocos2d.h"
#endif

#include "XmlEncrypt.h"
#include <vector>
#include <map>

// 抛出物类型
enum kThrowType
{
    kSpeed = 1,     //根据速度调整运行轨迹
    kBezier,        //贝塞尔曲线调整轨迹
    kFollow,        //跟随
    kTrace,         //跟踪
};

// 抛出物碰撞类型
enum kThrowAttackType
{
    kExplode = 1,  //碰撞后爆炸，产生AOE效果，飞出场景移出
    kCollide,  //碰撞后爆炸，但不产生AOE效果 单体伤害，碰撞后消失，飞出场景移出
    kDistance, //距离移出
    kOutScreen,//飞出场景移出
    kTime,     //时间移出
};

struct ThrowAttackConfig_t
{
    uint16_t nID;
    
    uint16_t nResID;
    // 抛出物类型
    kThrowType          nThrowType;
    
    // 抛出物是否固定方向，不随角色方向变动而变动
    uint16_t nFixDirection;
    //抛出物是否按第一次抛出的角色位置做变动
    uint16_t nFixPoi;
    // 碰撞类型
    kThrowAttackType    nAttackType;
    
    // 移出时的判定值 时间移出则为持续时间 距离移出则为移动距离
    float fAttackValue;
    
    uint16_t nJudgeCollide;
    float fAttackInterval;
    
    cocos2d::CCPoint    ptThrowStart;
    
    union ThrowUnion_t
    {
        struct ThrowSpeed_t
        {
            float               fThrowSpeedX;
            float               fThrowSpeedY;
            float               fThrowSpeedAddX;
            float               fThrowSpeedAddY;
        } throwSpeed;
        struct ThrowBezier_t
        {
            float               fThrowSpeed;
            float               fThrowHeight;
        } throwBezier;
        struct ThrowFollow_t
        {
            float               fThrowFollowX;
            float               fThrowFollowY;
        } throwFollow;
        struct ThrowTrace_t
        {
            float               fThrowTraceX;
            float               fThrowTraceY;            
        } throwTrace;
    }throwUnion;
};

typedef std::map<uint16_t, ThrowAttackConfig_t*>  ThrowAttackConfigMap;

class ThrowAttackConfig
{
public:
    ThrowAttackConfig();
    ~ThrowAttackConfig();

	bool loadFromFile(const char* xmlFile, bool bEncrypt = false);
    void unloadThrowAttackConfig();
    
    // 校验配置表
    bool checkThrowAttackConfigRight();
    
    ThrowAttackConfig_t* getThrowAttackFromID(uint16_t throwAttackID) const;
    
private:
    ThrowAttackConfigMap m_ThrowAttackConfigMap;
};

#endif
/*
<THROW_ATTACK_CONFIG>
<THROW_ATTACK ID="1" attackType="1" attackValue="0.2" throwType="1" throwStartX="72,1">
<THROW_SPEED throwSpeedX="2" throwSpeedY="0" throwSpeedAddX="2" throwSpeedAddY="0"/>
</THROW_ATTACK>
<THROW_ATTACK ID="2" attackType="1" attackValue="0.2" throwType="2" throwStartX="72,1">
<THROW_BEZIER throwSpeed="3" throwHeight="3"/>
</THROW_ATTACK>
<THROW_ATTACK ID="3" attackType="1" attackValue="0.2" throwType="3" throwStartX="72,1">
<THROW_FOLLOW throwFollowX="3" throwFollowY="3"/>
</THROW_ATTACK>
<THROW_ATTACK ID="4" attackType="1" attackValue="0.2" throwType="4" throwStartX="72,1">
<THROW_TRACE throwTraceX="3" throwTraceY="3"/>
</THROW_ATTACK>	
</THROW_ATTACK_CONFIG>
*/
