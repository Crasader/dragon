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
    
    this->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    this->setBackGroundColor(Color3B(255, 255, 255));
    this->setOpacity(255);
    this->setContentSize(CCDirector::getInstance()->getWinSize());
    
    return rs;
}

void LoginView::appendView()
{
    Size winSize = CCDirector::getInstance()->getWinSize();
    _sendButton = ui::Button::create("res/image/button/button02_green.png");
    _sendButton->setPosition(Vec2(winSize.width*3/4, winSize.height/2));
    this->addChild(_sendButton);
    
    _sendText = ui::TextField::create();
    _sendText->setPosition(Vec2(winSize.width/4, winSize.height/2));
    this->addChild(_sendText);
}

void LoginView::addClickListener()
{
    _sendButton->addClickEventListener([=](Ref *sender){this->onSendButtonClick((ui::Button*)sender);});
}


void LoginView::onSendButtonClick(ui::Button *sender)
{
    CCLOG("onSendButtonClick()................");
}
