#include "stm32f4xx.h"
#include "stdio.h"
#include "stdlib.h"
// device addr
#define deviceAddress 						0x41
#define deviceAddress2 						0x44
static uint8_t rotationNum=1;
//Touchscreen
#define STMPE811_SYS_CTRL1					(uint8_t)0x03U		//soft reset [1]
#define STMPE811_SYS_CTRL2					(uint8_t)0x04U		//TSC OFF [1]

#define STMPE811_ADC_CTRL1					(uint8_t)0x20U		//Sampling rate...
#define STMPE811_ADC_CTRL2					(uint8_t)0x21U		//clock set
#define STMPE811_ADC_CAPT					(uint8_t)0x22U		//Capture

#define STMPE811_INT_CTRL					(uint8_t)0x09U		//ISR Config
#define STMPE811_INT_EN						(uint8_t)0x0AU		//Enable
#define STMPE811_INT_STATUS					(uint8_t)0x0BU		//STATUS

#define STMPE811_TSC_CTRL					(uint8_t)0x40U
#define STMPE811_TSC_CFG					(uint8_t)0x41U
#define STMPE811_WDW_TR_X					(uint8_t)0x42U
#define STMPE811_WDW_TR_Y					(uint8_t)0x44U
#define STMPE811_WDW_BL_X					(uint8_t)0x46U
#define STMPE811_WDW_BL_Y					(uint8_t)0x48U

#define STMPE811_FIFO_TH					(uint8_t)0x4AU
#define STMPE811_FIFO_STA					(uint8_t)0x4BU
#define STMPE811_FIFO_SIZE					(uint8_t)0x4CU
#define STMPE811_TSC_DATA_X  				(uint8_t)0x4DU
#define STMPE811_TSC_DATA_Y					(uint8_t)0x4FU
#define STMPE811_TSC_DATA_Z					(uint8_t)0x51U
#define STMPE811_TSC_DATA_XYZ				(uint8_t)0x52U
#define STMPE811_TSC_FRACT_Z				(uint8_t)0x56U
#define STMPE811_TSC_DATA					(uint8_t)0x57U
#define STMPE811_TSC_I_DRIVE				(uint8_t)0x58U
#define STMPE811_TSC_SHIELD					(uint8_t)0x59U

// GPIO controller
#define STMPE811_GPIO_SETPIN				(uint8_t)0x10U
#define STMPE811_GPIO_CLERAPIN				(uint8_t)0x11U
#define STMPE811_GPIO_PINSTATE				(uint8_t)0x12U
#define STMPE811_GPIO_DIR					(uint8_t)0x13U
#define STMPE811_GPIO_EDGEDETECT 			(uint8_t)0x14U
#define STMPE811_GPIO_RISINGEDGE			(uint8_t)0x15U
#define STMPE811_GPIO_FALLINGEDGE			(uint8_t)0x16U
#define STMPE811_GPIO_AF					(uint8_t)0x17U

// TOUCH CONFIG X+ X- Y+ Y-
#define STMPE811_TOUCH_YD               	STMPE811_GPIO_7
#define STMPE811_TOUCH_XD               	STMPE811_GPIO_6
#define STMPE811_TOUCH_YU              		STMPE811_GPIO_5
#define STMPE811_TOUCH_XU              		STMPE811_GPIO_4
#define STMPE811_TOUCH_ALL                  (uint32_t)(STMPE811_TOUCH_YD | STMPE811_TOUCH_XD |STMPE811_TOUCH_YU | STMPE811_TOUCH_XU)

//GPIO PIN CONFIG
#define STMPE811_GPIO_0						0x01U
#define STMPE811_GPIO_1						0x02U
#define STMPE811_GPIO_2						0x04U
#define STMPE811_GPIO_3						0x08U
#define STMPE811_GPIO_4						0x10U
#define STMPE811_GPIO_5						0x20U
#define STMPE811_GPIO_6						0x40U
#define STMPE811_GPIO_7						0x80U
#define STMPE811_GPIO_ALL                	0xFFU

//240RGB x 320 ILI9341
//LCD
#define ILI9341_RESET			 		    0x01
#define ILI9341_SLEEP_OUT		  			0x11
#define ILI9341_NORMAL_MODE_ON				0x13
#define ILI9341_GAMMA			    		0x26
#define ILI9341_DISPLAY_OFF					0x28
#define ILI9341_DISPLAY_ON					0x29
#define ILI9341_COLUMN_ADDR					0x2A
#define ILI9341_PAGE_ADDR			  		0x2B
#define ILI9341_GRAM				    	0x2C
#define ILI9341_TEARING_OFF					0x34
#define ILI9341_TEARING_ON					0x35
#define ILI9341_DISPLAY_INVERSION			0xb4
#define ILI9341_MAC			        		0x36
#define ILI9341_PIXEL_FORMAT    			0x3A
#define ILI9341_WDB			    	  		0x51
#define ILI9341_WCD				      		0x53
#define ILI9341_RGB_INTERFACE   			0xB0
#define ILI9341_FRC					    	0xB1
#define ILI9341_BPC					    	0xB5
#define ILI9341_DFC				 	    	0xB6
#define ILI9341_Entry_Mode_Set				0xB7
#define ILI9341_POWER1						0xC0
#define ILI9341_POWER2						0xC1
#define ILI9341_VCOM1						0xC5
#define ILI9341_VCOM2						0xC7
#define ILI9341_POWERA						0xCB
#define ILI9341_POWERB						0xCF
#define ILI9341_PGAMMA						0xE0
#define ILI9341_NGAMMA						0xE1
#define ILI9341_DTCA						0xE8
#define ILI9341_DTCB						0xEA
#define ILI9341_POWER_SEQ					0xED
#define ILI9341_3GAMMA_EN					0xF2
#define ILI9341_INTERFACE					0xF6
#define ILI9341_PRC				   	  		0xF7
#define ILI9341_VERTICAL_SCROLL 			0x33

//READ related reg
#define ILI9341_MEMCONTROL         			0x36
#define ILI9341_MADCTL_MY  					0x80
#define ILI9341_MADCTL_MX  					0x40
#define ILI9341_MADCTL_MV  					0x20
#define ILI9341_MADCTL_ML  					0x10
#define ILI9341_MADCTL_RGB 					0x00
#define ILI9341_MADCTL_BGR 					0x08
#define ILI9341_MADCTL_MH  					0x04

