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
/*//��ʱ,Ҫ�����޸�,δ���
    if( nuNULL != m_pMemory )
    {
        munmap(m_pMemory,1);//1 Ҫ�޸�Ϊ ȡ��share��ramʵ�ʴ�С
        m_pMemory = nuNULL;
    }
    if( nuNULL != m_pMapping )
    {
        close(m_pMapping); 
        m_pMapping = nuNULL;
    }
*/
}