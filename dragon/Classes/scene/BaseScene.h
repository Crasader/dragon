//
//  BaseScene.h
//  dragon
//
//  Created by xingchong on 12/11/15.
//
//

#ifndef __dragon__BaseScene__
#define __dragon__BaseScene__

#include <stdio.h>
#include "cocos2d.h"


class BaseScene : public cocos2d::Scene
{
public:
    BaseScene();
    
    ~BaseScene();
    
    bool init() override;
    
    virtual void showMe();
    
protected:
    virtual void addLayerWithTag(int tag);

};

#endif /* defined(__dragon__BaseScene__) */
