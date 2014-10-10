// SockBaseZK.h: interface for the CSockBaseZK class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCKBASEZK_H__64218E6C_54A6_423B_871B_A11592DAB404__INCLUDED_)
#define AFX_SOCKBASEZK_H__64218E6C_54A6_423B_871B_A11592DAB404__INCLUDED_

#include "CrossSocketZK.h"
#include "inlineFunctionZK.h"
#include "ISocketServerIO.h"
#include "ClientOverlappedZK.h"
//#include "OverlappedVesselZK.h"

class CSockBaseZK : public CCrossSocketZK
{
public:
public:
	CSockBaseZK();
	virtual ~CSockBaseZK();

protected:
    SOCKET_ADDR m_sockAddr;
    CClientOverlappedZK m_overList;
    CIServerCallBack*    s_pCallback;
    class CSockServerZK* s_pServer;
//    static COverlappedVesselZK  s_overVessel;
public:
	inline CIServerCallBack* GetCallBack(){return s_pCallback;}
	inline void SetCallBack(CIServerCallBack* _callback){s_pCallback = _callback;}
	inline CSockServerZK* GetServer(){return s_pServer;}
	inline void SetServer(CSockServerZK* _server){s_pServer = _server;}
public:
    //inline functions
    nuUINT GetSocket() { return m_socket; }
    SOCKET_ADDR& GetSocketAddr() { return m_sockAddr; }
    COverlappedZK*  AllocOverlapped(nuBYTE nOverType, nuUINT nBufflen) { return m_overList.AllocOverlapped(this, nOverType, nBufflen); }
    virtual nubool  FreeOverlapped(COverlappedZK *pOverlapped) { return m_overList.FreeOverlapped(pOverlapped); }
    nuUINT GetOverlappedUsedCount() { return m_overList.GetNowCount(); }
};

#endif // !defined(AFX_SOCKBASEZK_H__64218E6C_54A6_423B_871B_A11592DAB404__INCLUDED_)
