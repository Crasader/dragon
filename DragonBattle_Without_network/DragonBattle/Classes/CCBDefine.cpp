//
//  CCBDefine.cpp
//  DragonBattle
//
//  Created by 白明江 on 1/13/14.
//
//

#include "CCBDefine.h"
#include "HRichLabelTTF.h"
#include "CCEditBoxLoader.h"
#include "ColorLabelTTFLoader.h"

CCNode* CCBDefine::createCCBNode(const char *strClassName,CCNodeLoader *pLoader,const char *strCCBFlieResource,CCNode *pnode)
{
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader(strClassName,pLoader);
    registerCustomCCNodeLoaders(ccNodeLoaderLibrary);
    cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
    CCNode * node = ccbReader->readNodeGraphFromFile(strCCBFlieResource, pnode);
    ccbReader->release();
    return node;
}

void CCBDefine::registerCustomCCNodeLoaders(CCNodeLoaderLibrary * ccNodeLoaderLibrary)
{
    ccNodeLoaderLibrary->registerCCNodeLoader("HRichLabelTTF", HRichLabelTTFLoader::loader());
    ccNodeLoaderLibrary->registerCCNodeLoader("CCEditBox", CCEditBoxLoader::loader());
    ccNodeLoaderLibrary->registerCCNodeLoader("ColorLabelTTF", ColorLabelTTFLoader::loader());
}