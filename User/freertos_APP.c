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
#include "timers.h"
#include "semphr.h"

/* task任务配置
    包括：任务句柄、任务优先级、堆栈大小、创建任务
    任务优先级高低和其对应的优先级数值成正比，也就是说任务优先级数值为0的优先级最低、中断则相反

    本程序应分为基本任务、定时任务和特殊任务。如显示屏任务、电源检测等应属于基本任务，音频播放属于特殊任务，数据包发送等属于定时任务；
 */




//------------------------------------------------------------------------------定义任务------------------------------------------------------------------------------
// 定义start任务
#define START_TASK_PRIO 1                   /* 任务优先级 */
#define START_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            StartTask_Handler;  /* 任务句柄 */
void start_task(void *pvParameters);        /* 任务函数 */

// 定义初始化任务
// #define INIT_TASK_PRIO 2            /* 任务优先级 */
// #define INIT_TASK_SIZE 128          /* 任务堆栈大小 */
// TaskHandle_t InitTask_Handler;      /* 任务句柄 */
// void init_task(void *pvParameters); /* 任务函数 */

// // 定义基础任务
// #define BASE_TASK_PRIO 3            /* 任务优先级 */
// #define BASE_TASK_SIZE 128          /* 任务堆栈大小 */
// TaskHandle_t BaseTask_Handler;      /* 任务句柄 */
// void base_task(void *pvParameters); /* 任务函数 */

// 定义6个周期任务
#define TASK_5MS_STK_SIZE 10            /* 任务优先级 */
#define TASK_5MS_PRIO 128           /* 任务堆栈大小 */
TaskHandle_t Task_5MS_Task_Handler; /* 任务句柄 */
void task_5ms(void *pvParameters);  /* 任务函数 */

#define TASK_20MS_STK_SIZE 11            /* 任务优先级 */
#define TASK_20MS_PRIO 128           /* 任务堆栈大小 */
TaskHandle_t Task_20MS_Task_Handler; /* 任务句柄 */
void task_20ms(void *pvParameters);  /* 任务函数 */

#define TASK_50MS_STK_SIZE 12            /* 任务优先级 */
#define TASK_50MS_PRIO 128           /* 任务堆栈大小 */
TaskHandle_t Task_50MS_Task_Handler; /* 任务句柄 */
void task_50ms(void *pvParameters);  /* 任务函数 */

#define TASK_100MS_STK_SIZE 13            /* 任务优先级 */
#define TASK_100MS_PRIO 128           /* 任务堆栈大小 */
TaskHandle_t Task_100MS_Task_Handler; /* 任务句柄 */
void task_100ms(void *pvParameters);       /* 任务函数 */

#define TASK_500MS_STK_SIZE 14            /* 任务优先级 */
#define TASK_500MS_PRIO 128           /* 任务堆栈大小 */
TaskHandle_t Task_500MS_Task_Handler; /* 任务句柄 */
void task_500ms(void *pvParameters);  /* 任务函数 */

#define TASK_1000MS_STK_SIZE 15            /* 任务优先级 */
#define TASK_1000MS_PRIO 128           /* 任务堆栈大小 */
TaskHandle_t Task_1000MS_Task_Handler; /* 任务句柄 */
void task_1000ms(void *pvParameters);  /* 任务函数 */

// #define TASK1_PRIO      2                   /* 任务优先级 */
// #define TASK1_STK_SIZE  128                 /* 任务堆栈大小 */
// TaskHandle_t            Task1Task_Handler;  /* 任务句柄 */
// void task1(void *pvParameters);             /* 任务函数 */

// #define TASK2_PRIO      3                   /* 任务优先级 */
// #define TASK2_STK_SIZE  128                 /* 任务堆栈大小 */
// TaskHandle_t            Task2Task_Handler;  /* 任务句柄 */
// void task2(void *pvParameters);             /* 任务函数 */
//------------------------------------------------------------------------------定义任务------------------------------------------------------------------------------




