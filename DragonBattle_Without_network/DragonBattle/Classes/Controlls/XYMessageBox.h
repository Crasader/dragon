//
//  XYMessageBox.h
//  DragonBattle
//
//  Created by 小敦 王 on 13-8-29.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_XYMessageBox_h
#define DragonBattle_XYMessageBox_h

#include "CCBUtils.h"
#include "XYButton.h"
#include "GameManager.h"
#include "XYNumberView.h"
#define MESSAGE_BOX_PRORITY -128

class BaseMessageBox : public TouchLayer
{
public:
    BaseMessageBox();
    ~BaseMessageBox();
    
    /*
     * @brief 创建BaseMessageBox(只有1个或2个按钮)，属性设置为-128，放在TopLayer层
     * @prama
        title:     消息框标题
        msg:       消息框内容 (标题和内容在create之后，都不需要改动，所以都在创建的时候传入)
        hasTwoBtn:  是否有两个按钮
        pTarget:    用于回调的Object（pSelFunc1和pSelFunc2必须来自同一个Target）
        pSelFunc1:  按钮1的回调
        pSelFunc2:  按键2的回调
     * @example：
        BaseMessageBox* pMsgBox = BaseMessageBox::create("title", ".......", true, this, callfuncN_selector(CCClasee::onFunc), NULL);
        TOPADDCHILD((pMsgBox));
     */
    static BaseMessageBox* create(const char* title, const char* msg, bool hasTwoBtn, CCObject* pTarget, cocos2d::SEL_CallFuncN pSelFunc1, cocos2d::SEL_CallFuncN pSelFunc2 = NULL);
    
    virtual void onFunc1(CCNode* pNode);
    virtual void onFunc2(CCNode* pNode);
    
    virtual bool showMessageBox();
    
    // 第几个btn设置名称
    void setBtnText(int ntnIdx, const char* name);
    
    // 设置标题
    void setTitle(const char* title, const char* fontName = "Arial-BoldMT", float fontSize = 20);
    
    // 设置内容
    void setMsg(const char* msg, const char* fontName = "Arial-BoldMT", float fontSize = 18);
    
    void setMsgAlignment(CCTextAlignment hAligment);
    
    // 如需设置颜色、大小等属性，需要扩展
    
    void setColorMask(bool isColor)
    {
        if (m_pColorMask)
            m_pColorMask->setVisible(isColor);
    }
    
    void setOpacity(GLubyte opacity)
    {
        if (m_pColorMask)
        {
            m_pColorMask->setOpacity(opacity);
        }
    }
    
    virtual void onEnter()
    {
        CCLayer::onEnter();
    }
    
    virtual void onExit()
    {
        CCLayer::onExit();
    }
    
    CCLabelTTF* m_pLabelTitle;
    CCLabelTTF* m_pLabelMsg;
protected:
    bool init(const char* title, const char* msg, bool hasTwoBtn, CCObject* pTarget, cocos2d::SEL_CallFuncN pSelFunc1, cocos2d::SEL_CallFuncN pSelFunc2 = NULL);
    XYButton* createBtn(CCObject* target, cocos2d::SEL_CallFuncN selConfirm);
protected:
    CCScale9Sprite*   m_pBkgSprite;
    XYButton*   m_pBtn1;
    XYButton*   m_pBtn2;
    CCObject*   m_pTarget;
    cocos2d::SEL_CallFuncN m_pSelFunc1;
    cocos2d::SEL_CallFuncN m_pSelFunc2;
    
    CCLayerColor* m_pColorMask;
};


enum eXYMBType
{
    E_MB_OK = 1,
    E_MB_CANCEL,
    E_MB_OKCANCEL,
};

// XYMessageBox 只有确定和取消两种按键
class XYMessageBox : public BaseMessageBox
{
public:
    XYMessageBox();
    ~XYMessageBox();
    
    /*
     * @brief 创建MessageBox，属性设置为-128，放在TopLayer层
     * @prama nMBType:消息框类型，E_MB_OK只有确定按键，E_MB_CANCEL只有去取消 E_MB_OKCANCEL有确定和取消按键
     title:消息框标题
     msg:消息框内容 (标题和内容在create之后，都不需要改动，所以都在创建的时候传入)
     target: 用于回调的Object
     selConfirm: OK按钮的回调
     selCancel: Cancel按钮的回调
     * @example：
     XYMessageBox* pMsgBox = XYMessageBox::create(e_MB_OKCANCEL, "title", ".......", this, callfuncN_selector(CCBPrestigeTitleCell::onChallengeOK), NULL);
     TOPADDCHILD((pMsgBox));
     */
    static XYMessageBox* create(int nMBType, const char* title, const char* msg, CCObject* target, cocos2d::SEL_CallFuncN selConfirm, cocos2d::SEL_CallFuncN selCancel);
    
    bool init(int nMBType, const char* title, const char* msg, CCObject* target, cocos2d::SEL_CallFuncN selConfirm, cocos2d::SEL_CallFuncN selCancel);
    
    // 设置OK按钮的名称
    void setOKBtnText(const char* name);
    
    // 设置取消按钮的名称
    void setCancelBtnText(const char* name);
    
    virtual void onOK(CCNode* pNode);
    virtual void onCancel(CCNode* pNode);
    
private:
    int m_nBtnType;
};

class XYToggleButton;
class RemindMessageBox : public BaseMessageBox
{
public:
    RemindMessageBox();
    ~RemindMessageBox();
    
    static RemindMessageBox* create(int remindID, const char* msg, CCObject* target, cocos2d::SEL_CallFuncN selConfirm, cocos2d::SEL_CallFuncN selCancel);

