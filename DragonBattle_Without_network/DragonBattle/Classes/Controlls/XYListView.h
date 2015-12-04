//
//  XYListView.h
//  XY
//
//  Created by long on 13-4-3.
//
//

#ifndef __XY__XYListView__
#define __XY__XYListView__
#include "XYScrollView.h"
#include "XYContainLayer.h"
class XYListView:public CCLayerColor
{
public:
    XYListView();
    ~XYListView();
    static XYListView* createWith(int Priority,CCSize size,CCArray *viewarr = NULL);
    void resetarr(CCArray *arr, bool bIsVertical = true, bool bIsRefresh = true, int nScrollIndex = -1);
    void resetListView(bool bIsVertical = true, bool bIsRefresh = true);
    CCArray *m_viewarr;
    XYScrollView *m_scrollview;
    XYContainLayer *m_containlayer;
    CC_SYNTHESIZE(float, m_distence, Distence);
    CC_SYNTHESIZE(float, m_topDistence, topDistence);
    CC_SYNTHESIZE(float, m_leftDistence, leftDistence);
private:
    bool initWith(int Priority,CCSize size,CCArray *viewarr = NULL);
    void createScroll(bool bIsVertical, int nScrollIndex);
    bool isInViewArr(CCNode* pNode);
    CCSize m_size;
    int m_Priority;
    void updateVisable(float time);
};

#endif /* defined(__XY__XYListView__) */
