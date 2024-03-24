/**
 ****************************************************************************************************
 * @file        demo.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MW579模块iBeacon模式测试实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 MiniSTM32 V4开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "demo.h"
#include "./BSP/ATK_MW579/atk_mw579.h"
#include "./BSP/LED/led.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"

#define DEMO_BLE_NAME           "ATK-MW579"                         /* 蓝牙名称 */
#define DEMO_BLE_HELLO          "HELLO ATK-MW579"                   /* 开机欢迎语 */
#define DEMO_BLE_ADPTIM         2                                   /* 广播速度 */
#define DEMO_BLE_IBEACON_UUID   "00000000000000000000000000000000"  /* 标识符 */
#define DEMO_BLE_IBEACON_MAJOR  20                                  /* 主标识 */
#define DEMO_BLE_IBEACON_MINOR  20                                  /* 次标识 */
#define DEMO_BLE_IBEACON_RSSI   200                                 /* 距离一米时的信号强度 */

/**
 * @brief       例程演示入口函数
 * @param       无
 * @retval      无
 */
void demo_run(void)
{
    uint8_t ret;
    
    /* ATK-MW579初始化 */
    ret = atk_mw579_init(ATK_MW579_UART_BAUDRATE_115200);
    if (ret != 0)
    {
        printf("ATK-MW579 init failed!\r\n");
        while (1)
        {
            LED0_TOGGLE();
            delay_ms(200);
        }
    }
    
    /* 配置ATK-MW579 */
    atk_mw579_enter_config_mode();
    ret  = atk_mw579_set_name(DEMO_BLE_NAME);
    ret += atk_mw579_set_hello(DEMO_BLE_HELLO);
    ret += atk_mw579_set_tpl(ATK_MW579_TPL_P0DBM);
    ret += atk_mw579_set_uart(ATK_MW579_UART_BAUDRATE_115200, ATK_MW579_UART_DATA_8, ATK_MW579_UART_PARI_NONE, ATK_MW579_UART_STOP_1);
    ret += atk_mw579_set_adptim(DEMO_BLE_ADPTIM);
    ret += atk_mw579_set_linkpassen(ATK_MW579_LINKPASSEN_OFF);
    ret += atk_mw579_set_leden(ATK_MW579_LEDEN_ON);
    ret += atk_mw579_set_slavesleepen(ATK_MW579_SLAVESLEEPEN_OFF);
    ret += atk_mw579_set_autosleep(ATK_MW579_AUTOSLEEP_LOW);
    ret += atk_mw579_set_powersleepen(ATK_MW579_POWERSLEEPEN_OFF);
    ret += atk_mw579_set_maxput(ATK_MW579_MAXPUT_OFF);
    ret += atk_mw579_set_ibeacon(DEMO_BLE_IBEACON_UUID, DEMO_BLE_IBEACON_MAJOR, DEMO_BLE_IBEACON_MINOR, DEMO_BLE_IBEACON_RSSI);
    ret += atk_mw579_set_mode(ATK_MW579_MODE_I);
    if (ret != 0)
    {
        printf("ATK-MW579 config failed!\r\n");
        while (1)
        {
            LED0_TOGGLE();
            delay_ms(200);
        }
    }
    
    while (1);
}
