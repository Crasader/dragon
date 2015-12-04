//
//  XYListScrollView.cpp
//  XY
//
//  Created by long on 12-12-27.
//
//

#include "XYListScrollView.h"
#define SCROLL_DEACCEL_RATE  0.95f
#define SCROLL_DEACCEL_DIST  1.0f
XYListScrollView::XYListScrollView()
{
    m_pXYScrollDelegate = NULL;
    m_index = 0;
    m_pPageNode = NULL;
    m_bInertia = false;
}

XYListScrollView::~XYListScrollView()
{}

XYListScrollView *XYListScrollView::create(int nindex,int pagecount,float width,int Priority,CCSize size, CCNode* container)
{
    XYListScrollView* pRet = new XYListScrollView();
    if (pRet && pRet->initWithViewSize(size, container))
    {
        pRet->m_width = width;
        pRet->m_handlerPriority = Priority;
        pRet->m_pagecount = pagecount;
        pRet->setListIndex(nindex);
        pRet->setDirection(kCCScrollViewDirectionHorizontal);
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

void XYListScrollView::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, m_handlerPriority, false);
}

bool XYListScrollView::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    if (XYScrollView::ccTouchBegan(pTouch, pEvent))
    {
        return true;
    }
    return false;
}

void XYListScrollView::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    CCScrollView::ccTouchMoved(touch, event);
    if (m_pXYScrollDelegate)
    {
        m_pXYScrollDelegate->scrollViewMoveChange(getCurIndex());
    }
}

void XYListScrollView::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    if (!this->isVisible())
    {
        return;
    }
    if (m_pTouches->containsObject(touch))
    {
        if (m_pTouches->count() == 1)
        {
            if (getInertia())
            {
                this->schedule(schedule_selector(XYListScrollView::deaccelerateScrolling));
            }
            else
            {
                this->relocateContainer(true);
            }
        }
        m_pTouches->removeObject(touch);
    }
    
    if (m_pTouches->count() == 0)
    {
        m_bDragging = false;
        m_bTouchMoved = false;
    }
}

void XYListScrollView::deaccelerateScrolling(float dt)
{
    if (m_bDragging)
    {
        this->unschedule(schedule_selector(XYListScrollView::deaccelerateScrolling));
        return;
    }
    
    float newX, newY;
    CCPoint maxInset, minInset;
    
    m_pContainer->setPosition(ccpAdd(m_pContainer->getPosition(), m_tScrollDistance));
    
    if (m_bBounceable)
    {
        maxInset = m_fMaxInset;
        minInset = m_fMinInset;
    }
    else
    {
        maxInset = this->maxContainerOffset();
        minInset = this->minContainerOffset();
    }
    
    //check to see if offset lies within the inset bounds
    newX     = MIN(m_pContainer->getPosition().x, maxInset.x);
    newX     = MAX(newX, minInset.x);
    newY     = MIN(m_pContainer->getPosition().y, maxInset.y);
    newY     = MAX(newY, minInset.y);
    
    newX = m_pContainer->getPosition().x;
    newY = m_pContainer->getPosition().y;
    
    m_tScrollDistance     = ccpSub(m_tScrollDistance, ccp(newX - m_pContainer->getPosition().x, newY - m_pContainer->getPosition().y));
    m_tScrollDistance     = ccpMult(m_tScrollDistance, SCROLL_DEACCEL_RATE);
    this->setContentOffset(ccp(newX,newY));
    
    if ((fabsf(m_tScrollDistance.x) <= SCROLL_DEACCEL_DIST &&
         fabsf(m_tScrollDistance.y) <= SCROLL_DEACCEL_DIST) ||
        newY > maxInset.y || newY < minInset.y ||
        newX > maxInset.x || newX < minInset.x ||
        newX == maxInset.x || newX == minInset.x ||
        newY == maxInset.y || newY == minInset.y)
    {
        this->unschedule(schedule_selector(XYListScrollView::deaccelerateScrolling));
        this->relocateContainer(true);
    }
    else
    {
        if (m_pXYScrollDelegate)
        {
            m_pXYScrollDelegate->scrollViewMoveChange(getCurIndex());
        }
    }
}

int XYListScrollView::getCurIndex()
{
    float x = -getContentOffset().x + m_width/2;
    int newindex = x/m_width;
    if (newindex <0)
    {
        newindex = 0;
    }
    if (newindex >= m_pagecount)
    {
        newindex = m_pagecount - 1;
    }
    return newindex;
}

void XYListScrollView::relocateContainer(bool animated)
{
    adjust();
}

void XYListScrollView::setListIndex(int nindex,bool animated)
{
    m_index = nindex;
    setContentOffset(ccp(-nindex*m_width, 0),animated);
    
    if (m_pPageNode)
    {
        m_pPageNode->setCurPageIndex(m_index);
    }
}

