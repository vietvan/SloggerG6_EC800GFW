/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: ��������ʵ�� (���豸����ʵ��)
 */
#include "hilink_open_network_adapter.h"

/* ����ԭ�� */
typedef enum {
    HILINK_NORMAL_BOOT = 0,      /* �����ϵ� */
    HILINK_WIFIDOG_REBOOT = 100, /* WIFI�������� */
    HILINK_SOFTDOG_REBOOT = 101, /* HiLink����������� */
    HILINK_HARDDOG_REBOOT = 102, /* �����������µ�Ӳ������λ���� */
    HILINK_HOTA_REBOOT = 103,    /* Hota�����ɹ������� */
    HILINK_BOOT_REASON_BUTT      /* ��ȡʧ�� */
} HilinkBootReason;

/*
 * ��ȡ����IP
 * localIp��ʾ���IP�Ļ���
 * len��ʾ���IP�Ļ��峤��
 * ����0��ʾ�ɹ�������-1��ʾʧ��
 * ע��: localIpΪ���ʮ���Ƹ�ʽ
 */
int HILINK_GetLocalIp(char *localIp, unsigned char len)
{
    return 0;
}

/*
 * ��ȡ����MAC��ַ
 * MAC��ʾ���MAC��ַ�Ļ���
 * len��ʾ���峤��
 * ����0��ʾ�ɹ�������-1��ʾʧ��
 * ע��: MAC��ʽΪa1b2c3d4e5f6
 */
int HILINK_GetMacAddr(unsigned char *mac, unsigned char len)
{
    return 0;
}

/*
 * ��ȡWiFi ssid
 * ssid��ʾ���WiFi ssid�Ļ���
 * ssidLen��ʾWiFi ssid�ĳ���
 * ����0��ʾ�ɹ�������-1��ʾʧ��
 */
int HILINK_GetWiFiSsid(char *ssid, unsigned int *ssidLen)
{
    return 0;
}

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
int HILINK_SetWiFiInfo(const char *ssid, unsigned int ssidLen, const char *pwd, unsigned int pwdLen)
{
    return 0;
}

/* �Ͽ�������WiFi */
void HILINK_ReconnectWiFi(void)
{
    return;
}

/*
 * ����WiFi
 * ����0��ʾ�ɹ�������-1��ʾʧ��
 */
int HILINK_ConnectWiFi()
{
    return 0;
}

/*
 * ��ȡ����״̬
 * stateΪ0��ʾ����Ͽ��������ӵ�����δ�����ip��stateΪ1��ʾ�������ҷ����ip
 * ����0��ʾ�ɹ�������-1��ʾʧ��
 */
int HILINK_GetNetworkState(int *state)
{
    return 0;
}

/*
 * ��ȡ��ǰ���ӵ�WiFi�� bssid
 * bssid��ʾ���WiFi bssid�Ļ���
 * bssidLen��ʾWiFi bssid����
 * ����0��ʾ�ɹ�������-1��ʾʧ��
 */
int HILINK_GetWiFiBssid(unsigned char *bssid, unsigned char *bssidLen)
{
    return 0;
}

/*
 * ��ȡ��ǰ���ӵ�WiFi�ź�ǿ�ȣ���λdb
 * rssi��ʾ�ź�ǿ��
 * ����0��ʾ�ɹ�������-1��ʾʧ��
 */
int HILINK_GetWiFiRssi(signed char *rssi)
{
    return 0;
}

/*
 * ����HiLink SDK
 * ��ϵͳ������������������HiLink����
 * ����0��ʾ�ɹ�������-1��ʾʧ��
 */
int HILINK_Restart(void)
{
    return 0;
}

/* ����SoftAP����ͬʱ��������station��
 * ����PIN������������
 * ���鿪��������SoftAP�ȵ������������STA��Ϊ2
 */
void HILINK_SetStationLimitNum(void)
{
    return;
}

/* SoftAP���������У�����IP�߳���Ӧ��station
 * ����PIN������������
 */
void HILINK_DisconnectStationByIp(const char *ip)
{
    return;
}

/*
 * ��ȡϵͳ����ԭ��
 * ����ֵ: HILINK_NORMAL_BOOT = 0,      �����ϵ�
 *         HILINK_WIFIDOG_REBOOT = 100, WIFI��������
 *         HILINK_SOFTDOG_REBOOT = 101, hilink�����������
 *         HILINK_HARDDOG_REBOOT = 102, �����������µ�Ӳ������λ����
 *         HILINK_HOTA_REBOOT = 103,    Hota�����ɹ�������
 *         HILINK_BOOT_REASON_BUTT      ��ȡʧ��
 */
unsigned char HILINK_GetSystemBootReason(void)
{
    return 0;
}