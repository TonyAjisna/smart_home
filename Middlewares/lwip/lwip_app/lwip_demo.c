#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdint.h>
#include <stdio.h>
#include <netdb.h>
#include "lwip/apps/mqtt.h"
#include "./BSP/LCD/lcd.h"
#include "./MALLOC/malloc.h"
#include "./BSP/LED/led.h"
#include "lwip_demo.h"
#include "hmac.h"
#include "string.h"
#include "./BSP/ADC/adc.h"
#include "./BSP/DHT11/dht11.h"


/* oneNET�ο����£�https://open.iot.10086.cn/doc/v5/develop/detail/251 */

//static const struct mqtt_connect_client_info_t mqtt_client_info =
//{
//    "MQTT",     /* �豸�� */
//    "366007",   /* ��ƷID */
//    "version=2018-10-31&res=products%2F366007%2Fdevices%2FMQTT&et=1672735919&method=md5&sign=qI0pgDJnICGoPdhNi%2BHtfg%3D%3D", /* pass */
//    100,  /* keep alive */
//    NULL, /* will_topic */
//    NULL, /* will_msg */
//    0,    /* will_qos */
//    0     /* will_retain */
//#if LWIP_ALTCP && LWIP_ALTCP_TLS  /* ���ܲ���������һ�㲻ʹ�ü��ܲ��� */
//  , NULL
//#endif
//};

static ip_addr_t g_mqtt_ip;
static mqtt_client_t* g_mqtt_client;
float g_temp = 0;    /* �¶�ֵ */
float g_humid = 0;   /* ʪ��ֵ */
unsigned char g_payload_out[200];
int g_payload_out_len = 0;
int g_publish_flag = 0;/* �����ɹ���־λ */
void lwip_ali_get_password(const char *device_secret, const char *content, char *password);

/**
 * @brief       mqtt�������ݻص�����
 * @param       arg������Ĳ���
 * @param       data������
 * @param       len�����ݴ�С
 * @param       flags����־
 * @retval      ��
 */
static void 
mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags)
{
    const struct mqtt_connect_client_info_t* client_info = (const struct mqtt_connect_client_info_t*)arg;
    LWIP_UNUSED_ARG(data);
    printf("\r\nMQTT client \"%s\" data cb: len %d, flags %d\n", client_info->client_id,(int)len, (int)flags);
    
    if (flags & MQTT_DATA_FLAG_LAST)
    {
        /* ��������Խ��հ����Ʒ��������� */ 
    }
}

/**
 * @brief       mqtt���뷢���ص�����
 * @param       arg������Ĳ���
 * @param       topic������
 * @param       tot_len�������С
 * @retval      ��
 */
static void
mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len)
{
    const struct mqtt_connect_client_info_t* client_info = (const struct mqtt_connect_client_info_t*)arg;

    printf("\r\nMQTT client \"%s\" publish cb: topic %s, len %d\r\n", client_info->client_id,
          topic, (int)tot_len);
}

/**
 * @brief       mqtt�����ص�����
 * @param       arg������Ĳ���
 * @param       err������ֵ
 * @retval      ��
 */
static void
mqtt_publish_request_cb(void *arg, err_t err)
{
    printf("publish success\r\n");
}

/**
 * @brief       mqtt������Ӧ�ص�����
 * @param       arg������Ĳ���
 * @param       err������ֵ
 * @retval      ��
 */
static void
mqtt_request_cb(void *arg, err_t err)
{
    const struct mqtt_connect_client_info_t* client_info = (const struct mqtt_connect_client_info_t*)arg;
    
    g_publish_flag = 1;
    printf("\r\nMQTT client \"%s\" request cb: err %d\r\n", client_info->client_id, (int)err);
}

/**
 * @brief       mqtt���ӻص�����
 * @param       client���ͻ��˿��ƿ�
 * @param       arg������Ĳ���
 * @param       status������״̬
 * @retval      ��
 */
static void
mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status)
{
    err_t err;
    
    const struct mqtt_connect_client_info_t* client_info = (const struct mqtt_connect_client_info_t*)arg;
    
    LWIP_UNUSED_ARG(client);

    printf("\r\nMQTT client \"%s\" connection cb: status %d\r\n", client_info->client_id, (int)status);

    /* �ж��Ƿ����� */ 
    if (status == MQTT_CONNECT_ACCEPTED)
    {
        /* �ж��Ƿ����� */ 
        if (mqtt_client_is_connected(client))
        {
            /* ���ô��뷢������Ļص� */
            mqtt_set_inpub_callback(g_mqtt_client,
                                    mqtt_incoming_publish_cb,
                                    mqtt_incoming_data_cb,
                                    NULL);
            
            /* ���Ĳ����������ö�����Ӧ��ص�����mqtt_sub_request_cb */ 
            err = mqtt_subscribe(client, DEVICE_SUBSCRIBE, 1, mqtt_request_cb, arg);
            
            if(err == ERR_OK)
            {
                printf("mqtt_subscribe return: %d\n", err);
                lcd_show_string(5, 170, 210, 16, 16, "mqtt_subscribe succeed", BLUE);
            }

        }
    }
    else/* ����ʧ�� */
    {
        printf("mqtt_connection_cb: Disconnected, reason: %d\n", status);
    } 
}

