//
//  LoadingScene.cpp
//  dragon
//
//  Created by xingchong on 12/8/15.
//
//

#include "LoadingScene.h"
#include "ui/UIWidget.h"

USING_NS_CC;

LoadingScene *LoadingScene::create()
{
    LoadingScene *ret = new LoadingScene();
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

bool LoadingScene::init()
{   bool ret = BaseScene::init();
    
    this->addWidgetOnScene((int)TagType::BG);
    this->addWidgetOnScene((int)TagType::UI);
    this->addWidgetOnScene((int)TagType::TIP);
    this->addWidgetOnScene((int)TagType::LOADING);
    
    return ret;
}


