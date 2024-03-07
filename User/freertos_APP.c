#include "freertos_APP.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
/*FreeRTOS*********************************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

/******************************************************************************************************/
/*FreeRTOS����*/

/* START_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define START_TASK_PRIO 1                   /* �������ȼ� */
#define START_STK_SIZE  128                 /* �����ջ��С */
TaskHandle_t            StartTask_Handler;  /* ������ */
void start_task(void *pvParameters);        /* ������ */

/* TASK1 ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define BASE_TASK_PRIO      3                   /* �������ȼ� */
#define BASE_TASK_STK_SIZE  128                 /* �����ջ��С */
TaskHandle_t            BaseTask_Handler;  /* ������ */
void base_task(void *pvParameters);             /* ������ */

#define TASK_10MS_PRIO      2                   /* �������ȼ� */
#define TASK_10MS_STK_SIZE  128                 /* �����ջ��С */
TaskHandle_t            TASK_10MS_Task_Handler;  /* ������ */
void task_10ms(void *pvParameters);  

#define TASK_1000MS_PRIO      3                   /* �������ȼ� */
#define TASK_1000MS_STK_SIZE  128                 /* �����ջ��С */
TaskHandle_t            TASK_1000MS_Task_Handler;  /* ������ */
void task_1000ms(void *pvParameters);  

SemaphoreHandle_t BinarySemaphore_10ms; /* ��ֵ�ź������ */
SemaphoreHandle_t BinarySemaphore_1000ms; /* ��ֵ�ź������ */

TimerHandle_t           Timer10Timer_Handler;/* ��ʱ��1��� */
TimerHandle_t           Timer1000Timer_Handler;/* ��ʱ��2��� */

void Timer10msCallback(TimerHandle_t xTimer);  /* ��ʱ��1��ʱ�ص����� */
void Timer1000msCallback(TimerHandle_t xTimer);  /* ��ʱ��2��ʱ�ص����� */


uint8_t task10msCounter = 0;
uint8_t task1000msCounter = 0;
/******************************************************************************************************/

/* LCDˢ��ʱʹ�õ���ɫ */
uint16_t lcd_discolor[11] = {WHITE, BLACK, BLUE, RED,
                             MAGENTA, GREEN, CYAN, YELLOW,
                             BROWN, BRRED, GRAY};

/**
 * @brief       FreeRTOS������ں���
 * @param       ��
 * @retval      ��
 */
void freertos_APP(void)
{
    lcd_show_string(10, 10, 220, 32, 32, "STM32", RED);
    lcd_show_string(10, 47, 220, 24, 24, "Timer", RED);
    lcd_show_string(10, 76, 220, 16, 16, "ATOM@ALIENTEK", RED);
    
    lcd_draw_rectangle(5, 110, 115, 314, BLACK);
    lcd_draw_rectangle(125, 110, 234, 314, BLACK);
    lcd_draw_line(5, 130, 115, 130, BLACK);
    lcd_draw_line(125, 130, 234, 130, BLACK);
    lcd_show_string(15, 111, 110, 16, 16, "Timer1: 000", BLUE);
    lcd_show_string(135, 111, 110, 16, 16, "Timer2: 000", BLUE);
    
    xTaskCreate((TaskFunction_t )start_task,            /* ������ */
                (const char*    )"start_task",          /* �������� */
                (uint16_t       )START_STK_SIZE,        /* �����ջ��С */
                (void*          )NULL,                  /* ������������Ĳ��� */
                (UBaseType_t    )START_TASK_PRIO,       /* �������ȼ� */
                (TaskHandle_t*  )&StartTask_Handler);   /* ������ */
    vTaskStartScheduler();
}

