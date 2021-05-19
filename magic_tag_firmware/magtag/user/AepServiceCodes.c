#include <ctype.h>
#include "AepServiceCodes.h"

gps_report gps_report_struct;

//�޷�������16λ  
uint_16 aep_htons(uint_16 source)  
{  

	if(AEP_ENDIANNESS == AEP_BIG_ENDIAN)
		return source;
	else
		return (uint_16)( 0
		| ((source & 0x00ff) << 8)
		| ((source & 0xff00) >> 8) );  
}  

//�޷�������32λ
uint_32 aep_htoni(uint_32 source)  
{  
	if(AEP_ENDIANNESS == AEP_BIG_ENDIAN)
		return source;
	else
		return 0
		| ((source & 0x000000ff) << 24)
		| ((source & 0x0000ff00) << 8)
		| ((source & 0x00ff0000) >> 8)
		| ((source & 0xff000000) >> 24);  
}

//�޷�������64λ
uint_64 aep_htonl(uint_64 source)  
{  
	if(AEP_ENDIANNESS == AEP_BIG_ENDIAN)
		return source;
	else
		return 0
		| ((source & (uint_64)(0x00000000000000ff)) << 56)
		| ((source & (uint_64)(0x000000000000ff00)) << 40)
		| ((source & (uint_64)(0x0000000000ff0000)) << 24)
		| ((source & (uint_64)(0x00000000ff000000)) << 8)
		| ((source & (uint_64)(0x000000ff00000000)) >> 8)
		| ((source & (uint_64)(0x0000ff0000000000)) >> 24)
		| ((source & (uint_64)(0x00ff000000000000)) >> 40)
		| ((source & (uint_64)(0xff00000000000000)) >> 56);
}

//float
float aep_htonf(float source)  
{  
	if(AEP_ENDIANNESS == AEP_BIG_ENDIAN)
		return source;
	else
	{
		uint_32 t= 0
			| ((*(uint_32*)&source & 0x000000ff) << 24)
			| ((*(uint_32*)&source & 0x0000ff00) << 8)
			| ((*(uint_32*)&source & 0x00ff0000) >> 8)
			| ((*(uint_32*)&source & 0xff000000) >> 24);
		return *(float*)&t;
	} 
}

//double
double aep_htond(double source)  
{  
	if(AEP_ENDIANNESS == AEP_BIG_ENDIAN)
		return source;
	else
	{
		uint_64 t= 0
			| ((*(uint_64*)&source & (uint_64)(0x00000000000000ff)) << 56)
			| ((*(uint_64*)&source & (uint_64)(0x000000000000ff00)) << 40)
			| ((*(uint_64*)&source & (uint_64)(0x0000000000ff0000)) << 24)
			| ((*(uint_64*)&source & (uint_64)(0x00000000ff000000)) << 8)
			| ((*(uint_64*)&source & (uint_64)(0x000000ff00000000)) >> 8)
			| ((*(uint_64*)&source & (uint_64)(0x0000ff0000000000)) >> 24)
			| ((*(uint_64*)&source & (uint_64)(0x00ff000000000000)) >> 40)
			| ((*(uint_64*)&source & (uint_64)(0xff00000000000000)) >> 56);
		return *(double*)&t;
	}
}

//16����ת�ַ���
void HexToStr(char *pbDest, char *pbSrc, int nLen)
{
	unsigned char ddl,ddh;
	int i;

	for (i=0; i<nLen; i++)
	{
		ddh = 48 + (unsigned char)pbSrc[i] / 16;
		ddl = 48 + (unsigned char)pbSrc[i] % 16;
		if (ddh > 57) ddh = ddh + 7;
		if (ddl > 57) ddl = ddl + 7;
		pbDest[i*2] = ddh;
		pbDest[i*2+1] = ddl;
	}

	//pbDest[nLen*2] = '\0';
}

//�ַ���ת16����
void StrToHex(char *pbDest, char *pbSrc, int nLen)
{
	unsigned char h1,h2;
	unsigned char s1,s2;
	int i;

	for (i=0; i<nLen; i++)
	{
		h1 = pbSrc[2*i];
		h2 = pbSrc[2*i+1];

		s1 = toupper(h1) - 0x30;
		if (s1 > 9) 
			s1 -= 7;

		s2 = toupper(h2) - 0x30;
		if (s2 > 9) 
			s2 -= 7;

		pbDest[i] = s1*16 + s2;
	}
}

