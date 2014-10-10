// SocketServerIO.h: interface for the CSocketServerIO class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCKETSERVERIO_H__7DBC8FAD_AB20_490E_B790_1C72A7B985E4__INCLUDED_)
#define AFX_SOCKETSERVERIO_H__7DBC8FAD_AB20_490E_B790_1C72A7B985E4__INCLUDED_

#include "ISocketServerIO.h"
#include "SockServerZK.h"

class CSocketServerIO : public CISocketServerIO
{
public:
	CSocketServerIO();
	virtual ~CSocketServerIO();
    ///////////////////////////////////////////////////////////////////////
    virtual nubool  StartModule(const SERVER_IO_START& sioStart);
    virtual nuVOID  CloseModule();

    virtual nubool  SockSend(nuLONG nIdx, nuUINT nID, nuUINT nCmd, const nuCHAR* pData, nuUINT nDataLen);
    virtual nubool  SockReply(nuLONG nIdx, nuUINT nID, nuUINT nCmd, const nuCHAR* pData, nuUINT nDataLen);
    virtual nubool  SockClose(nuLONG nIdx, nuUINT nID);
protected:
    CSockServerZK       m_sockServer;
//    CISocketCallBack*   m_pSockCallback;
};

#endif // !defined(AFX_SOCKETSERVERIO_H__7DBC8FAD_AB20_490E_B790_1C72A7B985E4__INCLUDED_)
