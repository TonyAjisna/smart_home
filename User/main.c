//=============================================================================
//文件名称：main.h
//功能概要：STM32F103C8核心检测
//版权所有：源地工作室www.vcc-gnd.com
//版权更新：2013-02-20
//调试方式：J-Link OB ARM SW方式 5MHz
//=============================================================================

//头文件
#include "stm32f10x.h"
#include "GPIOLIKE51.h"
#include "./bsp_moto.h"

//函数声明
void GPIO_Configuration(void);

//=============================================================================
//文件名称：Delay
//功能概要：延时
//参数说明：nCount：延时长短
//函数返回：无
//=============================================================================

void Delay(uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

// 延迟time毫秒
void Delay_ms(uint16_t time)
{  
    u16 i=0;
    while(time--)
    {
        i=10000;        //自己定义
        while(i--) ;  
    }
}

//=============================================================================
//文件名称：main
//功能概要：主函数
//参数说明：无
//函数返回：int
//=============================================================================
int main(void)
{
	  // GPIO_Configuration();
    GPIO_Moto_Init();
    while(1)
    {
        // Moto1正转2秒
        Moto1_Forward();
        Delay_ms(2000);
         
        // Moto1停止2秒
        Moto1_Pause();
        Delay_ms(2000);
                 
        // Moto1反转2秒
        Moto1_Reverse();
        Delay_ms(2000);
         
        // Moto1停止2秒
        Moto1_Pause();
        Delay_ms(2000);
    }
}

//=============================================================================
//文件名称：GPIO_Configuration
//功能概要：GPIO初始化
//参数说明：无
//函数返回：无
//=============================================================================
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC , ENABLE); 						 
//=============================================================================
//LED -> PC13
//=============================================================================			 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}



