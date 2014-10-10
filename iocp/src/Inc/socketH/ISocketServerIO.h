#ifndef _I_SOCKET_SERVER_IO_H_20111125
#define _I_SOCKET_SERVER_IO_H_20111125

#include "typeDefine.h"

class CISocketServerIO
{
public:
    CISocketServerIO() {}
    virtual ~CISocketServerIO() {}
    typedef struct tagSERVER_IO_START
    {
        class CIServerCallBack* sisCallback;
        nuCHAR*                 sisServerIp;
        nuUINT                  sisPort;
        nuUINT                  sisMaxClients;
    }SERVER_IO_START;
    virtual nubool  StartModule(const SERVER_IO_START& sioStart) = 0;
    virtual nuVOID  CloseModule() = 0;
    
    virtual nubool  SockSend(nuLONG nIdx, nuUINT nID, nuUINT nCmd, const nuCHAR* pData, nuUINT nDataLen) = 0;
    virtual nubool  SockReply(nuLONG nIdx, nuUINT nID, nuUINT nCmd, const nuCHAR* pData, nuUINT nDataLen) = 0;
    virtual nubool  SockClose(nuLONG nIdx, nuUINT nID) = 0;
};

///////////////////////////////////////////////////////////
class CIServerCallBack
{
public:
    CIServerCallBack() {}
    virtual ~CIServerCallBack() {}
    //////////////////////////////////////
    enum emReturnValue
    {
        CB_RET_NOPROCESS    = 0,
        CB_RET_NOREPLYED    = 1,
        CB_RET_REPLYED      = 2,
        CB_RET_LOGIN_SRUCESS    = 0x10000000,
        CB_RET_LOGIN_ERROR_ID   = (CB_RET_LOGIN_SRUCESS+1),
        CB_RET_LOGIN_ERROR_USER = (CB_RET_LOGIN_SRUCESS+2),
        CB_RET_LOGIN_ERROR_PW   = (CB_RET_LOGIN_SRUCESS+4),//多个错误时，可以组合错误值的低位值
        CB_RET_LOGIN_ERROR_OTHER= (CB_RET_LOGIN_SRUCESS+8),
    };
    virtual nuUINT  SockLogin(nuLONG nClientIdx, nuUINT nID, const nuCHAR* pdata, nuUINT nLen) = 0;
    enum emClientLogoutType
    {
        LOG_OUT_CLOSE_BY_CLIENT,
        LOG_OUT_CLOSE_BY_SERVER,
        LOG_OUT_CLOSE_BY_SAME,
        LOG_OUT_CLOSE_BY_NET,
        LOG_OUT_CLOSE_BY_DESTROY,
        LOG_OUT_CLOSE_BY_ERROR,
    };
    virtual nuUINT  SockLogout(nuLONG nClientIdx, nuUINT nID, nuUINT nOutType) = 0;
    typedef struct tagSOCK_DATA
    {
        nuUINT      sdID;
        nuUINT      sdCmd;
        nuUINT      sdWholeSize;
        nuUINT      sdNowAddr;
        nuUINT      sdNowSize;
        nuCHAR*     sdDataBuff;
    }SOCK_DATA;
    virtual nuUINT SockRecved(const SOCK_DATA& sockData) = 0;
    virtual nuUINT SockReplyed(const SOCK_DATA& sockData) = 0;
};

#endif