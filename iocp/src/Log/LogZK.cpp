// LogZK.cpp: implementation of the CLogZK class.
//
//////////////////////////////////////////////////////////////////////

#include "LogZK.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#include <time.h>
#include <string.h>

#ifdef _MSC_VER
#include <windows.h>
#endif

#define LOG_ENABLE
#define MAX_WRITE_STRING_LENGTH         512

#ifdef _MSC_VER
static CRITICAL_SECTION g_logger;
#endif

nuVOID  CLogZK::LogBuffer(nuCHAR* pBuffer,  nuINT nLen, nuINT cmdType, nuINT ID)
{
#ifdef _MSC_VER
        EnterCriticalSection(&g_logger);
#endif
        CheckFile();
        time_t theTime;
        time(&theTime);
        char cTime[30];
        tm* tms;
        tms = localtime(&theTime);
        strftime(cTime, 30, "%Y-%m-%d %H:%M:%S\0", tms);
        fprintf(curFile, "%s\n ", cTime);


        fprintf(curFile, "ID = %d \n",ID);
        fprintf(curFile, "CMD = 0x%08x \n", cmdType);
        fprintf(curFile, "LEN = %d \n",nLen);
        fprintf(curFile, "%s", "Data =");
        for(int i=0; i<(int)nLen;++i)
        {
            fprintf(curFile, "0x%02X ", (unsigned char)pBuffer[i]);
        }
        fprintf(curFile, "\n");
        fflush(curFile);
#ifdef _MSC_VER
        LeaveCriticalSection(&g_logger);
#endif
}

nuVOID  CLogZK::Log(nuINT flags, nuINT logger_level, const nuCHAR* format, va_list list)
{

#ifdef LOG_ENABLE
#ifdef _MSC_VER
    EnterCriticalSection(&g_logger);
#endif
    // log to file only if flag is enabled
    if( flags & ELF_LOG_TO_FILE)
    {
        CheckFile();
        time_t theTime;
        time(&theTime);
        char cTime[30];
        tm* tms;
        tms = localtime(&theTime);
        strftime(cTime, 30, "%Y-%m-%d %H:%M:%S\0", tms);
        fprintf(curFile, "%s ", cTime);

        switch(logger_level)
        {
        case LEVEL_INFO:
            fprintf(curFile, "%s","[INFO]   ");
            break;
        case LEVEL_WARNING:
            fprintf(curFile, "%s","[WARNING]");
            break;
        case LEVEL_ERROR:
            fprintf(curFile, "%s", "[ERROR]  ");
            break;
        }
        vfprintf(curFile, format, list);
    }

    // log to console only if flag is enabled
    if( flags & ELF_LOG_TO_CONSOLE)
    {
        switch(logger_level)
        {
        case LEVEL_INFO:
            printf("%s","[INFO]   ");
            break;
        case LEVEL_WARNING:
            printf( "%s","[WARNING]");
            break;
        case LEVEL_ERROR:
            printf( "%s", "[ERROR]  ");
            break;
        }
        vprintf(format, list);
    }
    va_end(list);
    fflush(curFile);
#ifdef _MSC_VER
    LeaveCriticalSection(&g_logger);
#endif
#endif //LOG_ENABLE
}

nuVOID  CLogZK::SetLogPath(nuCHAR* logPath_)
{
	
	if( logPath != NULL)
	{
		strcpy(logPath, logPath_);
	    strcat(logPath, "/");
	}
}


void CLogZK::CheckFile()
{
    time_t theTime;
    time(&theTime);
    tm* tms;
    tms = localtime(&theTime);

    char logFileName_[30];
    strftime(logFileName_, 30, "%Y%m%d%H%M%S.log", tms);

	char logFileName[MAX_PATH];
	strcpy(logFileName, logPath);
	strcat(logFileName, logFileName_);

    char logDate[20];
    strftime(logDate, 20, "%Y%m%d\0", tms);

    if( curFile == NULL)
    {
        curFile = fopen(logFileName, "a+");
		if( curFile == NULL)
		{
			printf("create %s failed\n", logFileName);
		}
        strcpy(fileName,logDate); 
    }
    //日期不同要重新创建一个日志文件
    if(curFile != NULL && strcmp(logDate, fileName) != 0 )
    {
        strcpy(fileName, logDate);
        fclose(curFile);
        curFile = fopen(logFileName, "a+");
    }
}

CLogZK::CLogZK()
{
    curFile = NULL;
	memset(logPath, 0, MAX_PATH);
#ifdef _MSC_VER
    InitializeCriticalSection(&g_logger);
#endif
}

CLogZK::~CLogZK()
{
#ifdef _MSC_VER
    DeleteCriticalSection(&g_logger);
#endif
}

nuVOID CLogZK::LogTcs(nuTCHAR *ptzString)
{
    printf(ptzString);
}

nuVOID CLogZK::LogStringA(nuCHAR* szStr)
{
    printf(szStr);
}

nuVOID CLogZK::LogStringW(nuWCHAR* wzStr)
{
    wprintf(L"%ls",wzStr);
    //wprintf(wzStr);
}

nuVOID CLogZK::LogValue(nuCHAR* pszHeader, nuLONG nNum, nuUINT nBase, nuCHAR* pszEnd)
{
    if( 2 == nBase ) 
    {
        printf("%sb%b%s", pszHeader, nNum, pszEnd);
    }
    else if( 8 == nBase )
    {
        printf("%so%o%s", pszHeader, nNum, pszEnd);
    }
    else if( 16 == nBase )
    {
        printf("%s0X%x%s", pszHeader, nNum, pszEnd);
    }
    else
    {
        printf("%s%d%s", pszHeader, nNum, pszEnd);
    }
}
