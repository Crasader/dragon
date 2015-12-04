//
//  ActivityConf.cpp
//  DragonBattle
//
//  Created by 白明江 on 11/18/13.
//
//

#include <ctime>
#include "ActivityConf.h"

ActivityConf::ActivityConf()
{
    m_ActivityMap.clear();
}

ActivityConf::~ActivityConf()
{
    unload();
}

bool ActivityConf::loadFromFile(char* filePath, bool bEncrypt/* = false*/)
{
    bool ret = true;
    
#ifdef WONPEE_SERVER
	const char* xmlFile = filePath;
#else
    const char* xmlFile = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(filePath).c_str();
#endif
	//xmlDocPtr doc = xmlParseFile(xmlFile);
    xmlDocPtr doc = XmlEncrypt::getXmlDocPtr(xmlFile, bEncrypt);
    
    if (!doc)
    {
        ERROR_RETURN(("ActivityConf::loadFromFile xmlParseDoc partner.xml failded"), false);
    }
    
    xmlNodePtr cur = xmlDocGetRootElement(doc);
    if (!cur)
    {
        ERROR_LOG("ActivityConf::loadFromFile xmlDocGetRootElement partner.xml failded");
        ret = false;
        xmlFreeDoc(doc);
        return ret;
    }
    
    cur = cur->xmlChildrenNode;
    while (cur)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar*)"activity")))
        {
            ActivityConf_t *pConf = new ActivityConf_t;
            DECODE_XML_PROP_INT(pConf->activity_id, cur, "id");
			DECODE_XML_PROP_INT_DEFAULT(pConf->need_lv, cur, "need_lv", 1);
			DECODE_XML_PROP_INT_DEFAULT(pConf->need_vip, cur, "need_vip", 0);
            
            decodeLimitDate(cur, (char*)"start_end_date", &(pConf->date_limit));
            decodeLimitTime(cur, (char*)"start_end_time", &(pConf->time_limit_vec));
            
#ifndef WONPEE_SERVER
            DECODE_XML_PROP_STR(pConf->name, cur, "name");
            DECODE_XML_PROP_STR(pConf->desc, cur, "des");
            
            decodePrizes(cur, (char*)"prizes", &(pConf->prizes));
#endif

            m_ActivityMap.insert(std::pair<uint32_t, ActivityConf_t*>(pConf->activity_id, pConf));
        }
        cur = cur->next;
    }
    ret = true;
    
    xmlFreeDoc(doc);
    return ret;
}

void ActivityConf::decodeLimitDate(xmlNodePtr cur, char* field, std::vector<StartEndDate_t>* limit_dates)
{
    int tmp_data[100] = {0};
    decode_xml_prop_arr_int_default(tmp_data, 100, cur, field, -1);
    
    for (int i = 0; i < 100; )
    {
        if (tmp_data[i] == -1)
        {
            break;
        }
        
        StartEndDate_t limit_date;
        
        int year = tmp_data[i ++];
        int mon = tmp_data[i ++];
        int mday = tmp_data[i ++];
        limit_date.nStartDate = year * 10000 + mon * 100 + mday;
        
        year = tmp_data[i ++];
        mon = tmp_data[i ++];
        mday = tmp_data[i ++];
        limit_date.nEndDate = year * 10000 + mon * 100 + mday;
        
        limit_dates->push_back(limit_date);
    }
}

void ActivityConf::decodeLimitTime(xmlNodePtr cur, char* field, std::vector<StartEndTime_t>* limitTime)
{
    int tmp_data[100] = {0};
    decode_xml_prop_arr_int_default(tmp_data, 100, cur, field, -1);
    
    for (int i = 0; i < 100; )
    {
        if (tmp_data[i] == -1)
        {
            break;
        }
        
        StartEndTime_t limit_time;
        
        int hour = tmp_data[i ++];
        int min = tmp_data[i ++];
        int sec = tmp_data[i ++];
        
        limit_time.nStartTime = hour * 10000 + min * 100 + sec ;
        
        hour = tmp_data[i ++];
        min = tmp_data[i ++];
        sec = tmp_data[i ++];
        
        limit_time.nEndTime = hour * 10000 + min * 100 + sec ;
        
        limitTime->push_back(limit_time);
    }
}

void ActivityConf::decodePrizes(xmlNodePtr cur, char* field, std::vector<uint32_t>* prizes)
{
    int tmp_data[20] = {0};
    decode_xml_prop_arr_int_default(tmp_data, 20, cur, field, -1);
    
    for (int i = 0; i < 20; )
    {
        if (tmp_data[i] == -1)
        {
            break;
        }
        
        uint32_t prize_id = (uint32_t)tmp_data[i ++];
        
        prizes->push_back(prize_id);
    }
}

const ActivityConf_t* ActivityConf::getActivityByID(int activity_id) const
{
    ActivityMap::const_iterator it = m_ActivityMap.find(activity_id);
    if (it != m_ActivityMap.end())
    {
        return it->second;
    }
    return NULL;
}

void ActivityConf::unload()
{
    ActivityMap::iterator it = m_ActivityMap.begin();
    while (it != m_ActivityMap.end())
    {
        delete it->second;
        it->second = NULL;
        ++ it;
    }
    m_ActivityMap.clear();
}

