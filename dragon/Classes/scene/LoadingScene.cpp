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

LoadingScene::LoadingScene()
{
    
}

LoadingScene::~LoadingScene()
{
    
}

bool LoadingScene::init()
{   bool ret = BaseScene::init();
    
    this->addLayerWithTag((int)TagType::BG);
    this->addLayerWithTag((int)TagType::UI);
    this->addLayerWithTag((int)TagType::TIP);
    this->addLayerWithTag((int)TagType::LOADING);
    
    return ret;
}


