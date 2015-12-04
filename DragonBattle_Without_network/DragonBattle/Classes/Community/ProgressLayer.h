//
//  ProgressLayer.h
//  DragonBattle
//
//  Created by 何 永翔 on 13-9-25.
//
//

#ifndef __DragonBattle__ProgressLayer__
#define __DragonBattle__ProgressLayer__

#include "TouchLayer.h"

class ProgressLayer : public TouchLayer
{
public:
    virtual ~ProgressLayer();
    
    static ProgressLayer* create(const char* pBg, const std::vector<std::string>* pFileVec, bool bHasTitle, SEL_CallFuncN selector = NULL, CCObject* pTarget = NULL);
    bool initWithConf(const char* pBg, const std::vector<std::string>* pFileVec, bool bHasTitle, SEL_CallFuncN selector, CCObject* pTarget);
    
private:
    void load(float dt);
    void runProgressOver();
    void destroySelf();
    
    ProgressLayer();
    
    SEL_CallFuncN m_pSelector;
    CCObject* m_pTarget;
    
    std::vector<std::string> m_fileVec;
    CCProgressTimer* m_pProgressSprite;
    int m_nIndex;
    int m_nCount;
};

#endif
