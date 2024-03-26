#include "freertos_APP.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/KEY/key.h"
#include "lwip_comm.h"
#include "lwip_demo.h"
#include "lwipopts.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdbool.h>
#include "timers.h"
#include "semphr.h"
#include "./BSP/DHT11/dht11.h"


/******************************************************************************************************/
/*FreeRTOS配置*/

/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define START_TASK_PRIO         5           /* 任务优先级 */
#define START_STK_SIZE          128         /* 任务堆栈大小 */
TaskHandle_t StartTask_Handler;             /* 任务句柄 */
void start_task(void *pvParameters);        /* 任务函数 */

/* LWIP_DEMO 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define LWIP_DMEO_TASK_PRIO     11          /* 任务优先级 */
#define LWIP_DMEO_STK_SIZE      1024        /* 任务堆栈大小 */
TaskHandle_t LWIP_Task_Handler;             /* 任务句柄 */
void lwip_demo_task(void *pvParameters);    /* 任务函数 */

/* LED_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define LED_TASK_PRIO           9           /* 任务优先级 */
#define LED_STK_SIZE            128         /* 任务堆栈大小 */
TaskHandle_t LEDTask_Handler;               /* 任务句柄 */
void led_task(void *pvParameters);          /* 任务函数 */


#define TASK_10MS_PRIO 12             /* 任务优先级 */
#define TASK_10MS_STK_SIZE 128       /* 任务堆栈大小 */
TaskHandle_t TASK_10MS_Task_Handler; /* 任务句柄 */
void task_10ms(void *pvParameters);

#define TASK_1000MS_PRIO 12             /* 任务优先级 */
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
/******************************************************************************************************/


/**
 * @breif       加载UI
 * @param       mode :  bit0:0,不加载;1,加载前半部分UI
 *                      bit1:0,不加载;1,加载后半部分UI
 * @retval      无
 */
void lwip_test_ui(uint8_t mode)
{
    uint8_t speed;
    uint8_t buf[30];
    
    if (mode & 1<< 0)
    {
        lcd_show_string(6, 10, 200, 32, 32, "STM32", DARKBLUE);
        lcd_show_string(6, 40, lcddev.width, 24, 24, "lwIP MQTTAliyun", DARKBLUE);
        lcd_show_string(6, 70, 200, 16, 16, "ATOM@ALIENTEK", DARKBLUE);
    }
    
    if (mode & 1 << 1)
    {
        lcd_show_string(5, 110, 200, 16, 16, "lwIP Init Successed", MAGENTA);
        
        if (g_lwipdev.dhcpstatus == 2)
        {
            sprintf((char*)buf,"DHCP IP:%d.%d.%d.%d",g_lwipdev.ip[0],g_lwipdev.ip[1],g_lwipdev.ip[2],g_lwipdev.ip[3]);     /* 显示动态IP地址 */
        }
        else
        {
            sprintf((char*)buf,"Static IP:%d.%d.%d.%d",g_lwipdev.ip[0],g_lwipdev.ip[1],g_lwipdev.ip[2],g_lwipdev.ip[3]);    /* 打印静态IP地址 */
        }
        
        lcd_show_string(5, 130, 200, 16, 16, (char*)buf, MAGENTA);
        
        speed = ethernet_chip_get_speed();      /* 得到网速 */
        
        if (speed)
        {
            lcd_show_string(5, 150, 200, 16, 16, "Ethernet Speed:100M", MAGENTA);
        }
        else
        {
            lcd_show_string(5, 150, 200, 16, 16, "Ethernet Speed:10M", MAGENTA);
        }
    }
}

/**
 * @breif       freertos_demo
 * @param       无
 * @retval      无
 */
void freertos_demo(void)
{
    /* start_task任务 */
    xTaskCreate((TaskFunction_t )start_task,
                (const char *   )"start_task",
                (uint16_t       )START_STK_SIZE,
                (void *         )NULL,
                (UBaseType_t    )START_TASK_PRIO,
                (TaskHandle_t * )&StartTask_Handler);

    vTaskStartScheduler(); /* 开启任务调度 */
}

