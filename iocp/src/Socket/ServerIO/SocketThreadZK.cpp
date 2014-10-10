// SocketThreadZK.cpp: implementation of the CSocketThreadZK class.
//
//////////////////////////////////////////////////////////////////////

#include "SocketThreadZK.h"
#include "SockServerZK.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSocketThreadZK::CSocketThreadZK()
{
    m_pSockServer   = nuNULL;
}

CSocketThreadZK::~CSocketThreadZK()
{

}

// nuDWORD CSocketThreadZK::ThreadBegin()
// {
//     return 1;
// }

nuDWORD CSocketThreadZK::ThreadAction()
{
    if( nuNULL == m_pSockServer ||
        CSockServerZK::IO_KEY_QUIT == m_pSockServer->IOCompletionProc() )
    {
        return THREAD_EXIT;
    }
    else
    {
        return THREAD_KEPTWORK;
    }
}

nuDWORD CSocketThreadZK::ThreadEnd()
{
    return 1;
}

// nuDWORD CSocketThreadZK::MessageProc(CBaseCodeListZK::BASE_LIST_NODE& node)
// {
//     return 1;
// }

nuFINAL CSocketThreadZK::StartThread(class CSockServerZK* pSockServer)
{
    if( nuNULL == pSockServer )
    {
        return FINAL_FAILURE;
    }
    m_pSockServer = pSockServer;
    if( !BeginThread() )
    {
        return FINAL_FAILURE;
    }
    return FINAL_SUCCESS;
}

nuFINAL CSocketThreadZK::CloseThread()
{
    EndThread();
    m_pSockServer = nuNULL;
    return FINAL_SUCCESS;
}