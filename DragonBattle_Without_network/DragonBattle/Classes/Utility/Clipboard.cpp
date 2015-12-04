//
//  Clipboard.cpp
//  DragonBattle
//
//  Created by 何 永翔 on 14-2-15.
//
//

#include "Clipboard.h"
#include "native_jni.h"

void Clipboard::copy(const char* pStr)
{
    writeToClipboard(pStr);
}
