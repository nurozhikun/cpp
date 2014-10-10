// ProtocolBaseZK.cpp: implementation of the CProtocolBaseZK class.
//
//////////////////////////////////////////////////////////////////////

#include "ProtocolBaseZK.h"
#include "inlineFunctionZK.h"
#include "BaseLogZK.h"
#include "CCriticalLock.h"
//#define _USE_OLD_PACKAGE_SIZE
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const nuBYTE CProtocolBaseZK::cs_header[] = {0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE};
const nuBYTE CProtocolBaseZK::cs_end[] = {0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF};

#define  _USE_OLD_PACKAGE_SIZE
#define _DEBUG_OUT_PACAGE(pBuffer, nSize)   DebugOut(pBuffer, nSize)

inline nuUINT CProtocolBaseZK::GetPackageBegin(nuCHAR* pData, nuUINT nDataLen)
{
    for(nuUINT i = 0; i <= nDataLen - CMD_INFO_HEAD_MARK_SIZE; ++i )
    {
        nubool bFind = nuTrue;
        for( nuUINT j = 0; j < CMD_INFO_HEAD_MARK_SIZE; ++j )
        {
            if( (nuCHAR)CMD_INFO_HEAD_MARK != pData[i+j] )
            {
                bFind = nuFalse;
                break;
            }
        }
        if( bFind )
        {
            return i;
        }
    }
    return nDataLen;
}

inline nuUINT CProtocolBaseZK::GetPackageEnd(nuCHAR* pData, nuUINT nDataLen)
{
    return nDataLen;
}

inline nubool CProtocolBaseZK::IsPackageEnd(nuCHAR* pData, nuUINT nDataLen)
{
    if( nDataLen < CMD_INFO_END_MARK_SIZE )
    {
        return nuFalse;
    }
    for( nuUINT i = 0; i < CMD_INFO_END_MARK_SIZE; ++i )
    {
        if( (nuCHAR)CMD_INFO_END_MARK != pData[i] )
        {
            return nuFalse;
        }
    }
    return nuTrue;
}

CProtocolBaseZK::CProtocolBaseZK()
{
    m_bLock         = nuFalse;
    m_pBackupBuff   = nuNULL;
    m_nBuffLen      = 0;
	InitializeCriticalSection(&m_bSection);
}

CProtocolBaseZK::~CProtocolBaseZK()
{
    ReleaseBackup();
	DeleteCriticalSection(&m_bSection);
}

nuUINT CProtocolBaseZK::PackageData(nuCHAR* pBuffer, nuUINT nLen, const nuCHAR* pData, 
                                    nuUINT nDataLen, nuUINT nID, nuUINT nCmd, 
                                    nuUINT nWholeSize /* = 0 */, nuUINT nAddr /* = 0 */)
{
    if( nuNULL == pData )
    {
        nDataLen = 0;
    }
    if( 0 == nWholeSize )
    {
        nWholeSize = nDataLen;
    }
    if( nLen < nDataLen + CMD_INFO_MIN_SIZE )
    {
        return 0;
    }
    //set header marks
    nuMemcpy(pBuffer, cs_header, CMD_INFO_HEAD_MARK_SIZE);
    pBuffer += CMD_INFO_HEAD_MARK_SIZE;
    //copy the package size
#ifdef _USE_OLD_PACKAGE_SIZE
    nDataLen += CMD_INFO_PACKAGE_COST;
    nuMemcpy(pBuffer, &nDataLen, CMD_INFO_HEAD_PACK_SIZE);
    nDataLen -= CMD_INFO_PACKAGE_COST;
#else
    nuMemcpy(pBuffer, &nDataLen, CMD_INFO_HEAD_PACK_SIZE);
#endif
    pBuffer += CMD_INFO_HEAD_PACK_SIZE;
    //copy the whole size of the data
    nuMemcpy(pBuffer, &nWholeSize, CMD_INFO_HEAD_DATA_SIZE);
    pBuffer += CMD_INFO_HEAD_DATA_SIZE;
    //copy the ID
    nuMemcpy(pBuffer, &nID, 4);
    pBuffer += CMD_INFO_HEAD_BUS_ID;
    nuMemcpy(pBuffer, &nCmd, CMD_INFO_HEAD_CMD_SIZE);
    pBuffer += CMD_INFO_HEAD_CMD_SIZE;
    if( pData && 0 != nDataLen )
    {
        nuMemcpy(pBuffer, pData, nDataLen);
        pBuffer += nDataLen;
    }
    nuMemcpy(pBuffer, &nAddr, CMD_INFO_END_DATA_ADDR);
    pBuffer += CMD_INFO_END_DATA_ADDR;
    nuMemcpy(pBuffer, cs_end, CMD_INFO_END_MARK_SIZE);
    return nDataLen + CMD_INFO_MIN_SIZE;
}