const uint16_t Font_ID [] = {													 // 7 * 10
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  // sp
0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x0000, 0x1000, 0x0000, 0x0000,  // !
0x2800, 0x2800, 0x2800, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  // "
0x2400, 0x2400, 0x7C00, 0x2400, 0x4800, 0x7C00, 0x4800, 0x4800, 0x0000, 0x0000,  // #
0x3800, 0x5400, 0x5000, 0x3800, 0x1400, 0x5400, 0x5400, 0x3800, 0x1000, 0x0000,  // $
0x2000, 0x5400, 0x5800, 0x3000, 0x2800, 0x5400, 0x1400, 0x0800, 0x0000, 0x0000,  // %
0x1000, 0x2800, 0x2800, 0x1000, 0x3400, 0x4800, 0x4800, 0x3400, 0x0000, 0x0000,  // &
0x1000, 0x1000, 0x1000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  // '
0x0800, 0x1000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x1000, 0x0800,  // (
0x2000, 0x1000, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x1000, 0x2000,  // )
0x1000, 0x3800, 0x1000, 0x2800, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  // *
0x0000, 0x0000, 0x1000, 0x1000, 0x7C00, 0x1000, 0x1000, 0x0000, 0x0000, 0x0000,  // +
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1000, 0x1000, 0x1000,  // ,
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x3800, 0x0000, 0x0000, 0x0000, 0x0000,  // -
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1000, 0x0000, 0x0000,  // .
0x0800, 0x0800, 0x1000, 0x1000, 0x1000, 0x1000, 0x2000, 0x2000, 0x0000, 0x0000,  // /
0x3800, 0x4400, 0x4400, 0x5400, 0x4400, 0x4400, 0x4400, 0x3800, 0x0000, 0x0000,  // 0
0x1000, 0x3000, 0x5000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x0000, 0x0000,  // 1
0x3800, 0x4400, 0x4400, 0x0400, 0x0800, 0x1000, 0x2000, 0x7C00, 0x0000, 0x0000,  // 2
0x3800, 0x4400, 0x0400, 0x1800, 0x0400, 0x0400, 0x4400, 0x3800, 0x0000, 0x0000,  // 3
0x0800, 0x1800, 0x2800, 0x2800, 0x4800, 0x7C00, 0x0800, 0x0800, 0x0000, 0x0000,  // 4
0x7C00, 0x4000, 0x4000, 0x7800, 0x0400, 0x0400, 0x4400, 0x3800, 0x0000, 0x0000,  // 5
0x3800, 0x4400, 0x4000, 0x7800, 0x4400, 0x4400, 0x4400, 0x3800, 0x0000, 0x0000,  // 6
0x7C00, 0x0400, 0x0800, 0x1000, 0x1000, 0x2000, 0x2000, 0x2000, 0x0000, 0x0000,  // 7
0x3800, 0x4400, 0x4400, 0x3800, 0x4400, 0x4400, 0x4400, 0x3800, 0x0000, 0x0000,  // 8
0x3800, 0x4400, 0x4400, 0x4400, 0x3C00, 0x0400, 0x4400, 0x3800, 0x0000, 0x0000,  // 9
0x0000, 0x0000, 0x1000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1000, 0x0000, 0x0000,  // :
0x0000, 0x0000, 0x0000, 0x1000, 0x0000, 0x0000, 0x0000, 0x1000, 0x1000, 0x1000,  // ;
0x0000, 0x0000, 0x0C00, 0x3000, 0x4000, 0x3000, 0x0C00, 0x0000, 0x0000, 0x0000,  // <
0x0000, 0x0000, 0x0000, 0x7C00, 0x0000, 0x7C00, 0x0000, 0x0000, 0x0000, 0x0000,  // =
0x0000, 0x0000, 0x6000, 0x1800, 0x0400, 0x1800, 0x6000, 0x0000, 0x0000, 0x0000,  // >
0x3800, 0x4400, 0x0400, 0x0800, 0x1000, 0x1000, 0x0000, 0x1000, 0x0000, 0x0000,  // ?
0x3800, 0x4400, 0x4C00, 0x5400, 0x5C00, 0x4000, 0x4000, 0x3800, 0x0000, 0x0000,  // @
0x1000, 0x2800, 0x2800, 0x2800, 0x2800, 0x7C00, 0x4400, 0x4400, 0x0000, 0x0000,  // A
0x7800, 0x4400, 0x4400, 0x7800, 0x4400, 0x4400, 0x4400, 0x7800, 0x0000, 0x0000,  // B
0x3800, 0x4400, 0x4000, 0x4000, 0x4000, 0x4000, 0x4400, 0x3800, 0x0000, 0x0000,  // C
0x7000, 0x4800, 0x4400, 0x4400, 0x4400, 0x4400, 0x4800, 0x7000, 0x0000, 0x0000,  // D
0x7C00, 0x4000, 0x4000, 0x7C00, 0x4000, 0x4000, 0x4000, 0x7C00, 0x0000, 0x0000,  // E
0x7C00, 0x4000, 0x4000, 0x7800, 0x4000, 0x4000, 0x4000, 0x4000, 0x0000, 0x0000,  // F
0x3800, 0x4400, 0x4000, 0x4000, 0x5C00, 0x4400, 0x4400, 0x3800, 0x0000, 0x0000,  // G
0x4400, 0x4400, 0x4400, 0x7C00, 0x4400, 0x4400, 0x4400, 0x4400, 0x0000, 0x0000,  // H
0x3800, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x3800, 0x0000, 0x0000,  // I
0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x4400, 0x3800, 0x0000, 0x0000,  // J
0x4400, 0x4800, 0x5000, 0x6000, 0x5000, 0x4800, 0x4800, 0x4400, 0x0000, 0x0000,  // K
0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x7C00, 0x0000, 0x0000,  // L
0x4400, 0x6C00, 0x6C00, 0x5400, 0x4400, 0x4400, 0x4400, 0x4400, 0x0000, 0x0000,  // M
0x4400, 0x6400, 0x6400, 0x5400, 0x5400, 0x4C00, 0x4C00, 0x4400, 0x0000, 0x0000,  // N
0x3800, 0x4400, 0x4400, 0x4400, 0x4400, 0x4400, 0x4400, 0x3800, 0x0000, 0x0000,  // O
0x7800, 0x4400, 0x4400, 0x4400, 0x7800, 0x4000, 0x4000, 0x4000, 0x0000, 0x0000,  // P
0x3800, 0x4400, 0x4400, 0x4400, 0x4400, 0x4400, 0x5400, 0x3800, 0x0400, 0x0000,  // Q
0x7800, 0x4400, 0x4400, 0x4400, 0x7800, 0x4800, 0x4800, 0x4400, 0x0000, 0x0000,  // R
0x3800, 0x4400, 0x4000, 0x3000, 0x0800, 0x0400, 0x4400, 0x3800, 0x0000, 0x0000,  // S
0x7C00, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x0000, 0x0000,  // T
0x4400, 0x4400, 0x4400, 0x4400, 0x4400, 0x4400, 0x4400, 0x3800, 0x0000, 0x0000,  // U
0x4400, 0x4400, 0x4400, 0x2800, 0x2800, 0x2800, 0x1000, 0x1000, 0x0000, 0x0000,  // V
0x4400, 0x4400, 0x5400, 0x5400, 0x5400, 0x6C00, 0x2800, 0x2800, 0x0000, 0x0000,  // W
0x4400, 0x2800, 0x2800, 0x1000, 0x1000, 0x2800, 0x2800, 0x4400, 0x0000, 0x0000,  // X
0x4400, 0x4400, 0x2800, 0x2800, 0x1000, 0x1000, 0x1000, 0x1000, 0x0000, 0x0000,  // Y
0x7C00, 0x0400, 0x0800, 0x1000, 0x1000, 0x2000, 0x4000, 0x7C00, 0x0000, 0x0000,  // Z
0x1800, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1800,  // [
0x2000, 0x2000, 0x1000, 0x1000, 0x1000, 0x1000, 0x0800, 0x0800, 0x0000, 0x0000,  /* \ */
0x3000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x3000,  // ]
0x1000, 0x2800, 0x2800, 0x4400, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  // ^
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFE00,  // _
0x2000, 0x1000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  // `
0x0000, 0x0000, 0x3800, 0x4400, 0x3C00, 0x4400, 0x4C00, 0x3400, 0x0000, 0x0000,  // a
0x4000, 0x4000, 0x5800, 0x6400, 0x4400, 0x4400, 0x6400, 0x5800, 0x0000, 0x0000,  // b
0x0000, 0x0000, 0x3800, 0x4400, 0x4000, 0x4000, 0x4400, 0x3800, 0x0000, 0x0000,  // c
0x0400, 0x0400, 0x3400, 0x4C00, 0x4400, 0x4400, 0x4C00, 0x3400, 0x0000, 0x0000,  // d
0x0000, 0x0000, 0x3800, 0x4400, 0x7C00, 0x4000, 0x4400, 0x3800, 0x0000, 0x0000,  // e
0x0C00, 0x1000, 0x7C00, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x0000, 0x0000,  // f
0x0000, 0x0000, 0x3400, 0x4C00, 0x4400, 0x4400, 0x4C00, 0x3400, 0x0400, 0x7800,  // g
0x4000, 0x4000, 0x5800, 0x6400, 0x4400, 0x4400, 0x4400, 0x4400, 0x0000, 0x0000,  // h
0x1000, 0x0000, 0x7000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x0000, 0x0000,  // i
0x1000, 0x0000, 0x7000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0xE000,  // j
0x4000, 0x4000, 0x4800, 0x5000, 0x6000, 0x5000, 0x4800, 0x4400, 0x0000, 0x0000,  // k
0x7000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x0000, 0x0000,  // l
0x0000, 0x0000, 0x7800, 0x5400, 0x5400, 0x5400, 0x5400, 0x5400, 0x0000, 0x0000,  // m
0x0000, 0x0000, 0x5800, 0x6400, 0x4400, 0x4400, 0x4400, 0x4400, 0x0000, 0x0000,  // n
0x0000, 0x0000, 0x3800, 0x4400, 0x4400, 0x4400, 0x4400, 0x3800, 0x0000, 0x0000,  // o
0x0000, 0x0000, 0x5800, 0x6400, 0x4400, 0x4400, 0x6400, 0x5800, 0x4000, 0x4000,  // p
0x0000, 0x0000, 0x3400, 0x4C00, 0x4400, 0x4400, 0x4C00, 0x3400, 0x0400, 0x0400,  // q
0x0000, 0x0000, 0x5800, 0x6400, 0x4000, 0x4000, 0x4000, 0x4000, 0x0000, 0x0000,  // r
0x0000, 0x0000, 0x3800, 0x4400, 0x3000, 0x0800, 0x4400, 0x3800, 0x0000, 0x0000,  // s
0x2000, 0x2000, 0x7800, 0x2000, 0x2000, 0x2000, 0x2000, 0x1800, 0x0000, 0x0000,  // t
0x0000, 0x0000, 0x4400, 0x4400, 0x4400, 0x4400, 0x4C00, 0x3400, 0x0000, 0x0000,  // u
0x0000, 0x0000, 0x4400, 0x4400, 0x2800, 0x2800, 0x2800, 0x1000, 0x0000, 0x0000,  // v
0x0000, 0x0000, 0x5400, 0x5400, 0x5400, 0x6C00, 0x2800, 0x2800, 0x0000, 0x0000,  // w
0x0000, 0x0000, 0x4400, 0x2800, 0x1000, 0x1000, 0x2800, 0x4400, 0x0000, 0x0000,  // x
0x0000, 0x0000, 0x4400, 0x4400, 0x2800, 0x2800, 0x1000, 0x1000, 0x1000, 0x6000,  // y
0x0000, 0x0000, 0x7C00, 0x0800, 0x1000, 0x2000, 0x4000, 0x7C00, 0x0000, 0x0000,  // z
0x1800, 0x1000, 0x1000, 0x1000, 0x2000, 0x2000, 0x1000, 0x1000, 0x1000, 0x1800,  // {
0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000,  // |
0x3000, 0x1000, 0x1000, 0x1000, 0x0800, 0x0800, 0x1000, 0x1000, 0x1000, 0x3000,  // }
0x0000, 0x0000, 0x0000, 0x7400, 0x4C00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  // ~
};

