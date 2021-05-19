#include <string.h>
#include "TinyLog.h"
#include "util.h"
#include "gps_parse.h"

#define gps_log(...)  os_log("\r\n[gps_log]");os_log(__VA_ARGS__);

char GPS_RX_BUF[USART_REC_LEN];
gps_save_data_t gps_save_data;
uint16_t point1=0;

void parseGpsBuffer()
{
	char *subString;
	char *subStringNext;
	char i = 0;
	if (gps_save_data.isGetData)
	{
		gps_save_data.isGetData = false;
		gps_log("++++parseGpsBuffer+++++++++++\r\n");
		gps_log(gps_save_data.GPS_Buffer);

		
		for (i = 0 ; i <= 6 ; i++)
		{
			if (i == 0)
			{
				if ((subString = strstr(gps_save_data.GPS_Buffer, ",")) == NULL)
					gps_log("err: 1\n");	//解析错
			}
			else
			{
				subString++;
				if ((subStringNext = strstr(subString, ",")) != NULL)
				{
					char usefullBuffer[2]; 
					switch(i)
					{
						case 1:memcpy(gps_save_data.UTCTime, subString, subStringNext - subString);break;	//获取UTC时间
						case 2:memcpy(usefullBuffer, subString, subStringNext - subString);break;	//获取UTC时间
						case 3:memcpy(gps_save_data.latitude, subString, subStringNext - subString);break;	//获取??度信??
						case 4:memcpy(gps_save_data.N_S, subString, subStringNext - subString);break;	//获取N/S
						case 5:memcpy(gps_save_data.longitude, subString, subStringNext - subString);break;	//获取经度信息
						case 6:memcpy(gps_save_data.E_W, subString, subStringNext - subString);break;	//获取E/W

						default:break;
					}

					subString = subStringNext;
					gps_save_data.isParseData = true;
					if(usefullBuffer[0] == 'A')
						gps_save_data.isUsefull = true;
					else if(usefullBuffer[0] == 'V')
						gps_save_data.isUsefull = false;

				}
				else
				{
					gps_log("err: 1\n");	//解析错
				}
			}


		}
	}
}

void printGpsBuffer()
{
	if (gps_save_data.isParseData)
	{
		gps_save_data.isParseData = false;
		
		gps_log("gps_save_data.UTCTime = ");
		gps_log(gps_save_data.UTCTime);
		gps_log("\r\n");

		if(gps_save_data.isUsefull)
		{
			gps_save_data.isUsefull = false;
			gps_log("gps_save_data.latitude = ");
			gps_log(gps_save_data.latitude);
			gps_log("\r\n");


			gps_log("gps_save_data.N_S = ");
			gps_log(gps_save_data.N_S);
			gps_log("\r\n");

			gps_log("gps_save_data.longitude = ");
			gps_log(gps_save_data.longitude);
			gps_log("\r\n");

			gps_log("gps_save_data.E_W = ");
			gps_log(gps_save_data.E_W);
			gps_log("\r\n");
		}
		else
		{
			gps_log("GPS DATA is not usefull!\r\n");
		}
		
	}
}

// 获取上报结构
void getGpsStruct(gps_report *report){
	parseGpsBuffer();
	if (gps_save_data.isParseData && gps_save_data.isUsefull){
		report->latitude = atof(gps_save_data.latitude);
		report->longitude = atof(gps_save_data.longitude);
	}else{
		report->latitude = 0;
		report->longitude = 0;
	}
}

// 转化为 hh:mm格式
void parseUTCtime(char * hhmm){
	// if(strlen(hhmm)<6)return;
	char utc_hour[3] = {0};
	parseGpsBuffer();
	if (gps_save_data.isParseData && strlen(gps_save_data.UTCTime)>6){
		strncpy(utc_hour, gps_save_data.UTCTime, 2);
		sprintf(hhmm, "%02d:%c%c\0", (atoi(utc_hour) + 8) % 24,
			gps_save_data.UTCTime[2], gps_save_data.UTCTime[3]);
	}else{
		sprintf(hhmm, "--:--\0");
	}
	gps_log("hhmm: %s, utc: %s", hhmm, gps_save_data.UTCTime);
}

void CLR_Buf(void) // 串口缓存清理
{
	memset(GPS_RX_BUF, 0, USART_REC_LEN); //清空
	point1 = 0;
}

void clrStruct()
{
	gps_save_data.isGetData = false;
	gps_save_data.isParseData = false;
	gps_save_data.isUsefull = false;
	memset(gps_save_data.GPS_Buffer, 0, GPS_Buffer_Length); //清空
	memset(gps_save_data.UTCTime, 0, UTCTime_Length);
	memset(gps_save_data.latitude, 0, latitude_Length);
	memset(gps_save_data.N_S, 0, N_S_Length);
	memset(gps_save_data.longitude, 0, longitude_Length);
	memset(gps_save_data.E_W, 0, E_W_Length);
}
