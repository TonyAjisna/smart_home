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

* 抢占优先级--子优先级，这里一般使用NVIC_PriorityGroup_4模式，即优先级配置寄存器的高4位全部用于抢占优先级，不使用子优先级。这样只需要设置抢占式优先级（子优先级不支持中断嵌套）。

* 三个系统中断优先级配置寄存器

  * SHPR1：SHPR1寄存器的地址为0xE000ED18, 用于配置 MemManage、BusFault、UsageFault 的中断优先级;
  * SHPR2：用于配置SVCall的中断优先级；
  * SHPR3：PendSV（可悬挂异常，可以延期执行对应的异常处理服务函数，只有在当前没有更高优先级的异常或者中断需要执行时，才能执行对应的异常服务函数）、SysTick的中断优先级；

* 三个中断屏蔽寄存器

  * PRIMASK：用于屏蔽除NMI（SRAM奇偶校验错误）和HardFault（硬件故障）外的所有异常和中断；

  * FAULTMASK：屏蔽除NMI外的所有异常和中断；

  * BASEPRI：

  * BASEPRI 用于设置一个中断屏蔽的阈值，设置好 BASEPRI 后，中断优先级低于 BASEPRI 的中断就都会被屏蔽掉，FreeRTOS 就是使用 BASEPRI 寄存器来管理受 FreeRTOS管理的中断的，而不受 FreeRTOS 管理的中断，则不受 FreeRTOS 的影响。

* 进出临界区

  ```c
  /* 进入临界区 */
  #define taskENTER_CRITICAL() 				portENTER_CRITICAL()
  #define portENTER_CRITICAL() 				vPortEnterCritical()
  /* 中断中进入临界区 */
  #define taskENTER_CRITICAL_FROM_ISR() 		portSET_INTERRUPT_MASK_FROM_ISR()
  #define portSET_INTERRUPT_MASK_FROM_ISR() 	ulPortRaiseBASEPRI()
  /* 退出临界区 */
  #define taskEXIT_CRITICAL() 				portEXIT_CRITICAL()
  #define portEXIT_CRITICAL() 				vPortExitCritical()
  /* 中断中退出临界区 */
  #define taskEXIT_CRITICAL_FROM_ISR(x) 		portCLEAR_INTERRUPT_MASK_FROM_ISR(x)
  #define portCLEAR_INTERRUPT_MASK_FROM_ISR(x) vPortSetBASEPRI(x)
  
  //详情解释请见FreeRTOS开发指南71页
  ```

##### 任务管理

###### 单任务和多任务系统

* 单任务一般就是main()循环中使用一个大循环，这个大循环的部分可以视为应用程序的后台，而应用程序的前台则是各种中断的中断服务函数。因此单任务系统也叫前后台系统。
* 多任务系统可以“同时”运行。

###### 任务状态

* 运行态、就绪态、阻塞态和挂起态；

###### 任务优先级

* ![image-20240304211626817](C:\Users\Zhaobw\AppData\Roaming\Typora\typora-user-images\image-20240304211626817.png)

###### 任务调度方式

* 抢占式调度、时间片调度和协程式调度。
* 抢占式调度：优先级高的任务抢占优先级低的任务，只有当优先级高的任务发生阻塞或者被挂起，低优先级的任务才可以运行。

###### 时间片调度

* 时间片调度主要是针对优先级相同的任务，当多个任务的优先级相同时，任务调度器会在每一次系统时钟节拍到的时候切换任务，也就是说CPU轮流运行优先级相同的任务，每个任务运行的时间就是一个系统时钟节拍。

###### 任务控制块

* 每个已创建的任务都有一个包含任务控制块。

  ```c
  typedef struct tskTaskControlBlock * TaskHandle_t; //具体代码省略
  ```

###### 任务栈

* 不论是裸机编程还是RTOS编程，栈空间的使用非常重要。函数中的局部变量、函数调用时的现场保护和函数的返回地址等都是存放在栈空间中的。

