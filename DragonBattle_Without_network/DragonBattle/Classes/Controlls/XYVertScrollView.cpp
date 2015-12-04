//
//  XYVertScrollView.cpp
//  DragonBattle
//
//  Created by zeng xiaolong on 13-7-2.
//
//

#include "XYVertScrollView.h"
XYVertScrollView::XYVertScrollView()
{
}
XYVertScrollView::~XYVertScrollView()
{
}
XYVertScrollView *XYVertScrollView::create(int Priority,CCSize size, CCNode* container)
{
    XYVertScrollView* pRet = new XYVertScrollView();
    if (pRet && pRet->initWithViewSize(size, container))
    {
        
        pRet->setDirection(kCCScrollViewDirectionVertical);
        pRet->setTouchPriority(Priority);
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool XYVertScrollView::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (XYScrollView::ccTouchBegan(pTouch, pEvent))
    {
        m_startPoi = pTouch->getLocation();
        m_bmove = true;
        m_firstmove = true;
        return true;
    }
    return false;
}

void XYVertScrollView::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    CCPoint touchLocation = touch->getLocation();
    
    float distance = ccpDistance(touchLocation, m_startPoi);
    if (distance < 10 && m_firstmove)
    {
        return;
    }
    
    if (fabsf(touchLocation.x - m_startPoi.x)>=fabsf(touchLocation.y - m_startPoi.y)&&m_firstmove)
    {
        m_bmove = false;
    }
    m_firstmove = false;
    
    if (m_bmove)
    {
        CCScrollView::ccTouchMoved(touch, event);
    }
}

XYHorizeListScrollView::XYHorizeListScrollView()
{

    
}
XYHorizeListScrollView::~XYHorizeListScrollView()
{

}

XYHorizeListScrollView *XYHorizeListScrollView::create(int nindex,int pagecount,float width,int Priority,CCSize size,CCNode* container)
{
    XYHorizeListScrollView* pRet = new XYHorizeListScrollView();
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

bool XYHorizeListScrollView::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    if (XYScrollView::ccTouchBegan(pTouch, pEvent))
    {
        m_startPoi = pTouch->getLocation();
        m_bmove = true;
        m_firstmove = true;
        return true;
    }
    return false;
}

void XYHorizeListScrollView::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    CCPoint touchLocation = touch->getLocation();
    if (ccpDistance(touchLocation, m_startPoi)<10)
    {
        return;
    }
    if (fabsf(touchLocation.x - m_startPoi.x)<fabsf(touchLocation.y - m_startPoi.y)&&m_firstmove)
    {
        m_bmove = false;
    }
    m_firstmove = false;
    if (m_bmove)
    {
        CCScrollView::ccTouchMoved(touch, event);
        if (m_pXYScrollDelegate)
        {
            m_pXYScrollDelegate->scrollViewMoveChange(getCurIndex());
        }
        
    }
}