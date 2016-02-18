//
//  HCSVFile.cpp
//  xygame
//
//  Created by xingchong on 11/19/14.
//
//

#include "HCSVFile.h"
#include "cocos2d.h"

#ifdef WIN32
#include <windows.h>
#endif

using namespace std;
USING_NS_CC;


//open csv file
bool HCSVFile::openFile(const char* fileName)
{

	m_FileName = fileName;

	string pathKey = FileUtils::getInstance()->fullPathForFilename(fileName);
	char* pBuffer = NULL;
	long bufferSize = 0;
	pBuffer = (char*)FileUtils::getInstance()->getFileData(pathKey.c_str(), "r", &bufferSize);
	/**
	FILE* pFile = fopen(pathKey.c_str(),"rb");
	if( NULL != pFile )
	{
		//fread();
		fseek(pFile, 0, SEEK_END);
		bufferSize = ftell(pFile);
		fseek(pFile, 0, SEEK_SET);
		pBuffer = new char[bufferSize+1]();
		fread((void*)pBuffer,sizeof(char),bufferSize,pFile);
		fclose(pFile);
	}
	*/
	bool ret = createDBFromMemery(pBuffer,bufferSize);
	delete []pBuffer;


	return ret;

}

bool HCSVFile::createDBFromMemery(const char* pMemStart, const unsigned long dwSize)
{

	if ( dwSize <= 0) return false;
	const char* pMemEnd = pMemStart + dwSize;

	char szLine[1024*10] = {0};
	register const char* pMem = pMemStart;
	pMem = GetLineFromMemory(szLine, 1024*10, pMem, pMemEnd );
	if(!pMem) 
		return false;

	
	ConvertStringToVector(szLine, m_vecFieldName, ",");
	if(m_vecFieldName.empty())
		return false;

	int index = 0;
	for (auto str : m_vecFieldName)
	{
		m_mapColomIndex[str] = index++;
	}
	

	pMem = GetLineFromMemory(szLine, 1024*10, pMem, pMemEnd);
	if(!pMem) 
		return false;
	ConvertStringToVector(szLine,m_vecFieldType,",");
	if( m_vecFieldType.empty())
		return false;


	do
	{
		pMem = GetLineFromMemory(szLine, 1024*10, pMem, pMemEnd);
		if(!pMem) break;

		DataSingleRecord  data;
		ConvertStringToVector(szLine, data.m_vecData, ",");
		if(data.m_vecData.empty())
			continue;

		int rowid =atoi((data.m_vecData)[0].c_str());		

#ifdef WIN32
		if (rowid > 100 &&  m_mapFileData.find(rowid) != m_mapFileData.end())
		{
			char temp[256] = "";
			sprintf(temp,"在表格 %s 中,有重复ID [%d] !!!",m_FileName.c_str(),rowid);
			MessageBoxA(0,temp,0,0);
		}
#endif
		
		m_mapFileData[rowid] = data; 

	}while(true);


	return true;
	
}
int	HCSVFile::ConvertStringToVector(const char* strSource, std::vector<string>& vRet, const char* szKey)
{
	vRet.clear();
	if(!strSource || !szKey)
		return 0;
	string strSrc = strSource;
	if(strSrc.empty())
		return 0;

	string::size_type nLeft = 0;
	string::size_type nRight = 0;

	nRight = strSrc.find(szKey);
	if(nRight == wstring::npos)
		nRight = strSrc.length();

	while(true)
	{
		string strItem = strSrc.substr(nLeft, nRight-nLeft);
		if(strItem.length() > 0)
			vRet.push_back(strItem);
		else						
			vRet.push_back("");
		if(nRight == strSrc.length())
			break;
		nLeft = nRight + strlen(szKey);
		nRight = strSrc.find(szKey, nLeft);

		if(nRight == string::npos)
			nRight = strSrc.length();
	}

	return (int)vRet.size();
}
const char*	HCSVFile::GetLineFromMemory(char* pStringBuf, int nBufSize, const char* pMemStart, const char* pMemEnd)
{
	const char* pMem = pMemStart;
	if( pMem>=pMemEnd || *pMem == NULL)
		return NULL;

	while(pMem<pMemEnd && pMem-pMemStart+1<nBufSize && *pMem != 0 && *pMem != '\r' && *pMem != '\n') 
		*(pStringBuf++) = *(pMem++);
	*pStringBuf = 0;
	while(pMem<pMemEnd && *pMem != 0 && (*pMem=='\r' || *pMem=='\n')) 
		pMem++;
	return pMem;
}
const char*	HCSVFile::getData(int id,const char* fieldName)
{


#ifdef WIN32
	if (id == 0 || id == -1)
	{
		char temp[256] = "";
		sprintf(temp,"在表格 %s 中,不能获得ID [ %d ] !!!",fieldName,id);
		MessageBoxA(0,temp,0,0);
	}
#endif

	map<string,int>::iterator it = m_mapColomIndex.find(fieldName);
	int colomIndex = 0;
	if (it != m_mapColomIndex.end())
		colomIndex = it->second;
	else
	{
#ifdef WIN32
		char temp[256] = "";
		sprintf(temp,"在表格 %s 中,找不到字段 [%s] !!!",m_FileName.c_str(),fieldName);


		MessageBoxA(0,temp,0,0);
		//assert(false,temp);
#endif
		
		return "";
	}
		
	

	map<int,DataSingleRecord>::iterator dataIt = m_mapFileData.find(id);
	if (dataIt != m_mapFileData.end())
	{
		return dataIt->second.getData(colomIndex,m_FileName).c_str();
	}
#ifdef WIN32
	char temp[256] = "";
	sprintf(temp,"在表格 %s 中,找不到ID [%d] !!!",m_FileName.c_str(),id);
	MessageBoxA(0,temp,0,0);
#endif
	return "";
}   

