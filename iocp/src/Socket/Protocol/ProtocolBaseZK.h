// ProtocolBaseZK.h: interface for the CProtocolBaseZK class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROTOCOLBASEZK_H__5F2B7D4B_788E_46CC_87AF_01412B32C31C__INCLUDED_)
#define AFX_PROTOCOLBASEZK_H__5F2B7D4B_788E_46CC_87AF_01412B32C31C__INCLUDED_

#include "socketProtocol.h"
#include "inlineFunctionZK.h"

//#define _USE_OLD_PACKAGE_SIZE

class CProtocolBaseZK  
{
public:
//     enum emCmdRange
//     {
//         CMD_CAR_DEFAULT_F           = 0x0,
//         CMD_CAR_DEFAULT_L           = 0xF,
//         CMD_CAR_LOGIN_F             = 0x10,
//         CMD_CAR_LOGIN_L             = 0xFF,
//         CMD_CAR_RECORD_F            = 0x100,
//         CMD_CAR_RECORD_L            = 0x1FF,
//         CMD_CAR_HEART_F             = 0x200,
//         CMD_CAR_HEART_L             = 0x2FF,
//         CMD_CAR_STATE_F             = 0x300,
//         CMD_CAR_STATE_L             = 0x4FF,
//         CMD_CAR_REQUEST_F           = 0x1000,
//         CMD_CAR_REQUEST_L           = 0x1FFF,
//         CMD_SERVER_DISPATCH_F       = 0x80000,
//         CMD_SERVER_DISPATCH_L       = 0x8FFFF,
//         CMD_FILE_RECORD_F           = 0x10000000,
//         CMD_FILE_RECORD_L           = 0x1FFFFFFF,
//         CMD_LINE_CAR_AND_SERVER     = 0x80000000
//     };
    enum emCmdType
    {
        CMD_DEFAULT                 = 0,
        CMD_FROM_TERMINAL_RECORD,
        CMD_REPLY_TERMINAL_RECORD,
        CMD_FROM_TERMINAL_HEART,
        CMD_REPLY_TERMINAL_HEART,
        CMD_FROM_TERMINAL_STATE,
        CMD_REPLY_TERMINAL_STATE,
        CMD_FROM_TERMINAL_REQUEST,
        CMD_REPLY_TERMINAL_REQUEST,
        CMD_FROM_SERVER_DISPATCH,
        CMD_REPLY_SERVER_DISPATCH,
        CMD_FILE_RECORD,
    };
    enum emCmdInfo
    {
        CMD_INFO_MAX_SIZE       = (16*1024),
        CMD_INFO_MIN_SIZE       = 40,
        CMD_INFO_HEAD_SIZE      = 28,//(HEAD+COMMAND(4))
        CMD_INFO_HEAD_MARK_SIZE = 8,
        CMD_INFO_HEAD_PACK_SIZE = 4,
        CMD_INFO_HEAD_DATA_SIZE = 4,
        CMD_INFO_HEAD_BUS_ID    = 8,
        CMD_INFO_HEAD_CMD_SIZE  = 4,
        CMD_INFO_END_DATA_ADDR  = 4,
        CMD_INFO_END_MARK_SIZE  = 8,
        CMD_INFO_PACKAGE_COST   = 20,
        CMD_INFO_HEAD_MARK      = 0xFE,
        CMD_INFO_END_MARK       = 0xEF,
    };
    enum emPackageType
    {
        PACKAGE_DEFAULT         = 0,
        PACKAGE_WHOLE           = 1,
        PACKAGE_PART_HEAD       = 2,
        PACKAGE_PART_DATA       = 3,
        PACKAGE_PART_END        = 4,
    };
    enum emManualSplit
    {
        MANUAL_SPLIT_NO,
        MANUAL_SPLIT_YES,
    };
    enum emCmdResult
    {
        CMD_ACTION_INVALID,
        CMD_ACTION_VALID,
        CMD_ACTION_LOST,
    };
    typedef struct tagCMD_DATA
    {
        nuWORD      cdCmdAction;
        nuBYTE      cdManualSplit;
        nuDWORD     cdCommand;
        struct tagDATA_BODY
        {
            nuDWORD     dsWholeSize;//the size of the whole data
            nuDWORD     dsNowSize;//the accumulative size of received data
            nuDWORD     dsAddress;
            nuCHAR*     dsDataBuffer;
        }           cdData;
        //
        struct tagPACK_BUS
        {
            nuDWORD     pbID;
            nuDWORD     pbReserver;
        }           cdBus;
    }CMD_DATA;
public:
	CProtocolBaseZK();
	virtual ~CProtocolBaseZK();
    virtual nuUINT ProcBinaryData(nuCHAR* pData, nuUINT nDataLen);
    static nuUINT  PackageData(nuCHAR* pBuffer, nuUINT nLen, const nuCHAR* pData, nuUINT nDataLen, nuUINT nID, nuUINT nCmd, nuUINT nWholeSize = 0, nuUINT nAddr = 0);
protected:
    virtual nuVOID ProcPackage(CMD_DATA& cmdData) = 0;
private:
    nuUINT  BackupData(nuCHAR* pData, nuUINT nLen);
    nuVOID  ReleaseBackup();
    nuUINT  AnalyseOnePackage(nuCHAR* pData, nuUINT nDataLen, CMD_DATA& cmdData);
    inline nuUINT   GetPackageBegin(nuCHAR* pData, nuUINT nDataLen);
    inline nuUINT   GetPackageEnd(nuCHAR* pData, nuUINT nDataLen);
    inline nubool   IsPackageEnd(nuCHAR* pData, nuUINT nDataLen);
    nuVOID  DebugOut(nuCHAR* pBuffer, nuUINT nLen);
private:
    nubool  m_bLock;
	CRITICAL_SECTION m_bSection;
    nuCHAR* m_pBackupBuff;
    nuDWORD m_nBuffLen;
    const static nuBYTE cs_header[CMD_INFO_HEAD_MARK_SIZE];
    const static nuBYTE cs_end[CMD_INFO_END_MARK_SIZE];

};

#endif // !defined(AFX_PROTOCOLBASEZK_H__5F2B7D4B_788E_46CC_87AF_01412B32C31C__INCLUDED_)
