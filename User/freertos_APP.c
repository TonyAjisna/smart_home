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

/* task��������
    ���������������������ȼ�����ջ��С����������
    �������ȼ��ߵͺ����Ӧ�����ȼ���ֵ�����ȣ�Ҳ����˵�������ȼ���ֵΪ0�����ȼ���͡��ж����෴

    ������Ӧ��Ϊ�������񡢶�ʱ�����������������ʾ�����񡢵�Դ����Ӧ���ڻ���������Ƶ�������������������ݰ����͵����ڶ�ʱ����
 */




//------------------------------------------------------------------------------��������------------------------------------------------------------------------------
// ����start����
#define START_TASK_PRIO 1                   /* �������ȼ� */
#define START_STK_SIZE  128                 /* �����ջ��С */
TaskHandle_t            StartTask_Handler;  /* ������ */
void start_task(void *pvParameters);        /* ������ */

// �����ʼ������
// #define INIT_TASK_PRIO 2            /* �������ȼ� */
// #define INIT_TASK_SIZE 128          /* �����ջ��С */
// TaskHandle_t InitTask_Handler;      /* ������ */
// void init_task(void *pvParameters); /* ������ */

// // �����������
// #define BASE_TASK_PRIO 3            /* �������ȼ� */
// #define BASE_TASK_SIZE 128          /* �����ջ��С */
// TaskHandle_t BaseTask_Handler;      /* ������ */
// void base_task(void *pvParameters); /* ������ */

// ����6����������
#define TASK_5MS_STK_SIZE 10            /* �������ȼ� */
#define TASK_5MS_PRIO 128           /* �����ջ��С */
TaskHandle_t Task_5MS_Task_Handler; /* ������ */
void task_5ms(void *pvParameters);  /* ������ */

#define TASK_20MS_STK_SIZE 11            /* �������ȼ� */
#define TASK_20MS_PRIO 128           /* �����ջ��С */
TaskHandle_t Task_20MS_Task_Handler; /* ������ */
void task_20ms(void *pvParameters);  /* ������ */

#define TASK_50MS_STK_SIZE 12            /* �������ȼ� */
#define TASK_50MS_PRIO 128           /* �����ջ��С */
TaskHandle_t Task_50MS_Task_Handler; /* ������ */
void task_50ms(void *pvParameters);  /* ������ */

#define TASK_100MS_STK_SIZE 13            /* �������ȼ� */
#define TASK_100MS_PRIO 128           /* �����ջ��С */
TaskHandle_t Task_100MS_Task_Handler; /* ������ */
void task_100ms(void *pvParameters);       /* ������ */

#define TASK_500MS_STK_SIZE 14            /* �������ȼ� */
#define TASK_500MS_PRIO 128           /* �����ջ��С */
TaskHandle_t Task_500MS_Task_Handler; /* ������ */
void task_500ms(void *pvParameters);  /* ������ */

#define TASK_1000MS_STK_SIZE 15            /* �������ȼ� */
#define TASK_1000MS_PRIO 128           /* �����ջ��С */
TaskHandle_t Task_1000MS_Task_Handler; /* ������ */
void task_1000ms(void *pvParameters);  /* ������ */

// #define TASK1_PRIO      2                   /* �������ȼ� */
// #define TASK1_STK_SIZE  128                 /* �����ջ��С */
// TaskHandle_t            Task1Task_Handler;  /* ������ */
// void task1(void *pvParameters);             /* ������ */

// #define TASK2_PRIO      3                   /* �������ȼ� */
// #define TASK2_STK_SIZE  128                 /* �����ջ��С */
// TaskHandle_t            Task2Task_Handler;  /* ������ */
// void task2(void *pvParameters);             /* ������ */
//------------------------------------------------------------------------------��������------------------------------------------------------------------------------




// �����ź������	freertos_APP.h
SemaphoreHandle_t BinarySemaphore_5ms;
SemaphoreHandle_t BinarySemaphore_20ms;
SemaphoreHandle_t BinarySemaphore_50ms;
SemaphoreHandle_t BinarySemaphore_100ms;
SemaphoreHandle_t BinarySemaphore_500ms;
SemaphoreHandle_t BinarySemaphore_1000ms;

// 5ms��ʱ�����
TimerHandle_t Timer_5ms_Handler;
TimerHandle_t Timer_20ms_Handler;
TimerHandle_t Timer_50ms_Handler;
TimerHandle_t Timer_100ms_Handler;
TimerHandle_t Timer_500ms_Handler;
TimerHandle_t Timer_1000ms_Handler;


//�������ڼ�����
uint8_t Counter5ms = 0;
uint8_t Counter20ms = 0;
uint8_t Counter50ms = 0;
uint8_t Counter100ms = 0;
uint8_t Counter500ms = 0;
uint8_t Counter1000ms = 0;

