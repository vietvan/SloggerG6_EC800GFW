/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: SoftAP�����ӿ�(���豸����ʵ��)
 */
#ifndef HILINK_OPEN_SOFTAP_ADAPTER_H
#define HILINK_OPEN_SOFTAP_ADAPTER_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * ��ȡ�㲥IP
 * broadcastIp��ʾ���IP�Ļ���
 * len��ʾ���IP�Ļ��峤��
 * ����0��ʾ�ɹ�������-1��ʾʧ��
 * ע��: broadcastIpΪ���ʮ���Ƹ�ʽ
 */
int HILINK_GetBroadcastIp(char *broadcastIp, unsigned char len);

/*
 * ��������ΪAPģʽ������SoftAP�ȵ�
 * ssid ��ʾ���ڴ���SoftAP��ssid
 * ssidLen��ʾssid����, ���ȡֵ64
 * ����0��ʾ�ɹ�������-1��ʾʧ��
 */
int HILINK_StartSoftAp(const char *ssid, unsigned int ssidLen);

/*
 * �ر�SoftAP�ȵ㲢�������л�stationģʽ
 * ����0��ʾ�ɹ�������-1��ʾʧ��
 */
int HILINK_StopSoftAp(void);

/*
 * �������ֹ��ܳ�ʼ��
 * power ��ʾ�豸���湦��
 * ����0��ʾ�ɹ�������-1��ʾʧ��
 */
int HILINK_NearDiscoveryInit(signed char power);

/*
 * �������ֹ��ܹر�
 * ����0��ʾ�ɹ�������-1��ʾʧ��
 */
int HILINK_NearDiscoveryUninit(void);

#ifdef __cplusplus
}
#endif
#endif
