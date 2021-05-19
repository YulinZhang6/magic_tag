#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

//RTOS
#include "thread.h"
#include "bsp.h"
#include "util.h"
#include "nb_thread.h"
#include "AepServicecodes.h"
#include "app_aep_profile.h"
#include "gps_parse.h"
/**************************** define ****************************************/

#define app_log(...)         \
	os_log("\r\n[app_log]"); \
	os_log(__VA_ARGS__);

/*********************************vriable*************************************/

THREAD_INIT(report_process, "auto report Thread");
THREAD_INIT(track_process, "track mode report Thread");
THREAD_INIT(press_key_process, "press key Thread");
// USER varibale
static BSP_status_t BSP_status;

static unsigned int reportPeriod = 3000;
static bool reportFlag = false;

// 追踪模式开关
unsigned char track_mode_on = 0;

/******************************** functions ********************************/

/**********************************************************************/
// 业务线程
void report_thread_init(void)
{
	thread_os_register(&report_process, NULL);
	thread_os_register(&press_key_process, NULL);
	thread_os_register(&track_process, NULL);
	track_mode_on = 0;
	nb_register_syn_ack(send_data_asy_callback);
	app_log("\r\nregister report thread.......[OK]\r\n");
}

// 定时上报
PROCESS_THREAD(report_process, ev, pdata)
{
	THREAD_BEGIN()

	while (1)
	{
		if(!track_mode_on){
			report_gps(0);
		}
		THREAD_OS_DELAY(1000 * 60 * 5);
	}

	THREAD_END()
}

// 追踪模式上报
PROCESS_THREAD(track_process, ev, pdata)
{
	THREAD_BEGIN()

	while (1)
	{
		if(track_mode_on){
			setStatus(LED0_DEVICE_ID, LED_PURPLE);
			report_gps(4);
			THREAD_OS_DELAY(1000 * 3);
			setStatus(LED0_DEVICE_ID, LED_OFF);
			THREAD_OS_DELAY(1000 * 2);
		}else{
			THREAD_OS_DELAY(1000 * 5);	// 5 秒
		}
		
	}

	THREAD_END()
}

// 按钮功能
PROCESS_THREAD(press_key_process, ev, pdata)
{
	THREAD_BEGIN()

	while (1)
	{
		// key1 短按切图，长按显示弹窗
		getStatus(KEY1, &BSP_status);
		if (BSP_status.key_value == 0)
		{
			THREAD_OS_DELAY(500);

			getStatus(KEY1, &BSP_status);
			if (BSP_status.key_value == 0) // 长按 显示弹窗
			{
				os_log("key1 pull down PAINT!\r\n");
				paintDialog();
				do // 抬起再退出
				{
					getStatus(KEY1, &BSP_status);
					if (BSP_status.key_value == 1)
					{
						break;
					}
					THREAD_OS_DELAY(30);
				} while (1);
			}
			else
			{ //短按 切屏
				os_log("key1 pull down switchInkBuffer!\r\n");
				switchInkBuffer();
				do
				{
					getStatus(KEY1, &BSP_status);
					if (BSP_status.key_value == 1)
					{
						break;
					}
					THREAD_OS_DELAY(30);
				} while (1);
			}
		}
		THREAD_OS_DELAY(100);
		// key2 控制上报
		getStatus(KEY2, &BSP_status);
		if (BSP_status.key_value == 0)
		{
			THREAD_OS_DELAY(800);

			getStatus(KEY2, &BSP_status);
			if (BSP_status.key_value == 0) // 长按 SOS
			{
				os_log("key2 pull down SOS!\r\n");
				report_gps(1);
				setStatus(MOTOR1, MOTOR_FORWARD);
				THREAD_OS_DELAY(100);
				setStatus(MOTOR1, MOTOR_OFF);
				do // 抬起再退出
				{
					getStatus(KEY2, &BSP_status);
					if (BSP_status.key_value == 1)
					{
						break;
					}
					THREAD_OS_DELAY(30);
				} while (1);
			}
			else
			{ //短按 收藏
				os_log("key2 pull down STAR!\r\n");
				setStatus(LED1_DEVICE_ID, LED_BLUE);
				report_gps(2);
				THREAD_OS_DELAY(50);
				setStatus(LED1_DEVICE_ID, LED_OFF);
				do
				{
					getStatus(KEY2, &BSP_status);
					if (BSP_status.key_value == 1)
					{
						break;
					}
					THREAD_OS_DELAY(30);
				} while (1);
			}
		}
		THREAD_OS_DELAY(100);
	}

	THREAD_END()
}
