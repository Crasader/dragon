//
//  MapObstacle.h
//  dragon
//
//  Created by xingchong on 3/2/16.
//
//

#ifndef __dragon__MapObstacle__
#define __dragon__MapObstacle__

#include <stdio.h>
#include "MapSprite.h"

class MapObstacle : public MapSprite
{
public:
    MapObstacle();
    ~MapObstacle();

public:
    virtual void update(float dt) override;
    
    virtual void initView();
    
};

#endif /* defined(__dragon__MapObstacle__) */
