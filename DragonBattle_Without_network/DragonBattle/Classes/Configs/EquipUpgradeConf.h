#ifndef EQUIP_UPGRADE_CONF_H__
#define EQUIP_UPGRADE_CONF_H__

#include "XmlEncrypt.h"
#include <map>
#include "ItemConf.h"

struct EquipUpgradeConf_t
{
	enum {
		ITEM_ARR_LEN = 6,//材料数组长度
	};
	uint32_t nEquipmentID;
	uint32_t nNextEquipmentID;
	uint32_t nNeedBookID;
	uint32_t nNeedYxb;
	ItemUnit_t itemArray[ITEM_ARR_LEN];
};

typedef std::map<uint32_t, EquipUpgradeConf_t*> EquipUpgradeMap;

class EquipUpgradeConfig
{
public:
	EquipUpgradeConfig();
	~EquipUpgradeConfig();

	bool loadFromFile(const char* filePath, bool bEncrypt = false);
	void unload();

	const EquipUpgradeConf_t* getEquipUpgrade(uint32_t swapID) const;

private:
	EquipUpgradeMap m_equipUpgradeMap;
};

#endif
