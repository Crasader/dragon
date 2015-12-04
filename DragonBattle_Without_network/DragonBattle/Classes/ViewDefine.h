//
//  ViewDefine.h
//  DragonBattle
//
//  Created by zeng xiaolong on 13-6-17.
//
//

#ifndef DragonBattle_ViewDefine_h
#define DragonBattle_ViewDefine_h
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;
USING_NS_CC;
#include "GlobalDefine.h"
#include "XYTopLayer.h"
#include "CCBDefine.h"
#include "TouchLayer.h"
#include "ThirdLayer.h"
#include "VisibleRect.h"
#include "NotificeDefine.h"
#define TOPADDCHILD(node) XYTopLayer::getInstance()->addChild(node)
#define SETCENTER(node)\
CCPoint poi = node->getParent()->convertToNodeSpace(SCREEN_MID_POS);\
node->setPosition(poi);
#define MIDDLE_POI(node)  ccp(node->getContentSize().width/2,node->getContentSize().height/2)  
#define OPEN_SUB_UI(parent, node) \
node->setTouchPriority(-20); \
(parent)->addChild((node), 129)


#endif
