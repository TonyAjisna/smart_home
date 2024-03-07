#include "freertos_APP.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./BSP/SDIO/sdio_sdcard.h"
#include "./BSP/ADC/adc.h"
#include "./BSP/RTC/rtc.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdbool.h>
#include "timers.h"
#include "semphr.h"

/******************************************************************************************************/
/*FreeRTOS配置*/

/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define START_TASK_PRIO 1            /* 任务优先级 */
#define START_STK_SIZE 128           /* 任务堆栈大小 */
TaskHandle_t StartTask_Handler;      /* 任务句柄 */
void start_task(void *pvParameters); /* 任务函数 */

/* TASK1 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define BASE_TASK_PRIO 3            /* 任务优先级 */
#define BASE_TASK_STK_SIZE 128      /* 任务堆栈大小 */
TaskHandle_t BaseTask_Handler;      /* 任务句柄 */
void base_task(void *pvParameters); /* 任务函数 */

#define TASK_10MS_PRIO 2             /* 任务优先级 */
#define TASK_10MS_STK_SIZE 128       /* 任务堆栈大小 */
TaskHandle_t TASK_10MS_Task_Handler; /* 任务句柄 */
void task_10ms(void *pvParameters);

#define TASK_1000MS_PRIO 2             /* 任务优先级 */
#define TASK_1000MS_STK_SIZE 128       /* 任务堆栈大小 */
TaskHandle_t TASK_1000MS_Task_Handler; /* 任务句柄 */
void task_1000ms(void *pvParameters);

SemaphoreHandle_t BinarySemaphore_10ms;   /* 二值信号量句柄 */
SemaphoreHandle_t BinarySemaphore_1000ms; /* 二值信号量句柄 */

TimerHandle_t Timer10Timer_Handler;   /* 定时器1句柄 */
TimerHandle_t Timer1000Timer_Handler; /* 定时器2句柄 */

void Timer10msCallback(TimerHandle_t xTimer);   /* 定时器1超时回调函数 */
void Timer1000msCallback(TimerHandle_t xTimer); /* 定时器2超时回调函数 */

uint8_t task10msCounter = 0;
uint8_t task1000msCounter = 0;

void freertos_APP(void)
{
    lcd_show_string(10, 10, 220, 32, 32, "SMART_HOME", RED);
    lcd_show_string(10, 47, 220, 24, 24, "FreeRTOS Porting", RED);
    lcd_show_string(10, 76, 220, 16, 16, "zhaobw@yeah.net", RED);
    lcd_show_string(23, 230, 200, 16, 16, "TEMPERATE:  00.00C", RED);

    xTaskCreate((TaskFunction_t)start_task,          /* 任务函数 */
                (const char *)"start_task",          /* 任务名称 */
                (uint16_t)START_STK_SIZE,            /* 任务堆栈大小 */
                (void *)NULL,                        /* 传入给任务函数的参数 */
                (UBaseType_t)START_TASK_PRIO,        /* 任务优先级 */
                (TaskHandle_t *)&StartTask_Handler); /* 任务句柄 */
    vTaskStartScheduler();
}

void start_task(void *pvParameters)
{
    taskENTER_CRITICAL(); /* 进入临界区 */

    /* 创建二值信号量 */
    if (BinarySemaphore_10ms == NULL)
    {
        BinarySemaphore_10ms = xSemaphoreCreateBinary();
    }
    if (BinarySemaphore_1000ms == NULL)
    {
        BinarySemaphore_1000ms = xSemaphoreCreateBinary();
    }

    /* 定时器1创建为周期定时器 */
    Timer10Timer_Handler = xTimerCreate((const char *)"Timer10ms",                   /* 定时器名 */
                                        (TickType_t)10,                              /* 定时器超时时间 */
                                        (UBaseType_t)pdTRUE,                         /* 周期定时器 */
                                        (void *)1,                                   /* 定时器ID */
                                        (TimerCallbackFunction_t)Timer10msCallback); /* 定时器回调函数 */
    /* 定时器2创建为周期定时器 */
    Timer1000Timer_Handler = xTimerCreate((const char *)"Timer1000ms",                   /* 定时器名 */
                                          (TickType_t)1000,                              /* 定时器超时时间 */
                                          (UBaseType_t)pdTRUE,                           /* 单次定时器 */
                                          (void *)2,                                     /* 定时器ID */
                                          (TimerCallbackFunction_t)Timer1000msCallback); /* 定时器回调函数 */
    /* 创建任务1 */
    xTaskCreate((TaskFunction_t)base_task,
                (const char *)"base_task",
                (uint16_t)BASE_TASK_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)BASE_TASK_PRIO,
                (TaskHandle_t *)&BaseTask_Handler);
    /* 创建任务2 */
    xTaskCreate((TaskFunction_t)task_10ms,
                (const char *)"task_10ms",
                (uint16_t)TASK_10MS_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)TASK_10MS_PRIO,
                (TaskHandle_t *)&TASK_10MS_Task_Handler);
    /* 创建任务2 */
    xTaskCreate((TaskFunction_t)task_1000ms,
                (const char *)"task_1000ms",
                (uint16_t)TASK_1000MS_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)TASK_1000MS_PRIO,
                (TaskHandle_t *)&TASK_1000MS_Task_Handler);
    vTaskDelete(StartTask_Handler); /* 删除开始任务 */
    taskEXIT_CRITICAL();            /* 退出临界区 */
}



