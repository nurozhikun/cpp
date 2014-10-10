#ifdef LOGDB_EXPORTS
#define LOG_API extern "C" __declspec(dllexport)
#else
#define LOG_API extern "C"
#endif
#include "LogDB.h"

static CLogDB*  gs_pLogDB  = NULL;
static int   gs_logsCount    = 0;

LOG_API void* Log_InitModule(void* pParam)
{
    if( NULL == gs_pLogDB )
    {
        gs_pLogDB = new CLogDB();
        gs_pLogDB->SetDBFileName("vehicle.db");
    }
    ++gs_logsCount;
    return gs_pLogDB;
}

LOG_API void Log_FreeModule()
{
    if( gs_logsCount > 0 )
    {
        --gs_logsCount;
    }
    if( 0 == gs_logsCount && NULL != gs_pLogDB )
    {
        delete gs_pLogDB;
        gs_pLogDB   = NULL;
    }
}