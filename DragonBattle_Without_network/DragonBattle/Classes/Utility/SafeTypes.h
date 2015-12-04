//
//  SafeTypes.h
//  LittleMason
//
//  Created by 吴 威 on 12-8-29.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
//  Description:    - Safe types are bit-processed built-in types, to avoid being modified through value-seeking in memory.
//                  - We'll implement it by circularly shifting 3 bit to the right.

#ifndef LittleMason_SafeTypes_h
#define LittleMason_SafeTypes_h

#include "GlobalDefine.h"

class SafeUint8
{
public:
    // constructor
    SafeUint8()
    {
        m_safeValue = 0;
    }
    
    // copy constructor
    SafeUint8(const uint8_t& src)
    {
        m_safeValue = (src >> 3) | ((src & 0x07) << 5);
    }
    
    SafeUint8(const int& src)
    {
        uint8_t temp = src;
        m_safeValue = (temp >> 3) | ((temp & 0x07) << 5);
    }
    
    // assignment constructor
    SafeUint8& operator=(uint8_t src)
    {
        m_safeValue = (src >> 3) | ((src & 0x07) << 5);
        return *this;
    }
    
    SafeUint8& operator=(const int& src)
    {
        uint8_t temp = src;
        m_safeValue = (temp >> 3) | ((temp & 0x07) << 5);
        return *this;
    }
    
    // type convertion
    operator uint8_t()
    {
        return (((m_safeValue & 0xE0) >> 5) | (m_safeValue << 3));
    }
    
    operator uint8_t() const
    {
        return (((m_safeValue & 0xE0) >> 5) | (m_safeValue << 3));
    }
    
    // write safe value to the file
    void writeSafeValue(FILE* pFile)
    {
        fwrite(&m_safeValue, sizeof(m_safeValue), 1, pFile);
    }
    
    // read safe value
    void readSafeValue(FILE* pFile)
    {
        fread(&m_safeValue, sizeof(m_safeValue), 1, pFile);
    }
    
    inline uint8_t  getSafeValue()              { return m_safeValue; }
    inline void     setSafeValue(uint8 value)   { m_safeValue = value; }

private:
    uint8_t   m_safeValue;
};

class SafeUint16
{
public:
    // constructor
    SafeUint16()
    {
        m_safeValue = 0;
    }
    
    // copy constructor
    SafeUint16(const uint16_t& src)
    {
        m_safeValue = (src >> 3) | ((src & 0x0007) << 13);
    }
    
    SafeUint16(const int& src)
    {
        uint16_t temp = src;
        m_safeValue = (temp >> 3) | ((temp & 0x0007) << 13);
    }
    
    // assignment constructor
    SafeUint16& operator=(uint16_t src)
    {
        m_safeValue = (src >> 3) | ((src & 0x0007) << 13);
        return *this;
    }
    
    SafeUint16& operator=(int src)
    {
        uint16_t temp = src;
        m_safeValue = (temp >> 3) | ((temp & 0x0007) << 13);
        return *this;
    }
    
    // type convertion
    operator uint16_t()
    {
        return (((m_safeValue & 0xE000) >> 13) | (m_safeValue << 3));
    }
    
    operator uint16_t() const
    {
        return (((m_safeValue & 0xE000) >> 13) | (m_safeValue << 3));
    }
    
    // write safe value to the file
    void writeSafeValue(FILE* pFile)
    {
        fwrite(&m_safeValue, sizeof(m_safeValue), 1, pFile);
    }
    
    // read safe value
    void readSafeValue(FILE* pFile)
    {
        fread(&m_safeValue, sizeof(m_safeValue), 1, pFile);
    }
    
    inline uint16_t getSafeValue()              { return m_safeValue;  }
    inline void     setSafeValue(uint16 value)  { m_safeValue = value; }

private:
    uint16_t  m_safeValue;
};

class SafeUint32
{
public:
    // constructor
    SafeUint32()
    {
        m_safeValue = 0;
    }
    
    // copy constructor
    SafeUint32(const uint32_t& src)
    {
        m_safeValue = (src >> 3) | ((src & 0x00000007) << 29);
    }
    
