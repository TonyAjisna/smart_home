#ifndef __BSP_MOTO_H__
#define __BSP_MOTO_H__
 
#include "stm32f1xx.h"
#include "./stm32f1xx_hal_gpio.h"
#include "./SYSTEM/sys/sys.h"
 
#define HIGH                        1
#define LOW                         0

#define GPIO_MOTO_PORT              GPIOA
#define GPIO_MOTO_Pin_IN1           GPIO_PIN_2
#define GPIO_MOTO_Pin_IN2           GPIO_PIN_3
#define GPIO_MOTO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* 发送引脚时钟使能 */

 
#define IN1(x)   do{ x ? \
                      HAL_GPIO_WritePin(GPIO_MOTO_PORT, GPIO_MOTO_Pin_IN1, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(GPIO_MOTO_PORT, GPIO_MOTO_Pin_IN1, GPIO_PIN_RESET); \
                  }while(0)     //拉低引脚
     
#define IN2(x)   do{ x ? \
                      HAL_GPIO_WritePin(GPIO_MOTO_PORT, GPIO_MOTO_Pin_IN2, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(GPIO_MOTO_PORT, GPIO_MOTO_Pin_IN2, GPIO_PIN_RESET); \
                  }while(0)     //拉低引脚
     
 
/*GPIO端口初始化*/
void GPIO_Moto_Init(void);
 
/*Moto1正转*/
void Moto1_Forward(void);
 
/*Moto1反转*/
void Moto1_Reverse(void);



 
#endif  /*__BSP_MOTO_H__*/