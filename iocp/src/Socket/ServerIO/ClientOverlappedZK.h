// ClientOverlappedZK.h: interface for the CClientOverlappedZK class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENTOVERLAPPEDZK_H__19CDD1FC_6920_4688_A31C_D34C7690CB4B__INCLUDED_)
#define AFX_CLIENTOVERLAPPEDZK_H__19CDD1FC_6920_4688_A31C_D34C7690CB4B__INCLUDED_

#include "OverlappedZK.h"

class CClientOverlappedZK  
{
public:
    enum emOverlappedType
    {
        OLP_TYPE_UNUSED = 0,
        OLP_TYPE_ACCEPT,
        OLP_TYPE_RECV,
        OLP_TYPE_SEND,
    };
    enum emConstants
    {
        CONST_MAX_OVERLAPPED_COUNT  = 10,
        CONST_BUFFER_DEFAULT_LEN    = 1024,
    };
public:
	CClientOverlappedZK();
	virtual ~CClientOverlappedZK();
    COverlappedZK*  AllocOverlapped(CCrossSocketZK* pSock, nuBYTE nOverType = OLP_TYPE_ACCEPT, nuUINT nBuffLen = CONST_BUFFER_DEFAULT_LEN);
    nubool          FreeOverlapped(COverlappedZK* pOverlapped);
    nuVOID          CleanOverlapped();
    //inline functions
    nuUINT          GetNowCount() { return m_nNowCount; }
private:
    COverlappedZK*  m_ppOverlappedList[CONST_MAX_OVERLAPPED_COUNT];
    nuUINT          m_nNowCount;
    nubool          m_bLock;
	CRITICAL_SECTION m_bSection;
};

#endif // !defined(AFX_CLIENTOVERLAPPEDZK_H__19CDD1FC_6920_4688_A31C_D34C7690CB4B__INCLUDED_)
