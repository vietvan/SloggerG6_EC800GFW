/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: SoftAP����ʵ�� (���豸����ʵ��)
 */
#include "hilink_open_softap_adapter.h"

/*
 * ��������ΪAPģʽ������SoftAP�ȵ�
 * ssid ��ʾ���ڴ���SoftAP��ssid
 * ssidLen��ʾssid����, ���ȡֵ64
 * ����0��ʾ�ɹ�������-1��ʾʧ��
 */
int HILINK_StartSoftAp(const char *ssid, unsigned int ssidLen)
{
    return 0;
}

/*
 * ��ȡ�㲥IP
 * broadcastIp��ʾ���IP�Ļ���
 * len��ʾ���IP�Ļ��峤��
 * ����0��ʾ�ɹ�������-1��ʾʧ��
 * ע��: broadcastIpΪ���ʮ���Ƹ�ʽ
 */
int HILINK_GetBroadcastIp(char *broadcastIp, unsigned char len)
{
    return 0;
}

/*
 * �ر�SoftAP�ȵ㲢�������л�stationģʽ
 * ����0��ʾ�ɹ�������-1��ʾʧ��
 */
int HILINK_StopSoftAp(void)
{
    return 0;
}