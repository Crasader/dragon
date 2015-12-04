//
//  XYLblButton.h
//  DragonBattle
//
//  Created by zeng xiaolong on 13-8-27.
//
//

#ifndef __DragonBattle__XYLblButton__
#define __DragonBattle__XYLblButton__
#include "XYBaseTouchSprite.h"
class XYLblButton:public XYBaseTouchSprite
{
public:
    
    XYLblButton();
    ~XYLblButton();
    static XYLblButton *create(const char *string, const char *fontName, float fontSize,int nPriority = 0);
    
    void setColor(ccColor3B color);
    void setString(const char* str);
    void setFontName(const char* fontname);
    void setFontSize(float fontSize);
    void setLine(bool hasline);
    void draw();
    
    virtual void touchBegin();
    virtual void touchEnd();
    
private:
    bool m_hasLine;
    virtual bool initWithLbl(const char *string, const char *fontName, float fontSize,int nPriority);
};

#endif /* defined(__DragonBattle__XYLblButton__) */
