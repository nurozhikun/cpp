// ClientCallbackZK.cpp: implementation of the CClientCallbackZK class.
//
//////////////////////////////////////////////////////////////////////

#include "ClientCallbackZK.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClientCallbackZK::CClientCallbackZK()
{

}

CClientCallbackZK::~CClientCallbackZK()
{

}

nuUINT CClientCallbackZK::ClientLogin(nuUINT nLoginMode)
{
    return 0;
}

nuVOID CClientCallbackZK::ClientLogout(nuUINT nOutType)
{
    ;
}

nuVOID CClientCallbackZK::ClientWritable(nubool bWrite)
{
    ;
}

nuUINT CClientCallbackZK::ClientRecved(RECV_DATA& recvData)
{
    return 0;
}

nuUINT CClientCallbackZK::ServerReplyed(RECV_DATA& recvData)
{
    return 0;
}