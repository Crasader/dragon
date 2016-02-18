//
//  HeroData.h
//  dragon
//
//  Created by xingchong on 2/18/16.
//
//

#ifndef __dragon__HeroData__
#define __dragon__HeroData__

#include <stdio.h>
#include "BattleBaseData.h"

class HeroData : public BattleBaseData
{
public:
    HeroData();
    ~HeroData();
    
    virtual void init(int guid, int configId, int unitType);
    void initData();
    void initSkill();
    
};

#endif /* defined(__dragon__HeroData__) */
