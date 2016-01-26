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
#include "ui/UIButton.h"
#include "ui/UITextField.h"

class LoginView : public BaseView
{
public:
    static LoginView *create();
    
    LoginView();
    
    ~LoginView();
    
    bool init() override;
    
private:
    virtual void appendView();
    
    virtual void addClickListener();
    
private:
    void onConnectButtonClick(ui::Button *sender);
    void onSendButtonClick(ui::Button *sender);

private:
    ui::Button      *_connectButton;
    ui::Button      *_sendButton;
    ui::TextField   *_sendText;
    
};

#endif /* defined(__dragon__LoginView__) */
