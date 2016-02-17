//
//  BattleManager.h
//  dragon
//
//  Created by xingchong on 2/17/16.
//
//

#ifndef __dragon__BattleManager__
#define __dragon__BattleManager__

#include <stdio.h>
#include "cocos2d.h"
#include "BattleMapUnits.h"

USING_NS_CC;

class BattleManager : public Ref
{
private:
    BattleManager();
    
public:
    ~BattleManager();
    
    static BattleManager *getInstance();
    
public:
    bool createLogicLayer();
    void start();
    void finish();
    void destory();
    
    BattleMapUnits *getBattleMapUnits(){return _battleMapUnits;};
    
private:
    BattleMapUnits *_battleMapUnits;
    
};

#endif /* defined(__dragon__BattleManager__) */
