#ifdef LOG_EXPORTS
#define LOG_API extern "C" __declspec(dllexport)
#else
#define LOG_API extern "C"
#endif

#include "LogZK.h"

static CLogZK*  gs_pLogZK       = nuNULL;
static nuUINT   gs_logsCount    = 0;

LOG_API nuVOID* Log_InitModule(nuVOID* pParam)
{
    if( nuNULL == gs_pLogZK )
    {
        gs_pLogZK = new CLogZK();
    }
    ++gs_logsCount;
    return gs_pLogZK;
}

LOG_API nuVOID Log_FreeModule()
{
    if( gs_logsCount > 0 )
    {
        --gs_logsCount;
    }
    if( 0 == gs_logsCount && nuNULL != gs_pLogZK )
    {
        delete gs_pLogZK;
        gs_pLogZK   = nuNULL;
    }
}