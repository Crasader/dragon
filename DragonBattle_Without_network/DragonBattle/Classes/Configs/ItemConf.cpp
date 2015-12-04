//
//  ItemConf.cpp
//
#ifdef WONPEE_SERVER
#include "cocos2d_diff.h"
#else
#include "cocos2d.h"
#endif

#include "ItemConf.h"
#include <map>

using namespace cocos2d;

ItemConfig::ItemConfig()
{
    
}

ItemConfig::~ItemConfig()
{
	this->unload();	
}

bool ItemConfig::loadFromFile(char* filePath, bool bEncrypt)
{
    bool ret = false;
    xmlNodePtr cur, chl;
#ifdef WONPEE_SERVER
	const char* xmlFile = filePath;
#else	
    const char* xmlFile = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(filePath).c_str();
#endif
   	xmlDocPtr doc = XmlEncrypt::getXmlDocPtr(xmlFile, bEncrypt);
    if (!doc)
    {
        ERROR_RETURN(("ItemConfig::loadFromFile xmlParseDoc item.xml failded"), false);
    }
    
    cur = xmlDocGetRootElement(doc);
    if (!cur)
    {
        ERROR_LOG("xmlDocGetRootElement %s error!", filePath);
        goto exit;
    }
    
    m_itemMap.clear();
    
    cur = cur->xmlChildrenNode;
    while (cur)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar*)"ITEMS_KIND")))
        {
            uint32_t kindID = 0;
            DECODE_XML_PROP_INT_DEFAULT(kindID, cur, "ID", 0);
            if (kindID >= ITEM_KIND_MAX)
            {
                return false;
            }
            
            chl = cur->xmlChildrenNode;
            if (!loadItems(chl))
            {
                goto exit;
            }
        }
        cur = cur->next;
    }
    
    ret = true;
exit:
    xmlFreeDoc(doc);
    return ret;
}
                
