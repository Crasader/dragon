//
//  VipConf.cpp
//  DragonBattle
//
//  Created by zeng xiaolong on 13-10-21.
//
//

#include "ShopConf.h"
#include "VipConf.h"

VipConf::VipConf()
{
    
}
VipConf::~VipConf()
{
    
}
bool VipConf::loadFromFile(char* filePath, bool bEncrypt)
{
    bool ret = false;
    xmlNodePtr cur;
#ifdef WONPEE_SERVER
	const char* xmlFile = filePath;
#else
    const char* xmlFile = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(filePath).c_str();
#endif
   	xmlDocPtr doc = XmlEncrypt::getXmlDocPtr(xmlFile, bEncrypt);
    if (!doc)
    {
        ERROR_RETURN(("VipConf::loadFromFile xmlParseDoc vip_config failded"), false);
    }
    
    cur = xmlDocGetRootElement(doc);
    if (!cur)
    {
        ERROR_LOG("xmlDocGetRootElement %s error!", filePath);
        goto exit;
    }

    cur = cur->xmlChildrenNode;
    while (cur)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar*)"VIP")))
        {
            uint32_t vipLV = 0;
            DECODE_XML_PROP_INT(vipLV, cur, "VIPLV");
            int lvFunction[VIPFUNCTIONCOUNT];
#ifndef WONPEE_SERVER
			DECODE_XML_PROP_STR_DEFAULT(des[vipLV], cur, "dec", "");
#endif
   			decode_xml_prop_arr_int_default(lvFunction,VIPFUNCTIONCOUNT,cur,(void*)"VIPfunction",0);
            for (int i = 0; i<VIPFUNCTIONCOUNT; i++)
            {
                vipLvAndFunction[vipLV][i] = lvFunction[i];
            }
        }
        cur = cur->next;
    }
    
    ret = true;
exit:
    xmlFreeDoc(doc);
    return ret;

}

int VipConf::getVipFunction(int viplv,int FunctionID)
{
    return vipLvAndFunction[viplv][FunctionID];
}

int VipConf::getVipLvByCostGold(int cost_gold)
{
    int vipLv = 0;
    for (int i = 1; i <= 12; i ++)
    {
        int need_gold = getVipFunction(i, 0);
        if (cost_gold < need_gold)
        {
            vipLv = i - 1;
            break;
        }
    }
    
    if (getVipFunction(12, 0) <= cost_gold) {
        vipLv = 12;
    }
    
    return vipLv;
}


