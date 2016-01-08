//
//  LoginView.cpp
//  dragon
//
//  Created by xingchong on 1/8/16.
//
//

#include "LoginView.h"

LoginView *LoginView::create()
{
    LoginView *ret = new LoginView();
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

LoginView::LoginView()
{
    
}

LoginView::~LoginView()
{
    
}

bool LoginView::init()
{
    bool rs = BaseView::init();
    
    return rs;
}

void LoginView::appendView()
{
    
}

void LoginView::addEventListener()
{
    
}

