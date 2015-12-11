//
//  BaseView.h
//  dragon
//
//  Created by xingchong on 12/11/15.
//
//

#ifndef __dragon__BaseView__
#define __dragon__BaseView__

#include <stdio.h>
#include "cocos2d.h"
#include "ui/UILayout.h"
#include "ui/UIListView.h"

USING_NS_CC;

class BaseView : public ui::Layout
{
public:
    BaseView();
    
    ~BaseView();
    
    bool init() override;
    
protected:
    virtual void addEventListener() = 0;
    
};

#endif /* defined(__dragon__BaseView__) */