// 创建信号量句柄	freertos_APP.h
SemaphoreHandle_t BinarySemaphore_5ms;
SemaphoreHandle_t BinarySemaphore_20ms;
SemaphoreHandle_t BinarySemaphore_50ms;
SemaphoreHandle_t BinarySemaphore_100ms;
SemaphoreHandle_t BinarySemaphore_500ms;
SemaphoreHandle_t BinarySemaphore_1000ms;

// 5ms定时器句柄
TimerHandle_t Timer_5ms_Handler;
TimerHandle_t Timer_20ms_Handler;
TimerHandle_t Timer_50ms_Handler;
TimerHandle_t Timer_100ms_Handler;
TimerHandle_t Timer_500ms_Handler;
TimerHandle_t Timer_1000ms_Handler;


//定义周期计数器
uint8_t Counter5ms = 0;
uint8_t Counter20ms = 0;
uint8_t Counter50ms = 0;
uint8_t Counter100ms = 0;
uint8_t Counter500ms = 0;
uint8_t Counter1000ms = 0;

// 统一创建二值信号量函数
void CreateBinarySemaphores(void)
{
    BinarySemaphore_5ms = xSemaphoreCreateBinary();
    BinarySemaphore_20ms = xSemaphoreCreateBinary();
    BinarySemaphore_50ms = xSemaphoreCreateBinary();
    BinarySemaphore_100ms = xSemaphoreCreateBinary();
    BinarySemaphore_500ms = xSemaphoreCreateBinary();
    BinarySemaphore_1000ms = xSemaphoreCreateBinary();
}





//------------------------------------------------------------------------------创建周期定时器------------------------------------------------------------------------------
void CreateTimers(void)
{
    /* 定时器创建为周期定时器 */
    Timer_5ms_Handler = xTimerCreate((const char *)"Timer_5ms",                     /* 定时器名 */
                                     (TickType_t)5,                                 /* 定时器超时时间 */
                                     (UBaseType_t)pdTRUE,                           /* 周期定时器 */
                                     (void *)1,                                     /* 定时器ID */
                                     (TimerCallbackFunction_t)Timer_5ms_Callback); /* 定时器回调函数 */

    /* 定时器创建为周期定时器 */
    Timer_20ms_Handler = xTimerCreate((const char *)"Timer_20ms",                    /* 定时器名 */
                                      (TickType_t)20,                                /* 定时器超时时间 */
                                      (UBaseType_t)pdTRUE,                           /* 周期定时器 */
                                      (void *)2,                                     /* 定时器ID */
                                      (TimerCallbackFunction_t)Timer_20ms_Callback); /* 定时器回调函数 */

    /* 定时器创建为周期定时器 */
    Timer_50ms_Handler = xTimerCreate((const char *)"Timer_50ms",                     /* 定时器名 */
                                      (TickType_t)50,                                 /* 定时器超时时间 */
                                      (UBaseType_t)pdTRUE,                            /* 周期定时器 */
                                      (void *)3,                                      /* 定时器ID */
                                      (TimerCallbackFunction_t)Timer_50ms_Callback); /* 定时器回调函数 */

    /* 定时器创建为周期定时器 */
    Timer_100ms_Handler = xTimerCreate((const char *)"Timer_100ms",                    /* 定时器名 */
                                       (TickType_t)100,                                /* 定时器超时时间 */
                                       (UBaseType_t)pdTRUE,                            /* 周期定时器 */
                                       (void *)4,                                      /* 定时器ID */
                                       (TimerCallbackFunction_t)Timer_100ms_Callback); /* 定时器回调函数 */

    /* 定时器创建为周期定时器 */
    Timer_500ms_Handler = xTimerCreate((const char *)"Timer_500ms",                    /* 定时器名 */
                                       (TickType_t)500,                                /* 定时器超时时间 */
                                       (UBaseType_t)pdTRUE,                            /* 周期定时器 */
                                       (void *)5,                                      /* 定时器ID */
                                       (TimerCallbackFunction_t)Timer_500ms_Callback); /* 定时器回调函数 */

    /* 定时器创建为周期定时器 */
    Timer_1000ms_Handler = xTimerCreate((const char *)"Timer_1000ms",                    /* 定时器名 */
                                       (TickType_t)1000,                                /* 定时器超时时间 */
                                       (UBaseType_t)pdTRUE,                            /* 周期定时器 */
                                       (void *)6,                                      /* 定时器ID */
                                       (TimerCallbackFunction_t)Timer_1000ms_Callback); /* 定时器回调函数 */                                                                      
}
//------------------------------------------------------------------------------创建周期定时器------------------------------------------------------------------------------







