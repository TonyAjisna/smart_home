/**
 ****************************************************************************************************
 * @file        demo.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MW8266D模块TCP透传实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 探索者 F407开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#ifndef __DEMO_H
#define __DEMO_H
#include "stdint.h"

void esp_run(void);
void esp_key0_fun(uint8_t is_unvarnished);
void esp_key1_fun(uint8_t *is_unvarnished);
void esp_upload_data(uint8_t is_unvarnished);

#endif
