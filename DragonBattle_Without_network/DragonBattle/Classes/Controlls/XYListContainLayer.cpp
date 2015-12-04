//
//  XYListContainLayer.cpp
//  DragonBattle
//
//  Created by zeng xiaolong on 13-7-2.
//
//

#include "XYListContainLayer.h"
#include "XYListScrollView.h"
XYListContainLayer::XYListContainLayer(void)
{

}

XYListContainLayer::~XYListContainLayer(void)
{

}

XYListContainLayer*XYListContainLayer::create(int Priority)
{
    XYListContainLayer *pRet = new XYListContainLayer();
    if (pRet && pRet->init())
    {
        pRet->setTouchEnabled(true);
        pRet->setTouchPriority(Priority);
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}


bool XYListContainLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    if (getParent())
    {
        CCPoint touchLocation = touch->getLocation();
        touchLocation = this->convertToNodeSpace(touchLocation);
        CCRect r = ((CCMenuItem*)this)->rect();
        r.origin = CCPointZero;
        if (r.containsPoint(touchLocation))
        {
            XYListScrollView *p = (XYListScrollView *)getParent();
            if (p->getVisableRect().containsPoint(p->convertToNodeSpace((this->convertToWorldSpace(this->convertTouchToNodeSpace(touch))))))
            {
                return false;
            }
            else
            {
                return true;
            }
        }
    }
    return false;
}