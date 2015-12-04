//
//  ShowMemoryLayer.h
//  DragonBattle
//
//  Created by 小敦 王 on 13-5-3.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_ShowMemoryLayer_h
#define DragonBattle_ShowMemoryLayer_h

#include "cocos2d.h"

class ShowMemoryLayer : public cocos2d::CCLayer
{
public:
    virtual bool init();
    
    CREATE_FUNC(ShowMemoryLayer);
    
    virtual void update(float dt);
    
    void showUserdMemory();
    
private:
    cocos2d::CCLabelTTF* m_pMemoryUsed;
};

#endif
