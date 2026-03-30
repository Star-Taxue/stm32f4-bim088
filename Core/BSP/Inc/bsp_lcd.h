#ifndef __BSP_LCD_H
#define __BSP_LCD_H

#include "stm32f4xx_hal.h"

// 定义BSP状态类型
typedef enum {
    BSP_LCD_OK = 0,
    BSP_LCD_ERROR,
    BSP_LCD_BUSY,
    BSP_LCD_TIMEOUT,
    BSP_LCD_NOT_SUPPORTED
} BSP_LCD_StatusTypeDef;

// LCD设备句柄结构体
typedef struct {
    SPI_HandleTypeDef* spi_handle;
    
    // 控制引脚定义
    GPIO_TypeDef* cs_port;
    uint16_t cs_pin;
    
    GPIO_TypeDef* dc_port;
    uint16_t dc_pin;
    
    GPIO_TypeDef* backlight_port;
    uint16_t backlight_pin;
    
    // 显示属性
    uint16_t width;
    uint16_t height;
    uint8_t direction;
} BSP_LCD_HandleTypeDef;

// 导出默认句柄
extern BSP_LCD_HandleTypeDef hlcd_default;

// 函数声明
BSP_LCD_StatusTypeDef BSP_LCD_Init(BSP_LCD_HandleTypeDef* hlcd);
BSP_LCD_StatusTypeDef BSP_LCD_InitDefault(SPI_HandleTypeDef* hspi);
BSP_LCD_StatusTypeDef BSP_LCD_DeInit(BSP_LCD_HandleTypeDef* hlcd);
BSP_LCD_StatusTypeDef BSP_LCD_DisplayOn(BSP_LCD_HandleTypeDef* hlcd);
BSP_LCD_StatusTypeDef BSP_LCD_DisplayOff(BSP_LCD_HandleTypeDef* hlcd);


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


#endif /* __BSP_LCD_H */