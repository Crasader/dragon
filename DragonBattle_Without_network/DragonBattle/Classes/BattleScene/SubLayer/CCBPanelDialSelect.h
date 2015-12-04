//
//  CCBPanelDialSelect.h
//  DragonBattle
//
//  Created by 何 永翔 on 14-2-18.
//
//

#ifndef __DragonBattle__CCBPanelDialSelect__
#define __DragonBattle__CCBPanelDialSelect__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "TouchLayer.h"
#include "GameData.h"

class ItemSprite;

class CCBPanelDialSelectDelegate
{
public:
    virtual void onPanelDialSelectDial(uint32_t nCostGold, int nTimes) = 0;
    virtual void onPanelDialSelectReturn() = 0;
};

class CCBPanelDialSelect : public GrayLayer,
public cocos2d::extension::CCBSelectorResolver,
public cocos2d::extension::CCBMemberVariableAssigner,
public cocos2d::extension::CCNodeLoaderListener
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(CCBPanelDialSelect, create);
    
    CCBPanelDialSelect();
    virtual ~CCBPanelDialSelect();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual bool onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue){return false;}
    virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);
    
private:
    CCSprite* m_pItems[14];
    
    CCControlButton* m_pBtnReturn;
    CCControlButton* m_pBtnGold;
    CCControlButton* m_pBtnDial;
    
    CCLabelTTF* m_pGoldTotal;
    CCLabelTTF* m_pDesc;
    
public:
    virtual void loadLayer();
    
    void setPrize(const char* pImg, const char* pName, int nId, int nCount, bool bIsBagFull);
    void setData(std::vector<item_unit_t>* pItemVec, int nTotalCostGold);
    
    void setDelegate(CCBPanelDialSelectDelegate* pDelegate) { m_pDelegate = pDelegate; }
    
private:
    void onReturn(CCObject* pTarget, CCControlEvent event);
    void onGold(CCObject* pTarget, CCControlEvent event);
    void onDial(CCObject* pTarget, CCControlEvent event);
    void getAllBattlePrize(cocos2d::CCObject *p);
    
    void selectItem(int nIndex);
    void onItem(CCNode* pNode);
    void setDialPosition();
    
    void dialToIndex(int nIndex, int nIndex1);
    
    std::map<int, int> m_getMap;
    
    ItemSprite* m_pItemSprites[14];
    int m_nFirstIndex;
    int m_nSecondIndex;
    int m_nDialIndex;
    int m_nTotalCostGold;
    bool m_bIsBagFull;
    
    CCBPanelDialSelectDelegate* m_pDelegate;
};

class CCBPanelDialSelectLoader : public cocos2d::extension::CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CCBPanelDialSelectLoader, loader);
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CCBPanelDialSelect);
};

#endif /* defined(__DragonBattle__CCBPanelDialSelect__) */
