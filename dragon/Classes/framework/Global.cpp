//
//  Global.cpp
//  dragon
//
//  Created by xingchong on 2/24/16.
//
//

#include "Global.h"

static Global *_instance = nullptr;

Global* Global::getInstance()
{
    if (!_instance)
    {
        _instance = new Global();
        _instance->init();
    }
    return _instance;
}


Global::Global()
{
    
}

Global::~Global()
{
    
}

bool Global::init()
{
    _csvConfig = CsvConfig::create();
    
    return true;
}
