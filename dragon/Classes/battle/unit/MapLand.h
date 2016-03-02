//
//  MapLand.h
//  dragon
//
//  Created by xingchong on 3/2/16.
//
//

#ifndef __dragon__MapLand__
#define __dragon__MapLand__

#include <stdio.h>
#include "MapObstacle.h"

class MapLand : public MapObstacle
{
public:
    MapLand();
    ~MapLand();
    
public:
    virtual void update(float dt) override;
    virtual void setLogicPosition(Vec2 point) override;
    
};

#endif /* defined(__dragon__MapLand__) */