//�¼��ϱ�:��ص͵�ѹ�澯
AepString battery_voltage_low_alarm_CodeEventReport (battery_voltage_low_alarm srcStruct)
{
	char* index;
	AepString resultStruct;
	unsigned short tempLen;

	unsigned short payloadLen = 5;
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)malloc(resultStruct.len + 1);
	memset(resultStruct.str, 0, resultStruct.len + 1);

	srcStruct.battery_voltage = aep_htonf(srcStruct.battery_voltage);

	index = resultStruct.str;

	memcpy(index, "07", 2);
	index += 1 * 2;

	tempLen = aep_htons(1002);//����ID
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	HexToStr(index, (char *)&srcStruct.battery_state, 1);
	index += 1 * 2;

	HexToStr(index, (char *)&srcStruct.battery_voltage, 4);
	index += 4 * 2;


	return resultStruct;
}

//�¼��ϱ�:�����ϱ�
AepString error_code_report_CodeEventReport (error_code_report srcStruct)
{
	char* index;
	AepString resultStruct;
	unsigned short tempLen;

	unsigned short payloadLen = 1;
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)malloc(resultStruct.len + 1);
	memset(resultStruct.str, 0, resultStruct.len + 1);


	index = resultStruct.str;

	memcpy(index, "07", 2);
	index += 1 * 2;

	tempLen = aep_htons(1001);//����ID
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	HexToStr(index, (char *)&srcStruct.error_code, 1);
	index += 1 * 2;


	return resultStruct;
}

//�����ϱ�:�ϱ�gps
AepString gps_report_CodeDataReport (gps_report srcStruct)
{
	char* index;
	AepString resultStruct;
	unsigned short tempLen;

	unsigned short payloadLen = 17;
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)malloc(resultStruct.len + 1);
	memset(resultStruct.str, 0, resultStruct.len + 1);

	srcStruct.longitude = aep_htond(srcStruct.longitude);
	srcStruct.latitude = aep_htond(srcStruct.latitude);

	index = resultStruct.str;

	memcpy(index, "02", 2);
	index += 1 * 2;

	tempLen = aep_htons(101);//����ID
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	HexToStr(index, (char *)&srcStruct.longitude, 8);
	index += 8 * 2;

	HexToStr(index, (char *)&srcStruct.latitude, 8);
	index += 8 * 2;

	HexToStr(index, (char *)&srcStruct.report_type, 1);
	index += 1 * 2;


	return resultStruct;
}

//ָ���·�:push_weather
int push_weather_DecodeCmdDown (char* source, push_weather* dest)
{
	char* index = source;
	int srcStrLen = strlen(source);
	int len = 0;


	memset(dest, 0, sizeof(push_weather));

	StrToHex((char *)&dest->weather.len, index, 2);
	dest->weather.len = aep_htons(dest->weather.len);
	index += 2 * 2;
	if ((len + dest->weather.len) * 2 > srcStrLen)
	{
		return AEP_CMD_PAYLOAD_PARSING_FAILED;
	}
	len += dest->weather.len + 2;

	dest->weather.str = malloc(dest->weather.len);
	StrToHex(dest->weather.str, index, dest->weather.len);
	index += dest->weather.len * 2;



	if (len * 2 > srcStrLen)
	{
		return AEP_CMD_PAYLOAD_PARSING_FAILED;
	}
	return AEP_CMD_SUCCESS;
}

//ָ���·�:����ͼƬ
int send_pic_DecodeCmdDown (char* source, send_pic* dest)
{
	char* index = source;
	int srcStrLen = strlen(source);
	int len = 101;


	memset(dest, 0, sizeof(send_pic));

	StrToHex(dest->pic1, index, 100);
	index += 100 * 2;

	StrToHex((char *)&dest->pic_idx, index, 1);
	index += 1 * 2;



	if (len * 2 > srcStrLen)
	{
		return AEP_CMD_PAYLOAD_PARSING_FAILED;
	}
	return AEP_CMD_SUCCESS;
}

