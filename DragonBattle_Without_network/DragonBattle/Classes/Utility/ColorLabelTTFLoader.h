//
//  ColorLabelTTFLoader.h
//  DragonBattle
//
//  Created by 白明江 on 3/4/14.
//
//

#ifndef __DragonBattle__ColorLabelTTFLoader__
#define __DragonBattle__ColorLabelTTFLoader__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ColorLabelTTF.h"

USING_NS_CC;
USING_NS_CC_EXT;

class ColorLabelTTFLoader : public CCNodeLoader
{
public:
    virtual ~ColorLabelTTFLoader() {};
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(ColorLabelTTFLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(ColorLabelTTF);

    virtual void onHandlePropTypeFloat(CCNode * pNode, CCNode * pParent, const char* pPropertyName, float pFloat, CCBReader * pCCBReader);
    virtual void onHandlePropTypeString(CCNode * pNode, CCNode * pParent, const char* pPropertyName, const char * pString, CCBReader * pCCBReader);

};

#endif /* defined(__DragonBattle__ColorLabelTTFLoader__) */
