//
//  ObstacleContainer.h
//  dragon
//
//  Created by xingchong on 3/3/16.
//
//

#ifndef __dragon__ObstacleContainer__
#define __dragon__ObstacleContainer__

#include <stdio.h>
#include <vector>
#include "cocos2d.h"
#include "MapObstacle.h"
USING_NS_CC;

class ObstacleContainer : public Ref
{
public:
    ObstacleContainer();
    ~ObstacleContainer();
    
private:
    std::vector<MapObstacle>        _mapObstacleVector;
    
};

#endif /* defined(__dragon__ObstacleContainer__) */
