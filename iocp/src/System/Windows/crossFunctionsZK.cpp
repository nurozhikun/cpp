#include "crossFunctionsZK.h"
#include "systemInc.h"

nuDWORD nuGetCurrentThreadId()
{
    return GetCurrentThreadId();
}
nuVOID  nuSleep(nuDWORD nTms)
{
    Sleep(nTms);
}
nuDWORD nuGetTickCount()
{
    return GetTickCount();
}
nuVOID* nuMalloc(nuUINT nSize)
{
    return malloc(nSize);
}
nuVOID  nuFree(nuVOID* p)
{
    if( p )
    {
        free(p);
    }
}

nuINT nuItoa(int a, nuCHAR* pBuffer, nuUINT nSize, nuINT nRadix)
{
#if _MSC_VER > 13100
    return _itoa_s(a, pBuffer, nSize, nRadix);
#else
    _itoa(a, pBuffer, nRadix);
    return 0;
#endif
}