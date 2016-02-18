#include "XlsReader.h"
#include "HCSVFile.h"


XlsReader * XlsReader::s_instance = NULL;

XlsReader::XlsReader(void)
{
}


XlsReader::~XlsReader(void)
{
	for (auto file : m_mapFiles)
	{
		delete file.second;
	}
	m_mapFiles.clear();
}

XlsReader * XlsReader::getInstance()
{
	if(!s_instance)
	{
		s_instance = new XlsReader();
	}

	return s_instance;
}

HCSVFile* XlsReader::getCSVFileByName(const char* filename)
{
	map<string,HCSVFile*>::iterator it  = m_mapFiles.find(filename);

	if (it != m_mapFiles.end())
	{
		return it->second;
	}

	HCSVFile *csvFile=new HCSVFile();
	string filePath = "config/xls/";
#ifdef WIN32
	char tempStr[1024] = "";

	GetPrivateProfileStringA("path","xlsconfigP","",tempStr,1024,"./init.ini");
	filePath = tempStr;
#endif
	filePath += filename;
	filePath += ".xlsx";


	csvFile->openXlsFile(filePath.c_str());

	m_mapFiles[filename] = csvFile;

	return csvFile;
}

void XlsReader::cleanup()
{
	for (auto file : s_instance->m_mapFiles)
	{
		delete file.second;
	}
	s_instance->m_mapFiles.clear();
}


