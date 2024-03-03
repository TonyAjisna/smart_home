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

/* ------------------------------Ӧ���߼������д��-------------------------------- */

/* �������������ﴴ�� */

/* task��������
    ���������������������ȼ�����ջ��С����������
 */
#define START_TASK_PRIO 1                   /* �������ȼ� */
#define START_STK_SIZE  128                 /* �����ջ��С */
TaskHandle_t            StartTask_Handler;  /* ������ */
void start_task(void *pvParameters);        /* ������ */

#define TASK1_PRIO      2                   /* �������ȼ� */
#define TASK1_STK_SIZE  128                 /* �����ջ��С */
TaskHandle_t            Task1Task_Handler;  /* ������ */
void task1(void *pvParameters);             /* ������ */

#define TASK2_PRIO      3                   /* �������ȼ� */
#define TASK2_STK_SIZE  128                 /* �����ջ��С */
TaskHandle_t            Task2Task_Handler;  /* ������ */
void task2(void *pvParameters);             /* ������ */




void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();       /* �����ٽ��� */

    /* ��������1 */
    xTaskCreate((TaskFunction_t )task1,
                (const char*    )"task1",
                (uint16_t       )TASK1_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK1_PRIO,
                (TaskHandle_t*  )&Task1Task_Handler);
    /* ��������2 */
    xTaskCreate((TaskFunction_t )task2,
                (const char*    )"task2",
                (uint16_t       )TASK2_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK2_PRIO,
                (TaskHandle_t*  )&Task2Task_Handler);
    vTaskDelete(StartTask_Handler); /* ɾ����ʼ���� */

    taskEXIT_CRITICAL();            /* �˳��ٽ��� */
}



/* LCDˢ��ʱʹ�õ���ɫ */
uint16_t lcd_discolor[11] = {WHITE, BLACK, BLUE, RED,
                             MAGENTA, GREEN, CYAN, YELLOW,
                             BROWN, BRRED, GRAY};


/* FreeRTOS������ں��� */
void freertos_APP(void)
{
    lcd_show_string(10, 10, 220, 32, 32, "STM32", RED);
    lcd_show_string(10, 47, 220, 24, 24, "FreeRTOS Porting", RED);
    lcd_show_string(10, 76, 220, 16, 16, "zhaobw@yeah.net", RED);
    
    xTaskCreate((TaskFunction_t )start_task,            /* ������ */
                (const char*    )"start_task",          /* �������� */
                (uint16_t       )START_STK_SIZE,        /* �����ջ��С */
                (void*          )NULL,                  /* ������������Ĳ��� */
                (UBaseType_t    )START_TASK_PRIO,       /* �������ȼ� */
                (TaskHandle_t*  )&StartTask_Handler);   /* ������ */
    vTaskStartScheduler();
}


bool SD_Status = true;
bool fonts_Status = true;
uint8_t tbuf[40];                           /* ��ǰ�����պ�ʱ�����ӡ��� */
uint8_t hour, min, sec, ampm;               /* RTC���� */
uint8_t year, month, date, week;
void task1(void *pvParameters) //��ʱ����Ϊ��ʾ������Ϣ��SD���������¶ȡ���ǰRTCʱ�䣩
{
    // uint32_t task1_num = 0;

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

    // if (!fonts_Status)
    // {
    //     /* ��ⲻ���ֿ� */
    //     lcd_show_string(30, 50, 300, 16, 16, "Font Error!", RED);
    //     vTaskDelay(200);
    //     lcd_fill(30, 50, 200 + 30, 50 + 16, WHITE);
    //     vTaskDelay(200);
    // }


    

    while(1)
    {
        // lcd_clear(lcd_discolor[++task1_num % 14]);                      /* ˢ����Ļ */
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

        vTaskDelay(1000);                                               /* ��ʱ1000ticks */
    }
}

void task2(void *pvParameters)
{
    float float_num = 0.0;
    short temp;
    
    /* ��ӡ�¶���Ϣ */
    lcd_show_string(23, 170, 200, 16, 16, "TEMPERATE:  00.00C", RED);
    temp = adc_get_temperature();   //�õ��¶�ֵ

    if (temp < 0)
    {
        temp = -temp;
        lcd_show_string(30 + 10 * 8, 170, 16, 16, 16, "-", RED);   /* ��ʾ���� */
    }
    else
    {
        lcd_show_string(30 + 10 * 8, 170, 16, 16, 16, " ", RED);   /* �޷��� */
    }
    lcd_show_xnum(30 + 11 * 8, 170, temp / 100, 2, 16, 0, RED);    /* ��ʾ�������� */
    lcd_show_xnum(30 + 14 * 8, 170, temp % 100, 2, 16, 0X80, RED); /* ��ʾС������ */

    while(1)
    {
        float_num += 0.01f;                         /* ������ֵ */

        vTaskDelay(1000);                           /* ��ʱ1000ticks */
    }
}

