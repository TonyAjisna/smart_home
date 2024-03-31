/**
 ****************************************************************************************************
 * @file        atk_mw579.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MW579ģ����������
 * @license     Copyright (c) 2020-2032, �������������ӿƼ����޹�˾
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

#ifndef __ATK_MW579_H
#define __ATK_MW579_H

#include "./SYSTEM/sys/sys.h"
#include "./BSP/ATK_MW579/atk_mw579_uart.h"

/* ���Ŷ��� */
#define ATK_MW579_STA_GPIO_PORT             GPIOF
#define ATK_MW579_STA_GPIO_PIN              GPIO_PIN_6
#define ATK_MW579_STA_GPIO_CLK_ENABLE()     do{ __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)
#define ATK_MW579_WKUP_GPIO_PORT            GPIOC
#define ATK_MW579_WKUP_GPIO_PIN             GPIO_PIN_0
#define ATK_MW579_WKUP_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)

/* IO���� */
#define ATK_MW579_READ_STA()                HAL_GPIO_ReadPin(ATK_MW579_STA_GPIO_PORT, ATK_MW579_STA_GPIO_PIN)
#define ATK_MW579_WKUP(x)                   do{ x ?                                                                                     \
                                                HAL_GPIO_WritePin(ATK_MW579_WKUP_GPIO_PORT, ATK_MW579_WKUP_GPIO_PIN, GPIO_PIN_SET) :    \
                                                HAL_GPIO_WritePin(ATK_MW579_WKUP_GPIO_PORT, ATK_MW579_WKUP_GPIO_PIN, GPIO_PIN_RESET);   \
                                            }while(0)

/* ����״̬ö�� */
typedef enum
{
    ATK_MW579_CONNECTED = 0x00,             /* ������ */
    ATK_MW579_DISCONNECTED,                 /* δ���� */
} atk_mw579_conn_sta_t;

/* ���书��ö�� */
typedef enum
{
    ATK_MW579_TPL_N20DBM = 0x00,            /* -20dBm */
    ATK_MW579_TPL_N14DBM,                   /* -14dBm */
    ATK_MW579_TPL_N8DBM,                    /* -8dBm */
    ATK_MW579_TPL_N3DBM,                    /* -3dBm */
    ATK_MW579_TPL_P0DBM,                    /* +0dBm */
    ATK_MW579_TPL_P1DBM,                    /* +1dBm */
    ATK_MW579_TPL_P2DBM,                    /* +2dBm */
    ATK_MW579_TPL_P3DBM,                    /* +3dBm */
} atk_mw579_tpl_t;

/* ���ڲ�����ö�� */
typedef enum
{
    ATK_MW579_UART_BAUDRATE_1200 = 0x00,    /* 1200bps */
    ATK_MW579_UART_BAUDRATE_2400,           /* 2400bps */
    ATK_MW579_UART_BAUDRATE_4800,           /* 4800bps */
    ATK_MW579_UART_BAUDRATE_9600,           /* 9600bps */
    ATK_MW579_UART_BAUDRATE_14400,          /* 14400bps */
    ATK_MW579_UART_BAUDRATE_19200,          /* 19200bps */
    ATK_MW579_UART_BAUDRATE_38400,          /* 38400bps */
    ATK_MW579_UART_BAUDRATE_43000,          /* 43000bps */
    ATK_MW579_UART_BAUDRATE_57600,          /* 57600bps */
    ATK_MW579_UART_BAUDRATE_76800,          /* 76800bps */
    ATK_MW579_UART_BAUDRATE_115200,         /* 115200bps */
    ATK_MW579_UART_BAUDRATE_128000,         /* 128000bps */
    ATK_MW579_UART_BAUDRATE_230400,         /* 230400bps */
} atk_mw579_uart_baudrate_t;

/*��������λö�� */
typedef enum
{
    ATK_MW579_UART_DATA_5 = 0x00,           /* 5λ����λ */
    ATK_MW579_UART_DATA_6,                  /* 6λ����λ */
    ATK_MW579_UART_DATA_7,                  /* 7λ����λ */
    ATK_MW579_UART_DATA_8,                  /* 8λ����λ */
} atk_mw579_uart_data_t;

/* ����У��λö�� */
typedef enum
{
    ATK_MW579_UART_PARI_NONE = 0x00,        /* ��У�� */
    ATK_MW579_UART_PARI_ODD,                /* ��У�� */
    ATK_MW579_UART_PARI_EVEN,               /* żУ�� */
} atk_mw579_uart_pari_t;

/* ����ֹͣλö�� */
typedef enum
{
    ATK_MW579_UART_STOP_1 = 0x00,           /* 1λֹͣλ */
    ATK_MW579_UART_STOP_2,                  /* 2λֹͣλ */
} atk_mw579_uart_stop_t;

/* ��·ƥ������ö�� */
typedef enum
{
    ATK_MW579_LINKPASSEN_ON = 0x00,         /* ʹ����·ƥ�� */
    ATK_MW579_LINKPASSEN_OFF,               /* �ر���·ƥ�� */
} atk_mw579_linkpassen_t;

/* ����LEDö�� */
typedef enum
{
    ATK_MW579_LEDEN_ON = 0x00,              /* ʹ�ܰ���LED */
    ATK_MW579_LEDEN_OFF,                    /* �رհ���LED */
} atk_mw579_leden_t;

/* ���豸����˯��ö�� */
typedef enum
{
    ATK_MW579_SLAVESLEEPEN_ON = 0x00,       /* ʹ�ܴ��豸����˯�� */
    ATK_MW579_SLAVESLEEPEN_OFF,             /* �رմ��豸����˯�� */
} atk_mw579_slavesleepen_t;

