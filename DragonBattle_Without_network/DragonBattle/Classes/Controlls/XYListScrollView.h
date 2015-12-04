//
//  XYListScrollView.h
//  XY
//
//  Created by long on 12-12-27.
//
//

#ifndef __XY__XYListScrollView__
#define __XY__XYListScrollView__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "XYScrollView.h"
USING_NS_CC;
USING_NS_CC_EXT;

class XYListScrollViewPage;
class XYScrollDelegate
{
public:
    virtual ~XYScrollDelegate(){}
    virtual void scrollViewIndex(int nindex){}
    virtual void scrollViewMoveChange(int nindex){}
    virtual void scrollToEnd(){}
};

class XYListScrollView:public XYScrollView
{
public:
    XYListScrollView();
    ~XYListScrollView();
    static XYListScrollView *create(int nindex,int pagecount,float width,int Priority,CCSize size,CCNode* container = NULL);
    void relocateContainer(bool animated);
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    virtual void registerWithTouchDispatcher();
    
    void deaccelerateScrolling(float dt);
    void adjust(void);
    CCPoint getContainLayerPoi();
    void setListIndex(int nindex,bool animated = true);
    void setPageNum(int num);
    int getCurIndex();
    /* nodePt 为node在XYListScrollView的parent上面的点 */
    void setScrollPage(CCPoint nodePt, XYListScrollViewPage* pNode = NULL);
    XYListScrollViewPage* getScrollPage() { return m_pPageNode;}
    float m_width;
    int   m_handlerPriority;
protected:
    CC_SYNTHESIZE(XYScrollDelegate*, m_pXYScrollDelegate, ScrollDelegate);
    CC_SYNTHESIZE(bool, m_bInertia, Inertia);//触摸结束滑动惯性
    CC_SYNTHESIZE(int, m_index, Index);
    CC_SYNTHESIZE(int, m_pagecount, Pagecount);
    XYListScrollViewPage* m_pPageNode;//不能加到ScrollView这层
};


/* ListScrollView的页数显示 页数的点是从node的(0,0)点开始左右放置
 */
class XYListScrollViewPage : public CCNode
{
public:
    XYListScrollViewPage();
    ~XYListScrollViewPage();
    
    static XYListScrollViewPage* create(int pageNum);
    bool init(int pageNum);
    
    void   setPageNum(int pageNum);
    void   setCurPageIndex(int pageIndex);
    
protected:
    virtual void setPagePoint();
    
private:
    int             m_nPageNum;
    int             m_nCurPageIndex;

    CCPoint         m_distancePt;
    CCSprite*       m_pGreenPoint;
    CCArray*        m_pPagePointArray;
};

#endif /* defined(__XY__XYListScrollView__) */
