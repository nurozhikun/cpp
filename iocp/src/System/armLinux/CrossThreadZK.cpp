// CrossThreadZK.cpp: implementation of the CCrossThreadZK class.
//
//////////////////////////////////////////////////////////////////////

#include "systemInc.h"
#include "CrossThreadZK.h"
#include "typeDefine.h"
#include "BaseLogZK.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//platform-dependent---------------------------------------------------------------
nuINT CCrossThreadZK::BeginThread(nuUINT nNodeCount /* = 0 */, 
                                  nuUINT nThdPriority /* = 0 */, 
                                  nuUINT nCreateFlags /* = 0 */, 
                                  nuUINT nStackSize /* = 1024*1024 */)
{
    nuINT nu_Ret = 0;
	if( m_handle )
    {
        return 1;
    }
    if( 0 != nNodeCount && !CreateNodeList(nNodeCount) )
    {
         return nuFalse;
    }
    nu_Ret = pthread_create(&m_nThdId
                            , NULL
                            , (void* (__stdcall *)(void *))ThreadProc
                            , this
                            );//if-success return 0    if-error return err-code
    if( nu_Ret == 0) 
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

nuVOID CCrossThreadZK::EndThread()
{
    struct timespec timeOut,remains; 
    timeOut.tv_sec = 0; 
    timeOut.tv_nsec = 25000; /* 25 milliseconds --> need edit for sys-clock*/ 

	if( m_nThdId && (pthread_self() != m_nThdId) )
	{
		if( m_bThdIsWorking )
		{
			m_bThdIsWorking = nuFalse;
			while( !m_bThdIsEnd )
			{
                nanosleep(&timeOut, &remains);
			}
		}
		m_handle = nuNULL;
		m_nThdId = 0;
		if( m_pStackAddr )
		{
			delete[] m_pStackAddr;
            m_pStackAddr = nuNULL;
		}
	}
}

nubool CCrossThreadZK::IsRunning()
{
    if( m_bThdIsWorking && nuGetCurrentThreadId() == m_nThdId )
    {
        return nuTrue;
    }
    return nuFalse;
}

//platform-independent---------------------------------------------------
CCrossThreadZK::CCrossThreadZK()
{
    m_handle		= nuNULL;
    m_pStackAddr	= nuNULL;
    m_nThdId		= 0;
    m_bThdIsWorking	= nuTrue;
    m_bThdIsEnd		= nuTrue;
    m_bActionDoing  = nuFalse;
    m_pCodeList     = nuNULL;
}
CCrossThreadZK::~CCrossThreadZK()
{
    DeleteNodeList();
}

nubool CCrossThreadZK::SendMessage(CBaseCodeListZK::BASE_LIST_NODE& node)
{
    if(nuGetCurrentThreadId() != m_nThdId)
    {
        MessageProc(node);
    }
    else
    {
        CBaseLock lock(&m_bActionDoing);
        MessageProc(node);
    }
    return nuTrue;
}

nubool CCrossThreadZK::PushMessage(const CBaseCodeListZK::BASE_LIST_NODE& node)
{
    if( nuNULL == m_pCodeList )
    {
        return nuFalse;
    }
    return m_pCodeList->PushNode(node);
}

nubool CCrossThreadZK::PopMessage(CBaseCodeListZK::BASE_LIST_NODE& node)
{
    if( nuNULL == m_pCodeList )
    {
        return nuFalse;
    }
    return m_pCodeList->PopNode(node);
}

nubool CCrossThreadZK::CreateNodeList(nuUINT nCount)
{
    m_pCodeList = new CBaseCodeListZK();
    if( nuNULL == m_pCodeList )
    {
        return nuFalse;
    }
    return m_pCodeList->CreateCodeList(nCount);
}

nuVOID CCrossThreadZK::DeleteNodeList()
{
    if( nuNULL != m_pCodeList )
    {
        m_pCodeList->DeleteCodeList();
        delete m_pCodeList;
        m_pCodeList = nuNULL;
    }
}
nuDWORD nuSTDCALL CCrossThreadZK::ThreadProc(CCrossThreadZK* pThdObject)
{
    if( nuNULL == pThdObject )
    {
        return 0;
    }
    pThdObject->m_bThdIsEnd = nuFalse;
    pThdObject->ThreadBegin();
    while(pThdObject->m_bThdIsWorking)
    {
        pThdObject->m_bActionDoing = nuTrue;
        nuDWORD nRet = pThdObject->ThreadAction();
        pThdObject->m_bActionDoing = nuFalse;
        if( pThdObject->m_bThdIsWorking )
        {
            if( THREAD_EXIT == nRet )
            {
                pThdObject->m_bThdIsWorking = nuFalse;
            }
            else if( THREAD_IDLE == nRet )
            {
                nuSleep(50);
            }
            else if( THREAD_SLEEP == nRet )
            {
                nuSleep(100);
            }
            else
            {
                nuSleep(25);
            }
        }
    }
    pThdObject->ThreadEnd();
    pThdObject->m_bThdIsEnd = nuTrue;
    printf("exit from while(pThdObject->m_bThdIsWorking)");
    return 1;
}
