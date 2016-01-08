//
//  ControlFactory.cpp
//  dragon
//
//  Created by xingchong on 12/25/15.
//
//

#include "ControlFactory.h"
#include "LoginControl.h"
#include "LoadingBattleControl.h"

static ControlFactory *_controlFactory = nullptr;

ControlFactory *ControlFactory::getInstance()
{
    if (!_controlFactory)
    {
        _controlFactory = new ControlFactory();
        _controlFactory->init();
    }
    return _controlFactory;
}


ControlFactory::ControlFactory()
{
}

ControlFactory::~ControlFactory()
{
}

void ControlFactory::init()
{
    _createrMap.insert(std::make_pair(ControlType::LOGIN,         [=](){return LoginControl::create();}          ));
    _createrMap.insert(std::make_pair(ControlType::LOADINGBATTLE, [=](){return LoadingBattleControl::create();}  ));
}


void ControlFactory::createControl(ControlType controlType)
{
    auto suitC = _controlMap.find(controlType);
    if (suitC != _controlMap.end())
    {
        CCLOG("The control has been created, controlName=%d", controlType);
        return;
    }
    auto suit = _createrMap.find(controlType); //find the create method
    if (suit != _createrMap.end())
    {
        BaseControl *baseControl = (suit->second)();
        _controlMap.insert(std::make_pair(controlType, baseControl));
    }
}


void ControlFactory::destroyControl(ControlType controlType)
{
    auto suitC = _controlMap.find(controlType);
    if (suitC != _controlMap.end()) //find it
    {
        BaseControl *baseControl = suitC->second;
        baseControl->destroy();
        _controlMap.erase(controlType);
    }
    else
    {
        CCLOG("The control has been destroied, controlName=%d", controlType);
    }
}

void ControlFactory::destroyAllControl()
{
    for (auto it=_controlMap.begin(); it!=_controlMap.end(); ++it)
    {
        it->second->destroy();
    }
    _controlMap.clear();
}






