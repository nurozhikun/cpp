// BaseFunctionsZK.cpp: implementation of the CBaseFunctionsZK class.
//
//////////////////////////////////////////////////////////////////////

#include "BaseFunctionsZK.h"
#include "inlineFunctionZK.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseFunctionsZK::CBaseFunctionsZK()
{

}

CBaseFunctionsZK::~CBaseFunctionsZK()
{

}

nuBYTE CBaseFunctionsZK::s_pMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

nuTIME CBaseFunctionsZK::GetLocalTime()
{
    time_t nTm = time(&nTm);
    return nTm;
}

nuTIME CBaseFunctionsZK::GetLocalTime(DATE& tmDate)
{
    time_t nTm = time(&nTm);
    nuMemcpy(&tmDate, localtime(&nTm), sizeof(DATE));
    return nTm;
}

nuTIME CBaseFunctionsZK::GetTime(nuTIME* pTime)
{
    nuTIME nTm = time((time_t *)pTime);
    return nTm;
}

nuVOID CBaseFunctionsZK::Time2Date(nuTIME nTime, DATE& dateOut)
{
    nuMemcpy(&dateOut, gmtime(&nTime), sizeof(dateOut));
}

nuVOID CBaseFunctionsZK::Time2Date(nuTIME nTime, const DATE& dateBase, DATE& dateOut)
{
    nTime += mktime((tm*)(&dateBase));
    nuMemcpy(&dateOut, gmtime(&nTime), sizeof(dateOut));
}

nuTIME CBaseFunctionsZK::Date2Time(const DATE& dateIn)
{
    return mktime((tm*)(&dateIn));
}

nuTIME CBaseFunctionsZK::Date2Time(const DATE& dateIn, const DATE& dateBase)
{
    return (mktime((tm*)(&dateIn)) - mktime((tm*)(&dateBase)));
}

nuVOID CBaseFunctionsZK::FormatTime2String(nuCHAR* pBuffer, nuUINT nBufSize, const DATE& date, emDateFormat nFormatType /* = Y_M_D_H_M_S */)
{
    switch(nFormatType)
    {
        case Y_M_D:
            strftime(pBuffer, nBufSize, "%Y-%m-%d", (tm*)(&date));
            break;
        case D_M_Y:
            strftime(pBuffer, nBufSize, "%D/%m/%Y", (tm*)(&date));
            break;
        case H_M_S_D_M_Y:
            strftime(pBuffer, nBufSize, "%H:%m:%S %d/%M/%Y", (tm*)(&date));
            break;
        case H_M_S:
            strftime(pBuffer, nBufSize, "%H:%m:%S", (tm*)(&date));
            break;
        default:
            strftime(pBuffer, nBufSize, "%Y-%m-%d %H:%M:%S", (tm*)(&date));
            break;
    }
}