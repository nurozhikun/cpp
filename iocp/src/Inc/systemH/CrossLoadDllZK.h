// CrossLoadDllZK.h: interface for the CCrossLoadDllZK class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CROSSLOADDLLZK_H__4C94BB5B_EF6D_4D47_9A43_DB50836083B7__INCLUDED_)
#define AFX_CROSSLOADDLLZK_H__4C94BB5B_EF6D_4D47_9A43_DB50836083B7__INCLUDED_

#include "typeDefine.h"

class CCrossLoadDllZK  
{
public:
	typedef nuVOID*(*DLL_InitModelProc)(nuVOID*);
    typedef nuVOID(*DLL_FreeModelProc)();
public:
	CCrossLoadDllZK(const nuTCHAR* pDllName, const nuTCHAR* pDllInitName, const nuTCHAR* pDllFreeName);
	virtual ~CCrossLoadDllZK();

	nuVOID*	LoadDll(const nuTCHAR* ptsDllPath, nuVOID* pParam);
	nuVOID	FreeDll();
protected:
	nuHOBJECT		m_hDll;
protected:
	static const nuTCHAR	sc_tsPostfix[];
	const nuTCHAR*			m_pptzNames[3];

};

#endif // !defined(AFX_CROSSLOADDLLZK_H__4C94BB5B_EF6D_4D47_9A43_DB50836083B7__INCLUDED_)
