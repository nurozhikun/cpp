#ifndef __I_LOG_ZK_H_20111228
#define __I_LOG_ZK_H_20111228

#include "typeDefine.h"

enum LOGGER_FLAGS
{
    ELF_LOG_TO_FILE                 = 1,                              // enable this flag to log to a file *enabled by default*
    ELF_LOG_TO_CONSOLE              = 1 << 1,               // enable this flag to log to the console *enabled by default*
};

enum LOGGER_LEVEL
{
    LEVEL_INFO =  1,
    LEVEL_WARNING = 1 << 1,
    LEVEL_ERROR   =     1 << 2,
};
class CILogZK
{
public:
    CILogZK() {}
    virtual ~CILogZK() {}

    virtual nuVOID  LogTcs(nuTCHAR *ptzString) = 0;
    virtual nuVOID  LogStringA(nuCHAR* szStr) = 0;
    virtual nuVOID  LogStringW(nuWCHAR* wzStr) = 0;
    virtual nuVOID  LogValue(nuCHAR* pszHeader, nuLONG nNum, nuUINT nBase, nuCHAR* pszEnd) = 0;
    virtual nuVOID  LogBuffer(nuCHAR* pBuffer,  nuINT nLen, nuINT cmdType, nuINT ID) = 0;
    virtual nuVOID  Log(nuINT flags, nuINT logger_level, const nuCHAR* format, va_list list) = 0;
	virtual nuVOID  SetLogPath(nuCHAR* logPath) = 0;
};

#endif