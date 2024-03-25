#ifndef _LWIP_DEMO_H
#define _LWIP_DEMO_H
#include "./SYSTEM/sys/sys.h"


/* �û���Ҫ�����豸��Ϣ�������º궨���е���Ԫ������ */
#define PRODUCT_KEY         "k0wydQAol09"                       /* �����ư䷢�Ĳ�ƷΨһ��ʶ��11λ���ȵ�Ӣ������������ */
#define DEVICE_NAME         "ali_server_test"                      /* �û�ע���豸ʱ���ɵ��豸Ψһ��ţ�֧��ϵͳ�Զ����ɣ�Ҳ��֧���û�����Զ����ţ���Ʒά����Ψһ  */
#define DEVICE_SECRET       "ce874fe488d9f4c05565b77d48a1ed49"  /* �豸��Կ����DeviceName�ɶԳ��֣�������һ��һ�ܵ���֤����  */

/* �궨��һ�㲻��Ҫ�޸ģ��ɲ鿴�����Ʒ����ֲ� */
#define HOST_NAME           PRODUCT_KEY".iot-as-mqtt.cn-shanghai.aliyuncs.com"                                   /* ���������� */
#define HOST_PORT           1883                                                                                 /* �����������˿ڣ��̶�1883 */
#define CONTENT             "clientId"DEVICE_NAME"deviceName"DEVICE_NAME"productKey"PRODUCT_KEY"timestamp789"    /* �����¼������ */
#define CLIENT_ID           DEVICE_NAME"|securemode=3,signmethod=hmacsha1,timestamp=789|"                        /* �ͻ���ID */
#define USER_NAME           DEVICE_NAME"&"PRODUCT_KEY                                                            /* �ͻ����û��� */
#define DEVICE_PUBLISH      "/sys/"PRODUCT_KEY"/"DEVICE_NAME"/thing/event/property/post"                         /* ���� */
#define DEVICE_SUBSCRIBE    "/sys/"PRODUCT_KEY"/"DEVICE_NAME"/thing/service/property/set"                        /* ���� */

void lwip_demo(void);
void upload_data(void);

#endif /* _CLIENT_H */
