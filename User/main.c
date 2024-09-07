#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/TIM/gtim.h"
#include "./BSP/HCSR04/hcsr04.h"
#include "demo.h"


int main(void)
{
    HAL_Init();                                             
    sys_stm32_clock_init(RCC_PLL_MUL9);                     
    delay_init(72);                                         
    usart_init(115200);                                     
    led_init();    
    gtim_timx_init(10 - 1, 84 - 1); /* 84 000 000 / 84  = 1000 000 1000Khz的计数频率，计数10次为10us */                                         
    HC_SR04_Init();
    
    while (1)
    {
        // LED0(0);                                            
        // delay_ms(200);
        // LED0(1);                                            
        // delay_ms(200);

        int Distance_mm=sonar_mm();			//获取距离测量结果，单位毫米（mm）
        if (Distance_mm >= 100)
        {
            LED0(0);
        }
        else
        {
            LED0(1);
        }
        delay_ms(300);						
    }
}
