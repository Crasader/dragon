//
//  LoginControl.cpp
//  dragon
//
//  Created by xingchong on 1/8/16.
//
//

#include "LoginControl.h"
#include "LoginScene.h"
#include "LoginView.h"

LoginControl *LoginControl::create()
{
    LoginControl *ret = new LoginControl();
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

LoginControl::LoginControl()
{
    
}

LoginControl::~LoginControl()
{
    
}

bool LoginControl::init()
{
    int tag = (int)LoginScene::TagType::UI;
    
    bool rs = BaseControl::init(tag);
    
    return rs;
}


void LoginControl::createView()
{
    _baseView = LoginView::create();
}

void LoginControl::request()
{
    
}

void LoginControl::addEvent()
{
    
}

void LoginControl::removeEvent()
{
    
}


