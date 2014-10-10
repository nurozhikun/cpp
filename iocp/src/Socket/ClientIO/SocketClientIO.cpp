// SocketClientIO.cpp: implementation of the CSocketClientIO class.
//
//////////////////////////////////////////////////////////////////////

#include "SocketClientIO.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSocketClientIO::CSocketClientIO()
{
    m_pThread           = nuNULL;
    m_bOutThreadWorking = nuFalse;
}

CSocketClientIO::~CSocketClientIO()
{
    ClientEnd();
}

nubool CSocketClientIO::ClientBegin(const CIClientVessel::CLIENT_START_PARAM& cltParam)
{
    if( !m_client.SetServer( cltParam.cspSzServerIp
        , cltParam.cspServerPort
        , cltParam.cspClientID
        , cltParam.cspUserID
        , cltParam.cspUserPw
        , cltParam.cspCallback ) )
    {
        return nuFalse;
    }
    if( cltParam.cspHasOwnerThread )
    {
        m_pThread = new CClientThreadZK(&m_client);
        if( nuNULL == m_pThread )
        {
            return nuFalse;
        }
        if( FINAL_SUCCESS != m_pThread->InitClientThread() )
        {
            return nuFalse;
        }
    }
    return nuTrue;
}

nuVOID CSocketClientIO::ClientEnd()
{
    if( nuNULL != m_pThread )
    {
        m_pThread->FreeClientThread();
        delete m_pThread;
    }
    else
    {
        while( m_bOutThreadWorking )
        {
            nuSleep(25);
        }
    }
    m_client.RelServer();
}

nubool CSocketClientIO::ResetClient(const RESET_PARAM& reParam)
{
	return nuFalse;
}

nuUINT CSocketClientIO::ClientSend(nuUINT nCmd, const nuCHAR* pData, nuUINT nDataLen)
{
    if( m_client.HasLogin() )
    {
        return m_client.SendTo(nCmd, pData, nDataLen);
    }
    else
    {
        return 0;
    }
}

nubool CSocketClientIO::OutThreadBegin()
{
    m_bOutThreadWorking = nuTrue;
    return nuTrue;
}

nubool CSocketClientIO::OutThreadAction()
{
    m_client.ClientAction();
    return nuTrue;
}

nubool CSocketClientIO::OutThreadEnd()
{
    m_client.Logout(CIClientCallBack::CLT_LOG_OUT_DESTROY);
    m_bOutThreadWorking = nuFalse;
    return nuTrue;
}