bool SD_Status = true;
bool fonts_Status = true;
uint8_t tbuf[40];                           /* 当前年月日和时分秒打印句柄 */
uint8_t hour, min, sec, ampm;               /* RTC参数 */
uint8_t year, month, date, week;
short temp;                                 /* 温度 */
void base_task(void *pvParameters)
{
    uint8_t key = 0;
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

    while (1)
    {
        if ((Timer10Timer_Handler != NULL) && (Timer1000Timer_Handler != NULL))
        {
            key = key_scan(0);

            switch (key)
            {
            case KEY0_PRES:
            {
                xTimerStart((TimerHandle_t)Timer10Timer_Handler,   /* 待启动的定时器句柄 */
                            (TickType_t)portMAX_DELAY);            /* 等待系统启动定时器的最大时间 */
                xTimerStart((TimerHandle_t)Timer1000Timer_Handler, /* 待启动的定时器句柄 */
                            (TickType_t)portMAX_DELAY);            /* 等待系统启动定时器的最大时间 */
                break;
            }
            case KEY1_PRES:
            {
                xTimerStop((TimerHandle_t)Timer10Timer_Handler,   /* 待停止的定时器句柄 */
                           (TickType_t)portMAX_DELAY);            /* 等待系统停止定时器的最大时间 */
                xTimerStop((TimerHandle_t)Timer1000Timer_Handler, /* 待停止的定时器句柄 */
                           (TickType_t)portMAX_DELAY);            /* 等待系统停止定时器的最大时间 */
                break;
            }
            default:
            {
                break;
            }
            }
        }

        vTaskDelay(10);
    }
}

void task_10ms(void *pvParameters)
{
    while (1)
    {
        xSemaphoreTake(BinarySemaphore_10ms, portMAX_DELAY); /* 获取二值信号量 */
        printf("task10msCounter: %u\n", task10msCounter);
    }
}

void task_1000ms(void *pvParameters)
{
    while (1)
    {
        xSemaphoreTake(BinarySemaphore_1000ms, portMAX_DELAY); /* 获取二值信号量 */
        printf("task1000msCounter: %u\n", task1000msCounter);

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

        /* 打印温度信息 */
        temp = adc_get_temperature();   //得到温度值
        if (temp < 0)
        {
            temp = -temp;
            lcd_show_string(30 + 10 * 8, 230, 16, 16, 16, "-", RED);   /* 显示负号 */
        }
        else
        {
            lcd_show_string(30 + 10 * 8, 230, 16, 16, 16, " ", RED);   /* 无符号 */
        }
        lcd_show_xnum(30 + 11 * 8, 230, temp / 100, 2, 16, 0, RED);    /* 显示整数部分 */
        lcd_show_xnum(30 + 14 * 8, 230, temp % 100, 2, 16, 0X80, RED); /* 显示小数部分 */
    }
}

void Timer10msCallback(TimerHandle_t xTimer)
{
    task10msCounter++;
    xSemaphoreGive(BinarySemaphore_10ms); /* 释放二值信号量 */
}

void Timer1000msCallback(TimerHandle_t xTimer)
{
    task1000msCounter++;
    xSemaphoreGive(BinarySemaphore_1000ms); /* 释放二值信号量 */
}
