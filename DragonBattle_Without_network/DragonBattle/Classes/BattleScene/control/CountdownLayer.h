//
//  CountdownLayer.h
//  DragonBattle
//
//  Created by zeng xiaolong on 13-11-1.
//
//

#ifndef __DragonBattle__CountdownLayer__
#define __DragonBattle__CountdownLayer__
#include "GlobalDefine.h"
#include "TouchLayer.h"
class CountdownLayer:public TouchLayer
{
private:
    CCObject* m_pTarget;
    SEL_CallFunc m_pBattleCbk;
public:
    CountdownLayer()
    {
        m_pTarget = NULL;
        m_pBattleCbk = NULL;
    }
    CREATE_FUNC(CountdownLayer);
    static CountdownLayer* create(CCObject* target, SEL_CallFunc cb)
    {
        CountdownLayer* ret = new CountdownLayer;
        ret->setBattleCbk(target, cb);
        if (ret->init())
        {
            ret->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(ret);
        }
        
        return ret;
    }
    void setBattleCbk(CCObject* target, SEL_CallFunc cbk)
    {
        m_pTarget = target;
        m_pBattleCbk = cbk;
    }
    int count;
    bool init()
    {
        if (!CCLayer::init())
        {
            return false;
        }
        setPrority(-10);
        count = 3;
        CCSprite *lbl = CCSprite::create("BattleUI/3.png");
        lbl->setPosition(ccpAdd(SCREEN_MID_POS, ccp(0, 20.f)));
        addChild(lbl);
        lbl->setScale(2);
        lbl->runAction(CCSequence::create(CCEaseExponentialIn::create(CCScaleTo::create(0.5, 1)),CCCallFuncN::create(this, callfuncN_selector(CountdownLayer::onCountTime)),NULL));
        return true;
    };
    void onCountTime(CCNode *node)
    {
       count--;
        if (count >0)
        {
            CCSprite *spr = (CCSprite*)node;
            char str[64] = {0};
            sprintf(str, "BattleUI/%d.png",count);
            CCSprite *pic = CCSprite::create(str);
            spr->setDisplayFrame(pic->displayFrame());
            spr->setScale(4);
            spr->runAction(CCSequence::create(CCEaseExponentialIn::create(CCScaleTo::create(0.4, 1)), CCDelayTime::create(0.4), CCCallFuncN::create(this, callfuncN_selector(CountdownLayer::onCountTime)),NULL));
        }
        else
        {
            CCSprite *spr = (CCSprite*)node;
            CCSprite *pic = CCSprite::create("BattleUI/start.png");
            spr->setDisplayFrame(pic->displayFrame());
            spr->setScale(0.2);
            CCCallFunc* cbkAct = NULL;
            if (m_pTarget && m_pBattleCbk)
            {
                cbkAct = CCCallFunc::create(m_pTarget, m_pBattleCbk);
                spr->runAction(CCSequence::create(CCEaseExponentialIn::create(CCScaleTo::create(0.4, 1)), CCDelayTime::create(0.4), cbkAct, CCCallFuncN::create(this,  callfuncN_selector(CountdownLayer::onRemoveSelf)), NULL));
            }
            else
            {
                spr->runAction(CCSequence::create(CCEaseExponentialIn::create(CCScaleTo::create(0.4, 1)), CCDelayTime::create(0.4), CCCallFuncN::create(this, callfuncN_selector(CountdownLayer::onRemoveSelf)), NULL));
            }
        }
    }
    void onRemoveSelf(CCNode *node)
    {
        removeFromParent();
    }
};

#endif /* defined(__DragonBattle__CountdownLayer__) */
