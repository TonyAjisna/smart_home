#include "freertos_APP.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/SDIO/sdio_sdcard.h"
#include "./BSP/ADC/adc.h"
#include "./BSP/RTC/rtc.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdbool.h>

/* ------------------------------应用逻辑代码编写区-------------------------------- */

/* 所有任务在这里创建 */

/* task任务配置
    包括：任务句柄、任务优先级、堆栈大小、创建任务
 */
#define START_TASK_PRIO 1                   /* 任务优先级 */
#define START_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            StartTask_Handler;  /* 任务句柄 */
void start_task(void *pvParameters);        /* 任务函数 */

#define TASK1_PRIO      2                   /* 任务优先级 */
#define TASK1_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            Task1Task_Handler;  /* 任务句柄 */
void task1(void *pvParameters);             /* 任务函数 */

#define TASK2_PRIO      3                   /* 任务优先级 */
#define TASK2_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            Task2Task_Handler;  /* 任务句柄 */
void task2(void *pvParameters);             /* 任务函数 */




void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();       /* 进入临界区 */

    /* 创建任务1 */
    xTaskCreate((TaskFunction_t )task1,
                (const char*    )"task1",
                (uint16_t       )TASK1_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK1_PRIO,
                (TaskHandle_t*  )&Task1Task_Handler);
    /* 创建任务2 */
    xTaskCreate((TaskFunction_t )task2,
                (const char*    )"task2",
                (uint16_t       )TASK2_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK2_PRIO,
                (TaskHandle_t*  )&Task2Task_Handler);
    vTaskDelete(StartTask_Handler); /* 删除开始任务 */

    taskEXIT_CRITICAL();            /* 退出临界区 */
}



/* LCD刷屏时使用的颜色 */
uint16_t lcd_discolor[11] = {WHITE, BLACK, BLUE, RED,
                             MAGENTA, GREEN, CYAN, YELLOW,
                             BROWN, BRRED, GRAY};


/* FreeRTOS例程入口函数 */
void freertos_APP(void)
{
    lcd_show_string(10, 10, 220, 32, 32, "STM32", RED);
    lcd_show_string(10, 47, 220, 24, 24, "FreeRTOS Porting", RED);
    lcd_show_string(10, 76, 220, 16, 16, "zhaobw@yeah.net", RED);
    
    xTaskCreate((TaskFunction_t )start_task,            /* 任务函数 */
                (const char*    )"start_task",          /* 任务名称 */
                (uint16_t       )START_STK_SIZE,        /* 任务堆栈大小 */
                (void*          )NULL,                  /* 传入给任务函数的参数 */
                (UBaseType_t    )START_TASK_PRIO,       /* 任务优先级 */
                (TaskHandle_t*  )&StartTask_Handler);   /* 任务句柄 */
    vTaskStartScheduler();
}


bool SD_Status = true;
bool fonts_Status = true;
uint8_t tbuf[40];                           /* 当前年月日和时分秒打印句柄 */
uint8_t hour, min, sec, ampm;               /* RTC参数 */
uint8_t year, month, date, week;
void task1(void *pvParameters) //暂时设置为显示基本信息（SD卡容量、温度、当前RTC时间）
{
    // uint32_t task1_num = 0;

    if (!SD_Status)
    {
        /* 检测不到SD卡 */
        lcd_show_string(30, 110, 200, 16, 16, "SD Card Error!", RED);
        vTaskDelay(1000);
        lcd_show_string(30, 110, 200, 16, 16, "Please Check! ", RED);
        vTaskDelay(1000);
        LED0_TOGGLE();  /* 红灯闪烁 */
    }
    else
    {
        /* 检测SD卡成功，打印信息 */
        lcd_show_string(30, 110, 200, 16, 16, "SD Card OK    ", RED);
        lcd_show_string(30, 140, 200, 16, 16, "SD Card Size:     MB", RED);
        lcd_show_num(30 + 13 * 8, 140, SD_TOTAL_SIZE_MB(&g_sdcard_handler), 5, 16, RED); /* 显示SD卡容量 */
    
    }

    // if (!fonts_Status)
    // {
    //     /* 检测不到字库 */
    //     lcd_show_string(30, 50, 300, 16, 16, "Font Error!", RED);
    //     vTaskDelay(200);
    //     lcd_fill(30, 50, 200 + 30, 50 + 16, WHITE);
    //     vTaskDelay(200);
    // }


    

    while(1)
    {
        // lcd_clear(lcd_discolor[++task1_num % 14]);                      /* 刷新屏幕 */
            /* 打印RTC系统时间 */
        rtc_get_date(&year, &month, &date, &week);  /* 打印年月日 */
        sprintf((char *)tbuf, "TIME: 20%02d/%02d/%02d", year, month, date);
        lcd_show_string(23, 200, 210, 16, 16, (char*)tbuf, RED);
        lcd_show_char(150, 200, ' ', 16, 0, BLUE);  /* 中间空格 */
        lcd_show_char(160, 200, ' ', 16, 0, BLUE);
        lcd_show_char(155, 200, ' ', 16, 0, BLUE);

        rtc_get_time(&hour, &min, &sec, &ampm);     /* 打印时分秒 */
        sprintf((char *)tbuf, "%02d:%02d:%02d", hour, min, sec);
        lcd_show_string(170, 200, 210, 16, 16, (char*)tbuf, RED);

        vTaskDelay(1000);                                               /* 延时1000ticks */
    }
}

void task2(void *pvParameters)
{
    float float_num = 0.0;
    short temp;
    
    /* 打印温度信息 */
    lcd_show_string(23, 170, 200, 16, 16, "TEMPERATE:  00.00C", RED);
    temp = adc_get_temperature();   //得到温度值

    if (temp < 0)
    {
        temp = -temp;
        lcd_show_string(30 + 10 * 8, 170, 16, 16, 16, "-", RED);   /* 显示负号 */
    }
    else
    {
        lcd_show_string(30 + 10 * 8, 170, 16, 16, 16, " ", RED);   /* 无符号 */
    }
    lcd_show_xnum(30 + 11 * 8, 170, temp / 100, 2, 16, 0, RED);    /* 显示整数部分 */
    lcd_show_xnum(30 + 14 * 8, 170, temp % 100, 2, 16, 0X80, RED); /* 显示小数部分 */

    while(1)
    {
        float_num += 0.01f;                         /* 更新数值 */

        vTaskDelay(1000);                           /* 延时1000ticks */
    }
}

