### smart_home项目记录

#### 基础工程搭建

* `Middlewares/FreeRTOS_CORE`用于存放FreeRTOS的内核C源码文件
* `Middlewares/FreeRTOS_PORT`用于存放FreeRTOS内核的移植文件

##### 添加FreeRTOSConfig.h文件

* FreeRTOSConfig.h 是 FreeRTOS 操作系统的配置文件，FreeRTOS操作系统是可裁剪的，以此节约内存资源；

##### 修改SYSTEM文件

* sys.h	usart.h	delay.h

##### 修改中断相关文件

* 在FreeRTOS的移植过程中会有三个重要的中断，分别是FreeRTOS系统时基定时器的中断（SysTick中断）、SVC中断、PendSV中断。这三个中断的中断服务函数在HAL库文件中都有定义。但在delay.c文件和FreeRTOS文件中已经定义，此时需要把HAL库种这三个中断服务函数注释掉。

#### 添加外设以及功能任务

##### FreeRTOS中断管理

* 中断优先级管理

  ```c
  typedef struct
  {
    __IOM uint32_t ISER[8U];               /*中断使能寄存器 */
          uint32_t RESERVED0[24U];
    __IOM uint32_t ICER[8U];               /*中断除能寄存器 */
          uint32_t RSERVED1[24U];
    __IOM uint32_t ISPR[8U];               /*中断使能挂起寄存器 */
          uint32_t RESERVED2[24U];
    __IOM uint32_t ICPR[8U];               /*中断除能挂起寄存器 */
          uint32_t RESERVED3[24U];
    __IOM uint32_t IABR[8U];               /*中断有效位寄存器 */
          uint32_t RESERVED4[56U];
    __IOM uint8_t  IP[240U];               /*中断优先级寄存器 */
          uint32_t RESERVED5[644U];
    __OM  uint32_t STIR;                   /*软件触发中断寄存器 */
  }  NVIC_Type;
  ```

* 抢占优先级--子优先级，这里一般使用NVIC_PriorityGroup_4模式，即优先级配置寄存器的高4位全部用于抢占优先级，不使用子优先级。这样只需要设置抢占式优先级。

##### 任务管理

* 本项目使用时间片调度