bool ItemConfig::loadItems(xmlNodePtr chl)
{
    while (chl)
    {
        if ((!xmlStrcmp(chl->name, (const xmlChar*)"ITEM")))
        {
            item_t* itm = new item_t();
            DECODE_XML_PROP_INT(itm->nID, chl, "item_id");
            int itemKind = getKindOfItem(itm->nID);
            DECODE_XML_PROP_STR(itm->name, chl, "item_name");
			DECODE_XML_PROP_INT_DEFAULT(itm->nLevel, chl, "level", 0);
			DECODE_XML_PROP_INT_DEFAULT(itm->nProfession, chl, "profession", 0);
            DECODE_XML_PROP_INT_DEFAULT(itm->nQuality, chl, "quarty", 1);
            DECODE_XML_PROP_INT_DEFAULT(itm->nBuyPrice, chl, "price_buy", 0);
            DECODE_XML_PROP_INT_DEFAULT(itm->nSellPrice, chl, "price_sell", 0);
            DECODE_XML_PROP_INT_DEFAULT(itm->nBuyGold, chl, "piece_gold", 0);
            DECODE_XML_PROP_INT_DEFAULT(itm->nMax, chl, "max", 1);
            DECODE_XML_PROP_INT_DEFAULT(itm->nMissionID,chl,"missionid",11);
            DECODE_XML_PROP_STR_DEFAULT(itm->desc, chl, "description", "");
            
            switch (itemKind)
            {
                case kAttributeValue:
                {
            		DEBUG_LOG("ITEM: (%d %s %d %d %d)", itm->nID, itm->name, itm->nBuyPrice, itm->nSellPrice, itm->nMax);
                    break;
                }
                case kBattleItemKind:
				{
                    DECODE_XML_PROP_INT_DEFAULT(itm->u.item.nItemType, chl, "item_type", 0);
                    DECODE_XML_PROP_INT_DEFAULT(itm->u.item.nEffect, chl, "effect", 0);
                    DECODE_XML_PROP_INT_DEFAULT(itm->u.item.nLastValue, chl, "last_val", 0);
                    DECODE_XML_PROP_INT_DEFAULT(itm->u.item.nUsed, chl, "used", 0);
					//DEBUG_LOG("item type[%d] effect[%d] last_val[%d]", itm->u.item.nItemType, itm->u.item.nEffect, itm->u.item.nLastValue);
					break;
				}
				case kGoodsKind:
                {
					DECODE_XML_PROP_INT_DEFAULT(itm->u.goods.nItemType, chl, "item_type", 0);
                    DECODE_XML_PROP_INT_DEFAULT(itm->u.goods.nEffect, chl, "effect", 0);
					decode_xml_prop_arr_int_default(itm->u.goods.EquipTypes, 2, chl, (void*)"Position", 0);
                    //DECODE_XML_PROP_INT_DEFAULT(itm->u.goods.fromUnit, chl, "uint", 0);
					if (isGemItemType(itm->u.goods.nItemType)) {
						if (itm->u.goods.EquipTypes[0]== 0) {
							ERROR_LOG("gem position err item_id = %u gem_pos = %u", itm->nID, itm->u.goods.EquipTypes[0]);
							return false;
						}
					}
					GemIdVec.push_back(itm->nID);
                	break;
                }
                case kComposeBookKind:
                {
                    DECODE_XML_PROP_INT_DEFAULT(itm->u.book.nEffect, chl, "effect", 0);
                    break;
                }
                case kEquipmentKind:
                {
                    DECODE_XML_PROP_INT(itm->u.equipment.nItemType, chl, "item_type");
                    DECODE_XML_PROP_INT_DEFAULT(itm->u.equipment.nSeriesID, chl, "series", 0);
                    DECODE_XML_PROP_INT_DEFAULT(itm->u.equipment.nEffect, chl, "effect", 0);
                    DECODE_XML_PROP_FLOAT_DEFAULT(itm->u.equipment.fEquipFac, chl, "equip_fac", 0.0);
                    DECODE_XML_PROP_INT_DEFAULT(itm->u.equipment.nBaseHp, chl, "base_hp", 0);
                    DECODE_XML_PROP_FLOAT_DEFAULT(itm->u.equipment.fBaseAtk, chl, "base_outatt", 0);
                    DECODE_XML_PROP_FLOAT_DEFAULT(itm->u.equipment.fBaseInnerAtk, chl, "base_inatt", 0);
                    DECODE_XML_PROP_FLOAT_DEFAULT(itm->u.equipment.fBaseDef, chl, "base_outdef", 0);
                    DECODE_XML_PROP_FLOAT_DEFAULT(itm->u.equipment.fBaseInnerDef, chl, "base_indef", 0);
                    DECODE_XML_PROP_FLOAT_DEFAULT(itm->u.equipment.fBaseCrit, chl, "base_crit", 0);
                    DECODE_XML_PROP_FLOAT_DEFAULT(itm->u.equipment.fBaseDefCrit, chl, "base_defcrit", 0);
                    break;
                }
				case kPartnerSkillKind:
				{
					DECODE_XML_PROP_INT_DEFAULT(itm->u.partnerSkill.nEffect, chl, "effect", 0);
					break;
				}
				case kPartnerCardKind:
				{
					DECODE_XML_PROP_INT_DEFAULT(itm->u.card.nPartnerID, chl, "partnerID", 0);
					DECODE_XML_PROP_INT_DEFAULT(itm->u.card.nEffect, chl, "effect", 0);
					break;
				}
                case kTemporaryItemKind:
                {
                    break;
                }
				case kStarItemKind:
				{
                    DECODE_XML_PROP_INT_DEFAULT(itm->u.star.nItemType, chl, "item_type", 0);
					DECODE_XML_PROP_INT_DEFAULT(itm->u.star.nEffect, chl, "effect", 0);
					DECODE_XML_PROP_INT_DEFAULT(itm->u.star.nBaseExp, chl, "base_exp", 0);
                    DECODE_XML_PROP_INT_DEFAULT(itm->u.star.nBaseHp, chl, "base_hp", 0);
                    DECODE_XML_PROP_INT_DEFAULT(itm->u.star.nBaseAtk, chl, "base_outatt", 0);
                    DECODE_XML_PROP_INT_DEFAULT(itm->u.star.nBaseInnerAtk, chl, "base_inatt", 0);
                    DECODE_XML_PROP_INT_DEFAULT(itm->u.star.nBaseDef, chl, "base_outdef", 0);
                    DECODE_XML_PROP_INT_DEFAULT(itm->u.star.nBaseInnerDef, chl, "base_indef", 0);
                    DECODE_XML_PROP_INT_DEFAULT(itm->u.star.nBaseCrit, chl, "base_crit", 0);
                    DECODE_XML_PROP_INT_DEFAULT(itm->u.star.nBaseDefCrit, chl, "base_defcrit", 0);
                    DECODE_XML_PROP_INT_DEFAULT(itm->u.star.nAddHurt, chl, "addhurt", 0);
                    DECODE_XML_PROP_INT_DEFAULT(itm->u.star.nReduceHurt, chl, "reduce_hurt", 0);
					break;
				}
                default:
                {
                    break;
                }
            }
			m_itemMap.insert(std::pair<uint32_t, item_t*>(itm->nID, itm));
        }
        chl = chl->next;
    }
    return true;
}

