//
//  XYNumberView.cpp
//  DragonBattle
//
//  Created by zeng xiaolong on 13-12-30.
//
//

#include "XYNumberView.h"
#include "XYContainLayer.h"
#include "XYListScrollView.h"
XYNumberView::XYNumberView()
{
    m_pDelegate = NULL;
}
XYNumberView::~XYNumberView()
{

}

XYNumberView*XYNumberView::create(int number,int prority)
{
    XYNumberView *pNumber = new XYNumberView();
    if (pNumber && pNumber->init(number,prority))
    {
        pNumber->autorelease();
        return pNumber;
    }
    CC_SAFE_DELETE(pNumber);
    return NULL;
}

bool XYNumberView::init(int number,int prority)
{
    if (!CCLayer::init())
    {
        return false;
    }
    CCSprite *sprbg = CCSprite::create("public/public_numberbg.png");
    addChild(sprbg);
    
    CCSprite *sprNumbg = CCSprite::create("public/public_number.png");
    addChild(sprNumbg);
    
    float cellWidth = 114.0f;
    CCSize size = CCSize(570, 90);
    m_pScrollView = XYListScrollView::create(0,number,cellWidth,prority-2,size);
    m_pScrollView->setPositionX(-size.width/2);
    m_pScrollView->setPositionY(-size.height/2);
    m_pScrollView->setInertia(true);
    addChild(m_pScrollView);
    m_pScrollView->setContentSize(CCSize(cellWidth*(number),size.height));
    m_pContainer = XYContainLayer::create(prority-2);
    m_pContainer->setContentSize(CCSizeMake(cellWidth*(number),size.height));
    m_pScrollView->setContainer(m_pContainer);
    m_pScrollView->setScrollDelegate(this);
    char str[64] = {0};
    for (int i = 0; i<number; i++)
    {
        sprintf(str, "%02d",i+1);
        CCLabelTTF *lbl = CCLabelTTF::create(str, FONTNAME, 50);
        lbl->setPosition(ccp((i+2)*cellWidth + cellWidth/2 + 2,size.height/2));
        lbl->setTag(i);
        m_pContainer->addChild(lbl);
    }
    sprNumbg = CCSprite::create("public/public_numberscale.png");
    addChild(sprNumbg);
    return true;
}

void XYNumberView::setTimes(int times)
{
    if (times<1)
    {
        times = 1;
    }
    m_pScrollView->setListIndex(times-1);
    scrollViewMoveChange(times-1);
}

int XYNumberView::getTimes()
{
    return m_pScrollView->getCurIndex() + 1;
}

void XYNumberView::scrollViewMoveChange(int index)
{
    if (m_pDelegate)
    {
        m_pDelegate->numberChange(index);
    }
    
    for (int i = 0; i<m_pScrollView->getPagecount(); i++)
    {
        CCNode *node = m_pContainer->getChildByTag(i);
        if (m_pScrollView->getCurIndex() == i)
        {
            node->setScale(1.4);
        }
        else
        {
            node->setScale(1);
        }
    }
}
