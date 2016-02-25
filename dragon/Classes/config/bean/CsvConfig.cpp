//
//  CsvConfig.cpp
//  dragon
//
//  Created by xingchong on 2/24/16.
//
//

#include "CsvConfig.h"

CsvConfig* CsvConfig::create()
{
    auto *ret = new CsvConfig();
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

CsvConfig::CsvConfig()
{
    
}

CsvConfig::~CsvConfig()
{
    
}

bool CsvConfig::init()
{
    _playerInit  = "player_init";
    _monsterInit = "monster_init";
    
    return true;
}


