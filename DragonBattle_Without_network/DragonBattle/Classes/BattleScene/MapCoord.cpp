//
//  MapCoord.cpp
//  DragonBattle
//
//  Created by 小敦 王 on 13-4-23.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "MapCoord.h"
#include "GlobalDefine.h"
USING_NS_CC;

#define MAP_MAX_HEIGHT SCREEN_HEIGTH

CCLayer* MapCoord::s_pMainLayer = NULL;
void MapCoord::setMainLayer(CCLayer* pLayer)
{
    s_pMainLayer = pLayer;
}

// convert screen position to layer position
CCPoint MapCoord::screenToLayer(CCPoint posOnScreen)
{
    // firstly, get the layer's position to screen and scale
    CCPoint layerPos = s_pMainLayer->getPosition();
    float layerScale = s_pMainLayer->getScale();
    
    // secondly, convert screen position to layer position
    CCPoint posOnLayer;
    posOnLayer.x = (posOnScreen.x - layerPos.x) / layerScale;
    posOnLayer.y = (posOnScreen.y - layerPos.y) / layerScale;
    
    return posOnLayer;
}

// convert layer position to screen position
CCPoint MapCoord::layerToScreen(CCPoint posOnLayer)
{
    // firstly, get the layer's position to screen and scale
    CCPoint layerPos = s_pMainLayer->getPosition();
    float layerScale = s_pMainLayer->getScale();
    
    // secondly, convert layer position to screen position
    CCPoint posOnScreen;
    posOnScreen.x = posOnLayer.x * layerScale + layerPos.x;
    posOnScreen.y = posOnLayer.y * layerScale + layerPos.y;
    
    return posOnScreen;
}

// convert screen position to map position
CCPoint MapCoord::screenToMap(CCPoint posOnScreen)
{
    // firstly, convert it to layer position
    CCPoint posOnLayer = screenToLayer(posOnScreen);
    
    // secondly, convert it to map position
    CCPoint posOnMap = layerToMap(posOnLayer);
    
    return posOnMap;
}

// convert map position to screen position
CCPoint MapCoord::mapToLayer(CCPoint posOnMap)
{
    CCPoint posOnLayer;
    posOnLayer.x = posOnMap.x;
    posOnLayer.y = MAP_MAX_HEIGHT - posOnMap.y;
    
    return posOnLayer;    
}

// convert layer position to map position
CCPoint MapCoord::layerToMap(CCPoint posOnLayer)
{
    CCPoint posOnMap;
    posOnMap.x = posOnLayer.x;
    posOnMap.y = MAP_MAX_HEIGHT - posOnLayer.y;
    
    return posOnMap;
}