//
//  ZQTextLink.h
//  TianJiang
//
//  Created by bless on 13-5-8.
//
//

#ifndef __TianJiang__ZQTextLink__
#define __TianJiang__ZQTextLink__

#include <iostream>
#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
#include "ViewDefine.h"
enum ENUM_TEXTLINK_TYPE
{
    ENUM_TEXTLINK_TYPE_LAB = 1,//文字
    ENUM_TEXTLINK_TYPE_BTN = 2,//按钮
    ENUM_RICHTEXT_TYPE_SPR = 3,//图片
    
};

class TextLinkData : public CCObject
{
public:
    TextLinkData()
    {
        nType = ENUM_TEXTLINK_TYPE_LAB;
        strName = "";
        invocation = NULL;
        nFontSize = 20;
        color = ccWHITE;
        pData = NULL;
    }
    ~TextLinkData()
    {
        
    }
    
    static TextLinkData* create()
    {
        TextLinkData *pData = new TextLinkData();
        pData->autorelease();
        return pData;
    }
    
    int nType;
    std::string strName;    
    CCInvocation *invocation;
    ccColor3B color;
    int nFontSize;
    
    
    void *pData;
};


class ZQTextLink : public CCLayer
{
public:
    ZQTextLink();
    virtual ~ZQTextLink();
    static ZQTextLink *create(CCArray *pArray, int nMaxWidth = 1136/**/, int nDistanceHeight = 0);//TextLinkData数组
    bool init(CCArray *pArray, int nMaxWidth, int nDistanceHeight);
    
};
#endif /* defined(__TianJiang__ZQTextLink__) */
