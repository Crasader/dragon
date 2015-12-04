//
//  CCBUtils.cpp
//  DragonBattle
//
//  Created by zeng xiaolong on 13-7-20.
//
//

#include "CCBUtils.h"

CCProgressTimer *CCBUtils::createProcess(CCSprite *spr)
{
    CCPoint poi = spr->getPosition();
    CCProgressTimer *bar = CCProgressTimer::create(spr);
    bar->setType(kCCProgressTimerTypeBar);
    bar->setMidpoint(CCPointMake(0,0));
    bar->setBarChangeRate(ccp(1,0));
    bar->setAnchorPoint(CCPointZero);
    bar->setPosition(poi);
    spr->getParent()->addChild(bar,0);
    spr->removeFromParent();
    return bar;
}

XYDownButton*CCBUtils::createDownButton(const char *pNormalFile,const char *pSelectFile, int nPriority,CCNode *node)
{
    XYDownButton *btn = XYDownButton::create(pNormalFile, pSelectFile, nPriority);
    btn->setPosition(node->getPosition());
    node->getParent()->addChild(btn);
    node->removeFromParent();
    return btn;
}

XYListView *CCBUtils::createListView(int Priority,CCNode *node)
{
    XYListView* view = XYListView::createWith( Priority, node->getContentSize());
    view->setPosition(node->getPosition());
    node->getParent()->addChild(view);
    node->removeFromParent();
    return view;
}

XYScrollView *CCBUtils::createScrollView(int Priority,CCNode *node)
{
    XYScrollView* view = XYScrollView::create(Priority, node->getContentSize());
    view->setPosition(node->getPosition());
    node->getParent()->addChild(view);
    node->removeFromParent();
    return view;
}

XYListScrollView *CCBUtils::createListScrollView(int Priority,CCNode *node)
{
    XYListScrollView* view = XYListScrollView::create(0,1,node->getContentSize().width,Priority,node->getContentSize());
    view->setPosition(node->getPosition());
    node->getParent()->addChild(view);
    node->removeFromParent();
    return view;
}