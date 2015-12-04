//
//  LoadingLayer.h
//  DragonBattle
//
//  Created by 小敦 王 on 13-5-7.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_LoadingLayer_h
#define DragonBattle_LoadingLayer_h

#include "cocos2d.h"
#include "TouchLayer.h"

class LoadingLayer : public TouchLayer
{
public:
    virtual bool init();
    
    // implement the "static node()" method manually
    CREATE_FUNC(LoadingLayer);
    
    static LoadingLayer* create(cocos2d::CCObject* pObject, cocos2d::SEL_CallFunc pTarFunc);
    
    void close();
    
    void createBkg();
    
    void runOffsetPercent(float offsetPercent);
    
    virtual void update(float dt);
    
    void setTargetFunction(CCObject* pObj, cocos2d::SEL_CallFunc pTarFunc);
    
    void setLoadingFinished();
    
private:
    CCProgressTimer*        m_pProgressSprite;
    
    float   m_fLastDt;
    float   m_fPercent;
    bool    m_bLoadingFinished;
    cocos2d::CCObject*      m_pTarObject;
    cocos2d::SEL_CallFunc   m_pTarFunc;
};

#endif
