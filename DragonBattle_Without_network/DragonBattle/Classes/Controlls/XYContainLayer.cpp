//
//  XYContainLayer.cpp
//  Xiyouji
//
//  Created by long on 12-12-12.
//
//

#include "XYContainLayer.h"
#include "XYScrollView.h"
XYContainLayer::XYContainLayer(void)
{
    m_layerArray = CCArray::create();
    CC_SAFE_RETAIN(m_layerArray);
    m_children = CCArray::create();
    CC_SAFE_RETAIN(m_children);
}


XYContainLayer::~XYContainLayer(void)
{
    CC_SAFE_RELEASE_NULL(m_layerArray);
    CC_SAFE_RELEASE_NULL(m_children);
}

XYContainLayer* XYContainLayer::create(int Priority)
{
    XYContainLayer *pRet = new XYContainLayer();
    if (pRet && pRet->initWithColor(ccc4(255, 255, 255, 0)))
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

void XYContainLayer::addContain(CCNode *child)
{
    m_layerArray->addObject(child);
}

void XYContainLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, CCLayer::getTouchPriority(), true);
}

bool XYContainLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    if (getParent())
    {
        CCPoint touchLocation = touch->getLocation();
        touchLocation = this->convertToNodeSpace(touchLocation);
        CCRect r = ((CCMenuItem*)this)->rect();
        r.origin = CCPointZero;
        if (r.containsPoint(touchLocation))
        {
            XYScrollView *p = (XYScrollView *)getParent();
            if (p->getVisableRect().containsPoint(p->convertToNodeSpace(touch->getLocation())))
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
void XYContainLayer::showAllChild()
{
    this->removeAllChildrenWithCleanup(true);
    float height = 0;
    float width = 0;
    CCObject *pItem;
    CCARRAY_FOREACH(m_layerArray,pItem)
    {
        CCNode *node = (CCNode*)pItem;
        node->setPosition(ccp(0,height));
        height += node->getContentSize().height;
        width = node->getContentSize().width;
        addChild(node);
    }
    m_height = height;
    setContentSize(CCSize(width,m_height));
}
