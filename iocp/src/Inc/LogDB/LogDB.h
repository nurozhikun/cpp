#ifndef LOG_DB_H
#define LOG_DB_H
#include <stdarg.h>
#include <vector>
#include "sqlite3.h"
#include "comm_function.h"

#include "ILogDB.h"
#define MAX_DATA_LENGTH 1024
#define MAX_INFO_LENGTH 256
#define FLUSH_SIZE 5000
#define SERVER_NAMAGER_FLUSH_SIZE 1000

typedef struct tagCMD_BUFF
{
    int id;
    int cmd_type;
    int length;
    char cmd_data[MAX_DATA_LENGTH];
    struct tagCMD_BUFF(int _id, int _cmd_type, int _length, char* pBuffer)
    {
        id = _id;
        cmd_type = _cmd_type;
        length = _length > MAX_DATA_LENGTH ? MAX_DATA_LENGTH : _length;
        if( pBuffer != NULL)
        {
            memset(cmd_data, 0, sizeof(cmd_data));
            memcpy(cmd_data, pBuffer, length);
        }
    }
}CMD_BUFF;

typedef struct tagInfo
{
    int level;
    char info[MAX_INFO_LENGTH];
    struct tagInfo(int _level, const char* pInfo)
    {
        level = _level;
        int length = strlen(pInfo)+1;
        length = min(MAX_INFO_LENGTH-1, length);
        memset(info, 0, sizeof(info));
        memcpy(info, pInfo, length);
    }
}INFO;

class CLogDB: public CILogDB
{
public:
     CLogDB();
    ~CLogDB();
    
    bool SetDBFileName(char* dbName);
    void  LogBuffer(char* pBuffer,  unsigned int nLen, unsigned int cmdType, int ID);
    void  Log(int logger_level, const char* pBuffer);
    void  LogServerManager(const char* pBuffer, unsigned int nLen, unsigned int cmdType, int ID);
    void  LogKeyValue(char* key, char* value);
    void  FlushBuffer();
private:
    int LogBuffer();
    int LogInfo();
    int LogServerManager();
    char* dbName;
    sqlite3* pDB;
    sqlite3_stmt* pStmt_cmds;
    sqlite3_stmt* pStmt_infos;
    sqlite3_stmt* pStmt_manager;
    sqlite3_stmt* pStmt_keyvalue;
    std::vector<CMD_BUFF> v_cmds;
    std::vector<INFO> v_infos;
    std::vector<CMD_BUFF> v_server_manager;
};
#endif