// ͳһ������ֵ�ź�������
void CreateBinarySemaphores(void)
{
    BinarySemaphore_5ms = xSemaphoreCreateBinary();
    BinarySemaphore_20ms = xSemaphoreCreateBinary();
    BinarySemaphore_50ms = xSemaphoreCreateBinary();
    BinarySemaphore_100ms = xSemaphoreCreateBinary();
    BinarySemaphore_500ms = xSemaphoreCreateBinary();
    BinarySemaphore_1000ms = xSemaphoreCreateBinary();
}





//------------------------------------------------------------------------------�������ڶ�ʱ��------------------------------------------------------------------------------
void CreateTimers(void)
{
    /* ��ʱ������Ϊ���ڶ�ʱ�� */
    Timer_5ms_Handler = xTimerCreate((const char *)"Timer_5ms",                     /* ��ʱ���� */
                                     (TickType_t)5,                                 /* ��ʱ����ʱʱ�� */
                                     (UBaseType_t)pdTRUE,                           /* ���ڶ�ʱ�� */
                                     (void *)1,                                     /* ��ʱ��ID */
                                     (TimerCallbackFunction_t)Timer_5ms_Callback); /* ��ʱ���ص����� */

    /* ��ʱ������Ϊ���ڶ�ʱ�� */
    Timer_20ms_Handler = xTimerCreate((const char *)"Timer_20ms",                    /* ��ʱ���� */
                                      (TickType_t)20,                                /* ��ʱ����ʱʱ�� */
                                      (UBaseType_t)pdTRUE,                           /* ���ڶ�ʱ�� */
                                      (void *)2,                                     /* ��ʱ��ID */
                                      (TimerCallbackFunction_t)Timer_20ms_Callback); /* ��ʱ���ص����� */

    /* ��ʱ������Ϊ���ڶ�ʱ�� */
    Timer_50ms_Handler = xTimerCreate((const char *)"Timer_50ms",                     /* ��ʱ���� */
                                      (TickType_t)50,                                 /* ��ʱ����ʱʱ�� */
                                      (UBaseType_t)pdTRUE,                            /* ���ڶ�ʱ�� */
                                      (void *)3,                                      /* ��ʱ��ID */
                                      (TimerCallbackFunction_t)Timer_50ms_Callback); /* ��ʱ���ص����� */

    /* ��ʱ������Ϊ���ڶ�ʱ�� */
    Timer_100ms_Handler = xTimerCreate((const char *)"Timer_100ms",                    /* ��ʱ���� */
                                       (TickType_t)100,                                /* ��ʱ����ʱʱ�� */
                                       (UBaseType_t)pdTRUE,                            /* ���ڶ�ʱ�� */
                                       (void *)4,                                      /* ��ʱ��ID */
                                       (TimerCallbackFunction_t)Timer_100ms_Callback); /* ��ʱ���ص����� */

    /* ��ʱ������Ϊ���ڶ�ʱ�� */
    Timer_500ms_Handler = xTimerCreate((const char *)"Timer_500ms",                    /* ��ʱ���� */
                                       (TickType_t)500,                                /* ��ʱ����ʱʱ�� */
                                       (UBaseType_t)pdTRUE,                            /* ���ڶ�ʱ�� */
                                       (void *)5,                                      /* ��ʱ��ID */
                                       (TimerCallbackFunction_t)Timer_500ms_Callback); /* ��ʱ���ص����� */

    /* ��ʱ������Ϊ���ڶ�ʱ�� */
    Timer_1000ms_Handler = xTimerCreate((const char *)"Timer_1000ms",                    /* ��ʱ���� */
                                       (TickType_t)1000,                                /* ��ʱ����ʱʱ�� */
                                       (UBaseType_t)pdTRUE,                            /* ���ڶ�ʱ�� */
                                       (void *)6,                                      /* ��ʱ��ID */
                                       (TimerCallbackFunction_t)Timer_1000ms_Callback); /* ��ʱ���ص����� */                                                                      
}
//------------------------------------------------------------------------------�������ڶ�ʱ��------------------------------------------------------------------------------







//------------------------------------------------------------------------------ʵ�����ڶ�ʱ��------------------------------------------------------------------------------
// ���붨ʱ����ʱ�����ź���
void Timer_5ms_Callback(TimerHandle_t xTimer)
{
    xSemaphoreGive(BinarySemaphore_5ms); // �ͷŶ�ֵ�ź�����0-->1
    Counter5ms ++;
}

void Timer_20ms_Callback(TimerHandle_t xTimer)
{
    xSemaphoreGive(BinarySemaphore_20ms); // �ͷŶ�ֵ�ź�����0-->1
    Counter20ms ++;
}

