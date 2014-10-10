// CrossLoadDllZK.cpp: implementation of the CCrossLoadDllZK class.
//
//////////////////////////////////////////////////////////////////////

#include "CrossLoadDllZK.h"
#include "systemInc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const nuTCHAR CCrossLoadDllZK::sc_tsPostfix[] = nuTEXT(".dll");

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
    if( nuNULL != m_hDll )
    {
        return nuNULL;
    }
    nuTCHAR tsFile[NU_MAX_PATH];
    if( NULL == ptsDllPath )
    {
        TCHAR tsPath[MAX_PATH];
        DWORD nNum = GetModuleFileName(NULL, tsPath, sizeof(tsPath));
        for(int i = nNum - 1; i >= 0; --i)
        {
            if( '\\' == tsPath[i] )
            {
                tsPath[++i] = '\0';
                break;
            }
        }
        _tcscpy(tsFile, tsPath);
    }
    else
    {
        _tcscpy(tsFile, ptsDllPath);
    }
    _tcscat(tsFile, m_pptzNames[0]);
    _tcscat(tsFile, sc_tsPostfix);
	if (!m_hDll)
	{
    m_hDll = LoadLibrary(tsFile);
	}
    if( NULL == m_hDll )
    {
        return nuNULL;
    }
    DLL_InitModelProc InitModel = (DLL_InitModelProc)GetProcAddress((HINSTANCE)m_hDll, m_pptzNames[1]);
    if( NULL == InitModel )
    {
        return nuNULL;
    }
    return InitModel(pParam);
}

nuVOID CCrossLoadDllZK::FreeDll()
{
    if( m_hDll)
    {
        DLL_FreeModelProc FreeModel = (DLL_FreeModelProc)GetProcAddress((HINSTANCE)m_hDll, m_pptzNames[2]);
        if( NULL != FreeModel )
        {
            FreeModel();
        }
        FreeLibrary((HINSTANCE)m_hDll);
        m_hDll = nuNULL;
    }
}
