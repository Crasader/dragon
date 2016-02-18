//
//  HeroData.cpp
//  dragon
//
//  Created by xingchong on 2/18/16.
//
//

#include "HeroData.h"

HeroData::HeroData()
{
    
}

HeroData::~HeroData()
{
    
}

void HeroData::init(int guid, int configId, int unitType)
{
    BattleBaseData::init(guid, configId, unitType);
    this->initData();
    this->initSkill();
}

void HeroData::initData()
{
    
}

void HeroData::initSkill()
{
    
}

