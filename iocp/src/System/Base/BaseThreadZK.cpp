// BaseThreadZK.cpp: implementation of the CBaseThreadZK class.
//
//////////////////////////////////////////////////////////////////////

#include "BaseThreadZK.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseThreadZK::CBaseThreadZK()
{
    m_bThdIsWorking    = nuFalse;
    m_bThdIsEnd        = nuTrue;
    m_bActionDoing     = nuFalse;
    m_pCodeList        = nuNULL;
}

CBaseThreadZK::~CBaseThreadZK()
{
    EndThread();
}

nuUINT CBaseThreadZK::BeginThread(nuUINT nNodeCount /* = 0 */, nuUINT nThdPriority /* = 0 */, nuUINT nCreateFlags /* = 0 */, nuUINT nStackSize /* = 1024*1024 */)
{
    if( 0 != nNodeCount && !CreateNodeList(nNodeCount) )
    {
        return 0;
    }
    m_bThdIsWorking = nuTrue;
    m_bThdIsEnd     = nuFalse;
    if( 0 == CreateThread(nThdPriority, nCreateFlags, nStackSize) )
    {
        m_bThdIsWorking = nuFalse;
        m_bThdIsEnd     = nuTrue;
        return 0;
    }
    return 1;
}

nuVOID CBaseThreadZK::EndThread()
{
    if( m_bThdIsWorking )
    {
        m_bThdIsWorking = nuFalse;
        if( m_nThdId && (nuGetCurrentThreadId() != m_nThdId) )
        {
            while( !m_bThdIsEnd )
            {
                nuSleep(25);
            }
        }
    }
    DeleteThread();
    DeleteNodeList();
}

nuDWORD CBaseThreadZK::ThreadLoop()
{
    m_bThdIsEnd = nuFalse;
    ThreadBegin();
    while(m_bThdIsWorking)
    {
        m_bActionDoing = nuTrue;
        nuDWORD nRet = ThreadAction();
        m_bActionDoing = nuFalse;
        //Sleep for switch
        if( m_bThdIsWorking )
        {
            if( THREAD_EXIT == nRet )
            {
                m_bThdIsWorking = nuFalse;
                break;
            }
            else if( THREAD_IDLE == nRet )
            {
                nuSleep(50);
            }
            else if( THREAD_SLEEP1 <= nRet )
            {
                nuSleep(100*(nRet - THREAD_SLEEP1 + 1));
            }
            else
            {
                nuSleep(25);
            }
        }
    }
    ThreadEnd();
    nuUINT nID = m_nThdId;
    m_bThdIsEnd = nuTrue;
    return nID;
}
nubool CBaseThreadZK::SendMessage(CBaseCodeListZK::BASE_LIST_NODE& node)
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

nubool CBaseThreadZK::PushMessage(const CBaseCodeListZK::BASE_LIST_NODE& node)
{
    if( nuNULL == m_pCodeList )
    {
        return nuFalse;
    }
    return m_pCodeList->PushNode(node);
}

nubool CBaseThreadZK::PopMessage(CBaseCodeListZK::BASE_LIST_NODE& node)
{
    if( nuNULL == m_pCodeList )
    {
        return nuFalse;
    }
    return m_pCodeList->PopNode(node);
}

nubool CBaseThreadZK::CreateNodeList(nuUINT nCount)
{
    m_pCodeList = new CBaseCodeListZK();
    if( nuNULL == m_pCodeList )
    {
        return nuFalse;
    }
    return m_pCodeList->CreateCodeList(nCount);
}

nuVOID CBaseThreadZK::DeleteNodeList()
{
    if( nuNULL != m_pCodeList )
    {
        m_pCodeList->DeleteCodeList();
        delete m_pCodeList;
        m_pCodeList = nuNULL;
    }
}
