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

class DataSingleRecord
{
public:
	const std::string& getData(int colomIndex,std::string& filename); 
public:
    std::vector<std::string> m_vecData;

	//static const char splitchar = '|';
};

#endif /* defined(__XYGAME__DATASINGLERECORD__) */
