#ifndef __FREERTOS_DEMO_H
#define __FREERTOS_DEMO_H

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

// �����ǽ����Ϊ������������������������
// ��������ʹ���ź�����ʵ�֣���ʱ��ָ��ʱ������ź�����Ӧ��������Խ�����������ȼ�Խ�ߣ�





// ���ȶ��弸��������صĺ� freertos_APP.h
#define FREERTOS_TASK_5MS 5U
#define FERRRTOS_TASK_20MS 20U
#define FREERTOS_TASK_50MS 50U
#define FREERTOS_TASK_100MS 100U
#define FREERTOS_TASK_500MS 500U
#define FREERTOS_TASK_1000MS 1000U



// 5ms��ʱ����ʱ�ص�����
void Timer_5ms_Callback(TimerHandle_t xTimer);
void Timer_20ms_Callback(TimerHandle_t xTimer);
void Timer_50ms_Callback(TimerHandle_t xTimer);
void Timer_100ms_Callback(TimerHandle_t xTimer);
void Timer_500ms_Callback(TimerHandle_t xTimer);
void Timer_1000ms_Callback(TimerHandle_t xTimer);

void CreateBinarySemaphores(void);
void CreateTimers(void);
void CreateTasks(void);
void start_task(void *pvParameters);

void freertos_APP(void);

#endif