nuUINT CProtocolBaseZK::ProcBinaryData(nuCHAR* pData, nuUINT nDataLen)
{
    //CBaseLock lock(&m_bLock);
    CCriticalLock lock(&m_bSection);
	nuCHAR *pBuffer = nuNULL;
    if( m_pBackupBuff )
    {
        pBuffer = new nuCHAR[m_nBuffLen + nDataLen];
        if( nuNULL != pBuffer )
        {
            nuMemcpy(pBuffer, m_pBackupBuff, m_nBuffLen);
            nuMemcpy(&pBuffer[m_nBuffLen], pData, nDataLen);
            nDataLen = m_nBuffLen + nDataLen;
        }
        pData = pBuffer;
        ReleaseBackup();
    }//join the left data and the now data together
    if( pData )
    {
        nuUINT nIndex = 0;
        while( nIndex < nDataLen )
        {
            CMD_DATA cmdData;
            nuUINT nOneSize = AnalyseOnePackage(&pData[nIndex], nDataLen-nIndex, cmdData);
            nIndex += nOneSize;
            if( CMD_ACTION_INVALID == cmdData.cdCmdAction )
            {
                //back up data
                BackupData(&pData[nIndex], nDataLen-nIndex);
                break;
            }
            else if( CMD_ACTION_VALID == cmdData.cdCmdAction )
            {
#ifdef NDEBUG
			 _DEBUG_OUT_PACAGE(&pData[nIndex-nOneSize], nOneSize);
#endif
             ProcPackage(cmdData);
            }
        }
    }
    if( pBuffer )
    {
        delete[] pBuffer;
    }
    return nDataLen;
}

nuUINT CProtocolBaseZK::BackupData(nuCHAR* pData, nuUINT nLen)
{
    if( nuNULL == pData || 0 == nLen )
    {
        return 0;
    }
    ReleaseBackup();
    m_pBackupBuff = new nuCHAR[nLen];
    if( nuNULL == m_pBackupBuff )
    {
        return 0;
    }
    nuMemcpy(m_pBackupBuff, pData, nLen);
    m_nBuffLen = nLen;
    return m_nBuffLen;
}

nuVOID CProtocolBaseZK::ReleaseBackup()
{
    if( m_pBackupBuff )
    {
        delete[] m_pBackupBuff;
        m_pBackupBuff   = nuNULL;
        m_nBuffLen      = 0;
    }
}

