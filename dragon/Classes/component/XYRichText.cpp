//
//  XYRichText.cpp
//  planegame
//
//  Created by xingchong on 3/3/16.
//
//

#include "XYRichText.h"
#include "2d/CCSprite.h"
#include "CCLabel.h"
#include "CCLayer.h"
#include "platform/CCFileUtils.h"

#define UTF8_ASCII(byte) (((unsigned char)(byte)>=0x00)&&((unsigned char)(byte)<=0x7F))
#define UTF8_FIRST(byte) (((unsigned char)(byte)>=0xC0)&&((unsigned char)(byte)<=0xFD))
#define UTF8_OTHER(byte) (((unsigned char)(byte)>=0x80)&&((unsigned char)(byte)<=0xBF))

#define DEFAULT_OUTLINE -1
#define DEFAULT_OUTLINE_COLOR (Color4B(0, 0, 0, 0))
#define DEFAULT_COLOR Color4B::WHITE
#define DEFAULT_UNDERLINE false
#define DEFAULT_UNDERLINE_COLOR (Color4B(0, 0, 0, 0))

int XYRichText::_set_outline = DEFAULT_OUTLINE;
Color4B XYRichText::_set_outline_color = DEFAULT_OUTLINE_COLOR;
bool XYRichText::_set_shadow = false;
bool XYRichText::_set_underline = DEFAULT_UNDERLINE;
Color4B XYRichText::_set_underline_color = DEFAULT_UNDERLINE_COLOR;
Color4B XYRichText::_set_textColor = DEFAULT_COLOR;
std::string XYRichText::_set_fontFile;
float XYRichText::_set_fontSize;
std::string XYRichText::_set_link;

static const char* keywords[] = {"size","fontname","outline","shadow","link","img","color","u"};

static Color4B int2ccc3(unsigned long color)
{
    Color4B ret;
    ret.r = (color&0xffffffff)>>24;
    ret.g = (color&0xffffff)>>16;
    ret.b = (color&0xffff)>>8;
    ret.a = color&0xff;
    return ret;
}

static int _calcCharCount(const char * pszText,int len)
{
    char *p = 0;
    long count = 0;
    
    if (!pszText || len <= 0) {
        return 0;
    }
    
    for(p=(char*)pszText; p<pszText+len; p++) {
        if (UTF8_ASCII(*p) || (UTF8_FIRST(*p))) {
            count++;
        }
    }
    
    return count;
}

static std::string utf8_substr(const std::string& str, unsigned long start, unsigned long leng)
{
    if (leng==0)
    {
        return "";
    }
    unsigned long c, i, ix, q, min=std::string::npos, max=std::string::npos;
    for (q=0, i=0, ix=str.length(); i < ix; i++, q++)
    {
        if (q==start)
        {
            min = i;
        }
        if (q <= start+leng || leng==std::string::npos)
        {
            max = i;
        }
        
        c = (unsigned char) str[i];
        
        if      (c<=127) i+=0;
        else if ((c & 0xE0) == 0xC0) i+=1;
        else if ((c & 0xF0) == 0xE0) i+=2;
        else if ((c & 0xF8) == 0xF0) i+=3;
        else return "";//invalid utf8
    }
    if (q <= start+leng || leng == std::string::npos)
    {
        max = i;
    }
    if (min==std::string::npos || max==std::string::npos)
    {
        return "";
    }
    return str.substr(min,max);
}


//XYRichElement define
bool XYRichElement::init(int tag, const cocos2d::Color3B &color, GLubyte opacity)
{
    bool ret = ui::RichElement::init(tag, color, opacity);
    return ret;
}

//element define start
XYRichElementText::XYRichElementText()
{
    _type = Type::TEXT;
    _color = Color3B::WHITE;
    _opacity = 255;
    _text = "";
    _fontSize = 0;
    _fontName = "";
    _textColor = Color4B::WHITE;
    _outLine = -1;
    _outLineColor = Color4B::BLACK;
    _shadow = false;
    _linkurl = "";
    _underLinecolor = Color4B(0,0,0,0);
    _underLinesize = -1;
    _touchCallback = nullptr;
}

