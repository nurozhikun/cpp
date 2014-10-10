// BaseLogZK.h: interface for the CBaseLogZK class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASELOGZK_H__3045CD9A_2AE8_47BA_8CFA_3F2F9DED0CDB__INCLUDED_)
#define AFX_BASELOGZK_H__3045CD9A_2AE8_47BA_8CFA_3F2F9DED0CDB__INCLUDED_


#define _USE_LOG_DEBUG
///////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _USE_LOG_DEBUG

#include "typeDefine.h"
#include "ILogZK.h"

class CBaseLogZK  
{
public:
	CBaseLogZK();
	virtual ~CBaseLogZK();
    static nuVOID   InitLog();
    static nuVOID   FreeLog();
    static nuVOID   LogTcs(nuTCHAR *t);
    
    static nuVOID  LogStringA(nuCHAR* szStr);
    static nuVOID  LogStringW(nuWCHAR* wzStr);
    static nuVOID  LogValue(nuCHAR* pszHeader, nuLONG nNum, nuUINT nBase, nuCHAR* pszEnd);
    static nuVOID  LogBuffer(nuCHAR* pBuffer,  nuINT nLen, nuINT cmdType, nuINT ID);
    static nuVOID  Log(nuINT flags, nuINT logger_level, const nuCHAR* format, ...);
    static nuVOID  Log(nuCHAR* fmt, ...);
	static nuVOID  SetLogPath(nuCHAR* logPath);

private: 
    static class CLoadLogZK*    s_pLoadLog;
    static class CILogZK*       s_pLog;
};
#define LOG_PATH(p)	                    CBaseLogZK::SetLogPath(p)
#define LOG_INIT                            CBaseLogZK::InitLog()
#define LOG_FREE                            CBaseLogZK::FreeLog()
#define LOG_TCS(t)                          CBaseLogZK::LogTcs(t)
#define LOG_STRA(s)                         CBaseLogZK::LogStringA(s)
#define LOG_STRW(w)                         CBaseLogZK::LogStringW(w)
#define LOG_VALUE(sh, v, nbase, se)         CBaseLogZK::LogValue(sh, v, nbase, se)

#if !defined(_MSC_VER) || _MSC_VER > 1400
#define LOG_INFO_FILE(fmt, ...)             CBaseLogZK::Log(ELF_LOG_TO_FILE, LEVEL_INFO, fmt, __VA_ARGS__);
#define LOG_WARNING_FILE(fmt, ...)          CBaseLogZK::Log(ELF_LOG_TO_FILE, LEVEL_WARNING, fmt, __VA_ARGS__);
#define LOG_ERROR_FILE(fmt, ...)            CBaseLogZK::Log(ELF_LOG_TO_FILE, LEVEL_ERROR, fmt, __VA_ARGS__);
#define LOG_INFO_CONSOLE(fmt, ...)          CBaseLogZK::Log(ELF_LOG_TO_CONSOLE, LEVEL_INFO, fmt, __VA_ARGS__);
#define LOG_WARNING_CONSOLE(fmt, ...)       CBaseLogZK::Log(ELF_LOG_TO_CONSOLE, LEVEL_WARNING, fmt, __VA_ARGS__);
#define LOG_ERROR_CONSOLE(fmt, ...)         CBaseLogZK::Log(ELF_LOG_TO_CONSOLE, LEVEL_ERROR, fmt, __VA_ARGS__);
#define LOG_BUFFER(pBuffer, nLen, cmdType, ID)   CBaseLogZK::LogBuffer(pBuffer, nLen, cmdType, ID);
#else
#define LOG_INFO_FILE                       /##/##/
#define LOG_BUFFER                          /##/##/
#define LOG_INFO_CONSOLE                    /##/##/
#define LOG_ERROR_FILE                      /##/##/

#endif


#else

#define LOG_INIT
#define LOG_FREE
#define LOG_TCS(t)
#define LOG_STRA(s)
#define LOG_STRW(w)
#define LOG_VALUE(sh, v, nbase, se)

#endif

#endif // !defined(AFX_BASELOGZK_H__3045CD9A_2AE8_47BA_8CFA_3F2F9DED0CDB__INCLUDED_)
