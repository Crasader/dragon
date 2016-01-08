//
//  LoginView.h
//  dragon
//
//  Created by xingchong on 1/8/16.
//
//

#ifndef __dragon__LoginView__
#define __dragon__LoginView__

#include <stdio.h>
#include "cocos2d.h"
#include "BaseView.h"

class LoginView : public BaseView
{
public:
    static LoginView *create();
    
    LoginView();
    
    ~LoginView();
    
    bool init() override;
    
private:
    virtual void appendView();
    
    virtual void addEventListener();
    
};

#endif /* defined(__dragon__LoginView__) */