/* ͨ��������ö�� */
typedef enum
{
    ATK_MW579_MAXPUT_ON = 0x00,             /* ʹ��ͨ�������� */
    ATK_MW579_MAXPUT_OFF,                   /* �ر�ͨ�������� */
} atk_mw579_maxput_t;

/* ����ģʽö�� */
typedef enum
{
    ATK_MW579_MODE_M = 0x00,                /* ���豸ģʽ */
    ATK_MW579_MODE_S,                       /* ���豸ģʽ */
    ATK_MW579_MODE_O,                       /* �۲���ģʽ */
    ATK_MW579_MODE_B,                       /* �㲥��ģʽ */
    ATK_MW579_MODE_I,                       /* iBeaconģʽ */
} atk_mw579_mode_t;

/* �Զ�˯��ö�� */
typedef enum
{
    ATK_MW579_AUTOSLEEPEN_ON = 0x00,        /* ʹ���Զ�˯�� */
    ATK_MW579_AUTOSLEEPEN_OFF,              /* �ر��Զ�˯�� */
} atk_mw579_autosleepen_t;

/* �Զ�˯��ģʽö�� */
typedef enum
{
    ATK_MW579_AUTOSLEEP_LOW = 0x00,         /* ǳ˯�� */
    ATK_MW579_AUTOSLEEP_DEEP,               /* ���˯�� */
} atk_mw579_autosleep_t;

/* �ϵ�˯��ö�� */
typedef enum
{
    ATK_MW579_POWERSLEEPEN_ON = 0x00,       /* ʹ���ϵ�˯�� */
    ATK_MW579_POWERSLEEPEN_OFF,             /* �ر��ϵ�˯�� */
} atk_mw579_powersleepen_t;

/* ������� */
#define ATK_MW579_EOK      0                /* û�д��� */
#define ATK_MW579_ERROR    1                /* ���� */
#define ATK_MW579_ETIMEOUT 2                /* ��ʱ���� */
#define ATK_MW579_EINVAL   3                /* �������� */

/* �������� */
uint8_t atk_mw579_init(atk_mw579_uart_baudrate_t baudrate);                                                                                         /* ATK-MW579��ʼ�� */
atk_mw579_conn_sta_t atk_mw579_get_conn_sta(void);                                                                                                  /* ��ȡATK-MW579����״̬ */
uint8_t atk_mw579_send_at_cmd(char *cmd, char *ack, uint32_t timeout);                                                                              /* ATK-MW579����ATָ�� */
uint8_t atk_mw579_enter_config_mode(void);                                                                                                          /* ATK-MW579��������ģʽ */
uint8_t atk_mw579_at_test(void);                                                                                                                    /* ATK-MW579 ATָ����� */
uint8_t atk_mw579_set_name(char *name);                                                                                                             /* ����ATK-MW579�������� */
uint8_t atk_mw579_set_hello(char *hello);                                                                                                           /* ����ATK-MW579������ӭ�� */
uint8_t atk_mw579_set_tpl(atk_mw579_tpl_t tpl);                                                                                                     /* ����ATK-MW579���书�� */
uint8_t atk_mw579_set_uart(atk_mw579_uart_baudrate_t baudrate, atk_mw579_uart_data_t data, atk_mw579_uart_pari_t pari, atk_mw579_uart_stop_t stop); /* ����ATK-MW579���ڲ��� */
uint8_t atk_mw579_set_adptim(uint16_t adptim);                                                                                                      /* ����ATK-MW579�㲥�ٶ� */
uint8_t atk_mw579_set_linkpassen(atk_mw579_linkpassen_t linkpassen);                                                                                /* ����ATK-MW579��·ƥ�� */
uint8_t atk_mw579_set_leden(atk_mw579_leden_t leden);                                                                                               /* ����ATK-MW579����LED */
uint8_t atk_mw579_set_slavesleepen(atk_mw579_slavesleepen_t slavesleepen);                                                                          /* ����ATK-MW579���豸����˯�� */
uint8_t atk_mw579_set_maxput(atk_mw579_maxput_t maxput);                                                                                            /* ����ATK-MW579ͨ�������� */
uint8_t atk_mw579_set_mode(atk_mw579_mode_t mode);                                                                                                  /* ����ATK-MW579����ģʽ */
uint8_t atk_mw579_scan_slave(void);                                                                                                                 /* ATK-MW579ɨ����Χ���豸 */
uint8_t atk_mw579_connadd_slave(char id[13]);                                                                                                           /* ATK-MW579������Χ���豸 */
uint8_t atk_mw579_enter_unvarnished(void);                                                                                                          /* ATK-MW579����͸�� */
uint8_t atk_mw579_set_linkpass(char *pass);                                                                                                         /* ����ATK-MW579��·ƥ������ */
uint8_t atk_mw579_set_autosleepen(atk_mw579_autosleepen_t autosleepen, uint8_t autotim);                                                            /* ����ATK-MW579�Զ�˯�� */
uint8_t atk_mw579_set_autosleep(atk_mw579_autosleep_t autosleep);                                                                                   /* ����ATK-MW579�Զ�˯��ģʽ */
uint8_t atk_mw579_set_powersleepen(atk_mw579_powersleepen_t powersleepen);                                                                          /* ����ATK-MW579�ϵ�˯�� */
uint8_t atk_mw579_set_ibeacon(char *uuid, uint16_t major, uint16_t minor, uint8_t rssi);                                                            /* ����ATK-MW579 iBeacon */
void atk_mw579_wakeup_by_pin(void);                                                                                                                 /* ͨ��WKUP���Ż���ATK-MW579 */
void atk_mw579_wakeup_by_uart(void);                                                                                                                /* ͨ��UART����ATK-MW579 */

#endif
