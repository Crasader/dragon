//
//  XYContainLayer.h
//  Xiyouji
//
//  Created by long on 12-12-12.
//
//

#ifndef __Xiyouji__XYContainLayer__
#define __Xiyouji__XYContainLayer__
#include "cocos2d.h"
USING_NS_CC;
    class XYContainLayer:public CCLayerColor
    {
        public:
        XYContainLayer(void);
        ~XYContainLayer(void);
        static XYContainLayer*create(int Priority);
        CCArray *m_layerArray;
        CCArray *m_children;
        void addContain(CCNode *child);
        virtual void registerWithTouchDispatcher();
        virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
        void showAllChild();
        float m_height;
    };

#endif
