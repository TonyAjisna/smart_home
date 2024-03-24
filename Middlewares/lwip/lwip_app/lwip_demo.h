/**
 ****************************************************************************************************
 * @file        lwip_demo.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-04
 * @brief       lwIP+Aliyun+MQTTʵ��
 * @license     Copyright (c) 2020-2032, �������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� ̽���� F407������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 ****************************************************************************************************
 */
 
#ifndef _LWIP_DEMO_H
#define _LWIP_DEMO_H
#include "./SYSTEM/sys/sys.h"


/* �û���Ҫ�����豸��Ϣ�������º궨���е���Ԫ������ */
#define PRODUCT_KEY         "a1S0GvSCHqT"                       /* �����ư䷢�Ĳ�ƷΨһ��ʶ��11λ���ȵ�Ӣ������������ */
#define DEVICE_NAME         "ATK_MQTT_DEV"                      /* �û�ע���豸ʱ���ɵ��豸Ψһ��ţ�֧��ϵͳ�Զ����ɣ�Ҳ��֧���û������Զ����ţ���Ʒά����Ψһ  */
#define DEVICE_SECRET       "39e260510264457f9a67c12b21eab692"  /* �豸��Կ����DeviceName�ɶԳ��֣�������һ��һ�ܵ���֤����  */

/* ���²����ĺ궨��̶�������Ҫ�޸ģ�ֻ�޸��Ϸ��Ĳ������� */
#define HOST_NAME           PRODUCT_KEY".iot-as-mqtt.cn-shanghai.aliyuncs.com"                                   /* ���������� */
#define HOST_PORT           1883                                                                                 /* �����������˿ڣ��̶�1883 */
#define CONTENT             "clientId"DEVICE_NAME"deviceName"DEVICE_NAME"productKey"PRODUCT_KEY"timestamp789"    /* �����¼������ */
#define CLIENT_ID           DEVICE_NAME"|securemode=3,signmethod=hmacsha1,timestamp=789|"                        /* �ͻ���ID */
#define USER_NAME           DEVICE_NAME"&"PRODUCT_KEY                                                            /* �ͻ����û��� */
#define DEVICE_PUBLISH      "/sys/"PRODUCT_KEY"/"DEVICE_NAME"/thing/event/property/post"                         /* ���� */
#define DEVICE_SUBSCRIBE    "/sys/"PRODUCT_KEY"/"DEVICE_NAME"/thing/service/property/set"                        /* ���� */

void lwip_demo(void);

#endif /* _CLIENT_H */