typedef enum TouchDetect{
	no_touch = 0,
	touched = 1
} TouchDetect_t;

struct loc{
	uint16_t x_loc;
	uint16_t y_loc;
} *get_loc;
void GPIO_config()
{

//	PA8 I2C
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER &= ~(3 << (2 * 8));
	GPIOA->MODER |= (GPIO_Mode_AF << (2 * 8));

	GPIOA->PUPDR &= ~((3 << (2 * 8)));
	GPIOA->PUPDR |= (GPIO_PuPd_UP << (2 * 8));

	GPIOA->OSPEEDR &= ~((3 << (2 * 8)));
	GPIOA->OSPEEDR |= (GPIO_Speed_100MHz << (2 * 8));

	GPIOA->OTYPER &= ~((1 << 8));
	GPIOA->OTYPER |= (GPIO_OType_OD << 8);

	GPIOA->AFR[1] |= (4 << (0x08 & 0x07) * 4);

//	 PC9 I2C
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	GPIOC->MODER &= ~(3 << (2 * 9));
	GPIOC->MODER |= (GPIO_Mode_AF << (2 * 9));

	GPIOC->PUPDR &= ~((3 << (2 * 9)));
	GPIOC->PUPDR |= (GPIO_PuPd_UP << (2 * 9));

	GPIOC->OSPEEDR &= ~((3 << (2 * 9)));
	GPIOC->OSPEEDR |= (GPIO_Speed_100MHz << (2 * 9));

	GPIOC->OTYPER &= ~((1 << 9));
	GPIOC->OTYPER |= (GPIO_OType_OD << 9);

	GPIOC->AFR[1] |= (4 << (0x09 & 0x07) * 4);


	//PA15 TP_INT Touchscreen
	GPIOA->MODER &= ~(3 << 2 * 15);
	GPIOA->MODER |= (GPIO_Mode_IN << (2 * 15));

	GPIOA->PUPDR &= ~((3 << (2 * 15)));
	GPIOA->PUPDR |= (GPIO_PuPd_NOPULL << (2 * 15));

	//TX RX PA9 PA10 for Debug
	GPIOA->MODER &= ~(3 << (2 * 9));
	GPIOA->MODER |= (GPIO_Mode_AF << (2 * 9));

	GPIOA->OTYPER&= ~(1 << 9);
	GPIOA->OTYPER|= GPIO_OType_PP << 9;

	GPIOA->PUPDR &= ~(3<<(2*9));
	GPIOA->PUPDR |= GPIO_PuPd_UP <<(2*9);

	GPIOA->OSPEEDR &= ~(3 << (2 * 9));
	GPIOA->OSPEEDR |= GPIO_Speed_50MHz << (2 * 9);

	GPIOA->MODER &= ~(3<<(2*10));
	GPIOA->MODER |= GPIO_Mode_AF << (2 * 10);

	GPIOA->OTYPER &= ~(1<<10);
	GPIOA->OTYPER |= GPIO_OType_PP << 10;

	GPIOA->PUPDR &= ~(3<<(2*10));
	GPIOA->PUPDR |= GPIO_PuPd_UP <<(2*10);

	GPIOA->OSPEEDR &= ~(3<<(2 * 10));
	GPIOA->OSPEEDR |= GPIO_Speed_50MHz <<(2 * 10);
	GPIOA->AFR[1] |= (7 << (0x9 & 0x07) * 4) + (7 << (0xA & 0x07) * 4); //PA9 PA10 AF7

	// SPI PF7 SCLK PF8 MISO PF9 MOSI
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;

	GPIOF->MODER &= ~((3 << (2 * 7)) | (3 << (2 * 9)));
	GPIOF->MODER |= GPIO_Mode_AF << (2 * 7) | GPIO_Mode_AF << (2 * 9);

	GPIOF->OSPEEDR &= ~((3 << (2 * 7)) | (3 << (2 * 9)));
	GPIOF->OSPEEDR |= GPIO_Speed_100MHz << (2 * 7) | GPIO_Speed_100MHz << (2 * 9);

	GPIOF->OTYPER &= ~(1<<7 | 1 << 9);
	GPIOF->OTYPER |= GPIO_OType_PP << 7 | GPIO_OType_PP << 9 ;

	GPIOF->AFR[0] |= (5 << (0x07 & 0x07) * 4);
	GPIOF->AFR[1] |= (5 << (0x09 & 0x07) * 4);
	// PA7 ACP_Reset
	GPIOA->MODER|= GPIO_Mode_OUT << (2 * 7);

	GPIOA->OSPEEDR|=GPIO_Speed_100MHz << (2 * 7);

	GPIOA->ODR|= 1 << 7;

	// PD13 Data Command Select DC
//	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
//
//	GPIOD->MODER|=GPIO_Mode_OUT << (2 * 13) | GPIO_Mode_OUT << (2 * 12);
//
//	GPIOD->OSPEEDR|=GPIO_Speed_100MHz << (2 * 13) | GPIO_Speed_100MHz << (2 * 12);
//
//	GPIOD->ODR |= 1 << 13 | 1 << 12;

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

	GPIOD->MODER|=GPIO_Mode_OUT << (2 * 13);

	GPIOD->OSPEEDR|=GPIO_Speed_50MHz << (2 * 13);

	GPIOD->ODR |= 0 << 13;
	// PD12 LCD: RDX
	GPIOD->MODER|=GPIO_Mode_OUT << (2 * 12);

	GPIOD->OSPEEDR|=GPIO_Speed_100MHz << (2 * 1);

	GPIOD->ODR |= 1 << 12;
	// PC2 SPI: CS
	GPIOC->MODER|= GPIO_Mode_OUT << (2 * 2);

	GPIOC->OSPEEDR|=GPIO_Speed_50MHz << (2 * 2);

	GPIOC->ODR|=1 << 2;
}

