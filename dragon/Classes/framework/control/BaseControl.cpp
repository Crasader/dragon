//
//  BaseControl.cpp
//  dragon
//
//  Created by xingchong on 12/18/15.
//
//

#include "BaseControl.h"
#include "ui/UIWidget.h"

BaseControl::BaseControl()
{
    
}

BaseControl::~BaseControl()
{
    
}

bool BaseControl::init()
{
    this->addEvent();
    
    return true;
}

void BaseControl::destroy()
{
    this->removeEvent();
    
    this->_baseView->removeFromParent();
}

void BaseControl::addViewOnLayer(int tag)
{
    Scene *runningScene = CCDirector::getInstance()->getRunningScene();
    ui::Widget *target  = (ui::Widget*)runningScene->getChildByTag(tag);
    
    Size winSize = CCDirector::getInstance()->getWinSize();
    Size size = _baseView->getContentSize();
    Vec2 pos = Vec2((winSize.width-size.width)/2, (winSize.height-size.height)/2);
    
    _baseView->setPosition(pos);
    target->addChild(_baseView);
}



