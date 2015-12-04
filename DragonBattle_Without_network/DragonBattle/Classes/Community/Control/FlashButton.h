//
//  FlashButton.h
//  DragonBattle
//
//  Created by 何 永翔 on 13-7-15.
//
//

#ifndef DragonBattle_FlashButton_h
#define DragonBattle_FlashButton_h

#include "XYBaseTouchSprite.h"

class FlashButton : public XYBaseTouchSprite
{
public:
    FlashButton();
    
    static FlashButton* create(const char* pBg, const char* pFlash1, const char* pFlash2, float nFlashSpeed, float nFlashInterval, int nPriority = 0);
    virtual bool initWithFile(const char* pBg, const char* pFlash1, const char* pFlash2, float nFlashSpeed, float nFlashInterval, int nPriority = 0);
};

#endif
