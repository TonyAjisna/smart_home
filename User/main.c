/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MW579模块iBeacon模式测试实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 MiniSTM32 V4开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./BSP/LCD/lcd.h"
#include "demo.h"


int main(void)
{
    HAL_Init();                                             /* ???HAL? */
    sys_stm32_clock_init(RCC_PLL_MUL9);                     /* ????, 72Mhz */
    delay_init(72);                                         /* ????? */
    usart_init(115200);                                     /* ????? */
    led_init();                                             /* ???LED */
    
    while (1)
    {
        LED0(0);                                            /* LED0? */
        delay_ms(500);
        LED0(1);                                            /* LED0? */
        delay_ms(500);
    }
}