/*
nuUINT CProtocolBaseZK::AnalyseOnePackage(nuCHAR* pData, nuUINT nDataLen, CMD_DATA& cmdData)
{
    cmdData.cdCmdAction = CMD_ACTION_INVALID;
    if( nDataLen < CMD_INFO_MIN_SIZE )
    {
        return 0;
    }
    nuUINT nIndex = 0;
    nuUINT nSameCount = 0;
    while( nIndex < nDataLen )
    {
        if( (nuCHAR)CMD_INFO_HEAD_MARK == pData[nIndex] )
        {
            ++nSameCount;
            if( CMD_INFO_HEAD_MARK_SIZE == nSameCount )
            {
                ++nIndex;
                break;
            }
        }
        else
        {
            nSameCount = 0;
        }
        ++nIndex;
    }
    if( CMD_INFO_HEAD_MARK_SIZE != nSameCount )//没能找到包头
    {
        return (nDataLen-nSameCount);
    }
    if( nDataLen - nIndex < CMD_INFO_MIN_SIZE - CMD_INFO_HEAD_MARK_SIZE )
    {
        return nIndex - CMD_INFO_HEAD_MARK_SIZE;
    }//数据包大小太小
    nuUINT nIdxEnd = nIndex;
    nSameCount = 0;
    while( nIdxEnd < nDataLen )
    {
        if( (nuCHAR)CMD_INFO_END_MARK == pData[nIdxEnd] )
        {
            ++nSameCount;
            if( CMD_INFO_END_MARK_SIZE == nSameCount )
            {
                ++nIdxEnd;
                break;
            }
        }
        else
        {
            nSameCount = 0;
        }
        ++nIdxEnd;
    }
    nubool bFindEnd = nuFalse;
    if( CMD_INFO_END_MARK_SIZE == nSameCount )
    {
        bFindEnd = nuTrue;
    }
    nuMemcpy(&cmdData.cdData.dsNowSize, &pData[nIndex], CMD_INFO_HEAD_PACK_SIZE);//copy the effective size of the package
    nIndex += CMD_INFO_HEAD_PACK_SIZE;
#ifdef _USE_OLD_PACKAGE_SIZE
    cmdData.cdData.dsNowSize -= CMD_INFO_PACKAGE_COST;
#endif
    if( cmdData.cdData.dsNowSize > CMD_INFO_MAX_SIZE - CMD_INFO_MIN_SIZE )
    {
    }//数据超过设置大小，需要丢掉数据包
    
    if( nDataLen - nIdxStart - CMD_INFO_MIN_SIZE < cmdData.cdData.dsNowSize )
    {
        return nIdxStart;
    }//it's not a unbroken package
    nuUINT nEndIdx = nIndex + cmdData.cdData.dsNowSize + CMD_INFO_PACKAGE_COST;
    if( !IsPackageEnd(&pData[nEndIdx], nDataLen - nEndIdx) )
    {
        return nEndIdx + CMD_INFO_END_MARK_SIZE;
    }//failed to find the mark of the end
    nuMemcpy(&cmdData.cdData.dsWholeSize, &pData[nIndex], CMD_INFO_HEAD_DATA_SIZE);
    nIndex += CMD_INFO_HEAD_DATA_SIZE;
    nuMemcpy(&cmdData.cdBus, &pData[nIndex], CMD_INFO_HEAD_BUS_ID);
    nIndex += CMD_INFO_HEAD_BUS_ID;
    nuMemcpy(&cmdData.cdCommand, &pData[nIndex], CMD_INFO_HEAD_CMD_SIZE);
    nIndex += CMD_INFO_HEAD_CMD_SIZE;
    //
    if( 0 == cmdData.cdData.dsWholeSize || cmdData.cdData.dsNowSize == cmdData.cdData.dsWholeSize )
    {
        cmdData.cdManualSplit = MANUAL_SPLIT_NO;
    }//a whole package
    else
    {
        cmdData.cdManualSplit = MANUAL_SPLIT_YES;
    }
    //one of the manual splitted packages
    cmdData.cdData.dsDataBuffer = &pData[nIndex];
    nIndex += cmdData.cdData.dsNowSize;
    nuMemcpy(&cmdData.cdData.dsAddress, &pData[nIndex], CMD_INFO_END_DATA_ADDR);
    nIndex += CMD_INFO_END_DATA_ADDR + CMD_INFO_END_MARK_SIZE;
    cmdData.cdCmdAction = CMD_ACTION_VALID;
    return nIndex;
}
*/
nuUINT CProtocolBaseZK::AnalyseOnePackage(nuCHAR* pData, nuUINT nDataLen, CMD_DATA& cmdData)
{
    cmdData.cdCmdAction = CMD_ACTION_INVALID;
    if( nDataLen < CMD_INFO_MIN_SIZE )
    {
        return 0;
    }
    nuUINT nIndex = GetPackageBegin(pData, nDataLen);//get the begin of a package.
    if( nIndex == nDataLen )//can't find the mark of the begin
    {
        return nDataLen;
    }
    if( nDataLen - nIndex < CMD_INFO_MIN_SIZE )
    {
        return nIndex;
    }
    nuUINT nIdxStart = nIndex;
    nIndex += CMD_INFO_HEAD_MARK_SIZE;
    nuMemcpy(&cmdData.cdData.dsNowSize, &pData[nIndex], CMD_INFO_HEAD_PACK_SIZE);//copy the effective size of the package
    nIndex += CMD_INFO_HEAD_PACK_SIZE;
#ifdef _USE_OLD_PACKAGE_SIZE
    cmdData.cdData.dsNowSize -= CMD_INFO_PACKAGE_COST;
#endif
    //@Add on 20120220
    if( cmdData.cdData.dsNowSize > CMD_INFO_MAX_SIZE - CMD_INFO_MIN_SIZE )
    {
        cmdData.cdCmdAction = CMD_ACTION_LOST;
        nIdxStart += 1;
        return (nIdxStart + GetPackageBegin(&pData[nIdxStart], nDataLen - nIdxStart));
    }
    //@End Add
    if( nDataLen - nIdxStart - CMD_INFO_MIN_SIZE < cmdData.cdData.dsNowSize )
    {
        return nIdxStart;
    }//it's not a unbroken package
    nuUINT nEndIdx = nIndex + cmdData.cdData.dsNowSize + CMD_INFO_PACKAGE_COST;
    if( !IsPackageEnd(&pData[nEndIdx], nDataLen - nEndIdx) )
    {
        return nEndIdx + CMD_INFO_END_MARK_SIZE;
    }//failed to find the mark of the end
    nuMemcpy(&cmdData.cdData.dsWholeSize, &pData[nIndex], CMD_INFO_HEAD_DATA_SIZE);
    nIndex += CMD_INFO_HEAD_DATA_SIZE;
    nuMemcpy(&cmdData.cdBus, &pData[nIndex], CMD_INFO_HEAD_BUS_ID);
    nIndex += CMD_INFO_HEAD_BUS_ID;
    nuMemcpy(&cmdData.cdCommand, &pData[nIndex], CMD_INFO_HEAD_CMD_SIZE);
    nIndex += CMD_INFO_HEAD_CMD_SIZE;
    //
    if( 0 == cmdData.cdData.dsWholeSize || cmdData.cdData.dsNowSize == cmdData.cdData.dsWholeSize )
    {
        cmdData.cdManualSplit = MANUAL_SPLIT_NO;
    }//a whole package
    else
    {
        cmdData.cdManualSplit = MANUAL_SPLIT_YES;
    }
    //one of the manual splitted packages
    cmdData.cdData.dsDataBuffer = &pData[nIndex];
    nIndex += cmdData.cdData.dsNowSize;
    nuMemcpy(&cmdData.cdData.dsAddress, &pData[nIndex], CMD_INFO_END_DATA_ADDR);
    nIndex += CMD_INFO_END_DATA_ADDR + CMD_INFO_END_MARK_SIZE;
    cmdData.cdCmdAction = CMD_ACTION_VALID;
    return nIndex;
}