XYRichElementText::~XYRichElementText()
{
    
}

bool XYRichElementText::init(const std::string &text, const std::string &fontName, float fontSize)
{
    bool ret = XYRichElement::init(1, Color3B::WHITE, 255);
    if (ret)
    {
        _text = text;
        _fontName = fontName;
        _fontSize = fontSize;
        return true;
    }
    return false;
}

XYRichElementText* XYRichElementText::create(const std::string &text, const std::string &fontName, float fontSize)
{
    XYRichElementText *ret = new XYRichElementText();
    if(ret && ret->init(text, fontName, fontSize))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


XYRichElementImage::XYRichElementImage()
{
    _type = Type::IMAGE;
    _tag = -1;
    _color = Color3B::WHITE;
    _opacity = 255;
}

XYRichElementImage::~XYRichElementImage()
{
    
}

bool XYRichElementImage::init(const std::string &filePath)
{
    if (XYRichElement::init(1, Color3B::WHITE, 255))
    {
        _filePath = filePath;
        return true;
    }
    return false;
}

XYRichElementImage* XYRichElementImage::create(const std::string &filePath)
{
    XYRichElementImage *ret = new XYRichElementImage();
    if (ret && ret->init(filePath))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}



XYRichElementCustomNode::XYRichElementCustomNode()
{
    _type = Type::CUSTOM;
    _customNode = nullptr;
}

XYRichElementCustomNode::~XYRichElementCustomNode()
{
    CC_SAFE_RELEASE(_customNode);
}

bool XYRichElementCustomNode::init(cocos2d::Node *customNode)
{
    if (XYRichElement::init(1, Color3B::WHITE, 255))
    {
        _customNode = customNode;
        _customNode->retain();
        return true;
    }
    return  false;
}

XYRichElementCustomNode* XYRichElementCustomNode::create(cocos2d::Node *customNode)
{
    XYRichElementCustomNode *ret = new XYRichElementCustomNode();
    if (ret && ret->init(customNode))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

//element define end


XYRichText::XYRichText()
{

}

XYRichText::~XYRichText()
{
    
}

XYRichText* XYRichText::create(std::string str, const std::string &fontFile, float fontSize, const cocos2d::Size &size)
{
    XYRichText *ret = new XYRichText();
    if (ret && ret->init(str, fontFile, fontSize, size))
//    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

bool XYRichText::init()
{
    bool ret = ui::RichText::init();
    
//    ui::RichElementText *text = ui::RichElementText::create(1, Color3B::WHITE, 255, "the text", "cocostudio/ui/fonts/msyh.ttf", 32);
//    this->pushBackElement(text);
    
    return ret;
}

bool XYRichText::init(std::string str, const std::string &fontFile, float fontSize, const cocos2d::Size &size)
{
    if (!Widget::init())
    {
        return false;
    }
    ignoreContentAdaptWithSize(false);
    //setContentSize(size);
    setSize(size);
    
    _set_fontSize = fontSize;
    _set_fontFile = fontFile;
    _set_textColor = DEFAULT_COLOR;
    
    std::string s = str;
    unsigned long posStart = 0;
    unsigned long posEnd = 0;
    
    while (posStart<s.length())
    {
        bool isEnd = false;
        posEnd = s.find("<",posStart);
        
        if (posStart!=posEnd)
        {
            std::string tempStr = s.substr(posStart,posEnd-posStart);
            std::string::value_type pos = tempStr.find("\n");
            if (pos!=std::string::npos)
            {
                std::string s1 = tempStr.substr(0,pos).c_str();
                if (!s1.empty())
                {
                    pushBackElement(createWithSet(s1));
                }
                
                pushBackElement(createWithSet(""));
                std::string s2 = tempStr.substr(pos+1).c_str();
                if (!s2.empty())
                {
                    pushBackElement(createWithSet(s2));
                }
                
            }
            else
            {
                CCLOG("%s",tempStr.c_str());
                pushBackElement(createWithSet(tempStr));
            }
            
            if (posEnd==std::string::npos)
            {
                break;
            }
            
        }
        
        posStart = posEnd+1;
        CCLOG("%c",s.at(posStart));
        if (s.at(posStart)=='/')
        {
            isEnd = true;
            posStart++;
        }
        
        int keyIndex = 0;
        for (keyIndex=0; keyIndex<8; keyIndex++)
        {
            if(s.compare(posStart, strlen(keywords[keyIndex]), keywords[keyIndex])==0)
            {
                break;
            }
        }
        
        if (keyIndex<8)
        {
            switch (keyIndex)
            {
                case 0:
                {
                    posEnd = s.find(">",posStart);
                    if (isEnd)
                    {
                        CCLOG("size end");
                        _set_fontSize = fontSize;
                    }
                    else
                    {
                        posStart = s.find("=",posStart)+1;
                        int size = atoi(s.substr(posStart,posEnd-posStart).c_str());
                        _set_fontSize = size;
                        CCLOG("%d",size);
                    }
                }
                    break;
                case 1:
                {
                    posEnd = s.find(">",posStart);
                    if (isEnd)
                    {
                        _set_fontFile = fontFile;
                        CCLOG("fontname end");
                    }
                    else
                    {
                        posStart = s.find("=",posStart)+1;
                        std::string temp = s.substr(posStart,posEnd-posStart);
                        std::string::value_type p1,p2;
                        p1 = temp.find("\"")+1;
                        p2 = temp.find("\"",p1);
                        std::string fontname = temp.substr(p1,p2-p1);
                        _set_fontFile = fontname;
                        CCLOG("fontname = %s",fontname.c_str());
                    }
                }
                    break;
                case 2:
                {
                    posEnd = s.find(">",posStart+1);
                    if (isEnd)
                    {
                        CCLOG("outline end");
                        _set_outline = DEFAULT_OUTLINE;
                        _set_outline_color = DEFAULT_OUTLINE_COLOR;
                    }
                    else
                    {
                        posStart = s.find("=",posStart)+1;
                        std::string temp = s.substr(posStart,posEnd-posStart);
                        int size = atoi(temp.c_str());
                        _set_outline = size;
                        CCLOG("outline %d",size);
                        unsigned long p1 = temp.find("=");
                        if (p1!=std::string::npos)
                        {
                            Color4B c = int2ccc3(strtoul(temp.substr(p1+1).c_str(), NULL, 16));
                            _set_outline_color = c;
                            CCLOG("outline color = %d,%d,%d,%d",c.r,c.g,c.b,c.a);
                        }
                    }
                }
                    break;
                case 3:
                {
                    posEnd = s.find(">",posStart);
                    if (isEnd)
                    {
                        CCLOG("shadow end");
                        _set_shadow = false;
                    }
                    else
                    {
                        _set_shadow = true;
                        CCLOG("shadow start");
                    }
                }
                    break;
                case 4:
                {
                    posEnd = s.find(">",posStart);
                    if (isEnd)
                    {
                        _set_link = "";
                        CCLOG("link end");
                    }
                    else
                    {
                        posStart = s.find("=",posStart)+1;
                        std::string temp = s.substr(posStart,posEnd-posStart);
                        std::string::value_type p1,p2;
                        p1 = temp.find("\"")+1;
                        p2 = temp.find("\"",p1);
                        std::string linkstr = temp.substr(p1,p2-p1);
                        _set_link = linkstr;
                        CCLOG("link = %s",linkstr.c_str());
                    }
                }
                    break;
                case 5:
                {
                    posEnd = s.find(">",posStart);
                    
                    posStart = s.find("=",posStart)+1;
                    
                    std::string temp = s.substr(posStart,posEnd-posStart);
                    std::string::value_type p1,p2;
                    p1 = temp.find("\"")+1;
                    p2 = temp.find("\"",p1);
                    std::string img = temp.substr(p1,p2-p1);
                    Sprite* s = Sprite::create(img);
                    if (s)
                    {
                        pushBackElement(XYRichElementCustomNode::create(s));
                    }
                    
                    CCLOG("img = %s",img.c_str());
                    
                }
                    break;
                case 6:
                {
                    posEnd = s.find(">",posStart);
                    if (isEnd)
                    {
                        _set_textColor = DEFAULT_COLOR;
                        CCLOG("color end");
                    }
                    else
                    {
                        posStart = s.find("=",posStart)+1;
                        Color4B c = int2ccc3(strtoul(s.substr(posStart,posEnd-posStart).c_str(), NULL, 16));
                        _set_textColor = c;
                        CCLOG("%d,%d,%d,%d",c.r,c.g,c.b,c.a);
                    }
                    
                }
                    break;
                case 7:
                {
                    posEnd = s.find(">",posStart);
                    if (isEnd)
                    {
                        _set_underline = false;
                        _set_underline_color = DEFAULT_UNDERLINE_COLOR;
                        CCLOG("underline end");
                    }
                    else
                    {
                        _set_underline = true;
                        if (s.substr(posStart,posEnd-posStart).find("=")!=std::string::npos)
                        {
                            posStart = s.find("=",posStart)+1;
                            Color4B c = int2ccc3(strtoul(s.substr(posStart,posEnd-posStart).c_str(), NULL, 16));
                            _set_underline_color = c;
                            CCLOG("%d,%d,%d,%d",c.r,c.g,c.b,c.a);
                        }
                        else
                        {
                            CCLOG("underline no color");
                        }
                        
                    }
                }
                    break;
                default:
                    break;
            }
        }
        
        posStart = posEnd+1;
    }
    
    return true;
}

ui::RichElement* XYRichText::createWithSet(const std::string& text)
{
    if (text.empty())
    {
        Node* node = Node::create();
        node->setContentSize(Size(getContentSize().width, 1));
        return XYRichElementCustomNode::create(node);
    }
    XYRichElementText* ret = XYRichElementText::create(text, _set_fontFile, _set_fontSize);
    if (_set_outline>0)
    {
        ret->setOutLine(_set_outline);
        ret->setOutLineColor(_set_outline_color);
    }
    
    ret->setShadow(_set_shadow);
    if (!_set_link.empty())
    {
//        ret->setLinkUrl(_set_link);
    }
    
    CCLOG("%d,%d,%d,%d",_set_textColor.r,_set_textColor.g,_set_textColor.b,_set_textColor.a);
    ret->setTextColor(_set_textColor);
    if (_set_underline)
    {
        ret->setUnderLineSize(2);
        if (_set_underline_color.a == 0)
        {
            ret->setUnderLineColor(_set_textColor);
        }
        else
        {
            ret->setUnderLineColor(_set_underline_color);
        }
    }
    
    
    return  ret;
}


void XYRichText::visit(Renderer* renderer, const Mat4 &parentTransform, uint32_t parentFlags)
{
    if (_enabled)
    {
        formatText();
        Widget::visit(renderer, parentTransform, parentFlags);
    }
}


void XYRichText::formatText()
{
    if (_formatTextDirty)
    {
        _elementRenderersContainer->removeAllChildren();
        _elementRenders.clear();
        if (_ignoreSize)
        {
            addNewLine();
            for (ssize_t i=0; i<_richElements.size(); i++)
            {
                XYRichElement* element = static_cast<XYRichElement*>(_richElements.at(i));
                Node* elementRenderer = nullptr;
                switch (element->_type)
                {
                    case ui::RichElement::Type::TEXT:
                    {
                        Label* elementLabel = nullptr;
                        XYRichElementText* elmtText = static_cast<XYRichElementText*>(element);
                        if (FileUtils::getInstance()->isFileExist(elmtText->_fontName))
                        {
                            elementLabel = Label::createWithTTF(elmtText->_text.c_str(), elmtText->_fontName, elmtText->_fontSize);
                        }
                        else
                        {
                            elementLabel = Label::createWithSystemFont(elmtText->_text.c_str(), elmtText->_fontName, elmtText->_fontSize);
                        }
                        if (elmtText->getOutLine()>0)
                        {
                            elementLabel->enableOutline(elmtText->getOutLineColor(),elmtText->getOutLine());
                        }
                        if (elmtText->getShadow())
                        {
                            elementLabel->enableShadow();
                        }
                        elementLabel->setTextColor(/*elmtText->getTextColor()*/Color4B::RED);
                        if (elmtText->getUnderLineSize()>0)
                        {
                            LayerColor* l = nullptr;
                            if (elmtText->getUnderLineColor().a == 0)
                            {
                                l =  LayerColor::create(elmtText->getTextColor(), elementLabel->getContentSize().width, elmtText->getUnderLineSize());
                            }
                            else
                            {
                                l = LayerColor::create(elmtText->getUnderLineColor(), elementLabel->getContentSize().width, elmtText->getUnderLineSize());
                            }
                            elementLabel->setUserObject(l);
                        }
                        if (elmtText->getTouchCallBack())
                        {
//                            std::string* tag = new std::string(elmtText->getTag());
//                            elementLabel->setUserData(tag);
//                            _touchDelegate[elementLabel] = elmtText->getTouchCallBack();
                        }
                        elementRenderer = elementLabel;
                        elementRenderer->setColor(elmtText->_color);
                        elementRenderer->setOpacity(elmtText->_opacity);
                        break;
                    }
                    case ui::RichElement::Type::IMAGE:
                    {
                        XYRichElementImage* elmtImage = static_cast<XYRichElementImage*>(element);
                        elementRenderer = Sprite::create(elmtImage->_filePath.c_str());
                        elementRenderer->setColor(elmtImage->_color);
                        elementRenderer->setOpacity(elmtImage->_opacity);
                        break;
                    }
                    case ui::RichElement::Type::CUSTOM:
                    {
                        XYRichElementCustomNode* elmtCustom = static_cast<XYRichElementCustomNode*>(element);
                        elementRenderer = elmtCustom->_customNode;
                        elementRenderer->setColor(elmtCustom->_color);
                        elementRenderer->setOpacity(elmtCustom->_opacity);
                        break;
                    }
                    default:
                        break;
                }
                
                pushToContainer(elementRenderer);
            }
        }
        else
        {
            addNewLine();
            for (ssize_t i=0; i<_richElements.size(); i++)
            {
                
                XYRichElement* element = static_cast<XYRichElement*>(_richElements.at(i));
                switch (element->_type)
                {
                    case ui::RichElement::Type::TEXT:
                    {
                        XYRichElementText* elmtText = static_cast<XYRichElementText*>(element);
                        handleTextRenderer(*elmtText);
                        break;
                    }
                    case ui::RichElement::Type::IMAGE:
                    {
                        XYRichElementImage* elmtImage = static_cast<XYRichElementImage*>(element);
                        handleImageRenderer(elmtImage->_filePath.c_str(), elmtImage->_color, elmtImage->_opacity);
                        break;
                    }
                    case ui::RichElement::Type::CUSTOM:
                    {
                        XYRichElementCustomNode* elmtCustom = static_cast<XYRichElementCustomNode*>(element);
                        handleCustomRenderer(elmtCustom->_customNode);
                        break;
                    }
                    default:
                        break;
                }
            }
        }
        formarRenderers();
        _formatTextDirty = false;
    }
}


void XYRichText::handleTextRenderer(const XYRichElementText& textInfo)
{
    auto fileExist = FileUtils::getInstance()->isFileExist(textInfo.getFontName());
    Label* textRenderer = nullptr;
    if (fileExist)
    {
        textRenderer = Label::createWithTTF(textInfo.getText(), textInfo.getFontName(), textInfo.getFontSize());
    }
    else
    {
        textRenderer = Label::createWithSystemFont(textInfo.getText(), textInfo.getFontName(), textInfo.getFontSize());
    }
    if (textInfo.getOutLine()>0)
    {
        textRenderer->enableOutline(textInfo.getOutLineColor(),textInfo.getOutLine());
    }
    if (textInfo.getShadow())
    {
        textRenderer->enableShadow();
    }
    
    float textRendererWidth = textRenderer->getContentSize().width;
    _leftSpaceWidth -= textRendererWidth;
    if (_leftSpaceWidth < 0.0f)
    {
        float overstepPercent = (-_leftSpaceWidth) / textRendererWidth;
        std::string curText = textInfo.getText();
        size_t stringLength = _calcCharCount(textInfo.getText().c_str(),textInfo.getText().length());//StringUtils::getCharacterCountInUTF8String(textInfo.getText());
        int leftLength = stringLength * (1.0f - overstepPercent);
        std::string leftWords = utf8_substr(curText,0,leftLength);
        std::string cutWords = utf8_substr(curText, leftLength, curText.length() - leftLength);
        if (leftLength > 0)
        {
            Label* leftRenderer = nullptr;
            if (fileExist)
            {
                leftRenderer = Label::createWithTTF(utf8_substr(leftWords, 0, leftLength), textInfo.getFontName(), textInfo.getFontSize());
            }
            else
            {
                leftRenderer = Label::createWithSystemFont(utf8_substr(leftWords, 0, leftLength), textInfo.getFontName(), textInfo.getFontSize());
            }
            if (leftRenderer)
            {
                leftRenderer->setColor(textInfo.getColor());
                leftRenderer->setOpacity(textInfo.getOpacity());
                
                if (textInfo.getOutLine()>0)
                {
                    leftRenderer->enableOutline(textInfo.getOutLineColor(),textInfo.getOutLine());
                }
                if (textInfo.getShadow())
                {
                    leftRenderer->enableShadow();
                }
                leftRenderer->setTextColor(textInfo.getTextColor());
                if (textInfo.getUnderLineSize()>0)
                {
                    LayerColor* l = nullptr;
                    if (textInfo.getUnderLineColor().a==0)
                    {
                        l =  LayerColor::create(textInfo.getTextColor(), leftRenderer->getContentSize().width, textInfo.getUnderLineSize());
                    }
                    else
                    {
                        l = LayerColor::create(textInfo.getUnderLineColor(), leftRenderer->getContentSize().width, textInfo.getUnderLineSize());
                    }
                    leftRenderer->setUserObject(l);
                }
                if (textInfo.getTouchCallBack())
                {
//                    std::string* tag = new std::string(textInfo.getTag());
//                    leftRenderer->setUserData(tag);
//                    _touchDelegate[leftRenderer] = textInfo.getTouchCallBack();
                }
                pushToContainer(leftRenderer);
            }
        }
        
        addNewLine();
        XYRichElementText cutRich = textInfo;
        cutRich.setText(cutWords);
        handleTextRenderer(cutRich);
    }
    else
    {
        textRenderer->setColor(textInfo.getColor());
        textRenderer->setOpacity(textInfo.getOpacity());
        
        if (textInfo.getOutLine()>0)
        {
            textRenderer->enableOutline(textInfo.getOutLineColor(),textInfo.getOutLine());
        }
        if (textInfo.getShadow())
        {
            textRenderer->enableShadow();
        }
        textRenderer->setTextColor(textInfo.getTextColor());
        if (textInfo.getUnderLineSize()>0)
        {
            LayerColor* l = nullptr;
            if (textInfo.getUnderLineColor().a==0)
            {
                l =  LayerColor::create(textInfo.getTextColor(), textRenderer->getContentSize().width, textInfo.getUnderLineSize());
            }
            else
            {
                l = LayerColor::create(textInfo.getUnderLineColor(), textRenderer->getContentSize().width, textInfo.getUnderLineSize());
            }
            textRenderer->setUserObject(l);
//            textRenderer->enableGlow(textInfo.getUnderLineColor());
        }
        if (textInfo.getTouchCallBack())
        {
//            std::string* tag = new std::string(textInfo.getTag());
//            textRenderer->setUserData(tag);
//            _touchDelegate[textRenderer] = textInfo.getTouchCallBack();
        }
        pushToContainer(textRenderer);
    }
}


void XYRichText::formarRenderers()
{
    if (_ignoreSize)
    {
        float newContentSizeWidth = 0.0f;
        float newContentSizeHeight = 0.0f;
        
        Vector<Node*>* row = (_elementRenders[0]);
        float nextPosX = 0.0f;
        for (ssize_t j=0; j<row->size(); j++)
        {
            Node* l = row->at(j);
            l->setAnchorPoint(Vec2::ZERO);
            l->setPosition(nextPosX, 0.0f);
            _elementRenderersContainer->addChild(l, 1);
            Size iSize = l->getContentSize();
            newContentSizeWidth += iSize.width;
            newContentSizeHeight = MAX(newContentSizeHeight, iSize.height);
            nextPosX += iSize.width;
        }
        _elementRenderersContainer->setContentSize(Size(newContentSizeWidth, newContentSizeHeight));
    }
    else
    {
        float newContentSizeHeight = 0.0f;
        float *maxHeights = new float[_elementRenders.size()];
        
        for (size_t i=0; i<_elementRenders.size(); i++)
        {
            Vector<Node*>* row = (_elementRenders[i]);
            float maxHeight = 0.0f;
            for (ssize_t j=0; j<row->size(); j++)
            {
                Node* l = row->at(j);
                maxHeight = MAX(l->getContentSize().height, maxHeight);
            }
            maxHeights[i] = maxHeight;
            newContentSizeHeight += maxHeights[i];
        }
        
        
        float nextPosY = _customSize.height;
        for (size_t i=0; i<_elementRenders.size(); i++)
        {
            Vector<Node*>* row = (_elementRenders[i]);
            float nextPosX = 0.0f;
            nextPosY -= (maxHeights[i] + _verticalSpace);
            
            for (ssize_t j=0; j<row->size(); j++)
            {
                Node* l = row->at(j);
                l->setAnchorPoint(Vec2::ZERO);
                l->setPosition(nextPosX, nextPosY);
                _elementRenderersContainer->addChild(l, 1);
                Node* under = dynamic_cast<Node*>(l->getUserObject());
                if (under)
                {
                    under->setPosition(Point(nextPosX,nextPosY-1));
                    _elementRenderersContainer->addChild(under);
                    l->setUserObject(nullptr);
                }
                
                nextPosX += l->getContentSize().width;
            }
        }
        _elementRenderersContainer->setContentSize(_contentSize);
        delete [] maxHeights;
    }
    
    size_t length = _elementRenders.size();
    for (size_t i = 0; i<length; i++)
    {
        Vector<Node*>* l = _elementRenders[i];
        l->clear();
        delete l;
    }
    _elementRenders.clear();
    
    if (_ignoreSize)
    {
        Size s = getVirtualRendererSize();
        this->setContentSize(s);
    }
    else
    {
        this->setContentSize(_customSize);
    }
    updateContentSizeWithTextureSize(_contentSize);
    _elementRenderersContainer->setPosition(_contentSize.width / 2.0f, _contentSize.height / 2.0f);
}