void UART_config()
{
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 9600;
    USART_Init(USART1, &USART_InitStructure);

	USART1->CR1 &= ~(USART_CR1_OVER8 | USART_CR1_UE | USART_CR1_M | USART_CR1_PCE | USART_CR1_PS | USART_CR1_TE | USART_CR1_RE);

	USART1->CR2 &= ~(USART_CR2_STOP);
	USART1->CR3 &= ~(USART_CR3_ONEBIT | USART_CR3_EIE |  USART_CR3_RTSE | USART_CR3_CTSE);

	USART1->BRR &= ~(0xFFFF); //clear Manitassa and Fraction
	//USART1->BRR = 52; //0.5M == WHY PCLK2
	//uint16_t x = (1000000L / 9600L / 16L);
	USART1->BRR = 90000000L / 9600L; //28.8ML / 9600L; //要算 假設以1M

	USART1->CR2 &= ~(USART_CR2_LINEN | USART_CR2_CLKEN);
	USART1->CR3 &= ~(USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN);

	USART1->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;//  | USART_CR1_TE | USART_CR1_RE; //| USART_CR1_TE | USART_CR1_RE;

	USART1->CR2 |= USART_CR2_CLKEN;
}
void UART_transmit(uint8_t *arr)
{
	while(*arr != '\0')
	{
		while ((USART1->SR & USART_SR_TXE) == 0); //等待Transmit data empty
		USART1->DR = *arr;
		arr++;
	}
	while ((USART1->SR & USART_SR_TXE) == 0); //等待Transmit data empty
}

