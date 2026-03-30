#ifndef BSP_IMU_H
#define BSP_IMU_H

#include "stm32f4xx_hal.h"

// 欧拉角结构体
typedef struct {
    float roll;   // 滚转角 (绕X轴)
    float pitch;  // 俯仰角 (绕Y轴)
    float yaw;    // 偏航角 (绕Z轴)
} euler_angle_t;

// 互补滤波器结构体
typedef struct {
    euler_angle_t angle;           // 当前角度
    float alpha;                   // 互补滤波系数
    uint32_t last_update;          // 上次更新时间
} complementary_filter_t;

// 函数声明
void imu_init(void);
void imu_update(float *accel, float *gyro, float dt);
void imu_get_angle(euler_angle_t *angle);
void complementary_filter_init(complementary_filter_t *filter, float alpha);
void complementary_filter_update(complementary_filter_t *filter, 
                                float *accel, float *gyro, float dt);

#endif // BSP_IMU_H