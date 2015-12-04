//
//  CCBCreateRoleLayer.h
//  DragonBattle
//
//  Created by zeng xiaolong on 13-6-22.
//
//

#ifndef __DragonBattle__CCBCreateRoleLayer__
#define __DragonBattle__CCBCreateRoleLayer__
#include "ViewDefine.h"
#include "XYEditBox.h"

class XYTouchSprite;

class CCBCreateRoleLayer: public TouchLayer
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
, public CCNodeLoaderListener
{
public:
    CCBCreateRoleLayer();
    ~CCBCreateRoleLayer();
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(CCBCreateRoleLayer, create);
    
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode);
    virtual bool onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName,CCBValue* pCCBValue){return false;}
    virtual void onNodeLoaded(CCNode * pNode, CCNodeLoader * pNodeLoader);
public:
    void onCreate(CCObject* pTarget, CCControlEvent event);
    void onBack(CCObject* pTarget, CCControlEvent event);
    void onLeft(CCNode* pNode);
    void onRight(CCNode* pNode);
    void onDice(CCObject* pTarget, CCControlEvent event);
    void onTouch(CCNode* pNode, void* pFlag);
    
    virtual void update(float dt);
private:
    void createRoleError(CCObject* pTarget);
    
    CCNode* m_pNodeParticle;
    
    CCControlButton *BtnBack;
    CCControlButton *BtnCreate;
    XYTouchSprite* m_pTouch;
    CCControlButton *BtnLeft;
    CCControlButton *BtnRight;
    CCControlButton *BtnDice;
    
    CCSprite*       m_pSprRoleDesc;
    
    CCSprite* m_Role0;
    CCSprite* m_Role1;
    CCSprite* m_Role2;
    CCSprite* m_Role[3];
    CCSprite* m_RoleType;
    
    XYBaseTouchSprite* m_pLeft;
    XYBaseTouchSprite* m_pRight;
    
    XYEditBox* m_pEditBoxAccount;
    void loadLayer();
    
    void setRoleScaleAndColor(CCSprite* pRole, double x, double y);
    int getRoleType();
    void setTitle();
    
    std::vector<string> m_firstVec;
    std::vector<string> m_maleVec;
    std::vector<string> m_femaleVec;
    
    void setVec(std::string* pStr, std::vector<string>* pVec);
    
    CCPoint m_circlePoint;
    int m_nX;
    int m_nY;
    int m_pRoleAngle[3];
    int m_deltaAngle;
    int m_nLastRoleType;
};

class CCBCreateRoleLayerLoader : public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CCBCreateRoleLayerLoader, loader);
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CCBCreateRoleLayer);
};

#endif /* defined(__DragonBattle__CCBCreateRoleLayer__) */
