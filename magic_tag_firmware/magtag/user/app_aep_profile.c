#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

//RTOS
#include "thread.h"
#include "bsp.h"
#include "util.h"
#include "nb_thread.h"
#include "AepServiceCodes.h"
#include "epd_w21.h"
#include "report_thread.h"

/**************************** define ****************************************/

#define app_log(...)         \
	os_log("\r\n[app_log]"); \
	os_log(__VA_ARGS__);

/*********************************vriable*************************************/

// USER varibale
static BSP_status_t BSP_status;


static char lost_idxs[50] = {0}; // 0 表示丢失
static bool isReceiving = false;

/******************************** functions ********************************/
void send_data_asy_callback(uint8_t result, uint32_t id)
{
	app_log("send syn ack, result:%d,messageID:%d\r\n", result, id);

	switch (result)
	{
	case SEND_SYN_RSP_SUCCESS:
		break;

	case SEND_SYN_RSP_TIMEOUT:
		break;

	case SEND_SYN_RSP_SENDFAILED:
		break;

	case SEND_SYN_RSP_RESET_MESSAGE:
		break;

	case SEND_SYN_RSP_WAITING:
		break;

	case SEND_SYN_RSP_NOTSEND:
		break;

	default:
		app_log("invalid syn ack!\r\n");
		break;
	}
}

static void send_data_syn_callback(void *msg)
{
	msg_data_t *m_msg = msg;
	if (m_msg == NULL)
		return;

	if (m_msg->result == SEND_ASY_RSP_SUCCESS)
	{
		app_log("send asy ack, messageID:%d\r\n", m_msg->messageID);
	}
	else if (m_msg->result == SEND_ASY_RSP_FAIL)
	{
		if (m_msg->messageID != 0)
		{
			app_log("send error, error id:%d\r\n", m_msg->messageID);
		}
		else
		{
			app_log("send error\r\n");
		}
	}
	else if (m_msg->result == SEND_ASY_RSP_TIMEOUT)
	{
		app_log("send error, mudule ack timeout\r\n");
	}
	else if (m_msg->result == SEND_ASY_RSP_SENDING)
	{
		app_log("last msg(id=%d) in sending,not send CON or NON data\r\n", m_msg->messageID);
	}

	if (m_msg->mode == SEND_MODE_NON || m_msg->mode == SEND_MODE_NON_RELEASE || m_msg->mode == SEND_MODE_NON_RELEASE_AFTER_REPLY)
	{
	}
	else if (m_msg->mode == SEND_MODE_CON || m_msg->mode == SEND_MODE_CON_RELEASE_AFTER_REPLY)
	{
	}

	//user can save messageID
	free(m_msg);
}

static int8_t send_msg(char *data, uint32_t len, uint8_t mode)
{
	int8_t ret;
	msg_data_t *msg;

	if (data == NULL || strlen(data) != len)
	{
		app_log("nb send params error:%d,%d\r\n", strlen(data), len);
		return -1;
	}

	msg = malloc(sizeof(msg_data_t));
	if (msg != NULL)
	{
		memcpy(msg->send_buff, data, len + 1);
		msg->mode = mode;
		msg->len = len;
		msg->callback = send_data_syn_callback;

		ret = nb_send(msg);
		if (ret < 0)
		{
			free(msg);
		}
	}
	else
	{
		app_log("malloc error\r\n");
		ret = -1;
	}

	return ret;
}
/*************************************************************************************/
// 发送GPS数据 mode: 0 正常，1：sos，2收藏，3获取天气，4追踪模式
void report_gps(int8_t mode)
{
	int8_t ret;
	AepString reportstr;

	// getStatus(BSP_ALL, &BSP_status);
	getGpsStruct(&gps_report_struct);
	
	gps_report_struct.report_type = mode;
	reportstr = codeDataReportByIdentifierToStr("gps_report", &gps_report_struct);
	// app_log(reportstr.str);

	ret = send_msg(reportstr.str, reportstr.len, SEND_MODE_CON);
	if (ret < 0)
	{
		app_log("send sensor data err\r\n");
	}

	free(reportstr.str);
}

// 判断是否有丢包
bool hasLost(char *data)
{
	uint8_t i = 0;
	for (; i < 50; i++)
	{
		if (data[i] == 0)
			return true;
	}
	return false;
}

// 改,解析数据,执行业务
void decode_aep_data(char *data)
{
	int8_t ret;
	AepCmdData result;
	AepString reportstr;

	result = decodeCmdDownFromStr(data);
	app_log("\ncmd: %s\n", result.serviceIdentifier);
	setStatus(LED1_DEVICE_ID, LED_PALE_BLUE);
	if (result.code == AEP_CMD_SUCCESS)
	{
		// 发送图片
		if (strcmp(result.serviceIdentifier, "send_pic") == 0)
		{
			setStatus(LED0_DEVICE_ID, LED_BLUE);
			if (isReceiving == false)
			{
				memset(lost_idxs, 0, 50);
				isReceiving = true;
			}
			send_pic *strc = (send_pic *)(result.data);
			uint8_t pic_idx = (uint8_t)strc->pic_idx;
			app_log("pic %d\n", pic_idx);
			memcpy(getCurInkBuffer() + pic_idx * 100, strc->pic1, sizeof(strc->pic1));
			setStatus(LED0_DEVICE_ID, LED_OFF);
			lost_idxs[pic_idx] = 1;
			if (pic_idx == 49)
			{
				send_pic_resp resp;
				memcpy(resp.lost_idxs, lost_idxs, sizeof(lost_idxs));
				// resp.lost_idxs = lost_idxs;
				// 发送反馈
				if (result.data != NULL)
				{
					free(result.data);
					// free(reportstr.str);
				}
				reportstr = codeDataReportByIdentifierToStr("send_pic_resp", &resp);
				app_log(reportstr.str);
				ret = send_msg(reportstr.str, reportstr.len, SEND_MODE_CON);
				if( ret < 0 )
				{
					app_log("send ret err\r\n");
				}
				if (reportstr.str != NULL)
				{
					free(reportstr.str);
				}
				if(hasLost(lost_idxs)==false){
					// 无丢失，刷新屏幕
					EpdDisFull((unsigned char *) getCurInkBuffer(),0);
					delay_us(300);
					// EpdDisFull((unsigned char *) inkBuffer,1);
					// delay_us(200);
					// EpdDisFull((unsigned char *) inkBuffer,1);
					isReceiving = false;
				}
				EpdDisFull((unsigned char *) getCurInkBuffer(),1);
				delay_us(200);
				app_log("\nEpdDisFull\n\n");
			}
			// EpdDisPart(0, 199, 4 * pic_idx, 4 * (pic_idx + 1), strc->pic1, 1);
		}
		// 接收推送天气
		else if (strcmp(result.serviceIdentifier, "push_weather") == 0){
			push_weather *strc = (push_weather *)(result.data);
			memcpy(weatherstr, strc->weather.str, MIN(strc->weather.len, 9));
			weatherstr[MIN(strc->weather.len, 9)] = '\0';
			app_log("push_weather: %s", weatherstr);
		}
		// 追踪模式
		else if (strcmp(result.serviceIdentifier, "track_mode") == 0){
			track_mode *strc = (track_mode *)(result.data);
			track_mode_on =  strc->act_result;
			app_log("track_mode: %d", track_mode_on);
		}
	}
	// 强制类型转换
	// os_log(result.data);
	setStatus(LED1_DEVICE_ID, LED_OFF);
	if (result.data != NULL)
	{
		free(result.data);
		// free(reportstr.str);
	}
}
