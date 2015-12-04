//
//  DictGenerator.h
//  DragonBattle
//
//  Created by 白明江 on 1/17/14.
//
//

#ifndef __DragonBattle__DictGenerator__
#define __DragonBattle__DictGenerator__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class DictGenerator: public CCObject
{
private:
    CC_SYNTHESIZE_READONLY(CCDictionary*, m_pDictResult, DictionayResult);
    
public:
    DictGenerator();
    virtual ~DictGenerator();
    
    static CCDictionary* dictionaryFromJSON(const char* data, int size);
    static CCDictionary* dictionaryFromJSON(std::string data);
    static std::string dictionaryToJSON(CCDictionary *dict);
    
protected:
    void traverseJsonValue(const std::string &name, CSJson::Value &value, void *data, bool isDict);
    
private:
    bool parseFromJSON(std::string data);
    static bool parseToJSON(CSJson::Value &value, void *data, bool isDict);
};

#endif /* defined(__DragonBattle__DictGenerator__) */
