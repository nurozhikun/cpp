// CrossGpsZK.cpp: implementation of the CCrossGpsZK class.
//
//////////////////////////////////////////////////////////////////////

#include "CrossGpsZK.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCrossGpsZK::CCrossGpsZK()
{
    m_hGps = nuNULL;
}

CCrossGpsZK::~CCrossGpsZK()
{

}

nubool CCrossGpsZK::OpenGps(nuCHAR* ptsGpsPort, nuINT nMode /* = OPEN_GPS_COMM */)
{
    if( nuNULL == m_hGps )
    {
        return nuFalse;
    }
    return nuTrue;
}

nuVOID CCrossGpsZK::CloseGps()
{
    if( m_hGps )
    {
        fclose((FILE*)m_hGps);
        m_hGps = nuNULL;
    }
}

nuUINT CCrossGpsZK::ReadData(nuCHAR* pBuffer, nuUINT nBufLen)
{
    return 0;
}