#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./BSP/LCD/lcd.h"
#include "demo.h"


int main(void)
{
    HAL_Init();                                             
    sys_stm32_clock_init(RCC_PLL_MUL9);                     
    delay_init(72);                                         
    usart_init(115200);                                     
    led_init();                                             
    
    while (1)
    {
        LED0(0);                                            
        delay_ms(200);
        LED0(1);                                            
        delay_ms(200);
    }
}
