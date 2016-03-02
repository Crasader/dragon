//
//  MonsterContainer.h
//  dragon
//
//  Created by xingchong on 3/3/16.
//
//

#ifndef __dragon__MonsterContainer__
#define __dragon__MonsterContainer__

#include <stdio.h>
#include <vector>
#include "cocos2d.h"
#include "HeroSprite.h"
#include "MapObstacle.h"

USING_NS_CC;

class MonsterContainer : public Ref
{
public:
    MonsterContainer();
    ~MonsterContainer();
    
private:
    std::vector<HeroSprite>     *_monsterVector;
    
};

#endif /* defined(__dragon__MonsterContainer__) */
