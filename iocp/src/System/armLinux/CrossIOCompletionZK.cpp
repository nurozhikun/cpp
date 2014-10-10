// CrossIOCompletionZK.cpp: implementation of the CCrossIOCompletionZK class.
//
//////////////////////////////////////////////////////////////////////

#include "CrossIOCompletionZK.h"
#include "systemInc.h"
#include "inlineFunctionZK.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCrossIOCompletionZK::CCrossIOCompletionZK()
{
    m_hIOCompletionPort     = nuNULL;
}

CCrossIOCompletionZK::~CCrossIOCompletionZK()
{

}


nubool CCrossIOCompletionZK::CreateIoPort(nuDWORD dwNumberOfThreads /* = 0 */)
{

/*//del for linux by panjun 20111226
    CloseIoPort();
    if( 0 == dwNumberOfThreads )
    {
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        dwNumberOfThreads = sysInfo.dwNumberOfProcessors;
    }
    m_nNumberOfThreads = dwNumberOfThreads;
    m_hIOCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE
                                , NULL
                                , 0
                                , dwNumberOfThreads );
    if( NULL == m_hIOCompletionPort )
    {
        return nuFalse;
    }
    return nuTrue;
*/
    return nuTrue;
}

nubool CCrossIOCompletionZK::AssociateIoPort(nuHOBJECT hObject, nuDWORD nCompletionKey /* = 0 */)
{
/*//del for linux by panjun 20111226
    if( nuNULL == m_hIOCompletionPort )
    {
        return nuFalse;
    }
    if( CreateIoCompletionPort((HANDLE)hObject, (HANDLE)m_hIOCompletionPort, nCompletionKey, 0) )
    {
        return nuTrue;
    }
    else
    {
        return nuFalse;
    }
*/
    return nuTrue;
}

nuVOID CCrossIOCompletionZK::CloseIoPort()
{
/*//del for linux by panjun 20111226
    if( nuNULL != m_hIOCompletionPort )
    {
        CloseHandle((HANDLE)m_hIOCompletionPort);
        m_hIOCompletionPort = nuNULL;
    }
*/
}

nuINT CCrossIOCompletionZK::GetQueueIoStatus(nuDWORD* pdwBytes , 
                                             nuDWORD* pdwKey , 
                                             nuVOID** ppOverlapped , 
                                             nuDWORD nMiniSeconds /* = nuINFINITE */)
{
/*//del for linux by panjun 20111226
    if( nuNULL == m_hIOCompletionPort )
    {
        return IO_QUEUE_STATUS_QUIT;
    }
    if( GetQueuedCompletionStatus((HANDLE)m_hIOCompletionPort
        , pdwBytes
        , pdwKey
        , (LPOVERLAPPED*)ppOverlapped
        , nMiniSeconds ) )
    {
        return IO_QUEUE_STATUS_SUCESS;
    }
    else
    {
        if( NULL == *ppOverlapped )
        {
            return IO_QUEUE_STATUS_TIMEOUT;
        }
        else
        {
            return IO_QUEUE_STATUS_FAILURE;
        }
    }
*/
    return IO_QUEUE_STATUS_FAILURE;
}

nubool CCrossIOCompletionZK::PostQueueIoStatus(nuDWORD dwBytes, nuDWORD nKeys, nuVOID* lpOverlapped)
{
/*//del for linux by panjun 20111226
    if( nuNULL == m_hIOCompletionPort || nuNULL == lpOverlapped )
    {
        return nuFalse;
    }
    if( PostQueuedCompletionStatus((HANDLE)m_hIOCompletionPort
            , dwBytes
            , nKeys
            , (LPOVERLAPPED)lpOverlapped) )
    {
        return nuTrue;
    }
    else
    {
        return nuFalse;
    }
*/
    return nuFalse;
}

// nuVOID* CCrossIOCompletionZK::CreateOverlapped()
// {
//     int *p = new int[(sizeof(OVERLAPPED)+sizeof(IO_SELF_OVERLAPPED)+3)/sizeof(int)];
//     if( p )
//     {
//         memset(p, 0, sizeof(OVERLAPPED)+sizeof(IO_SELF_OVERLAPPED));
//     }
//     return p;
// }
// 
// nuVOID	CCrossIOCompletionZK::DeleteOverlapped(nuVOID* pOverlapped)
// {
//     if( pOverlapped )
//     {
//         delete[] pOverlapped;
//     }
// }
// 
// CCrossIOCompletionZK::IO_SELF_OVERLAPPED* CCrossIOCompletionZK::GetOverlappedSelf(nuVOID* pOverlapped)
// {
//     if( pOverlapped )
//     {
//         return (IO_SELF_OVERLAPPED*)(((char*)pOverlapped) + sizeof(OVERLAPPED));
//     }
//     return NULL;
// }