// SockServerZK.cpp: implementation of the CSockServerZK class.
//
//////////////////////////////////////////////////////////////////////

#include "SockServerZK.h"
#include "inlineFunctionZK.h"
#include "BaseLogZK.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define _DEBUG_OUT(debug, pOverlapped, pClient, nLen) //DebugOut(debug, pOverlapped, pClient, nLen)

CSockServerZK::CSockServerZK()
{
    m_sockIn.siAF           = SOCK_AF_INET;
    m_sockIn.siType         = SOCK_T_STREAM;
    m_sockIn.siProtocol     = SOCK_IPPROTO_TCP;
    m_sockIn.siFlag         = SOCK_FLAG_OVERLAPPED;
    m_sockIn.siProtData     = nuNULL;

    m_nThreadCount  = 0;
    m_pThreadPool   = nuNULL;

    m_nAcceptReadyCount = 0;
    s_pServer       = this;
}

CSockServerZK::~CSockServerZK()
{

}

nuFINAL CSockServerZK::StartServer(nuUINT nMaxCount, 
                                   nuUINT nPort /* = CONST_SERVER_PORT */, 
                                   nuCHAR* psIP /* = nuNULL */)
{
    //load winsock2.dll
    if( !StartUp() )
    {
        return FINAL_FAILURE;
    }
    //create the list of clients
    m_clientMgr.InitClientMgr(nMaxCount);
    //create IoCompletion port
    if( !m_ioCompletionPort.CreateIoPort() )
    {
        return FINAL_FAILURE;
    }
    //create threads
    if( !CreateThreadPools() )
    {
        return FINAL_FAILURE;
    }
    //start server socket
    if( !SockCreate(&m_sockIn) )
    {
        return FINAL_FAILURE;
    }
    m_sockAddr.sinFamily  = SOCK_AF_INET;
    m_sockAddr.sinPort    = nPort;
    if( psIP && psIP[0])
    {
        nuStrcpy(m_sockAddr.sinAddr, psIP);
    }
    else
    {
        SockGetHostIP(m_sockAddr.sinAddr, sizeof(m_sockAddr.sinAddr));
    }
    if( !SockBind(m_sockAddr) )
    {
        return FINAL_FAILURE;
    }
    if( !SockListen() )
    {
        return FINAL_FAILURE;
    }
    if( !AssociateIoPort(this) )
    {
        return FINAL_FAILURE;
    }
    AddClientToAccept();//AcceptReady();
    return FINAL_SUCCESS;
}

nuFINAL CSockServerZK::CloseServer()
{
    SockClose();
    DeleteThreadPools();
    m_ioCompletionPort.CloseIoPort();
    m_clientMgr.FreeClientMgr();
    CleanUp();
    return FINAL_SUCCESS;
}

