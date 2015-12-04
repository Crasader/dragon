//
//  XYToggleButton.h
//  ChickRun
//
//  Created by long on 12-11-16.
//
//

#ifndef ChickRun_XYToggleButton_h
#define ChickRun_XYToggleButton_h
#include "XYBaseButton.h"
    class XYToggleButton:public XYBaseButton
    {
    public:
        XYToggleButton();
        ~XYToggleButton();
        static XYToggleButton * create(const char *pNormalFile,const char *pSelectFile, int nPriority);
        CC_SYNTHESIZE_READONLY(bool, m_isTurnOn,TurnOn);
        void setselect(bool select);
        CCPoint m_poi;
    private:
        virtual void touchEnd();
        virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* event);
        
    };
#endif