//�����ϱ�:send_pic_resp
AepString send_pic_resp_CodeDataReport (send_pic_resp srcStruct)
{
	char* index;
	AepString resultStruct;
	unsigned short tempLen;

	unsigned short payloadLen = 50;
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)malloc(resultStruct.len + 1);
	memset(resultStruct.str, 0, resultStruct.len + 1);


	index = resultStruct.str;

	memcpy(index, "02", 2);
	index += 1 * 2;

	tempLen = aep_htons(50);//����ID
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	HexToStr(index, (char *)srcStruct.lost_idxs, 50);
	index += 50 * 2;


	return resultStruct;
}

//�����ϱ�:�ź������ϱ�
AepString signal_report_CodeDataReport (signal_report srcStruct)
{
	char* index;
	AepString resultStruct;
	unsigned short tempLen;

	unsigned short payloadLen = 20;
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)malloc(resultStruct.len + 1);
	memset(resultStruct.str, 0, resultStruct.len + 1);

	srcStruct.rsrp = aep_htoni(srcStruct.rsrp);
	srcStruct.sinr = aep_htoni(srcStruct.sinr);
	srcStruct.pci = aep_htoni(srcStruct.pci);
	srcStruct.ecl = aep_htoni(srcStruct.ecl);
	srcStruct.cell_id = aep_htoni(srcStruct.cell_id);

	index = resultStruct.str;

	memcpy(index, "02", 2);
	index += 1 * 2;

	tempLen = aep_htons(2);//����ID
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	HexToStr(index, (char *)&srcStruct.rsrp, 4);
	index += 4 * 2;

	HexToStr(index, (char *)&srcStruct.sinr, 4);
	index += 4 * 2;

	HexToStr(index, (char *)&srcStruct.pci, 4);
	index += 4 * 2;

	HexToStr(index, (char *)&srcStruct.ecl, 4);
	index += 4 * 2;

	HexToStr(index, (char *)&srcStruct.cell_id, 4);
	index += 4 * 2;


	return resultStruct;
}

//ָ���·�:test_cmd
int test_cmd_DecodeCmdDown (char* source, test_cmd* dest)
{
	char* index = source;
	int srcStrLen = strlen(source);
	int len = 5;


	memset(dest, 0, sizeof(test_cmd));

	StrToHex((char *)&dest->test_int, index, 1);
	index += 1 * 2;

	StrToHex(dest->test_bin, index, 4);
	index += 4 * 2;



	if (len * 2 > srcStrLen)
	{
		return AEP_CMD_PAYLOAD_PARSING_FAILED;
	}
	return AEP_CMD_SUCCESS;
}

//ָ���·�:׷��ģʽ
int track_mode_DecodeCmdDown (char* source, track_mode* dest)
{
	char* index = source;
	int srcStrLen = strlen(source);
	int len = 1;


	memset(dest, 0, sizeof(track_mode));

	StrToHex((char *)&dest->act_result, index, 1);
	index += 1 * 2;



	if (len * 2 > srcStrLen)
	{
		return AEP_CMD_PAYLOAD_PARSING_FAILED;
	}
	return AEP_CMD_SUCCESS;
}

