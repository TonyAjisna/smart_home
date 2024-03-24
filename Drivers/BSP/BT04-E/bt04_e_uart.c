#include "./BSP/BT04-E/bt04_e_uart.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

static UART_HandleTypeDef g_uart_handle;                /* UART句柄 */
static struct 
{
    uint8_t buf[BT04_E_UART_RX_BUF_SIZE];               /* 帧接收缓冲 */
    struct 
    {
        uint16_t len    : 15;                           /* 帧接收长度，sta[14:0]  ：表示机构内位域的定义（即该变量占几个bit空间） */
        uint16_t finsh  : 1;                            /* 帧接收完成标志，sta[15] */
    } sta;
} g_uart_rx_frame = {0};                                /* UART接收帧缓冲信息结构体 */
static uint8_t g_uart_tx_buf[BT04_E_UART_TX_BUF_SIZE];  /* UART发送缓冲 */


/* 待打印数据 */
void bt04_e_uart_printf(char *fmt, ...)
{
    va_list ap;
    uint16_t len;

    va_start(ap, fmt);
    vsprintf((char *)g_uart_tx_buf, fmt, ap);
    va_end(ap);

    len = strlen((const char *)g_uart_tx_buf);
    HAL_UART_Transmit(&g_uart_handle, g_uart_tx_buf, len, HAL_MAX_DELAY);
}


/* UART重新开始接收数据 */
void bt04_e_uart_rx_restart(void){
    g_uart_rx_frame.sta.len = 0;
    g_uart_rx_frame.sta.finsh = 0;
}


/* 获得UART接收到的一帧数据 */
uint8_t *bt04_e_uart_rx_get_frame(void)
{
    if (g_uart_rx_frame.sta.finsh == 1)
    {
        g_uart_rx_frame.buf[g_uart_rx_frame.sta.len] = '\0';
        return g_uart_rx_frame.buf;
    }
    else
    {
        return NULL;
    }
}


/* 获得UART接收到的一帧数据的长度 */
uint16_t bt04_e_uart_rx_get_frame_len(void)
{
    if (g_uart_rx_frame.sta.finsh == 1)
    {
        return g_uart_rx_frame.sta.len;
    }
    else
    {
        return 0;
    }
}


/* UART初始化 */
void bt04_e_uart_init(uint32_t baudrate)
{
    g_uart_handle.Instance = BT04_E_UART_INTERFACE;     /* UART */
    g_uart_handle.Init.BaudRate     = baudrate;                     /* 波特率 */
    g_uart_handle.Init.WordLength   = UART_WORDLENGTH_8B;           /* 数据位 */
    g_uart_handle.Init.StopBits     = UART_STOPBITS_1;              /* 停止位 */
    g_uart_handle.Init.Parity       = UART_PARITY_NONE;             /* 校验位 */
    g_uart_handle.Init.Mode         = UART_MODE_TX_RX;              /* 收发模式 */
    g_uart_handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;          /* 无硬件流控 */
    g_uart_handle.Init.OverSampling = UART_OVERSAMPLING_16;         /* 过采样 */
    HAL_UART_Init(&g_uart_handle);                                  /* 使能ATK-MW579 UART
                                                                     * HAL_UART_Init()会调用函数HAL_UART_MspInit()
                                                                     * 该函数定义在文件usart.c中
                                                                     */
}


/* UAR中断回调函数 */
void BT04_E_UART_IRQHandler(void)
{
        uint8_t tmp;
    
    if (__HAL_UART_GET_FLAG(&g_uart_handle, UART_FLAG_ORE) != RESET)        /* UART接收过载错误中断 */
    {
        __HAL_UART_CLEAR_OREFLAG(&g_uart_handle);                           /* 清除接收过载错误中断标志 */
        (void)g_uart_handle.Instance->SR;                                   /* 先读SR寄存器，再读DR寄存器 */
        (void)g_uart_handle.Instance->DR;
    }
    
    if (__HAL_UART_GET_FLAG(&g_uart_handle, UART_FLAG_RXNE) != RESET)       /* UART接收中断 */
    {
        HAL_UART_Receive(&g_uart_handle, &tmp, 1, HAL_MAX_DELAY);           /* UART接收数据 */
        
        if (g_uart_rx_frame.sta.len < (ATK_MW579_UART_RX_BUF_SIZE - 1))     /* 判断UART接收缓冲是否溢出
                                                                             * 留出一位给结束符'\0'
                                                                             */
        {
            g_uart_rx_frame.buf[g_uart_rx_frame.sta.len] = tmp;             /* 将接收到的数据写入缓冲 */
            g_uart_rx_frame.sta.len++;                                      /* 更新接收到的数据长度 */
        }
        else                                                                /* UART接收缓冲溢出 */
        {
            g_uart_rx_frame.sta.len = 0;                                    /* 覆盖之前收到的数据 */
            g_uart_rx_frame.buf[g_uart_rx_frame.sta.len] = tmp;             /* 将接收到的数据写入缓冲 */
            g_uart_rx_frame.sta.len++;                                      /* 更新接收到的数据长度 */
        }
    }
    
    if (__HAL_UART_GET_FLAG(&g_uart_handle, UART_FLAG_IDLE) != RESET)       /* UART总线空闲中断 */
    {
        g_uart_rx_frame.sta.finsh = 1;                                      /* 标记帧接收完成 */
        
        __HAL_UART_CLEAR_IDLEFLAG(&g_uart_handle);                          /* 清除UART总线空闲中断 */
    }
}
