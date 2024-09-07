#ifndef __HCSR04_H
#define __HCSR04_H

#include "common.h"
#include "./SYSTEM/sys/sys.h"

/* 首先定义tx和rx引脚 */
#define ECHO_PORT           GPIOA
#define ECHO_PIN            GPIO_PIN_7

#define TRIG_PORT           GPIOA
#define TRIG_PIN            GPIO_PIN_6

#define HY_SRF05_CLK_ENABLE()   do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)

/* IO操作函数 */
#define TRIG_OUT(x)     do{ x ? \
                                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET) : \
                                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET); \
                            }while(0)  
#define ECHO_IN         HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7)         /* 数据端口输入 */



void HC_SR04_Init(void);
uint16_t sonar_mm(void);
float sonar(void);
void TIM_IRQHandler(void);



#endif