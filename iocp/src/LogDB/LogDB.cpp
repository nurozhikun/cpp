#include "LogDB.h"
#include <windows.h>
#include "comm_function.h"
static CRITICAL_SECTION g_loggerdb;
CLogDB::CLogDB()
{
    InitializeCriticalSection(&g_loggerdb);
    pDB = NULL;
    dbName = NULL;
}

CLogDB::~CLogDB()
{
    FlushBuffer();

    if( pStmt_infos != NULL)
    {
        sqlite3_finalize(pStmt_infos);
    }

    if( pStmt_cmds != NULL)
    {
        sqlite3_finalize(pStmt_cmds);
    }

    if( pDB != NULL)
    {
        sqlite3_close(pDB);
        pDB = NULL;
    }
    DeleteCriticalSection(&g_loggerdb);
}

bool CLogDB::SetDBFileName(char* dbName)
{
    if( dbName == NULL)
    {
        return false;
    }
    if( dbName != NULL && this->dbName != NULL 
        && strcmp(dbName, this->dbName) == 0)
    {
        return true;
    }

    EnterCriticalSection(&g_loggerdb);
    if( pDB != NULL)
    {
        sqlite3_close(pDB);
    }
    int rc = sqlite3_open(dbName, &pDB);
    if( rc != SQLITE_OK)
    {
        LeaveCriticalSection(&g_loggerdb);
        return false;
    }
    rc = sqlite3_exec( pDB, "create table if not exists log_cmd_data(log_time TEXT, id INTEGER, \
                            cmd_type TEXT, data_length INTEGER, record_index INTEGER, record_time TEXT, record_type TEXT, cmd_data BLOB)", 0, 0,0);
    rc = sqlite3_exec( pDB, "create table if not exists log_vehicle_info(log_time TEXT, level INTEGER, \
                            content TEXT)", 0, 0,0);
    rc = sqlite3_exec( pDB, "create table if not exists log_server_manager(log_time TEXT, ID TEXT, cmd_type TEXT, \
                            data_length INTEGER, cmd_data TEXT)", 0, 0,0);
    rc = sqlite3_exec( pDB, "create table if not exists log_server_info(log_time TEXT, key TEXT, value TEXT)", 0, 0,0);

    string insert_sql = "insert into log_cmd_data(log_time, id, cmd_type,data_length,record_index,record_time, \
        record_type,cmd_data) values(datetime('now','+8 hour'),?,?,?,?,?,?,?)";
    rc = sqlite3_prepare(pDB, insert_sql.c_str(), -1, &pStmt_cmds, 0);

     insert_sql = "insert into log_vehicle_info(log_time, level, content) values(datetime('now','+8 hour'),?,?)";
    rc = sqlite3_prepare(pDB, insert_sql.c_str(), -1, &pStmt_infos, 0);

    insert_sql = "insert into log_server_manager(log_time, ID, cmd_type,  data_length, cmd_data) \
                 values(datetime('now','+8 hour'), ?,?,?,?)";
    rc = sqlite3_prepare(pDB, insert_sql.c_str(), -1, &pStmt_manager, 0);

    insert_sql = "insert into log_server_info(log_time, key, value) values(datetime('now','+8 hour'),?,?)";
    rc = sqlite3_prepare(pDB, insert_sql.c_str(), -1, &pStmt_keyvalue, 0);

    LeaveCriticalSection(&g_loggerdb);
    return true;
}
void  CLogDB::FlushBuffer()
{
    EnterCriticalSection(&g_loggerdb);
    LogBuffer();
    LogInfo();
    LogServerManager();
    LeaveCriticalSection(&g_loggerdb);
}

void  CLogDB::LogKeyValue(char* key, char* value)
{
    if( key == NULL || value == NULL)
        return ;

    char *zErrorMsg = NULL;
    int result = sqlite3_exec( pDB, "begin transaction", 0, 0,  &zErrorMsg); //开始一个事务
    if( zErrorMsg != NULL)
        sqlite3_free(zErrorMsg);

    result = sqlite3_reset(pStmt_keyvalue);
    result = sqlite3_bind_text(pStmt_keyvalue, 1, key, strlen(key)+1, 0);
    result = sqlite3_bind_text(pStmt_keyvalue, 2, value, strlen(value)+1, 0);
    result = sqlite3_step(pStmt_keyvalue);
    result = sqlite3_reset(pStmt_keyvalue);

    result = sqlite3_exec( pDB, "commit transaction", 0, 0, &zErrorMsg); //提交事务
    if( zErrorMsg != NULL)
        sqlite3_free(zErrorMsg);
    return ;
}

