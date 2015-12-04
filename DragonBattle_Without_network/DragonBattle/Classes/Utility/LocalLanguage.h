//
//  LocalLanguage.h
//  TestGame
//
//  Created by 白明江 on 1/11/14.
//
//

#ifndef __TestGame__LocalLanguage__
#define __TestGame__LocalLanguage__

#include <iostream>
#include "cocos2d.h"
#include "xml_decode.h"
#include "XmlEncrypt.h"

USING_NS_CC;

class LocalString
{
public:
    LocalString();
    ~LocalString();
    
    bool loadFromFile(const char* filePath, bool bEncrypt = false);
    void unload();
    
    const char* getString(const char* key)
    {
        if (m_pStringValueMap->objectForKey(key))
        {
            return m_pStringValueMap->valueForKey(key)->getCString();
        }
        else
        {
            return NULL;
        }
    }
    
private:
    CCDictionary* m_pStringValueMap;
};

class LocalLanguage
{
private:
    static LocalString* s_pLocalString;
    
private:
    static std::string getFileWithCurLang();
public:
    static void loadLocalString();
    static void unLoadLocalString();
    static CCString* getLocalCCString(const char* key);
    static const char* getLocalString(const char* key);
};

#endif /* defined(__TestGame__LocalLanguage__) */
