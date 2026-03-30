/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    spi.h
  * @brief   This file contains all the function prototypes for
  *          the spi.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_H__
#define __SPI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern SPI_HandleTypeDef hspi1;

extern SPI_HandleTypeDef hspi2;

extern SPI_HandleTypeDef hspi3;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_SPI1_Init(void);
void MX_SPI2_Init(void);
void MX_SPI3_Init(void);

/* USER CODE BEGIN Prototypes */
/*------------------------------------------------------  引脚配置宏 -------------------------------------------------*/	

#define 	LCD_SCK_PIN      		 GPIO_PIN_3									// SCK引脚， 需要重定义SPI3的IO口复用
#define 	LCD_SCK_PORT     		 GPIOB                 					// SCK引脚用到的端口  
#define 	GPIO_LCD_SCK_CLK      __HAL_RCC_GPIOB_CLK_ENABLE()  		// SCK引脚IO口时钟

#define 	LCD_SDA_PIN      		 GPIO_PIN_5									// SDA引脚， 需要重定义SPI3的IO口复用
#define 	LCD_SDA_PORT    		 GPIOB                 					// SDA引脚用到的端口  
#define 	GPIO_LCD_SDA_CLK      __HAL_RCC_GPIOB_CLK_ENABLE()  		// SDA引脚IO口时钟


#define 	LCD_CS_PIN       				GPIO_PIN_15							// CS片选引脚，低电平有效
#define 	LCD_CS_PORT      				GPIOA                 			// CS引脚用到的端口 
#define 	GPIO_LCD_CS_CLK     			__HAL_RCC_GPIOA_CLK_ENABLE()	// CS引脚IO口时钟

#define  LCD_DC_PIN						GPIO_PIN_13				         // 数据指令选择  引脚				
#define	LCD_DC_PORT						GPIOD									// 数据指令选择  GPIO端口
#define 	GPIO_LCD_DC_CLK     			__HAL_RCC_GPIOD_CLK_ENABLE()	// 数据指令选择  GPIO时钟 	

#define  LCD_Backlight_PIN				GPIO_PIN_12				         // 背光  引脚				
#define	LCD_Backlight_PORT			GPIOD									// 背光 GPIO端口
#define 	GPIO_LCD_Backlight_CLK     __HAL_RCC_GPIOD_CLK_ENABLE()	// 背光 GPIO时钟 	


/*--------------------------------------------------------- 控制宏 ---------------------------------------------------*/

// 因为片选引脚需要频繁操作，使用寄存器效率会高些
#define 	LCD_CS_H    		 	LCD_CS_PORT->BSRR  = LCD_CS_PIN							// 片选拉高
#define 	LCD_CS_L     			LCD_CS_PORT->BSRR  = (uint32_t)LCD_CS_PIN << 16U	// 片选拉低

#define	LCD_DC_Command		   HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_RESET)	  		// 低电平，指令传输 
#define 	LCD_DC_Data		      HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_SET)				// 高电平，数据传输

#define 	LCD_Backlight_ON      HAL_GPIO_WritePin(LCD_Backlight_PORT, LCD_Backlight_PIN, GPIO_PIN_SET)		// 高电平，开启背光
#define 	LCD_Backlight_OFF  	 HAL_GPIO_WritePin(LCD_Backlight_PORT, LCD_Backlight_PIN, GPIO_PIN_RESET)	// 低电平，关闭背光

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __SPI_H__ */

