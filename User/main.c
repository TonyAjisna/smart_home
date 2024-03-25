#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./USMART/usmart.h"
#include "./BSP/KEY/key.h"
#include "./BSP/SRAM/sram.h"
#include "./MALLOC/malloc.h"
#include "freertos_APP.h"
#include "lwip_demo.h"
#include "./BSP/DHT11/dht11.h"


int main(void)
{
    HAL_Init();                         
    sys_stm32_clock_init(336, 8, 2, 7); 
    delay_init(168);                    
    usart_init(115200);                 
    usmart_dev.init(84);               
    led_init();                        
    lcd_init();                        
    key_init();                        
    sram_init(); 
    adc_temperature_init();                 /* 初始化ADC */                      
    dht11_init();

    my_mem_init(SRAMIN);                
    my_mem_init(SRAMEX);                
    my_mem_init(SRAMCCM);               

    freertos_demo();                 
}


