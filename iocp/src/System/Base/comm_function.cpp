#include "comm_function.h"
#include "math.h"
#include "time.h"
#include <windows.h>
#include <iostream>

using namespace std;

///  @brief char* 串转为string型 
///
///  @param char* 串
///  @return 字符串值
///  @note 防止 value==NULL直接=string而崩溃
string char_as_string(const char* value)
{
    if( value == NULL)
        return "";
    return value;
}
///  @brief 十进制int整数转为字符串
///
///  @param value 整数值
///  @return 字符串值
///  @note
string int_as_string(int value)
{
    char c[BUS_SIZE];  
    _itoa_s(value, c, BUS_SIZE, 10);
    return c;
}

///  @brief 字符串转整数
///
///  @param value 字符串指针
///  @return 对应的整数值
///  @note value应该是纯数字
int string_as_int(const char* value)
{
    if ( value == NULL)
        return 0;
    return atoi(value);
}

string int_as_hex_string(int value)
{
    char c[BUS_SIZE];  
    _itoa_s(value, c, BUS_SIZE, 16);
    return c;
}

int hex_string_as_int(const char* value)
{
    if ( value == NULL)
        return 0;
    int   a = 0; 
    sscanf(value   ,   "%x "   ,   &a );
    return a;
}
///  @brief 字符串转长整数
///
///  @param value 字符串指针
///  @return 对应的长整数值
///  @note value应该是纯数字
long string_as_long(const char* value)
{
    if ( value == NULL)
        return 0;
    return atol(value);
}

///  @brief 浮点数转字符串
///
///  @param value 浮点值
///  @return 字符串值
///  @note 
string double_as_string(double value)
{
    char c[BUS_SIZE];
    //最多显示10位,四舍五入,科学计数
    int err = _gcvt_s(c, BUS_SIZE, value, 10);
    if (err != 0)
    {
        return "";
    }
    else
    {
        return c;
    }
}
///  @brief 字符串转浮点数
///
///  @param value 字符串值
///  @return 浮点值
///  @note 
double string_as_double(const char* value)
{
    if ( value == NULL)
        return 0.0;
    return strtod(value, 0);
}

wstring UTF8ToUnicode(const char* str)
{
	if( str == NULL)
		return L"";
	int unicodeLen = ::MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	wchar_t * pUnicode = new wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen +1 )*sizeof(wchar_t));
	::MultiByteToWideChar(CP_UTF8, 0, str, -1, (LPWSTR)pUnicode, unicodeLen);

	wstring rt = (wchar_t*)pUnicode;
	delete[] pUnicode;
	return rt;
}
///  @brief ANSI转Unicode
///
///  @param str 多字节字符串
///  @return 宽字符串
///  @note
wstring ANSIToUnicode(const char* str)
{
    if( str == NULL)
        return L"";
    int unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
    wchar_t * pUnicode = new wchar_t[unicodeLen + 1];
    memset(pUnicode, 0, (unicodeLen +1 )*sizeof(wchar_t));
    ::MultiByteToWideChar(CP_ACP, 0, str, -1, (LPWSTR)pUnicode, unicodeLen);

    wstring rt = (wchar_t*)pUnicode;
    delete[] pUnicode;
    return rt;
}
///  @brief Unicode转ANSI
///
///  @param str 宽字符串
///  @return 多字节字符串
///  @note 
string UnicodeToANSI(const wchar_t* str)
{
    if( str == NULL)
        return "";

    int iTextLen = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
    char* pElementText = new char[iTextLen+1];
    memset( (void*)pElementText, 0, sizeof(char)*(iTextLen+1));
    ::WideCharToMultiByte(CP_ACP, 0, str, -1, pElementText, iTextLen, NULL, NULL);

    string strText = pElementText;
    delete[] pElementText;
    return strText;
}
///  @brief 取得uuid
///
///  @return uuid串
///  @note 
string get_uuid()
{
    UUID uuid;
    unsigned char *uuidstr;
    if( UuidCreate( &uuid ) != RPC_S_OK ) {
        return "";
    }
    if( UuidToStringA( &uuid, &uuidstr ) !=  RPC_S_OK ) {
        return "";
    }
    string uid =  (char*)uuidstr;
    RpcStringFreeA( &uuidstr );
    return uid;
}

