#ifndef __BT04_E_H
#define __BT04_E_H

/* 蓝牙模块驱动 */
#include "stm32f10x.h"
#include "./BSP/BT04-E/bt04_e_uart.h"
#include <stdint.h>

/* 引脚定义 */
#define BT04_E_STA_GPIO_PORT GPIOC
#define BT04_E_STA_GPIO_PIN GPIO_PIN_4
#define BT04_E_STA_GPIO_CLK_ENABLE()  \
    do                                \
    {                                 \
        __HAL_RCC_GPIOC_CLK_ENABLE(); \
    } while (0)
#define BT04_E_WKUP_GPIO_PORT GPIOD
#define BT04_E_WKUP_GPIO_PIN GPIO_PIN_4
#define BT04_E_WKUP_GPIO_CLK_ENABLE() \
    do                                \
    {                                 \
        __HAL_RCC_GPIOD_CLK_ENABLE(); \
    } while (0)

/* IO操作 */
// #define BT04_E_READ_STA()                HAL_GPIO_ReadPin(BT04_E_STA_GPIO_PORT, BT04_E_STA_GPIO_PIN)
// #define BT04_E_WKUP(x)                   do{ x ?                                                                                     \
//                                                 HAL_GPIO_WritePin(BT04_E_WKUP_GPIO_PORT, BT04_E_WKUP_GPIO_PIN, GPIO_PIN_SET) :    \
//                                                 HAL_GPIO_WritePin(BT04_E_WKUP_GPIO_PORT, BT04_E_WKUP_GPIO_PIN, GPIO_PIN_RESET);   \
//                                             }while(0)

/* 连接状态枚举 */
typedef enum
{
    BT04_E_CONNECTED = 0x00, /* 已连接 */
    BT04_E_DISCONNECTED,     /* 未连接 */
} bt04_e_conn_sta_t;

/*串口数据位枚举 */
typedef enum
{
    BT04_E_UART_DATA_5 = 0x00, /* 5位数据位 */
    BT04_E_UART_DATA_6,        /* 6位数据位 */
    BT04_E_UART_DATA_7,        /* 7位数据位 */
    BT04_E_UART_DATA_8,        /* 8位数据位 */
} bt04_e_uart_data_t;

/* 发射功率枚举 */
typedef enum
{
    BT04_E_TPL_N20DBM = 0x00, /* -20dBm */
    BT04_E_TPL_N14DBM,        /* -14dBm */
    BT04_E_TPL_N8DBM,         /* -8dBm */
    BT04_E_TPL_N3DBM,         /* -3dBm */
    BT04_E_TPL_P0DBM,         /* +0dBm */
    BT04_E_TPL_P1DBM,         /* +1dBm */
    BT04_E_TPL_P2DBM,         /* +2dBm */
    BT04_E_TPL_P3DBM,         /* +3dBm */
} bt04_e_tpl_t;

/* 串口波特率 */
typedef enum
{
    BT04_E_UART_BAUDRATE_2400 = 0x00, /* 2400bps */
    BT04_E_UART_BAUDRATE_4800,        /* 4800bps */
    BT04_E_UART_BAUDRATE_9600,        /* 9600bps，默认 */
    BT04_E_UART_BAUDRATE_19200,       /* 19200bps */
    BT04_E_UART_BAUDRATE_38400,       /* 38400bps */
    BT04_E_UART_BAUDRATE_57600,       /* 5760bps */
    BT04_E_UART_BAUDRATE_115200,      /* 115200bps */
} bt04_e_uart_baudrate_t;

/* 串口校验位枚举 */
typedef enum
{
    BT04_E_UART_PARI_NONE = 0x00, /* 无校验 */
    BT04_E_UART_PARI_ODD,         /* 奇校验 */
    BT04_E_UART_PARI_EVEN,        /* 偶校验 */
} bt04_e_uart_pari_t;

/* 串口停止位枚举 */
typedef enum
{
    BT04_E_UART_STOP_1 = 0x00, /* 1位停止位 */
    BT04_E_UART_STOP_2,        /* 2位停止位 */
} bt04_e_uart_stop_t;

/* 链路匹配连接枚举 */
typedef enum
{
    BT04_E_LINKPASSEN_ON = 0x00, /* 使能链路匹配 */
    BT04_E_LINKPASSEN_OFF,       /* 关闭链路匹配 */
} bt04_e_linkpassen_t;

/* 从设备断连睡眠枚举 */
typedef enum
{
    BT04_E_SLAVESLEEPEN_ON = 0x00, /* 使能从设备断连睡眠 */
    BT04_E_SLAVESLEEPEN_OFF,       /* 关闭从设备断连睡眠 */
} bt04_e_slavesleepen_t;

/* 通信最大输出枚举 */
typedef enum
{
    BT04_E_MAXPUT_ON = 0x00, /* 使能通信最大输出 */
    BT04_E_MAXPUT_OFF,       /* 关闭通信最大输出 */
} bt04_e_maxput_t;

