// BaseLogZK.cpp: implementation of the CBaseLogDB class.
//
//////////////////////////////////////////////////////////////////////

#include "BaseLogDB.h"

#include "LoadLogDB.h"
#include "ILogDB.h"

class CLoadLogDB* CBaseLogDB::s_pLoadLog;
class CILogDB* CBaseLogDB::s_pLog = nuNULL;
CBaseLogDB::CBaseLogDB()
{

}

CBaseLogDB::~CBaseLogDB()
{

}

nuVOID CBaseLogDB::InitLog()
{
    if( nuNULL == s_pLoadLog )
    {
        s_pLoadLog = new CLoadLogDB();
    }
    if( nuNULL == s_pLog && nuNULL != s_pLoadLog )
    {
        s_pLog = (CILogDB*)s_pLoadLog->Load();
    }
}

nuVOID CBaseLogDB::FreeLog()
{
    if( s_pLoadLog )
    {
        s_pLoadLog->Free();
        delete  s_pLoadLog;
        s_pLoadLog = nuNULL;
        s_pLog  = nuNULL;
    }
}
nuVOID  CBaseLogDB::LogBuffer(nuCHAR* pBuffer,  nuINT nLen, nuINT cmdType, nuINT ID)
{
    if( s_pLog )
    {
        s_pLog->LogBuffer(pBuffer, nLen, cmdType, ID);
    }
}

nuVOID  CBaseLogDB::LogServerManager(const nuCHAR* pBuffer,  nuINT nLen, nuINT cmdType, nuINT ID)
{
    if( s_pLog )
    {
        s_pLog->LogServerManager(pBuffer, nLen, cmdType, ID);
    }
}

nuVOID CBaseLogDB::LogKeyValue(nuCHAR* pKey, nuCHAR* pValue)
{
    if( s_pLog )
    {
        s_pLog->LogKeyValue(pKey, pValue);
    }
}

nuVOID   CBaseLogDB::Log(nuINT logger_level, const nuCHAR* format, ...)
{
    if( s_pLog )
    {
        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        va_list list;
        va_start(list, format);
        vsprintf(buffer, format, list);
        s_pLog->Log(logger_level, buffer);
        va_end(list);
    }
}