void Timer_50ms_Callback(TimerHandle_t xTimer)
{
    xSemaphoreGive(BinarySemaphore_50ms); // �ͷŶ�ֵ�ź�����0-->1
    Counter50ms ++;
}

void Timer_100ms_Callback(TimerHandle_t xTimer)
{
    xSemaphoreGive(BinarySemaphore_100ms); // �ͷŶ�ֵ�ź�����0-->1
    Counter100ms ++;
}

void Timer_500ms_Callback(TimerHandle_t xTimer)
{
    xSemaphoreGive(BinarySemaphore_500ms); // �ͷŶ�ֵ�ź�����0-->1
    Counter500ms ++;
}

void Timer_1000ms_Callback(TimerHandle_t xTimer)
{
    xSemaphoreGive(BinarySemaphore_1000ms); // �ͷŶ�ֵ�ź�����0-->1
    Counter1000ms ++;
}
//------------------------------------------------------------------------------ʵ�����ڶ�ʱ��------------------------------------------------------------------------------







//------------------------------------------------------------------------------ʵ������------------------------------------------------------------------------------
// ��������񴴽��ŵ�CreateTask()��
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

    // /* ��������1 */
    // xTaskCreate((TaskFunction_t)task1,
    //             (const char *)"task1",
    //             (uint16_t)TASK1_STK_SIZE,
    //             (void *)NULL,
    //             (UBaseType_t)TASK1_PRIO,
    //             (TaskHandle_t *)&Task1Task_Handler);
    // /* ��������2 */
    // xTaskCreate((TaskFunction_t)task2,
    //             (const char *)"task2",
    //             (uint16_t)TASK2_STK_SIZE,
    //             (void *)NULL,
    //             (UBaseType_t)TASK2_PRIO,
    //             (TaskHandle_t *)&Task2Task_Handler);
}

// ��ʼ���񣬴����������������������������
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           // �����ٽ���
    CreateBinarySemaphores();       // �����ź�ֵ
    CreateTimers();                 // ������ʱ��
    CreateTasks();                  // ��������
    vTaskDelete(StartTask_Handler); // ɾ����ʼ����
    taskEXIT_CRITICAL();            // �˳��ٽ���
}



void task_5ms(void *pvParameters)
{
    while (1)
    {
        xSemaphoreTake(BinarySemaphore_5ms, portMAX_DELAY);    // ��ȡ��ֵ�ź�����1-->0
        lcd_show_xnum(79, 111, Counter5ms, 3, 16, 0x80, BLUE); // ��ӡ����
    }
}

void task_20ms(void *pvParameters)
{
    while (1)
    {
        xSemaphoreTake(BinarySemaphore_20ms, portMAX_DELAY);    // ��ȡ��ֵ�ź�����1-->0
        lcd_show_xnum(79, 111, Counter20ms, 3, 16, 0x80, BLUE); // ��ӡ����
    }
}

void task_50ms(void *pvParameters)
{
    while (1)
    {
        xSemaphoreTake(BinarySemaphore_50ms, portMAX_DELAY);    // ��ȡ��ֵ�ź�����1-->0
        lcd_show_xnum(79, 111, Counter50ms, 3, 16, 0x80, BLUE); // ��ӡ����
    }
}

void task_100ms(void *pvParameters)
{
    while (1)
    {
        xSemaphoreTake(BinarySemaphore_100ms, portMAX_DELAY);    // ��ȡ��ֵ�ź�����1-->0
        lcd_show_xnum(79, 111, Counter100ms, 3, 16, 0x80, BLUE); // ��ӡ����
    }
}

void task_500ms(void *pvParameters)
{
    while (1)
    {
        xSemaphoreTake(BinarySemaphore_500ms, portMAX_DELAY);    // ��ȡ��ֵ�ź�����1-->0
        lcd_show_xnum(79, 111, Counter500ms, 3, 16, 0x80, BLUE); // ��ӡ����
    }
}

void task_1000ms(void *pvParameters)
{
    while (1)
    {
        xSemaphoreTake(BinarySemaphore_1000ms, portMAX_DELAY);    // ��ȡ��ֵ�ź�����1-->0
        lcd_show_xnum(79, 111, Counter1000ms, 3, 16, 0x80, BLUE); // ��ӡ����
    }
}

/* LCDˢ��ʱʹ�õ���ɫ */
uint16_t lcd_discolor[11] = {WHITE, BLACK, BLUE, RED,
                             MAGENTA, GREEN, CYAN, YELLOW,
                             BROWN, BRRED, GRAY};