AepCmdData decodeCmdDownFromStr(char* source)
{
	char* index;
	AepCmdData result;
	char cmdType;
	unsigned short serviceId;
	unsigned short payloadLen;

	memset(&result, 0, sizeof(AepCmdData));

	index = source;

	//����ָ������
	StrToHex(&cmdType, index, 1);
	index += 1 * 2;
	if (cmdType != 0x06)
	{
		result.code = AEP_CMD_INVALID_DATASET_TYPE;
	}

	//����Id����
	StrToHex((char *)&serviceId, index, 2);
	serviceId = aep_htons(serviceId);
	index += 2 * 2;

	StrToHex((char *)&result.taskId, index, 2);
	result.taskId = aep_htons(result.taskId);
	index += 2 * 2;

	//payload���Ƚ���
	StrToHex((char *)&payloadLen, index, 2);
	payloadLen = aep_htons(payloadLen);
	index += 2 * 2;

	if (strlen(index) < payloadLen * 2)
	{
		result.code = AEP_CMD_PAYLOAD_PARSING_FAILED;
		return result;
	}


	if (serviceId == 8005)
	{
		result.serviceIdentifier = "push_weather";
		result.data = malloc(sizeof(push_weather));
		memset(result.data, 0, sizeof(push_weather));
		result.code = push_weather_DecodeCmdDown(index, (push_weather*)result.data);
	}
	else if (serviceId == 8001)
	{
		result.serviceIdentifier = "send_pic";
		result.data = malloc(sizeof(send_pic));
		memset(result.data, 0, sizeof(send_pic));
		result.code = send_pic_DecodeCmdDown(index, (send_pic*)result.data);
	}
	else if (serviceId == 8999)
	{
		result.serviceIdentifier = "test_cmd";
		result.data = malloc(sizeof(test_cmd));
		memset(result.data, 0, sizeof(test_cmd));
		result.code = test_cmd_DecodeCmdDown(index, (test_cmd*)result.data);
	}
	else if (serviceId == 8008)
	{
		result.serviceIdentifier = "track_mode";
		result.data = malloc(sizeof(track_mode));
		memset(result.data, 0, sizeof(track_mode));
		result.code = track_mode_DecodeCmdDown(index, (track_mode*)result.data);
	}
	else 
	{
		result.serviceIdentifier = NULL;
		result.data = malloc(payloadLen);
		memset(result.data, 0, sizeof(payloadLen));
		StrToHex((char *)result.data, index, payloadLen);
		result.code = AEP_CMD_INVALID_DATASET_IDENTIFIER;
	}

	return result;
}

AepCmdData decodeCmdDownFromBytes(char* source, int len)
{
	char * str = malloc(len * 2 + 1);
	AepCmdData result;
	HexToStr(str, source, len);
	str[len * 2] = 0;
	
	result = decodeCmdDownFromStr(str);
	free(str);
	return result;
}

AepString codeDataReportByIdToStr (int serviceId, void * srcStruct)
{
	if (serviceId == 1002)
	{
		return battery_voltage_low_alarm_CodeEventReport(*(battery_voltage_low_alarm*)srcStruct);
	}
	else if (serviceId == 1001)
	{
		return error_code_report_CodeEventReport(*(error_code_report*)srcStruct);
	}
	else if (serviceId == 101)
	{
		return gps_report_CodeDataReport(*(gps_report*)srcStruct);
	}
	else if (serviceId == 50)
	{
		return send_pic_resp_CodeDataReport(*(send_pic_resp*)srcStruct);
	}
	else if (serviceId == 2)
	{
		return signal_report_CodeDataReport(*(signal_report*)srcStruct);
	}
	else 
	{
		AepString result = {0};
		return result;
	}
}

AepBytes codeDataReportByIdToBytes(int serviceId, void * srcStruct)
{
	AepString temp = codeDataReportByIdToStr(serviceId, srcStruct);
	AepBytes result = {0};
	result.len = temp.len / 2;
	if (result.len > 0)
	{
		result.str = malloc(result.len);
		StrToHex(result.str, temp.str, result.len);
		free(temp.str);
	}
	return result;
}

AepString codeDataReportByIdentifierToStr (char* serviceIdentifier, void * srcStruct)
{
	if (strcmp(serviceIdentifier, "battery_voltage_low_alarm") == 0)
	{
		return battery_voltage_low_alarm_CodeEventReport(*(battery_voltage_low_alarm*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, "error_code_report") == 0)
	{
		return error_code_report_CodeEventReport(*(error_code_report*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, "gps_report") == 0)
	{
		return gps_report_CodeDataReport(*(gps_report*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, "send_pic_resp") == 0)
	{
		return send_pic_resp_CodeDataReport(*(send_pic_resp*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, "signal_report") == 0)
	{
		return signal_report_CodeDataReport(*(signal_report*)srcStruct);
	}
	else 
	{
		AepString result = {0};
		return result;
	}
}

AepBytes codeDataReportByIdentifierToBytes(char* serviceIdentifier, void * srcStruct)
{
	AepString temp = codeDataReportByIdentifierToStr(serviceIdentifier, srcStruct);
	AepBytes result = {0};
	result.len = temp.len / 2;
	if (result.len > 0)
	{
		result.str = malloc(result.len);
		StrToHex(result.str, temp.str, result.len);
		free(temp.str);
	}
	return result;
}