void XYListScrollView::adjust(void)
{
    int newindex = m_index;
    if (m_tScrollDistance.x<-10 && !getInertia())
    {
        if (m_index < m_pagecount - 1)
        {
            newindex = m_index + 1;
        }
        else
        {
            if(m_pXYScrollDelegate)
            {
                m_pXYScrollDelegate->scrollToEnd();
            }
        }
    }
    else if(m_tScrollDistance.x>10 && !getInertia())
    {
        if (m_index>0)
        {
            newindex = m_index - 1;
        }
        else
        {
            if(m_pXYScrollDelegate)
            {
                m_pXYScrollDelegate->scrollToEnd();
            }
        }
    }
    else if(!getInertia())
    {
        float x = -getContentOffset().x + m_width/2;
        int index = x/m_width;
        if (index <0)
        {
            index = 0;
        }
        if (index >= m_pagecount)
        {
            index = m_pagecount - 1;
            if(m_pXYScrollDelegate)
            {
                m_pXYScrollDelegate->scrollToEnd();
            }
        }
        newindex = index;
    }
    else
    {
        newindex = getCurIndex();
    }
    
    if (newindex != m_index && m_pXYScrollDelegate != NULL)
    {
        m_pXYScrollDelegate->scrollViewMoveChange(newindex);
        m_pXYScrollDelegate->scrollViewIndex(newindex);
    }
    setListIndex(newindex);
}

void XYListScrollView::setPageNum(int num)
{
    m_pagecount = num;
    if (m_pPageNode)
    {
        m_pPageNode->setPageNum(m_pagecount);
    }
}

CCPoint XYListScrollView::getContainLayerPoi()
{
    if (m_pContainer)
    {
        return m_pContainer->getPosition();
    }
    else
    {
        return ccp(0,0);
    }
}

void XYListScrollView::setScrollPage(CCPoint nodePt, XYListScrollViewPage* pNode)
{
    if (m_pPageNode)
    {
        m_pPageNode->removeFromParent();
        m_pPageNode = NULL;
    }
    
    if (pNode == NULL)
    {
        m_pPageNode = XYListScrollViewPage::create(m_pagecount);
    }
    else
    {
        m_pPageNode = pNode;
    }
    

    m_pPageNode->setCurPageIndex(m_index);
    this->getParent()->addChild(m_pPageNode);
    m_pPageNode->setPosition(nodePt);
}

////////////////////////////////////////////////////////////////////////////////////////////////
XYListScrollViewPage::XYListScrollViewPage()
{
    m_pGreenPoint = NULL;
    m_pPagePointArray = CCArray::create();
    m_nPageNum = 0;
    m_nCurPageIndex = -1;
    
    m_distancePt = ccp(36, 0);    //两个点的距离默认是X方向36
    
    CC_SAFE_RETAIN(m_pPagePointArray);
}

XYListScrollViewPage::~XYListScrollViewPage()
{
    CC_SAFE_RELEASE_NULL(m_pPagePointArray);
}

XYListScrollViewPage* XYListScrollViewPage::create(int pageNum)
{
    XYListScrollViewPage* pRet = new XYListScrollViewPage();
    if (pRet && pRet->init(pageNum))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool XYListScrollViewPage::init(int pageNum)
{
    if (!CCNode::init())
    {
        return false;
    }
    this->setAnchorPoint(ccp(0.5, 0.5));
    this->setPageNum(pageNum);
    
    
    return true;
}

void XYListScrollViewPage::setPageNum(int pageNum)
{
    m_nPageNum = pageNum;
    this->setPagePoint();
}

void XYListScrollViewPage::setPagePoint()
{
    int pageNumInArray = m_pPagePointArray->count();
    
    if (m_nPageNum > pageNumInArray)
    {
        for (int i = 0; i < m_nPageNum - pageNumInArray; i++)
        {
            CCSprite* pSprite = CCSprite::create("public/option_empty.png");
            m_pPagePointArray->insertObject(pSprite, m_pPagePointArray->count());
            this->addChild(pSprite);
        }
    }
    
    //create ... 
    CCPoint startPt(0, 0);
    startPt.x = -1 * ((m_nPageNum % 2 == 1) ? (m_nPageNum / 2 * m_distancePt.x) : ((m_nPageNum - 1.0f) / 2 * m_distancePt.x));
    startPt.y = -1 * ((m_nPageNum % 2 == 1) ? (m_nPageNum / 2 * m_distancePt.y) : ((m_nPageNum + 1)/ 2* m_distancePt.y));

    for (int i = 0; i < m_nPageNum; i++)
    {
        CCSprite* pSprite = (CCSprite*)m_pPagePointArray->objectAtIndex(i);
        pSprite->setPosition(ccp(startPt.x + i * m_distancePt.x, startPt.y + i * m_distancePt.y));
        pSprite->setVisible(true);
    }
    
    for (int i = m_nPageNum; i < pageNumInArray; i++)
    {
        CCSprite* pSprite = (CCSprite*)m_pPagePointArray->objectAtIndex(i);
        pSprite->setVisible(false);
    }
    
    if (!m_pGreenPoint)
    {
        m_pGreenPoint = CCSprite::create("public/option_green.png");
        this->addChild(m_pGreenPoint);
    }
    if (m_nCurPageIndex != -1 && m_nCurPageIndex < m_nPageNum)
    {
        m_pGreenPoint->setPosition(((CCSprite*)m_pPagePointArray->objectAtIndex(0))->getPosition());
    }
}

void XYListScrollViewPage::setCurPageIndex(int pageIndex)
{
    if (pageIndex >= m_nPageNum || pageIndex < 0)
    {
        return;
    }
    m_nCurPageIndex = pageIndex;
    m_pGreenPoint->setPosition(((CCSprite*)m_pPagePointArray->objectAtIndex(m_nCurPageIndex))->getPosition());
}