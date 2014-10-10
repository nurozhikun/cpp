// CrossTaskZK.h: interface for the CCrossTaskZK class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CROSSTASKZK_H__222A3BBA_F9F8_4D7D_A39A_FB1F4286CCAE__INCLUDED_)
#define AFX_CROSSTASKZK_H__222A3BBA_F9F8_4D7D_A39A_FB1F4286CCAE__INCLUDED_

#include "inlineFunctionZK.h"
#include "BaseCodeListZK.h"

class CCrossTaskZK  
{
public:
	CCrossTaskZK()
    {
        m_handle        = nuNULL;
        m_pStackAddr    = nuNULL;
        m_nThdId        = 0;
    }
	virtual ~CCrossTaskZK()
    {
    }
protected:
    //platform-dependent
    nuUINT CreateThread(nuUINT nThdPriority = 0, nuUINT nCreateFlags = 0, nuUINT nStackSize = 1024*1024);
    nuVOID DeleteThread();//must call by other threads;
    nuVOID EndWaiting();
    virtual nuDWORD  ThreadLoop() = 0;
    static  nuDWORD nuSTDCALL ThreadProc(CCrossTaskZK* pThdObject)
    {
        if( nuNULL == pThdObject )
        {
            return 0;
        }
        nuDWORD nRes = pThdObject->ThreadLoop();
        pThdObject->EndWaiting();
        return nRes;
    }
protected:
    //cross-data
    nuHOBJECT           m_handle;
    nuVOID*             m_pStackAddr;
    nuDWORD             m_nThdId;
};

#endif // !defined(AFX_CROSSTASKZK_H__222A3BBA_F9F8_4D7D_A39A_FB1F4286CCAE__INCLUDED_)
