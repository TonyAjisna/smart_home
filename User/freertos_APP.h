#ifndef __FREERTOS_DEMO_H
#define __FREERTOS_DEMO_H

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

// 初步是将其分为基本任务、周期任务、特殊任务
// 周期任务使用信号量来实现，定时器指定时间更新信号量，应该是周期越长的任务优先级越高？





// 首先定义几个周期相关的宏 freertos_APP.h
#define FREERTOS_TASK_5MS 5U
#define FERRRTOS_TASK_20MS 20U
#define FREERTOS_TASK_50MS 50U
#define FREERTOS_TASK_100MS 100U
#define FREERTOS_TASK_500MS 500U
#define FREERTOS_TASK_1000MS 1000U



// 5ms定时器超时回调函数
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



