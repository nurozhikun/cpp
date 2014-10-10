// SockClientZK.cpp: implementation of the CSockClientZK class.
//
//////////////////////////////////////////////////////////////////////

#include "SockClientZK.h"
#include "SockServerZK.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
/////////////////////////////////////////////////////////////////////

CSockClientZK::CSockClientZK()
:m_protocol(this)
{
    m_nIndex        = -1;
    m_nID           = 0;
    m_nActiveTicks  = 0;
    m_nSendCount    = 0;
    m_nSocketStatus = SOCK_STATUS_CLOSE;
    m_nProcessThreaddCount  = 0;
}

CSockClientZK::~CSockClientZK()
{
    IoClientQuit(CIServerCallBack::LOG_OUT_CLOSE_BY_DESTROY);
}

nubool CSockClientZK::IoAccepted(COverlappedZK* pOverlapped)
{
    SetTicks();
    m_nSocketStatus |= SOCK_STATUS_ACCEPTED;
    return nuTrue;
}

nubool CSockClientZK::IoRecv(COverlappedZK* pOverlapped)
{
    if( nuNULL == pOverlapped )
    {
        return nuFalse;
    }
    nuINT nRecv = SockRecv(pOverlapped->GetSockBuff(), pOverlapped);
    if( SOCK_ACTION_SUCCESS == nRecv ||
        SOCK_ACTION_PENDING == nRecv )
    {
        m_nSocketStatus |= SOCK_STATUS_RECV;
        return nuTrue;
    }
    else
    {
        return nuFalse;
    }
}

nubool CSockClientZK::IoRecved(COverlappedZK* pOverlapped, nuDWORD dwBytes)
{
    if( 0 == dwBytes || nuNULL == pOverlapped )
    {
        return nuFalse;
    }
    else
    {
        SetTicks();
        //data process...
        m_protocol.ProcBinaryData(pOverlapped->GetBuffer(), dwBytes);
        return nuTrue;
    }
}

nubool CSockClientZK::IoSend(COverlappedZK* pOverlapped)
{
    if( nuNULL == pOverlapped )
    {
        return nuFalse;
    }
    nuINT nSendRes = SockSend(pOverlapped->GetSockBuff(), pOverlapped);
    if( SOCK_ACTION_SUCCESS != nSendRes && 
        SOCK_ACTION_PENDING != nSendRes )
    {
        return nuFalse;
    }
    m_nSocketStatus |= SOCK_STATUS_SEND;
    ++m_nSendCount;
    return nuTrue;
}

nubool CSockClientZK::IoSend(nuUINT nID, nuUINT nCmd, const nuCHAR* pData, nuUINT nDataLen)
{
    if( nuNULL == s_pServer )
    {
        return nuFalse;
    }
//    COverlappedZK* pOverlapped = s_pServer->GetOverlapped(this, nDataLen + CProtocolBaseZK::CMD_INFO_MIN_SIZE, COverlappedZK::OVERLAPPED_SEND);
    COverlappedZK* pOverlapped = AllocOverlapped(COverlappedZK::OVERLAPPED_SEND, nDataLen + CProtocolBaseZK::CMD_INFO_MIN_SIZE);
    if( nuNULL == pOverlapped )
    {
        return nuFalse;
    }
    if( 0 == CProtocolBaseZK::PackageData(pOverlapped->GetBuffer(), pOverlapped->GetBuffLen(), pData, nDataLen, nID, nCmd) ||
		!IoSend(pOverlapped) )
    {
		FreeOverlapped(pOverlapped);
        return nuFalse;
    }
  //  LOGDB_BUFFER( (nuCHAR*)pData,nDataLen, nCmd, nID);

	return nuTrue;
}

