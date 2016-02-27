//
//  BattleUnitSprite.h
//  dragon
//
//  Created by xingchong on 2/18/16.
//
//

#ifndef __dragon__BattleUnitSprite__
#define __dragon__BattleUnitSprite__

#include <stdio.h>
#include "MapSprite.h"

class BattleUnitSprite : public MapSprite
{
public:
    BattleUnitSprite();
    ~BattleUnitSprite();
    
public:
    virtual bool init() override;
    virtual void destroy() override;
    virtual void initData(int guid, int configId);
    virtual void initView();
    
private:
    ValueMap _dataDic;
    std::function<void()> _onLandCallback;
    std::function<void()> _offLandCallback;
};

#endif /* defined(__dragon__BattleUnitSprite__) */
