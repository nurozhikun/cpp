// OverlappedVesselZK.cpp: implementation of the COverlappedVesselZK class.
//
//////////////////////////////////////////////////////////////////////

#include "OverlappedVesselZK.h"
#include "inlineFunctionZK.h"
#include "CCriticalLock.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COverlappedVesselZK::COverlappedVesselZK()
{
    m_pHeader    = nuNULL;
    m_nNowCount  = 0;
    m_nPageCount = 0;
   // m_bLock      = nuFalse;
	InitializeCriticalSection(&m_bSection);
}

COverlappedVesselZK::~COverlappedVesselZK()
{
    while( m_pHeader )
    {
        nuUINT nCount = nuFUNC_MIN(PAGE_SIZE, m_nNowCount);
        for( nuUINT i = 0; i < nCount; ++i )
        {
            if( nuNULL != m_pHeader->pPage[i] )
            {
                delete m_pHeader->pPage[i];
            }
        }
        m_nNowCount -= nCount;
        OVERLAPPEDS_NODE* ptmp = m_pHeader->pNextPage;
        delete m_pHeader;
        m_pHeader = ptmp;
    }
	DeleteCriticalSection(&m_bSection);
}

COverlappedZK* COverlappedVesselZK::GetOverlapped(CCrossSocketZK* pSock, nuUINT nBuffLen, nuBYTE nOverType)
{
   // CBaseLock lock(&m_bLock);
	CCriticalLock lock(&m_bSection);
    COverlappedZK* pOverlapped = new COverlappedZK(m_nNowCount, pSock, nBuffLen, nOverType);
    if( pOverlapped )
    {
        PushOverlapped(pOverlapped);
    }
    return pOverlapped;
}

nuVOID COverlappedVesselZK::RelOverlapped(COverlappedZK* pOverlapped)
{
    if( pOverlapped )
    {
        //CBaseLock lock(&m_bLock);
		CCriticalLock lock(&m_bSection);
        if( PopOverlapped(pOverlapped) )
        {
            delete pOverlapped;
        }
    }
}

nubool COverlappedVesselZK::PushOverlapped(COverlappedZK* pOverlapped)
{
    nuDWORD nTotalCount = m_nPageCount * PAGE_SIZE;
    OVERLAPPEDS_NODE* pNode;
    if( m_nNowCount >= nTotalCount )
    {
        pNode = new OVERLAPPEDS_NODE;
        nuMemset(pNode, 0, sizeof(OVERLAPPEDS_NODE));
        if( nuNULL == m_pHeader )
        {
            m_pHeader = pNode;
        }
        else
        {
            while( nuNULL != m_pHeader->pNextPage )
            {
                m_pHeader = m_pHeader->pNextPage;
            }
            m_pHeader->pNextPage = pNode;
        }
        ++m_nPageCount;
    }
    else
    {
        pNode = m_pHeader;
        nuINT nPageIdx = m_nNowCount / PAGE_SIZE;
        while( nPageIdx > 0 )
        {
            pNode = pNode->pNextPage;
            --nPageIdx;
        }
    }
    nuUINT idx = m_nNowCount % PAGE_SIZE;
    ++m_nNowCount;
    pNode->pPage[idx] = pOverlapped;
    return nuTrue;
}

nubool COverlappedVesselZK::PopOverlapped(COverlappedZK* pOverlapped)
{
    nubool bres = nuFalse;
    if( pOverlapped )
    {
        nuUINT nIdx = pOverlapped->GetIndex();
        if( nIdx < m_nNowCount )
        {
            nuUINT nPage = nIdx / PAGE_SIZE;
            OVERLAPPEDS_NODE* pNode = m_pHeader;
            while( nPage > 0 )
            {
                pNode = pNode->pNextPage;
                --nPage;
            }
            nuUINT nAfters = m_nNowCount - nIdx - 1;
            nIdx %= PAGE_SIZE;
            if( pOverlapped == pNode->pPage[nIdx] )
            {
                COverlappedZK** ppNow = &pNode->pPage[nIdx];
                while( nAfters > 0 )
                {
                    ++nIdx;
                    if( nIdx == PAGE_SIZE )
                    {
                        pNode = pNode->pNextPage;
                        nIdx = 0;
                    }
                    pNode->pPage[nIdx]->DecIndex();
                    *ppNow = pNode->pPage[nIdx];
                    ppNow  = &pNode->pPage[nIdx];
                    --nAfters;
                }
                bres = nuTrue;
                --m_nNowCount;
            }
        }
    }
    return bres;
}

nuUINT COverlappedVesselZK::DeleteClientOverlapped(CCrossSocketZK* pClient)
{
    if( nuNULL == pClient || nuNULL == m_pHeader )
    {
        return 0;
    }
    nuUINT nRes = 0;
    //CBaseLock lock(&m_bLock);
    CCriticalLock lock(&m_bSection);
	nuUINT nCount = m_nNowCount;
    nuUINT i = 0;
    OVERLAPPEDS_NODE* pNow  = m_pHeader;
	return 1;
}
