//
//  HRichLabelTTF.cpp
//  card
//
//  Created by mingjiang on 13-5-24.
//
//

#include "HRichLabelTTF.h"

HRichLabelTTF::HRichLabelTTF()
: m_rdTexture(NULL)
, m_rdSprite(NULL)
, m_strokeColor(ccc4(255, 255, 255, 255))
, m_strokeSize(0)
, m_shadowColor(ccc4(255, 255, 255, 255))
, m_shadowOffset(CCPointZero)
{
    m_rdSprite = new CCSprite;
    hrdcount = 0;
    
    m_nOpacity = 0;
}

HRichLabelTTF::~HRichLabelTTF()
{
    CC_SAFE_RELEASE_NULL(m_rdSprite);
    CC_SAFE_RELEASE_NULL(m_rdTexture);
}

HRichLabelTTF* HRichLabelTTF::create(const char *string, const char *fontName, float fontSize, ccColor3B color)
{
    HRichLabelTTF* plabel = new HRichLabelTTF();
    if(plabel && plabel->initWithString(string, fontName, fontSize))
    {
        plabel->setColor(color);
        plabel->autorelease();
        return plabel;
    }
    else
    {
        delete plabel;
        plabel = NULL;
        return NULL;
    }
}

void HRichLabelTTF::setFontName(const char *fontName)
{
    if (m_pFontName->compare(fontName))
    {
        super::setFontName(fontName);
        this->updateRichTexture();
    }
}

void HRichLabelTTF::setString(const char *string)
{
    CCAssert(string != NULL, "Invalid string");
    
    if (m_string.compare(string))
    {
        super::setString(string);
        this->updateRichTexture();
    }
}

void HRichLabelTTF::setHorizontalAlignment(CCTextAlignment alignment)
{
    if (alignment != m_hAlignment)
    {
        super::setHorizontalAlignment(alignment);
        this->updateRichTexture();
    }
}

void HRichLabelTTF::setVerticalAlignment(CCVerticalTextAlignment verticalAlignment)
{
    if (verticalAlignment != m_vAlignment)
    {
        super::setVerticalAlignment(verticalAlignment);
        this->updateRichTexture();
    }
}

void HRichLabelTTF::setDimensions(const CCSize &dim)
{
    if (dim.width != m_tDimensions.width || dim.height != m_tDimensions.height)
    {
        super::setDimensions(dim);
        this->updateRichTexture();
    }
}

void HRichLabelTTF::setFontSize(float fontSize)
{
    if (m_fFontSize != fontSize)
    {
        super::setFontSize(fontSize);
        this->updateRichTexture();
    }
}

void HRichLabelTTF::onEnter()
{
    this->fixPostion();
    CCLabelTTF::onEnter();
}

void HRichLabelTTF::setParent(CCNode* parent)
{
    CCLabelTTF::setParent(parent);
    this->fixPostion();
}

void HRichLabelTTF::setPosition(const CCPoint &position)
{
    m_intendedPosition = position;
    if (getTexture()) {
        this->fixPostion();
    }else{
        CCLabelTTF::setPosition(m_intendedPosition);
    }
}

void HRichLabelTTF::setRotation(float fRotation)
{
    CCLabelTTF::setRotation(fRotation);
    this->fixPostion();
}

void HRichLabelTTF::setStroke(const ccColor3B& strokeColor, float strokeSize, GLubyte opacity)
{
    m_strokeColor = ccc4(strokeColor.r, strokeColor.g, strokeColor.b, opacity);
    if (strokeSize == -1)
    {
        strokeSize = this->getFontSize() / 20.0;
    }
    m_strokeSize = strokeSize;
    m_nOpacity = opacity;
    updateRichTexture();
}

void HRichLabelTTF::setShadow(const ccColor3B& shadowColor, const CCPoint&  shadowOffset, GLubyte opacity)
{
    m_shadowOffset = shadowOffset;
    m_shadowColor = ccc4(shadowColor.r, shadowColor.g, shadowColor.b, opacity);
    m_nOpacity = opacity;
    updateRichTexture();
}

