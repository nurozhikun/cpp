// ServerProtocolZK.h: interface for the CServerProtocolZK class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERPROTOCOLZK_H__4D809C3B_95C6_4C1E_A25A_3540CF5D6F2F__INCLUDED_)
#define AFX_SERVERPROTOCOLZK_H__4D809C3B_95C6_4C1E_A25A_3540CF5D6F2F__INCLUDED_

#include "ProtocolBaseZK.h"

class CServerProtocolZK : public CProtocolBaseZK
{
public:
	CServerProtocolZK(class CSockClientZK* pClient);
	virtual ~CServerProtocolZK();
    
protected:
    virtual nuVOID ProcPackage(CMD_DATA& cmdData);
private:
    class CSockClientZK*    m_pClient;
    
};

#endif // !defined(AFX_SERVERPROTOCOLZK_H__4D809C3B_95C6_4C1E_A25A_3540CF5D6F2F__INCLUDED_)
