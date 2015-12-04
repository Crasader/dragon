//
//  CCBPanelVictoryNew.h
//  DragonBattle
//
//  Created by 何 永翔 on 13-8-31.
//
//

#ifndef DragonBattle_CCBPanelVictoryNew_h
#define DragonBattle_CCBPanelVictoryNew_h

#include "cocos2d.h"
#include "cocos-ext.h"
#include "TouchLayer.h"
#include "OnlineManager.h"

typedef std::vector<std::string>  StrVec;

class CCBPanelVictoryNew :
public cocos2d::extension::CCBSelectorResolver,
public cocos2d::extension::CCBMemberVariableAssigner,
public cocos2d::extension::CCNodeLoaderListener,
public GrayLayer
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(CCBPanelVictoryNew, create);
    
    CCBPanelVictoryNew();
    virtual ~CCBPanelVictoryNew();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual bool onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue){return false;}
    virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);
    void onShowDialog(cocos2d::CCObject *p);
    void onAddConfirm(CCNode* p);
    void onCancel(CCNode* p);
private:
    void playStarSoundEffect();
    void onReturn(CCObject* pTarget, cocos2d::extension::CCControlEvent event);
    void onRenew(CCObject* pTarget, cocos2d::extension::CCControlEvent event);
    
    CCSprite* m_pRole;
    CCSprite* m_pCountDown;
    CCSprite* m_pBottomBlack;
    CCSprite* m_pBonus;
    CCSprite* m_pBottom;
    CCSprite* m_pCircle0;
    CCSprite* m_pCircle1;
    CCSprite* m_pStar[3];
    
    cocos2d::extension::CCControlButton* m_pBtnRenew;
    cocos2d::extension::CCControlButton* m_pBtnReturn;
    
public:
    virtual void loadLayer();
    
    void setResult(int mission_type, int nStar, StrVec* pRoleNameVec, StrVec* pRoleImgVec, int nInForce, int nExp, int nMoney, int nGold, std::map<int, int>* pDropMapItem);
    void setFirstWinButton(bool bIsFirst);
    
private:
    void shakeSelf();
    void startCalculate();
    void countDown(float dt);
    void update(float dt);
    void startRotate(float dt);
    void onItem(CCNode* pNode);
    
    void addToVec(int nItemId, int nItemCount, std::map<int, int>* pDropMapItem);

    int m_nCurrentNum;
    int m_nMissionType;
};

class CCBPanelVictoryNewLoader : public cocos2d::extension::CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CCBPanelVictoryNewLoader, loader);
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CCBPanelVictoryNew);
};

#endif
