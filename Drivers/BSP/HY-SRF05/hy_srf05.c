#include "hy_srf05.h"

/* 
    1、功能定义：利用单片机编程产生频率为40khz的方波，经过发射驱动电路放大，使超声波传感器发射端震荡，
                发射超声波。超声波经目标体（障碍物）反射回来，由传感器接收端接受，再经过接收电路放大、整形。
                当收到超声波的反射波时，接收电路输出端产生一个跳变。通过定时器计时，计算时间差，就可以计算出相应的距离。
*/

/* 超声波计数 */
uint16_t msHcCount = 0;

bool msHcCount_EN = false;

/* 初始化 */
void hy_srf05_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    HY_SRF05_CLK_ENABLE();                                    /* 时钟使能 */

    gpio_init_struct.Pin   = TRIG_PIN;                        /* TRIG引脚 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;            /* 50mhz */
    gpio_init_struct.Mode  = GPIO_MODE_OUTPUT_PP;             /* 推挽输出 */
    HAL_GPIO_Init(TRIG_PORT, &gpio_init_struct);              /* 初始化TRIG引脚 */

    gpio_init_struct.Pin  = ECHO_PIN;                         /* ECHO引脚 */
    gpio_init_struct.Mode = GPIO_MODE_INPUT;                  /* 浮空输入 */
    gpio_init_struct.Pull = GPIO_PULLUP;                      /* 上拉 */
    HAL_GPIO_Init(ECHO_PORT, &gpio_init_struct);              /* 初始化ECHO引脚 */
}

/* 获取定时器计数器值 */
uint32_t GetEchoTimer(void)
{
    uint32_t t = 0;
    t = msHcCount*1000; //将定时器计数转换为时间
}

/* 开启计数 */
void OpenMsCounter(void)
{
    msHcCount = 0;
    msHcCount_EN = true;
}

/* 关闭计数 */
void CloseMsCounter(void)
{
    msHcCount_EN = false;
}

/* 通过定时器推算距离 */
float Hysrf05Length(void)
{
    uint32_t t = 0;
    int i = 0;
    float lengthTemp = 0;
    float sum = 0;
    while(i!=5)
    {
        TRIG_OUT(1);    //TRIG口输出一个高电平
        delay_us(20);
        TRIG_OUT(0);
        while (!ECHO_IN) //等待ECHO口返回高电平信号，便打开计时
        {
            OpenMsCounter();
        }
        i++;
        while (ECHO_IN) //等待ECHO口返回高电平信号，便打开计时
        {
            CloseMsCounter();
        }
        t = GetEchoTimer();     //获取高电平持续时间
        lengthTemp = ((float)t/58.0);//lengthTemp = 58*t;
        sum = lengthTemp + sum ;
    }
    lengthTemp = sum/5.0;//测量五次取平均值
    return lengthTemp;
}

