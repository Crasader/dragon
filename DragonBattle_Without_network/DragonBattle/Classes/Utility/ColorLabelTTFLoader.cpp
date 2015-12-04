//
//  ColorLabelTTFLoader.cpp
//  DragonBattle
//
//  Created by 白明江 on 3/4/14.
//
//

#include "ColorLabelTTFLoader.h"

#define PROPERTY_FONTSIZE "fontSize"
#define PROPERTY_STRING "string"


void ColorLabelTTFLoader::onHandlePropTypeFloat(CCNode * pNode, CCNode * pParent, const char* pPropertyName, float pFloat, CCBReader * pCCBReader)
{
    if(strcmp(pPropertyName, PROPERTY_FONTSIZE) == 0)
    {
        ((ColorLabelTTF *)pNode)->setFontSize(pFloat);
    }
    else
    {
        CCNodeLoader::onHandlePropTypeFloat(pNode, pParent, pPropertyName, pFloat, pCCBReader);
    }
}

void ColorLabelTTFLoader::onHandlePropTypeString(CCNode * pNode, CCNode * pParent, const char* pPropertyName, const char * pString, CCBReader * pCCBReader)
{
    if(strcmp(pPropertyName, PROPERTY_STRING) == 0)
    {
        ((ColorLabelTTF *)pNode)->setString(pString);
    }
    else
    {
        CCNodeLoader::onHandlePropTypeString(pNode, pParent, pPropertyName, pString, pCCBReader);
    }
}