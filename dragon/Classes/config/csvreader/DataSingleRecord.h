//
//  CsvReader.h
//  xygame
//
//  Created by huangrui on 11/21/14.
//
//

#ifndef __XYGAME__DATASINGLERECORD__
#define __XYGAME__DATASINGLERECORD__


#include <string>
#include <vector>
#include "cocos2d.h"

USING_NS_CC;

class DataSingleRecord
{
public:
	const Value getData(int colomIndex, std::string& filename);
public:
    
    ValueVector m_vecData;
};

#endif /* defined(__XYGAME__DATASINGLERECORD__) */
