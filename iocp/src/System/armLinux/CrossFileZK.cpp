// CrossFileZK.cpp: implementation of the CCrossFileZK class.
//
//////////////////////////////////////////////////////////////////////

#include "CrossFileZK.h"
#include "inlineFunctionZK.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCrossFileZK::CCrossFileZK()
{
    m_file = nuNULL;
}

CCrossFileZK::~CCrossFileZK()
{
    fClose();
}

nubool CCrossFileZK::fOpen(nuTCHAR* tsName, nuBYTE nMode)
{
    fClose();
    nuTCHAR tsMode[10];
    tsMode[0] = 0;
    switch(nMode&(~FS_OPEN_TEXT))
    {
    case FS_OPEN_R_EXIST:
        nuTcscpy(tsMode, nuTEXT("r"));
        break;
    case FS_OPEN_RW_EXIST:
        nuTcscpy(tsMode, nuTEXT("r+"));
        break;
    case FS_OPEN_W_EMPTY:
        nuTcscpy(tsMode, nuTEXT("w"));
        break;
    case FS_OPEN_RW_EMPTY:
        nuTcscpy(tsMode, nuTEXT("w+"));
        break;
    case FS_OPEN_A:
        nuTcscpy(tsMode, nuTEXT("a"));
        break;
    case FS_OPEN_RA:
        nuTcscpy(tsMode, nuTEXT("a+"));
        break;
    default:
        return nuFalse;
    }
    if( nMode&FS_OPEN_TEXT )
    {
        nuTcscat(tsMode, nuTEXT("b"));
    }
    m_file = (nuHOBJECT*)fopen(tsName, tsMode);
    if( nuNULL == m_file )
    {
        return nuFalse;
    }
    return nuTrue;
}

nuVOID CCrossFileZK::fClose()
{
    if( nuNULL != m_file )
    {
        fclose((FILE*)m_file);
        m_file = nuNULL;
    }
}

nubool CCrossFileZK::fSeek(nuLONG nOffSet, nuBYTE nMode)
{
    if( nuNULL == m_file )
    {
        return nuFalse;
    }
    if( fseek((FILE*)m_file, nOffSet, nMode) )
    {
        return nuFalse;
    }
    else
    {
        return nuTrue;
    }
}

nuUINT CCrossFileZK::fRead(nuVOID* pBuffer, nuUINT nItemSize, nuUINT nItemCount)
{
    if( nuNULL == m_file )
    {
        return 0;
    }
    return fread(pBuffer, nItemSize, nItemCount, (FILE*)m_file);
}

nuUINT CCrossFileZK::fWrite(nuVOID* pBuffer, nuUINT nItemSize, nuUINT nItemCount)
{
    if( nuNULL == m_file )
    {
        return 0;
    }
    return fwrite(pBuffer, nItemSize, nItemCount, (FILE*)m_file);
}