/// @brief 解析时间
///
///  @param time_buf "1989-11-11 20:18:12"
///  @return 从1970年到现在的秒数
///  @note
int timeconvert(const char *time_buf)  
{  
    if( time_buf == NULL)
        return 0;
    struct tm p;
    sscanf_s(time_buf,"%d-%d-%d %d:%d:%d",&(p.tm_year),&(p.tm_mon),&(p.tm_mday),&(p.tm_hour),&(p.tm_min), &(p.tm_sec));  
    p.tm_year -= 1900;
    p.tm_mon -= 1;
    return (int)mktime(&p);  
}  
/// @brief 解析时间
///
///  @param  从1970年到现在的秒数
///  @return  time_str "1989-11-11 20:18:12"
///  @note
string converttime(time_t seconds)
{
    tm result;
    if( localtime_s( &result, (const time_t*)&seconds)  || seconds <= 1000)
        return "1980-01-01 01:01:01";

    char  sDate[30];
    strftime(sDate, 30, "%Y-%m-%d %H:%M:%S", &result);
    return sDate;
}


/// @brief 解析时间
///
///  @param time_str "20:18:12"
///  @return 20:18:12 距离今天的秒
///  @note
int SecondsFromDay(const char* time_str)
{
    if( time_str == NULL)
        return 0;
    int hour =    string_as_int(Parse(time_str, 1, ":").c_str());
    int minute = string_as_int(Parse(time_str, 2, ":").c_str());
    int second = string_as_int(Parse(time_str, 3, ":").c_str());;
    return hour*3600 + minute*60 + second;
}
///  @brief 转换时间为分钟
///
///  @param value 时间字符串 "01:01:01"
///  @return 分钟 1*60+1
///  @note 
int MinutesFromDay(const char* value)
{
    if( value == NULL)
        return 0;
    int hour = string_as_int(Parse(value, 1, ":").c_str());
    int minute = string_as_int(Parse(value, 2, ":").c_str());
    return hour*60+minute;
}

///  @brief 取得以某分割符的第pos个字符串，pos>0
///
///  @param str 坐标字符串 "130.00,23.00;131.00,23.20"
///  @param pos 位置 从1开始
///  @param demi 分割符
///  @return 指定位置的字符串
///  @note 返回值不要用char*接收
string Parse(const char* str, int pos, const char* demi)
{
    if( str == NULL || pos <= 0)
        return "";
    char* temp = (char*)malloc(strlen(str)+1);
    strcpy_s(temp, strlen(str)+1, str);

    string result;
    char *next_token1 = NULL;
    char* token = strtok_s(temp, demi, &next_token1);
    int current_pos = 1;
    while( token != NULL)
    {
        if( current_pos == pos)
        {
            result = token;
            free(temp);
            return result;
        }
        token = strtok_s(NULL, demi, &next_token1);
        current_pos++;
    }
    free(temp);
    return "";
}

//取字符串以demi分割的第pos个字符串，返回pos+1个字符串位置，如果没有则返回NULL
char* ParseEx(const char* str, int pos, string& result, const char* demi)
{
    if( str == NULL ||  *str == '\0' || pos <= 0)
        return NULL;
    char* temp = (char*)malloc(strlen(str)+1);
    strcpy(temp, str);

    unsigned int cur_pos = 0;
    char* token = strtok(temp, demi);
    int current_pos = 1;
    while( token != NULL)
    {

        cur_pos += ( strlen(token) + 1);

        if( current_pos == pos)
        {
            result = token;

            free(temp);
            if( cur_pos > strlen(str) )
            {
                return NULL;
            }
            return  (char*)(str +cur_pos);
        }
        token = strtok(NULL, demi);
        current_pos++;
    }
    free(temp);

    if( cur_pos > strlen(str) )
    {
        return NULL;
    }
    return  (char*)(str +cur_pos);
}
string GetStrByDemi(const char* str, int startPos, int endPos /* =-1*/, const char* demi /*= " "*/)
{
    if( startPos <1 || (endPos < startPos && endPos != -1))
        return  "";

    string sResult;
    int i = startPos;
    string temp = Parse(str, startPos, demi);
    string back;
    while( ! temp.empty())
    {
        back = temp;
        i++;
        temp = Parse(str, i, demi);

        if( (i-1) == endPos || temp.empty())
        {
           sResult += back;
           break;
        }
        else
        {
           sResult += (back+demi);
        }
    }
    return sResult;
}


