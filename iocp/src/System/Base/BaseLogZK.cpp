// BaseLogZK.cpp: implementation of the CBaseLogZK class.
//
//////////////////////////////////////////////////////////////////////

#include "BaseLogZK.h"
//#include <stdarg.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#ifdef _USE_LOG_DEBUG

#include "LoadLogZK.h"
#include "ILogZK.h"

class CLoadLogZK* CBaseLogZK::s_pLoadLog;
class CILogZK* CBaseLogZK::s_pLog = nuNULL;
CBaseLogZK::CBaseLogZK()
{
    
}

CBaseLogZK::~CBaseLogZK()
{

}

nuVOID  CBaseLogZK::SetLogPath(nuCHAR* logPath)
{
	if( s_pLog != NULL)
		s_pLog->SetLogPath(logPath);
}
nuVOID CBaseLogZK::InitLog()
{
    if( nuNULL == s_pLoadLog )
    {
        s_pLoadLog = new CLoadLogZK();
    }
    if( nuNULL == s_pLog && nuNULL != s_pLoadLog )
    {
        s_pLog = (CILogZK*)s_pLoadLog->Load();
    }
}

nuVOID CBaseLogZK::FreeLog()
{
    if( s_pLoadLog )
    {
        s_pLoadLog->Free();
        delete  s_pLoadLog;
        s_pLoadLog = nuNULL;
        s_pLog  = nuNULL;
    }
}

nuVOID CBaseLogZK::LogTcs(nuTCHAR *t) 
{ 
    if( s_pLog ) 
    {
        s_pLog->LogTcs(t); 
    }
}

nuVOID CBaseLogZK::LogStringA(nuCHAR* szStr)
{
    if( s_pLog )
    {
        s_pLog->LogStringA(szStr);
    }
}

nuVOID CBaseLogZK::LogStringW(nuWCHAR* wzStr)
{
    if( s_pLog )
    {
        s_pLog->LogStringW(wzStr);
    }
}

nuVOID CBaseLogZK::LogValue(nuCHAR* pszHeader, nuLONG nNum, nuUINT nBase, nuCHAR* pszEnd)
{
    if( s_pLog )
    {
        s_pLog->LogValue(pszHeader, nNum, nBase, pszEnd);
    }
}


nuVOID  CBaseLogZK::LogBuffer(nuCHAR* pBuffer,  nuINT nLen, nuINT cmdType, nuINT ID)
{
    if( s_pLog )
    {
        s_pLog->LogBuffer(pBuffer, nLen, cmdType, ID);
    }
}

nuVOID   CBaseLogZK::Log(nuINT flags, nuINT logger_level, const nuCHAR* format, ...)
{
    if( s_pLog )
    {
        va_list list;
        va_start(list, format);
        s_pLog->Log(flags, logger_level, format, list);
    }
}
#endif
