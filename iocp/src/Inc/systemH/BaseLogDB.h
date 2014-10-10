#ifndef BASE_LOG_DB_H
#define BASE_LOG_DB_H

#include "typeDefine.h"
#include "ILogDB.h"

class CBaseLogDB
{
public:
    enum emLogLevel
    {
        LEVEL_INFO =  1,
        LEVEL_WARNING = 1 << 1,
        LEVEL_ERROR   =     1 << 2,
    };
    CBaseLogDB();
    virtual ~CBaseLogDB();
    static nuVOID   InitLog();
    static nuVOID   FreeLog();

    static nuVOID  LogServerManager(const nuCHAR* pBuffer,  nuINT nLen, nuINT cmdType, nuINT ID);
    static nuVOID  LogBuffer(nuCHAR* pBuffer,  nuINT nLen, nuINT cmdType, nuINT ID);
    static nuVOID  LogKeyValue(nuCHAR* pKey, nuCHAR* pValue);
    static nuVOID  Log(nuINT logger_level, const nuCHAR* format, ...);

private: 
    static class CLoadLogDB*    s_pLoadLog;
    static class CILogDB*       s_pLog;
};

#define LOGDB_INIT                            CBaseLogDB::InitLog()
#define LOGDB_FREE                            CBaseLogDB::FreeLog()

#if !defined(_MSC_VER) || _MSC_VER > 1400
#define LOGDB_INFO_FILE(fmt, ...)             CBaseLogDB::Log(CBaseLogDB::LEVEL_INFO, fmt, __VA_ARGS__);
#define LOGDB_WARNING_FILE(fmt, ...)          CBaseLogDB::Log(CBaseLogDB::LEVEL_WARNING, fmt, __VA_ARGS__);
#define LOGDB_ERROR_FILE(fmt, ...)            CBaseLogDB::Log(CBaseLogDB::LEVEL_ERROR, fmt, __VA_ARGS__);
#define LOGDB_BUFFER(pBuffer, nLen, cmdType, ID)   CBaseLogDB::LogBuffer(pBuffer, nLen, cmdType, ID);
#define LOGDB_MANAGER(pBuffer, nLen, cmdType, ID)   CBaseLogDB::LogServerManager(pBuffer, nLen, cmdType, ID);
#define LOGDB_KEYVALUE(pKey, pValue)   CBaseLogDB::LogKeyValue(pKey, pValue);
#else
#define LOG_ERROR_FILE                         /##/##/
#define LOG_INFO_FILE                       /##/##/
#define LOG_BUFFER                          /##/##/
#define LOG_ERROR_FILE                      /##/##/
#endif



#endif