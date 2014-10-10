#ifndef __LOAD_LOGDB_H
#define __LOAD_LOGDB_H

#define _USE_LOG_DLL

#ifdef _USE_LOG_DLL
#include "CrossLoadDllZK.h"
class CLoadLogDB : public CCrossLoadDllZK
#else
#include "typeDefine.h"
extern "C" nuVOID*  Log_InitModule(nuVOID* pParam);
extern "C" nuVOID   Log_FreeModule();
class CLoadLogDB
#endif
{
public:
    CLoadLogDB()
#ifdef _USE_LOG_DLL
        :CCrossLoadDllZK(nuTEXT("LogSqlite"), nuTEXT("Log_InitModule"), nuTEXT("Log_FreeModule"))
#endif
    {
    }
    virtual ~CLoadLogDB()
    {
        Free();
    }
    nuVOID* Load(const nuTCHAR* ptzDllPath = nuNULL, nuVOID* pParam = nuNULL)
    {
#ifdef _USE_LOG_DLL
        return LoadDll(ptzDllPath, pParam);
#else
        return Log_InitModule(pParam);
#endif
    }
    nuVOID Free()
    {
#ifdef _USE_LOG_DLL
        FreeDll();
#else
        Log_FreeModule();
#endif
    }
};


#endif