* 使用静态方式创建任务时，需要用户自行分配一块内存，作为任务的栈空间。

  ```c
  TaskHandle_t xTaskCreateStatic( TaskFunction_t pxTaskCode,
                                  const char * const pcName,
                                  const uint32_t ulStackDepth,	//任务栈大小
                                  void * const pvParameters,
                                  UBaseType_t uxPriority,
                                  StackType_t * const puxStackBuffer,
                                  StaticTask_t * const pxTaskBuffer)
  ```

* 使用动态方式创建任务时，系统会自动从系统堆中分配一块内存，作为任务的栈空间。

  ```c
  BaseType_t xTaskCreate( TaskFunction_t pxTaskCode,
                          const char * const pcName,
                          const configSTACK_DEPTH_TYPE usStackDepth,	
                          void * const pvParameters,
                          UBaseType_t uxPriority,
                          TaskHandle_t * const pxCreatedTask)
  ```

###### FreeRTOS任务相关API函数

* 任务创建和删除

* 任务挂起和恢复



#### 基于MQTT协议连接阿里云服务器

##### MQTT简介

* MQTT（Message Queuing Telemetry Transport，消息队列遥测传输协议），是一种基于发布/订阅模式的轻量级通讯协议，构建在TCP/IP协议上。
* 实现MQTT协议需要：客户端、服务器端。MQTT协议有三种身份：发布者Publish、代理Broker（服务器）、订阅者（Subscribe）。其中消息的发布者和订阅者都是客户端，消息代理是服务器，消息发布者可以同时是订阅者。![image-20240318221734058](C:\Users\Zhaobw\AppData\Roaming\Typora\typora-user-images\image-20240318221734058.png)
* MQTT传输的消息分别是：主题Topic、消息的内容payload两部分。
  * Topic：消息的类型，订阅者（Subscribe）订阅后，就会收到该主题的消息内容（payload）。
  * Payload：可以理解为消息的内容，是指订阅者具体要使用的内容。

##### MQTT协议实现原理

* 1、要在客户端与代理服务端建立一个TCP连接，建立连接的过程是由客户端主动发起的，代理服务一直是处于指定端口的监听状态，当监听到有客户端要接入的时候，就会立即去处理。客户端在发起连接请求时，携带客户端ID、账号、密码（无账号密码使用除外）、心跳间隔时间等数据。代理服务收到后检查自己的连接权限配置中是否允许该账号密码连接，如果允许则建立会话标识并保存，绑定客户端ID与会话，并记录心跳间隔时间（判断是否掉线和启动遗嘱时用）和遗嘱消息等，然后回发连接成功确认消息给客户端，客户端收到连接成功的确认消息后，进入下一步（通常是开始订阅主题，如果不需要订阅则跳过）。![image-20240318222745792](C:\Users\Zhaobw\AppData\Roaming\Typora\typora-user-images\image-20240318222745792.png)
* 2、客户端将需要订阅的主题经过SUBSCRIBE报文发送给代理服务，代理服务则将这个主题记录到该客户端ID下（以后有这个主题发布就会发送给该客户端），然后回复确认消息SUBACK报文，客户端街道SUBACK报文后知道已经订阅发布成功，则处于监听代理服务推送的消息，也可以继续订阅其他主题或发布主题。![image-20240318223042414](C:\Users\Zhaobw\AppData\Roaming\Typora\typora-user-images\image-20240318223042414.png)
* 3、当某一客户端发布一个主题到代理服务后，代理服务先回复该客户端收到主题的确认消息，该客户端收到确认后就可以继续自己的逻辑了。但这是主题消息还没有发给订阅了这个主题的客户端，代理需要根据质量级别QoS来决定怎样处理这个主题。所以这里充分体现了是MQTT协议是异步通信模式，不是立刻端到端反应的。![image-20240318223311011](C:\Users\Zhaobw\AppData\Roaming\Typora\typora-user-images\image-20240318223311011.png)

##### 移植MQTT协议

* 配置MCU为TCP客户端模式。
* DNS解析阿里云网页转成IP地址：调用函数 gethostbyname 获取 DNS 解析的 IP 地址。
* MQTT连接：调用函数 mqtt_client_connect 连接服务器。
* 连接状态：对服务器发布和订阅操作。
* 循环发布数据到服务器中：定时1s调用mqtt_publish发布数据至服务器。
