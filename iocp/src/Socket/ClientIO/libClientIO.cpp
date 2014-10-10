#include "ClientVesselZK.h"

static CClientVesselZK*     gs_pClientVesselZK = nuNULL;
static UINT					gs_nVesselCount = 0;

#ifdef CLIENTIO_EXPORTS
#define CLIENTIO_API extern "C" __declspec(dllexport)
#else
#define CLIENTIO_API extern "C"
#endif


CLIENTIO_API nuVOID* ClientIO_InitModule(nuVOID* pParam)
{
	if( nuNULL == gs_pClientVesselZK )
	{
		LOG_INIT;
		gs_pClientVesselZK = new CClientVesselZK();
	}
	++gs_nVesselCount;
	return gs_pClientVesselZK;
}

CLIENTIO_API nuVOID ClientIO_FreeModule()
{
	--gs_nVesselCount;
	if( 0 == gs_nVesselCount && gs_pClientVesselZK )
	{
		LOG_FREE;
		delete gs_pClientVesselZK;
		gs_pClientVesselZK = nuNULL;
	}
}