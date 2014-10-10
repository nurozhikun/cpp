// OverlappedZK.h: interface for the COverlappedZK class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OVERLAPPEDZK_H__91B1A78D_075E_4F01_A3C1_38C8D5B4C888__INCLUDED_)
#define AFX_OVERLAPPEDZK_H__91B1A78D_075E_4F01_A3C1_38C8D5B4C888__INCLUDED_

#include "CrossSocketZK.h"
#include "CrossIOCompletionZK.h"
#include "inlineFunctionZK.h"
//*************************************
//Special class can't be inherited;
//*************************************
class COverlappedZK : public CCrossIOCompletionZK::PER_IO_OVERLAPPED 
{
public:
    enum emOverLappedType
    {
        OVERLAPPED_UNUSED   = 0,
        OVERLAPPED_ACCEPT,
        OVERLAPPED_RECV,
        OVERLAPPED_SEND,
        OVERLAPPED_EXIT,
    };
    enum emOverlappedWorkType
    {
        OL_WORK_TYPE_IDLE,
        OL_WORK_TYPE_IOQUEUE,
        OL_WORK_TYPE_THREAD,
    };
public:
	COverlappedZK(nuUINT Idx, CCrossSocketZK* pSocket, nuUINT nBuffLen, nuBYTE nOverType);
    ~COverlappedZK();//**Don't add virtual**
    //
    CCrossSocketZK::SOCKET_BUFFER& GetSockBuff() { return m_sockBuff; }
    nuCHAR* GetBuffer() { return m_sockBuff.sbBuffer.pBuffer; }
    nuDWORD GetBuffLen() { return m_sockBuff.sbBuffer.nBufferLen; }
    CCrossSocketZK* GetSocket() { return m_pSocket; }
    nuVOID  SetSocket(CCrossSocketZK* pSocket) { m_pSocket = pSocket; }
    nuBYTE  GetOverType() { return m_nOverType; }
    nuVOID  SetOverType(nuBYTE nOverType) { m_nOverType = nOverType; }
    nuUINT  GetIndex() { return m_nIndex; }
    nuVOID  DecIndex() { --m_nIndex; }
    nuVOID  SetIndex(nuUINT nIdx) { m_nIndex = nIdx; }
    nuVOID  SetWorkType(nuBYTE nType) { m_nWorkType = nType; }
private:
    CCrossSocketZK*                         m_pSocket;
    CCrossSocketZK::SOCKET_BUFFER           m_sockBuff;
    nuBYTE                                  m_nOverType;
    nuBYTE                                  m_nWorkType;
    //for memory manager
    nuUINT                                  m_nIndex;

};

#endif // !defined(AFX_OVERLAPPEDZK_H__91B1A78D_075E_4F01_A3C1_38C8D5B4C888__INCLUDED_)
