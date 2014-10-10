// CrossIOCompletionZK.h: interface for the CCrossIOCompletionZK class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CROSSIOCOMPLETIONZK_H__8019DA36_147A_4702_8AA2_40A0D625A526__INCLUDED_)
#define AFX_CROSSIOCOMPLETIONZK_H__8019DA36_147A_4702_8AA2_40A0D625A526__INCLUDED_

#include "typeDefine.h"

class CCrossIOCompletionZK  
{
public:
#ifdef WIN32_LEAN_AND_MEAN
    //system dependent
    typedef struct tagIO_SELF_OVERLAPPED
    {
        nuWORD      nStatus;
        nuWORD      nReserve;
        nuINT       nError;
        nuVOID*		pSelfObj;
	}IO_SELF_OVERLAPPED;
    typedef struct tagPER_IO_OVERLAPPED
    {
        OVERLAPPED          perOverlapped;
        IO_SELF_OVERLAPPED  perSelf;
    }PER_IO_OVERLAPPED;
#endif
public:
	CCrossIOCompletionZK();
	virtual ~CCrossIOCompletionZK();

    nubool  CreateIoPort(nuDWORD dwNumberOfThreads = 0);
    nubool  AssociateIoPort(nuHOBJECT hObject, nuDWORD nCompletionKey = 0);
    nuVOID  CloseIoPort();
    enum emIoQueueStatus
    {
        IO_QUEUE_STATUS_SUCESS,
        IO_QUEUE_STATUS_QUIT,
        IO_QUEUE_STATUS_TIMEOUT,
        IO_QUEUE_STATUS_FAILURE
    };
    nuINT   GetQueueIoStatus(nuDWORD* pdwBytes
        , nuDWORD* pdwKey
        , nuVOID** ppOverlapped
        , nuDWORD nMiniSeconds = nuINFINITE);
    nubool  PostQueueIoStatus(nuDWORD dwBytes, nuDWORD nKeys, nuVOID* lpOverlapped);

    nuDWORD GetNumberOfThreads()
    {
        return m_nNumberOfThreads;
    }
    //
public:
//     static nuVOID* CreateOverlapped();
//     static nuVOID  DeleteOverlapped(nuVOID* pOverlapped);
//     static IO_SELF_OVERLAPPED* GetOverlappedSelf(nuVOID* pOverlapped);
protected:
    nuHOBJECT               m_hIOCompletionPort;
    nuDWORD                 m_nNumberOfThreads;

};

#endif // !defined(AFX_CROSSIOCOMPLETIONZK_H__8019DA36_147A_4702_8AA2_40A0D625A526__INCLUDED_)
