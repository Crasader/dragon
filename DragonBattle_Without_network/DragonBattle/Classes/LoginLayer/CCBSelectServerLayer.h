//
//  CCBSelectServerLayer.h
//  DragonBattle
//
//  Created by zeng xiaolong on 13-6-22.
//
//

#ifndef __DragonBattle__CCBSelectServerLayer__
#define __DragonBattle__CCBSelectServerLayer__
#include "ViewDefine.h"
#include "XYScrollView.h"
#include "XYGroup.h"

enum kServerState
{
    kServerStateNormal = 0,
    kServerStateRecommend = 1,
    kServerStateBusy = 2,
};

class CCBSelectServerLayer: public TouchLayer
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
, public CCNodeLoaderListener
{
public:
    CCBSelectServerLayer();
    ~CCBSelectServerLayer();
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(CCBSelectServerLayer, create);
    
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode);
    virtual bool onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName,CCBValue* pCCBValue){return false;}
    virtual void onNodeLoaded(CCNode * pNode, CCNodeLoader * pNodeLoader);
private:
    void setServerList(CCArray*arr, int nLastLoginId);
private:
    void onEnter(CCObject* pTarget, CCControlEvent event);
    void onBack(CCObject* pTarget, CCControlEvent event);
    void loginComplete(CCNode* pSender,void* data);
    void onSelectServer(CCNode*pSender);
    
    XYSelectButton* createServerButton(CCDictionary *serverinfo, bool isLasted = false);
    
    CCControlButton *BtnEnter;
    CCControlButton *m_pBtnBack;
    CCNode* m_pNodeParticle;
    
    CCSprite* m_pLastLogin;
    
    XYScrollView *m_Scrollserver;
    XYGroup *m_group;
    int m_nIndex;
    void loadLayer();
};

class CCBSelectServerLayerLoader : public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CCBSelectServerLayerLoader, loader);
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CCBSelectServerLayer);
};

#endif /* defined(__DragonBattle__CCBSelectServerLayer__) */
