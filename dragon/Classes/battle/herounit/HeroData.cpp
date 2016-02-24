//
//  HeroData.cpp
//  dragon
//
//  Created by xingchong on 2/18/16.
//
//

#include "HeroData.h"
#include "ConfigReader.h"
#include "HCSVFile.h"
#include "Global.h"

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
    HCSVFile *csvFile = ConfigReader::getInstance()->getFileByName(Global::Csv::playerInit);
}

void HeroData::initSkill()
{
    
}

