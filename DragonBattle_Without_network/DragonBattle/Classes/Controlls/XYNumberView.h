//
//  XYNumberView.h
//  DragonBattle
//
//  Created by zeng xiaolong on 13-12-30.
//
//

#ifndef __DragonBattle__XYNumberView__
#define __DragonBattle__XYNumberView__
#include "cocos2d.h"
USING_NS_CC;
#include "ViewDefine.h"

class XYNumberViewDelegate
{
public:
    virtual ~XYNumberViewDelegate(){}
    virtual void numberChange(int index){}
};

class XYContainLayer;
class XYListScrollView;
class XYNumberView:public CCLayer,public XYScrollDelegate
{
public:
    XYNumberView();
    ~XYNumberView();
    static XYNumberView*create(int number,int prority);
    virtual bool init(int number,int prority);
    void setTimes(int times);
    int getTimes();
private:
    CC_SYNTHESIZE(XYNumberViewDelegate*, m_pDelegate,Delegate);
    void scrollViewMoveChange(int index);
    int m_index;
    void updateScale(float dt);
    XYListScrollView * m_pScrollView;
    XYContainLayer* m_pContainer;
};
#endif /* defined(__DragonBattle__XYNumberView__) */
