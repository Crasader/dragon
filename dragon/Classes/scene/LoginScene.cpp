//
//  LoginScene.cpp
//  dragon
//
//  Created by xingchong on 1/8/16.
//
//

#include "LoginScene.h"
#include "ui/UIWidget.h"
#include "ControlFactory.h"

USING_NS_CC;


LoginScene *LoginScene::create()
{
    LoginScene *ret = new LoginScene();
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

LoginScene::LoginScene()
{
    
}

LoginScene::~LoginScene()
{
    
}


bool LoginScene::init()
{
    bool rs = BaseScene::init();
    
    this->addLayerWithTag((int)TagType::BG);
    this->addLayerWithTag((int)TagType::UI);
    this->addLayerWithTag((int)TagType::TIP);
    this->addLayerWithTag((int)TagType::LOADING);
    
    return rs;
}

void LoginScene::onEnter()
{
    BaseScene::onEnter();
    
    ControlFactory::getInstance()->createControl(ControlFactory::ControlType::LOGIN);
}


