//
//  ItemSprite.h
//  DragonBattle
//
//  Created by 何 永翔 on 13-11-16.
//
//

#ifndef DragonBattle_ItemSprite_h
#define DragonBattle_ItemSprite_h

#include "XYBaseButton.h"

class ItemSprite:public XYBaseButton
{
public:
    ItemSprite()
    {
        itemID = 0;
        itemNum = 0;
    }
    static ItemSprite * create(const char * pFile,int nPriority = 0)
    {
        ItemSprite *pBtn = new ItemSprite();
        if (pBtn && pBtn->XYBaseTouchSprite::initWithFile(pFile,nPriority))
        {
            pBtn->autorelease();
            return pBtn;
        }
        CC_SAFE_DELETE(pBtn);
        return NULL;
    }
    int itemID;
    int itemNum;
};

#endif
