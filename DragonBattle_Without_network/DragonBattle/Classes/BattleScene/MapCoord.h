//
//  MapCoord.h
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-23.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_MapCoord_h
#define DragonBattle_MapCoord_h

#include "cocos2d.h"

class MapCoord
{
public:
    // set main layer
    static  void setMainLayer(cocos2d::CCLayer* pLayer);
    
    // convert screen position to layer position
    static  cocos2d::CCPoint screenToLayer(cocos2d::CCPoint posOnScreen);
    
    // convert layer position to screen position
    static  cocos2d::CCPoint layerToScreen(cocos2d::CCPoint posOnLayer);
    
    // convert screen position to map position
    static  cocos2d::CCPoint screenToMap(cocos2d::CCPoint posOnScreen);
    
    // convert map position to screen position
    static  cocos2d::CCPoint mapToLayer(cocos2d::CCPoint posOnMap);
    
    // convert layer position to map position
    static  cocos2d::CCPoint layerToMap(cocos2d::CCPoint posOnLayer);
    
private:
    static  cocos2d::CCLayer* s_pMainLayer;
};

#endif
