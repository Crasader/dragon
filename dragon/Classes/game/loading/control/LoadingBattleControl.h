//
//  LoadingBattleControl.h
//  dragon
//
//  Created by xingchong on 1/8/16.
//
//

#ifndef __dragon__LoadingBattleControl__
#define __dragon__LoadingBattleControl__

#include <stdio.h>
#include "BaseControl.h"

class LoadingBattleControl : public BaseControl
{
public:
    static LoadingBattleControl *create();
    
    LoadingBattleControl();
    
    ~LoadingBattleControl();
    
    bool init();
    
    void createView();
    
    void request();
    
    void addEvent();
    
    void removeEvent();
};

#endif /* defined(__dragon__LoadingBattleControl__) */
