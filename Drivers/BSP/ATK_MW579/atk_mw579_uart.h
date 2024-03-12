#ifndef __ATK_MW579_UART_H
#define __ATK_MW579_UART_H

#include "./SYSTEM/sys/sys.h"

/* ���Ŷ��� */
#define ATK_MW579_UART_TX_GPIO_PORT         GPIOB
#define ATK_MW579_UART_TX_GPIO_PIN          GPIO_PIN_10
#define ATK_MW579_UART_TX_GPIO_AF           GPIO_AF7_USART3
#define ATK_MW579_UART_TX_GPIO_CLK_ENABLE() do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)

#define ATK_MW579_UART_RX_GPIO_PORT         GPIOB
#define ATK_MW579_UART_RX_GPIO_PIN          GPIO_PIN_11
#define ATK_MW579_UART_RX_GPIO_AF           GPIO_AF7_USART3
#define ATK_MW579_UART_RX_GPIO_CLK_ENABLE() do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)

#define ATK_MW579_UART_INTERFACE            USART3
#define ATK_MW579_UART_IRQn                 USART3_IRQn
#define ATK_MW579_UART_IRQHandler           USART3_IRQHandler
#define ATK_MW579_UART_CLK_ENABLE()         do{ __HAL_RCC_USART3_CLK_ENABLE(); }while(0)

/* UART�շ������С */
#define ATK_MW579_UART_RX_BUF_SIZE          2048
#define ATK_MW579_UART_TX_BUF_SIZE          64

/* �������� */
void atk_mw579_uart_printf(char *fmt, ...);     /* ATK-MW579 UART printf */
void atk_mw579_uart_rx_restart(void);           /* ATK-MW579 UART���¿�ʼ�������� */
uint8_t *atk_mw579_uart_rx_get_frame(void);     /* ��ȡATK-MW579 UART���յ���һ֡���� */
uint16_t atk_mw579_uart_rx_get_frame_len(void); /* ��ȡATK-MW579 UART���յ���һ֡���ݵĳ��� */
void atk_mw579_uart_init(uint32_t baudrate);    /* ATK-MW579 UART��ʼ�� */

#endif