const char* HCSVFile::getDataType(const char* fieldName)
{
	map<string,int>::iterator it = m_mapColomIndex.find(fieldName);
	int colomIndex = 0;
	if (it != m_mapColomIndex.end())
		colomIndex = it->second;
	else
		return NULL;

	if (colomIndex < m_vecFieldType.size() )
	{
		return m_vecFieldType[colomIndex].c_str();
	}
	return NULL;
	
}

cocos2d::ValueMap HCSVFile::getRowData(int id)
{


#ifdef WIN32
	if (id == 0 || id == -1)
	{
		char temp[256] = "";
		sprintf(temp,"在表格 %s 中,不能获得ID [ %d ] !!!",m_FileName.c_str(),id);
		MessageBoxA(0,temp,0,0);
	}
#endif

	using namespace cocos2d;

	
	DataSingleRecord* data = NULL;  
	map<int,DataSingleRecord>::iterator it = m_mapFileData.find(id);
	if (it != m_mapFileData.end())
	{
		data = &(it->second);
	}
	else
	{
#ifdef WIN32


		
		int n = m_FileName.find_last_of("/");
		string  s = m_FileName.substr(n+1,7);
		if( s == "battle_")
		{

			char temp[256] = "";
			sprintf(temp,"在表格 %s 中,找不到ID [%d] !!!",m_FileName.c_str(),id);
			MessageBoxA(0,temp,0,0);
		}
#endif
	}

	ValueMap rowMap;
	if (data != NULL)
	{
		for (int i = 0; i< m_vecFieldName.size(); i++)
		{
			string key   = m_vecFieldName[i];
			Value value = Value(data->getData(i,m_FileName));
			rowMap[key] = value;
		}
	}
	return rowMap;

}

vector<int> HCSVFile::getAllId()
{
	using namespace cocos2d;
	vector<int> IDs;
	map<int,DataSingleRecord>::iterator it = m_mapFileData.begin();
	for (;it != m_mapFileData.end(); it++)
	{
		IDs.push_back(it->first);
	}
	return IDs;


}

