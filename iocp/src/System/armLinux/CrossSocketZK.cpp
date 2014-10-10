// CrossSocketZK.cpp: implementation of the CCrossSocketZK class.
//
//////////////////////////////////////////////////////////////////////

#include "CrossSocketZK.h"
#include "systemInc.h"

nuINT CCrossSocketZK::s_nSDllError	= -1;

CCrossSocketZK::CCrossSocketZK()
{
    m_if_socket_valid = 0;
//     m_socket	= INVALID_SOCKET;
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
    //linux needn't StartUp proc
	return nuTrue;
}

nuVOID CCrossSocketZK::CleanUp()
{
    //linux needn't CleanUp proc
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
	//linux may have 2 or more ipaddrs,now we use the first ip
    strcpy( psIPBuff, inet_ntoa( *((struct in_addr*)(hint->h_addr_list[0])) ) );
    printf("current bind ip: %s \n ", psIPBuff);
    return strlen(psIPBuff);
}

nubool CCrossSocketZK::SockCreate(const SOCKET_IN* pSocketIn /* = NULL */)
{
	int nAF			= AF_INET;
	int nType		= SOCK_STREAM;
	int nProtocol	= IPPROTO_TCP;

	if( nuNULL != pSocketIn )
	{
        nAF         = pSocketIn->siAF;
        nType       = pSocketIn->siType;
	}
	SockClose();
	
	m_socket = socket(nAF,nType,0);
	if( m_socket == -1 )
	{
		return nuFalse;
	}
	return nuTrue;
}

nubool CCrossSocketZK::SockClose()
{
	m_if_socket_valid = 0;
	close(m_socket);
    return nuTrue;
}

nubool CCrossSocketZK::SockShutdown(nuINT nFlag)
{
	m_if_socket_valid = 0;
	shutdown(m_socket, nFlag);
	return nuTrue;
}

nubool CCrossSocketZK::SockBind(SOCKET_ADDR &socketAddr)
{
	sockaddr_in sockAddrIn;
	//if( 0 == m_if_socket_valid )
	//{
	//	return nuFalse;
	//}
	sockAddrIn.sin_family		= socketAddr.sinFamily;
	sockAddrIn.sin_port			= htons(socketAddr.sinPort);
	sockAddrIn.sin_addr.s_addr  = inet_addr(socketAddr.sinAddr);
	if( bind(m_socket, (struct sockaddr*)(&sockAddrIn), sizeof(sockAddrIn)) == -1 )
	{
		return nuFalse;
	}
	return nuTrue;
}

nubool CCrossSocketZK::SockListen(nuINT nLog /* = 0 */)
{
	//if( 0 == m_if_socket_valid )
	//{
	//	return nuFalse;
	//}
    if( 0 == nLog )
    {
        nLog = 100;//need to define default max conn in headfile
    }
	if( listen(m_socket, nLog)==-1 )
	{
		return nuFalse;
	}
	return nuTrue;
}

nubool CCrossSocketZK::SockAccept(CCrossSocketZK& s, SOCKET_ADDR* pSocketAddr)
{
	sockaddr_in siClent;
    nuUINT sClient;
	nuUINT nLen = sizeof(siClent);
	//if( 0 == m_if_socket_valid )
	//{
	//	return nuFalse;
	//}
	if( (sClient=accept(m_socket, (struct sockaddr*)&siClent, &nLen)) == -1)
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
/*//Linux does not support
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
*/
    return 0;
}

nuVOID CCrossSocketZK::SockGetAcceptExAddr(nuVOID* pBuffer, SOCKET_ADDR* pRemoteAddr, SOCKET_ADDR* pLocalAddr /* = nuNULL  */)
{
/*//Linux does not support
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
*/
}

nubool CCrossSocketZK::SockSetOption(CCrossSocketZK& sListen)
{
    if( setsockopt( m_socket
            , SOL_SOCKET
            , sListen.m_opt_name
            , &sListen.m_opt_value
            , sizeof(sListen.m_opt_value))== -1)
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
	//if( 0 == m_if_socket_valid )
	//{
	//	return nuFalse;
	//}    
    if( write(m_socket,sockBuffer.sbBuffer.pBuffer,sockBuffer.sbBuffer.nBufferLen)==-1 )
    {
        printf("socket write error\n");
        return nuFalse;
    }
}

