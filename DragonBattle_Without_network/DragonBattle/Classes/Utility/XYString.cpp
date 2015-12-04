//
//  XYString.cpp
//  DragonBattle
//
//  Created by 白明江 on 1/17/14.
//
//

#include "XYString.h"


XYString* XYString::create(const std::string& str) {
    XYString* pRet = new XYString(str);
    pRet->autorelease();
    return pRet;
}

XYString* XYString::createWithFormat(const char* format, ...) {
    XYString* pRet = XYString::create("");
    va_list ap;
    va_start(ap, format);
    pRet->initWithFormatAndValist(format, ap);
    va_end(ap);
    
    return pRet;
}

XYString* XYString::createWithData(const unsigned char* pData, unsigned long nLen) {
    XYString* pRet = NULL;
    if (pData != NULL)
    {
        char* pStr = (char*)malloc(nLen+1);
        if (pStr != NULL)
        {
            pStr[nLen] = '\0';
            if (nLen > 0)
            {
                memcpy(pStr, pData, nLen);
            }
            
            pRet = XYString::create(pStr);
            free(pStr);
        }
    }
    return pRet;
}

XYString* XYString::createWithContentsOfFile(const char* pszFileName) {
    unsigned long size = 0;
    unsigned char* pData = 0;
    XYString* pRet = NULL;
    pData = CCFileUtils::sharedFileUtils()->getFileData(pszFileName, "rb", &size);
    pRet = XYString::createWithData(pData, size);
    CC_SAFE_DELETE_ARRAY(pData);
    return pRet;
}

XYString* XYString::createWithBoolean(bool value) {
    char pStr[64];
    strcpy(pStr, value ? "true" : "false");
    XYString* pRet = XYString::create(pStr);
    pRet->setValueType(SVT_BOOLEAN);
    return pRet;
}

XYString* XYString::createWithInteger(int value) {
    char pStr[64];
    sprintf(pStr, "%d", value);
    XYString* pRet = XYString::create(pStr);
    pRet->setValueType(SVT_INTEGER);
    return pRet;
}

XYString* XYString::createWithUInteger(unsigned int value) {
    char pStr[64];
    sprintf(pStr, "%d", value);
    XYString* pRet = XYString::create(pStr);
    pRet->setValueType(SVT_UINTEGER);
    return pRet;
}

XYString* XYString::createWithFloat(float value) {
    char pStr[64];
    sprintf(pStr, "%f", value);
    XYString* pRet = XYString::create(pStr);
    pRet->setValueType(SVT_FLOAT);
    return pRet;
}

XYString* XYString::createWithDouble(double value) {
    char pStr[64];
    sprintf(pStr, "%f", value);
    XYString* pRet = XYString::create(pStr);
    pRet->setValueType(SVT_DOUBLE);
    return pRet;
}


bool XYString::initWithFormatAndValist(const char* format, va_list ap) {
    int kMaxStringLen = (1024*32);
    
    bool bRet = false;
    char* pBuf = (char*)malloc(kMaxStringLen);
    if (pBuf != NULL) {
        vsnprintf(pBuf, kMaxStringLen, format, ap);
        m_sString = pBuf;
        free(pBuf);
        bRet = true;
    }
    return bRet;
}
