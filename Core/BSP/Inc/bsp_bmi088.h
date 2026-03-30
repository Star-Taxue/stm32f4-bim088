#ifndef BMI088_H
#define BMI088_H

#include "stm32f4xx_hal.h"

// 片选引脚定义
#define ACC_CS_PIN      GPIO_PIN_0
#define ACC_CS_PORT     GPIOB
#define GYRO_CS_PIN     GPIO_PIN_1
#define GYRO_CS_PORT    GPIOB

// BMI088 寄存器地址（根据数据手册修正）
// BMI088 加速度计寄存器地址
#define BMI088_ACC_CHIP_ID            0x00
#define BMI088_ACC_CHIP_ID_VALUE      0x1E
#define BMI088_ACC_ERR_REG            0x02
#define BMI088_ACC_STATUS             0x03
#define BMI088_ACC_DATA               0x12
#define BMI088_ACC_CONF               0x40
#define BMI088_ACC_RANGE              0x41
#define BMI088_ACC_PWR_CONF           0x7C
#define BMI088_ACC_PWR_CTRL           0x7D
#define BMI088_ACC_SOFTRESET          0x7E

// BMI088 陀螺仪寄存器地址
#define BMI088_GYRO_CHIP_ID           0x00
#define BMI088_GYRO_CHIP_ID_VALUE     0x0F
#define BMI088_GYRO_DATA              0x02
#define BMI088_GYRO_RANGE             0x0F
#define BMI088_GYRO_BANDWIDTH         0x10

// bsp_bmi088.h 中的寄存器定义需要更新
#define BMI088_ACC_CHIP_ID          0x00
#define BMI088_ACC_ERR_REG          0x02
#define BMI088_ACC_STATUS           0x03
#define BMI088_ACC_X_LSB            0x12  // 加速度X轴LSB
#define BMI088_ACC_X_MSB            0x13  // 加速度X轴MSB  
#define BMI088_ACC_Y_LSB            0x14  // 加速度Y轴LSB
#define BMI088_ACC_Y_MSB            0x15  // 加速度Y轴MSB
#define BMI088_ACC_Z_LSB            0x16  // 加速度Z轴LSB
#define BMI088_ACC_Z_MSB            0x17  // 加速度Z轴MSB
#define BMI088_ACC_SENSORTIME_0     0x18
#define BMI088_ACC_SENSORTIME_1     0x19
#define BMI088_ACC_SENSORTIME_2     0x1A
#define BMI088_ACC_INT_STAT_1       0x1D
#define BMI088_ACC_TEMP_MSB         0x22
#define BMI088_ACC_TEMP_LSB         0x23
#define BMI088_ACC_CONF             0x40
#define BMI088_ACC_RANGE            0x41
#define BMI088_ACC_INT1_IO_CONF     0x53
#define BMI088_ACC_INT2_IO_CONF     0x54
#define BMI088_ACC_INT_MAP_DATA     0x58
#define BMI088_ACC_SELF_TEST        0x6D
#define BMI088_ACC_PWR_CONF         0x7C
#define BMI088_ACC_PWR_CTRL         0x7D
#define BMI088_ACC_SOFTRESET        0x7E

#define BMI088_GYRO_CHIP_ID         0x00
#define BMI088_GYRO_RATE_X_LSB      0x02  // 陀螺仪X轴LSB
#define BMI088_GYRO_RATE_X_MSB      0x03  // 陀螺仪X轴MSB
#define BMI088_GYRO_RATE_Y_LSB      0x04  // 陀螺仪Y轴LSB
#define BMI088_GYRO_RATE_Y_MSB      0x05  // 陀螺仪Y轴MSB
#define BMI088_GYRO_RATE_Z_LSB      0x06  // 陀螺仪Z轴LSB
#define BMI088_GYRO_RATE_Z_MSB      0x07  // 陀螺仪Z轴MSB
#define BMI088_GYRO_INT_STAT_1      0x0A
#define BMI088_GYRO_RANGE           0x0F
#define BMI088_GYRO_BANDWIDTH       0x10
#define BMI088_GYRO_LPM1            0x11
#define BMI088_GYRO_SOFTRESET       0x14
#define BMI088_GYRO_INT_CTRL        0x15
#define BMI088_GYRO_INT3_INT4_CONF  0x16
#define BMI088_GYRO_INT3_INT4_MAP   0x18
#define BMI088_GYRO_SELF_TEST       0x3C




// 删除重复定义，保持一致性
#define BMI088_ACC_CHIP_ID_VALUE      0x1E
#define BMI088_GYRO_CHIP_ID_VALUE     0x0F



// 函数声明
void bmi088_init(SPI_HandleTypeDef *hspi);
void bmi088_read_accel(float *accel);
void bmi088_read_gyro(float *gyro);
uint8_t bmi088_acc_init(void);
uint8_t bmi088_gyro_init(void);

// 姿态解算相关函数声明
/**
  * @brief  更新姿态解算（通常在主循环中调用）
  * @param  accel: 指向包含加速度数据的数组 (g)
  * @param  gyro: 指向包含陀螺仪数据的数组 (°/s)
  * @param  dt: 时间间隔 (秒)
  * @retval None
  */
void bmi088_update_attitude(float *accel, float *gyro, float dt);

/**
  * @brief  获取当前的欧拉角 (俯仰、横滚、偏航)
  * @param  angles: 指向存储欧拉角的数组 (单位: 度)
  * @retval None
  */
void bmi088_get_euler_angles(float *angles);

/**
  * @brief  获取当前的四元数
  * @param  q: 指向存储四元数的数组 [q0, q1, q2, q3]
  * @retval None
  */
void bmi088_get_quaternion(float *q);

#endif // BMI088_H