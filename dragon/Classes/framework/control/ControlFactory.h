//
//  ControlFactory.h
//  dragon
//
//  Created by xingchong on 12/25/15.
//
//

#ifndef __dragon__ControlFactory__
#define __dragon__ControlFactory__

#include <stdio.h>
#include "cocos2d.h"
#include "BaseControl.h"

USING_NS_CC;


//typedef std::function<void(Ref*,Widget::TouchEventType)> ccWidgetTouchCallback;
//
//typedef void (Ref::*SEL_TouchEvent)(Ref*,TouchEventType);
//#define toucheventselector(_SELECTOR) (SEL_TouchEvent)(&_SELECTOR)
//
typedef std::function<BaseControl* ()> controlCreateMethod;


class ControlFactory : public Ref
{
public:
    enum class ControlType
    {
      LOGIN,
      LOADINGBATTLE
    };
    
public:
    static ControlFactory *getInstance();
    
    ControlFactory();
    
    ~ControlFactory();
    
    void init();
    
    void createControl(ControlType controlType);
    
    void destroyControl(ControlType controlType);
    
    void destroyAllControl();
    
private:
    std::map<ControlType, BaseControl*>        _controlMap;
    std::map<ControlType, controlCreateMethod> _createrMap;
    
};

#endif /* defined(__dragon__ControlFactory__) */
