//
//  LoadingBattleView.h
//  dragon
//
//  Created by xingchong on 12/11/15.
//
//

#ifndef __dragon__LoadingBattleView__
#define __dragon__LoadingBattleView__

#include <stdio.h>
#include "cocos2d.h"
#include "BaseView.h"

class LoadingBattleView : public BaseView
{
public:
    static LoadingBattleView *create();
    
    LoadingBattleView();
    
    ~LoadingBattleView();
    
    bool init() override;
    
private:
    virtual void appendView();
    
    virtual void addClickListener();
};

#endif /* defined(__dragon__LoadingBattleView__) */