void clock_config()
{
	// Set system clock as MSI
	RCC->APB1ENR |= 1 << 28;
	PWR->CR &= (uint32_t)~(PWR_CR_VOS);

	RCC->CFGR &= ~3;
	RCC->CR |= RCC_CR_HSION; // 0x1 HSI MODE On RCC_CFGR_SW_HSI
	RCC->CFGR |= RCC_CFGR_SW_HSI; // 0x00 HSI mode selection
	RCC->CR &= ~RCC_CR_PLLON;

	//RCC->CFGR &= ~(0xF << 4 | 0x7 << 10 | 0x7 << 13);
	//AHB 1 APB1 4 APB2 2

	RCC->CFGR &= ~(1 << 4);
	RCC->CFGR |= (5 << 10) | (4 << 13);
	//M 8 N 180 P 2 Q 2
	RCC->PLLCFGR = 2 << 24 | 0 << 16 | 180 << 6 | 8; //11 << 4


	RCC->CR |= RCC_CR_PLLON; // 0x1 HSI MODE On RCC_CFGR_SW_HSI
	while (!(RCC->CR & (1<<25)));
	RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
	RCC->CFGR |= RCC_CFGR_SW_PLL;

	FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_5WS;

	SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));
}


void SPI_config()
{
	RCC->APB2ENR |= RCC_APB2ENR_SPI5EN;
	RCC->AHB1ENR|=RCC_AHB1ENR_DMA2EN; //DMA

//	SPI_InitTypeDef* SPI_X;
//	SPI_TypeDef* SPIN;
//	SPI_Init(SPIN,SPI_X);

	SPI5->I2SCFGR &= ~(SPI_I2SCFGR_I2SMOD | SPI_I2SCFGR_I2SCFG | SPI_I2SCFGR_CHLEN);
	SPI5->CR1 &= ~(SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE | SPI_CR1_CRCEN | SPI_CR1_DFF | SPI_CR1_LSBFIRST
				 | SPI_CR1_SPE | SPI_CR1_BR | SPI_CR1_MSTR | SPI_CR1_CPOL | SPI_CR1_CPHA);

	SPI5->CR1 |= SPI_CR1_MSTR | SPI_CR1_SSM |SPI_CR1_SSI| SPI_CR1_BR_0; //BR = Buad rate 要調整

	SPI5->CR2 &= ~(SPI_CR2_SSOE | SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN);
	SPI5->CR2|=SPI_CR2_TXDMAEN;

	NVIC_EnableIRQ(SPI5_IRQn);

	DMA2_Stream4->CR &= ~(DMA_SxCR_EN);

	while((DMA2_Stream4->CR & DMA_SxCR_EN)==DMA_SxCR_EN);
	DMA2_Stream4->CR |= (2 << 25) | (DMA_SxCR_MINC) | 1 << 6 | (DMA_SxCR_TCIE); //memory to peh.

	NVIC_EnableIRQ(DMA2_Stream4_IRQn);

	SPI5->CR1|=SPI_CR1_SPE;
}
void SPI_transmit(uint8_t *data,uint16_t size)
{
	uint16_t i = 0;

	while(i < size)
	{
		while(!(SPI5->SR & SPI_SR_TXE));	//等待TXE = 1

		SPI5->DR = data[i];					//完成傳送TXE被清除
		i++;
	}
	while(SPI5->SR & SPI_SR_BSY);
	while(!(SPI5->SR & SPI_SR_TXE));		//等待TXE(最後一筆傳送完成)

	while(SPI5->SR & SPI_SR_BSY);			//等待傳輸結束

	(void) SPI5->DR;
	(void) SPI5->SR;
}
void SPI_transmit_DMA(uint8_t *data,uint16_t size)
{
	DMA2_Stream4->M0AR =(uint16_t)data;
	DMA2_Stream4->NDTR = size;
	DMA2_Stream4->PAR = (uint32_t)&SPI5->DR;

	DMA2_Stream4->CR|=DMA_SxCR_EN;			//傳輸完畢
}
void DMA2_Stream4_IRQHandler(void)
{
	if(DMA2->HISR&(DMA_HISR_TCIF4))			//收到DMA4 isr訊號 代表傳輸完畢
	{
		SPI5->CR2|=SPI_CR2_TXEIE;			//給SPI TXEIE ISR

		DMA2->HIFCR |=DMA_HIFCR_CTCIF4;		//清除DMA的ISR
	}
}
void SPI5_IRQHandler(void)
{
	if((SPI5->SR & SPI_SR_TXE) && ((SPI5->SR & SPI_SR_BSY) == 0))
	{
		SPI5->CR2 &= ~SPI_CR2_TXEIE;
	}
}

void LCD_Reset()
{
	//PF 7
	GPIOD->ODR &= ~(1 << 2 * 12);
	for(int i = 0;i < 9000000;i++);
	GPIOD->ODR = 1 << 2 * 12;
	for(int i = 0;i < 3600000;i++);
}
void LCD_Write_Cmd(uint8_t cmd)
{
//	//PC2 PD13
//	GPIOC->BSRRL = GPIO_BSRR_BR_2;		//LOW
//	GPIOD->BSRRH = GPIO_BSRR_BR_13;		//LOW
//	SPI_transmit(&cmd,1);
//	GPIOC->BSRRL = GPIO_BSRR_BS_2;		//HIGH

	GPIOC->ODR &= ~(1 << 2);		//LOW
	GPIOD->ODR &= ~(1 << 13);		//RESET
	SPI_transmit(&cmd,1);
	GPIOC->ODR |= 1 << 2;		//HIGH

}

void LCD_Write_Data(uint8_t data)
{
//	GPIOC->BSRRL = GPIO_BSRR_BR_2;		//RESET
//	GPIOD->BSRRH = GPIO_BSRR_BS_13;		//SET
//	SPI_transmit(&data,1);
//	GPIOC->BSRRL = GPIO_BSRR_BS_2;		//SET

	GPIOC->ODR &= ~(1 << 2);		//RESET
	GPIOD->ODR |= 1 << 13;		//SET
	SPI_transmit(&data,1);
	GPIOC->ODR |= 1 << 2;		//SET
}

void I2C_config()
{
//	I2C_InitTypeDef i2c_initstruct;
//	I2C_Init(I2C3,&i2c_initstruct);
	RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;


	/* Enable the I2C peripheral */
	I2C3->CR1 |= I2C_CR1_SWRST;
	I2C3->CR1 &= ~I2C_CR1_SWRST;

//	I2C3->OAR1 &= 0x0000;
	I2C3->OAR1 |= 0 << 15 | 0 << 1;
//	// Configure the speed (100kHz, 400kHz etc.)
	I2C3->CR2 |= 0x2D; // freq 45M hz

	// SM mode
	I2C3->CCR = 0xE1; // CCR=Thigh/TPCLK = 0.5 * (1/100kHz)/(1/45MHz)=20
	I2C3->TRISE = 0x2E; //rising time 1000ns / (1/4M) = 4--> 4 + 1

	I2C3->CR1 |= I2C_CR1_PE;

}

