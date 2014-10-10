#ifndef _SOCKET_PROTOCOL_H_2011109
#define _SOCKET_PROTOCOL_H_2011109

#include "typeDefine.h"

enum emSocketCommonConstants
{
	SCC_USER_ID_SIZE	= 64,
	SCC_USER_PW_SIZE	= 64,
};

enum emSocketProtocolCommand
{
    CMD_CLIENT_BASE             = 0,
    CMD_CLIENT_LOGIN            = 0x10,
    CMD_CLIENT_QUIT_BY_SAME		= 0x11, //被相同的ＩＤ踢掉
	CMD_CLIENT_LOGOUT		= 0x12,
    CMD_CLIENT_RECORD           = 0x100,
    CMD_CLIENT_RECORD_LOST		= 0x101,
    CMD_CLIENT_RECORD_GPS     = 0x102,
    CMD_CLIENT_HEARTBEAT        = 0x200,
    CMD_SERVER_BASE             = 0x40000000,
    CMD_SERVER_HEARTBEAT        = (CMD_CLIENT_HEARTBEAT+CMD_SERVER_BASE),
    CMD_REPLY_BASE              = 0x80000000,

    //enum below for gwq
    CMD_CLIENT_RECV_FAILED      = 0x300,
    CMD_CLIENT_KEY_DOWN         = 0x301,
    CMD_CLIENT_SHUTDOWN         = 0x302,
    CMD_CLIENT_VERSION          = 0x303,
    
    CMD_CLIENT_NUM_SCHEDULE     = 0x00001000,
    CMD_CLIENT_SCHEDULE         = 0x00001001,
    CMD_CLIENT_VEHICLE_INFO     = 0x00001002,
    CMD_CLIENT_COMM_DISPATCH    = 0x00001004,
    CMD_CLIENT_SPEC_DISPATCH    = 0x00001005,

    CMD_CLIENT_FILE_VERSION =  0x00003000, //文件版本请求
    CMD_CLIENT_FILE_DATA = 0x00003001,  //文件数据请求

    CMD_CLIENT_SEND_PARAMETER = 0x00003002, //请求发送配置
    CMD_CLIENT_GPS_SEND_PARAMETER = 0x00003005, //GPS上传配置
    CMD_CLIENT_DRIVER_CHECKIN = 0x00003006, //司机报到

	CMD_CLIENT_STOP_VERSION = 0x00003007, //停车场版本请求
	CMD_CLIENT_STOP_DATA  = 0x00003008, //停车场数据请求

	CMD_CLIENT_DRIVER_INFO = 0x00003009, //司机信息请求
	CMD_CLIENT_TWO_BAOZHAN =  0x00003010, //二次报站请求
	CMD_CLIENT_SPEECH_NUM =  0x00003011, // 语音打出号码请求
	CMD_CLIENT_SPEECH_CONFIG = 0x00003012, //服务用语配置请求

	CMD_CLIENT_SERVER_TIME  = 0x00003013, //请求服务器时间
	
    CMD_SERVER_NUM_SCHEDULE     = 0x40081000,
    CMD_SERVER_SCHEDULE         = 0x40081001,
    CMD_SERVER_VEHICLE_INFO     = 0x40081002,
    CMD_SERVER_COMM_DISPATCH    = 0x40081004,
    CMD_SERVER_SPEC_DISPATCH    = 0x40081005,
    CMD_SERVER_INFO_DRIVER      = 0x40082000,
    CMD_SERVER_ALARM_CMD        = 0x40082001,
    CMD_SERVER_SHIFT_LINE           = 0x40085004,
    CMD_SERVER_INFO_PASSENGER   = 0x40082010,
    CMD_SERVER_FILE_DATA =  0x40083001,  //文件数据回复

    CMD_SERVER_CMD_START_BUS  = 0x40084001,
    CMD_SERVER_CMD_CHANGE_DRIVER  = 0x40084004,
    CMD_SERVER_CMD_RECORD_LOST = 0x40085001,
    CMD_SERVER_CMD_SEND_PARAMETER = 0x40085002,
    CMD_SERVER_CMD_GPS_SEND_PARAMETER = 0x40085005,

	CMD_SERVER_CMD_STOP_DATA = 0x40085008, //停车场数据回复

    CMD_CLIENT_REPLY_NUM_SCHEDULE   = 0xC0081000,
    CMD_CLIENT_REPLY_SCHEDULE   = 0xC0081001,
    CMD_CLIENT_REPLY_INFO_DRIVER = 0xC0082000,
    CMD_CLIENT_REPLY_CMD_START_BUS =   0xC0084001,
    CMD_CLIENT_REPLY_CMD_SHIFT_LINE =  0xC0085004,
    CMD_CLIENT_REPLY_CMD_CHANGE_DRIVER =  0xC0084004,
};

#endif