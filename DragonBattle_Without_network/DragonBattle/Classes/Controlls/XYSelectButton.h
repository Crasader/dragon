//
//  XYSelectButton.h
//  XY
//
//  Created by long on 12-12-21.
//
//

#ifndef __XY__XYSelectButton__
#define __XY__XYSelectButton__
#include "cocos2d.h"
#include "XYBaseButton.h"
USING_NS_CC;
class XYSelectButton:public XYBaseButton
{
public:
    XYSelectButton();
    ~XYSelectButton();
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void touchEnd();
    static XYSelectButton* create(const char *pNormalFile, const char *pSelectFile, int nPriority,bool isChangePic = true);
    static XYSelectButton* create(const char *pNormalFile, const char *pSelectFile,const char *pDisableFile, int nPriority,bool isChangePic = true);
    bool initWithFile(const char * pNormalFile,const char *pSelectFile,const char *pDisableFile, int nPriority,bool isChangePic);
    CC_SYNTHESIZE(bool, m_bAnimate, Animate);
    void selet();
    void unselet();
    CC_SYNTHESIZE_READONLY(bool, m_isTurnOn,TurnOn);
    bool m_bIsChangePic;
    CCPoint m_poi;
};

class XYSpecialSelectButton:public XYSelectButton
{
public:
    XYSpecialSelectButton();
    ~XYSpecialSelectButton();
    static XYSpecialSelectButton* create(const char *pNormalFile, const char *pSelectFile, int nPriority,bool isChangePic = true);
    virtual void touchEnd();
};
#endif
