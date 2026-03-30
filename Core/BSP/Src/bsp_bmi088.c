#include "../BSP/Inc/bsp_bmi088.h"
#include "main.h"
#include <math.h>
#include "usart.h"
#include <stdio.h>
#include <string.h>
static SPI_HandleTypeDef *hspi_bmi088;

// PI读写函数，处理MISO线共享
// 加速度计SPI读取
static uint8_t bmi088_spi_read_accel(uint8_t reg) {
  uint8_t tx_data[2] = {(uint8_t)(reg | 0x80), 0x00};
  uint8_t rx_data[2] = {0};

  HAL_GPIO_WritePin(ACC_CS_PORT, ACC_CS_PIN, GPIO_PIN_RESET);
  for(int i = 0; i < 100; i++) {
    __NOP(); // 空操作，实现精确延迟
  }

  // 加速度计SPI读取：发送2字节，接收2字节，第一个是哑字节
  HAL_SPI_TransmitReceive(hspi_bmi088, tx_data, rx_data, 2, 100);

  HAL_GPIO_WritePin(ACC_CS_PORT, ACC_CS_PIN, GPIO_PIN_SET);
  for(int i = 0; i < 100; i++) {
    __NOP();
  }

  return rx_data[1]; // 返回第二个字节（有效数据）
}

static void bmi088_spi_write_accel(uint8_t reg, uint8_t data) {
  uint8_t tx_data[2] = {(uint8_t)(reg & 0x7F), data};

  HAL_GPIO_WritePin(ACC_CS_PORT, ACC_CS_PIN, GPIO_PIN_RESET);
  HAL_Delay(10);

  HAL_SPI_Transmit(hspi_bmi088, tx_data, 2, 100);

  HAL_GPIO_WritePin(ACC_CS_PORT, ACC_CS_PIN, GPIO_PIN_SET);
  HAL_Delay(20); // 写操作后需要等待
}

// 陀螺仪SPI读取函数
static uint8_t bmi088_spi_read_gyro(uint8_t reg) {
  uint8_t tx_data[2] = {(uint8_t)(reg | 0x80), 0x00}; // 读命令 + 哑字节
  uint8_t rx_data[2] = {0};

  HAL_GPIO_WritePin(GYRO_CS_PORT, GYRO_CS_PIN, GPIO_PIN_RESET);
  HAL_Delay(10);

  // 陀螺仪SPI读取：发送2字节，接收2字节
  HAL_SPI_TransmitReceive(hspi_bmi088, tx_data, rx_data, 2, 100);

  HAL_GPIO_WritePin(GYRO_CS_PORT, GYRO_CS_PIN, GPIO_PIN_SET);
  HAL_Delay(10);

  return rx_data[1]; // 第二个字节是有效数据
}

static void bmi088_spi_write_gyro(uint8_t reg, uint8_t data) {
  uint8_t tx_data[2] = {(uint8_t)(reg & 0x7F), data};

  HAL_GPIO_WritePin(GYRO_CS_PORT, GYRO_CS_PIN, GPIO_PIN_RESET);
  HAL_Delay(10);

  HAL_SPI_Transmit(hspi_bmi088, tx_data, 2, 100);

  HAL_GPIO_WritePin(GYRO_CS_PORT, GYRO_CS_PIN, GPIO_PIN_SET);
  HAL_Delay(20);
}

uint8_t bmi088_acc_init(void) {
  uint8_t acc_id = 0;
  int retry_count = 0;
  // 步骤1：确保CSB1初始为高
  HAL_GPIO_WritePin(ACC_CS_PORT, ACC_CS_PIN, GPIO_PIN_SET);
  HAL_Delay(10);
  
  // 步骤2：产生下降沿
  HAL_GPIO_WritePin(ACC_CS_PORT, ACC_CS_PIN, GPIO_PIN_RESET);
  HAL_Delay(10);
  
  // 步骤3：发送一个SPI命令（读芯片ID）来激活SPI模式
  uint8_t tx_data[2] = {BMI088_ACC_CHIP_ID | 0x80, 0x00};
  uint8_t rx_data[2] = {0};
  HAL_SPI_TransmitReceive(hspi_bmi088, tx_data, rx_data, 2, 100);
  
  // 步骤4：产生上升沿，完成SPI模式切换
  HAL_GPIO_WritePin(ACC_CS_PORT, ACC_CS_PIN, GPIO_PIN_SET);
  HAL_Delay(10);

  // 软复位
  bmi088_spi_write_accel(BMI088_ACC_SOFTRESET, 0xB6);
  HAL_Delay(50); // 等待复位完成

  // 检查芯片ID
  do {
    acc_id = bmi088_spi_read_accel(BMI088_ACC_CHIP_ID);
    HAL_Delay(10);
    retry_count++;
  } while (acc_id != BMI088_ACC_CHIP_ID_VALUE && retry_count < 10);

  if (acc_id != BMI088_ACC_CHIP_ID_VALUE) {
    return 0;
  }

  HAL_Delay(1); // 等待1ms（数据手册要求）

  // 正确的初始化顺序（数据手册第12页）
  // 1. 配置电源模式
  bmi088_spi_write_accel(BMI088_ACC_PWR_CONF, 0x00); // 退出节能模式
  HAL_Delay(5);
  
  // 2. 配置测量参数
  bmi088_spi_write_accel(BMI088_ACC_CONF, 0xA8); // ODR=100Hz, 正常滤波模式
  HAL_Delay(5);
  
  bmi088_spi_write_accel(BMI088_ACC_RANGE, 0x01); // ±6g范围
  HAL_Delay(5);

  // 3. 使能传感器（关键步骤）
  bmi088_spi_write_accel(BMI088_ACC_PWR_CTRL, 0x04);
  HAL_Delay(50); // 等待50ms稳定（数据手册要求）

  return 1;
}
// 陀螺仪初始化
uint8_t bmi088_gyro_init(void) {

  HAL_Delay(1000);

  // 检查芯片ID
  uint8_t gyro_id = bmi088_spi_read_gyro(BMI088_GYRO_CHIP_ID);
  if (gyro_id != BMI088_GYRO_CHIP_ID_VALUE) {
    return 0;
  }

  // 配置陀螺仪
  // GYRO_RANGE: ±2000dps
  bmi088_spi_write_gyro(BMI088_GYRO_RANGE, 0x00);
  HAL_Delay(10);

  // GYRO_BANDWIDTH: ODR=1000Hz, 带宽=116Hz
  bmi088_spi_write_gyro(BMI088_GYRO_BANDWIDTH, 0x02);
  HAL_Delay(10);

  // 陀螺仪默认在正常模式，无需额外配置

  return 1;
}

