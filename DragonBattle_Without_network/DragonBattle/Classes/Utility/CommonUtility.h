//
//  CommonUtility.h
//  DragonBattle
//
//  Created by 永翔 何 on 13-5-23.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonBattle_CommonUtility_h
#define DragonBattle_CommonUtility_h

#include <iostream>
#include <vector>
#include <map>
#include "UserData.h"
#include "cocos-ext.h"

int   get_hex(char ch);

char* hex2str(const char* hex_str, char* str);

typedef std::vector<std::string> strArray;
void split(std::string src, const char* token, strArray& vect);

class XYButton;
class ItemSprite;

class CommonUtility
{
public:
    static std::string& trim(std::string& str);
    
    static const long getCurrentMillisecond();
    static std::string getStringByLength(std::string& str, int length);
    static const int getUTF8Length(const char* string);
    static const char* getUTF8Substring(const char* string, int start, int length);
    
    static void saveMessage(const char* pPrefix, int nServerId, int nUserId, int nIndex, messageBody_t& body);
    static void insertMessage(std::map<int, messageBody>& messageMap, const char* pPrefix, int nServerId, int nUserId, messageBody& t, int nLimit);
    static char* getTalentString(char* pStr, int nTalent, float fTalentValue);
    
    static void setButtonVisible(cocos2d::extension::CCControlButton* pOriginBtn, bool bIsVisible);
    static void setButtonEnableState(cocos2d::extension::CCControlButton* pOriginBtn, bool bIsEnabled, const char* pBgFileName, int nFontSize, CCObject* target, cocos2d::extension::SEL_CCControlHandler action);
    static cocos2d::extension::CCControlButton* setDisabledButton(cocos2d::extension::CCControlButton* pOriginBtn, const char* pBgFileName, int nFontSize, CCObject* target, cocos2d::extension::SEL_CCControlHandler action);
    static void setBtnState(cocos2d::extension::CCControlButton* pBtn, CCRect rect, bool bEnable);
    static CCArray* getTipArray(std::vector<item_unit_t>& vec);
    static void setButtonGrayBg(XYButton* btn, bool enable, const char* normalfile, const char* grayFile);
    
    static ItemSprite* createButton(int nItemId, int nItemCount, CCObject *obj, SEL_CallFuncN callfun, bool bIsFlash, bool bShowName, int nPriority);
    static void createButtonEffect(CCNode* pNode, CCRect rect);
    
private:

};

#endif
