//=============================================================================
//�ļ����ƣ�main.h
//���ܸ�Ҫ��STM32F103C8���ļ��
//��Ȩ���У�Դ�ع�����www.vcc-gnd.com
//��Ȩ���£�2013-02-20
//���Է�ʽ��J-Link OB ARM SW��ʽ 5MHz
//=============================================================================

//ͷ�ļ�
#include "stm32f10x.h"
#include "GPIOLIKE51.h"
#include "./bsp_moto.h"

//��������
void GPIO_Configuration(void);

//=============================================================================
//�ļ����ƣ�Delay
//���ܸ�Ҫ����ʱ
//����˵����nCount����ʱ����
//�������أ���
//=============================================================================

void Delay(uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

// �ӳ�time����
void Delay_ms(uint16_t time)
{  
    u16 i=0;
    while(time--)
    {
        i=10000;        //�Լ�����
        while(i--) ;  
    }
}

//=============================================================================
//�ļ����ƣ�main
//���ܸ�Ҫ��������
//����˵������
//�������أ�int
//=============================================================================
int main(void)
{
	  // GPIO_Configuration();
    GPIO_Moto_Init();
    while(1)
    {
        // Moto1��ת2��
        Moto1_Forward();
        Delay_ms(2000);
         
        // Moto1ֹͣ2��
        Moto1_Pause();
        Delay_ms(2000);
                 
        // Moto1��ת2��
        Moto1_Reverse();
        Delay_ms(2000);
         
        // Moto1ֹͣ2��
        Moto1_Pause();
        Delay_ms(2000);
    }
}

//=============================================================================
//�ļ����ƣ�GPIO_Configuration
//���ܸ�Ҫ��GPIO��ʼ��
//����˵������
//�������أ���
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



