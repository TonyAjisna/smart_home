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
/*FreeRTOS����*/

/* START_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define START_TASK_PRIO 1            /* �������ȼ� */
#define START_STK_SIZE 128           /* �����ջ��С */
TaskHandle_t StartTask_Handler;      /* ������ */
void start_task(void *pvParameters); /* ������ */

/* TASK1 ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define BASE_TASK_PRIO 3            /* �������ȼ� */
#define BASE_TASK_STK_SIZE 128      /* �����ջ��С */
TaskHandle_t BaseTask_Handler;      /* ������ */
void base_task(void *pvParameters); /* ������ */

#define TASK_10MS_PRIO 2             /* �������ȼ� */
#define TASK_10MS_STK_SIZE 128       /* �����ջ��С */
TaskHandle_t TASK_10MS_Task_Handler; /* ������ */
void task_10ms(void *pvParameters);

#define TASK_1000MS_PRIO 2             /* �������ȼ� */
#define TASK_1000MS_STK_SIZE 128       /* �����ջ��С */
TaskHandle_t TASK_1000MS_Task_Handler; /* ������ */
void task_1000ms(void *pvParameters);

SemaphoreHandle_t BinarySemaphore_10ms;   /* ��ֵ�ź������ */
SemaphoreHandle_t BinarySemaphore_1000ms; /* ��ֵ�ź������ */

TimerHandle_t Timer10Timer_Handler;   /* ��ʱ��1��� */
TimerHandle_t Timer1000Timer_Handler; /* ��ʱ��2��� */

void Timer10msCallback(TimerHandle_t xTimer);   /* ��ʱ��1��ʱ�ص����� */
void Timer1000msCallback(TimerHandle_t xTimer); /* ��ʱ��2��ʱ�ص����� */

uint8_t task10msCounter = 0;
uint8_t task1000msCounter = 0;

void freertos_APP(void)
{
    lcd_show_string(10, 10, 220, 32, 32, "SMART_HOME", RED);
    lcd_show_string(10, 47, 220, 24, 24, "FreeRTOS Porting", RED);
    lcd_show_string(10, 76, 220, 16, 16, "zhaobw@yeah.net", RED);
    lcd_show_string(23, 230, 200, 16, 16, "TEMPERATE:  00.00C", RED);

    xTaskCreate((TaskFunction_t)start_task,          /* ������ */
                (const char *)"start_task",          /* �������� */
                (uint16_t)START_STK_SIZE,            /* �����ջ��С */
                (void *)NULL,                        /* ������������Ĳ��� */
                (UBaseType_t)START_TASK_PRIO,        /* �������ȼ� */
                (TaskHandle_t *)&StartTask_Handler); /* ������ */
    vTaskStartScheduler();
}

void start_task(void *pvParameters)
{
    taskENTER_CRITICAL(); /* �����ٽ��� */

    /* ������ֵ�ź��� */
    if (BinarySemaphore_10ms == NULL)
    {
        BinarySemaphore_10ms = xSemaphoreCreateBinary();
    }
    if (BinarySemaphore_1000ms == NULL)
    {
        BinarySemaphore_1000ms = xSemaphoreCreateBinary();
    }

    /* ��ʱ��1����Ϊ���ڶ�ʱ�� */
    Timer10Timer_Handler = xTimerCreate((const char *)"Timer10ms",                   /* ��ʱ���� */
                                        (TickType_t)10,                              /* ��ʱ����ʱʱ�� */
                                        (UBaseType_t)pdTRUE,                         /* ���ڶ�ʱ�� */
                                        (void *)1,                                   /* ��ʱ��ID */
                                        (TimerCallbackFunction_t)Timer10msCallback); /* ��ʱ���ص����� */
    /* ��ʱ��2����Ϊ���ڶ�ʱ�� */
    Timer1000Timer_Handler = xTimerCreate((const char *)"Timer1000ms",                   /* ��ʱ���� */
                                          (TickType_t)1000,                              /* ��ʱ����ʱʱ�� */
                                          (UBaseType_t)pdTRUE,                           /* ���ζ�ʱ�� */
                                          (void *)2,                                     /* ��ʱ��ID */
                                          (TimerCallbackFunction_t)Timer1000msCallback); /* ��ʱ���ص����� */
    /* ��������1 */
    xTaskCreate((TaskFunction_t)base_task,
                (const char *)"base_task",
                (uint16_t)BASE_TASK_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)BASE_TASK_PRIO,
                (TaskHandle_t *)&BaseTask_Handler);
    /* ��������2 */
    xTaskCreate((TaskFunction_t)task_10ms,
                (const char *)"task_10ms",
                (uint16_t)TASK_10MS_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)TASK_10MS_PRIO,
                (TaskHandle_t *)&TASK_10MS_Task_Handler);
    /* ��������2 */
    xTaskCreate((TaskFunction_t)task_1000ms,
                (const char *)"task_1000ms",
                (uint16_t)TASK_1000MS_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)TASK_1000MS_PRIO,
                (TaskHandle_t *)&TASK_1000MS_Task_Handler);
    vTaskDelete(StartTask_Handler); /* ɾ����ʼ���� */
    taskEXIT_CRITICAL();            /* �˳��ٽ��� */
}



