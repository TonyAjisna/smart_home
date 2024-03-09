/**
 ****************************************************************************************************
 * @file        demo.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MW8266Dģ��TCP͸��ʵ��
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� ̽���� F407������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
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
