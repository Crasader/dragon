//
//  XYGroup.h
//  XY
//
//  Created by long on 12-12-24.
//
//

#ifndef __XY__XYGroup__
#define __XY__XYGroup__
#include "XYSelectButton.h"
class XYGroup:public CCObject
    {
    public:
        XYGroup(void);
        ~XYGroup(void);
        CCArray *arrGroupBtn;
        
        XYSelectButton *getSelectBtn();
        void addBtn(XYSelectButton *btn);
        void removeBtn(XYSelectButton *btn);
        void setSelectButton(XYSelectButton *selebtn);
        void setSelectButtonByIndex(int index);
        void btnTouch(CCNode* node);
        static XYGroup* create();
        void setTouchEndEvent(CCObject *obj,SEL_CallFuncN callfun){m_pTarget = obj;m_pTouchEnded = callfun;}
    private:
        CCObject* m_pTarget;
        cocos2d::SEL_CallFuncN  m_pTouchEnded;
    };

#endif /* defined(__XY__XYGroup__) */
