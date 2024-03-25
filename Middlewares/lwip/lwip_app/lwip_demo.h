#ifndef _LWIP_DEMO_H
#define _LWIP_DEMO_H
#include "./SYSTEM/sys/sys.h"


/* 用户需要根据设备信息完善以下宏定义中的三元组内容 */
#define PRODUCT_KEY         "k0wydQAol09"                       /* 阿里云颁发的产品唯一标识，11位长度的英文数字随机组合 */
#define DEVICE_NAME         "ali_server_test"                      /* 用户注册设备时生成的设备唯一编号，支持系统自动生成，也可支持用户添加自定义编号，产品维度内唯一  */
#define DEVICE_SECRET       "ce874fe488d9f4c05565b77d48a1ed49"  /* 设备密钥，与DeviceName成对出现，可用于一机一密的认证方案  */

/* 宏定义一般不需要修改，可查看阿里云服务手册 */
#define HOST_NAME           PRODUCT_KEY".iot-as-mqtt.cn-shanghai.aliyuncs.com"                                   /* 阿里云域名 */
#define HOST_PORT           1883                                                                                 /* 阿里云域名端口，固定1883 */
#define CONTENT             "clientId"DEVICE_NAME"deviceName"DEVICE_NAME"productKey"PRODUCT_KEY"timestamp789"    /* 计算登录密码用 */
#define CLIENT_ID           DEVICE_NAME"|securemode=3,signmethod=hmacsha1,timestamp=789|"                        /* 客户端ID */
#define USER_NAME           DEVICE_NAME"&"PRODUCT_KEY                                                            /* 客户端用户名 */
#define DEVICE_PUBLISH      "/sys/"PRODUCT_KEY"/"DEVICE_NAME"/thing/event/property/post"                         /* 发布 */
#define DEVICE_SUBSCRIBE    "/sys/"PRODUCT_KEY"/"DEVICE_NAME"/thing/service/property/set"                        /* 订阅 */

void lwip_demo(void);
void upload_data(void);

#endif /* _CLIENT_H */
