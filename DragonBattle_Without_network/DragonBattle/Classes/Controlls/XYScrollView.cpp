//
//  XYScrollView.cpp
//  XY
//
//  Created by long on 12-12-25.
//
//

#include "XYScrollView.h"
XYScrollView::XYScrollView()
{
}

XYScrollView::~XYScrollView()
{
}

XYScrollView *XYScrollView::create(int Priority,CCSize size, CCNode* container)
{
    XYScrollView* pRet = new XYScrollView();
    if (pRet && pRet->initWithViewSize(size, container))
    {
        pRet->setTouchPriority(Priority);
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

CCRect XYScrollView::getVisableRect()
{
    return CCRect(0,0, m_tViewSize.width, m_tViewSize.height);
}

CCPoint XYScrollView::getContainLayerPoi()
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
