// ClientThreadZK.cpp: implementation of the CClientThreadZK class.
//
//////////////////////////////////////////////////////////////////////

#include "ClientThreadZK.h"
#include "BaseLogZK.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClientThreadZK::CClientThreadZK(CClientSocketZK* pclient)
{
    m_pClient = pclient;
}

CClientThreadZK::~CClientThreadZK()
{
    FreeClientThread();
}

nuFINAL CClientThreadZK::InitClientThread()
{
    if( 0 == BeginThread() )
    {
        printf("print 0 == BeginThread() FINAL_FAILURE\n");
        return FINAL_FAILURE;
    }
    return FINAL_SUCCESS;
}

nuVOID CClientThreadZK::FreeClientThread()
{
    EndThread();
}

nuDWORD CClientThreadZK::ThreadBegin()
{
    return THREAD_WORK;
}

nuDWORD CClientThreadZK::ThreadAction()
{
    m_pClient->ClientAction();
    return THREAD_WORK;
}

nuDWORD CClientThreadZK::ThreadEnd()
{
    m_pClient->Logout(CIClientCallBack::CLT_LOG_OUT_DESTROY);
    return THREAD_WORK;
}