bool SD_Status = true;
bool fonts_Status = true;
uint8_t tbuf[40];                           /* ��ǰ�����պ�ʱ�����ӡ��� */
uint8_t hour, min, sec, ampm;               /* RTC���� */
uint8_t year, month, date, week;
short temp;                                 /* �¶� */
void base_task(void *pvParameters)
{
    uint8_t key = 0;
    if (!SD_Status)
    {
        /* ��ⲻ��SD�� */
        lcd_show_string(30, 110, 200, 16, 16, "SD Card Error!", RED);
        vTaskDelay(1000);
        lcd_show_string(30, 110, 200, 16, 16, "Please Check! ", RED);
        vTaskDelay(1000);
        LED0_TOGGLE();  /* �����˸ */
    }
    else
    {
        /* ���SD���ɹ�����ӡ��Ϣ */
        lcd_show_string(30, 110, 200, 16, 16, "SD Card OK    ", RED);
        lcd_show_string(30, 140, 200, 16, 16, "SD Card Size:     MB", RED);
        lcd_show_num(30 + 13 * 8, 140, SD_TOTAL_SIZE_MB(&g_sdcard_handler), 5, 16, RED); /* ��ʾSD������ */
    
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
                xTimerStart((TimerHandle_t)Timer10Timer_Handler,   /* �������Ķ�ʱ����� */
                            (TickType_t)portMAX_DELAY);            /* �ȴ�ϵͳ������ʱ�������ʱ�� */
                xTimerStart((TimerHandle_t)Timer1000Timer_Handler, /* �������Ķ�ʱ����� */
                            (TickType_t)portMAX_DELAY);            /* �ȴ�ϵͳ������ʱ�������ʱ�� */
                break;
            }
            case KEY1_PRES:
            {
                xTimerStop((TimerHandle_t)Timer10Timer_Handler,   /* ��ֹͣ�Ķ�ʱ����� */
                           (TickType_t)portMAX_DELAY);            /* �ȴ�ϵͳֹͣ��ʱ�������ʱ�� */
                xTimerStop((TimerHandle_t)Timer1000Timer_Handler, /* ��ֹͣ�Ķ�ʱ����� */
                           (TickType_t)portMAX_DELAY);            /* �ȴ�ϵͳֹͣ��ʱ�������ʱ�� */
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
        xSemaphoreTake(BinarySemaphore_10ms, portMAX_DELAY); /* ��ȡ��ֵ�ź��� */
        printf("task10msCounter: %u\n", task10msCounter);
    }
}

void task_1000ms(void *pvParameters)
{
    while (1)
    {
        xSemaphoreTake(BinarySemaphore_1000ms, portMAX_DELAY); /* ��ȡ��ֵ�ź��� */
        printf("task1000msCounter: %u\n", task1000msCounter);

        /* ��ӡRTCϵͳʱ�� */
        rtc_get_date(&year, &month, &date, &week);  /* ��ӡ������ */
        sprintf((char *)tbuf, "TIME: 20%02d/%02d/%02d", year, month, date);
        lcd_show_string(23, 200, 210, 16, 16, (char*)tbuf, RED);
        lcd_show_char(150, 200, ' ', 16, 0, BLUE);  /* �м�ո� */
        lcd_show_char(160, 200, ' ', 16, 0, BLUE);
        lcd_show_char(155, 200, ' ', 16, 0, BLUE);
        rtc_get_time(&hour, &min, &sec, &ampm);     /* ��ӡʱ���� */
        sprintf((char *)tbuf, "%02d:%02d:%02d", hour, min, sec);
        lcd_show_string(170, 200, 210, 16, 16, (char*)tbuf, RED);

        /* ��ӡ�¶���Ϣ */
        temp = adc_get_temperature();   //�õ��¶�ֵ
        if (temp < 0)
        {
            temp = -temp;
            lcd_show_string(30 + 10 * 8, 230, 16, 16, 16, "-", RED);   /* ��ʾ���� */
        }
        else
        {
            lcd_show_string(30 + 10 * 8, 230, 16, 16, 16, " ", RED);   /* �޷��� */
        }
        lcd_show_xnum(30 + 11 * 8, 230, temp / 100, 2, 16, 0, RED);    /* ��ʾ�������� */
        lcd_show_xnum(30 + 14 * 8, 230, temp % 100, 2, 16, 0X80, RED); /* ��ʾС������ */
    }
}

void Timer10msCallback(TimerHandle_t xTimer)
{
    task10msCounter++;
    xSemaphoreGive(BinarySemaphore_10ms); /* �ͷŶ�ֵ�ź��� */
}

void Timer1000msCallback(TimerHandle_t xTimer)
{
    task1000msCounter++;
    xSemaphoreGive(BinarySemaphore_1000ms); /* �ͷŶ�ֵ�ź��� */
}
