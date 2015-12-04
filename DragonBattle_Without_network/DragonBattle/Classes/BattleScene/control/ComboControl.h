//
//  ComboControl.h
//  DragonBattle
//
//  Created by 永翔 何 on 13-5-23.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_ComboControl_h
#define DragonBattle_ComboControl_h

#include "cocos2d.h"

class ComboControl : public cocos2d::CCLayer
{
public:
    ComboControl();
    ~ComboControl();

    static ComboControl* create();
    virtual bool init();

    void setCombo(int nCombo);
    
private:
    void hide(float dt);
    
    int m_nCombo;
    cocos2d::CCLabelAtlas*  m_pCombo;
    cocos2d::CCSprite*      m_pSpriteFrame;
};

#endif
