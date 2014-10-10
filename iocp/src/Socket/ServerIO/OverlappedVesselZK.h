// OverlappedVesselZK.h: interface for the COverlappedVesselZK class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OVERLAPPEDVESSELZK_H__AA8BA66D_384C_4C58_A210_89DDD2B53B1A__INCLUDED_)
#define AFX_OVERLAPPEDVESSELZK_H__AA8BA66D_384C_4C58_A210_89DDD2B53B1A__INCLUDED_

#include "OverlappedZK.h"

class COverlappedVesselZK  
{
public:
    enum emPageSize
    {
        PAGE_SIZE   = 1024
    };
    typedef struct tagOVERLAPPEDS_NODE
    {
        COverlappedZK*          pPage[PAGE_SIZE];
        tagOVERLAPPEDS_NODE*    pNextPage;
    }OVERLAPPEDS_NODE;
public:
	COverlappedVesselZK();
	virtual ~COverlappedVesselZK();

    COverlappedZK*  GetOverlapped(CCrossSocketZK* pSock, nuUINT nBuffLen, nuBYTE nOverType);
    nuVOID          RelOverlapped(COverlappedZK* pOverlapped);

    nuUINT          DeleteClientOverlapped(CCrossSocketZK* pClient);

private:
    nubool  PushOverlapped(COverlappedZK* pOverlapped);
    nubool  PopOverlapped(COverlappedZK* pOverlapped);
private:
    OVERLAPPEDS_NODE*           m_pHeader;
    nuDWORD                     m_nNowCount;
    nuDWORD                     m_nPageCount;
   // nubool                      m_bLock;
	CRITICAL_SECTION m_bSection;
};

#endif // !defined(AFX_OVERLAPPEDVESSELZK_H__AA8BA66D_384C_4C58_A210_89DDD2B53B1A__INCLUDED_)
