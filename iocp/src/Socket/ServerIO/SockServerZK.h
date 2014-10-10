// SockServerZK.h: interface for the CSockServerZK class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCKSERVERZK_H__E1A619D6_868E_422A_BB8E_0436E3978832__INCLUDED_)
#define AFX_SOCKSERVERZK_H__E1A619D6_868E_422A_BB8E_0436E3978832__INCLUDED_

#include "SockBaseZK.h"
#include "CrossIOCompletionZK.h"
#include "SocketThreadZK.h"
//#include "OverlappedVesselZK.h"
#include "SockClientMgrZK.h"

class CSockServerZK : public CSockBaseZK
{
public:
    enum emSockServerConst
    {
        CONST_SERVER_PORT           = 5050,
        CONST_ACCPET_COUNT          = 10,
//        CONST_BUF_SIZE_ACCEPTE      = 128,
//        CONST_BUF_SIZE_RECV         = 1024,
    };
    enum emIoCompetionKey
    {
        IO_KEY_DEFAULT = 0,
        IO_KEY_QUIT,
        IO_KEY_SOCKSERVER,
        IO_KEY_SOCKCLIENT,
        IO_KEY_CLOSE_CLIENT,
        IO_KEY_CLIENT_ERROR,
        IO_KEY_OTHER,
    };
public:
	CSockServerZK();
	virtual ~CSockServerZK();

    nuFINAL StartServer(nuUINT nMaxCount, nuUINT nPort = CONST_SERVER_PORT, nuCHAR* psIP = nuNULL);
    nuFINAL CloseServer();

    enum emIOProcessResult
    {
        IO_PROC_SUCESS          = 0,
        IO_PROC_ACCEPT_SUCESS   = 1,
        IO_PROC_RECV_SUCESS     = 2,
        IO_PROC_SEND_SUCESS     = 3,
        IO_PROC_CLIENT_EXIT     = 0x10,
        IO_PROC_CLIENT_ERROR    = 0x20,
    };
    nuUINT  IOCompletionProc();

    nubool  IoClientSend(nuLONG nIdx, nuUINT nID, nuUINT nCmd, const nuCHAR* pData, nuUINT nDataLen);
    nubool  IoClientReply(nuLONG nIdx, nuUINT nID, nuUINT nCmd, const nuCHAR* pData, nuUINT nDataLen);
    nubool  IoClientClose(nuLONG nIdx, nuUINT nID);
private:
    nubool  CreateThreadPools();
    nuVOID  DeleteThreadPools(); 
    nuUINT  AddClientToAccept();
    nuUINT  IoAccepted(CSockClientZK* pClient, COverlappedZK *pOverlapped, nuDWORD dwBytes);
    nuUINT  IoClient(CSockClientZK* pClient, COverlappedZK *pOverlapped, nuDWORD dwBytes);
    //#debug code
    enum emDebugCode
    {
        DEBUG_CLOSE,
        DEBUG_ACCEPTED,
        DEBUG_RECV,
        DEBUG_SEND,
    };
    nuVOID  DebugOut(emDebugCode debug, COverlappedZK* pOverlapped, CSockClientZK* pClient, nuUINT nlen = 0);
    //#
private:
    SOCKET_IN   m_sockIn;
    CCrossIOCompletionZK    m_ioCompletionPort;
//    COverlappedVesselZK     m_overVessel;
    nuUINT                  m_nThreadCount;
    CSocketThreadZK*        m_pThreadPool;
    CSockClientMgrZK        m_clientMgr;
    nuUINT                  m_nAcceptReadyCount;
public:
    //inline functions
    nubool  AssociateIoPort(CSockBaseZK* psock, nuUINT nKey = IO_KEY_SOCKSERVER)
    {
        return m_ioCompletionPort.AssociateIoPort((nuHOBJECT)psock->GetSocket(), nKey);
    }
//     COverlappedZK* GetOverlapped(CCrossSocketZK* pSock, nuUINT nBuffLen, nuBYTE nOverType)
//     {
//         return m_overVessel.GetOverlapped(pSock, nBuffLen, nOverType);
//     }
//     nuVOID RelOverlapped(COverlappedZK* pOverlapped)
//     {
//         m_overVessel.RelOverlapped(pOverlapped);
//     }
//     nuVOID DeleteClientOverlapped(CSockClientZK* pClient)
//     {
//         m_overVessel.DeleteClientOverlapped(pClient);
//     }
    CSockClientZK*  GetClient(nuLONG nIdx, nuUINT nID) { return m_clientMgr.GetClient(nIdx, nID); }
//    nuUINT RemoveUnusefuls(CSockClientZK* pClient) { return m_clientMgr.RemoveUnusefuls(pClient); }
    nuUINT CheckIdleClients() { return m_clientMgr.CheckIdleClients(); }
    nubool RemoveSameClient(CSockClientZK* pClient) { return m_clientMgr.RemoveSameClient(pClient); }
	nubool IdIsExit(CSockClientZK* pClient) { return m_clientMgr.IdIsExit(pClient); }
};

#endif // !defined(AFX_SOCKSERVERZK_H__E1A619D6_868E_422A_BB8E_0436E3978832__INCLUDED_)