nubool CSockClientZK::IoSended(COverlappedZK* pOverlapped, nuDWORD dwBytes)
{
    SetTicks();
    --m_nSendCount;
    if( 0 == m_nSendCount )
    {
        m_nSocketStatus &= (~SOCK_STATUS_SEND);
    }
    //@test
//    IoClientQuit(CIServerCallBack::LOG_OUT_CLOSE_BY_SERVER);
    //@
    return nuTrue;
}

nuVOID CSockClientZK::CloseClient()
{
    if( m_nSocketStatus >= SOCK_STATUS_ACCEPTED )
    {
        SockShutdown(SOCK_SHUT_RECV);
    }
    if( SOCK_STATUS_CLOSE != m_nSocketStatus )
    {
        m_nSocketStatus = SOCK_STATUS_CLOSE;
        SockClose();
//        CleanIndex();
    }
}

nuUINT CSockClientZK::IoClientLoging(nuUINT nID, const nuCHAR* pData, nuUINT nLen)
{
    nuUINT nRes = CIServerCallBack::CB_RET_NOPROCESS;
    if( 0 == m_nID )
    {
        nRes = s_pCallback->SockLogin(GetIndex(), nID, pData, nLen);
        nuDWORD nData = 0;
        if( nRes > CIServerCallBack::CB_RET_LOGIN_SRUCESS )
        {
            nData = nRes - CIServerCallBack::CB_RET_LOGIN_SRUCESS;
        }
        IoSend(nID, CMD_CLIENT_LOGIN + CMD_REPLY_BASE, (nuCHAR*)&nData, 4); 
        if( 0 == nData )
        {
            m_nID   = nID;
            //delete the same socket...
//            s_pServer->RemoveUnusefuls(this);
            s_pServer->RemoveSameClient(this);
        }//success, close the clients with the same ID
		else
		{
		}//failure, close the now socket
        nRes = CIServerCallBack::CB_RET_REPLYED;
    }
    return nRes;
}

nuUINT CSockClientZK::IoClientQuit(nuBYTE nType)
{
    nuUINT nRes = 0;
    if( m_nID )
    {
        nRes = s_pCallback->SockLogout(GetIndex(), m_nID, nType);
        m_nID = 0;
    }
//     if( CIServerCallBack::LOG_OUT_CLOSE_BY_SERVER == nType )
//     {
//         if( m_nSocketStatus >= SOCK_STATUS_RECV )
//         {
//             SockShutdown();
//         }
//     }
    CloseClient();
    return nRes;
}

nuUINT CSockClientZK::IoClientHeartbeat(nuUINT nCount)
{
    return CIServerCallBack::CB_RET_NOREPLYED;
}

nuUINT CSockClientZK::IoClientRecved(const CIServerCallBack::SOCK_DATA &sockData)
{
    if( m_nID != m_nID )
    {
        return CIServerCallBack::CB_RET_NOPROCESS;
    }
    else
    {
        return s_pCallback->SockRecved(sockData);
    }
}

nuUINT CSockClientZK::IoClientReplyed(const CIServerCallBack::SOCK_DATA &sockData)
{
    if( m_nID != m_nID )
    {
        return CIServerCallBack::CB_RET_NOPROCESS;
    }
    else
    {
        return s_pCallback->SockReplyed(sockData);
    }
}
nubool  CSockClientZK::IsNoResponse(nuDWORD nNowTicks) 
{ 
    if( (nuDWORD)(nNowTicks - m_nActiveTicks) > CONST_NO_RESPONSE_INTERVAL )
    {
        if( m_nSocketStatus&SOCK_STATUS_SEND ||
            !IoSend(GetID(), CMD_SERVER_HEARTBEAT, nuNULL, 0) )
        {
            return nuTrue;
        }
    }
    return nuFalse;
}

nubool CSockClientZK::FreeOverlapped(COverlappedZK *pOverlapped)
{
    if( CSockBaseZK::FreeOverlapped(pOverlapped) )
    {
        if( 0 == GetOverlappedUsedCount() )
        {
            CloseClient();
            CleanIndex();
        }
        return nuTrue;
    }
    return nuFalse;
}