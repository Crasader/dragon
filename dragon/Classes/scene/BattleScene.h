//
//  BattleScene.h
//  dragon
//
//  Created by xingchong on 12/11/15.
//
//

#ifndef __dragon__BattleScene__
#define __dragon__BattleScene__

#include <stdio.h>
#include "cocos2d.h"
#include "BaseScene.h"

class BattleScene : public BaseScene
{

public:
    enum class TagType
    {
        REAR=301,
        MID,
        AHEAD,
        JOYPAD,
        UI,
        TIP,
        LOADING
    };
    
public:
    static BattleScene *create();
    
    BattleScene();
    
    ~BattleScene();
    
    bool init() override;
};



#endif /* defined(__dragon__BattleScene__) */
