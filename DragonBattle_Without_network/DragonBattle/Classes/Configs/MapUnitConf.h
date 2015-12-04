//
//  MapUnitConf.h
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-19.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_MapUnitConf_h
#define DragonBattle_MapUnitConf_h

#include <map>
#include <vector>

#ifdef WONPEE_SERVER
#include "cocos2d_diff.h"
#else
#include "cocos2d.h"
#endif

struct MapBaseUnit_t
{
    uint32_t m_ID;              //ID
    uint32_t m_nUnitType;       //类型
    cocos2d::CCPoint postion;   //配置的坐标
};

// 掉落物
struct MapDropUnit_t : public MapBaseUnit_t
{
    
};


// 宝箱类 可被攻击
struct MapBoxUnit : public MapBaseUnit_t
{
    
};

// 障碍物 地刺 火 陷阱
struct MapObstacleUnit : public MapBaseUnit_t
{
    
};

typedef std::map<uint32_t, MapBaseUnit_t*> MapUnitConfMap;
#endif
