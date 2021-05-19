/***
*AepServiceCodes.h - 定义上下行数据的结构体，还有提供组装上行报文的函数和解析下行报文的函数
*
*Purpose:
*	1.数据结构体命名和平台定义的服务标识一致
*	2.codeDataReportByIdToStr、codeDataReportByIdToBytes、codeDataReportByIdentifierToStr、codeDataReportByIdentifierToBytes为组装上报数据的函数，具体说明见函数前的注释
*	3.decodeCmdDownFromStr、decodeCmdDownFromBytes为解析平台发送过来数据的函数，具体说明见函数前的注释
****/
#ifndef AEPSERVICECODES_H
#define AEPSERVICECODES_H

#include <stdlib.h>
#include <string.h>


#define AEP_BIG_ENDIAN 'b'
#define AEP_LITTLE_ENDIAN 'l'

static union { char c[4]; unsigned long mylong; } endian_test = {{ 'l', '?', '?', 'b' } };
#define AEP_ENDIANNESS ((char)endian_test.mylong)


typedef unsigned long long uint_64;
typedef unsigned int uint_32;  
typedef unsigned short uint_16;

//命令解析响应码
#define AEP_CMD_SUCCESS 0						//执行成功
#define AEP_CMD_FAILED 1						//执行失败
#define AEP_CMD_INVALID_DATASET_TYPE 2			//无效数据集类型
#define AEP_CMD_INVALID_DATASET_IDENTIFIER 3	//无效数据集标识
#define AEP_CMD_PAYLOAD_PARSING_FAILED 4		//指令数据集Payload解析失败,紧凑二进制编码内容长度不符等


typedef struct AepStrStruct
{
	unsigned short len;
	char* str;
} AepString;
typedef AepString AepBytes;

//无符号整型16位  
uint_16 aep_htons(uint_16 source);

//无符号整型32位
uint_32 aep_htoni(uint_32 source);

//无符号整型64位
uint_64 aep_htonl(uint_64 source);

//float
float aep_htonf(float source);

//double
double aep_htond(double source);

//16进制转字符串
void HexToStr(char *pbDest, char *pbSrc, int nLen);

//字符串转16进制
void StrToHex(char *pbDest, char *pbSrc, int nLen);


//根据服务id生成上报数据的十六进制字符串,srcStruct需要根据服务定义传入对应的类型,返回结果为字符串
AepString codeDataReportByIdToStr(int serviceId, void * srcStruct);

//根据服务id生成上报数据的字节流,srcStruct需要根据服务定义传入对应的类型,返回结果为字节流
AepBytes codeDataReportByIdToBytes(int serviceId, void * srcStruct);

//根据服务标识生成上报数据的十六进制字符串,srcStruct需要根据服务定义传入对应的类型,返回结果为字符串
AepString codeDataReportByIdentifierToStr(char * serviceIdentifier, void * srcStruct);

//根据服务标识生成上报数据的字节流,srcStruct需要根据服务定义传入对应的类型,返回结果为字节流
AepBytes codeDataReportByIdentifierToBytes(char * serviceIdentifier, void * srcStruct);

//指令解析返回结构体，data在使用时需要根据serviceId强转为对应类型
typedef struct CmdStruct 
{
	char* serviceIdentifier;
	unsigned short taskId;
	void * data;
	int code;
} AepCmdData;
//解析接受到的报文数据,入参为十六进制字符串
AepCmdData decodeCmdDownFromStr(char* source);
//解析接受到的报文数据,入参为原始字节流
AepCmdData decodeCmdDownFromBytes(char* source, int len);



typedef struct battery_voltage_low_alarmStruct 
{
	char battery_state;
	float battery_voltage;
} battery_voltage_low_alarm;
//事件上报:电池低电压告警
AepString battery_voltage_low_alarm_CodeEventReport (battery_voltage_low_alarm srcStruct);


typedef struct error_code_reportStruct 
{
	char error_code;
} error_code_report;
//事件上报:故障上报
AepString error_code_report_CodeEventReport (error_code_report srcStruct);


typedef struct gps_reportStruct 
{
	double longitude;
	double latitude;
	char report_type;
} gps_report;
extern gps_report gps_report_struct;
//数据上报:上报gps
AepString gps_report_CodeDataReport (gps_report srcStruct);


typedef struct push_weatherStruct 
{
	AepString weather;
} push_weather;
//指令下发:push_weather
int push_weather_DecodeCmdDown (char* source, push_weather* dest);


typedef struct send_picStruct 
{
	char pic1[100];
	char pic_idx;
} send_pic;
//指令下发:发送图片
int send_pic_DecodeCmdDown (char* source, send_pic* dest);


typedef struct send_pic_respStruct 
{
	char lost_idxs[50];
} send_pic_resp;
//数据上报:send_pic_resp
AepString send_pic_resp_CodeDataReport (send_pic_resp srcStruct);


typedef struct signal_reportStruct 
{
	int rsrp;
	int sinr;
	int pci;
	int ecl;
	int cell_id;
} signal_report;
//数据上报:信号数据上报
AepString signal_report_CodeDataReport (signal_report srcStruct);


typedef struct test_cmdStruct 
{
	char test_int;
	char test_bin[4];
} test_cmd;
//指令下发:test_cmd
int test_cmd_DecodeCmdDown (char* source, test_cmd* dest);


typedef struct track_modeStruct 
{
	char act_result;
} track_mode;
//指令下发:追踪模式
int track_mode_DecodeCmdDown (char* source, track_mode* dest);



#endif
