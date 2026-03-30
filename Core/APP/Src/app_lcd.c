/**************************************************************************
* File name:   lcd.c
* Description: 使用各基本显示函数的组合来实现具体功能，如菜单界面
* Author:      星踏雪
* Date:        2026-01-3
* History:
*   2026-01-3 星踏雪 按照基地的软件规范整理并修改代码
**************************************************************************/

#include "app_lcd.h"
#include "drv_lcd.h"
#include "main.h"
#include <stdio.h>
#include <string.h>

#include "stdlib.h"
#include "bsp_bmi088.h"
#include "bsp_imu.h"
#include "spi.h"
#include "stm32f4xx_hal.h"


/**
 * @brief  LCD测试函数 - 显示欢迎信息和基本图形
 * @param  None
 * @retval None
 */
void      APP_LCD_Test(void) {
  // // 显示欢迎信息
  // APP_LCD_ShowWelcome();

  // HAL_Delay(2000); // 等待2秒

  // // 清屏
  // LCD_Clear();

  // // 显示系统信息
  // APP_LCD_ShowSystemInfo();

  // HAL_Delay(2000); // 等待2秒

  // // 清屏
  // LCD_Clear();

  // // 显示一些基本图形
  // // 画一个矩形
  // LCD_SetColor(LCD_RED);
  // LCD_DrawRect(10, 10, 50, 30);

  // // 画一个圆
  // LCD_SetColor(LCD_BLUE);
  // LCD_DrawCircle(100, 50, 20);

  // // 画一条线
  // LCD_SetColor(LCD_GREEN);
  // LCD_DrawLine(10, 100, 100, 150);

  // // 显示一些文本

  // LCD_SetColor(LCD_WHITE);

  // // for (int i = 0; i <= 3000; i++) 
  // // {
  // //   LCD_DisplayNumber(10, 180, i, 4);
  // // }

  // LCD_DisplayString(10, 210, "LCD Test Complete!");
  // LCD_Clear();


  //

    bmi088_init(&hspi1);
    imu_init();
    HAL_Delay(1000);
}

/**
 * @brief  显示欢迎信息
 * @param  None
 * @retval None
 */
void APP_LCD_ShowWelcome(void) {
  // 设置背景色为白色
  LCD_Clear();

  // 显示欢迎标题
  LCD_SetColor(LCD_BLUE);
  LCD_DisplayString(10, 30, "Projet: ");
  LCD_DisplayString(10, 60, "STM32F4-LCD");

  // 显示版本信息
  LCD_SetColor(LCD_BLUE);
  LCD_DisplayString(10, 100, "LCD Demo v1.0");

  // 显示作者信息
  LCD_DisplayString(10, 130, "By XTX");
  LCD_DisplayString(10, 160, "From ICB");
}

/**
 * @brief  显示系统信息
 * @param  None
 * @retval None
 */
void APP_LCD_ShowSystemInfo(void) {
  char buffer[50];

  // 显示标题
  LCD_SetColor(LCD_RED);
  LCD_DisplayString(10, 20, "System Info");

  // 显示芯片信息
  LCD_SetColor(LCD_RED);
  LCD_DisplayString(10, 54, "MCU: STM32F407");

  // 显示时钟信息
  LCD_DisplayString(10, 78, "SYSCLK: 168MHz");

  // 显示LCD信息
  LCD_DisplayString(10, 102, "LCD: 240x240");

  // 显示SPI接口信息
  LCD_DisplayString(10, 106, "Interface: SPI3");

  // 显示时间计数器
  static int counter = 0;
  counter++;
  sprintf(buffer, "Run Count: %d", counter);
  LCD_DisplayString(10, 130, buffer);
}


  float accel_data[3]; // 存储加速度数据 [x, y, z] 单位: g
  float gyro_data[3];  // 存储陀螺仪数据 [x, y, z] 单位: °/s
  euler_angle_t angle; // 存储姿态角度
void app_lcd_bmi088_test(void)
{
  
  // 读取传感器数据
  bmi088_read_accel(accel_data);
  bmi088_read_gyro(gyro_data);
  
  // 计算时间间隔
  float dt = 0.02;

  
  // 更新姿态解算
  imu_update(accel_data, gyro_data, dt);
  
  // 获取姿态角度
  imu_get_angle(&angle);  
  bmi088_flag = 0;
  // 显示传感器原始数据

  // LCD_SetColor(LCD_WHITE);
  // LCD_DisplayString(10, 10, "BMI088 Data:");
  
  // LCD_SetColor(LCD_GREEN);
  // LCD_DisplayString(10, 30, "AX");
  // LCD_DisplayString(10, 50, "AY");
  // LCD_DisplayString(10, 70, "AZ");
  // LCD_DisplayString(10, 90, "AX");
  // LCD_DisplayString(10, 110, "AY");
  // LCD_DisplayString(10, 130, "AZ");

  // LCD_DisplayNumber(80, 30, accel_data[0]*100,4);
  // LCD_DisplayNumber(80, 50, accel_data[1]*100,4);
  // LCD_DisplayNumber(80, 70, accel_data[2]*100,4);
  // LCD_DisplayNumber(80, 90, gyro_data[0],4);
  // LCD_DisplayNumber(80, 110, gyro_data[1],4);
  // LCD_DisplayNumber(80, 130, gyro_data[2],4);
  
  
  // // 显示姿态角度
  // LCD_SetColor(LCD_YELLOW);
  // LCD_DisplayString(10, 150, "Attitude:");
  
  // LCD_SetColor(LCD_CYAN);
  // LCD_DisplayString(10, 170, "ROLL");
  // LCD_DisplayString(10, 190, "PITCH");
  // LCD_DisplayString(10, 210, "YAW");

  // LCD_DisplayNumber(80, 170, angle.roll, 4);
  // LCD_DisplayNumber(80, 190          , angle.pitch,4);
  // LCD_DisplayNumber(80, 210, angle.yaw,  4);

  //printf("jy901b:\r\n");
  printf("bmi088: %d.%02d, %d.%02d, %d.%02d\r\n",
    (int)angle.roll,abs((int)(angle.roll * 100) % 100),
    (int)angle.pitch,abs((int)(angle.pitch * 100) % 100),
    (int)angle.yaw,abs((int)(angle.yaw * 100) % 100));
  // printf("bmi088: %d, %d, %d\r\n",(int)(accel_data[0]),(int)(accel_data[1]),(int)(accel_data[2]));
 // printf("jy901b: %f, %f, %f\r\n",fAngle[0], fAngle[1], fAngle[2]);
}