//
//  BaseControl.h
//  dragon
//
//  Created by xingchong on 12/18/15.
//
//

#ifndef __dragon__BaseControl__
#define __dragon__BaseControl__

#include <stdio.h>
#include "cocos2d.h"
#include "BaseView.h"

USING_NS_CC;

class BaseControl : public Ref
{
public:
    BaseControl();
    
    virtual ~BaseControl();
    
    bool init(int tag);
    
    void destroy();

private:
    //add view on target layer by tag
    void addViewOnLayer(int tag);
    
protected:
    
    virtual void createView() = 0;
    
    virtual void request() = 0;
    
    virtual void addEvent() = 0;
    
    virtual void removeEvent() = 0;
    
protected:
    BaseView *_baseView;
    
};

#endif /* defined(__dragon__BaseControl__) */
