//
//  UiResources.h
//  DragonBattle
//  UI资源配置
//  Created by 小敦 王 on 13-4-18.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_UiResource_h
#define DragonBattle_UiResource_h

#include "cocos2d.h"
#include <map>

enum kUIResourceType{
    kUIResForSprite = 0,
    kUIResForMenu,
};

struct UIResourceInfo_t
{
    uint32_t    ID;
    std::string strName;
    bool        bFlipX;
    bool        bFilpY;
    float       fScale;
    cocos2d::CCPoint pt;
    cocos2d::CCPoint anchorPt;
    uint8_t     nResType;
    
    UIResourceInfo_t()
    {
        ID = 0;
        strName = "";
        bFilpY = false;
        bFlipX = false;
        fScale = 1.0;
        pt.x = 0.0;
        pt.y = 0.0;
        anchorPt = cocos2d::CCPoint(0.5, 0.5);
        nResType = kUIResForSprite;
    }
    
    UIResourceInfo_t(uint32_t _ID, const char* name, cocos2d::CCPoint point, cocos2d::CCPoint achPt, float scale = 1.0, kUIResourceType type = kUIResForSprite)
    {
        ID = _ID;
        strName = name;
        bFilpY = false;
        bFlipX = false;
        fScale = scale;
        pt = point;
        anchorPt = achPt;
        nResType = type;
    }
};

typedef std::map<uint32_t, UIResourceInfo_t*> UIResourceMap;

// 图片资源
class UIResources
{
public:
    UIResources();
    ~UIResources();
    
    static UIResources* sharedInstance();
    static void purgeInstance();
    void initUIResources();
    
    cocos2d::CCSprite* createUISpriteWithID(uint32_t ID);
    UIResourceInfo_t* getUIResourceWithID(uint32_t ID);
    
private:
    void insertResouceInfo(uint32_t ID, const char* name, cocos2d::CCPoint point, cocos2d::CCPoint achPt);
    
private:
    static UIResources* pSharedInstance;
    UIResourceMap m_pResourceMap;//UI 图片
};

#endif
