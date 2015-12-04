
#ifndef lmsg_Button_h
#define lmsg_Button_h

#include "cocos2d.h"
#include "XYBaseButton.h"
USING_NS_CC;
class XYButton : public XYBaseButton
{
    public:
        XYButton(void);
        ~XYButton(void);
        virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
        virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
        virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
        
        void touchBegin();
        static XYButton * create(const char * pFile,int nPriority = 0);
        static XYButton * createWithFileAndPriority(const char * pNormalFile, const char *pSelectFile,const char *pDisableFile,int nPriority);
};
#endif
