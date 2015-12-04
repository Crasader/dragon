//
//  XYMenuItemLabel.h
//  DragonBattle
//
//  Created by 何 永翔 on 13-11-20.
//
//

#ifndef __DragonBattle__XYMenuItemLabel__
#define __DragonBattle__XYMenuItemLabel__

#include "cocos2d.h"

class XYMenuItemLabel : public cocos2d::CCMenuItemLabel
{
public:
    XYMenuItemLabel();
    
    static XYMenuItemLabel* create(CCNode* label, CCObject* target, cocos2d::SEL_MenuHandler selector);
    
    void setLine(bool hasline);
    void draw();
private:
    bool m_hasLine;
};

#endif /* defined(__DragonBattle__XYMenuItemLabel__) */
