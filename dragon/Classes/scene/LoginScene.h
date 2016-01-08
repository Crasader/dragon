//
//  LoginScene.h
//  dragon
//
//  Created by xingchong on 1/8/16.
//
//

#ifndef __dragon__LoginScene__
#define __dragon__LoginScene__

#include <stdio.h>
#include "cocos2d.h"
#include "BaseScene.h"

class LoginScene : public BaseScene
{
    
public:
    enum class TagType
    {
        BG=101,
        UI,
        TIP,
        LOADING
    };
    
public:
    static LoginScene *create();
    
    LoginScene();
    
    ~LoginScene();
    
    bool init() override;
    
    void onEnter() override;
    
};

#endif /* defined(__dragon__LoginScene__) */
