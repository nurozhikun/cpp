// Copyright (c) 2011 Commnet Corp.  All rights reserved. 
// Author: Wu Zhikun
// Date: 2011-8-18
// FileName: typeDefine.h
#ifndef __TYPE_DEFINE_H_20110818
#define __TYPE_DEFINE_H_20110818

#include "corssDefine.h"
//#include <string>
//#include "Logger.h"
typedef void			nuVOID;
typedef bool			nuBoolean, nubool;
typedef int				nuBOOL;
typedef char			nuCHAR, nuINT8;
typedef unsigned char	nuBYTE, nuUINT8;
typedef short			nuSHORT, nuINT16;
typedef unsigned short	nuWORD, nuUINT16;
typedef int				nuINT;
typedef unsigned int	nuUINT;
typedef long			nuLONG, nuINT32;
typedef unsigned long	nuDWORD, nuUINT32;
typedef double          nuDOUBLE;
typedef float           nuFLOAT;
typedef void*			nuHOBJECT;
#if !defined(_MSC_VER) || _MSC_VER > 1400
typedef wchar_t			nuWCHAR;
#else
typedef unsigned short	nuWCHAR;
#endif
typedef int             nuFINAL;
typedef FILE            nuFILE;
//typedef std::string     nuSTRING;
typedef time_t          nuTIME;
typedef unsigned int    nuSIZE;
typedef struct tagnuPOINT
{
    nuLONG  x;
    nuLONG  y;
}nuPOINT;


//for locking the 
/*typedef volatile bool   nuLOCK;*/
#define nuLOCK          nubool
#define nuLocked        true
#define nuUnlokced      false

enum emFinal
{
    FINAL_SUCCESS   = 0,
    FINAL_FAILURE   = 1,
};

#define nuSTDCALL							__stdcall

#define nuTrue								true
#define nuFalse								false
#define nuNULL								0
#define nuINVALID_OBJECT                    ((nuHOBJECT)(-1))
#define nuINFINITE                          0xFFFFFFFF
#define nuINVALID_INDEX                     ((nuUINT)(-1))

#ifdef _UNICODE
#define _NU_PATH_UNICODE
typedef unsigned short						nuTCHAR;
#define nuTEXT(x)                           L##x
#else//end _UNICODE
typedef char								nuTCHAR;
#define nuTEXT(x)                           x
#endif

#define nuL(a)								L##a

#ifndef NU_MAX_PATH
#define NU_MAX_PATH							256
#endif

#endif