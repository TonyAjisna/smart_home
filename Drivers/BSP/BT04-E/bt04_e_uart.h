#ifndef __BTO4_E_UART_H
#define __BTO4_E_UART_H

#include "./SYSTEM/sys/sys.h"

/* 引脚定义 */
#define BT04_E_UART_TX_GPIO_PORT            GPIOC
#define BT04_E_UART_TX_GPIO_PIN             GPIO_PIN_12
/* 这里对于时钟开启，需要先确认该GPIO挂载在哪个时钟组下 */
#define BT04_E_UART_TX_GPIO_CLK_ENABLE()    do{   __HAL_RCC_GPIOC_CLK_ENABLE(); } while (0) 

#define BT04_E_UART_RX_GPIO_PORT            GPIOD
#define BT04_E_UART_RX_GPIO_PIN             GPIO_PIN_2
#define BT04_E_UART_RX_GPIO_CLK_ENABLE()    do{   __HAL_RCC_GPIOD_CLK_ENABLE(); } while (0) 

#define BT04_E_UART_INTERFACE               UART5
#define BT04_E_UART_IRQn                    UART5_IRQn
#define BT04_E_UART_IRQHandler              UART5_IRQHandler
#define BT04_E_UART_CLK_ENABLE()            do{   __HAL_RCC_UART5_CLK_ENABLE(); } while (0) 

/* UART收发缓冲大小 */
#define BT04_E_UART_RX_BUF_SIZE             2048
#define BT04_E_UART_TX_BUF_SIZE             64

/* 操作函数 */
void bt04_e_uart_printf(char *fmt, ...);
void bt04_e_uart_rx_restart(void);                  /* UART重新开始接收数据 */
uint8_t *bt04_e_uart_rx_get_frame(void);            /* 获取UART接收到的一帧数据 */
uint16_t bt04_e_uart_rx_get_frame_len(void);        /* 获取接收到的一帧的长度 */
void bt04_e_uart_init(uint32_t baudrate);           /* UART初始化 */ 

#endif