void ItemConfig::unload()
{
    ItemMap::iterator it = m_itemMap.begin();
    while (it != m_itemMap.end())
    {
		delete it->second;
		it->second = NULL;
    	++it;
	}
    m_itemMap.clear();
}

const item_t* ItemConfig::getItem(uint32_t itemID) const
{
	ItemMap::const_iterator it = m_itemMap.find(itemID);
	if (it != m_itemMap.end())	
    {
		return it->second;
    }
    
    return NULL;
}

const item_t* ItemConfig::getPreLvGem(uint32_t GemId)
{
	ItemMap::const_iterator it = m_itemMap.find(GemId);
	if (it == m_itemMap.end() || it->second->nLevel <= 1) {
		return NULL;
    }
	uint32_t newGemId = GemId - 1;
	ItemMap::const_iterator itNew = m_itemMap.find(newGemId);
	if (itNew != m_itemMap.end()) {
		if (isKindOfItem(itNew->second->nID, kGoodsKind) 
			&& isGemItemType(itNew->second->u.goods.nItemType)
			&& itNew->second->nLevel + 1 == it->second->nLevel
			&& itNew->second->u.goods.nItemType == it->second->u.goods.nItemType) {
			return itNew->second;
		}
	}

	for (uint32_t i = 0; i < GemIdVec.size(); ++i) {
		itNew = m_itemMap.find(GemIdVec[i]);
		if (itNew != m_itemMap.end() 
			&& itNew->second->nLevel + 1 == it->second->nLevel
			&& itNew->second->u.goods.nItemType == it->second->u.goods.nItemType) {
			return itNew->second;
		}
	}

	return NULL;
}

const item_t* ItemConfig::getNextLvGem(uint32_t GemId)
{
	ItemMap::const_iterator it = m_itemMap.find(GemId);
	if (it == m_itemMap.end() || it->second->nLevel >= GEM_LEVEL_MAX) {
		return NULL;
    }
	uint32_t newGemId = GemId + 1;
	ItemMap::const_iterator itNew = m_itemMap.find(newGemId);
	if (itNew != m_itemMap.end()) {
		if (isKindOfItem(itNew->second->nID, kGoodsKind) 
			&& isGemItemType(itNew->second->u.goods.nItemType)
			&& itNew->second->nLevel == it->second->nLevel + 1
			&& itNew->second->u.goods.nItemType == it->second->u.goods.nItemType) {
			return itNew->second;
		}
    }

	for (uint32_t i = 0; i < GemIdVec.size(); ++i) {
		itNew = m_itemMap.find(GemIdVec[i]);
		if (itNew != m_itemMap.end() 
			&& itNew->second->nLevel == it->second->nLevel + 1
			&& itNew->second->u.goods.nItemType == it->second->u.goods.nItemType) {
			return itNew->second;
		}
	}
	return NULL;
}

#ifdef WONPEE_SERVER
#include <vector>
const item_t* ItemConfig::getRandPartnerCard(uint32_t quarty)
{
	std::vector<item_t*> cardVec;
	ItemMap::const_iterator it = m_itemMap.begin();
	for ( ; it != m_itemMap.end(); ++it) {
		item_t* itm = it->second;
		int itemKind = getKindOfItem(itm->nID);
		if (itemKind == kPartnerCardKind && itm->nQuality == quarty && itm->u.card.nEffect == 0) {
			cardVec.push_back(itm);
		}
    }
	if (cardVec.empty()) {
		return NULL;
	}

	uint32_t rand_idx = rand() % cardVec.size();
	return cardVec[rand_idx];
}
#endif
