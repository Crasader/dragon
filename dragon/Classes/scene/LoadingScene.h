//
//  LoadingScene.h
//  dragon
//
//  Created by xingchong on 12/8/15.
//
//

#ifndef __dragon__LoadingScene__
#define __dragon__LoadingScene__

#include <stdio.h>
#include "cocos2d.h"
#include "BaseScene.h"

class LoadingScene : public BaseScene
{
public:
    enum class TagType
    {
        BG=401,
        UI,
        TIP,
        LOADING
    };
    
public:
    static LoadingScene *create();
    
    LoadingScene();
    
    ~LoadingScene();
    
    bool init() override;

    
    
};

#endif /* defined(__dragon__LoadingScene__) */
