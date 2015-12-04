//
//   BattleMap.h
//  DragonBattle
//
//  Created by zeng xiaolong on 13-12-3.
//
//

#ifndef __DragonBattle__BattleMap__
#define __DragonBattle__BattleMap__
#include "cocos2d.h"
#include "BattleSubBGNode.h"
USING_NS_CC;
class BattleMapUnits;
class BattleMap:public CCNode
{
public:
    BattleMap();
    ~BattleMap();
    static BattleMap *createWithID(int MapConfID,BattleMapUnits *pMapUnits);
    bool initWithID(int MapConfID,BattleMapUnits *pMapUnits);
    BattleSubBGNode*    m_pFarBg;
    BattleSubBGNode*    m_pMidBg;
    BattleSubBGNode*    m_pLandBG; //近景层背景
    BattleSubBGNode*    m_pNearBg;
    BattleMapUnits*     m_pMapUnits;
    float getBattleMapLength();
    void  resetBackGroud(int fMapLength);
    void  recreateBackGround(BattleSubBGNode* bg,float speedX);
    bool                m_bMarquee;//跑马灯
    int i;
    bool directY;
};

#endif /* defined(__DragonBattle___BattleMap__) */