double xmult(double x1,double y1,double x2,double y2,double x0,double y0)
{
    return (x1-x0)*(y2-y0)-(x2-x0)*(y1-y0);
}
double area_triangle(double x1,double y1,double x2,double y2,double x3,double y3)
{
    return fabs(xmult(x1,y1,x2,y2,x3,y3))/2;
}

///  @brief 求点ex,ey到线段x1,y1, x2,y2的垂足与最短距离
///
///  @param x1 y1 线段第一个坐标点
///  @param x2 y2 线段第二个坐标点
///  @param ex ey 其它点
///  @param px, py 垂足点
///  @return 最短距离
///  @note
double dis_ptoline(double x1,double y1,double x2,double y2,double ex,double ey,double* px,double* py)
{ //第一个点，第二个点，目标点  ax ay为答案点  返回距离
    double yd=sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
    double t2=sqrt((x2-ex)*(x2-ex)+(y2-ey)*(y2-ey));
    double t1=sqrt((x1-ex)*(x1-ex)+(y1-ey)*(y1-ey));
    double dis=area_triangle(x1,y1,x2,y2,ex,ey)*2/yd;
    double tem1=sqrt(t1*t1-dis*dis);
    double tem2=sqrt(t2*t2-dis*dis);

    if (tem1>yd || tem2>yd)
    {
        if (t1>t2)
        {
            *px=x2;
            *py=y2;
            return t2;
        }
        else
        {
            *px=x1;
            *py=y1;
            return t1;
        }
    }
    *px=x1+(x2-x1)*tem1/yd;
    *py=y1+(y2-y1)*tem1/yd;
    return dis;
}

/// @brief 求两点间的距离
///
///  @param x1 y1 第一个点
///  @param x2 y2 第二个点
///  @return 两点间距离
///  @note
double distance(double x1, double y1, double x2, double y2)
{
    return sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}


/// @brief 取得当前时间 20120101020101
///  @return 取得当前时间
///  @note
string get_current_time()
{
    time_t theTime;
    time(&theTime);
    tm* tms;
    tms = localtime(&theTime);

    char logDate[30];
    strftime(logDate, 30, "%Y%m%d%H%M%S", tms);
    
    return string(logDate);
}

//返回距离今天的秒数
string get_current_date()
{
    time_t theTime;
    time(&theTime);
    tm* tms;
    tms = localtime(&theTime);
    char logDate[12];
    strftime(logDate, 12, "%Y%m%d\0", tms);
    return logDate;
}

double DistanceBetweenTwoCoordinates(int x1, int y1, int x2, int y2)
{
	double lon1 = double(x1)/(100000);
	double lat1 = double(y1)/(100000);
	double lon2 =double(x2)/(100000);
	double lat2 = double(y2)/(100000);

	double PI=3.1415936;
	double EARTH_RADIUS = 6371.004; //地球平均半径
	double radLat1 = lat1 * PI / 180.0;
	double radLat2 = lat2 * PI / 180.0;
	double a = radLat1 - radLat2;
	double b = (lon1* PI / 180.0) - (lon2 * PI / 180.0);
	double s = 2 * asin(sqrt(pow(sin(a / 2), 2) + cos(radLat1) * cos(radLat2) * pow(sin(b / 2), 2)));
	s = s * EARTH_RADIUS * 1000;
	return s;
}

bool isSamlePoint(int x1, int y1, int x2, int y2)
{
	return (x1 == x2) && (y1 ==y2);
}

int AngleDiff(int nAngle1, int nAngle2)
{
	nAngle1 %= 360;
	nAngle2 %= 360;
	if( nAngle1 < 0 )	nAngle1 += 360;
	if( nAngle2 < 0 )	nAngle2 += 360;
	int nDiff = nAngle2 - nAngle1;
	if( nDiff < 0 )		nDiff += 360;
	if( nDiff > 180)	nDiff = 360 - nDiff;
	return nDiff;
}
#define  PI 3.1415
int AngleOfPoints(int x1, int y1, int x2, int y2)
{
	int nAngle = (int)(180 * atan2((double)x2-x1, (double)y2-y1) / PI);
	if( nAngle < 0)	nAngle += 360;
	return nAngle;
}