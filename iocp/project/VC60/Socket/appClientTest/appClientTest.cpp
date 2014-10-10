#include <stdio.h>
#include <iostream>
#include <tchar.h>
#include "systemInc.h"
#include "LoadClientIO.h"
#include "ISocketClientIO.h"
#include "ClientCallbackZK.h"
using namespace std;

#define _CLIENTS          100
int main(int argc, char* argv[])
{
    TCHAR tsPath[MAX_PATH];
    DWORD nNum = GetModuleFileName(NULL, tsPath, sizeof(tsPath));
    for(int i = nNum - 1; i >= 0; --i)
    {
        if( '\\' == tsPath[i] )
        {
            tsPath[++i] = '\0';
            break;
        }
	}
    CLoadClientIO   loadclient;
    CIClientVessel* pVessel = (CIClientVessel*)loadclient.Load(tsPath);
    if( nuNULL == pVessel )
    {
        return 0;
    }
    if( !pVessel->InitClientVessel(_CLIENTS) )
    {
        return 0;
    }
    CClientCallbackZK callback;
    CIClientVessel::CLIENT_START_PARAM cltParam;
    cltParam.cspCallback        = &callback;
    cltParam.cspHasOwnerThread  = nuTrue;
    cltParam.cspServerPort      = 5070;
    cltParam.cspSzServerIp      = "192.168.110.176";
    cltParam.cspTsPath          = tsPath;
    cltParam.cspClientID        = 1001;
    cltParam.cspUserID          = "admin";
    cltParam.cspUserPw          = "123456";
    CISocketClientIO* ppClient[_CLIENTS];
    memset(ppClient, 0, sizeof(ppClient));
    for( i = 0; i < _CLIENTS; ++i )
    {
        cltParam.cspClientID = 100 + i;
        ppClient[i] = (CISocketClientIO*)pVessel->CreateClient(cltParam);
        if( nuNULL == ppClient[i] )
        {
            break;
        }
        Sleep(100);
    }
    char cEnter = 0;
    while( 'Q' != cEnter )
    {
        cin >> cEnter;
	}
    for( i = _CLIENTS - 1; i >= 0; --i )
    {
        if( ppClient[i] )
        {
            pVessel->DeleteClient(ppClient[i]);
        }
    }
    pVessel->FreeClientVessel();
    return 0;
}