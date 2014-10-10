#ifndef _LOAD_CLIENT_IO_H_20111207
#define _LOAD_CLIENT_IO_H_20111207

#define _USE_CLIENTIO_DLL

#ifdef _USE_CLIENTIO_DLL
#include "CrossLoadDllZK.h"
class CLoadClientIO : public CCrossLoadDllZK
#else
#include "typeDefine.h"
extern "C" nuVOID*  ClientIO_InitModule(nuVOID* pParam);
extern "C" nuVOID   ClientIO_FreeModule();
class CLoadClientIO  
#endif
{
public:
    CLoadClientIO()
#ifdef _USE_CLIENTIO_DLL
        :CCrossLoadDllZK(nuTEXT("ClientIO"), nuTEXT("ClientIO_InitModule"), nuTEXT("ClientIO_FreeModule"))
#endif
    {
    }
    virtual ~CLoadClientIO()
    {
        Free();
    }
    nuVOID* Load(const nuTCHAR* ptzDllPath = nuNULL, nuVOID* pParam = nuNULL)
    {
#ifdef _USE_CLIENTIO_DLL
        return LoadDll(ptzDllPath, pParam);
#else
        return ClientIO_InitModule(pParam);
#endif
    }
    nuVOID Free()
    {
#ifdef _USE_CLIENTIO_DLL
        FreeDll();
#else
        ClientIO_FreeModule();
#endif
    }
};

#endif