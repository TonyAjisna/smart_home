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