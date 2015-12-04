//
//  LibAttrCal.h
//  DragonBattle
//
//  Created by 何 永翔 on 13-6-25.
//
//

#ifndef DragonBattle_LibAttrCal_h
#define DragonBattle_LibAttrCal_h

#include "EquipAttribute.h"

namespace attrcal
{
    class LibAttrCal
    {
    public:
        /*static float floor(float fValue)
        {
            if (fValue == (int)(fValue))
            {
                return fValue;
            }
            else
            {
                return fValue > 0 ? (int)fValue : ((int)fValue - 1);
            }
        }*/
        static float ceil(float fValue)
        {
            if (fValue == (int)(fValue))
            {
                return fValue;
            }
            else
            {
                return fValue > 0 ? ((int)fValue + 1) : (int)fValue;
            }
        }
    };
}

#endif
