#include "play.h"
#include "playConfig.h"
#include "./BSP/ATK_MW579/atk_mw579.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"

#define BLE_NAME            "ATK-MW579-MASTER"        /* 蓝牙名称 */
#define BLE_HELLO           "HELLO ATK-MW579-MASTER"  /* 蓝牙名称 */
#define BLE_ADPTIM           5                        /* 广播速度 */


uint8_t *recv_dat = NULL;
void atk_mw579_play(void)
{
    uint8_t ret;
    uint8_t key;
    uint8_t slave_id;

    /* atk_mw579初始化 */
    ret = atk_mw579_init(ATK_MW579_UART_BAUDRATE_115200);
    if (ret != 0)
    {
        printf("ATK-MW579-MASTER init failed!\r\n");
        // return;
    }
    else
    {
        printf("ATK-MW579-MASTER init success!\r\n");
    }

    /* 配置ATK-MW579-MASTER */
    atk_mw579_enter_config_mode();
    ret  = atk_mw579_set_name(BLE_NAME);
    ret += atk_mw579_set_hello(BLE_HELLO);
    ret += atk_mw579_set_tpl(ATK_MW579_TPL_P0DBM);
    ret += atk_mw579_set_uart(ATK_MW579_UART_BAUDRATE_115200, ATK_MW579_UART_DATA_8, ATK_MW579_UART_PARI_NONE, ATK_MW579_UART_STOP_1);
    ret += atk_mw579_set_adptim(BLE_ADPTIM);
    ret += atk_mw579_set_linkpassen(ATK_MW579_LINKPASSEN_OFF);
    ret += atk_mw579_set_leden(ATK_MW579_LEDEN_ON);
    ret += atk_mw579_set_slavesleepen(ATK_MW579_SLAVESLEEPEN_OFF);
    ret += atk_mw579_set_maxput(ATK_MW579_MAXPUT_OFF);
    ret += atk_mw579_set_mode(ATK_MW579_MODE_M);
    if (ret != 0)
    {
        printf("ATK-MW579-MASTER config failed!\r\n");
    }
    else
    {
        printf("ATK-MW579-MASTER config success!\r\n");
    }

    /* 重新进入配置模式 */
    atk_mw579_enter_config_mode();

    /* 扫描周围从设备 */
    if (atk_mw579_scan_slave() == 0)
    {
        printf("ATK-MW579-MASTER scan success!\r\n");
    }
    else
    {
        printf("ATK-MW579-MASTER scan failed!\r\n");
    }
    
    delay_ms(1000);

    /* 将扫描到的设备发送至串口调试器 */
    recv_dat = atk_mw579_uart_rx_get_frame();
    if (recv_dat != NULL)
    {
        printf("%s", recv_dat);
        printf("Enter number to connect: \r\n");
    }

    /* 根据从设备MAC地址连接从设备 */
    printf("Connect to number %s\r\n", (char *)ATK_MW579_SLAVE_MAC);
    atk_mw579_connadd_slave("383B263631E5");
    // uint8_t isConn = atk_mw579_connadd_slave(ATK_MW579_SLAVE_MAC);
    // if (isConn == ATK_MW579_ERROR)
    // {
    //     printf("ATK-MW579-SLAVE connect failed!\r\n");
    //     return;
    // }
    // else
    // {
    //     printf("ATK-MW579-SLAVE connect success!\r\n");
    // }


    /* 等待从设备连接后，进入透传 */
    while (atk_mw579_get_conn_sta() == ATK_MW579_CONNECTED);
    atk_mw579_enter_unvarnished();
}

void atk_mw579_transmission(void)
{
    /* 判断是否连接至从设备 */
    if (atk_mw579_get_conn_sta() == ATK_MW579_CONNECTED)
    {
        /* 透传发送数据至从设备 */
        atk_mw579_uart_printf("From ATK-MW579\r\n");
        
        /* 透传接收来自从设备的数据 */
        recv_dat = atk_mw579_uart_rx_get_frame();
        if (recv_dat != NULL)
        {
            printf("%s", recv_dat);
            atk_mw579_uart_rx_restart();
        }
        delay_ms(10);
    }
    else
    {
        printf("ATK-MW579-SLAVE connect failed!\r\n");
    }
}