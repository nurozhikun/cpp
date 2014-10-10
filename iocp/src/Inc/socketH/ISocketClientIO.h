#ifndef _I_SOCKET_CLIENT_IO_H_20111206
#define _I_SOCKET_CLIENT_IO_H_20111206

#include "typeDefine.h"

class CISocketClientIO
{
public:
    CISocketClientIO()
    {
    }
    virtual ~CISocketClientIO()
    {
    }
    //
	struct RESET_PARAM{
		nuUINT	nClientID;
		nuCHAR* sUserID;
		nuCHAR* sUserPw;
	};
	virtual nubool	ResetClient(const RESET_PARAM& reParam) = 0;
    virtual nuUINT  ClientSend(nuUINT nCmd, const nuCHAR* pData, nuUINT nDataLen) = 0;
    virtual nubool  OutThreadBegin() = 0;
    virtual nubool  OutThreadAction() = 0;
    virtual nubool  OutThreadEnd() = 0;
};

class CIClientCallBack
{
public:
    CIClientCallBack()
    {
    }
    virtual ~CIClientCallBack()
    {
    }
    enum emRecvBack
    {
        RB_CLIENT_NOPROCESS = 0,
        RB_CLIENT_NOREPLY,
        RB_CLIENT_REPLEYED,
    };
    virtual nuVOID  ClientWritable(nubool bWrite) = 0;//{ return 0; }
    enum emLoginMode
    {
        LOG_IN_SUCESS       = 0,
        LOG_IN_WRONG_ID     = 1,
        LOG_IN_WRONG_USER   = 2,
        LOG_IN_WRONG_PW     = 4,
    };
    virtual nuUINT  ClientLogin(nuUINT nLoginMode) = 0;//{ return 0; }
    enum emLogOutType
    {
        CLT_LOG_OUT_NO      = 0,
        CLT_LOG_OUT_USER,
        CLT_LOG_OUT_SERVER,
        CLT_LOG_OUT_NET,
        CLT_LOG_OUT_DESTROY,
        CLT_LOG_OUT_ERROR,
    };
    virtual nuVOID  ClientLogout(nuUINT nOutType) = 0;//{ return 0; }
    typedef struct tagRECV_DATA
    {
        nuUINT      sdID;
        nuUINT      sdCmd;
        nuUINT      sdWholeSize;
        nuUINT      sdNowAddr;
        nuUINT      sdNowSize;
        nuCHAR*     sdDataBuff;
    }RECV_DATA;
    virtual nuUINT  ClientRecved(RECV_DATA& recvData) = 0;// { return RB_CLIENT_NOPROCESS; }
    virtual nuUINT  ServerReplyed(RECV_DATA& recvData) = 0;
	virtual nubool	CanSend() { return nuFalse; }
	virtual nuVOID	FrequentCall() {}
};


class CIClientVessel
{
public:
    CIClientVessel()
    {
    }
    virtual ~CIClientVessel()
    {
    }
    virtual nubool  InitClientVessel(nuUINT nMaxClients) = 0;
    virtual nuVOID  FreeClientVessel() = 0;
    
    typedef struct tagCLIENT_START_PARAM
    {
        nubool                  cspHasOwnerThread;
        const nuTCHAR*          cspTsPath;
        class CIClientCallBack* cspCallback;
        nuCHAR*                 cspSzServerIp;
        nuUINT                  cspServerPort;
        nuUINT                  cspClientID;
        nuCHAR*                 cspUserID;
        nuCHAR*                 cspUserPw;
    }CLIENT_START_PARAM;
    virtual CISocketClientIO*   CreateClient(const CLIENT_START_PARAM& cltParam) = 0;
    virtual nubool  DeleteClient(CISocketClientIO* pClient) = 0;
};

#endif