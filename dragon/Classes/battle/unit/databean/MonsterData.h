//
//  MonsterData.h
//  dragon
//
//  Created by xingchong on 2/25/16.
//
//

#ifndef __dragon__MonsterData__
#define __dragon__MonsterData__

#include <stdio.h>
#include "BattleBaseData.h"

class MonsterData : public BattleBaseData
{
public:
    MonsterData();
    ~MonsterData();
    
    virtual void init(int guid, int configId, int unitType) override;
    
private:
    int _skillInitId;
    
};

#endif /* defined(__dragon__MonsterData__) */
