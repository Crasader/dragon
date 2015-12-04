//
//  DictGenerator.cpp
//  DragonBattle
//
//  Created by 白明江 on 1/17/14.
//
//

#include "DictGenerator.h"
#include "XYString.h"

DictGenerator::DictGenerator(): CCObject(), m_pDictResult(NULL)
{
    
}

DictGenerator::~DictGenerator()
{
    m_pDictResult = NULL;
}

CCDictionary* DictGenerator::dictionaryFromJSON(const char* data, int size)
{
    if (size < 1) size = strlen(data);
    std::string pText((char*)data, 0, size);
    
    return dictionaryFromJSON(pText);
}

CCDictionary* DictGenerator::dictionaryFromJSON(std::string data)
{
    DictGenerator generator;
    if (!generator.parseFromJSON(data))
    {
        return NULL;
    }
    
    return generator.getDictionayResult();
}

bool DictGenerator::parseToJSON(CSJson::Value &value, void *data, bool isDict)
{
    if (isDict) {
        CCDictionary *dict = (CCDictionary *)data;
        CCArray *keys = dict->allKeys();
        
        int count = dict->count();
        if (count < 1) return false;
        
        bool keyIsString = dynamic_cast<CCString *>(keys->objectAtIndex(0)) != NULL;
        
        for (int i = 0; i < count; ++i)
        {
            CCString *key = keyIsString ? (CCString *)keys->objectAtIndex(i)
                                        : XYString::createWithInteger(((CCInteger *)keys->objectAtIndex(i))->getValue());
            
			if(!key) continue;
            
            CCObject *content = keyIsString ? (CCObject *)dict->objectForKey(key->m_sString)
                                            : (CCObject *)dict->objectForKey(((CCInteger *)keys->objectAtIndex(i))->getValue());
            
            if (dynamic_cast<CCDictionary *>(content))
            {
                CSJson::Value obj(CSJson::objectValue);
                parseToJSON(obj, content, true);
                
                value[key->m_sString] = obj;
            }
            else if (dynamic_cast<CCArray *>(content))
            {
                CSJson::Value obj(CSJson::arrayValue);
                parseToJSON(obj, content, false);
                
                value[key->m_sString] = obj;
            }
            else if (dynamic_cast<XYString *>(content))
            {
                XYString *str = (XYString *)content;
                switch (str->getValueType())
                {
                    case SVT_BOOLEAN:
                        value[key->m_sString] = str->boolValue();
                        break;
                    case SVT_INTEGER:
                        value[key->m_sString] = str->intValue();
                        break;
                    case SVT_UINTEGER:
                        value[key->m_sString] = str->uintValue();
                        break;
                    case SVT_FLOAT:
                        value[key->m_sString] = str->floatValue();
                        break;
                    case SVT_DOUBLE:
                        value[key->m_sString] = str->doubleValue();
                        break;
                    default:
                        value[key->m_sString] = str->m_sString;
                        break;
                }
            }
        }
    }
    else
    {
        CCArray *array = (CCArray *)data;
        
        int count = array->count();
        if (count < 1) return false;
        
        int index = 0;
        for (int i = 0; i < count; ++i)
        {
            CCObject *content = (CCObject *)array->objectAtIndex(i);
            if (dynamic_cast<CCDictionary *>(content))
            {
                CSJson::Value obj(CSJson::objectValue);
                parseToJSON(obj, content, true);
                value[index++] = obj;
            }
            else if (dynamic_cast<CCArray *>(content))
            {
                CSJson::Value obj(CSJson::arrayValue);
                parseToJSON(obj, content, false);
                value[index++] = obj;
            }
            else if (dynamic_cast<XYString *>(content))
            {
                XYString *str = (XYString *)content;
                switch (str->getValueType())
                {
                    case SVT_BOOLEAN:
                        value[index++] = str->boolValue();
                        break;
                    case SVT_INTEGER:
                        value[index++] = str->intValue();
                        break;
                    case SVT_UINTEGER:
                        value[index++] = str->uintValue();
                        break;
                    case SVT_FLOAT:
                        value[index++] = str->floatValue();
                        break;
                    case SVT_DOUBLE:
                        value[index++] = str->doubleValue();
                        break;
                    default:
                        value[index++] = str->m_sString;
                        break;
                }
            }
        }
    }
    return true;
}

std::string DictGenerator::dictionaryToJSON(CCDictionary *dict)
{
    CSJson::Value root(CSJson::objectValue);
    
    if (!parseToJSON(root, dict, true)) return "";
    
    return root.toStyledString();
}


bool DictGenerator::parseFromJSON(std::string pText)
{

    m_pDictResult = NULL;
    CSJson::Reader reader;
    CSJson::Value value;
    
    if (reader.parse(pText, value))
    {
        if  (value.type() == CSJson::objectValue)
        {
            m_pDictResult = CCDictionary::create();

            CSJson::Value::Members members(value.getMemberNames());
            for (CSJson::Value::Members::iterator it = members.begin(); it != members.end(); ++it)
            {
                const std::string &name = *it;
                traverseJsonValue(name, value[name], m_pDictResult, true);
            }
        }
    }
    return true;
}

void DictGenerator::traverseJsonValue(const std::string &name, CSJson::Value &value, void *data, bool isDict)
{

    switch (value.type())
    {
        case CSJson::objectValue:
        {
            CCDictionary *dict = CCDictionary::create();
            if (isDict)
            {
                CCDictionary *lastDict = (CCDictionary *)data;

                lastDict->setObject(dict, name);
            }
            else
            {
                CCArray *lastArray = (CCArray *)data;

                lastArray->addObject(dict);
            }
            
            CSJson::Value::Members members(value.getMemberNames());

            for (CSJson::Value::Members::iterator it = members.begin(); it != members.end(); ++it)
            {
                const std::string &name = *it;
                traverseJsonValue(name, value[name], dict, true);
            }
        }
            break;
        case CSJson::arrayValue:
        {

            CCArray *array = CCArray::create();
            if (isDict)
            {
                CCDictionary *lastDict = (CCDictionary *)data;

                lastDict->setObject(array, name);
            }
            else
            {
                CCArray *lastArray = (CCArray *)data;

                lastArray->addObject(array);
            }
            
            int size = value.size();

            for (int i = 0; i < size; ++i)
            {
                traverseJsonValue("", value[i], array, false);
            }
        }
            break;
        default: {

            XYString *obj = NULL;//XYString::create(value.asString());
            switch (value.type())
            {
                case CSJson::intValue:
                    obj = XYString::createWithInteger(value.asInt());
                    break;
                case CSJson::uintValue:
                    obj = XYString::createWithUInteger(value.asUInt());
                    break;
                case CSJson::realValue:
                    obj = XYString::createWithFloat(value.asDouble());
                    break;
                case CSJson::booleanValue:
                    obj = XYString::createWithBoolean(value.asBool());
                    break;
                case CSJson::stringValue:
                    obj = XYString::create(value.asString());
                    break;
                default:
                    break;
            }
            
            if (obj)
            {
                if (isDict)
                {
                    CCDictionary *dict = (CCDictionary *)data;
                    dict->setObject(obj, name);
                }
                else
                {
                    CCArray *array = (CCArray *)data;
                    array->addObject(obj);
                }
            }
        }
            break;
    }
}