void I2C_WriteByte(uint8_t send_addr,uint8_t mem_addr, uint8_t *data)
{
	while(I2C3->SR2 & I2C_SR2_BUSY);
	I2C1->CR1 |= I2C_CR1_ACK;  // Enable the ACK
	I2C3->CR1 |= I2C_CR1_START;

	while (!(I2C3->SR1 & I2C_SR1_SB));
	I2C3->DR = send_addr << 1; // 7bit + 1bit R/W --> W:0 ,R:1

	while(!(I2C3->SR1 & I2C_SR1_ADDR));
	uint8_t temp = I2C3->SR1 | I2C3->SR2;

	while(!(I2C3->SR1 & I2C_SR1_TXE));
	I2C3->DR = mem_addr;

	while(!(I2C3->SR1 & I2C_SR1_TXE));
	I2C3->DR = *data;

	while(!(I2C3->SR1 & I2C_SR1_BTF));
	I2C3->CR1 |= I2C_CR1_STOP;
}
void I2C_WriteMultiByte(uint8_t send_addr,uint8_t mem_addr, uint8_t *buffer, uint8_t datalen)
{
	while(I2C3->SR2 & I2C_SR2_BUSY);
	I2C1->CR1 |= I2C_CR1_ACK;  // Enable the ACK
	I2C3->CR1 |= I2C_CR1_START;

	while (!(I2C3->SR1 & I2C_SR1_SB));
	I2C3->DR = send_addr << 1; // 7bit + 1bit R/W --> W:0 ,R:1

	while(!(I2C3->SR1 & I2C_SR1_ADDR));
	uint8_t temp = I2C3->SR1 | I2C3->SR2;

	while(!(I2C3->SR1 & I2C_SR1_TXE));
	I2C3->DR = mem_addr;


	while(!(I2C3->SR1 & I2C_SR1_TXE));
	for(uint8_t i = 0;i < datalen;i++)
	{
		I2C3->DR = buffer[i];
		while(!(I2C3->SR1 & I2C_SR1_BTF));
	}

	I2C3->CR1 |= I2C_CR1_STOP;
}
void I2C_ReadByte(uint8_t send_addr,uint8_t mem_addr, uint8_t *data)
{

	while((I2C3->SR2 & I2C_SR2_BUSY) != 0);
	I2C1->CR1 |= I2C_CR1_ACK;  // Enable the ACK
	I2C3->CR1 |= I2C_CR1_START;

	while (!(I2C3->SR1 & I2C_SR1_SB));
	I2C3->DR = send_addr << 1; // 7bit + 1bit R/W --> R

	while (!(I2C3->SR1 & I2C_SR1_ADDR));	//ADDR = 1 and ACK = 1
	uint8_t temp = I2C3->SR1 | I2C3->SR2;

	while(!(I2C3->SR1 & I2C_SR1_TXE));
	I2C3->DR = mem_addr; //memory addr

	while(!(I2C3->SR1 & I2C_SR1_TXE)); //Repeat start
	I2C1->CR1 |= I2C_CR1_ACK;  // Enable the ACK
	I2C3->CR1|=I2C_CR1_START;

	while(!(I2C3->SR1 & I2C_SR1_SB));    //等待SB = 1
	I2C3->DR=send_addr<<1 | 1;	//Read

	while(!(I2C3->SR1 & I2C_SR1_ADDR));		//等待ADDR
	I2C3->CR1 &= ~I2C_CR1_ACK;				//ACK = 0 --> &= ~ACK
	temp = I2C3->SR1 | I2C3->SR2;
	I2C3->CR1|=I2C_CR1_STOP;

	while(!(I2C3->SR1 & I2C_SR1_RXNE));		//是否接收玩了
	uint16_t datatemp = I2C3->DR;
	*data=I2C3->DR;
}
void I2C_ReadMultiByte(uint8_t send_addr,uint8_t mem_addr, uint8_t *data,uint8_t length)
{
	while((I2C3->SR2 & I2C_SR2_BUSY) != 0);
	I2C1->CR1 |= I2C_CR1_ACK;  // Enable the ACK
	I2C3->CR1 |= I2C_CR1_START;

	while (!(I2C3->SR1 & I2C_SR1_SB));
	I2C3->DR = send_addr << 1; // 7bit + 1bit R/W --> R

	while (!(I2C3->SR1 & I2C_SR1_ADDR));	//ADDR = 1 and ACK = 1
	uint8_t temp = I2C3->SR1 | I2C3->SR2;

	while(!(I2C3->SR1 & I2C_SR1_TXE));
	I2C3->DR = mem_addr; //memory addr

	while(!(I2C3->SR1 & I2C_SR1_TXE));  //Repeat start
	I2C1->CR1 |= I2C_CR1_ACK;  // Enable the ACK
	I2C3->CR1|=I2C_CR1_START;

	while(!(I2C3->SR1&I2C_SR1_SB));    //等待SB = 1
	I2C3->DR=send_addr<<1 | 1;	//Read

	while(!(I2C3->SR1&I2C_SR1_ADDR));		//等待ADDR
	temp = I2C3->SR1 | I2C3->SR2;
	I2C3->CR1|=I2C_CR1_ACK;				//ACK = 0 --> &= ~ACK

	while(length > 0)
	{
		if(length == 1)
		{
			I2C3->CR1 &= ~I2C_CR1_ACK;
			I2C3->CR1|=I2C_CR1_STOP;
			while(!(I2C3->SR1& I2C_SR1_RXNE));
			*data++ = I2C3->DR;
			break;
		}
		else
		{
			while(!(I2C3->SR1&I2C_SR1_RXNE));
			*data++ = I2C3->DR;
			length--;
		}
	}
}
void LCD_init(void)
{
	//LCD power init
	LCD_Write_Cmd(ILI9341_RESET);
	for(int i = 0;i < 240000;i++);
	LCD_Write_Cmd(ILI9341_DISPLAY_OFF); // display off
	LCD_Write_Cmd(ILI9341_POWER1);		// power on
	LCD_Write_Data(0x26);				//GVDD voltage P.178 4.75v
	LCD_Write_Cmd(ILI9341_POWER2); 		//DDVDH, VGH, VGL
	LCD_Write_Data(0x11);				//mode
	LCD_Write_Cmd(ILI9341_VCOM1);
	LCD_Write_Data (0x35);				// Set the VCOMH voltage (0x35 = 4.025v)
	LCD_Write_Data (0x3e); 				// Set the VCOML voltage (0x3E = -0.950v)
	LCD_Write_Cmd(ILI9341_VCOM2);		//control VCOM
	LCD_Write_Data(0xbe);

	//memory
	LCD_Write_Cmd(ILI9341_MAC);
	LCD_Write_Data(0x48);
	LCD_Write_Cmd(ILI9341_PIXEL_FORMAT); // pixel format set
	LCD_Write_Data(0x55); 				  //16bit /pixel
	LCD_Write_Cmd(ILI9341_FRC); 		  //Frame Rate
	LCD_Write_Data(0);
	LCD_Write_Data(0x1F);				  //spec 31 clock

	//DDRAM
	LCD_Write_Cmd(ILI9341_COLUMN_ADDR); // column set
	LCD_Write_Data(0x00);				// x0 High
	LCD_Write_Data(0x00);				// x0 Low
	LCD_Write_Data(0x00);				// x1 High 240
	LCD_Write_Data(0xEF);				// x1 Low 240
	LCD_Write_Cmd(ILI9341_PAGE_ADDR);	// Page set
	LCD_Write_Data(0x00);				// x0 High
	LCD_Write_Data(0x00);				// x0 Low
	LCD_Write_Data(0x01);				// x1 High 320
	LCD_Write_Data(0x3F);				// x1 Low  320
	LCD_Write_Cmd(ILI9341_TEARING_OFF); // tearing effect off

	//Entry
	LCD_Write_Cmd(ILI9341_Entry_Mode_Set); // entry mode set
	LCD_Write_Data(0x07);				   // Normal Disp. and low volt detect.

	LCD_Write_Cmd(ILI9341_DFC);			   // Display function
	LCD_Write_Data(0x0a);				   // Interval scan and AGND
	LCD_Write_Data(0x82);				   // white and 5frame scan
	LCD_Write_Data(0x27);			   	   // 320line
	LCD_Write_Data(0x00);			   	   // clock divisor

	// power on
	LCD_Write_Cmd(ILI9341_SLEEP_OUT); // sleep out
	for(int i = 0;i < 400000;i++);
	LCD_Write_Cmd(ILI9341_DISPLAY_ON);
	for(int i = 0;i < 400000;i++);
	LCD_Write_Cmd (ILI9341_GRAM); // memory write
	for(int i = 0;i < 20000;i++);
}
void LCD_SetAddrWindow(uint16_t X1,uint16_t Y1,uint16_t X2,uint16_t Y2)
{
	LCD_Write_Cmd(ILI9341_COLUMN_ADDR); 	// Column addr set
	LCD_Write_Data(X1 >> 8);
	LCD_Write_Data(X1 & 0xFF);     // XSTART
	LCD_Write_Data((X2 - 1)>>8);
	LCD_Write_Data((X2 - 1) & 0xFF);     // XEND

	LCD_Write_Cmd(ILI9341_PAGE_ADDR); 	// Row addr set
	LCD_Write_Data(Y1>>8);
	LCD_Write_Data(Y1 & 0xFF);     // YSTART
	LCD_Write_Data((Y2 - 1)>>8);
	LCD_Write_Data((Y2 - 1) & 0xFF);     // YEND

	LCD_Write_Cmd(ILI9341_GRAM); 		// write to RAM
}
void ILI9341_setRotation(uint8_t rotate)
{
	switch(rotate)
	{
	case 1:
	rotationNum = 1;
	LCD_Write_Cmd(ILI9341_MEMCONTROL);
	LCD_Write_Data(ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR);
	break;
	case 2:
	rotationNum = 2;
	LCD_Write_Cmd(ILI9341_MEMCONTROL);
	LCD_Write_Data(ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR);
	break;
	case 3:
	rotationNum = 3;
	LCD_Write_Cmd(ILI9341_MEMCONTROL);
	LCD_Write_Data(ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR);
	break;
	case 4:
	rotationNum = 4;
	LCD_Write_Cmd(ILI9341_MEMCONTROL);
	LCD_Write_Data(ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR);
	break;
	default:
	rotationNum = 1;
	LCD_Write_Cmd(ILI9341_MEMCONTROL);
	LCD_Write_Data(ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR);
	break;
	}
}
//static __inline void ILI9341_FillColor( uint32_t ulAmout_Point, uint16_t usColor )
//{
//    uint32_t i = 0;
//
//    /* memory write */
//    LCD_Write_Cmd (ILI9341_GRAM);
//
//    for (i = 0; i < ulAmout_Point; i ++ )
//        LCD_Write_Data(0XFFFF);
//}
static void ConvHL(uint8_t *s, int32_t l)
{
	uint8_t temp;
	while(l > 0)
	{
		temp = *(s+1);
		*(s+1) = *(s);
		*(s) = temp;
		s += 2;
		l -= 2;
	}
}
void LCD_DrawPixel(uint8_t *s,uint8_t x_pos, uint8_t y_pos)
{
	LCD_SetAddrWindow(x_pos,y_pos,x_pos,y_pos);
	GPIOD->ODR = 1 << 13;		//SET
	GPIOC->ODR &= 1 << 2;		//RESET
	LCD_Write_Data(s);
	GPIOC->ODR |= 1 << 2;		//SET
}
void LCD_DrawBitmap(uint16_t w, uint16_t h, uint8_t *s)
{
	int index = w * h * 2;
	GPIOD->ODR = 1 << 13;		//SET
	GPIOC->ODR &= 1 << 2;		//RESET

	while(index)
	{
		LCD_Write_Data(s);
		index--;
	}
	GPIOC->ODR |= 1 << 2;		//SET
}
void Draw_Char(uint16_t x_pos, uint16_t y_pos, uint8_t *s,char ch)
{
	//uint8_t i = 40;	//8 * 5
	unsigned char i = 0x00;
	unsigned char j = 0x00;
	unsigned char value = 0x00;
	LCD_SetAddrWindow(x_pos,y_pos,x_pos + 8,y_pos+5);

	for(i = 0x00; i < 0x07; i++)
	 {
		 for(j = 0x00; j < 0x10; j++)
		 {
			 value = 0x0000;
			 value = ((Font_ID[((unsigned char)ch) - 0x20]));

			 if(((value >> j) & 0x01) != 0x00)
			 {
				 LCD_DrawBitmap(x_pos, y_pos, 0xFFFF);
			 }
			 else
			 {
				 LCD_DrawBitmap(x_pos, y_pos, 0x0000);
			 }
			 y_pos += 10;
		  }
		  y_pos -= (10 << 0x03);
		  x_pos += 7;
	  }
}

