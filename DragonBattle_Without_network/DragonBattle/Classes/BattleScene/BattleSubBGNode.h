//
//  BattleSubBGNode.h
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-17.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_BattleSubBGNode_h
#define DragonBattle_BattleSubBGNode_h

#include "cocos2d.h"

#include "GameData.h"

class BattleSubBGNode : public cocos2d::CCNode
{
public:
    BattleSubBGNode();
    ~BattleSubBGNode();
    
    // create近景层，根据地图长度来随机计算各个图片的坐标
    static BattleSubBGNode* createNearScene(const MissionBGVec* pBgConf, float fMapLength);
    
    bool initNearScene(const MissionBGVec* pBgConf, float fMapLength);
    
    inline float getNodeWidth() { return m_fWidth; }
    
    inline void setNodeWidth(float width) { m_fWidth = width; }
    
private:
    float   m_fWidth;
};

#endif
