//
//  LoadingBattleView.cpp
//  dragon
//
//  Created by xingchong on 12/11/15.
//
//

#include "LoadingBattleView.h"


LoadingBattleView *LoadingBattleView::create()
{
    LoadingBattleView *ret = new LoadingBattleView();
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

bool LoadingBattleView::init()
{
    bool ret = BaseView::init();
    
    //append my view
    
    this->addEventListener();
    
    return ret;
}


void LoadingBattleView::addEventListener()
{
    
}
