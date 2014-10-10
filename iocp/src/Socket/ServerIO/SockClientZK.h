// SockClientZK.h: interface for the CSockClientZK class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCKCLIENTZK_H__B09FA5F8_BEA5_4CDC_8D64_C75FA5744EC8__INCLUDED_)
#define AFX_SOCKCLIENTZK_H__B09FA5F8_BEA5_4CDC_8D64_C75FA5744EC8__INCLUDED_

#include "SockBaseZK.h"
#include "OverlappedZK.h"
#include "ServerProtocolZK.h"

class CSockClientZK : public CSockBaseZK
{
public: 
    enum emClientConsts
    {
        CONST_NO_RESPONSE_INTERVAL  = 100*1000,
    };
public:
	CSockClientZK();
	virtual ~CSockClientZK();

    nubool  IoAccepted(COverlappedZK* pOverlapped);
    nubool  IoRecv(COverlappedZK* pOverlapped);
    nubool  IoRecved(COverlappedZK* pOverlapped, nuDWORD dwBytes);
    nubool  IoSend(COverlappedZK* pOverlapped);
    nubool  IoSend(nuUINT nID, nuUINT nCmd, const nuCHAR* pData, nuUINT nDataLen);
    nubool  IoSended(COverlappedZK* pOverlapped, nuDWORD dwBytes);
    nuUINT  IoClientLoging(nuUINT nID, const nuCHAR* pData, nuUINT nLen);
    nuUINT  IoClientQuit(nuBYTE nType);
    nuUINT  IoClientHeartbeat(nuUINT nCount);
    nuUINT  IoClientRecved(const CIServerCallBack::SOCK_DATA &sockData);
    nuUINT  IoClientReplyed(const CIServerCallBack::SOCK_DATA &sockData);
    virtual nubool  FreeOverlapped(COverlappedZK *pOverlapped);
private:
    nuVOID  CloseClient();//has connected
private:
    nuLONG              m_nIndex;
    nuUINT              m_nID;
    nuDWORD             m_nActiveTicks;
    CServerProtocolZK   m_protocol;
    enum emSocketStatus
    {
        SOCK_STATUS_CLOSE       = 0x0,
        SOCK_STATUS_CREATE      = 0x01,
        SOCK_STATUS_ACCEPTED    = 0x02,
        SOCK_STATUS_RECV        = 0x04,
        SOCK_STATUS_SEND        = 0x08,
    };
    nuBYTE              m_nSocketStatus;
    nuBYTE              m_nProcessThreaddCount;
    nuWORD              m_nSendCount;
public://inline functions
    nuLONG  GetIndex() { return m_nIndex; }
    nuUINT  GetID() { return m_nID; }
    nuVOID  CleanIndex() { m_nIndex = -1; m_nSocketStatus = SOCK_STATUS_CLOSE; m_nSendCount = 0; }
    nuVOID  SetIndex(nuLONG nIdx) { m_nIndex = nIdx; m_nID = 0; m_nSocketStatus = SOCK_STATUS_CREATE; m_overList.CleanOverlapped(); }
    nubool  IsInvalid() { return (-1 == m_nIndex); }
    nuVOID  SetTicks() { m_nActiveTicks = nuGetTickCount(); }
    nuUINT  GetActiveTicks() { return m_nActiveTicks; }
    nubool  IsAccpepted() { return (m_nSocketStatus&SOCK_STATUS_ACCEPTED) ? nuTrue : nuFalse; }
    nubool  IsAccepting() { return (SOCK_STATUS_CREATE == m_nSocketStatus); }
    nuBYTE  GetSocketStatus() { return m_nSocketStatus; }
    nubool  IsNoResponse(nuDWORD nNowTicks) ;//{ return ((nuDWORD)(nNowTicks-m_nActiveTicks) > CONST_NO_RESPONSE_INTERVAL);}
    nuVOID  SetThreadProcess() { ++m_nProcessThreaddCount; }
    nuVOID  RelThreadProcess() { --m_nProcessThreaddCount; }
};

#endif // !defined(AFX_SOCKCLIENTZK_H__B09FA5F8_BEA5_4CDC_8D64_C75FA5744EC8__INCLUDED_)
