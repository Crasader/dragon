//
//  Obstacle.h
//  DragonBattle
//
//  Created by 永翔 何 on 13-5-19.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_Obstacle_h
#define DragonBattle_Obstacle_h

#include "cocos2d.h"
#include "BattleMissionConf.h"
#include "MapObject.h"

class MapObstacle : public MapObject
{
public:
    MapObstacle();
    ~MapObstacle();
    
    static MapObstacle* create();
    
    static MapObstacle* create(const Obstacle_t* pConf);
    virtual bool init(const Obstacle_t* pConf = NULL);
    
    void setCollideRect(cocos2d::CCRect rc);
    // 得到碰撞区域
    cocos2d::CCRect getCollideRect();
    
    inline void setObstacleDirection(kObstacleDirection nObstacleDirection) { m_nObstacleDirection = nObstacleDirection; }
    inline kObstacleDirection getObstacleDirection() { return m_nObstacleDirection; }
    
private:
    cocos2d::CCSprite*      m_pSelfSprite;  //障碍物sprite
    const Obstacle_t* m_pConf;        //障碍物配置
    cocos2d::CCRect         m_rcCollide;    //碰撞区域
    cocos2d::CCLayerColor* m_pRectColor;
    
    kObstacleDirection m_nObstacleDirection;
};

#endif
