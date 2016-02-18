//
//  BattleBaseData.cpp
//  dragon
//
//  Created by xingchong on 2/18/16.
//
//

#include "BattleBaseData.h"

BattleBaseData::BattleBaseData()
{
    
}

BattleBaseData::~BattleBaseData()
{
    
}

void BattleBaseData::init(int guid, int configId, int unitType)
{
    _guid     = guid;
    _configId = configId;
    _unitType = unitType;
}