//------------------------------------------------------------------------------实现周期定时器------------------------------------------------------------------------------
// 引入定时器定时更新信号量
void Timer_5ms_Callback(TimerHandle_t xTimer)
{
    xSemaphoreGive(BinarySemaphore_5ms); // 释放二值信号量，0-->1
    Counter5ms ++;
}

void Timer_20ms_Callback(TimerHandle_t xTimer)
{
    xSemaphoreGive(BinarySemaphore_20ms); // 释放二值信号量，0-->1
    Counter20ms ++;
}

void Timer_50ms_Callback(TimerHandle_t xTimer)
{
    xSemaphoreGive(BinarySemaphore_50ms); // 释放二值信号量，0-->1
    Counter50ms ++;
}

void Timer_100ms_Callback(TimerHandle_t xTimer)
{
    xSemaphoreGive(BinarySemaphore_100ms); // 释放二值信号量，0-->1
    Counter100ms ++;
}

void Timer_500ms_Callback(TimerHandle_t xTimer)
{
    xSemaphoreGive(BinarySemaphore_500ms); // 释放二值信号量，0-->1
    Counter500ms ++;
}

void Timer_1000ms_Callback(TimerHandle_t xTimer)
{
    xSemaphoreGive(BinarySemaphore_1000ms); // 释放二值信号量，0-->1
    Counter1000ms ++;
}
//------------------------------------------------------------------------------实现周期定时器------------------------------------------------------------------------------







//------------------------------------------------------------------------------实现任务------------------------------------------------------------------------------
// 解耦，将任务创建放到CreateTask()中
void CreateTasks(void)
{
    // xTaskCreate((TaskFunction_t)Base_task,
    //             (const char *)"Base_task",
    //             (uint16_t)TASK_5MS_STK_SIZE,
    //             (void *)NULL,
    //             (UBaseType_t)TASK_5MS_PRIO,
    //             (TaskHandle_t *)&Task_5MS_Task_Handler);

    xTaskCreate((TaskFunction_t)task_5ms,
                (const char *)"task_5ms",
                (uint16_t)TASK_5MS_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)TASK_5MS_PRIO,
                (TaskHandle_t *)&Task_5MS_Task_Handler);

    xTaskCreate((TaskFunction_t)task_20ms,
                (const char *)"task_20ms",
                (uint16_t)TASK_20MS_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)TASK_20MS_PRIO,
                (TaskHandle_t *)&Task_20MS_Task_Handler);

    xTaskCreate((TaskFunction_t)task_50ms,
                (const char *)"task_50ms",
                (uint16_t)TASK_50MS_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)TASK_50MS_PRIO,
                (TaskHandle_t *)&Task_50MS_Task_Handler);

    xTaskCreate((TaskFunction_t)task_100ms,
                (const char *)"task_100ms",
                (uint16_t)TASK_100MS_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)TASK_100MS_PRIO,
                (TaskHandle_t *)&Task_100MS_Task_Handler);

    xTaskCreate((TaskFunction_t)task_500ms,
                (const char *)"task_500ms",
                (uint16_t)TASK_500MS_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)TASK_500MS_PRIO,
                (TaskHandle_t *)&Task_500MS_Task_Handler);

    xTaskCreate((TaskFunction_t)task_1000ms,
                (const char *)"task_1000ms",
                (uint16_t)TASK_1000MS_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)TASK_1000MS_PRIO,
                (TaskHandle_t *)&Task_1000MS_Task_Handler);

    // /* 创建任务1 */
    // xTaskCreate((TaskFunction_t)task1,
    //             (const char *)"task1",
    //             (uint16_t)TASK1_STK_SIZE,
    //             (void *)NULL,
    //             (UBaseType_t)TASK1_PRIO,
    //             (TaskHandle_t *)&Task1Task_Handler);
    // /* 创建任务2 */
    // xTaskCreate((TaskFunction_t)task2,
    //             (const char *)"task2",
    //             (uint16_t)TASK2_STK_SIZE,
    //             (void *)NULL,
    //             (UBaseType_t)TASK2_PRIO,
    //             (TaskHandle_t *)&Task2Task_Handler);
}

