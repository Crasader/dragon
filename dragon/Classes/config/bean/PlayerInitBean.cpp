//
//  PlayerInitBean.cpp
//  dragon
//
//  Created by xingchong on 2/18/16.
//
//

#include "PlayerInitBean.h"

static PlayerInitBean* create(int configId)
{
    PlayerInitBean* ret = new PlayerInitBean();
    if (ret->init(configId))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

PlayerInitBean::PlayerInitBean()
{
    _fileName = "player_init";
}

PlayerInitBean::~PlayerInitBean()
{
    
}

bool PlayerInitBean::init(int configId)
{
    _configId = configId;
    HCSVFile* csvFile = ConfigReader::getInstance()->getFileByName(_fileName);
    ValueMap rowValueMap = csvFile->getRowData(configId);
    
    _id = rowValueMap["id"].asInt();
    _name = rowValueMap.at("name").asString();
    _resTemplateId = rowValueMap.at("resTemplateId").asInt();
    _resEffectId = rowValueMap.at("resEffectId").asInt();
    std::string temp = rowValueMap["skillInitIds"].asString();
    _skillInitIds = this->stringToMutiVector(temp);
    _aiInitId = rowValueMap.at("aiInitId").asInt();
    
    return true;
}
