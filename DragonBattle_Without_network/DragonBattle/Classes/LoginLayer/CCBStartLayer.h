//
//  CCBStartLayer.h
//  DragonBattle
//
//  Created by 何 永翔 on 13-10-18.
//
//

#ifndef __DragonBattle__CCBStartLayer__
#define __DragonBattle__CCBStartLayer__

#include "ViewDefine.h"

class CCBStartLayer: public TouchLayer
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
, public CCNodeLoaderListener
{
public:
    CCBStartLayer();
    ~CCBStartLayer();
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(CCBStartLayer, create);
    
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode);
    virtual bool onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName,CCBValue* pCCBValue){return false;}
    virtual void onNodeLoaded(CCNode * pNode, CCNodeLoader * pNodeLoader);
public:
    void onStart(CCObject* pTarget, CCControlEvent event);
    void onBack(CCObject* pTarget, CCControlEvent event);
    inline void setNickName(const char* pName) { m_nickName = pName; }
    
private:
    CCControlButton*    m_pBtnStart;
    CCControlButton*    m_pBtnBack;
    CCSprite*           m_pSprLastedSver;
    CCSprite*           m_pSprGameIcon;
    CCLayerColor*       m_pLayerTransit;
    
    std::string m_nickName;

private:
    void loadLayer();
    void onSelectSvr(CCNode*);
    
    void startGame();
    void startGameCbk(CCObject*);
    void startNetWork();
    
};

class CCBStartLayerLoader : public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CCBStartLayerLoader, loader);
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CCBStartLayer);
};

#endif
