//
//  LoadingBattleControl.cpp
//  dragon
//
//  Created by xingchong on 1/8/16.
//
//

#include "LoadingBattleControl.h"
#include "LoadingScene.h"
#include "LoadingBattleView.h"

LoadingBattleControl *LoadingBattleControl::create()
{
    LoadingBattleControl *ret = new LoadingBattleControl();
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

LoadingBattleControl::LoadingBattleControl()
{
    
}

LoadingBattleControl::~LoadingBattleControl()
{
    
}

bool LoadingBattleControl::init()
{
    int tag = (int)LoadingScene::TagType::UI;
    
    bool rs = BaseControl::init(tag);
    
    return rs;
}


void LoadingBattleControl::createView()
{
    _baseView = LoadingBattleView::create();
}

void LoadingBattleControl::request()
{
    
}

void LoadingBattleControl::addEvent()
{
    
}

void LoadingBattleControl::removeEvent()
{
    
}