int CLogDB::LogServerManager()
{
    char *zErrorMsg = NULL;
    int result = sqlite3_exec( pDB, "begin transaction", 0, 0,  &zErrorMsg); //开始一个事务
    if( zErrorMsg != NULL)
        sqlite3_free(zErrorMsg);
    for(int i=0; i<(int)v_server_manager.size(); ++i)
    {       
        string id = "0x"+int_as_hex_string(v_server_manager.at(i).id); 
        string cmd_type =  "0x"+int_as_hex_string(v_server_manager.at(i).cmd_type); 
        int length = v_server_manager.at(i).length;
        string content;
        if( v_server_manager.at(i).cmd_type < 0x02000000 && v_server_manager.at(i).cmd_type >= 0x01000000)//web发过来的指令，要转成ANSI
        {
            wstring temp = (wchar_t*)(v_server_manager.at(i).cmd_data);
            content = UnicodeToANSI(temp.c_str());
        }
        else
        {
            content.assign(v_server_manager.at(i).cmd_data, length);
        }

        result = sqlite3_reset(pStmt_manager);
        result = sqlite3_bind_text(pStmt_manager, 1, id.c_str(), id.size(), 0);
        result = sqlite3_bind_text(pStmt_manager, 2, cmd_type.c_str(), cmd_type.size(), 0);
        result = sqlite3_bind_int(pStmt_manager, 3, length);
        result = sqlite3_bind_text(pStmt_manager, 4, content.c_str(), content.size(), 0);
        result = sqlite3_step(pStmt_manager);
        result = sqlite3_reset(pStmt_manager);
    }
    result = sqlite3_exec( pDB, "commit transaction", 0, 0, &zErrorMsg); //提交事务
    if( zErrorMsg != NULL)
        sqlite3_free(zErrorMsg);
    return 0;
}
void  CLogDB::LogServerManager(const char* pBuffer, unsigned int nLen, unsigned int cmdType, int ID)
{
    if( pBuffer == NULL)
        return ;
    EnterCriticalSection(&g_loggerdb);
    v_server_manager.push_back(CMD_BUFF(ID, cmdType, nLen, (char*)pBuffer));
    if( v_server_manager.size() >= SERVER_NAMAGER_FLUSH_SIZE)
    {
        LogServerManager();
        v_server_manager.clear();
    }
    LeaveCriticalSection(&g_loggerdb);
}
int CLogDB::LogBuffer()
{
    char *zErrorMsg = NULL;
    int result = sqlite3_exec( pDB, "begin transaction", 0, 0,  &zErrorMsg); //开始一个事务
    if( zErrorMsg != NULL)
        sqlite3_free(zErrorMsg);
    for(int i=0; i<(int)v_cmds.size(); ++i)
    {       
        int id = v_cmds.at(i).id;
        string cmd_type =  "0x"+int_as_hex_string(v_cmds.at(i).cmd_type); 
        int length = v_cmds.at(i).length;
        char* pData = v_cmds.at(i).cmd_data;

        int recordIndex  = 0;
        string time;
        string record_cmd_type;
        if( length >= 12 && pData != NULL && 
             ( v_cmds.at(i).cmd_type == 0x00000100 || v_cmds.at(i).cmd_type == 0x00000101 ) )
        {
            memcpy(&recordIndex, pData, sizeof(int));
            int record_type = 0;
            memcpy(&record_type, pData+4, sizeof(int));
            record_cmd_type = "0x"+int_as_hex_string(record_type); 

            int iTime  = 0;
            memcpy(&iTime, pData+8, sizeof(int));
            time = converttime(iTime);

        }
        result = sqlite3_reset(pStmt_cmds);
        result = sqlite3_bind_int(pStmt_cmds, 1,id);
        result = sqlite3_bind_text(pStmt_cmds, 2, cmd_type.c_str(), cmd_type.size(), 0);
        result = sqlite3_bind_int(pStmt_cmds, 3, length);
        result = sqlite3_bind_int(pStmt_cmds, 4, recordIndex);
        result = sqlite3_bind_text(pStmt_cmds, 5, time.c_str(), time.size(), 0);
        result = sqlite3_bind_text(pStmt_cmds, 6, record_cmd_type.c_str(), record_cmd_type.size(), 0);
        result = sqlite3_bind_blob(pStmt_cmds, 7, pData, length, 0);
        result = sqlite3_step(pStmt_cmds);
        result = sqlite3_reset(pStmt_cmds);

    }
    result = sqlite3_exec( pDB, "commit transaction", 0, 0, &zErrorMsg); //提交事务
    if( zErrorMsg != NULL)
        sqlite3_free(zErrorMsg);
    return 0;
}
void  CLogDB::LogBuffer(char* pBuffer,  unsigned int nLen, unsigned int cmdType, int ID)
{
    if( pBuffer == NULL)
        return ;
    EnterCriticalSection(&g_loggerdb);
    v_cmds.push_back(CMD_BUFF(ID, cmdType, nLen, pBuffer));
    if( v_cmds.size() >= FLUSH_SIZE)
    {
        LogBuffer();
        v_cmds.clear();
    }
    LeaveCriticalSection(&g_loggerdb);
}
int CLogDB::LogInfo()
{
    //char *zErrorMsg = NULL;
    int result = sqlite3_exec( pDB, "begin transaction", 0, 0,  NULL ); //开始一个事务
    for(int i=0; i<(int)v_infos.size(); ++i)
    {       
        result = sqlite3_reset(pStmt_infos);
        result = sqlite3_bind_int(pStmt_infos, 1, v_infos.at(i).level);
        result = sqlite3_bind_text(pStmt_infos, 2, v_infos.at(i).info, strlen(v_infos.at(i).info), 0);
        result = sqlite3_step(pStmt_infos);
         result = sqlite3_reset(pStmt_infos);
    }
    result = sqlite3_exec( pDB, "commit transaction", 0, 0, NULL ); //提交事务

    return 0;
}
void  CLogDB::Log(int logger_level, const char* pBuffer)
{
    if( pBuffer == NULL)
        return;
    EnterCriticalSection(&g_loggerdb);
    v_infos.push_back(INFO(logger_level, pBuffer));
    if( v_infos.size() >= 100)
    {
        LogInfo();
        v_infos.clear();
    }
    LeaveCriticalSection(&g_loggerdb);
}