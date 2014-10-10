// BaseCodeListZK.cpp: implementation of the CBaseCodeListZK class.
//
//////////////////////////////////////////////////////////////////////

#include "BaseCodeListZK.h"
#include "inlineFunctionZK.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseCodeListZK::CBaseCodeListZK()
{
    m_pCodeList     = NULL;
    m_nCodeCount    = 0;
    CreateCodeList();
}

CBaseCodeListZK::~CBaseCodeListZK()
{
    DeleteCodeList();
}

nubool CBaseCodeListZK::CreateCodeList(nuUINT nCodeCount /* = BASE_LIST_NODE_DEFAULT_COUNT */)
{
    if( m_nCodeCount == nCodeCount )
    {
        return nuTrue;
    }
    DeleteCodeList();
    m_pCodeList = (BASE_LIST_NODE*)nuMalloc(sizeof(BASE_LIST_NODE)*nCodeCount);
    if( NULL == m_pCodeList )
    {
        return nuFalse;
    }
    m_nCodeCount    = nCodeCount;
    m_nFIndex       = 0;
    m_nLIndex       = 0;
    m_nNowCount     = 0;
    m_bWorking      = nuFalse;
    return nuTrue;
}

nuVOID CBaseCodeListZK::DeleteCodeList()
{
    if( nuNULL != m_pCodeList )
    {
        nuFree(m_pCodeList);
        m_pCodeList     = nuNULL;
        m_nCodeCount    = 0;
    }
}
//潜在问题,当node超过分配的数量后,数量不会增加,一直是第1个节点被覆盖
// m_nLIndex +[m_nCodeCount]- m_nFIndex = m_nNowCount
nubool CBaseCodeListZK::PushNode(const BASE_LIST_NODE& codeNode, nuBYTE nAcType /* = ACTION_AT_END */)
{
    if( nuNULL == m_pCodeList )
    {
        return nuFalse;
    }
    CBaseLock lock(&m_bWorking);
    nuLONG nIndex = -1;
    if( ACTION_DEL_ALL_OTHERS == nAcType )
    {
        m_nNowCount = 0;
        m_nFIndex   = 0;
        m_nLIndex   = 0;
        nIndex = m_nLIndex;
        ++m_nLIndex;
        ++m_nNowCount;
    }
    else if( ACTION_AT_HEAD == nAcType )
    {
        if( 0 == m_nFIndex )
        {
            m_nFIndex = m_nCodeCount - 1;
        }
        else
        {
            --m_nFIndex;
        }
        nIndex = m_nFIndex;
        //////////////////////////////////////////
        if( m_nNowCount < m_nCodeCount )
        {
            ++m_nNowCount;
        }
        else
        {
            m_nLIndex = m_nFIndex;
        }
        ///////////////////////////////////////////
    }
    else if( ACTION_FULL_COVER == nAcType )
    {
        nIndex = m_nLIndex;
        if( m_nNowCount == m_nCodeCount )
        {
            m_nLIndex   = (m_nLIndex + 1) % m_nCodeCount;
            m_nFIndex   = m_nLIndex;
        }
        else
        {
            ++m_nNowCount;
            m_nLIndex   = (m_nLIndex + 1) % m_nCodeCount;
        }
    }
    else if( ACTION_FULL_DEL == nAcType )
    {
        if( m_nNowCount == m_nCodeCount )
        {
            m_nNowCount = 0;
            m_nFIndex   = 0;
            m_nLIndex   = 0;
            nIndex = m_nLIndex;
            ++m_nLIndex;
            ++m_nNowCount;
        }
        else
        {
            nIndex = m_nLIndex;
            ++m_nNowCount;
            m_nLIndex   = (m_nLIndex + 1) % m_nCodeCount;
        }
    }
    else
    {
        if( m_nNowCount != m_nCodeCount )
        {
           // nIndex = m_nFIndex;
            nIndex = m_nLIndex;
            ++m_nNowCount;
            m_nLIndex   = (m_nLIndex + 1) % m_nCodeCount;
        }
    }
    if( -1 != nIndex )
    {
        m_pCodeList[nIndex] = codeNode;
        return nuTrue;
    }
    else
    {
        return nuFalse;
    }
}

nubool CBaseCodeListZK::PopNode(BASE_LIST_NODE& codeNode, nuBYTE nAcType /* = ACTION_AT_HEAD */)
{
    if( nuNULL == m_pCodeList || 0 == m_nNowCount )
    {
        return nuFalse;
    }
    CBaseLock lock(&m_bWorking);
    if( ACTION_AT_END == nAcType )
    {
        if( 0 == m_nLIndex )
        {
            m_nLIndex = m_nCodeCount - 1;
        }
        else
        {
            --m_nLIndex;
        }
        codeNode = m_pCodeList[m_nLIndex];
        --m_nNowCount;
    }
    else
    {
        codeNode = m_pCodeList[m_nFIndex];
        --m_nNowCount;
        m_nFIndex = (m_nFIndex + 1) % m_nCodeCount;
    }
    return nuTrue;
}