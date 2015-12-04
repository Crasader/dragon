//
//  DirtyWordManager.cpp
//  iPetLand
//
//  Created by 小敦 王 on 13-2-1.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include <iostream>
using namespace std;

#include "DirtyWordManager.h"

DirtyWordManager* DirtyWordManager::pSharedManager = NULL;
DirtyWordManager::DirtyWordManager()
{
    
}

DirtyWordManager::~DirtyWordManager()
{
    
}

DirtyWordManager* DirtyWordManager::sharedManager()
{
    if (pSharedManager == NULL)
    {
        pSharedManager = new DirtyWordManager();
        pSharedManager->init();
    }
    return pSharedManager;
}


/**
 * @brief 去掉 s 中的空白符(空格/tab/换行)
 */
int trim_blank(char *s)
{
    int len;
    for (len = 0; s[len]; len++);
    
    char temp[256] = {0};
    memcpy(temp, s, len);
    
    int l = 0;
    for (int i = 0; i < len; i++)
    {
        if (temp[i] != ' ' && temp[i] != '\t' && temp[i] != '\n')
        {
            s[l++] = temp[i];
        }
    }
    s[l] = '\0';
    
    return 0;
}

const char dirtyWord[][256] = {"diaoyudao", "jiangeliedao", "钓鱼岛", "尖隔列岛", "尖阁列岛", "间隔列岛", "间阁列岛"};
#define DIRTY_WORD_CNT  (sizeof(dirtyWord) / sizeof(dirtyWord[0]))

bool DirtyWordManager::init()
{
    return true;
}

bool DirtyWordManager::isDirtyWord(const char* word)
{
    char buff[256] = {0};
    memcpy(buff, word, strlen(word));
    trim_blank(buff);
    string tmp = buff;
    
    for (int i = 0; i < DIRTY_WORD_CNT; i++)
    {
        int l = tmp.find(dirtyWord[i]);
        if (l != -1)
        {
            return true;
        }
    }
    
    return false;
}