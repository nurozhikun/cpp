// ClientSocketZK.cpp: implementation of the CClientSocketZK class.
//
//////////////////////////////////////////////////////////////////////

#include "ClientSocketZK.h"
#include "CCriticalLock.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClientSocketZK::CClientSocketZK()
:m_protocol(this)
{
    m_sockAddr.sinFamily    = SOCK_AF_INET;
    m_sockAddr.sinPort      = 0;
    m_sockAddr.sinAddr[0]   = 0;
    m_bNeedCreate           = nuFalse;
    m_bWrite                = nuFalse;
    m_bNeedConnect          = nuFalse;
    m_bConnected            = nuFalse;
    m_bLogin                = nuFalse;
    nuMemset(m_userData, 0, sizeof(m_userData));
    nuMemset(&m_buffRecv, 0, sizeof(m_buffRecv));
    m_buffRecv.sbBuffer.nBufferLen  = CProtocolBaseZK::CMD_INFO_MAX_SIZE;//CONST_RECV_BUFFER_SIZE;
    m_buffRecv.sbBuffer.pBuffer     = new nuCHAR[m_buffRecv.sbBuffer.nBufferLen];
    nuMemset(&m_buffSend, 0, sizeof(m_buffSend));
    m_buffSend.sbBuffer.nBufferLen  = CProtocolBaseZK::CMD_INFO_MAX_SIZE;//CONST_SEND_BUFFER_SIZE;
    m_buffSend.sbBuffer.pBuffer     = new nuCHAR[m_buffSend.sbBuffer.nBufferLen];
    m_pCallback     = nuNULL;
    m_nLoginTicks   = 0;
    m_bLock         = nuFalse;
	m_nLoginError   = 0;
	InitializeCriticalSection(&m_bSection);
}

CClientSocketZK::~CClientSocketZK()
{
    DeleteClient();
    delete[] m_buffSend.sbBuffer.pBuffer;
    delete[] m_buffRecv.sbBuffer.pBuffer;
	DeleteCriticalSection(&m_bSection);
}

nubool CClientSocketZK::SetServer(nuCHAR* szServerIp, nuUINT szPort, nuUINT nClienID, nuCHAR* szUserID, nuCHAR* szUserPw, CIClientCallBack* pCallback)
{
    if( !StartUp() )
    {
        return nuFalse;
    }
    m_sockAddr.sinPort  = szPort;
    nuStrcpy(m_sockAddr.sinAddr, szServerIp);
	nuMemset(m_userData, 0, sizeof(m_userData));
    nuStrncpy(m_userData, szUserID, SCC_USER_ID_SIZE);
    nuStrncpy(&m_userData[SCC_USER_ID_SIZE], szUserPw, SCC_USER_PW_SIZE);
    m_nClientID     = nClienID;
    m_bNeedCreate   = nuTrue;
	m_pCallback     = pCallback;
	m_nLoginError   = 0;
    return nuTrue;
}

nuVOID CClientSocketZK::RelServer()
{
    CleanUp();
}

nubool CClientSocketZK::CreateClient()
{
    SOCKET_IN sockIn;
    sockIn.siAF         = SOCK_AF_INET;
    sockIn.siType       = SOCK_T_STREAM;
    sockIn.siProtocol   = SOCK_IPPROTO_TCP;
    sockIn.siFlag       = SOCK_FLAG_NONBLOCKING;
    if( !SockCreate(&sockIn) )
    {
        return nuFalse;
    }
    m_bNeedConnect  = nuTrue;
    return nuTrue;
}

nuVOID CClientSocketZK::DeleteClient()
{
    if( m_bConnected )
    {
        SockShutdown();
        m_bWrite        = nuFalse;
        m_bConnected    = nuFalse;
        m_bLogin        = nuFalse;
        m_pCallback->ClientWritable(m_bWrite);
    }
    SockClose();
}

nubool CClientSocketZK::ConnectServer()
{
    m_bConnected        = nuFalse;
    int nRet = SockConnect(m_sockAddr);
    if( SOCK_ACTION_SUCCESS == nRet )
    {
        m_bWrite        = nuTrue;
        m_bConnected    = nuTrue;
    }
    if( SOCK_ACTION_SUCCESS != nRet &&
        SOCK_ACTION_PENDING != nRet )
    {
        return nuFalse;
    }
    return nuTrue;
}


