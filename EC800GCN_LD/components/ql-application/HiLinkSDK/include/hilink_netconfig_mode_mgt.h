/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: HiLink SDK����ģʽ����
 */
#ifndef HILINK_NETCONFIG_MODE_MGT_H
#define HILINK_NETCONFIG_MODE_MGT_H

#ifdef __cplusplus
extern "C" {
#endif

/* HiLink SDK֧�ֵ�����ģʽ */
enum HILINK_NetConfigMode {
    HILINK_NETCONFIG_NONE, /* ��������, ͨ�����ߵ��ֶ����ӵ����� */
    HILINK_NETCONFIG_WIFI, /* HiLink SDK�ṩ��WiFi�Զ����� */
    HILINK_NETCONFIG_OTHER, /* ��������ģʽ, APP����WiFi����Ϣ, ���ɷ��յ�WiFi��Ϣ���ݺ�, ���õ�HiLink SDK */
    HILINK_NETCONFIG_BOTH, /* ��������ģʽ��WiFi������� */
    HILINK_NETCONFIG_REGISTER_ONLY, /* HiLink SDK SoftAp����������ע����Ϣ */
    HILINK_NETCONFIG_NO_SOFTAP_REGISTER_ONLY, /* ������SoftAp, PIN������������ע����Ϣ(ͨ������/4G/5G�Ƚ�������) */
    HILINK_NETCONFIG_NAN_SOFTAP, /* WiFi��֪���̾�������SoftAp��� */
    HILINK_NETCONFIG_BUTT /* �Ƿ�����ģʽ */
};

/* ���ò�Ʒ����ģʽ, ע��: ��Ҫ������HiLink SDK����֮ǰ���ñ��ӿ���������ģʽ */
int HILINK_SetNetConfigMode(enum HILINK_NetConfigMode netConfigMode);

/* ��ѯ��ǰ��Ʒ������ģʽ, ����ֵΪ��ǰ��Ʒ������ģʽ */
enum HILINK_NetConfigMode HILINK_GetNetConfigMode(void);

/*
 * �������������������������ݷ��ͽӿڵĺ�������
 * ����buf��ʾ���������ݵĻ�������ַ��lenΪ���������ݵĳ���
 * ����0��ʾ�������ݷ��ͳɹ������ط�0��ʾ����ʧ��
 */
typedef int (*HILINK_BT_SendBtDataCallback)(const unsigned char *buf, unsigned int len);

/*
 * ���������������������������ݷ��ͽӿ�ע���HiLink SDK��
 * ����callback��ʾ��ע����������ݷ��ͽӿ�
 * ����0��ʾ�ص�����ע��ɹ�������-1��ʾ�ص�����ע��ʧ��
 * ע��: �������豸�����߻����ڵ���hilink_main֮ǰ����
 */
int HILINK_BT_RegisterBtDataSendCallback(HILINK_BT_SendBtDataCallback callback);

/*
 * ������������������������ͨ���������յ��������ݺ󣬵��ô˽ӿڽ�����͸����HiLink SDK
 * ����buf��ʾ���յ������ݰ����壬len��ʾ���ݰ������С
 * ����0��ʾ����ɹ�������-1��ʾ����ʧ��
 */
int HILINK_BT_ProcessBtData(const unsigned char *buf, unsigned int len);

#ifdef __cplusplus
}
#endif

#endif
