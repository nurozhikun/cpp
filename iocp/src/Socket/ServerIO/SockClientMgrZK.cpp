// SockClientMgrZK.cpp: implementation of the CSockClientMgrZK class.
//
//////////////////////////////////////////////////////////////////////

#include "SockClientMgrZK.h"
#include "inlineFunctionZK.h"
#include "CCriticalLock.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSockClientMgrZK::CSockClientMgrZK()
{
    m_nMaxCount         = 0;
    m_pClientList       = nuNULL;
 //   m_bLock             = nuFalse;
    m_nLastRemoveTicks  = 0;
	InitializeCriticalSection(&m_bSection);
}

CSockClientMgrZK::~CSockClientMgrZK()
{
	DeleteCriticalSection(&m_bSection);
}

nuFINAL CSockClientMgrZK::InitClientMgr(nuUINT nMaxCount)
{
    if( 0 == nMaxCount )
    {
        return FINAL_FAILURE;
    }
    m_pClientList = new CSockClientZK[nMaxCount];
    if( nuNULL == m_pClientList )
    {
        return FINAL_FAILURE;
    }
    m_nMaxCount = nMaxCount;
    return FINAL_SUCCESS;
}

nuVOID CSockClientMgrZK::FreeClientMgr()
{
    if( nuNULL != m_pClientList )
    {
        delete[] m_pClientList;
        m_pClientList   = nuNULL;
        m_nMaxCount     = 0;
    }
}

CSockClientZK* CSockClientMgrZK::AllocatClient()
{
  //  CBaseLock lock(&m_bLock);
	CCriticalLock lock(&m_bSection);
    if( nuNULL == m_pClientList )
    {
        return nuNULL;
    }
    for(nuUINT i = 0; i < m_nMaxCount; ++i)
    {
        if( m_pClientList[i].IsInvalid() )
        {
            m_pClientList[i].SetIndex(i);
            return &m_pClientList[i];
        }
    }
    return nuNULL;
}

CSockClientZK* CSockClientMgrZK::GetClient(nuLONG nIdx, nuUINT nID)
{
    if( nIdx < 0 || (nuDWORD)nIdx >= m_nMaxCount || nuNULL == m_pClientList || nID != m_pClientList[nIdx].GetID() )
    {
        return nuNULL;
    }
    else
    {
        return &m_pClientList[nIdx];
    }
}
// 
// nuUINT CSockClientMgrZK::RemoveUnusefuls(CSockClientZK* pClient /* = nuNULL */)
// {
//     if( nuNULL == m_pClientList )
//     {
//         return 0;
//     }
//     if( nuNULL == pClient && (nuGetTickCount() - m_nLastRemoveTicks < CSockClientZK::CONST_NO_RESPONSE_INTERVAL) )
//     {
//         return 0;
//     }
//     CBaseLock lock(&m_bLock);
//     nuUINT nRet = 0;
//     m_nLastRemoveTicks = nuGetTickCount();
//     for( nuUINT i = 0; i < m_nMaxCount; ++i )
//     {
//         if( m_pClientList[i].IsAccpepted() && 
//             ( m_pClientList[i].IsNoResponse(m_nLastRemoveTicks) || 
//               ( ((nuNULL != pClient) && (nuLONG)i != pClient->GetIndex() && pClient->GetID() == m_pClientList[i].GetID() ))) )
//         {
//             if( nuNULL == pClient )
//             {
//                 m_pClientList[i].IoClientQuit(CIServerCallBack::LOG_OUT_CLOSE_BY_NET);
//             }
//             else
//             {
//                 m_pClientList[i].IoClientQuit(CIServerCallBack::LOG_OUT_CLOSE_BY_SAME);
//             }
//             ++nRet;
//         }
//     }
//     return nRet;
// }

nubool CSockClientMgrZK::RemoveSameClient(CSockClientZK* pClient)
{
    if( nuNULL == pClient || nuNULL == m_pClientList )
    {
        return nuFalse;
    }
    //CBaseLock lock(&m_bLock);
	CCriticalLock lock(&m_bSection);
    for( nuUINT i = 0; i < m_nMaxCount; ++i )
    {
        if( m_pClientList[i].IsAccpepted() && (nuLONG)i != pClient->GetIndex() && pClient->GetID() == m_pClientList[i].GetID() )
        {
            m_pClientList[i].IoSend(m_pClientList[i].GetID(), CMD_CLIENT_QUIT_BY_SAME, nuNULL, 0);
            m_pClientList[i].IoClientQuit(CIServerCallBack::LOG_OUT_CLOSE_BY_SAME);
            return nuTrue;
        }
    }
    return nuFalse;
}

nubool CSockClientMgrZK::IdIsExit(CSockClientZK* pClient)
{
	if( nuNULL == pClient || nuNULL == m_pClientList )
	{
		return nuFalse;
	}
	for( nuUINT i = 0; i < m_nMaxCount; ++i )
	{
		if( m_pClientList[i].IsAccpepted() && (nuLONG)i != pClient->GetIndex() && pClient->GetID() == m_pClientList[i].GetID() )
		{
			return nuTrue;
		}
	}
	return nuFalse;
}

nuUINT CSockClientMgrZK::CheckIdleClients()
{
    if( nuNULL == m_pClientList || nuGetTickCount() - m_nLastRemoveTicks < CSockClientZK::CONST_NO_RESPONSE_INTERVAL )
    {
        return 0;
    }
   // CBaseLock lock(&m_bLock);
	CCriticalLock lock(&m_bSection);
    nuUINT nRet = 0;
    m_nLastRemoveTicks = nuGetTickCount();
    for( nuUINT i = 0; i < m_nMaxCount; ++i )
    {
        if( m_pClientList[i].IsAccpepted() && 
            m_pClientList[i].IsNoResponse(m_nLastRemoveTicks))
        {
            m_pClientList[i].IoClientQuit(CIServerCallBack::LOG_OUT_CLOSE_BY_NET);
            ++nRet;
        }
    }
    return nRet;
}
