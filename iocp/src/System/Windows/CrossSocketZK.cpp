// CrossSocketZK.cpp: implementation of the CCrossSocketZK class.
//
//////////////////////////////////////////////////////////////////////

#include "CrossSocketZK.h"
#include "systemInc.h"
#include "BaseLogZK.h"
#pragma comment(lib, "ws2_32.lib")
nuINT CCrossSocketZK::s_nSDllError	= -1;

CCrossSocketZK::CCrossSocketZK()
{
	m_socket	= INVALID_SOCKET;
//     m_sockIn.siAF       = SOCK_AF_INET;
//     m_sockIn.siType     = SOCK_T_STREAM;
//     m_sockIn.siProtocol = SOCK_IPPROTO_TCP;
//     m_sockIn.siFlag     = SOCK_FLAG_OVERLAPPED;
//     m_sockIn.siProtData = NULL;
}

CCrossSocketZK::~CCrossSocketZK()
{
	
}

nubool CCrossSocketZK::StartUp()
{
	if( !s_nSDllError )
	{
		return nuTrue;
	}
	WSADATA wsaData;
	s_nSDllError = WSAStartup(WINSOCK_VERSION, &wsaData);
	if( s_nSDllError )
	{
		return nuFalse;
	}
	return nuTrue;
}

nuVOID CCrossSocketZK::CleanUp()
{
	if( !s_nSDllError )
	{
		WSACleanup();
		s_nSDllError = -1;
	}
}

nuUINT CCrossSocketZK::SockGetHostIP(nuCHAR* psIPBuff, nuUINT nIpBuffLen, nuCHAR* pcsHostName /* = nuNULL */)
{
    if( nuNULL == psIPBuff )
    {
        return 0;
    }
    char sHostName[256];
    if( nuNULL == pcsHostName )
    {
        if( gethostname(sHostName, 256) )
        {
            return 0;
        }
        pcsHostName = sHostName;
    }
    hostent* hint = gethostbyname(pcsHostName);
    if( NULL == hint )
    {
        return 0;
    }
    //TO-DO 多块网卡的处理
    strcpy( psIPBuff, inet_ntoa( *((struct in_addr*)(hint->h_addr)) ) );
    printf("current bind ip: %s \n ", psIPBuff);
    return strlen(psIPBuff);
}

nubool CCrossSocketZK::SockCreate(const SOCKET_IN* pSocketIn /* = NULL */)
{
	int nAF			= AF_INET;
	int nType		= SOCK_STREAM;
	int nProtocol	= IPPROTO_TCP;
	LPWSAPROTOCOL_INFO pProtInfo = NULL;
	DWORD nFlag		= WSA_FLAG_OVERLAPPED;
	if( nuNULL != pSocketIn )
	{
/*        m_sockIn    = *pSocketIn;*/
		//... Add others states
        nAF         = pSocketIn->siAF;
        nType       = pSocketIn->siType;
        nProtocol   = pSocketIn->siProtocol;
        if( SOCK_FLAG_OVERLAPPED != pSocketIn->siFlag )
        {
            nFlag = 0;
        }
//        nFlag       = pSocketIn->siFlag;
	}
	SockClose();
	m_socket = WSASocket(nAF, nType, nProtocol, pProtInfo, 0, nFlag);
	if( INVALID_SOCKET == m_socket )
	{
		return nuFalse;
	}
    if( SOCK_FLAG_NONBLOCKING == pSocketIn->siFlag )
    {
        u_long  iMode = 1;
        ioctlsocket(m_socket, FIONBIO, &iMode);
    }
	return nuTrue;
}

nubool CCrossSocketZK::SockClose()
{
	if( INVALID_SOCKET != m_socket )
	{
		closesocket((SOCKET)m_socket);
		m_socket = INVALID_SOCKET;
        return nuTrue;
	}
    return nuFalse;
}

nubool CCrossSocketZK::SockShutdown(nuINT nFlag)
{
	if( INVALID_SOCKET != m_socket )
	{
		if( !shutdown((SOCKET)m_socket, nFlag) )
		{
			return nuTrue;
		}
	}
	return nuFalse;
}

