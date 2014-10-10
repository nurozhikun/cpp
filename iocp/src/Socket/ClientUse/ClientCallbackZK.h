// ClientCallbackZK.h: interface for the CClientCallbackZK class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENTCALLBACKZK_H__12B4958B_5D54_4BBF_985C_8095B3D754A0__INCLUDED_)
#define AFX_CLIENTCALLBACKZK_H__12B4958B_5D54_4BBF_985C_8095B3D754A0__INCLUDED_

#include "ISocketClientIO.h"

class CClientCallbackZK : public CIClientCallBack
{
public:
	CClientCallbackZK();
	virtual ~CClientCallbackZK();

    virtual nuVOID  ClientWritable(nubool bWrite);//{ return 0; }
    virtual nuUINT  ClientLogin(nuUINT nLoginMode);//{ return 0; }
    virtual nuVOID  ClientLogout(nuUINT nOutType);//{ return 0; }
    virtual nuUINT  ClientRecved(RECV_DATA& recvData);// { return RB_CLIENT_NOPROCESS; }
    virtual nuUINT  ServerReplyed(RECV_DATA& recvData);
};

#endif // !defined(AFX_CLIENTCALLBACKZK_H__12B4958B_5D54_4BBF_985C_8095B3D754A0__INCLUDED_)
