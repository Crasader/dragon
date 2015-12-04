//
//  XYListView.cpp
//  XY
//
//  Created by long on 13-4-3.
//
//

#include "XYListView.h"
XYListView::XYListView()
{
    m_containlayer = NULL;
    m_viewarr = CCArray::create();
    CC_SAFE_RETAIN(m_viewarr);
    m_distence = 0;
    m_topDistence = 0;
    m_leftDistence = 0;
}
XYListView::~XYListView()
{
    CC_SAFE_RELEASE_NULL(m_viewarr);
}

XYListView* XYListView::createWith(int Priority,CCSize size,CCArray *viewarr)
{
    XYListView *pview = new XYListView();
    if (pview &&pview->initWith(Priority,size,viewarr))
    {
        pview->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pview);
    }
    return pview;
}

bool XYListView::initWith(int Priority,CCSize size,CCArray *viewarr)
{
    if (!CCLayerColor::init())
    {
        return false;
    }
    m_size = size;
    m_Priority = Priority;
    m_scrollview = XYScrollView::create(Priority,size);
    addChild(m_scrollview);
    resetarr(viewarr);
    schedule(schedule_selector(XYListView::updateVisable),0.1);
    return true;
}

void XYListView::resetarr(CCArray *arr, bool bIsVertical, bool bIsRefresh, int nScrollIndex)
{
    if (arr)
    {
        if (bIsRefresh && m_containlayer != NULL)
        {
            m_containlayer->removeFromParent();
            m_containlayer = NULL;
        }
        m_viewarr->removeAllObjects();
        m_viewarr->addObjectsFromArray(arr);
        createScroll(bIsVertical, nScrollIndex);
    }
}

void XYListView::resetListView(bool bIsVertical, bool bIsRefresh)
{
    if (bIsRefresh && m_containlayer != NULL)
    {
        m_containlayer->removeFromParent();
        m_containlayer = NULL;
    }
    createScroll(bIsVertical, -1);
}

void XYListView::updateVisable(float time)
{
    for (int i = 0; i<m_viewarr->count(); i++)
    {
        CCNode * node = (CCNode *)m_viewarr->objectAtIndex(i);
        CCPoint poi = m_scrollview->convertToNodeSpace(m_containlayer->convertToWorldSpace(node->getPosition()));
        
        CCRect rect = CCRect(poi.x, poi.y, node->getContentSize().width, node->getContentSize().height);
        if (m_scrollview->getVisableRect().intersectsRect(rect))
        {
            node->setVisible(true);
        }
        else
        {
            node->setVisible(false);
        }
    }
}

void XYListView::createScroll(bool bIsVertical, int nScrollIndex)
{
    bool isRearrange = false;
    if (m_containlayer != NULL)
    {
        isRearrange = true;
    }
    else
    {
        m_containlayer = XYContainLayer::create(m_Priority);
        m_scrollview->setContainer(m_containlayer);
        m_scrollview->setDirection(bIsVertical ? kCCScrollViewDirectionVertical :kCCScrollViewDirectionHorizontal);
    }
    
    int num = m_viewarr->count();
    
    CCObject *obj = NULL;
    if (bIsVertical)
    {
        float fheigh = 0;
        CCARRAY_FOREACH(m_viewarr, obj)
        {
            CCNode *node = (CCNode*)obj;
            fheigh += node->getContentSize().height*node->getScale()  + m_distence;
        }
        
        if (num > 0)
        {
            fheigh -= m_distence;
            fheigh += 2*m_topDistence;
        }
        
        if (m_size.height> fheigh)
        {
            m_scrollview->setContentSize(m_size);
            m_containlayer->setContentSize(m_size);
        }
        else
        {
            m_scrollview->setContentSize(CCSize(m_size.width, fheigh));
            m_containlayer->setContentSize(CCSize(m_size.width, fheigh));
        }
        
        float heigh = m_containlayer->getContentSize().height;
        float curheigh = 0;
        for (int i = 0; i<num; i++)
        {
            CCNode *cell = (CCNode*)m_viewarr->objectAtIndex(i);
            cell->setAnchorPoint(ccp(0, 0));
            if (i == 0)
            {
                curheigh += cell->getContentSize().height*cell->getScale() +m_topDistence;
            }
            else
            {
                curheigh += cell->getContentSize().height*cell->getScale()  + m_distence;
            }
            cell->setPosition(ccp(0+m_leftDistence, heigh -curheigh));
            if (cell->getParent() == NULL)
            {
                m_containlayer->addChild(cell);
            }
        }
        CCArray* pArray = m_containlayer->getChildren();
        if (pArray != NULL)
        {
            for (int i = 0; i < pArray->count(); i++)
            {
                CCNode* pNode = dynamic_cast<CCNode*>(pArray->objectAtIndex(i));
                if (!this->isInViewArr(pNode))
                {
                    pNode->removeFromParent();
                }
            }
        }
        if (!isRearrange)
        {
            if (nScrollIndex != -1)
            {
                float y = -(m_viewarr->count() * 1.0f - 1 - nScrollIndex) * (dynamic_cast<CCNode*>(m_viewarr->objectAtIndex(0))->getContentSize().height + m_distence);
                if (y < m_scrollview->minContainerOffset().y) y = m_scrollview->minContainerOffset().y;
                m_scrollview->setContentOffset(ccp(m_scrollview->minContainerOffset().x, y));
                //m_scrollview->setContentOffset(ccp(m_leftDistence, -100));
            }
            else
            {
                m_scrollview->setContentOffset(m_scrollview->minContainerOffset());
            }
        }
    }
    else
    {
        float fwidth = 0;
        CCARRAY_FOREACH(m_viewarr, obj)
        {
            CCNode *node = (CCNode*)obj;
            fwidth += node->getContentSize().width*node->getScale() + m_distence;
        }
        
        if (num > 0)
        {
            fwidth -= m_distence;
            fwidth += 2*m_topDistence;
        }
        
        if (m_size.width> fwidth)
        {
            m_scrollview->setContentSize(m_size);
            m_containlayer->setContentSize(m_size);
        }
        else
        {
            m_scrollview->setContentSize(CCSize(fwidth, m_size.height));
            m_containlayer->setContentSize(CCSize(fwidth, m_size.height));
        }
        
        for (int i = 0; i<num; i++)
        {
            CCNode *cell = (CCNode*)m_viewarr->objectAtIndex(i);
            cell->setAnchorPoint(ccp(0, 0));
            cell->setPosition(ccp((cell->getContentSize().width*cell->getScale() + m_distence) * i+m_topDistence, m_leftDistence));
            if (cell->getParent() == NULL)
            {
                m_containlayer->addChild(cell);
            }
        }
        CCArray* pArray = m_containlayer->getChildren();
        if (pArray != NULL)
        {
            for (int i = 0; i < pArray->count(); i++)
            {
                CCNode* pNode = dynamic_cast<CCNode*>(pArray->objectAtIndex(i));
                if (!this->isInViewArr(pNode))
                {
                    pNode->removeFromParent();
                }
            }
        }
        if (!isRearrange)
        {
            m_scrollview->setContentOffset(m_scrollview->maxContainerOffset());
        }
    }
    
    
}

bool XYListView::isInViewArr(CCNode* pNode)
{
    for (int i = 0; i < m_viewarr->count(); i++)
    {
        if (m_viewarr->objectAtIndex(i) == pNode)
        {
            return true;
        }
    }
    return false;
}
