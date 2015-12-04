//
//  XYScrollView.h
//  XY
//
//  Created by long on 12-12-25.
//
//

#ifndef __XY__XYScrollView__
#define __XY__XYScrollView__
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
class XYScrollView:public CCScrollView
{
public:
    XYScrollView();
    ~XYScrollView();
    static XYScrollView *create(int Priority,CCSize size, CCNode* container = NULL);
    CCRect getVisableRect();
    CCPoint getContainLayerPoi();
};
#endif /* defined(__XY__XYScrollView__) */
