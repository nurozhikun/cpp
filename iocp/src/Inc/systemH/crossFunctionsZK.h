#ifndef _CROSS_FUNCTIONS_ZK_H_20110826
#define _CROSS_FUNCTIONS_ZK_H_20110826

#include "typeDefine.h"

#define _USE_TEST

////////////////////////////////////////////////////////////////////////////////////////////////////////
#define _USE_DEBUG
#ifdef _USE_DEBUG

#include "CrossDebugOutZK.h"

#define nuDebugOutStrA(s)       CCrossDebugOutZK::DebugOutStringA(s)
#define nuDebugOutInteger(a)    CCrossDebugOutZK::DebugOutInteger(a)

#else

#define nuDebugOutStrA(s)
#define nuDebugOutInteger(a)

#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////
#define _USE_CLIB_MICRO

#ifdef _USE_CLIB_MICRO

#include "systemInc.h"

#ifdef LINUX_LEAN_AND_MEAN  
//define in systemInc.h

#define nuTcscpy        strcpy
#define nuTcsncpy       strncpy
#define nuTcscat        strcat
#define nuTcslen        strlen
#define nuTcscmp        strcmp

#define nuStrcpy        strcpy
#define nuStrncpy       strncpy
#define nuStrcat        strcat
#define nuStrlen        strlen
#define nuStrcmp        strcmp

#define nuMemset        memset
#define nuMemcpy        memcpy
#define nuMemmove       memmove

#define nuAtol          atol

#else 
#define nuTcscpy        _tcscpy
#define nuTcsncpy       _tcsncpy
#define nuTcscat        _tcscat
#define nuTcslen        _tcslen
#define nuTcscmp        _tcscmp
#define nuItots         _itot

#define nuStrcpy        strcpy
#define nuStrncpy       strncpy
#define nuStrcat        strcat
#define nuStrlen        strlen
#define nuStrcmp        strcmp
#define nuStrncmp		strncmp

#define nuMemset        memset
#define nuMemcpy        memcpy
#define nuMemmove       memmove

#define nuWcscpy		wcscpy
#define nuWcsncpy		wcsncpy

#define nuAtol          atol
#define nuAtof          atof
#endif

#else //_USE_CLIB_MICRO


#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////


nuDWORD nuGetCurrentThreadId();
nuVOID  nuSleep(nuDWORD nTms);
nuDWORD nuGetTickCount();
nuVOID* nuMalloc(nuUINT nSize);
nuVOID  nuFree(nuVOID* p);

nuINT   nuItoa(int a, nuCHAR* pBuffer, nuUINT nSize, nuINT nRadix);
nuLONG  nuGetTime(nuLONG* pTime);
#endif