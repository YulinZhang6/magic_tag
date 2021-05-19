#ifndef __GPS_PARSE_H
#define __GPS_PARSE_H

//�������鳤��
#define USART_REC_LEN   200
#define GPS_Buffer_Length 80
#define UTCTime_Length 11
#define latitude_Length 11
#define N_S_Length 2
#define longitude_Length 12
#define E_W_Length 2 

#include "AepServiceCodes.h"

typedef struct 
{
	char GPS_Buffer[GPS_Buffer_Length];
	char isGetData;		//�Ƿ��ȡ��GPS����
	char isParseData;	//�Ƿ�������
	char UTCTime[UTCTime_Length];		//UTCʱ��
	char latitude[latitude_Length];		//γ��
	char N_S[N_S_Length];		//N/S
	char longitude[longitude_Length];		//����
	char E_W[E_W_Length];		//E/W
	char isUsefull;		//��λ��Ϣ�Ƿ���Ч
} gps_save_data_t;



void CLR_Buf(void);
void clrStruct(void);
void parseGpsBuffer(void);
void printGpsBuffer(void);
void getGpsStruct(gps_report*);
void parseUTCtime(char * hhmm);


#endif
