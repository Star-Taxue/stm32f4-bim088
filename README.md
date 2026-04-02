# BMI088_test


# 若是手动添加文件或文件夹，需要修改CMakeLists.txt文件中的以下两项
# 将c文件和头文件加入被编译项
# Add sources to executable
# target_sources(${CMAKE_PROJECT_NAME} PRIVATE
#     # Add user sources here
#     Core/DRV/Src/lcd_image.c
#     Core/DRV/Src/lcd_fonts.c
#     # Core/DRV/Src/lcd_spi_130.c
#     Core/DRV/Src/drv_lcd.c
#     Core/BSP/Src/bsp_bmi088.c
#     Core/BSP/Src/bsp_lcd.c
#     Core/APP/Src/app_lcd.c
#     Core/BSP/Src/bsp_imu.c
# )

# # Add include paths
# target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
#     # Add user defined include paths
#     Core/DRV/Inc
#     Core/BSP/Inc
#     Core/APP/Inc
# )

项目基于：STM32F407VET6,BMI088姿态传感器

测试项目：BMI088_test
具体测试内容：BMI088传感器数据采集，数据处理，数据可视化
测试工具：vscode + ST官方STM32开发插件 + VOFA+ 

测试结果：加速度计、角速度计、四元数解算并映射到欧拉角、互补滤波等一切正常 
