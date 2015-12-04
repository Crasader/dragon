//
//  CStrokeLabel.h
//  ForStroke
//
//  Created by mingjiang 13-5-23.
//
//

#ifndef __DragonBattle__HRichLabelTTF__
#define __DragonBattle__HRichLabelTTF__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define _Stroke_Label_Angle_ 3

class HRichLabelTTF : public CCLabelTTF {
    
    typedef CCLabelTTF super;
    
public:
    int hrdcount;
    CREATE_FUNC(HRichLabelTTF);
    
    static HRichLabelTTF* create(const char *string, const char *fontName, float fontSize,ccColor3B color = ccWHITE);
    
    ///
    HRichLabelTTF();
    virtual ~HRichLabelTTF();
    
//    CC_PROPERTY_BOOL(bool, m_bBlack, Black);
    virtual void setOpacity(GLubyte opacity);
    virtual void setStroke(const ccColor3B& strokeColor, float strokeSize = 1, GLubyte opacity = 0xff);
    virtual void setShadow(const ccColor3B& shadowColor, const CCPoint&  shadowOffset, GLubyte opacity = 0xff);
    virtual void setShadow(float shadowOffset,const ccColor3B& shadowColor=ccBLACK, GLubyte opacity = 0xff);
    
    const ccColor3B getStrokeColor() { return  ccc3(m_strokeColor.r, m_strokeColor.g, m_strokeColor.b); }
    GLubyte getStrokeOpacity() { return m_strokeColor.a; }
    float getStrokeSize() { return  m_strokeSize; }
    
    const ccColor3B getShadowColor() { return ccc3(m_shadowColor.r, m_shadowColor.g, m_shadowColor.b); }
    GLubyte getShadowOpacity() { return m_shadowColor.a; }
    CCPoint getShadowOffset() { return m_shadowOffset; }
    
    virtual void setGradientColor(const ccColor3B& beginColor, const ccColor3B& endColor);
    
    // super fun will change texture
    virtual void setString(const char *string);
    // super fun change texture, but have to use HRichLabelTTF object to call.
    virtual void setFontName(const char *fontName);
    virtual void setFontSize(float fontSize);
    virtual void setHorizontalAlignment(CCTextAlignment alignment);
    virtual void setVerticalAlignment(CCVerticalTextAlignment verticalAlignment);
    virtual void setDimensions(const CCSize &dim);
    
    // super fun will change position
    virtual void onEnter();
    virtual void setParent(CCNode* parent);
    virtual void setPosition(const CCPoint &position);
    virtual void setRotation(float fRotation);
    
    virtual void draw();
    
protected:
    virtual bool updateRichTexture();
    virtual void fixPostion();
    
    // only shadow
    void updateShadowSprite();
    
    // have stroke
    void renderShadowTexture(float w, float h);
    void renderStrokeTexture(float w, float h);
    
    //
    void updateGradientColor();
    
    
protected:
    CCPoint m_intendedPosition;
    ccColor3B m_intendedColor;
    
    float m_strokeSize;
    ccColor4B m_strokeColor;
    
    ccColor4B m_shadowColor;
    CCPoint m_shadowOffset;
    
    ccColor3B m_beginColor;
    ccColor3B m_endColor;
    
    ccColor3B m_beginColorUnmodified;
    ccColor3B m_endColorUnmodified;
    
    CCRenderTexture* m_rdTexture;
    CCSprite* m_rdSprite;
    
    int m_nOpacity;
};

class HRichLabelTTFLoader : public CCLabelTTFLoader {
    
public:
    virtual ~HRichLabelTTFLoader() {};
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(HRichLabelTTFLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(HRichLabelTTF);
    
};

#endif /* defined(__card__HRichLabelTTF__) */
