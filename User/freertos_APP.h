#ifndef __FREERTOS_APP_H
#define __FREERTOS_APP_H

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

#define FERRRTOS_TASK_20MS 20U
#define FREERTOS_TASK_1000MS 1000U

void freertos_demo(void); 

// 5ms定时器超时回调函�?
void Timer10msCallback(TimerHandle_t xTimer);
void Timer1000msCallback(TimerHandle_t xTimer);


#endif
