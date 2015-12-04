//
//  DirtyWordManager.h
//  iPetLand
//
//  Created by 小敦 王 on 13-2-1.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_DirtyWordManager_h
#define DragonBattle_DirtyWordManager_h

#include <vector>
#include <string>

class DirtyWordManager
{
public:
    DirtyWordManager();
    ~DirtyWordManager();
    
    bool init();
    
    static DirtyWordManager* sharedManager();
    bool isDirtyWord(const char* word);
    
private:
    static DirtyWordManager* pSharedManager;
};

#define checkDirtyWord(s) DirtyWordManager::sharedManager()->isDirtyWord(s)

#endif
