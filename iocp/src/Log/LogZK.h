// LogZK.h: interface for the CLogZK class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGZK_H__C6E6624B_B782_4AA3_BBB4_E41134C27BFD__INCLUDED_)
#define AFX_LOGZK_H__C6E6624B_B782_4AA3_BBB4_E41134C27BFD__INCLUDED_

#include "ILogZK.h"
#include <stdio.h>
#include <stdarg.h>

class CLogZK : public CILogZK
{
public:
	CLogZK();
	virtual ~CLogZK();
    virtual nuVOID  LogTcs(nuTCHAR *ptzString);
    virtual nuVOID  LogStringA(nuCHAR* szStr);
    virtual nuVOID  LogStringW(nuWCHAR* wzStr);
    virtual nuVOID  LogValue(nuCHAR* pszHeader, nuLONG nNum, nuUINT nBase, nuCHAR* pszEnd);

    virtual  nuVOID  LogBuffer(nuCHAR* pBuffer,  nuINT nLen, nuINT cmdType, nuINT ID);
    virtual  nuVOID  Log(nuINT flags, nuINT logger_level, const nuCHAR* format, va_list list);
	virtual nuVOID  SetLogPath(nuCHAR* logPath_);
private:
    nuVOID              CheckFile();
    FILE* curFile;
    char fileName[100];
    unsigned int     s_flags;
	char logPath[MAX_PATH];
};

#endif // !defined(AFX_LOGZK_H__C6E6624B_B782_4AA3_BBB4_E41134C27BFD__INCLUDED_)
