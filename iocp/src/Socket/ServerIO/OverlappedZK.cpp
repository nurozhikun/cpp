// OverlappedZK.cpp: implementation of the COverlappedZK class.
//
//////////////////////////////////////////////////////////////////////

#include "OverlappedZK.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
COverlappedZK::COverlappedZK(nuUINT Idx, CCrossSocketZK* pSocket, nuUINT nBuffLen, nuBYTE nOverType)
{
    m_nIndex                = Idx;
    m_pSocket               = pSocket;
    m_nOverType             = nOverType;
    m_sockBuff.sbBytes      = 0;
    m_sockBuff.sbFlags      = 0;
    m_sockBuff.sbBuffer.nBufferLen  = nBuffLen;
    nuMemset(&perOverlapped, 0, sizeof(perOverlapped));
    nuMemset(&perSelf, 0, sizeof(perSelf));
//    pObject                 = nuNULL;
    if( nBuffLen )
    {
        m_sockBuff.sbBuffer.pBuffer = new nuCHAR[nBuffLen+1];
    }
    else
    {
        m_sockBuff.sbBuffer.pBuffer = nuNULL;
    }
    if( m_sockBuff.sbBuffer.pBuffer )
    {
        nuMemset(m_sockBuff.sbBuffer.pBuffer, 0, nBuffLen+1);
    }
    m_nWorkType = OL_WORK_TYPE_IDLE;
}

COverlappedZK::~COverlappedZK()
{
    if( m_sockBuff.sbBuffer.pBuffer )
    {
        delete[] m_sockBuff.sbBuffer.pBuffer;
    }
}