/* 工作模式枚举 */
typedef enum
{
    BT04_E_MODE_M = 0x00, /* 主设备模式 */
    BT04_E_MODE_S,        /* 从设备模式 */
    BT04_E_MODE_O,        /* 观察者模式 */
    BT04_E_MODE_B,        /* 广播者模式 */
    BT04_E_MODE_I,        /* iBeacon模式 */
} bt04_e_mode_t;

/* 自动睡眠枚举 */
typedef enum
{
    BT04_E_AUTOSLEEPEN_ON = 0x00, /* 使能自动睡眠 */
    BT04_E_AUTOSLEEPEN_OFF,       /* 关闭自动睡眠 */
} bt04_e_autosleepen_t;

/* 自动睡眠模式枚举 */
typedef enum
{
    BT04_E_AUTOSLEEP_LOW = 0x00, /* 浅睡眠 */
    BT04_E_AUTOSLEEP_DEEP,       /* 深度睡眠 */
} bt04_e_autosleep_t;

/* 上电睡眠枚举 */
typedef enum
{
    BT04_E_POWERSLEEPEN_ON = 0x00, /* 使能上电睡眠 */
    BT04_E_POWERSLEEPEN_OFF,       /* 关闭上电睡眠 */
} bt04_e_powersleepen_t;

/* 错误代码 */
#define BT04_E_EOK 0      /* 没有错误 */
#define BT04_E_ERROR 1    /* 错误 */
#define BT04_E_ETIMEOUT 2 /* 超时错误 */
#define BT04_E_EINVAL 3   /* 参数错误 */

/* 操作函数 */
uint8_t bt04_e_init(bt04_e_uart_baudrate_t baudrate);                                                                                /* BT04_E初始化 */
bt04_e_conn_sta_t bt04_e_get_conn_sta(void);                                                                                         /* 获取BT04_E连接状态 */
uint8_t bt04_e_send_at_cmd(char *cmd, char *ack, uint32_t timeout);                                                                  /* BT04_E发送AT指令 */
uint8_t bt04_e_enter_config_mode(void);                                                                                              /* BT04_E进入配置模式 */
uint8_t bt04_e_at_test(void);                                                                                                        /* BT04_E AT指令测试 */
uint8_t bt04_e_set_name(char *name);                                                                                                 /* 设置BT04_E蓝牙名称 */
uint8_t bt04_e_set_hello(char *hello);                                                                                               /* 设置BT04_E开机欢迎语 */
uint8_t bt04_e_set_tpl(bt04_e_tpl_t tpl);                                                                                            /* 设置BT04_E发射功率 */
uint8_t bt04_e_set_uart(bt04_e_uart_baudrate_t baudrate, bt04_e_uart_data_t data, bt04_e_uart_pari_t pari, bt04_e_uart_stop_t stop); /* 设置BT04_E串口参数 */
uint8_t bt04_e_set_adptim(uint16_t adptim);                                                                                          /* 设置BT04_E广播速度 */
uint8_t bt04_e_set_linkpassen(bt04_e_linkpassen_t linkpassen);                                                                       /* 设置BT04_E链路匹配 */
uint8_t bt04_e_set_slavesleepen(bt04_e_slavesleepen_t slavesleepen);                                                                 /* 设置BT04_E从设备断连睡眠 */
uint8_t bt04_e_set_maxput(bt04_e_maxput_t maxput);                                                                                   /* 设置BT04_E通信最大输出 */
uint8_t bt04_e_set_mode(bt04_e_mode_t mode);                                                                                         /* 设置BT04_E工作模式 */
uint8_t bt04_e_scan_slave(void);                                                                                                     /* BT04_E扫描周围从设备 */
uint8_t bt04_e_conn_slave(uint8_t id);                                                                                               /* BT04_E连接周围从设备 */
uint8_t bt04_e_enter_unvarnished(void);                                                                                              /* BT04_E进入透传 */
uint8_t bt04_e_set_linkpass(char *pass);                                                                                             /* 设置BT04_E链路匹配密码 */
uint8_t bt04_e_set_autosleepen(bt04_e_autosleepen_t autosleepen, uint8_t autotim);                                                   /* 设置BT04_E自动睡眠 */
uint8_t bt04_e_set_autosleep(bt04_e_autosleep_t autosleep);                                                                          /* 设置BT04_E自动睡眠模式 */
uint8_t bt04_e_set_powersleepen(bt04_e_powersleepen_t powersleepen);                                                                 /* 设置BT04_E上电睡眠 */
uint8_t bt04_e_set_ibeacon(char *uuid, uint16_t major, uint16_t minor, uint8_t rssi);                                                /* 设置BT04_E iBeacon */
void bt04_e_wakeup_by_pin(void);                                                                                                     /* 通过WKUP引脚唤醒BT04_E */
void bt04_e_wakeup_by_uart(void);

#endif
