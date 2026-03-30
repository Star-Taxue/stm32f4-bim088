#include "../BSP/Inc/bsp_imu.h"
#include "main.h"
#include <math.h>

// 互补滤波器实例
static complementary_filter_t imu_filter;

/**
 * @brief 初始化IMU姿态解算
 * @param None
 * @retval None
 */
void imu_init(void) {
    // 初始化互补滤波器，滤波系数为0.98
    complementary_filter_init(&imu_filter, 0.98f);
}

/**
 * @brief 初始化互补滤波器
 * @param filter 滤波器结构体指针
 * @param alpha 滤波系数 (0 < alpha < 1)
 * @retval None
 */
void complementary_filter_init(complementary_filter_t *filter, float alpha) {
    // 初始化角度为0
    filter->angle.roll = 0.0f;
    filter->angle.pitch = 0.0f;
    filter->angle.yaw = 0.0f;
    
    // 设置滤波系数
    filter->alpha = alpha;
    
    // 初始化时间戳
    filter->last_update = HAL_GetTick();
}

/**
 * @brief 更新IMU姿态
 * @param accel 加速度计数据 [x, y, z] (单位: g)
 * @param gyro 陀螺仪数据 [x, y, z] (单位: °/s)
 * @param dt 时间间隔 (单位: 秒)
 * @retval None
 */
void imu_update(float *accel, float *gyro, float dt) {
    complementary_filter_update(&imu_filter, accel, gyro, dt);
}

/**
 * @brief 互补滤波更新
 * @param filter 滤波器结构体指针
 * @param accel 加速度计数据 [x, y, z] (单位: g)
 * @param gyro 陀螺仪数据 [x, y, z] (单位: °/s)
 * @param dt 时间间隔 (单位: 秒)
 * @retval None
 */
// 改进的互补滤波器更新函数
void complementary_filter_update(complementary_filter_t *filter, 
                                float *accel, float *gyro, float dt) {
    // 1. 首先对加速度计数据进行低通滤波，减少噪声
    static float accel_filtered[3] = {0};
    float alpha = 0.8f; // 低通滤波系数
    
    for(int i = 0; i < 3; i++) {
        accel_filtered[i] = alpha * accel_filtered[i] + (1 - alpha) * accel[i];
    }
    
    // 2. 检查加速度计数据有效性（静止时模长应接近1g）
    float accel_magnitude = sqrtf(accel_filtered[0]*accel_filtered[0] + 
                                 accel_filtered[1]*accel_filtered[1] + 
                                 accel_filtered[2]*accel_filtered[2]);
    
    // 3. 计算加速度计角度（只在数据有效时使用）
    float accel_roll, accel_pitch;
    
    if(fabs(accel_magnitude - 1.0f) < 0.3f) { // 数据有效范围：0.7g - 1.3g
        // 计算 roll (绕X轴旋转) - 修正符号
        accel_roll = atan2f(accel_filtered[1], accel_filtered[2]) * 180.0f / M_PI;
        
        // 计算 pitch (绕Y轴旋转) - 修正符号
        accel_pitch = atan2f(-accel_filtered[0], 
                            sqrtf(accel_filtered[1]*accel_filtered[1] + 
                                  accel_filtered[2]*accel_filtered[2])) * 180.0f / M_PI;
    } else {
        // 数据无效，完全依赖陀螺仪
        accel_roll = filter->angle.roll;
        accel_pitch = filter->angle.pitch;
    }
    
    // 4. 陀螺仪积分
    float gyro_roll = filter->angle.roll + gyro[0] * dt;
    float gyro_pitch = filter->angle.pitch + gyro[1] * dt;
    float gyro_yaw = filter->angle.yaw + gyro[2] * dt;
    
    // 5. 互补滤波融合 - 根据数据有效性调整系数
    float effective_alpha = (fabs(accel_magnitude - 1.0f) < 0.3f) ? filter->alpha : 0.99f;
    
    filter->angle.roll = effective_alpha * gyro_roll + (1.0f - effective_alpha) * accel_roll;
    filter->angle.pitch = effective_alpha * gyro_pitch + (1.0f - effective_alpha) * accel_pitch;
    filter->angle.yaw = gyro_yaw;
    
    // 6. 角度限制
    if (filter->angle.roll > 180.0f) filter->angle.roll -= 360.0f;
    if (filter->angle.roll < -180.0f) filter->angle.roll += 360.0f;
    if (filter->angle.pitch > 180.0f) filter->angle.pitch -= 360.0f;
    if (filter->angle.pitch < -180.0f) filter->angle.pitch += 360.0f;
    if (filter->angle.yaw > 180.0f) filter->angle.yaw -= 360.0f;
    if (filter->angle.yaw < -180.0f) filter->angle.yaw += 360.0f;
}

/**
 * @brief 获取当前姿态角度
 * @param angle 角度结构体指针
 * @retval None
 */
void imu_get_angle(euler_angle_t *angle) {
    angle->roll = imu_filter.angle.roll;
    angle->pitch = imu_filter.angle.pitch;
    angle->yaw = imu_filter.angle.yaw;
}