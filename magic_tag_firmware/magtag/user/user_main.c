#include <string.h>
#include <stdint.h>

#include "bsp.h"
#include "thread.h"
#include "bsp_thread.h"
#include "nb_thread.h"
#include "app_aep_profile.h"
#include "report_thread.h"
#include "epd_w21.h"
#include "stm32f103xb.h"
/*************************************variable********************/

// 用户主函数
void user_main(void)
{
	/*********** log init ****************/
	os_log_init();
	// gps_uart_init(115200);
	
	/************ platform init **********/
	platform_init();
	
	/*********** bsp init ****************/
	bsp_init();
	
	
	/************ screen init ***********/
	EpdInitFull();
	delay_us(300);
	// EpdDisFull((unsigned char *) m, 0);
	EpdDisFull((unsigned char *) getCurInkBuffer(),1);
	/************ thread-os start *******/
  thread_os_start();
}

// 用户线程
void app_thread_init(void)
{
	bsp_thread_init();
	
	nb_thread_init();
	image_buffer_init();
	// app_profile_thread_init();
	report_thread_init();
}








