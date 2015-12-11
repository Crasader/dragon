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


bool BattleScene::init()
{
    bool ret = BaseScene::init();
    
    this->addWidgetOnScene((int)TagType::REAR);
    this->addWidgetOnScene((int)TagType::MID);
    this->addWidgetOnScene((int)TagType::AHEAD);
    this->addWidgetOnScene((int)TagType::JOYPAD);
    this->addWidgetOnScene((int)TagType::UI);
    this->addWidgetOnScene((int)TagType::TIP);
    this->addWidgetOnScene((int)TagType::LOADING);
    
    return ret;
}

