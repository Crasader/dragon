//
//  CCBDefine.h
//  DragonBattle
//
//  Created by zeng xiaolong on 13-6-6.
//
//
#include "cocos2d.h"
#include "cocos-ext.h"
#include "CCBUtils.h"

USING_NS_CC;
USING_NS_CC_EXT;

#ifndef DragonBattle_CCBDefine_h
#define DragonBattle_CCBDefine_h

class ItemInfoControl;
class HRichLabelTTF;

class CCBDefine
{
public:
    static CCNode *createCCBNode(const char *strClassName,CCNodeLoader *pLoader,const char *strCCBFlieResource,CCNode *pnode);
    static void registerCustomCCNodeLoaders(CCNodeLoaderLibrary * ccNodeLoaderLibrary);
};

#define CREATE_CCB_NODE(strClassName,pLoader,strCCBFlieResource,pnode) CCBDefine::createCCBNode(\
strClassName,pLoader,strCCBFlieResource,pnode)
#endif