/**
 * @brief       start_task
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void start_task(void *pvParameters)
{
    pvParameters = pvParameters;
    
    g_lwipdev.lwip_display_fn = lwip_test_ui;
    
    lwip_test_ui(1);    /* 加载后前部分UI */
    
    while (lwip_comm_init() != 0)
    {
        lcd_show_string(30, 110, 200, 16, 16, "lwIP Init failed!!", RED);
        delay_ms(500);
        lcd_fill(30, 50, 200 + 30, 50 + 16, WHITE);
        lcd_show_string(30, 110, 200, 16, 16, "Retrying...       ", RED);
        delay_ms(500);
        LED1_TOGGLE();
    }
    
    while (!ethernet_read_phy(PHY_SR))  /* 检查MCU与PHY芯片是否通信成功 */
    {
        printf("MCU与PHY芯片通信失败，请检查电路或者源码！！！！\r\n");
    }
    
    while ((g_lwipdev.dhcpstatus != 2) && (g_lwipdev.dhcpstatus != 0XFF))  /* 等待DHCP获取成功/超时溢出 */
    {
        vTaskDelay(5);
    }
    

    
    taskENTER_CRITICAL();           /* 进入临界区 */

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
    /* 创建lwIP任务 */
    xTaskCreate((TaskFunction_t )lwip_demo_task,
                (const char*    )"lwip_demo_task",
                (uint16_t       )LWIP_DMEO_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LWIP_DMEO_TASK_PRIO,
                (TaskHandle_t*  )&LWIP_Task_Handler);

    /* LED测试任务 */
    xTaskCreate((TaskFunction_t )led_task,
                (const char*    )"led_task",
                (uint16_t       )LED_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED_TASK_PRIO,
                (TaskHandle_t*  )&LEDTask_Handler);

    vTaskDelete(StartTask_Handler); /* 删除开始任务 */
    taskEXIT_CRITICAL();            /* 退出临界区 */
    
}

/**
 * @brief       lwIP运行例程
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void lwip_demo_task(void *pvParameters)
{
    pvParameters = pvParameters;
    uint8_t key = 0;

    if ((Timer10Timer_Handler != NULL) && (Timer1000Timer_Handler != NULL))
    {
        xTimerStart((TimerHandle_t)Timer10Timer_Handler,   /* 待启动的定时器句柄 */
                            (TickType_t)portMAX_DELAY);            /* 等待系统启动定时器的最大时间 */
        xTimerStart((TimerHandle_t)Timer1000Timer_Handler, /* 待启动的定时器句柄 */
                            (TickType_t)portMAX_DELAY);            /* 等待系统启动定时器的最大时间 */
    }
    
    
    lwip_demo();
    
    while (1)
    {
        // if ((Timer10Timer_Handler != NULL) && (Timer1000Timer_Handler != NULL))
        // {
        //     key = key_scan(0);

        //     switch (key)
        //     {
        //     case KEY0_PRES:
        //     {
        //         xTimerStart((TimerHandle_t)Timer10Timer_Handler,   /* 待启动的定时器句柄 */
        //                     (TickType_t)portMAX_DELAY);            /* 等待系统启动定时器的最大时间 */
        //         xTimerStart((TimerHandle_t)Timer1000Timer_Handler, /* 待启动的定时器句柄 */
        //                     (TickType_t)portMAX_DELAY);            /* 等待系统启动定时器的最大时间 */
        //         break;
        //     }
        //     case KEY1_PRES:
        //     {
        //         xTimerStop((TimerHandle_t)Timer10Timer_Handler,   /* 待停止的定时器句柄 */
        //                    (TickType_t)portMAX_DELAY);            /* 等待系统停止定时器的最大时间 */
        //         xTimerStop((TimerHandle_t)Timer1000Timer_Handler, /* 待停止的定时器句柄 */
        //                    (TickType_t)portMAX_DELAY);            /* 等待系统停止定时器的最大时间 */
        //         break;
        //     }
        //     default:
        //     {
        //         break;
        //     }
        //     }
        // }
        // vTaskDelay(5);
    }
}

/**
 * @brief       系统再运行
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void led_task(void *pvParameters)
{
    pvParameters = pvParameters;
    
    while (1)
    {
        LED1_TOGGLE();
        vTaskDelay(1000);
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

extern float g_temp;    /* 温度值 */
extern float g_humid;   /* 湿度值 */
void task_1000ms(void *pvParameters)
{
    while (1)
    {
        xSemaphoreTake(BinarySemaphore_1000ms, portMAX_DELAY); /* 获取二值信号量 */
        printf("task1000msCounter: %u\n", task1000msCounter);
    }
}

uint8_t temperature = 0;
uint8_t humidity = 0;
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
