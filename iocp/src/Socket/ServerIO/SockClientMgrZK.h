// SockClientMgrZK.h: interface for the CSockClientMgrZK class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCKCLIENTMGRZK_H__31479C9A_4AE8_4349_B381_9415779C52D4__INCLUDED_)
#define AFX_SOCKCLIENTMGRZK_H__31479C9A_4AE8_4349_B381_9415779C52D4__INCLUDED_

#include "SockClientZK.h"

class CSockClientMgrZK  
{
public:
	CSockClientMgrZK();
	virtual ~CSockClientMgrZK();

    nuFINAL InitClientMgr(nuUINT nMaxCount);
    nuVOID  FreeClientMgr();

    CSockClientZK*  AllocatClient();
    CSockClientZK*  GetClient(nuLONG nIdx, nuUINT nID);
/*    nuUINT          RemoveUnusefuls(CSockClientZK* pClient = nuNULL);*/
    nubool          RemoveSameClient(CSockClientZK* pClient);
	nubool			IdIsExit(CSockClientZK* pClient);
    nuUINT          CheckIdleClients();
private:
    nuUINT              m_nMaxCount;
    CSockClientZK*      m_pClientList;
  //  nubool              m_bLock;
	CRITICAL_SECTION m_bSection;
    nuDWORD             m_nLastRemoveTicks;

};

#endif // !defined(AFX_SOCKCLIENTMGRZK_H__31479C9A_4AE8_4349_B381_9415779C52D4__INCLUDED_)
