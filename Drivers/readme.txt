    该文件夹下面存放驱动层代码，包括：
    1，BSP：正点原子提供的板级支持包驱动代码（原HARDWARE文件夹下的代码），如：LED、BEEP、KEY、EXTI、TIMER、WDG...等
    2，CMSISI：ARM提供的CMSIS代码（主要包括各种头文件和启动文件（.s文件），为了方便使用，减少占用空间，我们进行了精简）	      
    3，STM32F4xx_HAL_Driver：ST提供的F4 HAL库驱动代码
    4，SYSTEM：正点原子提供的系统级核心驱动代码，包括sys、delay和usart三个子文件夹，分别提供时钟配置，延时和串口驱动等关键代码，方便大家构建工程
