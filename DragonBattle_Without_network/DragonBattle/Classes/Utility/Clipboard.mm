//
//  Clipboard.cpp
//  DragonBattle
//
//  Created by 何 永翔 on 14-2-15.
//
//

#include "Clipboard.h"

void Clipboard::copy(const char* pStr)
{
    UIPasteboard *pasteboard = [UIPasteboard generalPasteboard];
    pasteboard.string = [[NSString alloc] initWithUTF8String:pStr];
}