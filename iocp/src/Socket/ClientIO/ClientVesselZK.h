// ClientVesselZK.h: interface for the CClientVesselZK class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENTVESSELZK_H__06A74B21_F98D_4DBE_A6AD_84402B283E61__INCLUDED_)
#define AFX_CLIENTVESSELZK_H__06A74B21_F98D_4DBE_A6AD_84402B283E61__INCLUDED_

#include "SocketClientIO.h"

class CClientVesselZK : public CIClientVessel  
{
public:
	CClientVesselZK();
	virtual ~CClientVesselZK();

    virtual nubool  InitClientVessel(nuUINT nMaxClients);
    virtual nuVOID  FreeClientVessel();

    virtual CISocketClientIO*   CreateClient(const CLIENT_START_PARAM& cltParam);
    virtual nubool  DeleteClient(CISocketClientIO* pClient);

private:
    CSocketClientIO**   m_ppClientList;
    nuUINT              m_nMaxClients;
};

#endif // !defined(AFX_CLIENTVESSELZK_H__06A74B21_F98D_4DBE_A6AD_84402B283E61__INCLUDED_)
