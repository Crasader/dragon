//
//  CCBPanelFailureGuide.h
//  DragonBattle
//
//  Created by 何 永翔 on 14-3-6.
//
//

#ifndef __DragonBattle__CCBPanelFailureGuide__
#define __DragonBattle__CCBPanelFailureGuide__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "TouchLayer.h"

class ImproveTipPanel;

class CCBPanelFailureGuide :
public cocos2d::extension::CCBSelectorResolver,
public cocos2d::extension::CCBMemberVariableAssigner,
public cocos2d::extension::CCNodeLoaderListener,
public TouchLayer
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(CCBPanelFailureGuide, create);
    
    CCBPanelFailureGuide();
    virtual ~CCBPanelFailureGuide();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual bool onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue){return false;}
    virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);
    void onShowDialog(cocos2d::CCObject *p);
    void onAddConfirm(CCNode* p);
    void onCancel(CCNode* p);
private:
    void onClose(CCObject* pTarget, cocos2d::extension::CCControlEvent event);
    void onPartner(CCObject* pTarget, cocos2d::extension::CCControlEvent event);
    void onStrengthen(CCObject* pTarget, cocos2d::extension::CCControlEvent event);
    void onInforce(CCObject* pTarget, cocos2d::extension::CCControlEvent event);
    void onSkill(CCObject* pTarget, cocos2d::extension::CCControlEvent event);
    
    cocos2d::extension::CCControlButton* m_pBtnClose;
    cocos2d::extension::CCControlButton* m_pBtnPartner;
    cocos2d::extension::CCControlButton* m_pBtnStrengthen;
    cocos2d::extension::CCControlButton* m_pBtnInforce;
    cocos2d::extension::CCControlButton* m_pBtnSkill;
    
public:
    virtual void loadLayer();
    
    void setMissionType(uint32_t missionType) { m_nMissionType = missionType; }
    
private:
    void createLinkLabel(CCNode* pNode);
    void rearrangePosition(CCArray* pArray);
    
private:
    uint32_t m_nMissionType;
};

class CCBPanelFailureGuideLoader : public cocos2d::extension::CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CCBPanelFailureGuideLoader, loader);
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CCBPanelFailureGuide);
};

#endif /* defined(__DragonBattle__CCBPanelFailureGuide__) */
