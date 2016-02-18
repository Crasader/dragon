//
//  ConfigReader.cpp
//  xygame
//
//  Created by xingchong on 11/18/14.
//
//

#include "CsvReader.h"
#include "HCSVFile.h"

CsvReader *CsvReader::s_instance = NULL;

CsvReader::CsvReader()
{
}

CsvReader::~CsvReader()
{
	clear();
	
}

CsvReader *CsvReader::getInstance()
{
    if (!s_instance)
    {
        s_instance = new CsvReader();
    }
    return s_instance;
}

HCSVFile * CsvReader::getCSVFileByName(const char* filename)
{
	using namespace std;

	std::map<std::string,HCSVFile*>::iterator it  = m_mapFiles.find(filename);

	if (it != m_mapFiles.end())
	{
		return it->second;
	}
	
     HCSVFile *csvFile=new HCSVFile();
     string filePath = "config/csv/";	
#ifdef WIN32
	 char tempStr[1024] = "";

	 GetPrivateProfileStringA("path","config","",tempStr,1024,"./init.ini");
	 filePath = tempStr;
#endif
     filePath += filename;
     filePath += ".csv";


     csvFile->openFile(filePath.c_str());

     m_mapFiles[filename] = csvFile;
    
    return csvFile;
}

void CsvReader::clear()
{
	for (auto file : s_instance->m_mapFiles)
	{
		delete file.second;
	}
	s_instance->m_mapFiles.clear();
}