nubool CCrossSocketZK::SockBind(SOCKET_ADDR &socketAddr)
{
	if( INVALID_SOCKET == m_socket )
	{
		return nuFalse;
	}
	sockaddr_in sockAddrIn;
	sockAddrIn.sin_family		= socketAddr.sinFamily;
	sockAddrIn.sin_port			= htons(socketAddr.sinPort);
	sockAddrIn.sin_addr.s_addr  = inet_addr(socketAddr.sinAddr);
	if( bind( (SOCKET)m_socket, (SOCKADDR*)(&sockAddrIn), sizeof(sockAddrIn)) )
	{
		return nuFalse;
	}
	return nuTrue;
}

nubool CCrossSocketZK::SockListen(nuINT nLog /* = 0 */)
{
	if( INVALID_SOCKET == m_socket )
	{
		return nuFalse;
	}
    if( 0 == nLog )
    {
        nLog = SOMAXCONN;
    }
	if( listen(m_socket, nLog) )
	{
		return nuFalse;
	}
	return nuTrue;
}

nubool CCrossSocketZK::SockAccept(CCrossSocketZK& s, SOCKET_ADDR* pSocketAddr)
{
	if( INVALID_SOCKET == m_socket )
	{
		return nuFalse;
	}
	sockaddr_in siClent;
	int nLen = sizeof(siClent);
	SOCKET sClient = WSAAccept(m_socket, (SOCKADDR*)&siClent, &nLen, NULL, NULL);
	if( INVALID_SOCKET == sClient )
	{
		return nuFalse;
	}
	s.SockClose();
	s.m_socket = sClient;
	if( pSocketAddr )
	{
		pSocketAddr->sinFamily	= siClent.sin_family;
		pSocketAddr->sinPort	= ntohs(siClent.sin_port);
		char* pAddr = inet_ntoa(siClent.sin_addr);
		if( pAddr )
		{
			strcpy(pSocketAddr->sinAddr, pAddr);
		}
	}
	return nuTrue;
}

nuINT CCrossSocketZK::SockAcceptEx(CCrossSocketZK& s, nuCHAR *pBuffer, nuDWORD& dwRecvs, nuVOID* pOverlapped)
{
    if( INVALID_SOCKET == m_socket )
    {
        return SOCK_ACTION_FAILURE;
    }
    DWORD dwBytesRecvs;
    static LPFN_ACCEPTEX lpfnAcceptEx = NULL;
    if( nuNULL == lpfnAcceptEx )
    {
        GUID GuidAcceptEx = WSAID_ACCEPTEX;
        if( WSAIoctl( m_socket
            , SIO_GET_EXTENSION_FUNCTION_POINTER
            , &GuidAcceptEx
            , sizeof (GuidAcceptEx)
            , &lpfnAcceptEx
            , sizeof (lpfnAcceptEx)
            , &dwBytesRecvs
            , NULL
            , NULL ) )
        {
            return SOCK_ACTION_FAILURE;
        }
    }
    if( lpfnAcceptEx(m_socket
            , s.m_socket
            , pBuffer
            , 0
            , sizeof (sockaddr_in) + SOCKET_ADDRESS_LENGTH
            , sizeof (sockaddr_in) + SOCKET_ADDRESS_LENGTH
            , &dwRecvs
            , (LPOVERLAPPED)pOverlapped) )
    {
        return SOCK_ACTION_SUCCESS;
    }
    else if( ERROR_IO_PENDING == WSAGetLastError() )
    {
        return SOCK_ACTION_PENDING;
    }
    else
    {
        return SOCK_ACTION_FAILURE;
    }
}

