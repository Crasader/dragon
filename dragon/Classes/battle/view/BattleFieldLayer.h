//
//  BattleFieldLayer.h
//  dragon
//
//  Created by xingchong on 3/3/16.
//
//

#ifndef __dragon__BattleFieldLayer__
#define __dragon__BattleFieldLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "MonsterContainer.h"
#include "HeroContainer.h"
#include "ObstacleContainer.h"

USING_NS_CC;

class BattleFieldLayer : public Layer
{
public:
    BattleFieldLayer();
    ~BattleFieldLayer();
    
    virtual bool init() override;
    void start();
    void finish();
    void destory();
    
private:
    MonsterContainer     *_monsterContainer;
    HeroContainer        *_heroContainer;
    ObstacleContainer    *_obstacleContainer;
    HeroSprite           *_myHero;
    
};

#endif /* defined(__dragon__BattleFieldLayer__) */
