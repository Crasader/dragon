//
//  TouchLayer.h
//  DragonBattle
//
//  Created by zeng xiaolong on 13-6-7.
//
//

#ifndef __DragonBattle__TouchLayer__
#define __DragonBattle__TouchLayer__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "GlobalDefine.h"

USING_NS_CC;
USING_NS_CC_EXT;
#define GRAYLAYER (301)

class GameManager;

class TouchLayer:public CCLayer
{
public:
    TouchLayer();
    ~TouchLayer();
    CREATE_FUNC(TouchLayer);
    virtual void setPrority(int priorty);
    virtual int getPrority();
    virtual void loadLayer(){};
    
    virtual void keyBackClicked();
    
    virtual void onEnter();
    virtual void onExit();
    virtual void removeFromParent();
private:
    int m_nPriority;
protected:
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
};

class GrayLayer:public TouchLayer
{
public:
    bool init()
    {
        if (CCLayer::init())
        {
            CCLayerColor *layer = CCLayerColor::create(ccc4(0, 0, 0, 200));
            addChild(layer,-1);
            //许翔：基准点左下方不会受颜色影响，故给个偏移量
            const int offset = 500;
            layer->setPosition(ccp(-offset, -offset));
            layer->setContentSize(CCSizeMake(SCREEN_WIDTH + 2 * offset, SCREEN_HEIGTH + 2 * offset));
            layer->setTag(GRAYLAYER);
            return true;
        }
        return false;
    };

};

#endif /* defined(__DragonBattle__TouchLayer__) */
