//
//  XYRichText.h
//  planegame
//
//  Created by xingchong on 3/3/16.
//
//

#ifndef __planegame__XYRichText__
#define __planegame__XYRichText__

#include <stdio.h>
#include "UIRichText.h"

USING_NS_CC;

/*
 <size=15></size>
 <fontname=“Arial”></fontname>
 <outline=2 color = 0xFFFFFFFF></outline>
 <shadow></shadow>
 <link=“”></link>
 <img=“”>
 <color=0XFFFFFFFF></color>
 <u=0xFF000000></u>
 */

class XYRichElement : public ui::RichElement
{
public:
    XYRichElement(){};
    virtual ~XYRichElement(){};
    bool init(int tag, const Color3B& color, GLubyte opacity);
    friend class XYRichText;
};

class XYRichElementText : public XYRichElement
{
public:
    XYRichElementText();
    ~XYRichElementText();
    bool init(const std::string& text, const std::string& fontName, float fontSize);
    static XYRichElementText* create(const std::string& text, const std::string& fontName, float fontSize);
    
protected:
    CC_SYNTHESIZE(std::string,_text,Text);
    CC_SYNTHESIZE(std::string,_fontName,FontName);
    CC_SYNTHESIZE(float,_fontSize,FontSize);
    CC_SYNTHESIZE(Color4B,_textColor,TextColor);
    CC_SYNTHESIZE(Color3B, _color, Color);
    CC_SYNTHESIZE(GLubyte,_opacity,Opacity);
    CC_SYNTHESIZE(int,_outLine,OutLine);
    CC_SYNTHESIZE(Color4B,_outLineColor,OutLineColor);
    CC_SYNTHESIZE(bool,_shadow,Shadow);
    CC_SYNTHESIZE(Color4B,_underLinecolor,UnderLineColor);
    CC_SYNTHESIZE(int,_underLinesize,UnderLineSize);
    CC_SYNTHESIZE_READONLY(std::string,_linkurl,LinkUrl);
    CC_SYNTHESIZE_READONLY(std::function<void (std::string)>, _touchCallback, TouchCallBack);
    
private:
    friend class XYRichText;
};

//the image element
class XYRichElementImage : public XYRichElement
{
public:
    XYRichElementImage();
    ~XYRichElementImage();
    bool init(const std::string& filePath);
    static XYRichElementImage* create(const std::string& filePath);
    
private:
    std::string _filePath;
    Rect _textureRect;
    int  _textureType;
    
    friend class XYRichText;
};

//the custom node element
class XYRichElementCustomNode : public XYRichElement
{
public:
    XYRichElementCustomNode();
    ~XYRichElementCustomNode();
    
    bool init(Node* customNode);
    static XYRichElementCustomNode* create(Node* customNode);
    
private:
    Node *_customNode;
    
    friend class XYRichText;
};


class XYRichText : public ui::RichText
{
public:
    XYRichText();
    ~XYRichText();
    
    static XYRichText* create(std::string str,const std::string& fontFile, float fontSize,const Size& size);
    bool init(std::string str,const std::string& fontFile, float fontSize,const Size& size);
    bool init() override;
    
private:
    ui::RichElement* createWithSet(const std::string& text);
    void handleTextRenderer(const XYRichElementText& textInfo);
    void formatText();
    void visit(cocos2d::Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags) override;
    void formarRenderers();
    
private:
    static float _set_fontSize;
    static std::string _set_fontFile;
    static int _set_outline;
    static Color4B _set_outline_color;
    static bool _set_shadow;
    static std::string _set_link;
    static Color4B _set_textColor;
    static bool _set_underline;
    static Color4B _set_underline_color;
    
};

#endif /* defined(__planegame__XYRichText__) */
