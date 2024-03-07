#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./BSP/SRAM/sram.h"
#include "./MALLOC/malloc.h"
#include "./BSP/RTC/rtc.h"
#include "./BSP/TIMER/gtim.h"
#include "./BSP/ADC/adc.h"
#include "./BSP/SRAM/sram.h"
#include "./BSP/SDIO/sdio_sdcard.h"
#include "./MALLOC/malloc.h"
#include "./BSP/NORFLASH/norflash.h"
#include "./BSP/ES8388/es8388.h"
#include "./APP/audioplay.h"
#include "./BSP/TIMER_BASE/btim.h"
#include "./FATFS/exfuns/exfuns.h"
#include <stdbool.h>
#include "freertos_APP.h"

extern bool SD_Status;
extern bool fonts_Status;

int main(void)
{

    
    uint16_t ledrpwmval = 0;
    uint8_t dir = 1;  
    uint8_t status = 0;                         /* 标�?�是否响铃状�? */
    uint8_t status1 = 1;                        /* 标�?�闹钟是否生�? */
    uint8_t status2 = 1; 
    uint8_t key;
    uint8_t res = 0;
    uint8_t tbuf1[40];                          /* 响铃时间打印句柄 */
    uint8_t tbuf2[40];                          /* 倒�?�时打印句柄 */
    uint64_t t = 0;                              /* 设置程序心跳�?10ms */
    uint8_t t1 = 0;

    uint8_t alarm[3] = {12, 19, 0};                  /* 暂时写�?�响铃时�? */
    uint8_t alarm1[3] = {0, 0, 0}; 

    HAL_Init();                                 /* 初�?�化HAL�?? */
    sys_stm32_clock_init(336, 8, 2, 7);         /* 设置时钟,168Mhz */
    delay_init(168);                            /* 延时初�?�化 */
    usart_init(115200);                         /* 串口初�?�化�??115200 */
    led_init();                                 /* 初�?�化LED */
    lcd_init();                                 /* 初�?�化LCD */
    key_init();                                 /* 初�?�化按键 */
    sram_init();                                /* SRAM初�?�化 */
    my_mem_init(SRAMIN);                        /* 初�?�化内部SRAM内存�?? */
    my_mem_init(SRAMEX);                        /* 初�?�化外部SRAM内存�?? */
    my_mem_init(SRAMCCM);                       /* 初�?�化内部CCM内存�?? */

    adc_temperature_init();                     /* 初�?�化ADC */
    rtc_init();                                 /* 初�?�化rtc实时时钟 */
    gtim_timx_pwm_chy_init(500 - 1, 84 - 1);    /* 84 000 000 / 84 = 1 000 000 1Mhz的�?�数频率�?2Khz的PWM */
    btim_timx_int_init(5000 - 1, 8400 - 1);     /* 84 000 000 / 84 00 = 10 000 10Khz的�?�数频率，�?�数5K次为500ms */

    /* 检测不到SD�? */
    while (sd_init())
    {
        //  SD_Status = false;
    }

    /* 检测不到字�? */
    // while (fonts_init())
    // {
    //     fonts_Status = false;
    // }

    exfuns_init();                      /* 为fatfs相关变量申�?�内�? */
    f_mount(fs[0], "0:", 1);            /* 挂载SD�? */
    res = f_mount(fs[1], "1:", 1);      /* 挂载FLASH */

    if (res == 0X0D)                    /* FLASH磁盘，FAT文件系统错�??，重新格式化FLASH */
    {
        /* 格式�? FLASH,1:,盘�??;0,使用默�?�格式化参数 */
        res = f_mkfs("1:", 0, 0, FF_MAX_SS); 

        if (res == 0)
        {
            f_setlabel((const TCHAR *)"1:ALIENTEK");            /* 设置Flash磁盘的名字为ALIENTEK */
        }

        // vTaskDelay(1000);        
    }

    es8388_init();              /* ES8388初�?�化 */
    es8388_adda_cfg(1, 1);      /* 开启DAC和ADC */
    es8388_output_cfg(1, 1);    /* DAC选择通道输出 */
    es8388_hpvol_set(25);       /* 设置耳机音量  */
    es8388_spkvol_set(30);      /* 设置喇叭音量 */





    freertos_APP();                    /* 运�?�FreeRTOS例程 */
}
