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
/*FreeRTOS����*/

/* START_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define START_TASK_PRIO         5           /* �������ȼ� */
#define START_STK_SIZE          128         /* �����ջ��С */
TaskHandle_t StartTask_Handler;             /* ������ */
void start_task(void *pvParameters);        /* ������ */

/* LWIP_DEMO ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define LWIP_DMEO_TASK_PRIO     11          /* �������ȼ� */
#define LWIP_DMEO_STK_SIZE      1024        /* �����ջ��С */
TaskHandle_t LWIP_Task_Handler;             /* ������ */
void lwip_demo_task(void *pvParameters);    /* ������ */

/* LED_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define LED_TASK_PRIO           9           /* �������ȼ� */
#define LED_STK_SIZE            128         /* �����ջ��С */
TaskHandle_t LEDTask_Handler;               /* ������ */
void led_task(void *pvParameters);          /* ������ */


#define TASK_10MS_PRIO 12             /* �������ȼ� */
#define TASK_10MS_STK_SIZE 128       /* �����ջ��С */
TaskHandle_t TASK_10MS_Task_Handler; /* ������ */
void task_10ms(void *pvParameters);

#define TASK_1000MS_PRIO 12             /* �������ȼ� */
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
/******************************************************************************************************/


/**
 * @breif       ����UI
 * @param       mode :  bit0:0,������;1,����ǰ�벿��UI
 *                      bit1:0,������;1,���غ�벿��UI
 * @retval      ��
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
            sprintf((char*)buf,"DHCP IP:%d.%d.%d.%d",g_lwipdev.ip[0],g_lwipdev.ip[1],g_lwipdev.ip[2],g_lwipdev.ip[3]);     /* ��ʾ��̬IP��ַ */
        }
        else
        {
            sprintf((char*)buf,"Static IP:%d.%d.%d.%d",g_lwipdev.ip[0],g_lwipdev.ip[1],g_lwipdev.ip[2],g_lwipdev.ip[3]);    /* ��ӡ��̬IP��ַ */
        }
        
        lcd_show_string(5, 130, 200, 16, 16, (char*)buf, MAGENTA);
        
        speed = ethernet_chip_get_speed();      /* �õ����� */
        
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
 * @param       ��
 * @retval      ��
 */
void freertos_demo(void)
{
    /* start_task���� */
    xTaskCreate((TaskFunction_t )start_task,
                (const char *   )"start_task",
                (uint16_t       )START_STK_SIZE,
                (void *         )NULL,
                (UBaseType_t    )START_TASK_PRIO,
                (TaskHandle_t * )&StartTask_Handler);

    vTaskStartScheduler(); /* ����������� */
}

/**
 * @brief       start_task
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void start_task(void *pvParameters)
{
    pvParameters = pvParameters;
    
    g_lwipdev.lwip_display_fn = lwip_test_ui;
    
    lwip_test_ui(1);    /* ���غ�ǰ����UI */
    
    while (lwip_comm_init() != 0)
    {
        lcd_show_string(30, 110, 200, 16, 16, "lwIP Init failed!!", RED);
        delay_ms(500);
        lcd_fill(30, 50, 200 + 30, 50 + 16, WHITE);
        lcd_show_string(30, 110, 200, 16, 16, "Retrying...       ", RED);
        delay_ms(500);
        LED1_TOGGLE();
    }
    
    while (!ethernet_read_phy(PHY_SR))  /* ���MCU��PHYоƬ�Ƿ�ͨ�ųɹ� */
    {
        printf("MCU��PHYоƬͨ��ʧ�ܣ������·����Դ�룡������\r\n");
    }
    
    while ((g_lwipdev.dhcpstatus != 2) && (g_lwipdev.dhcpstatus != 0XFF))  /* �ȴ�DHCP��ȡ�ɹ�/��ʱ��� */
    {
        vTaskDelay(5);
    }
    

    
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
    /* ����lwIP���� */
    xTaskCreate((TaskFunction_t )lwip_demo_task,
                (const char*    )"lwip_demo_task",
                (uint16_t       )LWIP_DMEO_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LWIP_DMEO_TASK_PRIO,
                (TaskHandle_t*  )&LWIP_Task_Handler);

    /* LED�������� */
    xTaskCreate((TaskFunction_t )led_task,
                (const char*    )"led_task",
                (uint16_t       )LED_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED_TASK_PRIO,
                (TaskHandle_t*  )&LEDTask_Handler);

    vTaskDelete(StartTask_Handler); /* ɾ����ʼ���� */
    taskEXIT_CRITICAL();            /* �˳��ٽ��� */
    
}

/**
 * @brief       lwIP��������
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void lwip_demo_task(void *pvParameters)
{
    pvParameters = pvParameters;
    uint8_t key = 0;

    if ((Timer10Timer_Handler != NULL) && (Timer1000Timer_Handler != NULL))
    {
        xTimerStart((TimerHandle_t)Timer10Timer_Handler,   /* �������Ķ�ʱ����� */
                            (TickType_t)portMAX_DELAY);            /* �ȴ�ϵͳ������ʱ�������ʱ�� */
        xTimerStart((TimerHandle_t)Timer1000Timer_Handler, /* �������Ķ�ʱ����� */
                            (TickType_t)portMAX_DELAY);            /* �ȴ�ϵͳ������ʱ�������ʱ�� */
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
        //         xTimerStart((TimerHandle_t)Timer10Timer_Handler,   /* �������Ķ�ʱ����� */
        //                     (TickType_t)portMAX_DELAY);            /* �ȴ�ϵͳ������ʱ�������ʱ�� */
        //         xTimerStart((TimerHandle_t)Timer1000Timer_Handler, /* �������Ķ�ʱ����� */
        //                     (TickType_t)portMAX_DELAY);            /* �ȴ�ϵͳ������ʱ�������ʱ�� */
        //         break;
        //     }
        //     case KEY1_PRES:
        //     {
        //         xTimerStop((TimerHandle_t)Timer10Timer_Handler,   /* ��ֹͣ�Ķ�ʱ����� */
        //                    (TickType_t)portMAX_DELAY);            /* �ȴ�ϵͳֹͣ��ʱ�������ʱ�� */
        //         xTimerStop((TimerHandle_t)Timer1000Timer_Handler, /* ��ֹͣ�Ķ�ʱ����� */
        //                    (TickType_t)portMAX_DELAY);            /* �ȴ�ϵͳֹͣ��ʱ�������ʱ�� */
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
 * @brief       ϵͳ������
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
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
        xSemaphoreTake(BinarySemaphore_10ms, portMAX_DELAY); /* ��ȡ��ֵ�ź��� */
        printf("task10msCounter: %u\n", task10msCounter);
    }
}

extern float g_temp;    /* �¶�ֵ */
extern float g_humid;   /* ʪ��ֵ */
void task_1000ms(void *pvParameters)
{
    while (1)
    {
        xSemaphoreTake(BinarySemaphore_1000ms, portMAX_DELAY); /* ��ȡ��ֵ�ź��� */
        printf("task1000msCounter: %u\n", task1000msCounter);
    }
}

uint8_t temperature = 0;
uint8_t humidity = 0;
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
