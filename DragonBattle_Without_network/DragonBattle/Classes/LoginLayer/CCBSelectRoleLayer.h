//
//  CCBSelectRoleLayer.h
//  DragonBattle
//
//  Created by zeng xiaolong on 13-6-22.
//
//

#ifndef __DragonBattle__CCBSelectRoleLayer__
#define __DragonBattle__CCBSelectRoleLayer__

#include "ViewDefine.h"
#include "XYEditBox.h"
class CCBSelectRoleLayer: public TouchLayer
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
, public CCNodeLoaderListener
{
public:
    CCBSelectRoleLayer();
    ~CCBSelectRoleLayer();
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(CCBSelectRoleLayer, create);
    
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode);
    virtual bool onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName,CCBValue* pCCBValue){return false;}
    virtual void onNodeLoaded(CCNode * pNode, CCNodeLoader * pNodeLoader);
public:
    void onCreate(CCObject* pTarget, CCControlEvent event);
    void onBack(CCObject* pTarget, CCControlEvent event);
    void onBegin(CCObject* pTarget, CCControlEvent event);
    
    void setUser(int nRoleType, int nLv, const char* pName);
    
private:
    CCControlButton *BtnBack;
    //CCControlButton *BtnCreate;
    CCControlButton *BtnBegin;
    
    CCSprite* m_pRoleType;
    CCSprite* m_pRole;
    
    CCLabelTTF* m_pLv;
    CCLabelTTF* m_pName;
    CCLabelTTF* m_pServerName;
    
    void loadLayer();
};

class CCBSelectRoleLayerLoader : public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CCBSelectRoleLayerLoader, loader);
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CCBSelectRoleLayer);
};

#endif /* defined(__DragonBattle__CCBSelectRoleLayer__) */
