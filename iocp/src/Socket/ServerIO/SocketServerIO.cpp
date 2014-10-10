// SocketServerIO.cpp: implementation of the CSocketServerIO class.
//
//////////////////////////////////////////////////////////////////////

#include "SocketServerIO.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSocketServerIO::CSocketServerIO()
{
//    m_pSockCallback = nuNULL;
}

CSocketServerIO::~CSocketServerIO()
{

}

nubool CSocketServerIO::StartModule(const SERVER_IO_START& sioStart)
{
	m_sockServer.SetCallBack(sioStart.sisCallback);
    if( m_sockServer.StartServer(sioStart.sisMaxClients, sioStart.sisPort, sioStart.sisServerIp) )
    {
        return nuFalse;
    }
    return nuTrue;
}

nuVOID CSocketServerIO::CloseModule()
{
    m_sockServer.CloseServer();
}

nubool CSocketServerIO::SockSend(nuLONG nIdx, nuUINT nID, nuUINT nCmd, const nuCHAR* pData, nuUINT nDataLen)
{
    return m_sockServer.IoClientSend(nIdx, nID, nCmd, pData, nDataLen);
}

nubool CSocketServerIO::SockReply(nuLONG nIdx, nuUINT nID, nuUINT nCmd, const nuCHAR* pData, nuUINT nDataLen)
{
    return m_sockServer.IoClientReply(nIdx, nID, nCmd, pData, nDataLen);
}

nubool CSocketServerIO::SockClose(nuLONG nIdx, nuUINT nID)
{
    return m_sockServer.IoClientClose(nIdx, nID);
}
