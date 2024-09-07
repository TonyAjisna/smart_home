#include "hcsr04.h"
#include "./SYSTEM/delay/delay.h"

uint64_t time = 0;      //声明变量，用来计时
uint64_t time_end = 0;  //声明变量，存储回波信号时间 


/* ECHO_PIN和TRIG_PIN设置 */
void HC_SR04_Init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    HY_SRF05_CLK_ENABLE();

    gpio_init_struct.Pin = TRIG_PIN;    //TRIG
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;		//设置GPIO口速度50Mhz
    HAL_GPIO_Init(TRIG_PORT, &gpio_init_struct);
    delay_us(15);       //延时15us

    gpio_init_struct.Pin = ECHO_PIN;  //ECHO
    gpio_init_struct.Mode = GPIO_MODE_INPUT;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;		//设置GPIO口速度50Mhz
    gpio_init_struct.Pull = GPIO_PULLDOWN;		//下拉输入模式
    HAL_GPIO_Init(ECHO_PORT, &gpio_init_struct);
    HAL_GPIO_WritePin(GPIOA, ECHO_PIN, GPIO_PIN_RESET); //输出低电平
}


/* 测距并返回单位为mm的距离结果 */
uint16_t sonar_mm(void)
{
    uint32_t Distance, Distance_mm = 0;
    TRIG_OUT(1);                            //输出高电平
    delay_us(15);                           //延时15μs
    TRIG_OUT(0);                            //输出低电平
    while (ECHO_IN == 0);                   //等待低电平结束
    time=0;                                 //计时清零
    while (ECHO_IN == 1);                   //等待高电平
    time_end =  time;                       //记录结束时的时间
	if(time_end/100<38)						//判断是否小于38毫秒，大于38毫秒的就是超时，直接调到下面返回0
	{
		Distance=(time_end*346)/2;			//计算距离，25°C空气中的音速为346m/s
		Distance_mm=Distance/100;			//因为上面的time_end的单位是10微秒，所以要得出单位为毫米的距离结果，还得除以100
	}
	return Distance_mm;						//返回测距结果
}


/* 测距并返回单位为m的距离结果 */
float sonar(void)
{
    uint32_t Distance, Distance_mm = 0;
    float Distance_m=0;
    TRIG_OUT(1);                            //输出高电平
    delay_us(15);                           //延时15μs
    TRIG_OUT(0);                            //输出低电平
    while (ECHO_IN == 0);                   //等待低电平结束
    time=0;                                 //计时清零
    while (ECHO_IN == 1);                   //等待高电平
    time_end =  time;                       //记录结束时的时间
	if(time_end/100<38)						//判断是否小于38毫秒，大于38毫秒的就是超时，直接调到下面返回0
	if(time_end/100<38)
	{
		Distance=(time_end*346)/2;
		Distance_mm=Distance/100;
		Distance_m=Distance_mm/1000;
	}
	return Distance_m;						//返回测距结果
}


/* 这里需要添加一个计时中断，更新time的值 */
void TIM_IRQHandler(void)			//更新中断函数，用来计时，每10微秒变量time加1
{
		time++;
}