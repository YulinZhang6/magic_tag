#ifndef _DISPLAY_EPD_W21_H_
#define _DISPLAY_EPD_W21_H_

extern void DriverDelay(unsigned long xms);

extern void EPD_DisplayPart(unsigned char *Image);

extern void EPD_DisplayPartBaseImage(unsigned char *Image);

extern void EpdDisFull(unsigned char *DisBuffer, unsigned char Label);

extern void EpdInitFull(void);

extern void EpdInitPart(void);

extern void SpiWrite(unsigned char value);

// DIN	PA0
// CLK	PA1
// CS	PB9
// DC	PC13
// RST	PB5
// BUSY	PB8


#define EPD_W21_MOSI_0	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET)
#define EPD_W21_MOSI_1	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET)

#define EPD_W21_CLK_0	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET)
#define EPD_W21_CLK_1	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET)

#define EPD_W21_CS_0	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET)
#define EPD_W21_CS_1	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET)

#define EPD_W21_DC_0	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET)
#define EPD_W21_DC_1	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET)

#define EPD_W21_RST_0	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET)
#define EPD_W21_RST_1	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET)

#define EPD_W21_BUSY_LEVEL 0
#define isEPD_W21_BUSY HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8)

extern unsigned char isDialogShow;
extern unsigned char cur_ink_idx;
extern char weatherstr[];
extern unsigned char* getCurInkBuffer(void);
extern void switchInkBuffer(void);
extern void image_buffer_init(void);
extern void restoreImg();
#endif
/***********************************************************
						end file
***********************************************************/


