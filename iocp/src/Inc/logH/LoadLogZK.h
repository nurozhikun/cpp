#ifndef __LOAD_LOGZK_H_20111228
#define __LOAD_LOGZK_H_20111228

#define _USE_LOG_DLL

#ifdef _USE_LOG_DLL
#include "CrossLoadDllZK.h"
class CLoadLogZK : public CCrossLoadDllZK
#else
#include "typeDefine.h"
extern "C" nuVOID*  Log_InitModule(nuVOID* pParam);
extern "C" nuVOID   Log_FreeModule();
class CLoadLogZK
#endif
{
public:
    CLoadLogZK()
#ifdef _USE_LOG_DLL
    :CCrossLoadDllZK(nuTEXT("Log"), nuTEXT("Log_InitModule"), nuTEXT("Log_FreeModule"))
#endif
    {
    }
    virtual ~CLoadLogZK()
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