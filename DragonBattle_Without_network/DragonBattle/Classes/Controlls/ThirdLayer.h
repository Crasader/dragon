//
//  ThirdLayer.h
//  DragonBattle
//
//  Created by zeng xiaolong on 13-6-17.
//
//

#ifndef __DragonBattle__ThirdLayer__
#define __DragonBattle__ThirdLayer__
#include "cocos2d.h"
USING_NS_CC;

class ThirdLayer:public CCLayer
{
public:
    ThirdLayer();
    ~ThirdLayer();
    virtual void setPrority(int priorty);
    virtual int getPrority();
    virtual void loadLayer(){};
private:
    int m_nPriority;
protected:
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
};


#endif /* defined(__DragonBattle__ThirdLayer__) */