nuINT CCrossSocketZK::SockRecv(SOCKET_BUFFER& sockBuffer, nuVOID* pOverlapped)
{
    nuUINT nbytes;
	//if( 0 == m_if_socket_valid )
	//{
	//	return nuFalse;
	//}
    if( (nbytes=read(m_socket,sockBuffer.sbBuffer.pBuffer,sockBuffer.sbBuffer.nBufferLen)) == -1)
    { 
        printf("socket read error\n");
        return SOCK_ACTION_FAILURE;;
    }
    sockBuffer.sbBytes = nbytes;
    *(sockBuffer.sbBuffer.pBuffer + nbytes) = '\0';
    return SOCK_ACTION_SUCCESS;
}

nuINT CCrossSocketZK::SockGetOverlappedResult(nuVOID* pOverlapped, nuDWORD &dwBytes, nuDWORD &dwFlags)
{
/*//
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
        return SOCK_ACTION_ERROR;
    }
*/    
}

nuINT CCrossSocketZK::SockConnect(SOCKET_ADDR& sockAddr)
{
	sockaddr_in sockAddrIn;
	sockAddrIn.sin_family		= sockAddr.sinFamily;
	sockAddrIn.sin_port			= htons(sockAddr.sinPort);
	sockAddrIn.sin_addr.s_addr  = inet_addr(sockAddr.sinAddr);
    if(connect(m_socket,(struct sockaddr *)(&sockAddrIn),sizeof(sockAddrIn)) == -1)
    {
        fprintf(stderr,"Connect Error:%s\a\n",strerror(errno));
        printf("m_socket = %d\n",m_socket);
        printf("sockAddr.sinFamily = %d\n",sockAddr.sinFamily);
        printf("sockAddr.sinPort = %d\n",sockAddr.sinPort);
        printf("sockAddr.sinAddr = %s\n",sockAddr.sinAddr);
        printf("sizeof(sockAddrIn) = %d\n",sizeof(sockAddrIn));        
        return nuFalse;         
    }
    return nuTrue;
}

nuINT CCrossSocketZK::SockSelect(SOCKET_SELECT& socks)
{
	fd_set	fdSetRead;
	fd_set	fdSetWrite;
	fd_set	fdSetError;
	nuUINT i = 0;
    nuBYTE nNewState = SOCK_SELECT_NONE;
    nuINT nRet = 0;
    struct timeval timeout;
	FD_ZERO(&fdSetRead);
	FD_ZERO(&fdSetWrite);
	FD_ZERO(&fdSetError);
	timeout.tv_sec	= socks.ssTimeMs/1000;
	timeout.tv_usec	= 0;
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
        ++i;
	}
    nRet = select(socks.ssCount, &fdSetRead, &fdSetWrite, &fdSetError, &timeout);//for linux
	if( nRet < 0 )
	{
		printf("socket select error\n");
	}
    else if( nRet == 0 )
    {
        //printf("socket select time-out\n");
    }
    else
    {
        for( i = 0; i<socks.ssCount; i++ )
        {
    		nuBYTE nNewState = SOCK_SELECT_NONE;
    		if( (socks.ssSockState[i] & SOCK_SELECT_READ) &&
    			FD_ISSET(socks.sspSocketsList[i]->m_socket, &fdSetRead) )
    		{
    			nNewState |= SOCK_SELECT_READ;
    		}
    		if( (socks.ssSockState[i] & SOCK_SELECT_WRITE) &&
    			FD_ISSET(socks.sspSocketsList[i]->m_socket, &fdSetWrite) )
    		{
    			nNewState |= SOCK_SELECT_WRITE;
    		}
    		if( (socks.ssSockState[i] & SOCK_SELECT_ERROR) &&
    			FD_ISSET(socks.sspSocketsList[i]->m_socket, &fdSetError) )
    		{
    			nNewState |= SOCK_SELECT_ERROR;
    		}
            socks.ssSockState[i] = nNewState;
        }
    }
	return nRet;
}
