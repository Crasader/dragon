//
//  DropItem.h
//  DragonBattle
//
//  Created by 何 永翔 on 13-7-23.
//
//

#ifndef DragonBattle_DropItem_h
#define DragonBattle_DropItem_h

#include "cocos2d.h"
#include "GlobalDefine.h"

class DropItem : public cocos2d::CCSprite
{
public:
    DropItem();
    virtual ~DropItem();
    
    static DropItem* create(int nItemID, int nCount, bool bIsThief);
    bool initWithConfig(int nItemID, int nCount, bool bIsThief);
    
    CCRect getUnitRect();
    
    void updatePos();
    
    inline int GetItemID() { return m_nItemID; }
    inline int GetItemCount() { return m_nCount; }
    inline bool IsThief() { return m_bIsThief; }
    
private:
    kObstacleDirection getObstacleDirection(float moveX, float moveY, float* pCorrectX, float* pCorrectY);
    void stopAnimation();
    
    int m_nItemID;
    int m_nCount;
    bool m_bIsThief;
    float m_fInitSpeedY;
};

#endif
