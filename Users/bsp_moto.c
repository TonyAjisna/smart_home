#include "bsp_moto.h"
#include "./stm32f1xx_hal.h"
#include "./stm32f103xe.h"
#include "./SYSTEM/sys/sys.h"
 
/*GPIO端口初始化*/
void GPIO_Moto_Init(void)
{
    // 结构体
    GPIO_InitTypeDef GPIO_InitStruct;
      
    // 开时钟
    GPIO_MOTO_CLK_ENABLE();
      
    // 实例化
    GPIO_InitStruct.Pin = GPIO_MOTO_Pin_IN1|GPIO_MOTO_Pin_IN2;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
      
    // 初始化
    HAL_GPIO_Init(GPIO_MOTO_PORT, &GPIO_InitStruct);
     
    GPIO_ResetBits(GPIO_MOTO_PORT, GPIO_MOTO_Pin_IN1|GPIO_MOTO_Pin_IN2);
}
 
/*Moto1正转*/
void Moto1_Forward(void)
{
    IN1(HIGH);
    IN2(LOW);
}
 
/*Moto1反转*/
void Moto1_Reverse(void)
{
    IN1(LOW);
    IN2(HIGH);
}
