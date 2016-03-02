//
//  HeroContainer.h
//  dragon
//
//  Created by xingchong on 3/3/16.
//
//

#ifndef __dragon__HeroContainer__
#define __dragon__HeroContainer__

#include <stdio.h>
#include <vector>
#include "cocos2d.h"
#include "HeroSprite.h"
#include "MapObstacle.h"

USING_NS_CC;

class HeroContainer : public Ref
{
public:
    HeroContainer();
    ~HeroContainer();
    
private:
    HeroSprite                         *_myHero;
    std::vector<HeroSprite>     *_partnerVector;
    
};
#endif /* defined(__dragon__HeroContainer__) */
