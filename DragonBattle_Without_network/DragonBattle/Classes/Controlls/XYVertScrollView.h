//
//  XYVertScrollView.h
//  DragonBattle
//
//  Created by zeng xiaolong on 13-7-2.
//
//

#ifndef __DragonBattle__XYVertScrollView__
#define __DragonBattle__XYVertScrollView__
#include "XYScrollView.h"
#include "XYListScrollView.h"
class XYVertScrollView:public XYScrollView
{
public:
    XYVertScrollView();
    ~XYVertScrollView();
    
    static XYVertScrollView *create(int Priority,CCSize size, CCNode* container = NULL);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    bool m_bmove;
    bool m_firstmove;
private:
    CCPoint m_startPoi;
};

class XYHorizeListScrollView:public XYListScrollView
{
public:
    XYHorizeListScrollView();
    ~XYHorizeListScrollView();
    static XYHorizeListScrollView *create(int nindex,int pagecount,float width,int Priority,CCSize size,CCNode* container = NULL);
    bool m_bmove;
    bool m_firstmove;
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
private:
    CCPoint m_startPoi;
};
#endif /* defined(__DragonBattle__XYVertScrollView__) */
