//
//  BaseView.cpp
//  dragon
//
//  Created by xingchong on 12/11/15.
//
//

#include "BaseView.h"

BaseView::BaseView():
_needBg(true)
{
    
}

BaseView::~BaseView()
{
    
}


bool BaseView::init()
{
    bool ret = ui::Layout::init();
    //append my view
    this->appendView();
    
    this->addClickListener();
    
    this->addTouchedBg();

    return ret;
}

void BaseView::addTouchedBg()
{
    if (_needBg)
    {
        _bg = ui::Layout::create();
        _bg->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
        _bg->setBackGroundColor(Color3B(0, 0, 0));
        _bg->setOpacity(125);
        _bg->setContentSize(CCDirector::getInstance()->getWinSize());
        _bg->setLocalZOrder(-1);
        Vec2 pos = this->convertToWorldSpace(Vec2(0, 0));
        _bg->setPosition(pos);
        this->addChild(_bg);
    }
}

void BaseView::setNeedBg(bool pNeedBg)
{
    this->_needBg = pNeedBg;
}


