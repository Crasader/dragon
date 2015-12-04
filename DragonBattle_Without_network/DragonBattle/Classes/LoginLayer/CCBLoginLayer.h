//
//  CCBLoginLayer.h
//  DragonBattle
//
//  Created by zeng xiaolong on 13-6-21.
//
//

#ifndef __DragonBattle__CCBLoginLayer__
#define __DragonBattle__CCBLoginLayer__

#include "ViewDefine.h"
#include "XYEditBox.h"
#include "XYToggleButton.h"

class CCBLoginLayer: public TouchLayer
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
, public CCNodeLoaderListener
, public cocos2d::extension::CCEditBoxDelegate
{
public:
    CCBLoginLayer();
    ~CCBLoginLayer();
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(CCBLoginLayer, create);
    
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode);
    virtual bool onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName,CCBValue* pCCBValue){return false;}
    virtual void onNodeLoaded(CCNode * pNode, CCNodeLoader * pNodeLoader);
    
    void loadLayer();
    
    virtual void editBoxEditingDidBegin(CCEditBox* editBox) { }
    virtual void editBoxReturn(CCEditBox* editBox)
    {
        if (editBox == m_pEditBoxUser)
        {
            m_pEditBoxUser->closeKeyboard();
            if (m_pEditBoxPwd) m_pEditBoxPwd->openKeyboard();
        }
    }
    
private:
    void checkBulletinOver(CCNode* pNode, void* pData);
    void logInError(CCObject* pTarget);

private:
    CCLabelTTF* m_pLblVersion;
    
    /* Normal Login */
private:
    CCNode* m_pNomalNode;
    
    CCControlButton* m_pBtnBegin;
    CCControlButton* m_pBtnRegister;
    
    CCSprite* m_pTicket;
    XYToggleButton* m_pBtnTicket;

    CCEditBox* m_pEditBoxUser;
    CCEditBox* m_pEditBoxPwd;
    
public:
    void onBegin(CCObject* pTarget, CCControlEvent event);
    void onRegister(CCObject* pTarget, CCControlEvent event);
    
    /* Tencent Platform Login */
private:
    CCNode* m_pTencentNode;
    CCControlButton* m_pBtnLoginWithQQ;
    CCControlButton* m_pBtnLoginWithWX;
public:
    void onLoginWithQQ(CCObject* pTarget, CCControlEvent event);
    void onLoginWithWX(CCObject* pTarget, CCControlEvent event);
};

class CCBLoginLayerLoader : public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CCBLoginLayerLoader, loader);
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CCBLoginLayer);
};

#endif /* defined(__DragonBattle__CCBLoginLayer__) */
