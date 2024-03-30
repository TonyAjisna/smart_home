#ifndef __PLAY_H
#define __PLAY_H

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

// ATK-MW579模块配置
void atk_mw579_play(void);

//主从通信
void atk_mw579_transmission(void);

#endif
