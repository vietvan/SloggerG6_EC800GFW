/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: ϵͳ������ڴ�ӿ�(���豸����ʵ��)
 */
#ifndef HILINK_OPEN_MEM_ADAPTER_H
#define HILINK_OPEN_MEM_ADAPTER_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * WIFI��֪�ײ�������ʼ��
 * ����0��ʾ�ɹ�������-1��ʾʧ��
 */
int HILINK_NanInit(void);

/*
 * WIFI��֪�ײ�����ȥ��ʼ��
 * ����0��ʾ�ɹ�������-1��ʾʧ��
 */
int HILINK_NanUninit(void);

/*
 * WIFI��֪�������ķ���
 * ����0��ʾ�ɹ�������-1��ʾʧ��
 */
int HILINK_NanStartSubscribe(const char *serviceName, unsigned char localHandle, void *handle);

/*
 * WIFI��ֹ֪ͣ���ķ���
 * ����0��ʾ�ɹ�������-1��ʾʧ��
 */
int HILINK_NanStopSubscribe(unsigned char localHandle);

/*
 * WIFI��֪�����ӿ�
 * ����0��ʾ�ɹ�������-1��ʾʧ��
 */
int HILINK_NanSendPacket(unsigned char *macAddr, unsigned char peerHandle, unsigned char localHandle,
    unsigned char *msg, int len);

/*
 * WIFI��֪�л������̾�״̬
 * ����0��ʾ�ɹ�������-1��ʾʧ��
 */
int HILINK_NanSwitchSafeDistance(void);

/*
 * WIFI��֪�л�����������״̬
 * ����0��ʾ�ɹ�������-1��ʾʧ��
 */
int HILINK_NanSwitchNormalDistance(void);

/*
 * beacon֡�����͹ر�
 * ����0��ʾ�ɹ�������-1��ʾʧ��
 */
int HILINK_NanBeaconSwitch(unsigned char enable);

int HILINK_SoftApDeauthStation(const unsigned char *mac, unsigned char len);

typedef enum {
    EVT_AP_START = 1,
    EVT_STA_CONNECT,
    EVT_STA_DISCONNECT
} SoftApEventType;

typedef struct {
    char addr[6]; //Quectel larson.li MAC_ADDRESS_LEN �궨���Ҳ��������ݾ���MAC LENΪ6
} EventStaConnect;

typedef struct {
    unsigned char addr[6];//Quectel larson.li MAC_ADDRESS_LEN �궨���Ҳ��������ݾ���MAC LENΪ6
    unsigned short reasonCode;
} EventStaDisconnect;

typedef union {
    EventStaConnect staConnect;
    EventStaDisconnect staDisconnect;
} SoftApEventInfo;

typedef struct {
    SoftApEventType type;
    SoftApEventInfo info;
} SoftApEvent;

typedef void (*SoftApEventCallback)(const SoftApEvent *event);

/* ע��SoftAP�¼�����ص����� */
void HILINK_RegSoftApEventCallback(SoftApEventCallback callback);

#ifdef __cplusplus
}
#endif

#endif