/**
 * @brief       lwip_demo����
 * @param       ��
 * @retval      ��
 */
void lwip_demo(void)
{
    struct hostent *server;
    static struct mqtt_connect_client_info_t mqtt_client_info;
    
    
    server = gethostbyname((char *)HOST_NAME);        /* ��oneNET��������ַ���� */
    memcpy(&g_mqtt_ip,server->h_addr,server->h_length); /* �ѽ����õĵ�ַ�����mqtt_ip�������� */
    
    char *PASSWORD;
    PASSWORD = mymalloc(SRAMIN, 300); /* Ϊ���������ڴ� */
    lwip_ali_get_password(DEVICE_SECRET, CONTENT, PASSWORD);  /*  ͨ��hmac_sha1�㷨�õ�password  */
    
    
    /* ����һ���յĿͻ�����Ϣ�ṹ */
    memset(&mqtt_client_info, 0, sizeof(mqtt_client_info));
    
    /* ���ÿͻ��˵���Ϣ�� */ 
    mqtt_client_info.client_id = (char *)CLIENT_ID;     /* �豸���� */
    mqtt_client_info.client_user = (char *)USER_NAME;   /* ��ƷID */
    mqtt_client_info.client_pass = (char *)PASSWORD;    /* ������������� */
    mqtt_client_info.keep_alive = 100;                  /* ����ʱ�� */
    mqtt_client_info.will_msg = NULL;
    mqtt_client_info.will_qos = NULL;
    mqtt_client_info.will_retain = 0;
    mqtt_client_info.will_topic = 0;
    
    myfree(SRAMIN, PASSWORD);                 /* �ͷ��ڴ� */
    
    /* ����MQTT�ͻ��˿��ƿ� */
    g_mqtt_client = mqtt_client_new();
    
    /* ���ӷ����� */
    mqtt_client_connect(g_mqtt_client,        /* ���������ƿ� */
                        &g_mqtt_ip, MQTT_PORT,/* ������IP��˿ں� */
                        mqtt_connection_cb, LWIP_CONST_CAST(void*, &mqtt_client_info),/* ���÷��������ӻص����� */
                        &mqtt_client_info); /* MQTT������Ϣ */
    while(1)
    {
        upload_data();
        vTaskDelay(1000);
    }
}

/**
 * @brief      ��16������ת��Ϊ�ַ���
 * @param      pbSrc - ����16����������ʼ��ַ
 * @param      nLen - 16���������ֽ���
 * @param       pbDest - ���Ŀ���ַ���
 * @retval     ��
 */
void lwip_ali_hextostr(uint8_t *pbDest, uint8_t *pbSrc, int nLen)
{
    char ddl, ddh;
    int i;

    for (i = 0; i < nLen; i++)
    {
        ddh = 48 + pbSrc[i] / 16;
        ddl = 48 + pbSrc[i] % 16;

        if (ddh > 57) ddh = ddh + 7;

        if (ddl > 57) ddl = ddl + 7;

        pbDest[i * 2] = ddh;
        pbDest[i * 2 + 1] = ddl;
    }

    pbDest[nLen * 2] = '\0';
}

/**
 * @brief      ͨ��hmac_sha1�㷨��ȡpassword
 * @param      device_secret---�豸����Կ
 * @param      content --- ��¼����
 * @param      password---���ص�����ֵ
 * @retval     ��
 */
void lwip_ali_get_password(const char *device_secret, const char *content, char *password)
{
    char buf[256] = {0};
    int len = sizeof(buf);
    hmac_sha1((uint8_t *)device_secret, strlen(device_secret), (uint8_t *)content, strlen(content), (uint8_t *)buf, (unsigned int *)&len);
    lwip_ali_hextostr((uint8_t *)password, (uint8_t *)buf, len);
}


extern uint8_t temperature;
extern uint8_t humidity;
void upload_data(void)
{
    if (g_publish_flag == 1)
    {
        dht11_read_data(&temperature, &humidity);             /* ��ȡ��ʪ��ֵ */
        g_temp = (float)temperature;       /* �õ��¶�ֵ */
        g_humid = (float)humidity;/* ʪ�ȵ����� */
        g_temp = 23.8;       /* �õ��¶�ֵ */
        g_humid = 56.1;/* ʪ�ȵ����� */
        sprintf((char *)g_payload_out, "{\"params\":{\"CurrentTemperature\":+%0.1f,\"RelativeHumidity\":%0.1f},\"method\":\"thing.event.property.post\"}", g_temp, g_humid);
        g_payload_out_len = strlen((char *)g_payload_out);
        mqtt_publish(g_mqtt_client,DEVICE_PUBLISH,g_payload_out,g_payload_out_len,1,0,mqtt_publish_request_cb,NULL);
    }
}