nuVOID CCrossSocketZK::SockGetAcceptExAddr(nuVOID* pBuffer, SOCKET_ADDR* pRemoteAddr, SOCKET_ADDR* pLocalAddr /* = nuNULL  */
                                           )
{
    static LPFN_GETACCEPTEXSOCKADDRS lpfnGetAcceptExSockAdddrs = NULL;
    if( nuNULL == lpfnGetAcceptExSockAdddrs )
    {
        DWORD dwBytesRecvs;
        GUID GuidAcceptEx = WSAID_GETACCEPTEXSOCKADDRS;
        if( WSAIoctl( m_socket
            , SIO_GET_EXTENSION_FUNCTION_POINTER
            , &GuidAcceptEx
            , sizeof (GuidAcceptEx)
            , &lpfnGetAcceptExSockAdddrs
            , sizeof (lpfnGetAcceptExSockAdddrs)
            , &dwBytesRecvs
            , NULL
            , NULL ) )
        {
            return ;
        }
    }
    sockaddr_in *pSockLocal, *pSockRemote;
    nuINT nLocalLen, nRemoteLen;
    lpfnGetAcceptExSockAdddrs( pBuffer
        , 0
        , sizeof (sockaddr_in) + SOCKET_ADDRESS_LENGTH
        , sizeof (sockaddr_in) + SOCKET_ADDRESS_LENGTH
        , (LPSOCKADDR*)&pSockLocal
        , &nLocalLen
        , (LPSOCKADDR*)&pSockRemote
        , &nRemoteLen );
    if( pLocalAddr )
    {
        pLocalAddr->sinFamily   = pSockLocal->sin_family;
        pLocalAddr->sinPort     = ntohs(pSockLocal->sin_port);
        strcpy( pLocalAddr->sinAddr, inet_ntoa( pSockLocal->sin_addr) );
    }
    if( pRemoteAddr )
    {
        pRemoteAddr->sinFamily   = pSockRemote->sin_family;
        pRemoteAddr->sinPort     = ntohs(pSockRemote->sin_port);
        strcpy( pRemoteAddr->sinAddr, inet_ntoa( pSockRemote->sin_addr) );
    }
}

nubool CCrossSocketZK::SockSetOption(CCrossSocketZK& sListen)
{
    if( setsockopt( m_socket
            , SOL_SOCKET
            , SO_UPDATE_ACCEPT_CONTEXT
            , (char*)&(sListen.m_socket)
            , sizeof(sListen.m_socket) ) )
    {
        return nuFalse;
    }
    else
    {
        return nuTrue;
    }
}

// nuVOID* CCrossSocketZK::CreateOverlapped()
// {
// 	char *p = new char[sizeof(OVERLAPPED)+sizeof(SOCKET_SELF_OVERLAPPED)];
//     if( p )
//     {
//         memset(p, 0, sizeof(OVERLAPPED)+sizeof(SOCKET_SELF_OVERLAPPED));
//     }
//     return p;
// }
// 
// nuVOID	CCrossSocketZK::DeleteOverlapper(nuVOID* pOverlapped)
// {
// 	if( pOverlapped )
// 	{
// 		delete[] pOverlapped;
// 	}
// }
// 
// CCrossSocketZK::SOCKET_SELF_OVERLAPPED* CCrossSocketZK::GetSelfOverlapped(nuVOID* pOverlapped)
// {
// 	if( pOverlapped )
// 	{
// 		return (SOCKET_SELF_OVERLAPPED*)(((char*)pOverlapped) + sizeof(OVERLAPPED));
// 	}
// 	return NULL;
// }

nuINT CCrossSocketZK::SockSend(SOCKET_BUFFER& sockBuffer, nuVOID* pOverlapped)
{
	if( INVALID_SOCKET == m_socket )
	{
		return SOCK_ACTION_FAILURE;
	}
	if( !WSASend( m_socket
		, (WSABUF*)&sockBuffer.sbBuffer
		, 1
		, &sockBuffer.sbBytes
		, sockBuffer.sbFlags
		, (WSAOVERLAPPED*)pOverlapped
		, NULL ) )
	{
		return SOCK_ACTION_SUCCESS;
	}
	else
	{
        int error = WSAGetLastError();
		if( error  == WSA_IO_PENDING )
		{
			return SOCK_ACTION_PENDING;
		}
		else
		{
			return SOCK_ACTION_ERROR;
		}
	}
}

nuINT CCrossSocketZK::SockRecv(SOCKET_BUFFER& sockBuffer, nuVOID* pOverlapped)
{
	if( INVALID_SOCKET == m_socket )
	{
		return SOCK_ACTION_FAILURE;
	}
	if( !WSARecv( m_socket
		, (WSABUF*)&sockBuffer.sbBuffer
		, 1
		, &sockBuffer.sbBytes
		, &sockBuffer.sbFlags
		, (WSAOVERLAPPED*)pOverlapped
		, NULL ) )
	{
		return SOCK_ACTION_SUCCESS;
	}
	else
	{
        //test...
        int nError = WSA_IO_PENDING;
		if( WSA_IO_PENDING  == WSAGetLastError() )
		{
			return SOCK_ACTION_PENDING;
		}
		else
		{
			return SOCK_ACTION_ERROR;
		}
	}
}

