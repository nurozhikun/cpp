// ClientSocketZK.h: interface for the CClientSocketZK class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENTSOCKETZK_H__6D9925EC_40DF_4D4E_A417_274205D16D0E__INCLUDED_)
#define AFX_CLIENTSOCKETZK_H__6D9925EC_40DF_4D4E_A417_274205D16D0E__INCLUDED_

#include "CrossSocketZK.h"
#include "inlineFunctionZK.h"
#include "ISocketClientIO.h"
#include "ClientProtocolZK.h"
#include "BaseLogZK.h"

#define _SEND_HEARTBEAT_ALWAYS

class CClientSocketZK : public CCrossSocketZK 
{
public:
    enum emConstants
    {
//        CONST_SEND_BUFFER_SIZE          = 1024,
//        CONST_RECV_BUFFER_SIZE          = 1024,
        CONST_USER_LOGIN_INTERVAL       = 5*1000,//ms
        CONST_USER_HEARTBEAT_INTERVAL   = 30*1000,//ms
    };
public:
	CClientSocketZK();
	virtual ~CClientSocketZK();

    nubool  SetServer(nuCHAR* szServerIp, nuUINT szPort, nuUINT nClienID
        , nuCHAR* szUserID, nuCHAR* szUserPw, CIClientCallBack* pCallback);
    nuVOID  RelServer();
    nuVOID  Logout(nuUINT nOutType);
    nubool  ClientAction();
    nuUINT  SendTo(nuUINT nCmd, const nuCHAR* pData, nuUINT nDataLen);
    nuVOID  ServerReply(CIClientCallBack::RECV_DATA& recvData);
    nuVOID  ServerFrom(CIClientCallBack::RECV_DATA& recvData);
private:
    nubool  CreateClient();
    nuVOID  DeleteClient();
    nubool  ConnectServer();
    nuVOID  Logining();
    nuVOID  Login(const nuCHAR* pdata, nuUINT nLen);
    nuVOID  HeardBeatSend();
    nuVOID  HeardBeatRecv();
    //
    nuUINT  FdWrite();
    nuUINT  FdRead();
    nuUINT  FdError();
    nubool  Reconnect();
    enum emDebugCode
    {
        DEBUG_CLOSE,
        DEBUG_CONNECTED,
        DEBUG_RECV,
        DEBUG_SEND,
    };
    nuVOID  DebugOut(nuUINT nDebug = DEBUG_CLOSE);
private:
    nuUINT          m_nClientID;
    SOCKET_ADDR     m_sockAddr;
    nubool          m_bNeedCreate;
    nubool          m_bNeedConnect;
    nubool          m_bConnected;
    nubool          m_bWrite;
    nubool          m_bLogin;
    nubool          m_bLock;
	CRITICAL_SECTION m_bSection;
    nuDWORD         m_nLoginTicks;
    nuCHAR          m_userData[SCC_USER_ID_SIZE + SCC_USER_PW_SIZE];
    SOCKET_BUFFER   m_buffSend;
    SOCKET_BUFFER   m_buffRecv;
    CIClientCallBack*   m_pCallback;
    CClientProtocolZK   m_protocol;
    nuDWORD         m_nHeartBeatTicks;
    nuDWORD         m_nHeartBeatCount;
    nuDWORD         m_nRecvTicks;
	nuINT			m_nLoginError;
public://inline functions
    nubool  NeedConnect() { return m_bNeedConnect; }
    nubool  HasLogin() { return m_bLogin; }
};

#endif // !defined(AFX_CLIENTSOCKETZK_H__6D9925EC_40DF_4D4E_A417_274205D16D0E__INCLUDED_)