/**
 * @brief       start_task
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           /* �����ٽ��� */
    /* ������ֵ�ź��� */
    if (BinarySemaphore_10ms == NULL)
    {
        BinarySemaphore_10ms = xSemaphoreCreateBinary();
    }
    if (BinarySemaphore_1000ms == NULL)
    {
        BinarySemaphore_1000ms = xSemaphoreCreateBinary();
    }
    
    
    BinarySemaphore_1000ms = xSemaphoreCreateBinary();
    /* ��ʱ��1����Ϊ���ڶ�ʱ�� */
    Timer10Timer_Handler = xTimerCreate((const char*  )"Timer10ms",                 /* ��ʱ���� */
                                      (TickType_t   )10,                      /* ��ʱ����ʱʱ�� */
                                      (UBaseType_t  )pdTRUE,                    /* ���ڶ�ʱ�� */
                                      (void*        )1,                         /* ��ʱ��ID */
                                      (TimerCallbackFunction_t)Timer10msCallback); /* ��ʱ���ص����� */
    /* ��ʱ��2����Ϊ���ڶ�ʱ�� */
    Timer1000Timer_Handler = xTimerCreate((const char*  )"Timer1000ms",                 /* ��ʱ���� */
                                     (TickType_t    )1000,                      /* ��ʱ����ʱʱ�� */
                                     (UBaseType_t   )pdTRUE,                   /* ���ζ�ʱ�� */
                                     (void*         )2,                         /* ��ʱ��ID */
                                     (TimerCallbackFunction_t)Timer1000msCallback);  /* ��ʱ���ص����� */
    /* ��������1 */
    xTaskCreate((TaskFunction_t )base_task,
                (const char*    )"base_task",
                (uint16_t       )BASE_TASK_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )BASE_TASK_PRIO,
                (TaskHandle_t*  )&BaseTask_Handler);
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

/**
 * @brief       base_task
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void base_task(void *pvParameters)
{
    uint8_t key = 0;
    
    while (1)
    {
        if ((Timer10Timer_Handler != NULL) && (Timer1000Timer_Handler != NULL))
        {
            key = key_scan(0);
            
            switch (key)
            {
                case KEY0_PRES:
                {
                    xTimerStart((TimerHandle_t  )Timer10Timer_Handler,   /* �������Ķ�ʱ����� */
                                (TickType_t     )portMAX_DELAY);        /* �ȴ�ϵͳ������ʱ�������ʱ�� */
                    xTimerStart((TimerHandle_t  )Timer1000Timer_Handler,   /* �������Ķ�ʱ����� */
                                (TickType_t     )portMAX_DELAY);        /* �ȴ�ϵͳ������ʱ�������ʱ�� */
                    break;
                }
                case KEY1_PRES:
                {
                    xTimerStop((TimerHandle_t  )Timer10Timer_Handler,    /* ��ֹͣ�Ķ�ʱ����� */
                                (TickType_t     )portMAX_DELAY);        /* �ȴ�ϵͳֹͣ��ʱ�������ʱ�� */
                    xTimerStop((TimerHandle_t  )Timer1000Timer_Handler,    /* ��ֹͣ�Ķ�ʱ����� */
                                (TickType_t     )portMAX_DELAY);        /* �ȴ�ϵͳֹͣ��ʱ�������ʱ�� */
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
    }
}

/**
 * @brief       Timer1��ʱ�ص�����
 * @param       xTimer : �������(δ�õ�)
 * @retval      ��
 */
void Timer10msCallback(TimerHandle_t xTimer)
{
    static uint32_t timer1_num = 0;

    xSemaphoreGive(BinarySemaphore_10ms);                    /* �ͷŶ�ֵ�ź��� */
    task10msCounter ++;

    lcd_fill(6, 131, 114, 313, lcd_discolor[++timer1_num % 11]);    /* LCD����ˢ�� */
    lcd_show_xnum(79, 111, timer1_num, 3, 16, 0x80, BLUE);          /* ��ʾ��ʱ��1��ʱ���� */
}

/**
 * @brief       Timer2��ʱ�ص�����
 * @param       xTimer : �������(δ�õ�)
 * @retval      ��
 */
void Timer1000msCallback(TimerHandle_t xTimer)
{
    static uint32_t timer2_num = 0;
    
    xSemaphoreGive(BinarySemaphore_1000ms);                    /* �ͷŶ�ֵ�ź��� */
    task1000msCounter ++;

    lcd_fill(126, 131, 233, 313, lcd_discolor[++timer2_num % 11]);  /* LCD����ˢ�� */
    lcd_show_xnum(199, 111, timer2_num, 3, 16, 0x80, BLUE);         /* ��ʾ��ʱ��2��ʱ���� */
}