nuINT CCrossSocketZK::SockGetOverlappedResult(nuVOID* pOverlapped, nuDWORD &dwBytes, nuDWORD &dwFlags)
{
    if( !pOverlapped )
    {
        return SOCK_ACTION_FAILURE;
    }
    if( WSAGetOverlappedResult(m_socket, (LPWSAOVERLAPPED)pOverlapped, &dwBytes, FALSE, &dwFlags) )
    {
        return SOCK_ACTION_SUCCESS;
    }
    else
    {
        LOG_ERROR_FILE("WSAGetOverlappedResult->%d", WSAGetLastError());
        return SOCK_ACTION_ERROR;
    }
}

nuINT CCrossSocketZK::SockConnect(SOCKET_ADDR& sockAddr)
{
	sockaddr_in sockAddrIn;
	sockAddrIn.sin_family		= sockAddr.sinFamily;
	sockAddrIn.sin_port			= htons(sockAddr.sinPort);
	sockAddrIn.sin_addr.s_addr  = inet_addr(sockAddr.sinAddr);
	if( connect(m_socket, (SOCKADDR*)&sockAddrIn, sizeof(sockAddrIn)) )
	{
        //test...
        int nError = WSAGetLastError();
		if( WSAEWOULDBLOCK == WSAGetLastError() )
		{
			return SOCK_ACTION_PENDING;
		}
		else
		{
			return SOCK_ACTION_ERROR;
		}
	}
	else
	{
		return SOCK_ACTION_SUCCESS;
	}
}

nuINT CCrossSocketZK::SockSelect(SOCKET_SELECT& socks)
{
	fd_set	fdSetRead;
	fd_set	fdSetWrite;
	fd_set	fdSetError;
	FD_ZERO(&fdSetRead);
	FD_ZERO(&fdSetWrite);
	FD_ZERO(&fdSetError);
	nuUINT i = 0;
	while( i < socks.ssCount )
	{
		if( socks.ssSockState[i] & SOCK_SELECT_READ )
		{
			FD_SET(socks.sspSocketsList[i]->m_socket, &fdSetRead);
		}
		if( socks.ssSockState[i] & SOCK_SELECT_WRITE )
		{
			FD_SET(socks.sspSocketsList[i]->m_socket, &fdSetWrite);
		}
		if( socks.ssSockState[i] & SOCK_SELECT_ERROR )
		{
			FD_SET(socks.sspSocketsList[i]->m_socket, &fdSetError);
		}
//		socks.ssSockState[i] = SOCK_SELECT_NONE;
		++i;
	}
	TIMEVAL timeout;
	timeout.tv_sec	= socks.ssTimeMs/1000;
	timeout.tv_usec	= 0;
	nuINT nRet = select(0, &fdSetRead, &fdSetWrite, &fdSetError, &timeout);
	if( SOCKET_ERROR == nRet )
	{
		return 0;
	}
//	nuINT nCount = nRet;
	for( i = 0; /*nCount > 0 &&*/ i < socks.ssCount; ++i )
	{
		nuBYTE nNewState = SOCK_SELECT_NONE;
		if( (socks.ssSockState[i] & SOCK_SELECT_READ) &&
			FD_ISSET(socks.sspSocketsList[i]->m_socket, &fdSetRead) )
		{
			nNewState |= SOCK_SELECT_READ;
			/*--nCount;*/
		}
		if( (socks.ssSockState[i] & SOCK_SELECT_WRITE) &&
			FD_ISSET(socks.sspSocketsList[i]->m_socket, &fdSetWrite) )
		{
			nNewState |= SOCK_SELECT_WRITE;
			/*--nCount;*/
		}
		if( (socks.ssSockState[i] & SOCK_SELECT_ERROR) &&
			FD_ISSET(socks.sspSocketsList[i]->m_socket, &fdSetError) )
		{
			nNewState |= SOCK_SELECT_ERROR;
			/*--nCount;*/
		}
        socks.ssSockState[i] = nNewState;
	}
	return nRet;
}