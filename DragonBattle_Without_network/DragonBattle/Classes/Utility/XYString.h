//
//  XYString.h
//  DragonBattle
//
//  Created by 白明江 on 1/17/14.
//
//

#ifndef __DragonBattle__XYString__
#define __DragonBattle__XYString__
#include "cocos2d.h"

USING_NS_CC;

enum XYString_VALUETYPE {
    SVT_STRING,
    SVT_BOOLEAN,
    SVT_INTEGER,
    SVT_UINTEGER,
    SVT_FLOAT,
    SVT_DOUBLE
};

class XYString: public CCString {
    CC_SYNTHESIZE(int, m_iValueType, ValueType);
public:
    XYString(): m_iValueType(SVT_STRING) {}
    XYString(const char* str): CCString(str), m_iValueType(SVT_STRING) {}
    XYString(const std::string& str): CCString(str), m_iValueType(SVT_STRING) {}
    XYString(const CCString& str): CCString(str), m_iValueType(SVT_STRING) {}
    
    static XYString* create(const std::string& str);
    static XYString* createWithFormat(const char* format, ...);
    static XYString* createWithData(const unsigned char* pData, unsigned long nLen);
    static XYString* createWithContentsOfFile(const char* pszFileName);
    
    static XYString* createWithBoolean(bool value);
    static XYString* createWithInteger(int value);
    static XYString* createWithUInteger(unsigned int value);
    static XYString* createWithFloat(float value);
    static XYString* createWithDouble(double value);
private:
    bool initWithFormatAndValist(const char* format, va_list ap);
    
};

#endif /* defined(__DragonBattle__XYString__) */
