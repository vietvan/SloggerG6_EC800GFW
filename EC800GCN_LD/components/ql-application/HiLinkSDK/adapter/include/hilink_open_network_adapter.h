/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: ���������ӿ�(���豸����ʵ��)
 */
#ifndef HILINK_OPEN_NETWORK_ADAPTER_H
#define HILINK_OPEN_NETWORK_ADAPTER_H

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_SUPPORTRATE_LIST_NUM   8
#define MAX_EXTERNSUPPORTRATE_LIST 4
#define MIN_PACKET_LEN             128
#define HILINK_BEACON_PERIOD       50 /* Beacon֡��������50ms */
#define MAX_CHANNEL_SWITCH_NUM     17

/* WiFi RSSI�ּ����� */
#define RSSI_INVALID_VALUE  31
#define RSSI_VALUE_SECTION1 (-65)
#define RSSI_VALUE_SECTION2 (-75)
#define RSSI_VALUE_SECTION3 (-82)
#define RSSI_VALUE_SECTION4 (-88)

/* WiFi�ź�ǿ�ȷּ�ֵ */
#define INTENSITY_FULL   100
#define INTENSITY_HIGH   80
#define INTENSITY_MIDDLE 60
#define INTENSITY_LOW    40
#define INTENSITY_EMPTY  20

#define MAC_ADDRESS_LEN 6

/*
 * ��ȡ����IP
 * localIp��ʾ���IP�Ļ���
 * len��ʾ���IP�Ļ��峤��
 * ����0��ʾ�ɹ�������-1��ʾʧ��
 * ע��: localIpΪ���ʮ���Ƹ�ʽ
 */
int HILINK_GetLocalIp(char *localIp, unsigned char len);

/*
 * ��ȡ����MAC��ַ
 * mac��ʾ���MAC��ַ�Ļ���
 * len��ʾ���峤��
 * ����0��ʾ�ɹ�������-1��ʾʧ��
 * ע��: mac��ʽΪa1b2c3d4e5f6
 */
int HILINK_GetMacAddr(unsigned char *mac, unsigned char len);

/*
 * ��ȡWiFi ssid
 * ssid��ʾ���WiFi ssid�Ļ���
 * ssidLen��ʾWiFi ssid�ĳ���
 * ����0��ʾ�ɹ�������-1��ʾʧ��
 */
int HILINK_GetWiFiSsid(char *ssid, unsigned int *ssidLen);

/*
 * ����WiFi�˺���Ϣ
 * ssid��ʾWiFi ssid
 * ssidLen��ʾWiFi ssid�ĳ���
 * pwd��ʾWiFi����
 * pwdLen��ʾWiFi���볤��
 * ����0��ʾ�ɹ�������-1��ʾʧ��
 * ע�⣺(1) ssid��pwdΪ�ձ�ʾ���WiFi��Ϣ;
 *       (2) ���õ�WiFi��Ϣ��Ҫ�־û�����ȷ���豸��������Ȼ���Ի��WiFi������Ϣ
 */
int HILINK_SetWiFiInfo(const char *ssid, unsigned int ssidLen, const char *pwd, unsigned int pwdLen);

/* �Ͽ�������WiFi */
void HILINK_ReconnectWiFi(void);

/*
 * ����WiFi
 * ����0��ʾ�ɹ�������-1��ʾʧ��
 */
int HILINK_ConnectWiFi(void);

/*
 * ��ȡ����״̬
 * stateΪ0��ʾ����Ͽ��������ӵ�����δ�����ip��stateΪ1��ʾ�������ҷ����ip
 * ����0��ʾ�ɹ�������-1��ʾʧ��
 */
int HILINK_GetNetworkState(int *state);

/*
 * ��ȡ��ǰ���ӵ�WiFi�� bssid
 * bssid��ʾ���WiFi bssid�Ļ���
 * bssidLen��ʾWiFi bssid����
 * ����0��ʾ�ɹ�������-1��ʾʧ��
 */
int HILINK_GetWiFiBssid(unsigned char *bssid, unsigned char *bssidLen);

/*
 * ��ȡ��ǰ���ӵ�WiFi�ź�ǿ�ȣ���λdb
 * rssi��ʾ�ź�ǿ��
 * ����0��ʾ�ɹ�������-1��ʾʧ��
 */
int HILINK_GetWiFiRssi(signed char *rssi);

/*
 * ����HiLink SDK
 * ��ϵͳ������������������HiLink����
 * ����0��ʾ�ɹ�������-1��ʾʧ��
 */
int HILINK_Restart(void);

/* ����SoftAP����ͬʱ��������station��
 * ����PIN������������
 * ���鿪��������SoftAP�ȵ������������STA��Ϊ2
 */
void HILINK_SetStationLimitNum(void);

/* SoftAP���������У�����IP�߳���Ӧ��station
 * ����PIN������������
 */
void HILINK_DisconnectStationByIp(const char *ip);

/*
 * ��ȡϵͳ����ԭ��
 * ����ֵ: HILINK_NORMAL_BOOT = 0,      �����ϵ�
 *         HILINK_WIFIDOG_REBOOT = 100, WIFI��������
 *         HILINK_SOFTDOG_REBOOT = 101, hilink�����������
 *         HILINK_HARDDOG_REBOOT = 102, �����������µ�Ӳ������λ����
 *         HILINK_HOTA_REBOOT = 103,    Hota�����ɹ�������
 *         HILINK_BOOT_REASON_BUTT      ��ȡʧ��
 */
unsigned char HILINK_GetSystemBootReason(void);

#ifdef __cplusplus
}
#endif
#endif