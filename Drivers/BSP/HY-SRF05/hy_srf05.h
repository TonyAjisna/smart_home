// #ifndef _HY_SRF05_H_
// #define _HY_SRF05_H_

// #include "./SYSTEM/sys/sys.h"
// #include "./SYSTEM/delay/delay.h"

// /* 首先定义tx和rx引脚 */
// #define ECHO_PORT           GPIOA
// #define ECHO_PIN            GPIO_PIN_7

// #define TRIG_PORT           GPIOA
// #define TRIG_PIN            GPIO_PIN_6

// #define HY_SRF05_CLK_ENABLE()   do{ _HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)

// /* IO操作函数 */
// #define TRIG_OUT(x)     do{ x ? \
//                                 HAL_GPIO_WritePin(DHT11_DQ_GPIO_PORT, DHT11_DQ_GPIO_PIN, GPIO_PIN_SET) : \
//                                 HAL_GPIO_WritePin(DHT11_DQ_GPIO_PORT, DHT11_DQ_GPIO_PIN, GPIO_PIN_RESET); \
//                             }while(0)  
// #define ECHO_IN         HAL_GPIO_ReadPin(DHT11_DQ_GPIO_PORT, DHT11_DQ_GPIO_PIN)         /* 数据端口输入 */


 
// void hy_srf05_init(void);

// #endif