nuVOID CProtocolBaseZK::DebugOut(nuCHAR* pBuffer, nuUINT nLen)
{
    pBuffer += CProtocolBaseZK::CMD_INFO_HEAD_MARK_SIZE;
    nuINT nN = 0;
    nuMemcpy(&nN, pBuffer, sizeof(nuINT));
#ifdef _USE_OLD_PACKAGE_SIZE
    nN -= CMD_INFO_PACKAGE_COST;
#endif
    //printf 5 Headers
    pBuffer += 8;
    nuINT ID = 0;
    nuMemcpy(&ID, pBuffer, sizeof(nuINT));
    printf("ID  = %d\n", ID);
    pBuffer += 8;

    nuUINT cmd = 0;
    nuMemcpy(&cmd, pBuffer, sizeof(nuUINT));
    printf("CMD = 0x%x\n", cmd);

    pBuffer += 4;
    printf("N   = %d\n", nN);
    //printf data
    //if( nN )
    //{
    //    printf("DATA: ");
    //    for( nuINT i = 0; i < nN; ++i )
    //    {
    //        printf("0x%02x ", (nuBYTE)pBuffer[i]);
    //    }
    //    printf("\n");
    //}
    //pBuffer += nN;
    //pBuffer += 4;

    printf("---------------------------------------------------\n");
}