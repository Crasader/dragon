//
//  LoginControl.h
//  dragon
//
//  Created by xingchong on 1/8/16.
//
//

#ifndef __dragon__LoginControl__
#define __dragon__LoginControl__

#include <stdio.h>
#include "BaseControl.h"

class LoginControl : public BaseControl
{
public:
    static LoginControl *create();
    
    LoginControl();
    
    ~LoginControl();
    
    bool init();
    
    void createView();
    
    void request();
    
    void addEvent();
    
    void removeEvent();
    
};

#endif /* defined(__dragon__LoginControl__) */
