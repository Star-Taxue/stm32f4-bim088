#include "bsp_lcd.h"
#include "../DRV/Inc/drv_lcd.h"
#include "spi.h"
// 声明外部SPI初始化函数
extern void MX_SPI1_Init(void);  // 这个函数在spi.c中定义
extern void MX_SPI3_Init(void);  // 这个函数在spi.c中定义
extern void LCD_SetSPIHandle(SPI_HandleTypeDef* hspi);


// 定义默认的LCD句柄
BSP_LCD_HandleTypeDef hlcd_default = {
    .spi_handle = NULL,  // 将在初始化时设置
    .cs_port = LCD_CS_PORT,
    .cs_pin = LCD_CS_PIN,
    .dc_port = LCD_DC_PORT,
    .dc_pin = LCD_DC_PIN,
    .backlight_port = LCD_Backlight_PORT,
    .backlight_pin = LCD_Backlight_PIN,
    .width = 240,
    .height = 240,
    .direction = 0
};
/**
  * @brief  LCD初始化
  * @param  hlcd: LCD句柄
  * @retval BSP_LCD_StatusTypeDef: LCD状态
  */
BSP_LCD_StatusTypeDef BSP_LCD_Init(BSP_LCD_HandleTypeDef* hlcd) {
    if (hlcd == NULL) {
        return BSP_LCD_ERROR;
    }
    
    // 初始化SPI接口
    // 根据句柄中的SPI实例选择初始化函数
    if (hlcd->spi_handle == &hspi1) {
        MX_SPI1_Init();
    } else if (hlcd->spi_handle == &hspi3) {
        MX_SPI3_Init();
    }
    
    // 在DRV层设置SPI句柄
    LCD_SetSPIHandle(hlcd->spi_handle);
    
    // GPIO引脚应在MX_GPIO_Init()中初始化，此处仅设置CS,DC,BL默认状态
    HAL_GPIO_WritePin(hlcd->cs_port, hlcd->cs_pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(hlcd->dc_port, hlcd->dc_pin, GPIO_PIN_RESET);
    // 背光引脚状态根据需要设置
    HAL_GPIO_WritePin(hlcd->backlight_port, hlcd->backlight_pin, GPIO_PIN_RESET);
    
    // 调用DRV层的初始化函数
    SPI_LCD_Init();
    
    return BSP_LCD_OK;
}

/**
  * @brief  LCD去初始化
  * @param  hlcd: LCD句柄
  * @retval BSP_LCD_StatusTypeDef: LCD状态
  */
BSP_LCD_StatusTypeDef BSP_LCD_DeInit(BSP_LCD_HandleTypeDef* hlcd) {
    if (hlcd == NULL) {
        return BSP_LCD_ERROR;
    }
    
    // 复位所有引脚
    HAL_GPIO_DeInit(hlcd->cs_port, hlcd->cs_pin);
    HAL_GPIO_DeInit(hlcd->dc_port, hlcd->dc_pin);
    HAL_GPIO_DeInit(hlcd->backlight_port, hlcd->backlight_pin);
    
    return BSP_LCD_OK;
}

/**
  * @brief  开启LCD显示
  * @param  hlcd: LCD句柄
  * @retval BSP_LCD_StatusTypeDef: LCD状态
  */
BSP_LCD_StatusTypeDef BSP_LCD_DisplayOn(BSP_LCD_HandleTypeDef* hlcd) {
    if (hlcd == NULL) {
        return BSP_LCD_ERROR;
    }
    
    HAL_GPIO_WritePin(hlcd->backlight_port, hlcd->backlight_pin, GPIO_PIN_SET);
    return BSP_LCD_OK;
}

/**
  * @brief  关闭LCD显示
  * @param  hlcd: LCD句柄
  * @retval BSP_LCD_StatusTypeDef: LCD状态
  */
BSP_LCD_StatusTypeDef BSP_LCD_DisplayOff(BSP_LCD_HandleTypeDef* hlcd) {
    if (hlcd == NULL) {
        return BSP_LCD_ERROR;
    }
    
    HAL_GPIO_WritePin(hlcd->backlight_port, hlcd->backlight_pin, GPIO_PIN_RESET);
    return BSP_LCD_OK;
}

BSP_LCD_StatusTypeDef BSP_LCD_InitDefault(SPI_HandleTypeDef* hspi) {
    hlcd_default.spi_handle = hspi;
    return BSP_LCD_Init(&hlcd_default);
}

