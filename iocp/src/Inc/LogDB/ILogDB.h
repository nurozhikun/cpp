#ifndef ILOG_DB_H
#define ILOG_DB_H
#include <stdarg.h>
#include <stdio.h>
class CILogDB
{
public:
    CILogDB() {}
    virtual ~CILogDB() {}

    virtual bool  SetDBFileName(char* dbName) = 0;
    virtual void  LogBuffer(char* pBuffer,  unsigned int nLen, unsigned int cmdType, int ID) = 0;
    virtual void  Log(int logger_level, const char* pBuffer) = 0;
    virtual void  LogServerManager(const char* pBuffer, unsigned int nLen, unsigned int cmdType, int ID) = 0;
    virtual void  LogKeyValue(char* key, char* value) = 0;
    virtual void  FlushBuffer() = 0;
};
#endif