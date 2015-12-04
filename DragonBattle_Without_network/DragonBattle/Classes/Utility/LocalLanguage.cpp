//
//  LocalLanguage.cpp
//  TestGame
//
//  Created by 白明江 on 1/11/14.
//
//

#include "LocalLanguage.h"

#define FILE_WITH_LANG_FORMAT ("lang/%s/local_string.xml")
#define DEFAULT_FILE_WITH_LANG ("lang/zh/local_string.xml")

LocalString::LocalString()
{
    m_pStringValueMap = CCDictionary::create();
    m_pStringValueMap->retain();
}

LocalString::~LocalString()
{
    unload();
}

bool LocalString::loadFromFile(const char* filePath, bool bEncrypt)
{
    bool ret = true;

    const char* xmlFile = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(filePath).c_str();
    xmlDocPtr doc = XmlEncrypt::getXmlDocPtr(xmlFile, bEncrypt);
    
    if (!doc)
    {
        ERROR_RETURN(("LocalString::loadFromFile xmlParseDoc LocalString:.xml failded"), false);
    }
    
    xmlNodePtr cur = xmlDocGetRootElement(doc);
    if (!cur)
    {
        ERROR_LOG("LocalString::loadFromFile xmlDocGetRootElement partner.xml failded");
        ret = false;
        xmlFreeDoc(doc);
        return ret;
    }
    
    cur = cur->xmlChildrenNode;
    while (cur)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar*)"string")))
        {
            char tmp [512] = {0};
            DECODE_XML_PROP_STR_DEFAULT(tmp, cur, "name", "");
            std::string key(tmp);
            
            xmlChar* content = xmlNodeGetContent(cur);
            /*
            if (content)
            {
                m_pStringValueMap->setObject(CCString::create((const char*)content), key);
                xmlFree(content);
            }
             */
            
            //转义字符
            char pBuffer [512] = {0};
            if (content)
            {
                int len = strlen((const char*)content);
                memset(tmp, 0, sizeof(tmp));
                memccpy(tmp, content, 0, len);
                
                int index = 0;
                for (int i = 0; i < 512; ++i) {
                    
                    if (tmp[i] != '\\')
                    {
                        pBuffer[index++] = tmp[i];
                    }
                    else
                    {
                        ++i;
                        switch (tmp[i]) {
                            case 'r':
                            pBuffer[index++] = '\r';
                            break;
                            case 'n':
                            pBuffer[index++] = '\n';
                            break;
                            case 't':
                            pBuffer[index++] = '\t';
                            break;
                            default:
                            pBuffer[index++] = tmp[i];
                            break;
                        }
                    }
                }

                xmlFree(content);
            }
            
            m_pStringValueMap->setObject(CCString::create(pBuffer), key);
            
        }
        cur = cur->next;
    }
    ret = true;
    
    xmlFreeDoc(doc);
    return ret;
}

void LocalString::unload()
{
    m_pStringValueMap->removeAllObjects();
    CC_SAFE_RELEASE_NULL(m_pStringValueMap);
}


LocalString* LocalLanguage::s_pLocalString = NULL;

std::string LocalLanguage::getFileWithCurLang()
{
    char str[128] = {0};
    
    int type = CCApplication::sharedApplication()->getCurrentLanguage();
    switch (type)
    {
        case kLanguageChinese:
        {
            sprintf(str, FILE_WITH_LANG_FORMAT, "zh");
            break;
        }
        case kLanguageEnglish:
        {
            sprintf(str, FILE_WITH_LANG_FORMAT, "en");
            break;
        }
            
        default:
            break;
    }
    
    return std::string(str);
}

void LocalLanguage::loadLocalString()
{
    if (s_pLocalString == NULL)
    {
        s_pLocalString = new LocalString;
    }
    
    std::string filePath = getFileWithCurLang();
    
    if (CCFileUtils::sharedFileUtils()->isFileExist(filePath) == false) //如果不存在使用默认语言文件
    {
        filePath = DEFAULT_FILE_WITH_LANG;
    }
    
    if (!s_pLocalString->loadFromFile(filePath.c_str(), false))
    {
        DEBUG_LOG("Load local setting error!");
    }
}

void LocalLanguage::unLoadLocalString()
{
    CC_SAFE_DELETE(s_pLocalString);
    s_pLocalString = NULL;
}

CCString* LocalLanguage::getLocalCCString(const char* key)
{
    const char* value = getLocalString(key);
    return CCString::create(value);
}

const char* LocalLanguage::getLocalString(const char* key)
{
    const char* value = s_pLocalString->getString(key);
    if (value)
    {
        return value;
    }
    else
    {
        return "";
    }
}