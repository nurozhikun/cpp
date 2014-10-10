// ServerProtocolZK.cpp: implementation of the CServerProtocolZK class.
//
//////////////////////////////////////////////////////////////////////

#include "ServerProtocolZK.h"
#include "SockClientZK.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CServerProtocolZK::CServerProtocolZK(class CSockClientZK* pClient)
{
    m_pClient   = pClient;
}

CServerProtocolZK::~CServerProtocolZK()
{

}

nuVOID CServerProtocolZK::ProcPackage(CMD_DATA& cmdData)
{
    nuUINT nRet = CIServerCallBack::CB_RET_NOPROCESS;
    if( CMD_REPLY_BASE <= cmdData.cdCommand )
    {
        CIServerCallBack::SOCK_DATA sockData;
        sockData.sdCmd          = cmdData.cdCommand;
        sockData.sdID           = cmdData.cdBus.pbID;
        sockData.sdWholeSize    = cmdData.cdData.dsWholeSize;
        sockData.sdNowSize      = cmdData.cdData.dsNowSize;
        sockData.sdNowAddr      = cmdData.cdData.dsAddress;
        sockData.sdDataBuff     = cmdData.cdData.dsDataBuffer;
        nRet = m_pClient->IoClientReplyed(sockData);
    }//received the reply from a client
    else
    {
        if( CMD_CLIENT_LOGIN == cmdData.cdCommand )
        {
            nRet = m_pClient->IoClientLoging(cmdData.cdBus.pbID, cmdData.cdData.dsDataBuffer, cmdData.cdData.dsNowSize);
        }
        else if( CMD_CLIENT_HEARTBEAT == cmdData.cdCommand )
        {
            nRet = m_pClient->IoClientHeartbeat(*((nuUINT*)cmdData.cdData.dsDataBuffer));
        }
        else
        {
            CIServerCallBack::SOCK_DATA sockData;
            sockData.sdCmd          = cmdData.cdCommand;
            sockData.sdID           = cmdData.cdBus.pbID;
            sockData.sdWholeSize    = cmdData.cdData.dsWholeSize;
            sockData.sdNowSize      = cmdData.cdData.dsNowSize;
            sockData.sdNowAddr      = cmdData.cdData.dsAddress;
            sockData.sdDataBuff     = cmdData.cdData.dsDataBuffer;
            nRet = m_pClient->IoClientRecved(sockData);
        }
        if( CIServerCallBack::CB_RET_NOREPLYED == nRet && cmdData.cdCommand < CMD_REPLY_BASE )
        {
            m_pClient->IoSend(cmdData.cdBus.pbID, cmdData.cdCommand|CMD_REPLY_BASE, nuNULL, 0);
        }
    }//received the message from a client
}
