//
//  XYDownButton.h
//  DragonBattle
//
//  Created by zeng xiaolong on 13-7-22.
//
//

#ifndef __DragonBattle__XYDownButton__
#define __DragonBattle__XYDownButton__
#include "XYBaseButton.h"

class XYDownButton:public XYBaseButton
{
public:
    XYDownButton();
    ~XYDownButton();
    static XYDownButton * create(const char *pNormalFile,const char *pSelectFile, int nPriority);
private:
    virtual void touchBegin();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    virtual void touchDown(float ftime);
    float m_duration;
};

#endif /* defined(__DragonBattle__XYDownButton__) */