    SafeUint32(const int& src)
    {
        uint32_t temp = src;
        m_safeValue = (temp >> 3) | ((temp & 0x00000007) << 29);
    }
    
    // assignment constructor
    SafeUint32& operator=(uint32_t src)
    {
        m_safeValue = (src >> 3) | ((src & 0x00000007) << 29);
        return *this;
    }
    
    SafeUint32& operator=(int src)
    {
        uint32_t temp = src;
        m_safeValue = (temp >> 3) | ((temp & 0x00000007) << 29);
        return *this;
    }
    
    // type convertion
    operator uint32_t()
    {
        return (((m_safeValue & 0xE0000000) >> 29) | (m_safeValue << 3));
    }
    
    operator uint32_t() const
    {
        return (((m_safeValue & 0xE0000000) >> 29) | (m_safeValue << 3));
    }
    
    // write safe value to the file
    void writeSafeValue(FILE* pFile)
    {
        fwrite(&m_safeValue, sizeof(m_safeValue), 1, pFile);
    }
    
    // read safe value
    void readSafeValue(FILE* pFile)
    {
        fread(&m_safeValue, sizeof(m_safeValue), 1, pFile);
    }
    
    inline uint32_t getSafeValue()              { return m_safeValue; }
    inline void     setSafeValue(uint32 value)  { m_safeValue = value; }
    
private:
    uint32_t m_safeValue;
};

/*#ifdef COCOS2D_DEBUG
// unit test
void testSafeUint8()
{
    SafeUint8 test1 = 0;
    SafeUint8 test2 = 1;
    SafeUint8 test3 = 55;
    SafeUint8 test4 = 128;
    SafeUint8 test5 = 190;
    SafeUint8 test6 = 255;
    uint8 u1 = test1;
    uint8 u2 = test2;
    uint8 u3 = test3;
    uint8 u4 = test4;
    uint8 u5 = test5;
    uint8 u6 = test6;
    cocos2d::CCLog("SafeUint8 Test: %d-%d, %d-%d, %d-%d, %d-%d, %d-%d, %d-%d", \
          test1.getSafeValue(), u1, test2.getSafeValue(), u2, test3.getSafeValue(), u3, \
          test4.getSafeValue(), u4, test5.getSafeValue(), u5, test6.getSafeValue(), u6);
}

void testSafeUint16()
{
    SafeUint16 test1 = 0;
    SafeUint16 test2 = 1;
    SafeUint16 test3 = 256;
    SafeUint16 test4 = 32768;
    SafeUint16 test5 = 54321;
    SafeUint16 test6 = 65535;
    uint16 u1 = test1;
    uint16 u2 = test2;
    uint16 u3 = test3;
    uint16 u4 = test4;
    uint16 u5 = test5;
    uint16 u6 = test6;
    cocos2d::CCLog("SafeUint16 Test: %d-%d, %d-%d, %d-%d, %d-%d, %d-%d, %d-%d", \
          test1.getSafeValue(), u1, test2.getSafeValue(), u2, test3.getSafeValue(), u3, \
          test4.getSafeValue(), u4, test5.getSafeValue(), u5, test6.getSafeValue(), u6);
}

void testSafeUint32()
{
    SafeUint32 test1 = 0;
    SafeUint32 test2 = 1;
    SafeUint32 test3 = 4567;
    SafeUint32 test4 = 65536;
    SafeUint32 test5 = 248691746;
    SafeUint32 test6 = (int)4294967295;
    uint32 u1 = test1;
    uint32 u2 = test2;
    uint32 u3 = test3;
    uint32 u4 = test4;
    uint32 u5 = test5;
    uint32 u6 = test6;
    cocos2d::CCLog("SafeUint16 Test: %d-%d, %d-%d, %d-%d, %d-%d, %d-%d, %d-%u", \
          test1.getSafeValue(), u1, test2.getSafeValue(), u2, test3.getSafeValue(), u3, \
          test4.getSafeValue(), u4, test5.getSafeValue(), u5, test6.getSafeValue(), u6);
}
#endif*/

#endif
