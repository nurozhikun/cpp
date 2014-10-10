// BaseFunctionsZK.h: interface for the CBaseFunctionsZK class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEFUNCTIONSZK_H__701C1650_47CE_44E7_A876_63F7F9DDD422__INCLUDED_)
#define AFX_BASEFUNCTIONSZK_H__701C1650_47CE_44E7_A876_63F7F9DDD422__INCLUDED_

#include "typeDefine.h"

class CBaseFunctionsZK  
{
public:
	CBaseFunctionsZK();
	virtual ~CBaseFunctionsZK();
    typedef struct tagDATE
    {
        nuUINT tm_sec;     /* seconds after the minute - [0,59] */
        nuUINT tm_min;     /* minutes after the hour - [0,59] */
        nuUINT tm_hour;    /* hours since midnight - [0,23] */
        nuUINT tm_mday;    /* day of the month - [1,31] */
        nuUINT tm_mon;     /* months since January - [0,11] */
        nuUINT tm_year;    /* years since 1900 */
        nuUINT tm_wday;    /* days since Sunday - [0,6] */
        nuUINT tm_yday;    /* days since January 1 - [0,365] */
        nuUINT tm_isdst;   /* daylight savings time flag */
    }DATE;
    static nuTIME GetLocalTime();
    static nuTIME GetLocalTime(DATE& tmDate);
    static nuVOID Time2Date(nuTIME nTime, DATE& dateOut);
    static nuVOID Time2Date(nuTIME nTime, const DATE& dateBase, DATE& dateOut);
    static nuTIME Date2Time(const DATE& dateIn);
    static nuTIME Date2Time(const DATE& dateIn, const DATE& dateBase);
    enum emDateFormat
    {
        Y_M_D_H_M_S,
        Y_M_D,
        D_M_Y,
        H_M_S_D_M_Y,
        H_M_S,
    };
    static nuVOID FormatTime2String(nuCHAR* pBuffer, nuUINT nBufSize, const DATE& date, emDateFormat nFormatType = Y_M_D_H_M_S);
    static nuTIME GetTime(nuTIME* pTime);
    ////////////////////////////////////////////////////////////////////////////////////////
private:
    static nuBYTE   s_pMonth[];
    static DATE     s_baseDate;

};

#endif // !defined(AFX_BASEFUNCTIONSZK_H__701C1650_47CE_44E7_A876_63F7F9DDD422__INCLUDED_)
