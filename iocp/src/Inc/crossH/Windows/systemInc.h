#ifndef _SYSTEM_INC_H_20110818
#define _SYSTEM_INC_H_20110818

#define WIN32_LEAN_AND_MEAN
#if _MSC_VER >= 1400 // for vc8, or vc9 vc10
#pragma  warning(disable:4996 4355) 
#endif
#include <windows.h>
#include <process.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <stdio.h>
#include <tchar.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <io.h>
#include <string.h>
#include <malloc.h>

#endif