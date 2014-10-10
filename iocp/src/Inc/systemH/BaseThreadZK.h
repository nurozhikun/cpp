// BaseThreadZK.h: interface for the CBaseThreadZK class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASETHREADZK_H__08EB7479_F0FA_4728_B918_6DA7B554C0EC__INCLUDED_)
#define AFX_BASETHREADZK_H__08EB7479_F0FA_4728_B918_6DA7B554C0EC__INCLUDED_

#include "CrossTaskZK.h"

class CBaseThreadZK : public CCrossTaskZK 
{
public:
	CBaseThreadZK();
	virtual ~CBaseThreadZK();
    inline nubool IsRunning()
    {
        if( m_bThdIsWorking && nuGetCurrentThreadId() == m_nThdId )
        {
            return nuTrue;
        }
        return nuFalse;
    }
    inline nuDWORD GetThreadId()
    {
        return m_nThdId;
    }
    
    virtual nubool SendMessage(CBaseCodeListZK::BASE_LIST_NODE& node);
    virtual nubool PushMessage(const CBaseCodeListZK::BASE_LIST_NODE& node);
    virtual nubool PopMessage(CBaseCodeListZK::BASE_LIST_NODE& node);
    
protected:
    nuUINT BeginThread(nuUINT nNodeCount = 0, nuUINT nThdPriority = 0, nuUINT nCreateFlags = 0, nuUINT nStackSize = 1024*1024);
    nuVOID EndThread();//must call by other threads;
    nubool CreateNodeList(nuUINT nCount);
    nuVOID DeleteNodeList();
    enum emThreadReturn
    {
        THREAD_IDLE,
        THREAD_WORK,
        THREAD_EXIT,
        THREAD_SLEEP1,
        THREAD_SLEEP2,
        THrEAD_SLEEP3,
    };
    virtual nuDWORD ThreadBegin() { return THREAD_WORK; }
    virtual nuDWORD ThreadEnd() { return THREAD_WORK; }
    virtual nuDWORD ThreadAction() { return THREAD_EXIT; }
    virtual nuDWORD MessageProc(CBaseCodeListZK::BASE_LIST_NODE& node) { return THREAD_IDLE; }
private:
    virtual nuDWORD  ThreadLoop();
protected:
    nubool              m_bThdIsWorking;
    nubool              m_bActionDoing;
    CBaseCodeListZK*    m_pCodeList;
private:
    nubool              m_bThdIsEnd;
};

#endif // !defined(AFX_BASETHREADZK_H__08EB7479_F0FA_4728_B918_6DA7B554C0EC__INCLUDED_)
