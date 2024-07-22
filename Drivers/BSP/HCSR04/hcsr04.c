#include "hcsr04.h"

uint64_t time = 0;      //声明变量，用来计时
uint64_t time_end = 0;  //声明变量，存储回波信号时间 


/* ECHO_PIN和TRIG_PIN设置 */
void HC_SR04_Init(void)
{
    
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