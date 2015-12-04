//
//  CCBPanelCardSelectNew.h
//  DragonBattle
//
//  Created by 何 永翔 on 13-9-2.
//
//

#ifndef DragonBattle_CCBPanelCardSelectNew_h
#define DragonBattle_CCBPanelCardSelectNew_h

#include "cocos2d.h"
#include "cocos-ext.h"
#include "TouchLayer.h"

struct card_t {
    std::string pImg;
    int nNum;
};

class CCBPanelCardSelectNewDelegate
{
public:
    virtual void onPanelCardSelectNewSlotClick(uint32_t nCostGold, uint32_t nCardState) = 0;
    virtual void onPanelCardSelectNewClose() = 0;
    virtual void onPanelCardSelectNewReturn() = 0;
};

class CCBPanelCardSelectNew :
public cocos2d::extension::CCBSelectorResolver,
public cocos2d::extension::CCBMemberVariableAssigner,
public cocos2d::extension::CCNodeLoaderListener,
public TouchLayer
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(CCBPanelCardSelectNew, create);
    
    CCBPanelCardSelectNew();
    virtual ~CCBPanelCardSelectNew();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual bool onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue){return false;}
    virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);
    
private:
    void onAutoSelectCard(CCObject* pTarget, cocos2d::extension::CCControlEvent event);
    void onSlotClick(CCObject* pTarget, cocos2d::extension::CCControlEvent event);
    void onStart(CCObject* pTarget, cocos2d::extension::CCControlEvent event);
    void onClose(CCObject* pTarget, cocos2d::extension::CCControlEvent event);
    void onReturn(CCObject* pTarget, cocos2d::extension::CCControlEvent event);
    void onRenew(CCObject* pTarget, cocos2d::extension::CCControlEvent event);
    void canClick(CCNode* pNode);
    
    void countDown(float dt);
    
    CCLabelTTF* m_pGold;
    
    CCSprite* m_pBg;
    CCSprite* m_pCountDown;
    
    cocos2d::extension::CCControlButton* m_pSlot[5];
    cocos2d::extension::CCControlButton* m_pBtnStart;
    cocos2d::extension::CCControlButton* m_pBtnBg;
    cocos2d::extension::CCControlButton* m_pBtnClose;
    cocos2d::extension::CCControlButton* m_pBtnRenew;
    cocos2d::extension::CCControlButton* m_pBtnReturn;
    
public:
    virtual void loadLayer();
    
    void setData(std::vector<std::string>* pImgVec, std::vector<std::string>* pNameVec, std::vector<int>* pNumVec);
    void setCardPrize(const char* pImg, const char* pName, int nId, int nCount);
    inline void setDelegate(CCBPanelCardSelectNewDelegate* pDelegate) { m_pDelegate = pDelegate; }
    
private:
    void onConfirmSpend(CCNode* pNode);
    void onCancelSpend(CCNode* pNode);
    
    void setGold(int nTotal);
    int getGold();
    int getTotalGold(int nTotal);
    void flipCard(CCNode* pFace, CCNode* pBack, float fDuration);
    CCSprite* createCard(const char* pImgUrl, const char* pName, int nNum, int nTag, CCPoint pos, bool bIsOutLight = false);
    
    std::map<int, card_t> m_CardMap;
    std::map<int, int> m_PosMap;
    CCBPanelCardSelectNewDelegate* m_pDelegate;
    bool m_bCanClick;
    int m_nCardTag;
    
    SEL_CCControlHandler m_countDownHandler;
};

class CCBPanelCardSelectNewLoader : public cocos2d::extension::CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CCBPanelCardSelectNewLoader, loader);
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CCBPanelCardSelectNew);
};

#endif
