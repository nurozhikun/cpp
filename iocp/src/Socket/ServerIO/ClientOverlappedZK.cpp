// ClientOverlappedZK.cpp: implementation of the CClientOverlappedZK class.
//
//////////////////////////////////////////////////////////////////////
#include "ClientOverlappedZK.h"
#include "BaseLogZK.h"
#include "CCriticalLock.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClientOverlappedZK::CClientOverlappedZK()
{
    nuMemset(m_ppOverlappedList, 0, sizeof(m_ppOverlappedList));
    m_nNowCount = 0;
    m_bLock     = nuFalse;
	InitializeCriticalSection(&m_bSection);
}

CClientOverlappedZK::~CClientOverlappedZK()
{
//     for( nuUINT i = 0; i < CONST_MAX_OVERLAPPED_COUNT; ++i )
//     {
//         if( nuNULL != m_ppOverlappedList[i] )
//         {
//             delete m_ppOverlappedList[i];
//         }
//     }
    CleanOverlapped();
	DeleteCriticalSection(&m_bSection);
}

COverlappedZK* CClientOverlappedZK::AllocOverlapped(CCrossSocketZK* pSock, 
                                                    nuBYTE nOverType /* = OLP_TYPE_ACCEPT */, 
                                                    nuUINT nBuffLen /* = CONST_BUFFER_DEFAULT_LEN */)
{
    if( nuNULL == pSock || CONST_MAX_OVERLAPPED_COUNT == m_nNowCount )
    {
        return nuNULL;
    }
    //CBaseLock lock(&m_bLock);
	CCriticalLock lock(&m_bSection);
    for( nuUINT i = 0; i < CONST_MAX_OVERLAPPED_COUNT; ++i )
    {
        if( nuNULL == m_ppOverlappedList[i] )
        {
            m_ppOverlappedList[i] = new COverlappedZK(i, pSock, nBuffLen, nOverType);
            if( nuNULL == m_ppOverlappedList[i] )
            {
                break;
            }
            ++m_nNowCount;
            return m_ppOverlappedList[i];
        }
    }
    return nuNULL;
}

nubool CClientOverlappedZK::FreeOverlapped(COverlappedZK* pOverlapped)
{
    if( 0 == m_nNowCount )
    {
        return nuFalse;
    }
    //CBaseLock lock(&m_bLock);
	CCriticalLock lock(&m_bSection);
    for( nuUINT i = 0; i < CONST_MAX_OVERLAPPED_COUNT; ++i )
    {
        if( pOverlapped == m_ppOverlappedList[i] )
        {
            delete m_ppOverlappedList[i];
            m_ppOverlappedList[i]   = nuNULL;
            --m_nNowCount;
            return nuTrue;
        }
    }
    return nuFalse;
}

nuVOID CClientOverlappedZK::CleanOverlapped()
{
    for( nuUINT i = 0; i < CONST_MAX_OVERLAPPED_COUNT; ++i )
    {
        if( nuNULL != m_ppOverlappedList[i] )
        {
            delete m_ppOverlappedList[i];
            m_ppOverlappedList[i]   = nuNULL;
        }
    }
    m_nNowCount = 0;
}