bool HCSVFile::openXlsFile(const char* fileName)
{
	m_FileName = fileName;
#ifdef WIN32
	Book* book = xlCreateXMLBook();
	if(!book) return false;

	wchar_t* pF = ctow(FileUtils::getInstance()->fullPathForFilename(fileName).c_str());
	if(!book->load(pF))
	{
		return false;
	}

	Sheet* sheet = book->getSheet(0);
	if(!sheet) return false;

	//sheet->readStr()
	//sheet->cellType()

	int iCol = 0;
	string sCol = "";
	while ((sCol = getXlsCellData(sheet, 0, iCol))!="")
	{
		m_vecFieldName.push_back(sCol);
		m_mapColomIndex[sCol] = iCol++;
	}

	for (int i=0; i<iCol; i++)
	{
		m_vecFieldType.push_back(getXlsCellData(sheet, 1, i));
	}

	//支持最多2048行的读取
	for(int j=3; j<2048; j++)
	{
		DataSingleRecord  data;
		for (int k=0; k<iCol; k++)
		{
			string cData = getXlsCellData(sheet, j, k);
			if (cData != "")
			{
				data.m_vecData.push_back(cData);
			}
		}

		if(data.m_vecData.empty()) continue;

		int rowid =atoi((data.m_vecData)[0].c_str());		

		if (rowid > 100 &&  m_mapFileData.find(rowid) != m_mapFileData.end())
		{
			char temp[256] = "";
			sprintf(temp,"在表格 %s 中,有重复ID [%d] !!!",m_FileName.c_str(),rowid);
			MessageBoxA(0,temp,0,0);
		}

		m_mapFileData[rowid] = data;
	}

#endif // WIN32
	
	return true;
}


#ifdef WIN32
char * HCSVFile::w2c(char *pcstr,const wchar_t *pwstr, size_t len)

{

	int nlength=wcslen(pwstr);

	//获取转换后的长度

	int nbytes = WideCharToMultiByte( CP_UTF8, // specify the code page used to perform the conversion

		0,         // no special flags to handle unmapped characters

		pwstr,     // wide character string to convert

		nlength,   // the number of wide characters in that string

		NULL,      // no output buffer given, we just want to know how long it needs to be

		0,

		NULL,      // no replacement character given

		NULL );    // we don't want to know if a character didn't make it through the translation

	// make sure the buffer is big enough for this, making it larger if necessary

	if(nbytes>len)   nbytes=len;

	// 通过以上得到的结果，转换unicode 字符为ascii 字符

	WideCharToMultiByte( CP_UTF8, // specify the code page used to perform the conversion

		0,         // no special flags to handle unmapped characters

		pwstr,   // wide character string to convert

		nlength,   // the number of wide characters in that string

		pcstr, // put the output ascii characters at the end of the buffer

		nbytes,                           // there is at least this much space there

		NULL,      // no replacement character given

		NULL );

	return pcstr ;

}

wchar_t* HCSVFile::ctow(const char *str)

{

	wchar_t* buffer;

	if(str)

	{

		size_t nu = strlen(str);

		size_t n =(size_t)MultiByteToWideChar(CP_ACP,0,(const char *)str,int(nu),NULL,0);

		buffer=0;

		buffer = new wchar_t[n+1];
		memset(buffer, 0, (n+1)*sizeof(wchar_t));

		//if(n>=len) n=len-1;

		::MultiByteToWideChar(CP_ACP,0,(const char *)str,int(nu),buffer,int(n));

	}

	return buffer;
}

std::string HCSVFile::getXlsCellData(Sheet *pSheet, unsigned int row, unsigned int col)
{
	string ret = "";
	if(!pSheet) return ret;

	switch (pSheet->cellType(row,col))
	{
	case CELLTYPE_STRING:
		{
			char szContent[1024];
			memset(szContent, 0, sizeof(szContent));
			w2c(szContent,  pSheet->readStr(row, col), sizeof(szContent)-1);
			ret = szContent;
		}
		break;
	case CELLTYPE_NUMBER:
		{
			char szNum[50];
			sprintf(szNum, "%f", pSheet->readNum(row, col));
			size_t sLen = strlen(szNum);

			int pointIndex = -1;
			for (int i=0; i<sLen; i++)
			{
				if ('.'==szNum[i])
				{
					pointIndex = i;
					break;
				}
			}
			if (pointIndex > 0)
			{
				for (int i=sLen-1; i>pointIndex; i--)
				{
					if ('0' == szNum[i])
					{
						szNum[i] = '\0';
					}
					else
					{
						break;
					}
				}
			}
			sLen = strlen(szNum);
			if ('.'==szNum[sLen-1])
			{
				szNum[sLen-1] = '\0';
			}

			ret = szNum;
		}
		break;
	default:
		break;
	}

	return ret;
}
#endif // WIN32











