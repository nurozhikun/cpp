// CrossThreadZK.h: interface for the CCrossThreadZK class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CROSSTHREADZK_H__CA8CC5F5_C1FB_4C8B_A775_F63C7BCFC8BC__INCLUDED_)
#define AFX_CROSSTHREADZK_H__CA8CC5F5_C1FB_4C8B_A775_F63C7BCFC8BC__INCLUDED_

#include "inlineFunctionZK.h"
#include "BaseCodeListZK.h"
//#include "BaseLogZK.h"

class CCrossThreadZK  
{
public:
    //platform-dependent
    nuINT  BeginThread(nuUINT nNodeCount = 0, nuUINT nThdPriority = 0, nuUINT nCreateFlags = 0, nuUINT nStackSize = 1024*1024);
    nuVOID EndThread();//must call by other threads;
    nuVOID EndWaiting();
    //platform-independent
    inline nubool IsRunning();
    inline CCrossThreadZK();
    inline virtual ~CCrossThreadZK();
    inline nuDWORD GetThreadId()
    {
        return m_nThdId;
    }
    
    inline virtual nubool SendMessage(CBaseCodeListZK::BASE_LIST_NODE& node);
    inline virtual nubool PushMessage(const CBaseCodeListZK::BASE_LIST_NODE& node);
    inline virtual nubool PopMessage(CBaseCodeListZK::BASE_LIST_NODE& node);
    
protected:
    inline nubool CreateNodeList(nuUINT nCount);
    inline nuVOID DeleteNodeList();
    enum emThreadReturn
    {
        THREAD_IDLE,
        THREAD_WORK,
        THREAD_EXIT,
        THREAD_SLEEP,
        THREAD_KEPTWORK,
    };
    virtual nuDWORD ThreadBegin() { return THREAD_WORK; }
    virtual nuDWORD ThreadEnd() { return THREAD_WORK; }
    virtual nuDWORD ThreadAction() { return THREAD_EXIT; }
    virtual nuDWORD MessageProc(CBaseCodeListZK::BASE_LIST_NODE& node) { return THREAD_IDLE; }
private:
    static    nuDWORD nuSTDCALL ThreadProc(CCrossThreadZK* pThdObject);
protected:
    //cross-data
    nuHOBJECT           m_handle;
    nuVOID*             m_pStackAddr;
    nuDWORD             m_nThdId;
    //////////////////////////////////////////
    nubool              m_bThdIsWorking;
    nubool              m_bActionDoing;
    CBaseCodeListZK*    m_pCodeList;
private:
    nubool              m_bThdIsEnd;

};
//platform-independent---------------------------------------------------
inline nubool CCrossThreadZK::IsRunning()
{
    if( m_bThdIsWorking && nuGetCurrentThreadId() == m_nThdId )
    {
        return nuTrue;
    }
    return nuFalse;
}

inline CCrossThreadZK::CCrossThreadZK()
{
    m_handle           = nuNULL;
    m_pStackAddr       = nuNULL;
    m_nThdId           = 0;
    m_bThdIsWorking    = nuTrue;
    m_bThdIsEnd        = nuTrue;
    m_bActionDoing     = nuFalse;
    m_pCodeList        = nuNULL;
}
inline CCrossThreadZK::~CCrossThreadZK()
{
    DeleteNodeList();
}

inline nubool CCrossThreadZK::SendMessage(CBaseCodeListZK::BASE_LIST_NODE& node)
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

inline nubool CCrossThreadZK::PushMessage(const CBaseCodeListZK::BASE_LIST_NODE& node)
{
    if( nuNULL == m_pCodeList )
    {
        return nuFalse;
    }
    return m_pCodeList->PushNode(node);
}

inline nubool CCrossThreadZK::PopMessage(CBaseCodeListZK::BASE_LIST_NODE& node)
{
    if( nuNULL == m_pCodeList )
    {
        return nuFalse;
    }
    return m_pCodeList->PopNode(node);
}

inline nubool CCrossThreadZK::CreateNodeList(nuUINT nCount)
{
    m_pCodeList = new CBaseCodeListZK();
    if( nuNULL == m_pCodeList )
    {
        return nuFalse;
    }
    return m_pCodeList->CreateCodeList(nCount);
}

inline nuVOID CCrossThreadZK::DeleteNodeList()
{
    if( nuNULL != m_pCodeList )
    {
        m_pCodeList->DeleteCodeList();
        delete m_pCodeList;
        m_pCodeList = nuNULL;
    }
}
inline nuDWORD nuSTDCALL CCrossThreadZK::ThreadProc(CCrossThreadZK* pThdObject)
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
            else if( THREAD_WORK == nRet )
            {
				nuSleep(10);
            }
            else if( THREAD_IDLE == nRet )
            {
				nuSleep(50);
            }
            else if( THREAD_SLEEP == nRet )
            {
                nuSleep(100);
            }
			else if (THREAD_KEPTWORK == nRet)
			{
			
			}
        }
    }
    pThdObject->ThreadEnd();
    pThdObject->m_bThdIsEnd = nuTrue;
    pThdObject->EndWaiting();
    return 1;
}

#endif // !defined(AFX_CROSSTHREADZK_H__CA8CC5F5_C1FB_4C8B_A775_F63C7BCFC8BC__INCLUDED_)
