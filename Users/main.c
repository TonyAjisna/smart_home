#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "demo.h"
#include "bsp_moto.h"
#include <stdint.h>
#include "./stm32f1xx_hal_rcc_ex.h"

void show_mesg(void)
{
    /* �������ʵ����Ϣ */
    printf("\n");
    printf("********************************\r\n");
    printf("STM32\r\n");
    printf("ATK-MW579\r\n");
    printf("ATOM@ALIENTEK\r\n");
    printf("********************************\r\n");
    printf("\r\n");
}

int main(void)
{
    uint32_t t = 0;
    HAL_Init();                                             /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);                     /* ����ʱ��, 72Mhz */
    delay_init(72);                                         /* ��ʱ��ʼ�� */
    usart_init(115200);                                     /* ��ʼ������ */
    led_init();                                             /* ��ʼ��LED */
    show_mesg();                        /* ��ʾʵ����Ϣ */
    // demo_run();                         /* ����ʾ������ */
    GPIO_Moto_Init();

    while (1)
    {
        Moto1_Forward();
        delay_ms(1000);
        Moto1_Reverse();
        delay_ms(1000);
    }
    
}
