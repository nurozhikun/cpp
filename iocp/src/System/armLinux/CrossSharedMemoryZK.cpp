// CrossSharedMemoryZK.cpp: implementation of the CCrossSharedMemoryZK class.
//
//////////////////////////////////////////////////////////////////////

#include "CrossSharedMemoryZK.h"
#include "inlineFunctionZK.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCrossSharedMemoryZK::CCrossSharedMemoryZK()
{
    m_pMemory   = nuNULL;
    m_pMapping  = nuNULL;
}

CCrossSharedMemoryZK::~CCrossSharedMemoryZK()
{
    CloseShareMemory();
}

nuVOID* CCrossSharedMemoryZK::CreateSharedMemory(const nuCHAR* psName)
{
    CloseShareMemory();
    if( OpenSharedMemory(psName) )
    {
        return m_pMemory;
    }
//     FILE* pFile = CreateFileA( psName
//         , GENERIC_READ | GENERIC_WRITE
//         , FILE_SHARE_READ | FILE_SHARE_WRITE
//         , NULL
//         , OPEN_ALWAYS
//         , FILE_ATTRIBUTE_NORMAL
//         , NULL);
//     char* psFile = nuNULL;
    return m_pMemory;
}

nuVOID* CCrossSharedMemoryZK::OpenSharedMemory(const nuCHAR* psName)
{
    return m_pMemory;
}

nuVOID CCrossSharedMemoryZK::CloseShareMemory()
{
/*//暂时,要整体修改,未完成
    if( nuNULL != m_pMemory )
    {
        munmap(m_pMemory,1);//1 要修改为 取消share的ram实际大小
        m_pMemory = nuNULL;
    }
    if( nuNULL != m_pMapping )
    {
        close(m_pMapping); 
        m_pMapping = nuNULL;
    }
*/
}