// 开始任务，创建基本任务、特殊任务和周期任务
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           // 进入临界区
    CreateBinarySemaphores();       // 创建信号值
    CreateTimers();                 // 创建定时器
    CreateTasks();                  // 创建任务
    vTaskDelete(StartTask_Handler); // 删除开始任务
    taskEXIT_CRITICAL();            // 退出临界区
}



void task_5ms(void *pvParameters)
{
    while (1)
    {
        xSemaphoreTake(BinarySemaphore_5ms, portMAX_DELAY);    // 获取二值信号量，1-->0
        lcd_show_xnum(79, 111, Counter5ms, 3, 16, 0x80, BLUE); // 打印次数
    }
}

void task_20ms(void *pvParameters)
{
    while (1)
    {
        xSemaphoreTake(BinarySemaphore_20ms, portMAX_DELAY);    // 获取二值信号量，1-->0
        lcd_show_xnum(79, 111, Counter20ms, 3, 16, 0x80, BLUE); // 打印次数
    }
}

void task_50ms(void *pvParameters)
{
    while (1)
    {
        xSemaphoreTake(BinarySemaphore_50ms, portMAX_DELAY);    // 获取二值信号量，1-->0
        lcd_show_xnum(79, 111, Counter50ms, 3, 16, 0x80, BLUE); // 打印次数
    }
}

void task_100ms(void *pvParameters)
{
    while (1)
    {
        xSemaphoreTake(BinarySemaphore_100ms, portMAX_DELAY);    // 获取二值信号量，1-->0
        lcd_show_xnum(79, 111, Counter100ms, 3, 16, 0x80, BLUE); // 打印次数
    }
}

void task_500ms(void *pvParameters)
{
    while (1)
    {
        xSemaphoreTake(BinarySemaphore_500ms, portMAX_DELAY);    // 获取二值信号量，1-->0
        lcd_show_xnum(79, 111, Counter500ms, 3, 16, 0x80, BLUE); // 打印次数
    }
}

void task_1000ms(void *pvParameters)
{
    while (1)
    {
        xSemaphoreTake(BinarySemaphore_1000ms, portMAX_DELAY);    // 获取二值信号量，1-->0
        lcd_show_xnum(79, 111, Counter1000ms, 3, 16, 0x80, BLUE); // 打印次数
    }
}

/* LCD刷屏时使用的颜色 */
uint16_t lcd_discolor[11] = {WHITE, BLACK, BLUE, RED,
                             MAGENTA, GREEN, CYAN, YELLOW,
                             BROWN, BRRED, GRAY};

/* FreeRTOS例程入口函数 */
void freertos_APP(void)
{
    lcd_show_string(10, 10, 220, 32, 32, "SMART_HOME", RED);
    lcd_show_string(10, 47, 220, 24, 24, "FreeRTOS Porting", RED);
    lcd_show_string(10, 76, 220, 16, 16, "zhaobw@yeah.net", RED);

    xTaskCreate((TaskFunction_t)start_task,          /* 任务函数 */
                (const char *)"start_task",          /* 任务名称 */
                (uint16_t)START_STK_SIZE,            /* 任务堆栈大小 */
                (void *)NULL,                        /* 传入给任务函数的参数 */
                (UBaseType_t)START_TASK_PRIO,        /* 任务优先级 */
                (TaskHandle_t *)&StartTask_Handler); /* 任务句柄 */
    vTaskStartScheduler();
}

