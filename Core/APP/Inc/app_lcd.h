#ifndef __APP_LCD_H
#define __APP_LCD_H

#ifdef __cplusplus
extern "C" {
#endif

/* 包含的头文件 */
#include "bsp_lcd.h"


/* 宏定义 */


/* 枚举与联合体 */


/* 结构体定义 */


/* Extern变量声明 */
extern int bmi088_flag;


/* 公用函数声明 */
void APP_LCD_Test(void);
void APP_LCD_ShowWelcome(void);
void APP_LCD_ShowSystemInfo(void);
void app_lcd_bmi088_test(void);



#ifdef __cplusplus
}
#endif

#endif /* __APP_LCD_H */