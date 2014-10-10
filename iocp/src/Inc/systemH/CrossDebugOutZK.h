// CrossDebugOutZK.h: interface for the CCrossDebugOutZK class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CROSSDEBUGOUTZK_H__DBA6D977_2333_4A01_8B16_04447E1835B5__INCLUDED_)
#define AFX_CROSSDEBUGOUTZK_H__DBA6D977_2333_4A01_8B16_04447E1835B5__INCLUDED_

#include "typeDefine.h"

class CCrossDebugOutZK  
{
public:
	CCrossDebugOutZK();
	virtual ~CCrossDebugOutZK();
    static nuVOID   DebugOutStringA(const nuCHAR* pcs);
    static nuVOID   DebugOutInteger(nuLONG nValue);

};

#endif // !defined(AFX_CROSSDEBUGOUTZK_H__DBA6D977_2333_4A01_8B16_04447E1835B5__INCLUDED_)
