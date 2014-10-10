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
    if( nuNULL != m_pMemory )
    {
        UnmapViewOfFile(m_pMemory);
        m_pMemory = nuNULL;
    }
    if( nuNULL != m_pMapping )
    {
        CloseHandle(m_pMapping);
        m_pMapping = nuNULL;
    }
}