nubool CClientSocketZK::ClientAction()
{
	if( nuNULL != m_pCallback )
	{
		m_pCallback->FrequentCall();
	}
	if (m_nLoginError)
	{
		return nuFalse;
	}
    if( m_bNeedCreate )
    {
        if( !CreateClient() )
        {
            return nuFalse;
        }
        m_bNeedCreate = nuFalse;
    }
    if( m_bNeedConnect )
    {
        if( !ConnectServer() )
        {
            return nuFalse;
        }
        m_bNeedConnect = nuFalse;
    }
    Logining();
    HeardBeatSend();
    SOCKET_SELECT   sockSelect;
    sockSelect.ssCount  = 0;
    sockSelect.ssTimeMs = 1000;
    sockSelect.sspSocketsList[sockSelect.ssCount]   = this;
    sockSelect.ssSockState[sockSelect.ssCount]      = SOCK_SELECT_READ/*|SOCK_SELECT_ERROR*/;
    if( !m_bNeedConnect )
    {
        sockSelect.ssSockState[sockSelect.ssCount] |= SOCK_SELECT_ERROR;
    }
    //if( !m_bWrite )
    //{
    //    sockSelect.ssSockState[sockSelect.ssCount] |= SOCK_SELECT_WRITE;
    //}
    sockSelect.ssSockState[sockSelect.ssCount] |= SOCK_SELECT_WRITE;
    ++(sockSelect.ssCount);
    if( 0 == SockSelect(sockSelect) )
    {
//         Reconnect();
        return nuFalse;
    }
    nuBYTE nState = sockSelect.ssSockState[0];
    if( SOCK_SELECT_READ&nState )
    {
        FdRead();
    }
    if( SOCK_SELECT_WRITE&nState )
    {
        FdWrite();
    }
    if( SOCK_SELECT_ERROR&nState )
    {
        FdError();
    }
    return nuTrue;
}

nuVOID CClientSocketZK::Logining()
{
	if( m_bWrite && !m_bLogin && !m_nLoginError)
    {
        if( nuGetTickCount() - m_nLoginTicks >= CONST_USER_LOGIN_INTERVAL )
        {
            m_nLoginTicks = nuGetTickCount();
            SendTo(CMD_CLIENT_LOGIN, m_userData, SCC_USER_ID_SIZE + SCC_USER_PW_SIZE);
        }
    }
}

nuVOID CClientSocketZK::HeardBeatSend()
{
    if( m_bLogin )
    {
        if( nuGetTickCount() - m_nHeartBeatTicks > CONST_USER_HEARTBEAT_INTERVAL )
        {
            ++m_nHeartBeatCount;
            m_nHeartBeatTicks = nuGetTickCount();
            SendTo(CMD_CLIENT_HEARTBEAT, (nuCHAR*)(&m_nHeartBeatCount), 4);
        }
    }
}

nuVOID CClientSocketZK::HeardBeatRecv()
{
    ;
}

nuVOID CClientSocketZK::Login(const nuCHAR* pdata, nuUINT nLen)
{
    if( pdata == nuNULL || nLen != 4)
        return;

    nuINT nRetValue = -1;
    nuMemcpy(&nRetValue, pdata, sizeof(nuINT));
    if( nRetValue > 0)
    {
        LOG_ERROR_FILE("login failed! error code:%d \n", nRetValue);
		m_nLoginError = nRetValue;
		m_pCallback->ClientLogin(nRetValue);
        return;
    }
    if( !m_bLogin )
    {
        //...
        nuUINT nLoginMode = CIClientCallBack::LOG_IN_SUCESS;
        /*
        if( nuNULL != pdata && nLen > 4 )
        {
            nLoginMode = *((nuUINT*)pdata);
            m_bLogin = nuTrue;
            m_nHeartBeatCount   = 0;
            m_nLoginTicks = m_nHeartBeatTicks = nuGetTickCount();
        }*/
        m_bLogin = nuTrue;
		m_nLoginError = 0;
        m_nHeartBeatCount   = 0;
        m_nLoginTicks = m_nHeartBeatTicks = nuGetTickCount();
        //
        m_pCallback->ClientLogin(nLoginMode);
    }
}

nuVOID CClientSocketZK::Logout(nuUINT nOutType)
{
    if( m_bLogin )
    {
        m_bLogin = nuFalse;
        m_pCallback->ClientLogout(nOutType);
    }
    DeleteClient();
}

nuUINT CClientSocketZK::FdWrite()
{
    m_bWrite        = nuTrue;
    m_bConnected    = nuTrue;
    m_pCallback->ClientWritable(m_bWrite);
    return 0;
}

