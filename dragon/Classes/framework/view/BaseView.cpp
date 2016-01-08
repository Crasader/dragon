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
    
    this->addEventListener();
    
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
        _bg->setBackGroundColorOpacity(125);
        _bg->setContentSize(Size(1140, 640));
        _bg->setTouchEnabled(true);
        Vec2 pos = this->convertToNodeSpace(Vec2(0,0));
        _bg->setPosition(pos);
        this->addChild(_bg);
        this->sortAllChildren();
    }
}

void BaseView::setNeedBg(bool pNeedBg)
{
    this->_needBg = pNeedBg;
}


