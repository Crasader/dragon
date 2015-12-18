//
//  BaseScene.cpp
//  dragon
//
//  Created by xingchong on 12/11/15.
//
//

#include "BaseScene.h"
#include "ui/UIWidget.h"

USING_NS_CC;

BaseScene::BaseScene()
{
    
}

BaseScene::~BaseScene()
{
    
}

bool BaseScene::init()
{
    return Scene::init();
}


void BaseScene::addLayerWithTag(int tag)
{
    ui::Widget *widget = ui::Widget::create();
    widget->setZOrder(tag);
    widget->setTag(tag);
    this->addChild(widget);
}

void  BaseScene::showMe()
{
    Director * director = Director::getInstance();
    Scene *scene =  director->getRunningScene();
    if (scene)
    {
        director->replaceScene(this);
    }
    else
    {
        director->runWithScene(this);
    }
}

