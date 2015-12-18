//
//  BattleScene.cpp
//  dragon
//
//  Created by xingchong on 12/11/15.
//
//

#include "BattleScene.h"

BattleScene *BattleScene::create()
{
    BattleScene *ret = new BattleScene();
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    else
    {   CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

BattleScene::BattleScene()
{
    
}

BattleScene::~BattleScene()
{
    
}

bool BattleScene::init()
{
    bool ret = BaseScene::init();
    
    this->addLayerWithTag((int)TagType::REAR);
    this->addLayerWithTag((int)TagType::MID);
    this->addLayerWithTag((int)TagType::AHEAD);
    this->addLayerWithTag((int)TagType::JOYPAD);
    this->addLayerWithTag((int)TagType::UI);
    this->addLayerWithTag((int)TagType::TIP);
    this->addLayerWithTag((int)TagType::LOADING);
    
    return ret;
}

