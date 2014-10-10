#include "FileIni.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <math.h>

CFileIni::CFileIni()
{

}
CFileIni::CFileIni(const char* _filename)
{
    char tsPath[MAX_PATH];
    int nNum = GetModuleFileNameA(NULL, tsPath, sizeof(tsPath));
    for(int i = nNum - 1; i >= 0; --i)
    {
        if( '\\' == tsPath[i] )
        {
            tsPath[++i] = '\0';
            break;
        }
    }
    string file_name;
    file_name += tsPath;
    file_name += _filename;
    if(ini.LoadFile(file_name.c_str()) < 0 )
    {
        printf("Load %s failed", file_name);
    }

}


CFileIni::~CFileIni()
{
}
bool CFileIni::LoadFile(const char* _filename)
{
	if (ini.LoadFile(_filename) < 0)
	{
		return false;
	}
	return true;
}
bool CFileIni::LoadData(const char* _buffer, size_t _len)
{
	if (ini.LoadData(_buffer, _len) < 0)
	{
		return false;
	}
	return true;
}
void CFileIni::GetAllSections(vector<string>& section_names)
{
     CSimpleIniA::TNamesDepend a_names;
     ini.GetAllSections(a_names);
     CSimpleIniA::TNamesDepend::iterator ite = a_names.begin();
     for(; ite != a_names.end(); ++ite)
     {
         section_names.push_back(ite->pItem);
     }
}

void CFileIni::GetSectionContent(const char* section_name, map<string, string>& section_content)
{
    const CSimpleIni::TKeyVal * pSectionData = ini.GetSection(section_name);
    if (pSectionData) {
        // iterate over all keys and dump the key name and value
        CSimpleIni::TKeyVal::const_iterator iKeyVal = pSectionData->begin();
        for ( ;iKeyVal != pSectionData->end(); ++iKeyVal) {
            section_content[iKeyVal->first.pItem] = iKeyVal->second;
        }
    }
}



void CFileIni::GetInt(const char* section_name, const char* key_name, int& value, int _default)
{
     value = ini.GetLongValue(section_name, key_name, _default);
}

void CFileIni::GetDouble(const char* section_name, const char* key_name,  double& value, double _default)
{
    value = ini.GetDoubleValue(section_name, key_name, _default);
}                

void CFileIni::GetString(const char* section_name, const char* key_name,   string& value, const char* _default)
{
	const char* pRes = ini.GetValue(section_name, key_name, _default);
	if (pRes)
	{
		value = pRes;
	}
	else
	{
		value = "";
	}
}



