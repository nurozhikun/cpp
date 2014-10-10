// ClientProtocolZK.h: interface for the CClientProtocolZK class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENTPROTOCOLZK_H__D0CAF298_B003_4866_B492_CF767D11E354__INCLUDED_)
#define AFX_CLIENTPROTOCOLZK_H__D0CAF298_B003_4866_B492_CF767D11E354__INCLUDED_

#include "ProtocolBaseZK.h"

class CClientProtocolZK : public CProtocolBaseZK 
{
public:
	CClientProtocolZK(class CClientSocketZK* pClient);
	virtual ~CClientProtocolZK();
protected:
    virtual nuVOID ProcPackage(CMD_DATA& cmdData);

    class CClientSocketZK*  m_pClient;

};

#endif // !defined(AFX_CLIENTPROTOCOLZK_H__D0CAF298_B003_4866_B492_CF767D11E354__INCLUDED_)