void HRichLabelTTF::setShadow(float shadowOffset,const ccColor3B& shadowColor, GLubyte opacity)
{
    m_shadowOffset = ccp(shadowOffset, -shadowOffset);
    m_shadowColor = ccc4(shadowColor.r, shadowColor.g, shadowColor.b, opacity);
    m_nOpacity = opacity;
    updateRichTexture();
}

//void HRichLabelTTF::setBlack(bool var)
//{
//    if (m_bBlack != var) {
//        m_bBlack = var;
//        if (var) {
//            m_intendedColor = getColor();
//            GLubyte gray = m_intendedColor.r*0.299 + m_intendedColor.g*0.587 + m_intendedColor.b*0.114;
//            setColor(ccc3(gray, gray, gray));
//        }else{
//            setColor(m_intendedColor);
//        }
//    }
//}
//
//bool HRichLabelTTF::isBlack()
//{
//    return m_bBlack;
//}

void HRichLabelTTF::setGradientColor(const ccColor3B& beginColor, const ccColor3B& endColor)
{
    m_beginColor = m_beginColorUnmodified = beginColor;
    m_endColor = m_endColorUnmodified = endColor;
    
    if (m_bOpacityModifyRGB)
    {
        m_beginColor.r = beginColor.r * m_nOpacity/255.0f;
        m_beginColor.g = beginColor.g * m_nOpacity/255.0f;
        m_beginColor.b = beginColor.b * m_nOpacity/255.0f;
        
        m_endColor.r = endColor.r * m_nOpacity/255.0f;
        m_endColor.g = endColor.g * m_nOpacity/255.0f;
        m_endColor.b = endColor.b * m_nOpacity/255.0f;
    }
    
    updateGradientColor();
}

void HRichLabelTTF::updateGradientColor()
{
    ccColor4B color4a = { m_beginColor.r, m_beginColor.g, m_beginColor.b, m_nOpacity };
    ccColor4B color4b = { m_endColor.r, m_endColor.g, m_endColor.b, m_nOpacity };
    
    m_sQuad.bl.colors = color4b;
    m_sQuad.br.colors = color4b;
    m_sQuad.tl.colors = color4a;
    m_sQuad.tr.colors = color4a;
    
    // renders using batch node
    if (m_pobBatchNode)
    {
        if (m_uAtlasIndex != CCSpriteIndexNotInitialized)
        {
            m_pobTextureAtlas->updateQuad(&m_sQuad, m_uAtlasIndex);
        }
        else
        {
            // no need to set it recursively
            // update dirty_, don't update recursiveDirty_
            setDirty(true);
        }
    }
    
}

void HRichLabelTTF::draw()
{
    if (m_rdSprite->getTexture()) {
        m_rdSprite->visit();
    }
    
    if (m_rdTexture) {
        m_rdTexture->visit();
    }
    
    super::draw();
}

GLubyte opMult(GLubyte opaL, GLubyte opaR)
{
    return (GLubyte)((opaL/255.f) * (opaR/255.f) * 255);
}

void HRichLabelTTF::setOpacity(GLubyte opacity)
{
    m_nOpacity = opacity;
    if (opacity == getOpacity())
    {
        return;
    }
    super::setOpacity(opacity);
    if (m_rdTexture) {
        m_rdTexture->getSprite()->setOpacity(opMult(opacity, m_strokeColor.a));
    }
    
    if (m_rdSprite && m_rdSprite->getTexture()) {
        m_rdSprite->setOpacity(opMult(opacity, m_shadowColor.a));
    }
}