// 初始化 BMI088
void bmi088_init(SPI_HandleTypeDef *hspi) {
  hspi_bmi088 = hspi;

  // 初始化片选引脚
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = ACC_CS_PIN | GYRO_CS_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  // 默认拉高片选信号
  HAL_GPIO_WritePin(ACC_CS_PORT, ACC_CS_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GYRO_CS_PORT, GYRO_CS_PIN, GPIO_PIN_SET);

  HAL_Delay(200);

  // 初始化加速度计和陀螺仪
  if (!bmi088_acc_init() || !bmi088_gyro_init()) {
    // 初始化失败处理
    Error_Handler();
  }
}

// 加速度计数据读取
void bmi088_read_accel(float *accel) {
    uint8_t rx_data[7] = {0};
    
    HAL_GPIO_WritePin(ACC_CS_PORT, ACC_CS_PIN, GPIO_PIN_RESET);
    HAL_Delay(1);
    
    // 从ACC_X_LSB(0x12)开始读取7个字节
    uint8_t tx_cmd = BMI088_ACC_X_LSB | 0x80;
    
    if (HAL_SPI_TransmitReceive(hspi_bmi088, &tx_cmd, rx_data, 7, 100) != HAL_OK) {
        HAL_GPIO_WritePin(ACC_CS_PORT, ACC_CS_PIN, GPIO_PIN_SET);
        accel[0] = accel[1] = accel[2] = 0.0f;
        return;
    }
    
    HAL_GPIO_WritePin(ACC_CS_PORT, ACC_CS_PIN, GPIO_PIN_SET);

    // 正确解析顺序（数据手册第20页）
    // 注意：读取LSB会锁定MSB寄存器
    int16_t raw_x = (int16_t)((rx_data[1] << 8) | rx_data[2]);
    int16_t raw_y = (int16_t)((rx_data[3] << 8) | rx_data[4]);
    int16_t raw_z = (int16_t)((rx_data[5] << 8) | rx_data[6]);
    // printf("Raw - X: %d, %d, Z%d\r\n", raw_x, raw_y, raw_z);
    // printf("bmi088: %d, %d, %d\r\n",(int)(accel_data[0]),(int)(accel_data[1]),(int)(accel_data[2]));
    // 转换为g值（±6g量程，灵敏度5460 LSB/g - 数据手册第8页）
    accel[0] = (float)raw_y / 5460.0f;
    accel[1] = (float)raw_z / 5460.0f;
    accel[2] = (float)raw_x / 5460.0f;
}

// 陀螺仪数据读取
void bmi088_read_gyro(float *gyro) {
    uint8_t rx_data[7] = {0};
    
    HAL_GPIO_WritePin(GYRO_CS_PORT, GYRO_CS_PIN, GPIO_PIN_RESET);
    HAL_Delay(1);
    
    // 从RATE_X_LSB(0x02)开始读取7个字节
    uint8_t tx_cmd = BMI088_GYRO_RATE_X_LSB | 0x80;
    
    if (HAL_SPI_TransmitReceive(hspi_bmi088, &tx_cmd, rx_data, 7, 100) != HAL_OK) {
        HAL_GPIO_WritePin(GYRO_CS_PORT, GYRO_CS_PIN, GPIO_PIN_SET);
        gyro[0] = gyro[1] = gyro[2] = 0.0f;
        return;
    }
    
    HAL_GPIO_WritePin(GYRO_CS_PORT, GYRO_CS_PIN, GPIO_PIN_SET);

    // 陀螺仪数据解析（数据手册第27页）
    int16_t raw_x = (int16_t)((rx_data[2] << 8) | rx_data[1]);  // X_MSB + X_LSB
    int16_t raw_y = (int16_t)((rx_data[4] << 8) | rx_data[3]);  // Y_MSB + Y_LSB
    int16_t raw_z = (int16_t)((rx_data[6] << 8) | rx_data[5]);  // Z_MSB + Z_LSB

    // 转换为°/s（±2000dps量程，灵敏度16.384 LSB/°/s - 数据手册第28页）
    gyro[0] = (float)raw_x / 16.384f;
    gyro[1] = (float)raw_y / 16.384f;
    gyro[2] = (float)raw_z / 16.384f;
}





