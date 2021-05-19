/***
*AepServiceCodes.h - �������������ݵĽṹ�壬�����ṩ��װ���б��ĵĺ����ͽ������б��ĵĺ���
*
*Purpose:
*	1.���ݽṹ��������ƽ̨����ķ����ʶһ��
*	2.codeDataReportByIdToStr��codeDataReportByIdToBytes��codeDataReportByIdentifierToStr��codeDataReportByIdentifierToBytesΪ��װ�ϱ����ݵĺ���������˵��������ǰ��ע��
*	3.decodeCmdDownFromStr��decodeCmdDownFromBytesΪ����ƽ̨���͹������ݵĺ���������˵��������ǰ��ע��
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

//���������Ӧ��
#define AEP_CMD_SUCCESS 0						//ִ�гɹ�
#define AEP_CMD_FAILED 1						//ִ��ʧ��
#define AEP_CMD_INVALID_DATASET_TYPE 2			//��Ч���ݼ�����
#define AEP_CMD_INVALID_DATASET_IDENTIFIER 3	//��Ч���ݼ���ʶ
#define AEP_CMD_PAYLOAD_PARSING_FAILED 4		//ָ�����ݼ�Payload����ʧ��,���ն����Ʊ������ݳ��Ȳ�����


typedef struct AepStrStruct
{
	unsigned short len;
	char* str;
} AepString;
typedef AepString AepBytes;

//�޷�������16λ  
uint_16 aep_htons(uint_16 source);

//�޷�������32λ
uint_32 aep_htoni(uint_32 source);

//�޷�������64λ
uint_64 aep_htonl(uint_64 source);

//float
float aep_htonf(float source);

//double
double aep_htond(double source);

//16����ת�ַ���
void HexToStr(char *pbDest, char *pbSrc, int nLen);

//�ַ���ת16����
void StrToHex(char *pbDest, char *pbSrc, int nLen);


//���ݷ���id�����ϱ����ݵ�ʮ�������ַ���,srcStruct��Ҫ���ݷ����崫���Ӧ������,���ؽ��Ϊ�ַ���
AepString codeDataReportByIdToStr(int serviceId, void * srcStruct);

//���ݷ���id�����ϱ����ݵ��ֽ���,srcStruct��Ҫ���ݷ����崫���Ӧ������,���ؽ��Ϊ�ֽ���
AepBytes codeDataReportByIdToBytes(int serviceId, void * srcStruct);

//���ݷ����ʶ�����ϱ����ݵ�ʮ�������ַ���,srcStruct��Ҫ���ݷ����崫���Ӧ������,���ؽ��Ϊ�ַ���
AepString codeDataReportByIdentifierToStr(char * serviceIdentifier, void * srcStruct);

//���ݷ����ʶ�����ϱ����ݵ��ֽ���,srcStruct��Ҫ���ݷ����崫���Ӧ������,���ؽ��Ϊ�ֽ���
AepBytes codeDataReportByIdentifierToBytes(char * serviceIdentifier, void * srcStruct);

//ָ��������ؽṹ�壬data��ʹ��ʱ��Ҫ����serviceIdǿתΪ��Ӧ����
typedef struct CmdStruct 
{
	char* serviceIdentifier;
	unsigned short taskId;
	void * data;
	int code;
} AepCmdData;
//�������ܵ��ı�������,���Ϊʮ�������ַ���
AepCmdData decodeCmdDownFromStr(char* source);
//�������ܵ��ı�������,���Ϊԭʼ�ֽ���
AepCmdData decodeCmdDownFromBytes(char* source, int len);



typedef struct battery_voltage_low_alarmStruct 
{
	char battery_state;
	float battery_voltage;
} battery_voltage_low_alarm;
//�¼��ϱ�:��ص͵�ѹ�澯
AepString battery_voltage_low_alarm_CodeEventReport (battery_voltage_low_alarm srcStruct);


typedef struct error_code_reportStruct 
{
	char error_code;
} error_code_report;
//�¼��ϱ�:�����ϱ�
AepString error_code_report_CodeEventReport (error_code_report srcStruct);


typedef struct gps_reportStruct 
{
	double longitude;
	double latitude;
	char report_type;
} gps_report;
extern gps_report gps_report_struct;
//�����ϱ�:�ϱ�gps
AepString gps_report_CodeDataReport (gps_report srcStruct);


typedef struct push_weatherStruct 
{
	AepString weather;
} push_weather;
//ָ���·�:push_weather
int push_weather_DecodeCmdDown (char* source, push_weather* dest);


typedef struct send_picStruct 
{
	char pic1[100];
	char pic_idx;
} send_pic;
//ָ���·�:����ͼƬ
int send_pic_DecodeCmdDown (char* source, send_pic* dest);


typedef struct send_pic_respStruct 
{
	char lost_idxs[50];
} send_pic_resp;
//�����ϱ�:send_pic_resp
AepString send_pic_resp_CodeDataReport (send_pic_resp srcStruct);


typedef struct signal_reportStruct 
{
	int rsrp;
	int sinr;
	int pci;
	int ecl;
	int cell_id;
} signal_report;
//�����ϱ�:�ź������ϱ�
AepString signal_report_CodeDataReport (signal_report srcStruct);


typedef struct test_cmdStruct 
{
	char test_int;
	char test_bin[4];
} test_cmd;
//ָ���·�:test_cmd
int test_cmd_DecodeCmdDown (char* source, test_cmd* dest);


typedef struct track_modeStruct 
{
	char act_result;
} track_mode;
//ָ���·�:׷��ģʽ
int track_mode_DecodeCmdDown (char* source, track_mode* dest);



#endif
