// CrossSocketZK.h: interface for the CCrossSocketZK class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CROSSSOCKETZK_H__ECCFDA9F_5925_4A2B_866B_5E5D82B4BBD3__INCLUDED_)
#define AFX_CROSSSOCKETZK_H__ECCFDA9F_5925_4A2B_866B_5E5D82B4BBD3__INCLUDED_

#include "typeDefine.h"
#include "inlineFunctionZK.h"

#define SOCKET_ADDRESS_LENGTH                   16

class CCrossSocketZK// : public CISocketZK
{
public:
    CCrossSocketZK();
    virtual ~CCrossSocketZK();
    
    static nubool    StartUp();
    static nuVOID    CleanUp();
    
    enum emSocketFamily
    {
        SOCK_AF_UNSPEC,
        SOCK_AF_UNIX,
        SOCK_AF_INET,
    };
    enum emSocketType
    {
        SOCK_T_STREAM     = 1,               /* stream socket */
        SOCK_T_DGRAM      = 2,               /* datagram socket */
        SOCK_T_RAW        = 3,               /* raw-protocol interface */
        SOCK_T_RDM        = 4,               /* reliably-delivered message */
        SOCK_T_SEQPACKET  = 5,  
    };
    enum emSocketProtocol
    {
        SOCK_IPPROTO_IP         = 0,             /* dummy for IP */
        SOCK_IPPROTO_ICMP       = 1,               /* control message protocol */
        SOCK_IPPROTO_IGMP       = 2,               /* internet group management protocol */
        SOCK_IPPROTO_GGP        = 3,              /* gateway^2 (deprecated) */
        SOCK_IPPROTO_TCP        = 6,               /* tcp */
        SOCK_IPPROTO_PUP        = 12,              /* pup */
        SOCK_IPPROTO_UDP        = 17,              /* user datagram protocol */
        SOCK_IPPROTO_IDP        = 22,              /* xns idp */
        SOCK_IPPROTO_ND         = 77,              /* UNOFFICIAL net disk proto */ 
        SOCK_IPPROTO_RAW        = 255,             /* raw IP packet */
        SOCK_IPPROTO_MAX        = 256,
    };
    enum emSocketFlag
    {
        SOCK_FLAG_DEFAULT       = 0,
        SOCK_FLAG_OVERLAPPED    = 0x01,
        SOCK_FLAG_NONBLOCKING   = 0x02,
    };
    typedef struct tagSOCKET_IN
    {
        nuINT    siAF;
        nuINT    siType;
        nuINT    siProtocol;
        nuDWORD    siFlag;
        nuVOID*    siProtData;
    }SOCKET_IN;
    nuUINT  SockGetHostIP(nuCHAR* psIPBuff, nuUINT nIpBuffLen, nuCHAR* pcsHostName = nuNULL);
    nubool    SockCreate(const SOCKET_IN* pSocketIn = nuNULL);
    nubool    SockClose();
    enum emSockShutFlag
    {
        SOCK_SHUT_RECV = 0,
        SOCK_SHUT_SEND,
        SOCK_SHUT_BOTH,
    };
    nubool    SockShutdown(nuINT nFlag = SOCK_SHUT_BOTH);
    
    typedef struct tagSOCKET_ADDR
    {
        nuSHORT        sinFamily;
        nuWORD        sinPort;
        nuCHAR        sinAddr[24];
    }SOCKET_ADDR;
    nubool    SockBind(SOCKET_ADDR &socketAddr);
    nubool    SockListen(nuINT nLog = 0);
    nubool  SockAccept(CCrossSocketZK& s, SOCKET_ADDR* pSocketAddr);
    nuINT   SockAcceptEx(CCrossSocketZK& s, nuCHAR *pBuffer, nuDWORD& dwRecvs, nuVOID* pOverlapped);
    nuVOID  SockGetAcceptExAddr(nuVOID* pBuffer, SOCKET_ADDR* pRemoteAddr, SOCKET_ADDR* pLocalAddr = nuNULL );
    nubool  SockSetOption(CCrossSocketZK& sListen);
    typedef struct tagSOCKET_BUFFER
    {
        nuDWORD        sbBytes;
        nuDWORD        sbFlags;
        struct tagBUFFER 
        {
            nuDWORD    nBufferLen;
            nuCHAR*    pBuffer;
        }sbBuffer;
    }SOCKET_BUFFER;
    static nubool  CreateSocketBuffer(SOCKET_BUFFER& buff, nuUINT nBufLen)
    { 
        buff.sbBuffer.pBuffer = (nuCHAR*)nuMalloc(nBufLen);
        if( buff.sbBuffer.pBuffer )
        {
            buff.sbBuffer.nBufferLen    = nBufLen;
            return nuTrue;
        }
        else
        {
            buff.sbBuffer.nBufferLen    = 0;
            return nuFalse;
        }
    }
    static nuVOID  DeleteSocketBuffer(SOCKET_BUFFER& buff) 
    { 
        if( buff.sbBuffer.pBuffer ) 
        {
            nuFree(buff.sbBuffer.pBuffer);
            buff.sbBuffer.pBuffer   = nuNULL;
        }
    }
//     typedef struct tagSOCKET_SELF_OVERLAPPED
//     {
//         nuINT        ssoReserver;
//     }SOCKET_SELF_OVERLAPPED;
//      nuVOID* CreateOverlapped();
//      nuVOID    DeleteOverlapper(nuVOID* pOverlapped);
//     SOCKET_SELF_OVERLAPPED* GetSelfOverlapped(nuVOID* pOverlapped);
    enum emSOCK_ERROR
    {
        SOCK_ACTION_SUCCESS    = 0,
        SOCK_ACTION_FAILURE,
        SOCK_ACTION_PENDING,
        SOCK_ACTION_ERROR    = -1,
    };
    nuINT    SockSend(SOCKET_BUFFER& sockBuffer, nuVOID* pOverlapped);
    nuINT    SockRecv(SOCKET_BUFFER& sockBuffer, nuVOID* pOverlapped);
    nuINT   SockGetOverlappedResult(nuVOID* pOverlapped, nuDWORD &dwBytes, nuDWORD &dwFlags);
    nuINT    SockConnect(SOCKET_ADDR& sockAddr);
#define SOCKET_SELECT_LIST_COUNT        32
    enum emSocketSelectState
    {
        SOCK_SELECT_NONE    = 0,
        SOCK_SELECT_READ    = 1,
        SOCK_SELECT_WRITE    = 2,
//        SOCK_SELECT_CLOSE    = 4,
//        SOCK_SELECT_CONNECT = 8,
        SOCK_SELECT_ERROR    = 128,
        SOCK_SELECT_ALL     = 0xFF
    };
    typedef struct tagSOCKET_SELECT
    {
        nuLONG    ssTimeMs;
        nuUINT    ssCount;
        nuBYTE    ssSockState[SOCKET_SELECT_LIST_COUNT];
        CCrossSocketZK*    sspSocketsList[SOCKET_SELECT_LIST_COUNT];
    }SOCKET_SELECT;
    nuINT    SockSelect(SOCKET_SELECT& socks);
    nuUINT            m_opt_value; //only for linux    
    nuUINT            m_opt_name;  //only for linux    
protected:
    static nuINT    s_nSDllError;
protected:
    nuUINT            m_socket;
    nuUINT            m_if_socket_valid; //only for linux
};

#endif // !defined(AFX_CROSSSOCKETZK_H__ECCFDA9F_5925_4A2B_866B_5E5D82B4BBD3__INCLUDED_)
