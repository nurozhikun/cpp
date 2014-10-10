
#include "SocketServerIO.h"
//#include <vld.h>
#include "BaseLogZK.h"
#include <list>

static std::list<CSocketServerIO*> gs_pSocketServerIOList;

#ifdef SERVERIO_EXPORTS
#define SERVERIO_API extern "C" __declspec(dllexport)
#else
#define SERVERIO_API extern "C"
#endif


SERVERIO_API nuVOID* ServerIO_InitModule(nuVOID* pParam)
{
    LOG_INIT;
	CSocketServerIO* pServerIO = new CSocketServerIO();
	if (pServerIO)
	{
		gs_pSocketServerIOList.push_back(pServerIO);
	}
    return pServerIO;
}

SERVERIO_API nuVOID ServerIO_FreeModule()
{	
	for (std::list<CSocketServerIO*>::iterator it = gs_pSocketServerIOList.begin(); it!=gs_pSocketServerIOList.end();++it)
	{
		delete *it;
		*it = NULL;
	}
	gs_pSocketServerIOList.clear();
    LOG_FREE;
}
