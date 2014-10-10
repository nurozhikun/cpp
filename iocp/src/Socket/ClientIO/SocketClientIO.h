// SocketClientIO.h: interface for the CSocketClientIO class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCKETCLIENTIO_H__9F2B27C7_08FD_409A_8A7D_340D2FE36E42__INCLUDED_)
#define AFX_SOCKETCLIENTIO_H__9F2B27C7_08FD_409A_8A7D_340D2FE36E42__INCLUDED_

#include "ISocketClientIO.h"
#include "ClientThreadZK.h"

class CSocketClientIO : public CISocketClientIO
{
public:
	CSocketClientIO();
	virtual ~CSocketClientIO();
    
    nubool  ClientBegin(const CIClientVessel::CLIENT_START_PARAM& cltParam);
    nuVOID  ClientEnd();
	virtual nubool	ResetClient(const RESET_PARAM& reParam);
    virtual nuUINT  ClientSend(nuUINT nCmd, const nuCHAR* pData, nuUINT nDataLen);
    ///////////////////////////////////////////////////////////////////////////////////
    virtual nubool  OutThreadBegin();
    virtual nubool  OutThreadAction();
    virtual nubool  OutThreadEnd();
private:
    CClientSocketZK     m_client;
    CClientThreadZK*    m_pThread;
    nubool              m_bOutThreadWorking;
};

#endif // !defined(AFX_SOCKETCLIENTIO_H__9F2B27C7_08FD_409A_8A7D_340D2FE36E42__INCLUDED_)
