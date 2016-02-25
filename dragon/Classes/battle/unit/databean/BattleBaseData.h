//
//  BattleBaseData.h
//  dragon
//
//  Created by xingchong on 2/18/16.
//
//

#ifndef __dragon__BattleBaseData__
#define __dragon__BattleBaseData__

#include <stdio.h>
#include "cocos2d.h"
#include "ConfigReader.h"
#include "HCSVFile.h"
#include "Global.h"

USING_NS_CC;

class BattleBaseData : public Ref
{
public:
    BattleBaseData();
    ~BattleBaseData();
    
    virtual void init(int guid, int configId, int unitType);
    
protected:
    int _configId;
    int _guid;
    int _unitType;
    int _resId;
    int _aiId;
    
    float _moveSpeed;
    int _jumpSpeed;
    
    int _level;
    int _maxHp;
    int _curHp;
    int _attack;
    int _defense;
    int _hitRate;
    int _jouk;
    int _crit;
    float _critRate;
    int _agile;
    int _power;
    
};

#endif /* defined(__dragon__BattleBaseData__) */
