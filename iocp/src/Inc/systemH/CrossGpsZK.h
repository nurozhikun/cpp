// CrossGpsZK.h: interface for the CCrossGpsZK class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CROSSGPSZK_H__DC18E802_28DD_497B_BE85_905A8578F08B__INCLUDED_)
#define AFX_CROSSGPSZK_H__DC18E802_28DD_497B_BE85_905A8578F08B__INCLUDED_

#include "crossFunctionsZK.h"

class CCrossGpsZK  
{
public:
	CCrossGpsZK();
	virtual ~CCrossGpsZK();
    enum emGpsOpenMode
    {
        OPEN_GPS_SERIAL,
    };

    nubool  OpenGps(nuCHAR* ptsGpsPort, nuINT nMode = OPEN_GPS_SERIAL);
    nuVOID  CloseGps();
    nuUINT  ReadData(nuCHAR* pBuffer, nuUINT nBufLen);

protected:
    nuHOBJECT   m_hGps;

};

#endif // !defined(AFX_CROSSGPSZK_H__DC18E802_28DD_497B_BE85_905A8578F08B__INCLUDED_)
