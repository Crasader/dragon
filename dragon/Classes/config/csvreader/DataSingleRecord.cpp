//
//  ConfigReader.cpp
//  xygame
//
//  Created by huangrui on 11/21/14.
//
//

#include "DataSingleRecord.h"
#include <assert.h>


#include <sstream>

#ifdef WIN32
#include <windows.h>
#endif
using namespace std;

const std::string& DataSingleRecord::getData(int colomIndex,std::string& filename)
{

#ifdef WIN32

	if (colomIndex >= m_vecData.size())
	{
		char temp[256] = "";
		sprintf(temp,"%s ������һ��û����� ���� ע���������������ID",filename.c_str());
		MessageBoxA(0,temp,0,0);
	}
#endif		
	
	if ( colomIndex < m_vecData.size())
		return m_vecData[colomIndex];
	return "";
}
