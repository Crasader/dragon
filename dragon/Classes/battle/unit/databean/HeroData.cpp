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
    HCSVFile *csvFile = ConfigReader::getInstance()->getFileByName(Global::getInstance()-> _csvConfig->_playerInit);
    ValueMap heroConfig = csvFile->getRowData(_configId);
    _resId = heroConfig["resTemplateId"].asInt();
    
    ValueVector tempVector;
    csvFile->strToValueVector(heroConfig["skillInitIds"].asString().c_str(), tempVector, "|");
    for (int i=0; i<tempVector.size(); i++) {
        ValueVector skillCell;
        csvFile->strToValueVector(tempVector[i].asString().c_str(), skillCell, ";");
        _skillVector[i] = skillCell;
    }
    
    _moveSpeed = 0.4;
    _jumpSpeed = 1;
    _level     = 1;
    _maxHp     = 1000;
    _curHp     = 1000;
    _attack    = 50;
    _defense   = 10;
    _hitRate   = 100;
    _jouk      = 75;
    _crit      = 40;
    _critRate  = 1.5;
    _power     = 100;
}

void HeroData::initSkill()
{
    
}

