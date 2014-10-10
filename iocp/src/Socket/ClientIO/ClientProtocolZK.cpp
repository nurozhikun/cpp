// ClientProtocolZK.cpp: implementation of the CClientProtocolZK class.
//
//////////////////////////////////////////////////////////////////////

#include "ClientProtocolZK.h"
#include "ClientSocketZK.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClientProtocolZK::CClientProtocolZK(class CClientSocketZK* pClient)
: m_pClient(pClient)
{

}

CClientProtocolZK::~CClientProtocolZK()
{

}

nuVOID CClientProtocolZK::ProcPackage(CMD_DATA& cmdData)
{
    if( m_pClient )
    {
        CIClientCallBack::RECV_DATA recvData;
        recvData.sdCmd      = cmdData.cdCommand;
        recvData.sdID       = cmdData.cdBus.pbID;
        recvData.sdDataBuff = cmdData.cdData.dsDataBuffer;
        recvData.sdNowSize  = cmdData.cdData.dsNowSize;
        recvData.sdNowAddr  = cmdData.cdData.dsAddress;
        recvData.sdWholeSize= cmdData.cdData.dsWholeSize;
        if( CMD_REPLY_BASE <= recvData.sdCmd )
        {
            recvData.sdCmd &=(~CMD_REPLY_BASE);
            m_pClient->ServerReply(recvData);
        }
        else
        {
            m_pClient->ServerFrom(recvData);
        }

    }
}


