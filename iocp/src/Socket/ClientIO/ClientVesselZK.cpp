// ClientVesselZK.cpp: implementation of the CClientVesselZK class.
//
//////////////////////////////////////////////////////////////////////

#include "ClientVesselZK.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClientVesselZK::CClientVesselZK()
{
    m_ppClientList  = nuNULL;
    m_nMaxClients   = 0;
}

CClientVesselZK::~CClientVesselZK()
{
    if( nuNULL != m_ppClientList )
    {
        for( nuUINT i = 0; i < m_nMaxClients; ++i )
        {
            if( nuNULL != m_ppClientList[i] )
            {
                delete m_ppClientList[i];
            }
        }
        delete[] m_ppClientList;
        m_ppClientList = nuNULL;
    }
}

bool CClientVesselZK::InitClientVessel(nuUINT nMaxClients)
{
	if( NULL != m_ppClientList || nMaxClients <= m_nMaxClients )
	{
		return nuTrue;
	}
	CSocketClientIO **pTmp = new CSocketClientIO*[nMaxClients];
	if( NULL == pTmp )
	{
		return nuFalse;
	}
	if( NULL != m_ppClientList )
	{
		nuMemcpy(pTmp, m_ppClientList, sizeof(CSocketClientIO*)*m_nMaxClients);
		delete[] m_ppClientList;
	}
	else
	{
		nuMemset(pTmp, 0, sizeof(CSocketClientIO*)*nMaxClients);
	}
	m_ppClientList = pTmp;
    m_nMaxClients = nMaxClients;
    return nuTrue;
}

nuVOID CClientVesselZK::FreeClientVessel()
{
}

CISocketClientIO* CClientVesselZK::CreateClient(const CLIENT_START_PARAM& cltParam)
{
    if( nuNULL == m_ppClientList )
    {
        return nuNULL;
    }
    for( nuUINT i = 0; i < m_nMaxClients; ++i )
    {
        if( nuNULL == m_ppClientList[i] )
        {
            m_ppClientList[i] = new CSocketClientIO();
            if( nuNULL == m_ppClientList[i] )
            {
                return nuNULL;
            }
            if( !m_ppClientList[i]->ClientBegin(cltParam) )
            {
                delete m_ppClientList[i];
                m_ppClientList[i] = nuNULL;
                return nuNULL;
            }
            return m_ppClientList[i];
        }
    }
    return nuNULL;
}

nubool CClientVesselZK::DeleteClient(CISocketClientIO* pClient)
{
    if( nuNULL == pClient )
    {
        return nuFalse;
    }
    for( nuUINT i = 0; i < m_nMaxClients; ++i )
    {
        if( pClient == m_ppClientList[i] )
        {
            m_ppClientList[i] = nuNULL;
            delete pClient;
            return nuTrue;
        }
    }
    return nuFalse;
}