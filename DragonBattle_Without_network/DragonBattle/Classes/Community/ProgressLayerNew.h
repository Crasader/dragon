//
//  ProgressLayerNew.h
//  DragonBattle
//
//  Created by 何 永翔 on 14-2-24.
//
//

#ifndef __DragonBattle__ProgressLayerNew__
#define __DragonBattle__ProgressLayerNew__

#include "TouchLayer.h"

class ProgressLayerNew : public TouchLayer
{
public:
    virtual ~ProgressLayerNew();
    
    static ProgressLayerNew* create(const std::vector<std::string>* pFileVec, bool bHasTitle, SEL_CallFuncN selector = NULL, CCObject* pTarget = NULL);
    bool initWithConf(const std::vector<std::string>* pFileVec, bool bHasTitle, SEL_CallFuncN selector, CCObject* pTarget);
    
private:
    void load(float dt);
    void runProgressOver();
    void destroySelf();
    
    ProgressLayerNew();
    void setSprite(CCPoint shadowPosition, float fShadowScale, CCPoint rolePosition, float fRoleScale, const char* pRoleName, const char* pListName, int nRoleId);
    
    SEL_CallFuncN m_pSelector;
    CCObject* m_pTarget;
    
    std::vector<std::string> m_fileVec;
    CCProgressTimer* m_pProgressSprite;
    int m_nIndex;
    int m_nCount;
};

#endif /* defined(__DragonBattle__ProgressLayerNew__) */
