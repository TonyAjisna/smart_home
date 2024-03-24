#ifndef __BSP_MOTO_H__
#define __BSP_MOTO_H__
 
#include "stm32f1xx.h"
 
#define HIGH                        1
#define LOW                         0
 
#define GPIO_MOTO_CLK               RCC_APB2Periph_GPIOA
#define GPIO_MOTO_PORT              GPIOA
#define GPIO_MOTO_Pin_IN1           GPIO_PIN_2
#define GPIO_MOTO_Pin_IN2           GPIO_PIN_3
 
#define IN1(a)\
    if(a)   GPIO_SetBits(GPIO_MOTO_PORT, GPIO_MOTO_Pin_IN1);\
    else    GPIO_ResetBits(GPIO_MOTO_PORT, GPIO_MOTO_Pin_IN1);
     
#define IN2(a)\
    if(a)   GPIO_SetBits(GPIO_MOTO_PORT, GPIO_MOTO_Pin_IN2);\
    else    GPIO_ResetBits(GPIO_MOTO_PORT, GPIO_MOTO_Pin_IN2);
     
 
/*GPIO端口初始化*/
void GPIO_Moto_Init(void);
 
/*Moto1正转*/
void Moto1_Forward(void);
 
/*Moto1反转*/
void Moto1_Reverse(void);
 
#endif  /*__BSP_MOTO_H__*/