#ifndef __BT04_E_H
#define __BT04_E_H

/* 蓝牙模块驱动 */
#include "stm32f10x.h"

/* 引脚定义 */
#define BT04_E_STA_GPIO_PORT            GPIOC
#define BT04_E_STA_GPIO_PIN             GPIO_PIN_4
#define BT04_E_STA_GPIO_CLK             RCC_APB2Periph_GPIOC
#define BT04_E_STA_GPIO_CLK_ENABLE()    do{   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); } while (0)
#define BT04_E_WKUP_GPIO_PORT           GPIOD
#define BT04_E_WKUP_GPIO_PIN            GPIO_PIN_4
#define BT04_E_WKUP_GPIO_CLK            RCC_APB2Periph_GPIOD
#define BT04_E_WKUP_GPIO_CLK_ENABLE()   do{   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); } while (0)


/* IO操作 */
#define BT04_E_READ_STA()                HAL_GPIO_ReadPin(ATK_MW579_STA_GPIO_PORT, ATK_MW579_STA_GPIO_PIN)
#define BT04_E_WKUP(x)                   do{ x ?                                                                                     \
                                                HAL_GPIO_WritePin(ATK_MW579_WKUP_GPIO_PORT, ATK_MW579_WKUP_GPIO_PIN, GPIO_PIN_SET) :    \
                                                HAL_GPIO_WritePin(ATK_MW579_WKUP_GPIO_PORT, ATK_MW579_WKUP_GPIO_PIN, GPIO_PIN_RESET);   \
                                            }while(0)

#endif
