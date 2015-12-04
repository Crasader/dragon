//
//  CCBUtils.h
//  DragonBattle
//
//  Created by zeng xiaolong on 13-7-20.
//
//

#ifndef __DragonBattle__CCBUtils__
#define __DragonBattle__CCBUtils__
#include "ViewDefine.h"
#include "XYDownButton.h"
#include "XYListView.h"
#include "XYScrollView.h"
#include "XYListScrollView.h"
class CCBUtils
{
public:
    static CCProgressTimer * createProcess(CCSprite *spr);
    static XYDownButton*createDownButton(const char *pNormalFile,const char *pSelectFile, int nPriority,CCNode *node);
    static XYListView *createListView(int Priority,CCNode *node);
    static XYScrollView *createScrollView(int Priority,CCNode *node);
    static XYListScrollView *createListScrollView(int Priority,CCNode *node);
};

#endif /* defined(__DragonBattle__CCBUtils__) */
