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
    virtual void appendView() = 0;
    
    virtual void addEventListener() = 0;

    void setNeedBg(bool pNeedBg); //use before BaseView::init()
    
private:
    void addTouchedBg();
    
private:
    bool _needBg; //the view need touchable bg or not
    
    ui::Layout *_bg;//the bg
    
};

#endif /* defined(__dragon__BaseView__) */