nuUINT CClientSocketZK::FdRead()
{
    m_buffRecv.sbBytes  = 0;
    nuUINT nLogout = CIClientCallBack::CLT_LOG_OUT_NO;
    nuUINT nRes = SockRecv(m_buffRecv, nuNULL);    
    if( SOCK_ACTION_SUCCESS == nRes )
    {
        if( 0 == m_buffRecv.sbBytes)
        {
            nLogout = CIClientCallBack::CLT_LOG_OUT_SERVER;
        }
        else
        {
            m_nRecvTicks = nuGetTickCount();
            m_protocol.ProcBinaryData(m_buffRecv.sbBuffer.pBuffer, m_buffRecv.sbBytes);
        }
#ifndef _SEND_HEARTBEAT_ALWAYS
        m_nHeartBeatTicks   = nuGetTickCount();
#endif
    }
    else if( SOCK_ACTION_ERROR == nRes )
    {
        nLogout = CIClientCallBack::CLT_LOG_OUT_ERROR;
    }
    if( CIClientCallBack::CLT_LOG_OUT_NO != nLogout )
    {
        Logout(nLogout);
        m_bNeedCreate   = nuTrue;
    }
    return m_buffRecv.sbBytes;
}

nuUINT CClientSocketZK::FdError()
{
    //failed to connected, please recall 'connect'
    m_bNeedConnect  = nuTrue;
    return 0;
}

nuUINT CClientSocketZK::SendTo(nuUINT nCmd, const nuCHAR* pData, nuUINT nDataLen)
{
    //multiple thread
   // CBaseLock lock(&m_bLock);
    CCriticalLock lock(&m_bSection);
	if( !m_bWrite )
    {
        return 0;
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    m_buffSend.sbBuffer.nBufferLen = m_protocol.PackageData(m_buffSend.sbBuffer.pBuffer, CProtocolBaseZK::CMD_INFO_MAX_SIZE, pData, nDataLen, m_nClientID, nCmd);
    if( 0 == m_buffSend.sbBuffer.nBufferLen )
    {
        return 0;
    }
    nuINT nRes = SockSend(m_buffSend, nuNULL);
    if( SOCK_ACTION_PENDING == nRes )
    {
        m_bWrite    = nuFalse;
        m_pCallback->ClientWritable(m_bWrite);
    }
    nDataLen = m_buffSend.sbBuffer.nBufferLen;
    if( SOCK_ACTION_PENDING == nRes ||
        SOCK_ACTION_SUCCESS == nRes )
    {
        return nDataLen;
    }
    else
    {
        return 0;
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

nuVOID CClientSocketZK::ServerReply(CIClientCallBack::RECV_DATA& recvData)
{
    if( CMD_CLIENT_LOGIN == recvData.sdCmd )
    {
        Login(recvData.sdDataBuff, recvData.sdNowSize);
    }
    else if( CMD_CLIENT_HEARTBEAT == recvData.sdCmd )
    {
        HeardBeatRecv();
    }
    else 
    {
        m_pCallback->ServerReplyed(recvData);
    }
}

nuVOID CClientSocketZK::ServerFrom(CIClientCallBack::RECV_DATA& recvData)
{
	if (CMD_CLIENT_QUIT_BY_SAME == recvData.sdCmd)
	{
		m_nLoginError = CMD_CLIENT_QUIT_BY_SAME;
	}
    if( CIClientCallBack::RB_CLIENT_NOREPLY == m_pCallback->ClientRecved(recvData) )
    {
        SendTo(recvData.sdCmd|CMD_REPLY_BASE, nuNULL, 0);
    }
}

nubool CClientSocketZK::Reconnect()
{
    if( nuGetTickCount() - m_nRecvTicks > 2*CONST_USER_HEARTBEAT_INTERVAL )
    {
        Logout(CIClientCallBack::CLT_LOG_OUT_NET);
        m_bNeedCreate = nuTrue;
        return nuTrue;
    }
    return nuFalse;
}

nuVOID CClientSocketZK::DebugOut(nuUINT nDebug /* = DEBUG_CLOSE */)
{
    switch( nDebug )
    {
    case DEBUG_CLOSE:
        printf("%s is closed\n", m_sockAddr.sinAddr);
        break;
    case DEBUG_CONNECTED:
        break;
    case DEBUG_RECV:
        break;
    case DEBUG_SEND:
        break;
    default:
        break;
    }
}
