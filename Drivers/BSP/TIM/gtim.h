#ifndef __GTIM_H
#define __GTIM_H

#include "./SYSTEM/sys/sys.h"


/* 通用定时器 定义 */
#define GTIM_TIMX_INT                       TIM2
#define GTIM_TIMX_INT_IRQn                  TIM2_IRQn
#define GTIM_TIMX_INT_IRQHandler            TIM2_IRQHandler
#define GTIM_TIMX_INT_CLK_ENABLE()          do{ __HAL_RCC_TIM2_CLK_ENABLE(); }while(0)  /* TIM2时钟使能 */


/* 函数声明 */
void gtim_timx_init(uint16_t arr, uint16_t psc);        /* 通用定时器 定时中断初始化函数 */

#endif

