void TouchPanel_init()
{
	//Init mode GPIO OFF, TSC ADC ON
	uint8_t Buffer[1];
	I2C_WriteByte(deviceAddress,STMPE811_SYS_CTRL1,2); //RESET
	for(int i = 0;i < 450000;i++);
	I2C_WriteByte(deviceAddress,STMPE811_SYS_CTRL1,0); //關閉休眠模式--> 開啟
	for(int i = 0;i < 90000;i++);

	I2C_ReadByte(deviceAddress,STMPE811_SYS_CTRL2,Buffer);
	Buffer[0] &= ~(0x04U);
	I2C_WriteByte(deviceAddress,STMPE811_SYS_CTRL2,Buffer);
	for(int i = 0;i < 90000;i++);

	I2C_ReadByte(deviceAddress,STMPE811_GPIO_AF,Buffer);
	Buffer[0] &= ~((uint8_t)STMPE811_TOUCH_ALL);
	I2C_WriteByte(deviceAddress,STMPE811_GPIO_AF,Buffer);	//AF for TSC

	I2C_ReadByte(deviceAddress,STMPE811_SYS_CTRL2,&Buffer);
	Buffer[0] &= ~(0x02);
	I2C_WriteByte(deviceAddress,STMPE811_SYS_CTRL2,Buffer);

	I2C_ReadByte(deviceAddress,STMPE811_SYS_CTRL2,Buffer);
	Buffer[0] &= ~(0x01);
	I2C_WriteByte(deviceAddress,STMPE811_SYS_CTRL2,Buffer);

	I2C_ReadByte(deviceAddress,STMPE811_SYS_CTRL2,Buffer);


	I2C_WriteByte(deviceAddress, STMPE811_ADC_CTRL1, 0x49U);	// sampling 80 clock and 12 bit ADC
	for(int i = 0;i < 90000;i++);

	I2C_WriteByte(deviceAddress, STMPE811_ADC_CTRL2, 0x01U);	//ADC freq 3.25MHz

	//開始設置TSC
	/* Configuration:
	- Touch average control    : 8 samples
	- Touch delay time         : 500 uS
	- Panel driver setting time: 500 uS
	*/
	I2C_WriteByte(deviceAddress, STMPE811_TSC_CFG, 0xDAU); //10011010 0x9A

	// FIFO Threadhold (Threadhold to ISR), STATUS
	I2C_WriteByte(deviceAddress, STMPE811_FIFO_TH, 0x01U);
	// Reset FIFO
	I2C_ReadByte(deviceAddress,STMPE811_FIFO_STA,Buffer);
	Buffer[0] = 0x01U;
	I2C_WriteByte(deviceAddress, STMPE811_FIFO_STA, Buffer[0]);
	// Out of Reset, setup
	Buffer[0] = 0x00U;
	I2C_WriteByte(deviceAddress, STMPE811_FIFO_STA, Buffer);

	// range and accuracy of the pressure measurement
	I2C_WriteByte(deviceAddress, STMPE811_TSC_FRACT_Z, 0x01U);
	// driven mode (50mA)
	I2C_WriteByte(deviceAddress, STMPE811_TSC_I_DRIVE, 0x01U);

	I2C_ReadByte(deviceAddress,STMPE811_TSC_CTRL,Buffer);
	Buffer[0] =  0x01U;
	I2C_WriteByte(deviceAddress, STMPE811_TSC_CTRL, Buffer);

	//  Clear all the status pending bits if any
	// Writing '1' to this register clears the corresponding bits. Writing '0' has no effect.
	I2C_WriteByte(deviceAddress, STMPE811_INT_STATUS, 0xFFU);
	for(int i = 0;i < 225000;i++);

}
TouchDetect_t IsTouch()
{
	uint8_t value = 0;
	uint8_t getXYZ[4];
	I2C_ReadByte(deviceAddress,STMPE811_TSC_CTRL,&value);
	value &= 1 << 7; // STATUS

	if(value == (1<<7))
	{
		I2C_ReadByte(deviceAddress, STMPE811_FIFO_SIZE, &value);

		if(value>0) //是否有空間讀取和是否為真實?
		{
			return touched;
		}
	}

	else
	{
		I2C_WriteByte(deviceAddress, STMPE811_FIFO_STA, 1);	//Reset

		I2C_WriteByte(deviceAddress, STMPE811_FIFO_STA, 0);	//Seton
	}
	return no_touch;
}
uint16_t Read_X(uint32_t data_XYZ, uint16_t x)
{
	int16_t val, dx;
	val = (data_XYZ >> 20) & 0x00000FFF;

	if (val <= 3000) {
		val = 3900 - val;
	} else {
		val = 3800 - val;
	}

	val /= 15;

	if (val > 239) {
		val = 239;
	} else if (val < 0) {
		val = 0;
	}

	dx = (val > x) ? (val - x) : (x - val);
	if (dx > 4) {
		return val;
	}
	return x;
}
uint16_t Read_Y(uint32_t data_XYZ, uint16_t y)
{
	int16_t val, dy;
	val = (data_XYZ >> 8) & 0x00000FFF;

	val -= 360;
	val = val / 11;

	if (val <= 0) {
		val = 0;
	} else if (val >= 320) {
		val = 319;
	}

	dy = (val > y) ? (val - y) : (y - val);
	if (dy > 4) {
		return val;
	}
	return y;
}
void gettouchvalue()
{
	uint32_t data_XYZ;

	uint8_t getXYZ[4];
	uint16_t X,Y;

	I2C_ReadMultiByte(deviceAddress, STMPE811_TSC_DATA_XYZ, getXYZ, 4);

	data_XYZ = getXYZ[0] << 24 | getXYZ[1] << 16 | getXYZ[2] << 8 | getXYZ[3];

	X = Read_X(data_XYZ, get_loc->x_loc);
	X = 239 - X;
	Y = Read_Y(data_XYZ, get_loc->y_loc);
	Y = 319 - Y;
	char buffer[32];
	sprintf(buffer, "X = %d, Y = %d", X,Y);
	UART_transmit((uint8_t*)buffer);
	UART_transmit((uint8_t*)"\r\n");

	I2C_WriteByte(deviceAddress, STMPE811_FIFO_STA,1);
	I2C_WriteByte(deviceAddress, STMPE811_FIFO_STA,0);
}
uint16_t getID()
{
	uint8_t data[2];
	uint16_t ID;

	I2C_ReadMultiByte(deviceAddress, 0x00, data, 2);
	ID = data[0] << 8 | data[1];
	return ID;
}

int main(void)
{

	clock_config();
	GPIO_config();

	UART_config();
	SPI_config();
	I2C_config();

	TouchPanel_init();

	LCD_init();
	ILI9341_setRotation(1);
	LCD_SetAddrWindow(80,80,160,160);
	LCD_DrawBitmap(80,80,0x8430);
	LCD_SetAddrWindow(80,160,160,320);
	LCD_DrawBitmap(80,160,0xF81F);

	while(1)
	{
		uint16_t ID = getID();	//是否有讀取到晶片(STMPE811)
		if(IsTouch())
			gettouchvalue();

	}
}
