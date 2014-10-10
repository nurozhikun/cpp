
#include "crossFunctionsZK.h"
#include "systemInc.h"

nuDWORD nuGetCurrentThreadId()
{
    return pthread_self();
}
nuVOID  nuSleep(nuDWORD nTms)
{
    nuDWORD nTempTime;
    nTempTime = nTms/1000;
    if(nTempTime==0)
    {
        sleep(1);
    }
    else
    {
        sleep(nTempTime);
    }
}
nuDWORD nuGetTickCount()
{
    struct timeval current;
    nuDWORD currentTimeMS =0;
    gettimeofday(&current, NULL);  
    currentTimeMS = current.tv_sec * 1000 + current.tv_usec/1000;  
    return currentTimeMS;//return time by ms of daytime
	//struct sysinfo s_info;
	//nuDWORD reTimeMS = 0;
	//int error;
	//error = sysinfo(&s_info);
	//reTimeMS = s_info.uptime*1000;
    //return reTimeMS;//return time by second since boot-to-now
}

nuLONG nuGetTime(nuLONG* pTime)
{
    nuLONG nTm = time((time_t *)pTime);
    return nTm;
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