/* FreeRTOS������ں��� */
void freertos_APP(void)
{
    lcd_show_string(10, 10, 220, 32, 32, "SMART_HOME", RED);
    lcd_show_string(10, 47, 220, 24, 24, "FreeRTOS Porting", RED);
    lcd_show_string(10, 76, 220, 16, 16, "zhaobw@yeah.net", RED);

    xTaskCreate((TaskFunction_t)start_task,          /* ������ */
                (const char *)"start_task",          /* �������� */
                (uint16_t)START_STK_SIZE,            /* �����ջ��С */
                (void *)NULL,                        /* ������������Ĳ��� */
                (UBaseType_t)START_TASK_PRIO,        /* �������ȼ� */
                (TaskHandle_t *)&StartTask_Handler); /* ������ */
    vTaskStartScheduler();
}

bool SD_Status = true;
// bool fonts_Status = true;
// uint8_t tbuf[40];             /* ��ǰ�����պ�ʱ�����ӡ��� */
// uint8_t hour, min, sec, ampm; /* RTC���� */
// uint8_t year, month, date, week;
// void task1(void *pvParameters) // ��ʱ����Ϊ��ʾ������Ϣ��SD���������¶ȡ���ǰRTCʱ�䣩
// {
//     // uint32_t task1_num = 0;

//     if (!SD_Status)
//     {
//         /* ��ⲻ��SD�� */
//         lcd_show_string(30, 110, 200, 16, 16, "SD Card Error!", RED);
//         vTaskDelay(1000);
//         lcd_show_string(30, 110, 200, 16, 16, "Please Check! ", RED);
//         vTaskDelay(1000);
//         LED0_TOGGLE(); /* �����˸ */
//     }
//     else
//     {
//         /* ���SD���ɹ�����ӡ��Ϣ */
//         lcd_show_string(30, 110, 200, 16, 16, "SD Card OK    ", RED);
//         lcd_show_string(30, 140, 200, 16, 16, "SD Card Size:     MB", RED);
//         lcd_show_num(30 + 13 * 8, 140, SD_TOTAL_SIZE_MB(&g_sdcard_handler), 5, 16, RED); /* ��ʾSD������ */
//     }

//     // if (!fonts_Status)
//     // {
//     //     /* ��ⲻ���ֿ� */
//     //     lcd_show_string(30, 50, 300, 16, 16, "Font Error!", RED);
//     //     vTaskDelay(200);
//     //     lcd_fill(30, 50, 200 + 30, 50 + 16, WHITE);
//     //     vTaskDelay(200);
//     // }

//     while (1)
//     {
//         // lcd_clear(lcd_discolor[++task1_num % 14]);                      /* ˢ����Ļ */
//         /* ��ӡRTCϵͳʱ�� */
//         rtc_get_date(&year, &month, &date, &week); /* ��ӡ������ */
//         sprintf((char *)tbuf, "TIME: 20%02d/%02d/%02d", year, month, date);
//         lcd_show_string(23, 200, 210, 16, 16, (char *)tbuf, RED);
//         lcd_show_char(150, 200, ' ', 16, 0, BLUE); /* �м�ո� */
//         lcd_show_char(160, 200, ' ', 16, 0, BLUE);
//         lcd_show_char(155, 200, ' ', 16, 0, BLUE);

//         rtc_get_time(&hour, &min, &sec, &ampm); /* ��ӡʱ���� */
//         sprintf((char *)tbuf, "%02d:%02d:%02d", hour, min, sec);
//         lcd_show_string(170, 200, 210, 16, 16, (char *)tbuf, RED);

//         vTaskDelay(1000); /* ��ʱ1000ticks */
//     }
// }

// void task2(void *pvParameters)
// {
//     float float_num = 0.0;
//     short temp;

//     /* ��ӡ�¶���Ϣ */
//     lcd_show_string(23, 170, 200, 16, 16, "TEMPERATE:  00.00C", RED);
//     temp = adc_get_temperature(); // �õ��¶�ֵ

//     if (temp < 0)
//     {
//         temp = -temp;
//         lcd_show_string(30 + 10 * 8, 170, 16, 16, 16, "-", RED); /* ��ʾ���� */
//     }
//     else
//     {
//         lcd_show_string(30 + 10 * 8, 170, 16, 16, 16, " ", RED); /* �޷��� */
//     }
//     lcd_show_xnum(30 + 11 * 8, 170, temp / 100, 2, 16, 0, RED);    /* ��ʾ�������� */
//     lcd_show_xnum(30 + 14 * 8, 170, temp % 100, 2, 16, 0X80, RED); /* ��ʾС������ */

//     while (1)
//     {
//         float_num += 0.01f; /* ������ֵ */

//         vTaskDelay(1000); /* ��ʱ1000ticks */
//     }
// }
