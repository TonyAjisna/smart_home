#include "i2c.h"

I2C_HandleTypeDef  hi2c1;


/* 
 1、先使能IIC的SCL和SDA对应的GPIO时钟 
 2、设置对应DPIO工作模式（开漏输出）
 3、参考IIC总线协议，编写信号函数（起始信号、停止信号、应答信号）
 4、编写IIC的读写函数
*/
void MX_I2C1_Init(void)
{
    // hi2c1.Instance = I2C1;
}