nuUINT CSockServerZK::IOCompletionProc()
{
    nuDWORD dwBytes = 0;
    nuDWORD dwKey = IO_KEY_DEFAULT;
    COverlappedZK*  pOverlapped = nuNULL;
    nuINT nStatus = m_ioCompletionPort.GetQueueIoStatus(&dwBytes, &dwKey, (nuVOID**)(&pOverlapped));
    //....
    if( CCrossIOCompletionZK::IO_QUEUE_STATUS_QUIT == nStatus    ||
        IO_KEY_QUIT == dwKey )
    {
        if( nuNULL != pOverlapped)
        {
            CSockBaseZK* pSockBase = static_cast<CSockBaseZK*>(pOverlapped->GetSocket());
            pSockBase->FreeOverlapped(pOverlapped);
        }
        return IO_KEY_QUIT;
    }
    else if( CCrossIOCompletionZK::IO_QUEUE_STATUS_TIMEOUT == nStatus )
    {
        return dwKey;
    }
    
    CSockClientZK* pClient = static_cast<CSockClientZK*>(pOverlapped->GetSocket());
    if( nuNULL == pClient )
    {
        LOG_STRA("Error: the socket handle in the overlapped is invalid!\n");
        return dwKey;
    }
    nuUINT nRes = IO_PROC_CLIENT_ERROR;
    if( CCrossIOCompletionZK::IO_QUEUE_STATUS_SUCESS == nStatus )
    {
        if( IO_KEY_SOCKSERVER == dwKey )
        {
            nRes = IoAccepted(pClient, pOverlapped, dwBytes);
        }
        else if( IO_KEY_SOCKCLIENT == dwKey )
        {
            nRes = IoClient(pClient, pOverlapped, dwBytes);
        }  
    }
    else
    {
        if( pClient->IsAccepting() )
        {
            --m_nAcceptReadyCount;
            AddClientToAccept();
        }
        nRes =IO_PROC_CLIENT_ERROR;
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if( IO_PROC_RECV_SUCESS < nRes )
    {
        if( IO_PROC_CLIENT_EXIT <= nRes )
        {
            if( IO_PROC_CLIENT_EXIT == nRes )
            {
                pClient->IoClientQuit(CIServerCallBack::LOG_OUT_CLOSE_BY_CLIENT);
            }
            else
            {
                pClient->IoClientQuit(CIServerCallBack::LOG_OUT_CLOSE_BY_ERROR);
            }
        }
        //放在这后面很重要，好先关系Socket再删除overlapped对象
        pClient->FreeOverlapped(pOverlapped);
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    m_clientMgr.RemoveUnusefuls();
    CheckIdleClients();
    return dwKey;
}

nuUINT CSockServerZK::AddClientToAccept()
{
    nuUINT nRes = 0;
    CSockClientZK* pClient = nuNULL;
    COverlappedZK* pOverlapped = nuNULL;
    while( m_nAcceptReadyCount < CONST_ACCPET_COUNT )
    {
        //allocate a client from clients and create client for accept TO-DO memery leaks
        CSockClientZK* pClient = m_clientMgr.AllocatClient();
        if( nuNULL == pClient ||
            !pClient->SockCreate(&m_sockIn) )
        {
            break;
        }
		pClient->SetServer(this);
		pClient->SetCallBack(this->GetCallBack());
        pOverlapped = pClient->AllocOverlapped(COverlappedZK::OVERLAPPED_ACCEPT, CProtocolBaseZK::CMD_INFO_MAX_SIZE);
        nuDWORD nLen = pOverlapped->GetBuffLen();
        nuINT nRet = SockAcceptEx( *pClient
            , pOverlapped->GetBuffer()
            , nLen
            , pOverlapped );
        if( SOCK_ACTION_SUCCESS != nRet &&
            SOCK_ACTION_PENDING != nRet )
        {
            break;
        }
        pOverlapped = nuNULL;
        pClient     = nuNULL;
        ++m_nAcceptReadyCount;
        ++nRes;
    }
    if( nuNULL != pClient )
    {
        if( nuNULL != pOverlapped )
        {
            pClient->FreeOverlapped(pOverlapped);
        }
        pClient->IoClientQuit(CIServerCallBack::LOG_OUT_CLOSE_BY_ERROR);
    }
    return nRes;
}

nubool CSockServerZK::CreateThreadPools()
{
    m_nThreadCount = m_ioCompletionPort.GetNumberOfThreads() * 2;
    if( m_nThreadCount > CClientOverlappedZK::CONST_MAX_OVERLAPPED_COUNT )
    {
        m_nThreadCount = CClientOverlappedZK::CONST_MAX_OVERLAPPED_COUNT;
    }
    if( 0 == m_nThreadCount )
    {
        return nuFalse;
    }
    m_pThreadPool = new CSocketThreadZK[m_nThreadCount];
    if( nuNULL == m_pThreadPool )
    {
        return nuFalse;
    }
    for(nuUINT i = 0; i < m_nThreadCount; ++i)
    {
        if( m_pThreadPool[i].StartThread(this) )
        {
            return nuFalse;
        }
    }
    return nuTrue;
}

nuVOID CSockServerZK::DeleteThreadPools()
{
    if( m_pThreadPool )
    {
        nuUINT i;
        for(i = 0; i < m_nThreadCount; ++i)
        {
            COverlappedZK* pOverlapped = AllocOverlapped(COverlappedZK::OVERLAPPED_EXIT, 128);
            while( nuNULL == pOverlapped )
            {
                nuSleep(25);
                pOverlapped = AllocOverlapped(COverlappedZK::OVERLAPPED_EXIT, 128);
            }
            m_ioCompletionPort.PostQueueIoStatus(0, IO_KEY_QUIT, pOverlapped);
//            m_ioCompletionPort.PostQueueIoStatus(0, IO_KEY_QUIT, m_overVessel.GetOverlapped(nuNULL, 0, COverlappedZK::OVERLAPPED_EXIT) );
        }
        for(i = 0; i < m_nThreadCount; ++i)
        {
            m_pThreadPool[i].CloseThread();
        }
        delete[] m_pThreadPool;
        m_pThreadPool = nuNULL;
        m_nThreadCount = 0;
    }
}

nuUINT CSockServerZK::IoAccepted(CSockClientZK* pClient, COverlappedZK *pOverlapped, nuDWORD dwBytes)
{
    nuUINT nRes = IO_PROC_CLIENT_ERROR;
/*    CSockClientZK* pClient = static_cast<CSockClientZK*>( pOverlapped->GetSocket() );*/
    if( pClient->IoAccepted(pOverlapped)    && 
        pClient->SockSetOption(*this)       && 
        AssociateIoPort(pClient, IO_KEY_SOCKCLIENT) )
    {
        SockGetAcceptExAddr(pOverlapped->GetBuffer(), &pClient->GetSocketAddr(), nuNULL);
        _DEBUG_OUT(DEBUG_ACCEPTED, pOverlapped, pClient, 0);
        pOverlapped->SetOverType(COverlappedZK::OVERLAPPED_RECV);
        if( pClient->IoRecv(pOverlapped) )
        {
            nRes = IO_PROC_ACCEPT_SUCESS;
//            bRes        = nuTrue;
        }
    }
    --m_nAcceptReadyCount;
    AddClientToAccept();
    return nRes;
}
//if the return value is true, Don't delete the pOverlapped
nuUINT CSockServerZK::IoClient(CSockClientZK* pClient, COverlappedZK *pOverlapped, nuDWORD dwBytes)
{
    if( COverlappedZK::OVERLAPPED_RECV == pOverlapped->GetOverType() )
    {
        _DEBUG_OUT(DEBUG_RECV, pOverlapped, pClient, dwBytes);//test...
        if( !pClient->IoRecved(pOverlapped, dwBytes) )
        {
            return IO_PROC_CLIENT_EXIT;
        }
        else if( !pClient->IoRecv(pOverlapped) )
        {
            return IO_PROC_CLIENT_ERROR;
        }
        else
        {
            return IO_PROC_RECV_SUCESS;
        }
    }
    else if( COverlappedZK::OVERLAPPED_SEND == pOverlapped->GetOverType() )
    {
        pClient->IoSended(pOverlapped, dwBytes);
        return IO_PROC_SEND_SUCESS;
    }
    else
    {
        return IO_PROC_CLIENT_ERROR;
    }
}

nubool CSockServerZK::IoClientSend(nuLONG nIdx, nuUINT nID, nuUINT nCmd, const nuCHAR* pData, nuUINT nDataLen)
{
    CSockClientZK* pClient = GetClient(nIdx, nID);
    if( nuNULL == pClient )
    {
        return nuFalse;
    }
    /*
    COverlappedZK* pOverlapped = m_overVessel.GetOverlapped(pClient, nDataLen + CProtocolBaseZK::CMD_INFO_MIN_SIZE, COverlappedZK::OVERLAPPED_SEND);
    if( nuNULL == pOverlapped )
    {
        return nuFalse;
    }
    if( 0 == CProtocolBaseZK::PackageData(pOverlapped->GetBuffer(), pOverlapped->GetBuffLen(), pData, nDataLen, nID, nCmd) )
    {
        return nuFalse;
    }
    return pClient->IoSend(pOverlapped);
    */
    return pClient->IoSend(nID, nCmd, pData, nDataLen);
}

nubool CSockServerZK::IoClientReply(nuLONG nIdx, nuUINT nID, nuUINT nCmd, const nuCHAR* pData, nuUINT nDataLen)
{
    nCmd &=(~CMD_REPLY_BASE);
    return IoClientSend(nIdx, nID, nCmd, pData, nDataLen);
}

nubool CSockServerZK::IoClientClose(nuLONG nIdx, nuUINT nID)
{
    CSockClientZK* pClient = GetClient(nIdx, nID);
    if( nuNULL == pClient )
    {
        return nuFalse;
    }
    if( pClient->IoClientQuit(CIServerCallBack::LOG_OUT_CLOSE_BY_SERVER) )
    {
        return nuTrue;
    }
    else
    {
        return nuFalse;
    }
}

//
nuVOID CSockServerZK::DebugOut(emDebugCode debug, COverlappedZK* pOverlapped, CSockClientZK* pClient, nuUINT nlen)
{
    nuUINT i = 0;
    nuCHAR* pBuffer = pOverlapped->GetBuffer();
    nuUINT nN = 0;
    switch(debug)
    {
    case DEBUG_CLOSE://close
        printf("%s: Closed[index = %d]\n", pClient->GetSocketAddr().sinAddr, pClient->GetIndex());
        break;
    case DEBUG_ACCEPTED://Accepted
        printf("%s: Accepted[index = %d]\n", pClient->GetSocketAddr().sinAddr, pClient->GetIndex());
        break;
    case DEBUG_RECV://recved
        printf("RECV FROM:%s\n", pClient->GetSocketAddr().sinAddr);
        break;
    case DEBUG_SEND://sent
        printf("SENT TO:%s\n", pClient->GetSocketAddr().sinAddr);
        break;
    default:
        printf("Number = %d\n", debug);
        break;
    }
}
