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

//void  BaseScene::showMe()
//{
//    Scene *runningScene = CCDirector::getInstance()->getRunningScene();
//    if (runningScene)
//    {
//        CCDirector::getInstance()->replaceScene(this);
//    }
//    else
//    {
//        CCDirector::getInstance()->runWithScene(this);
//    }
//    Scene *runningScene2 = CCDirector::getInstance()->getRunningScene();
//    CCLOG("");
//}

