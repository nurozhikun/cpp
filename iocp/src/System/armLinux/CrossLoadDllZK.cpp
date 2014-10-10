// CrossLoadDllZK.cpp: implementation of the CCrossLoadDllZK class.
//
//////////////////////////////////////////////////////////////////////

#include "CrossLoadDllZK.h"
#include "systemInc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const nuTCHAR CCrossLoadDllZK::sc_tsPostfix[] = nuTEXT(".so");

CCrossLoadDllZK::CCrossLoadDllZK(const nuTCHAR* pDllName, const nuTCHAR* pDllInitName, const nuTCHAR* pDllFreeName)
{
	m_pptzNames[0]	= pDllName;
	m_pptzNames[1]	= pDllInitName;
	m_pptzNames[2]  = pDllFreeName;
	m_hDll			= nuNULL;
}

CCrossLoadDllZK::~CCrossLoadDllZK()
{

}

nuVOID* CCrossLoadDllZK::LoadDll(const nuTCHAR* ptsDllPath, nuVOID* pParam)
{
    if( NULL == ptsDllPath || nuNULL != m_hDll )
    {
        return nuNULL;
    }
    nuTCHAR tsFile[NU_MAX_PATH];
    strcpy(tsFile, ptsDllPath);
    strcat(tsFile, m_pptzNames[0]);
    strcat(tsFile, sc_tsPostfix);
    m_hDll = dlopen(tsFile, RTLD_NOW);
    return m_hDll;
}

nuVOID CCrossLoadDllZK::FreeDll()
{
    if(m_hDll)
    {
        dlclose(m_hDll);
        m_hDll = nuNULL;
    }
}
