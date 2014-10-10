#ifndef _LOAD_SERVERIO_H_20111130
#define _LOAD_SERVERIO_H_20111130

#define _USE_SERVERIO_DLL

#ifdef _USE_SERVERIO_DLL
#include "CrossLoadDllZK.h"
class CLoadServerIO : public CCrossLoadDllZK
#else
#include "typeDefine.h"
extern "C" nuVOID*  ServerIO_InitModule(nuVOID* pParam);
extern "C" nuVOID   ServerIO_FreeModule();
class CLoadServerIO  
#endif
{
public:
	CLoadServerIO()
#ifdef _USE_SERVERIO_DLL
		:CCrossLoadDllZK(nuTEXT("ServerIO"), nuTEXT("ServerIO_InitModule"), nuTEXT("ServerIO_FreeModule"))
#endif
	{
	}
	virtual ~CLoadServerIO()
	{
		Free();
	}
	nuVOID* Load(const nuTCHAR* ptzDllPath = nuNULL, nuVOID* pParam = nuNULL)
	{
#ifdef _USE_SERVERIO_DLL
		return LoadDll(ptzDllPath, pParam);
#else
		return ServerIO_InitModule(pParam);
#endif
	}
	nuVOID Free()
	{
#ifdef _USE_SERVERIO_DLL
		FreeDll();
#else
		ServerIO_FreeModule();
#endif
	}
};

#endif