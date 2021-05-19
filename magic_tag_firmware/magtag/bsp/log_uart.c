#include <string.h>
#include <stdbool.h>

#include "log_uart.h"
#include "os_soft_timer.h"
#include "TinyLog.h"
#include "gps_parse.h"

//static time_handle_t log_uart_time_handle;
static log_uart_buf_t log_uart_buf;
static bool log_uart_flag = false;
/*****************************************/
extern uint16_t point1;	// gps 计数
extern gps_save_data_t gps_save_data;
extern char GPS_RX_BUF[USART_REC_LEN];
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
uint16_t USART_RX_STA = 0; //接收状态标记

//static void log_uart_timout_cb(void *args);
static HAL_StatusTypeDef log_uart_Init(void *m_uart, uint32_t m_BaudRate);
static uint32_t log_uart_write(uint8_t *buf, uint32_t len);

extern void UART_SetConfig(UART_HandleTypeDef *huart);

static HAL_StatusTypeDef log_uart_Init(void *m_uart, uint32_t m_BaudRate)
{
	static UART_HandleTypeDef m_huart;

	m_huart.Instance = (USART_TypeDef *)m_uart;
	m_huart.Init.BaudRate = m_BaudRate;
	m_huart.Init.WordLength = UART_WORDLENGTH_8B;
	m_huart.Init.StopBits = UART_STOPBITS_1;
	m_huart.Init.Parity = UART_PARITY_NONE;
	m_huart.Init.Mode = UART_MODE_TX_RX;
	m_huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	m_huart.Init.OverSampling = UART_OVERSAMPLING_16;

	/* Init the low level hardware : GPIO, CLOCK */
	HAL_UART_MspInit(&m_huart);

	/* Disable the Peripheral */
	__HAL_UART_DISABLE(&m_huart);

	/* Set the UART Communication parameters */
	UART_SetConfig(&m_huart);

	/* In asynchronous mode, the following bits must be kept cleared:
  - LINEN and CLKEN bits in the USART_CR2 register,
  - SCEN, HDSEL and IREN  bits in the USART_CR3 register.*/
	CLEAR_BIT(m_huart.Instance->CR2, (USART_CR2_LINEN | USART_CR2_CLKEN));
	CLEAR_BIT(m_huart.Instance->CR3, (USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN));

	/* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
	//SET_BIT(m_huart.Instance->CR3, USART_CR3_EIE);

	/* Enable the PEIE and RXNEIE */
	SET_BIT(m_huart.Instance->CR1, USART_CR1_RXNEIE);

	/* Enable the Peripheral */
	__HAL_UART_ENABLE(&m_huart);

	return HAL_OK;
}

//static void log_uart_timout_cb(void *args)
//{
//	//printf("%s\r\n",log_uart_buf.buf);
//	log_uart_flag = true;
//
//}

extern void ty_printf_init(void *write_cb);

void log_uart_init(uint32_t m_BaudRate)
{
	log_uart_Init(USART2, m_BaudRate);
	ty_printf_init(log_uart_write);
	//log_uart_time_handle = soft_timer_register(OS_ONE_SHOT_MODE, log_uart_timout_cb, NULL);
}

static uint32_t log_uart_write(uint8_t *buf, uint32_t len)
{
	for (uint32_t i = 0; i < len; i++)
	{
		USART2->DR = buf[i];
		while (!((USART2->SR & UART_FLAG_TXE) == UART_FLAG_TXE))
		{
		};
	}

	return len;
}

uint32_t log_uart_read(void *buf, uint32_t len)
{
	uint32_t cnt;

	if (buf == NULL)
		return 0;

	if (log_uart_flag == false)
		return 0;

	cnt = log_uart_buf.count;

	memcpy(buf, log_uart_buf.buf, log_uart_buf.count);

	log_uart_buf.count = 0;
	memset(log_uart_buf.buf, 0, cnt);
	log_uart_flag = false;

	return cnt;
}

void USART2_IRQHandler(void)
{
	uint16_t uhdata;
  	uint8_t Res;
	// os_log("in usart2\r\n");
	if ((USART2->SR & USART_SR_RXNE) == USART_SR_RXNE)
	{
		/* Clear RXNE interrupt flag */
		USART2->SR &= ~USART_SR_RXNE;

		uhdata = (uint16_t)READ_REG(USART2->DR);
    Res = (uint8_t)(uhdata & 0xff);
		log_uart_buf.buf[log_uart_buf.count] = (uint8_t)(uhdata & 0xff);

		// 处理GPS
		if(Res == '$'){
			point1 = 0;
		}
		
		GPS_RX_BUF[point1++] = Res;
    if (GPS_RX_BUF[0] == '$' && GPS_RX_BUF[4] == 'M' && GPS_RX_BUF[5] == 'C') //确定是否收到"GPRMC/GNRMC"这一帧数据
		{
			if (Res == '\n')
			{
				memset(gps_save_data.GPS_Buffer, 0, GPS_Buffer_Length); //清空
				memcpy(gps_save_data.GPS_Buffer, GPS_RX_BUF, point1); //保存数据
				gps_save_data.isGetData = true;
				point1 = 0;
				memset(GPS_RX_BUF, 0, USART_REC_LEN); //清空
			}
		}

		if (point1 >= USART_REC_LEN)
		{
			point1 = USART_REC_LEN;
		}

// ***********************************************************************************
		if (log_uart_buf.buf[log_uart_buf.count] == '\n') //Enter
		{
			if (log_uart_buf.count == 0)
			{
				log_uart_buf.buf[log_uart_buf.count] = '\0';
			}
			else
			{
				if (log_uart_buf.buf[log_uart_buf.count - 1] == '\r')
				{
					log_uart_buf.buf[log_uart_buf.count - 1] = '\0';
				}
				else
				{
					log_uart_buf.buf[log_uart_buf.count] = '\0';
				}
			}

			if (log_uart_buf.buf[0] != 0)
			{
				log_uart_flag = true;
			}
		}
		else if (log_uart_buf.buf[log_uart_buf.count] == 0x08) //Backspace
		{
			if (log_uart_buf.count > 0)
			{
				log_uart_buf.count--;
			}

			return;
		}

		if (++log_uart_buf.count >= LOG_UART_BUFF_LEN)
		{
			log_uart_buf.count = 0;
		}
	}
}
