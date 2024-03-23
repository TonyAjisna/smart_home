#include "./bsp_moto.h"
 
/*GPIO端口初始化*/
void GPIO_Moto_Init(void)
{
    // 结构体
    GPIO_InitTypeDef GPIO_InitStruct;
      
    // 开时钟
    RCC_APB2PeriphClockCmd(GPIO_MOTO_CLK, ENABLE);
      
    // 实例化
    GPIO_InitStruct.GPIO_Pin = GPIO_MOTO_Pin_IN1|GPIO_MOTO_Pin_IN2;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
      
    // 初始化
    GPIO_Init(GPIO_MOTO_PORT, &GPIO_InitStruct);
     
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