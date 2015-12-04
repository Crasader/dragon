//
//  MapObject.h
//  DragonBattle
//
//  Created by 小敦 王 on 13-5-8.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_MapObject_h
#define DragonBattle_MapObject_h

#include "cocos2d.h"
#include "GlobalDefine.h"

class MapObject : public cocos2d::CCNode
{
public:
    MapObject();
    ~MapObject();
    
    uint32_t getID() { return m_ID; }
    kUnitFightType getFightType() { return m_kFightType; }
    
protected:
    kUnitFightType  m_kFightType;
    uint32_t        m_ID;
};

#endif