bool SD_Status = true;
// bool fonts_Status = true;
// uint8_t tbuf[40];             /* 当前年月日和时分秒打印句柄 */
// uint8_t hour, min, sec, ampm; /* RTC参数 */
// uint8_t year, month, date, week;
// void task1(void *pvParameters) // 暂时设置为显示基本信息（SD卡容量、温度、当前RTC时间）
// {
//     // uint32_t task1_num = 0;

//     if (!SD_Status)
//     {
//         /* 检测不到SD卡 */
//         lcd_show_string(30, 110, 200, 16, 16, "SD Card Error!", RED);
//         vTaskDelay(1000);
//         lcd_show_string(30, 110, 200, 16, 16, "Please Check! ", RED);
//         vTaskDelay(1000);
//         LED0_TOGGLE(); /* 红灯闪烁 */
//     }
//     else
//     {
//         /* 检测SD卡成功，打印信息 */
//         lcd_show_string(30, 110, 200, 16, 16, "SD Card OK    ", RED);
//         lcd_show_string(30, 140, 200, 16, 16, "SD Card Size:     MB", RED);
//         lcd_show_num(30 + 13 * 8, 140, SD_TOTAL_SIZE_MB(&g_sdcard_handler), 5, 16, RED); /* 显示SD卡容量 */
//     }

//     // if (!fonts_Status)
//     // {
//     //     /* 检测不到字库 */
//     //     lcd_show_string(30, 50, 300, 16, 16, "Font Error!", RED);
//     //     vTaskDelay(200);
//     //     lcd_fill(30, 50, 200 + 30, 50 + 16, WHITE);
//     //     vTaskDelay(200);
//     // }

//     while (1)
//     {
//         // lcd_clear(lcd_discolor[++task1_num % 14]);                      /* 刷新屏幕 */
//         /* 打印RTC系统时间 */
//         rtc_get_date(&year, &month, &date, &week); /* 打印年月日 */
//         sprintf((char *)tbuf, "TIME: 20%02d/%02d/%02d", year, month, date);
//         lcd_show_string(23, 200, 210, 16, 16, (char *)tbuf, RED);
//         lcd_show_char(150, 200, ' ', 16, 0, BLUE); /* 中间空格 */
//         lcd_show_char(160, 200, ' ', 16, 0, BLUE);
//         lcd_show_char(155, 200, ' ', 16, 0, BLUE);

//         rtc_get_time(&hour, &min, &sec, &ampm); /* 打印时分秒 */
//         sprintf((char *)tbuf, "%02d:%02d:%02d", hour, min, sec);
//         lcd_show_string(170, 200, 210, 16, 16, (char *)tbuf, RED);

//         vTaskDelay(1000); /* 延时1000ticks */
//     }
// }

// void task2(void *pvParameters)
// {
//     float float_num = 0.0;
//     short temp;

//     /* 打印温度信息 */
//     lcd_show_string(23, 170, 200, 16, 16, "TEMPERATE:  00.00C", RED);
//     temp = adc_get_temperature(); // 得到温度值

//     if (temp < 0)
//     {
//         temp = -temp;
//         lcd_show_string(30 + 10 * 8, 170, 16, 16, 16, "-", RED); /* 显示负号 */
//     }
//     else
//     {
//         lcd_show_string(30 + 10 * 8, 170, 16, 16, 16, " ", RED); /* 无符号 */
//     }
//     lcd_show_xnum(30 + 11 * 8, 170, temp / 100, 2, 16, 0, RED);    /* 显示整数部分 */
//     lcd_show_xnum(30 + 14 * 8, 170, temp % 100, 2, 16, 0X80, RED); /* 显示小数部分 */

//     while (1)
//     {
//         float_num += 0.01f; /* 更新数值 */

//         vTaskDelay(1000); /* 延时1000ticks */
//     }
// }