    void onOK(CCNode* pNode);
    void onCancel(CCNode* pNode);
    
    bool showMessageBox();
    
    void setTipOff();
    
protected:
    bool init(int remindID, const char* msg, CCObject* target, cocos2d::SEL_CallFuncN selConfirm, cocos2d::SEL_CallFuncN selCancel);
    
    virtual void onFunc1(CCNode* pNode);

private:
    XYToggleButton* m_pToggleBtn;
    uint32_t        m_nRemindMsgID;
};

class BuyMessageBox : public BaseMessageBox
{
public:
    BuyMessageBox();
    ~BuyMessageBox();
    
    static BuyMessageBox* create(const char* msg, const char* icon, int nNum, int nCost, bool bIsGold, CCObject* target, cocos2d::SEL_CallFuncN selConfirm, cocos2d::SEL_CallFuncN selCancel);
    
    void onOK(CCNode* pNode);
    void onCancel(CCNode* pNode);
    
protected:
    bool init(const char* msg, const char* icon, int nNum, int nCost, bool bIsGold, CCObject* target, cocos2d::SEL_CallFuncN selConfirm, cocos2d::SEL_CallFuncN selCancel);
    
private:
    
};

class GoldGuideMessageBox:public BaseMessageBox
{
public:    
    static GoldGuideMessageBox* create(const char* msg)
    {
        GoldGuideMessageBox* pRet = new GoldGuideMessageBox();
        
        if (pRet && pRet->init(msg))
        {
            pRet->autorelease();
            return pRet;
        }
        else
        {
            delete pRet;
            pRet = NULL;
        }
        return pRet;
    }
    
    void onOK(CCNode* pNode)
    {
        GameManager::sharedInstance()->showRechargeLayer();
    }
    void onCancel(CCNode* pNode)
    {
        removeFromParent();
    }
    static void showGetGuideLayer(const char *msg)
    {
        GoldGuideMessageBox *box = GoldGuideMessageBox::create(msg);
        box->showMessageBox();
    }
protected:
    bool init(const char* msg)
    {
        if (!BaseMessageBox::init("", msg, true, this, callfuncN_selector(GoldGuideMessageBox::onOK)))
        {
            return false;
        }
        m_pLabelTitle->setString("提示");
        m_pLabelMsg->setString(msg);
        setBtnText(1,"充值");
         setBtnText(2,"取消");
        return true;
    }
};

class MallGuideMessageBox:public BaseMessageBox
{
public:
    static MallGuideMessageBox* create(const char* msg)
    {
        MallGuideMessageBox* pRet = new MallGuideMessageBox();
        
        if (pRet && pRet->init(msg))
        {
            pRet->autorelease();
            return pRet;
        }
        else
        {
            delete pRet;
            pRet = NULL;
        }
        return pRet;
    }
    
    void onOK(CCNode* pNode)
    {
        //GameManager::sharedInstance()->showLayerWithMenuType(kMenuStoreMenuTag);
    }
    void onCancel(CCNode* pNode)
    {
        removeFromParent();
    }
    static void showGetGuideLayer(const char *msg)
    {
        MallGuideMessageBox *box = MallGuideMessageBox::create(msg);
        box->setMsgAlignment(kCCTextAlignmentCenter);
        box->showMessageBox();
    }
protected:
    bool init(const char* msg, int nPage = 1)
    {
        if (!BaseMessageBox::init("", msg, true, this, callfuncN_selector(MallGuideMessageBox::onOK)))
        {
            return false;
        }
        m_pLabelTitle->setString("提示");
        m_pLabelMsg->setString(msg);
        setBtnText(1,"前往商城");
        setBtnText(2,"取消");
        return true;
    }
};

class RankAwardMessageBox:public BaseMessageBox
{
public:
    static RankAwardMessageBox* create(int nCopper,int nPower, CCObject* pTarget, cocos2d::SEL_CallFuncN pSelFunc1);
    bool init(int nCopper,int nPower, CCObject* pTarget, cocos2d::SEL_CallFuncN pSelFunc1);
    void onClose(CCNode* pNode);
};



class GoodsCountMessageBox : public BaseMessageBox,public XYNumberViewDelegate
{
private:
    CC_SYNTHESIZE(CCLabelTTF* ,m_pLblTotal,LblTotal);
    CC_SYNTHESIZE(int, m_iBuyCountLimit, BuyCountLimit);
    CC_SYNTHESIZE(int, m_iBuyCount, BuyCount);
    CC_SYNTHESIZE(int, m_iCost, Cost);
    CC_SYNTHESIZE(XYNumberViewDelegate*, m_pDelegete, Delegate);
public:
    GoodsCountMessageBox();
    virtual ~GoodsCountMessageBox();
    static GoodsCountMessageBox* create(const char* title, const char* msg, int nCost, int limitCount ,CCObject* target, cocos2d::SEL_CallFuncN selConfirm, cocos2d::SEL_CallFuncN selCancel);
    
    void onOK(CCNode* pNode);
    void onCancel(CCNode* pNode);
    
    void onReduceClick(CCNode* pNode);
    void onAddClick(CCNode* pNode);
    void numberChange(int index);
protected:
    XYNumberView *numberView;
    bool init(const char* title, const char* msg, int nCost , int limitCount , CCObject* target, cocos2d::SEL_CallFuncN selConfirm, cocos2d::SEL_CallFuncN selCancel);
};

class RechargeMessageBox:public BaseMessageBox
{
public:
    CREATE_FUNC(RechargeMessageBox);
    bool init();
    void onClose(CCNode* pNode);
    void onRecharge(CCNode* pNode);
};

#endif
