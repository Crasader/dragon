//
//  ColorLabelTTF.h
//  DragonBattle
//
//  Created by 白明江 on 10/24/13.
//
//

#ifndef __DragonBattle__ColorLabelTTF__
#define __DragonBattle__ColorLabelTTF__

#include "cocos2d.h"
#include "GlobalDefine.h"

USING_NS_CC;

class ColorLabelTTF : public CCNode {
public:
    
    /**
     *
     * 默认 分隔符 $
     *
     */
    
    static ColorLabelTTF* create()
    {
        std::vector<ccColor3B> colorVec;
        return create("", ccWHITE, colorVec, FONTNAME, 24);
    }
    
    static ColorLabelTTF* create(const char* str, ccColor3B defaultColor, std::vector<ccColor3B> colorVec, const char* font, int fontSize)
    {
        return create(str, "$", defaultColor, colorVec, font, fontSize);
    }
    
    static ColorLabelTTF* create(const char* str, const char* split, ccColor3B defaultColor, std::vector<ccColor3B> colorVec, const char* font, int fontSize);
    
    void setString(const char* str);
    void setStringWithNodeTag(const char* str, int tag);
    
    void setColor(ccColor3B defaultColor);
    void setColor(std::vector<ccColor3B> colorVec);
    void setColor(ccColor3B defaultColor, std::vector<ccColor3B> colorVec);
    void setColorWithNodeTag(ccColor3B color, int tag);
    
    void setFontSize(float size);
    void setFontSizeWithNodeTag(float size, int tag);
    void refresh();
    
private:
    bool init(const char* str, const char* token, ccColor3B defaultColor, std::vector<ccColor3B> colorVec, const char* font, int fontSize);
    
};

#endif /* defined(__DragonBattle__ColorLabelTTF__) */
