//
//  MonsterData.cpp
//  dragon
//
//  Created by xingchong on 2/25/16.
//
//

#include "MonsterData.h"

MonsterData::MonsterData()
{
    
}

MonsterData::~MonsterData()
{
    
}

void MonsterData::init(int guid, int configId, int unitType)
{
    BattleBaseData::init(guid, configId, unitType);
    
    HCSVFile *csvFile = ConfigReader::getInstance()->getFileByName(Global::getInstance()->_csvConfig->_monsterInit);
    ValueMap monsterConfig = csvFile->getRowData(_configId);
    _resId       = monsterConfig["resTemplateId"].asInt();
    _moveSpeed   = monsterConfig["moveSpeed"].asInt();
    _jumpSpeed   = 1;
    _level       = monsterConfig["level"].asInt();
    _maxHp       = monsterConfig["maxHp"].asInt();
    _curHp       = monsterConfig["maxHp"].asInt();
    _attack      = monsterConfig["attack"].asInt();
    _defense     = monsterConfig["defense"].asInt();
    _hitRate     = monsterConfig["hitRate"].asInt();
    _jouk        = monsterConfig["jouk"].asInt();
    _crit        = monsterConfig["crit"].asInt();
    _critRate    = monsterConfig["critRate"].asFloat();
    _agile       = monsterConfig["agile"].asInt();
    _power       = monsterConfig["power"].asInt();
    _aiId        = monsterConfig["aiInitId"].asInt();
    _skillInitId = monsterConfig["skillInitId"].asInt();
}


