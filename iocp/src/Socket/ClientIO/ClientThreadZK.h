// ClientThreadZK.h: interface for the CClientThreadZK class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENTTHREADZK_H__E0ABE0BC_E170_4230_A4B5_09C03792923E__INCLUDED_)
#define AFX_CLIENTTHREADZK_H__E0ABE0BC_E170_4230_A4B5_09C03792923E__INCLUDED_

#include "CrossThreadZK.h"
#include "ClientSocketZK.h"

class CClientThreadZK : public CCrossThreadZK 
{
public:
	CClientThreadZK(CClientSocketZK* pclient);
	virtual ~CClientThreadZK();
    
    nuFINAL InitClientThread();
    nuVOID  FreeClientThread();

private:
    virtual nuDWORD ThreadBegin();
    virtual nuDWORD ThreadAction();
    virtual nuDWORD ThreadEnd();
//    virtual nuDWORD MessageProc(CBaseCodeListZK::BASE_LIST_NODE& node) { return 1; }
private:
    CClientSocketZK*    m_pClient;

};

#endif // !defined(AFX_CLIENTTHREADZK_H__E0ABE0BC_E170_4230_A4B5_09C03792923E__INCLUDED_)
