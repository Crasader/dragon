//
//  CCBPanelFailureNew.h
//  DragonBattle
//
//  Created by 何 永翔 on 13-9-2.
//
//

#ifndef DragonBattle_CCBPanelFailureNew_h
#define DragonBattle_CCBPanelFailureNew_h

#include "cocos2d.h"
#include "cocos-ext.h"
#include "TouchLayer.h"

class ImproveTipPanel;

class CCBPanelFailureNew :
public cocos2d::extension::CCBSelectorResolver,
public cocos2d::extension::CCBMemberVariableAssigner,
public cocos2d::extension::CCNodeLoaderListener,
public GrayLayer
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(CCBPanelFailureNew, create);
    
    CCBPanelFailureNew();
    virtual ~CCBPanelFailureNew();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual bool onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue){return false;}
    virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);
    void onShowDialog(cocos2d::CCObject *p);
    void onAddConfirm(CCNode* p);
    void onCancel(CCNode* p);
private:
    void onReturn(CCObject* pTarget, cocos2d::extension::CCControlEvent event);
    void onRenew(CCObject* pTarget, cocos2d::extension::CCControlEvent event);
    
    void countDown(float dt);
    bool isShowFailureGuide();
    
    CCSprite* m_pCountDown;
    
    cocos2d::extension::CCControlButton* m_pBtnRenew;
    cocos2d::extension::CCControlButton* m_pBtnReturn;
    
public:
    virtual void loadLayer();
    
    void setMissionType(uint32_t missionType) { m_nMissionType = missionType; }
    
private:
    void shakeSelf();
    
private:
    uint32_t m_nMissionType;
};

class CCBPanelFailureNewLoader : public cocos2d::extension::CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CCBPanelFailureNewLoader, loader);
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CCBPanelFailureNew);
};

#endif