bool HRichLabelTTF::updateRichTexture()
{
    hrdcount++;
    CC_SAFE_RELEASE_NULL(m_rdTexture);
    
    //
    bool needsShadow = m_string.size() > 0 && !m_shadowOffset.equals(CCPointZero);
    bool needsStroke = m_string.size() > 0 && m_strokeSize != 0;
    
    if (needsShadow || needsStroke) {
        
        CCTexture2D *tex = this->getTexture();
        m_rdSprite->initWithTexture(tex);
        m_rdSprite->setFlipY(false);
        m_rdSprite->setBlendFunc((ccBlendFunc){GL_ONE, GL_ONE_MINUS_SRC_ALPHA});
        
        if (needsStroke){
            
            int italic = strstr(getFontName(), "Italic") ? 2 : 0; // 斜体字
            
            float offsetX = (m_strokeSize * 2 + fabs(m_shadowOffset.x*2)) + italic;
            float offsetY = m_strokeSize * 2 + fabs(m_shadowOffset.y*2);
            
            float x=tex->getContentSize().width + offsetX;
            float y=tex->getContentSize().height+ offsetY;
            
            m_rdTexture = CCRenderTexture::create(x, y);
            m_rdTexture->retain();
            m_rdTexture->begin();
            {
                m_rdSprite->setOpacity(255);
                m_rdSprite->setColor(ccWHITE);
                renderStrokeTexture(x, y);
            }
            m_rdTexture->end();
            m_rdTexture->getSprite()->setColor(ccc3(m_strokeColor.r, m_strokeColor.g, m_strokeColor.b));
            m_rdTexture->getSprite()->setOpacity(opMult(m_strokeColor.a, getOpacity()));
            m_rdTexture->getSprite()->setAnchorPoint(ccp(0, 1));
            
            if (needsShadow)
            {
                m_rdSprite->setTexture(m_rdTexture->getSprite()->getTexture());
                updateShadowSprite();
                m_rdSprite->setTextureRect(CCRectMake(0, 0, x, y));
                m_rdSprite->setFlipY(true);
            }
            else
            {
                //m_rdSprite->setTexture(NULL);
            }
            
            setTextureRect(CCRectMake(-offsetX/2, -offsetY/2, x, y));
            
        }else{
            updateShadowSprite();
        }
    }else{
        if (m_rdSprite->getTexture()) {
            m_rdSprite->setTexture(NULL);
        }
    }
    
    this->fixPostion();
    
    return true;
}

// only shadow
void HRichLabelTTF::updateShadowSprite()
{
    m_rdSprite->setAnchorPoint(CCPointZero);
    m_rdSprite->setColor(ccc3(m_shadowColor.r, m_shadowColor.g, m_shadowColor.b));
    m_rdSprite->setOpacity(opMult(m_shadowColor.a, getOpacity()));
    m_rdSprite->setPosition(m_shadowOffset);
}

// have stroke
void HRichLabelTTF::renderShadowTexture(float w, float h)
{
    float tempX = w/2 + m_shadowOffset.x;
    float tempY = h/2 + m_shadowOffset.y;
    
    m_rdSprite->setPosition(ccp(tempX, tempY));
    m_rdSprite->visit();
}

void HRichLabelTTF::renderStrokeTexture(float w, float h)
{
    int sp = 6*(6-m_strokeSize);
    sp = sp<6?6:sp;
    for(int i=0; i<360; i+=sp)
    {
        float tempX = w/2 + cos(CC_DEGREES_TO_RADIANS(i))*m_strokeSize;
        float tempY = h/2 + sin(CC_DEGREES_TO_RADIANS(i))*m_strokeSize;
        m_rdSprite->setPosition(ccp(tempX,tempY));
        m_rdSprite->visit();
    }
}


void HRichLabelTTF::fixPostion()
{
    if (getTexture() == NULL)return;
    
    CCSize dim = getTexture()->getContentSize();
    
    super::setPosition(m_intendedPosition);
    
    if (getScaleX() < 0.3 || getScaleY() < 0.3) return;
    
    CCPoint worldSpaceTopleft = this->convertToWorldSpace(ccp(0, dim.height));
    
    worldSpaceTopleft.x = roundf(worldSpaceTopleft.x * CC_CONTENT_SCALE_FACTOR()) / CC_CONTENT_SCALE_FACTOR();
    worldSpaceTopleft.y = roundf(worldSpaceTopleft.y * CC_CONTENT_SCALE_FACTOR()) / CC_CONTENT_SCALE_FACTOR();
    
    CCPoint nodeSpaceTopleft = this->convertToNodeSpace(worldSpaceTopleft);
    
    CCPoint delta = ccpSub(nodeSpaceTopleft, ccp(0, dim.height));
    CCPoint newPos = ccpAdd(getPosition(), delta);
    
    super::setPosition(newPos);
}
