//
//  BattleMapUnits.h
//  dragon
//
//  Created by xingchong on 2/17/16.
//
//

#ifndef __dragon__BattleMapUnits__
#define __dragon__BattleMapUnits__

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;

class BattleMapUnits : public Layer
{
public:
    BattleMapUnits();
    ~BattleMapUnits();
    
    void start();
    void finish();
    void destory();
    
};

#endif /* defined(__dragon__BattleMapUnits__) */
