#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "demo.h"
#include "bsp_moto.h"
#include <stdint.h>
#include "./stm32f1xx_hal_rcc_ex.h"

void show_mesg(void)
{
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
    uint8_t key = 0;
    HAL_Init();                                             
    sys_stm32_clock_init(RCC_PLL_MUL9);                     
    delay_init(72);                                         
    usart_init(115200);                                     
    led_init();                                             
    show_mesg();       
    key_init();                  
    // demo_run();                      
    GPIO_Moto_Init();

    while (1)
    {
        key = key_scan(0);
        switch (key)
        {
            case KEY0_PRES:                                     /* KEY0被按下 */
            {
                Moto1_Forward();                                  /* LED0状态翻转 */
                delay_ms(1000);
                break;
            }
            case WKUP_PRES:                                     /* KEY_UP被按下 */
            {
                Moto1_Reverse();                                  /* LED1状态翻转 */
                delay_ms(1000);
                break;
            }
            default:
            {
                break;
            }
        }
        // LED0(0);
        // delay_ms(1000);
        // LED0(1);
        // delay_ms(1000);
    }
}
