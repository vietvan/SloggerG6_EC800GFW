/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: HiLink��Ʒ����ͷ�ļ�
 * Notes: ���ļ��еĽӿ���Ҫ�����ṩ������������ʹ�ã�Ϊ��ǰ����ݣ������Ͻӿ��ݲ������±���淶����.
 */
#ifndef HILINK_DEVICE_H
#define HILINK_DEVICE_H

#ifdef __cplusplus
extern "C" {
#endif

/* �豸�汾��Ϣ */
#define FIRMWARE_VER "1.0.0"
#define SOFTWARE_VER "1.0.0"
#define HARDWARE_VER "1.0.0"

/* �豸��Ʒ��Ϣ */
#define PRODUCT_ID    "1011"
#define DEVICE_TYPE   "01D"
#define MANUAFACTURER "011"
#define DEVICE_MODEL  "SP mini3-HL"


/* ��ȷ���豸����Ӣ�����ͳ���Ӣ��������֮�Ͳ�����17�ֽ� */
#define DEVICE_TYPE_NAME   "Switch"
#define MANUAFACTURER_NAME "Broadlink"

/* ͨ��Э������: WiFi */
#define PROTOCOL_TYPE 1

/* HiLink SDK ֪ͨ����ģ������ԭ�� */
#define HILINK_REBOOT_WATCHDOG  0
#define HILINK_REBOOT_DEVDELETE 1

/* �豸���ƶ����ӶϿ�(�汾��ǰ����) */
#define HILINK_M2M_CLOUD_OFFLINE       0
/* �豸�����ƶ˳ɹ���������������̬(�汾��ǰ����) */
#define HILINK_M2M_CLOUD_ONLINE        1
/* �豸���ƶ����ӳ�ʱ��Ͽ�(�汾��ǰ����) */
#define HILINK_M2M_LONG_OFFLINE        2
/* �豸���ƶ����ӳ�ʱ��Ͽ����������(�汾��ǰ����) */
#define HILINK_M2M_LONG_OFFLINE_REBOOT 3
/* HiLink�߳�δ���� */
#define HILINK_UNINITIALIZED           4
/* �豸��������ģʽ */
#define HILINK_LINK_UNDER_AUTO_CONFIG  5
/* �豸����10���ӳ�ʱ״̬ */
#define HILINK_LINK_CONFIG_TIMEOUT     6
/* �豸��������·���� */
#define HILINK_LINK_CONNECTTING_WIFI   7
/* �豸�Ѿ�����·���� */
#define HILINK_LINK_CONNECTED_WIFI     8
/* �豸���������ƶ� */
#define HILINK_M2M_CONNECTTING_CLOUD   9
/* �豸��·���������ӶϿ� */
#define HILINK_M2M_CLOUD_DISCONNECT    10
/* �豸��ע�� */
#define HILINK_DEVICE_REGISTERED       11
/* �豸����� */
#define HILINK_DEVICE_UNREGISTER       12
/* �豸��λ�����λ */
#define HILINK_REVOKE_FLAG_SET         13
/* �豸Э��ע����Ϣʧ�� */
#define HILINK_NEGO_REG_INFO_FAIL      14

/* �豸���к���󳤶� */
#define MAX_SN_LENGTH 40

/* ��ȡBI �����ӿں��� */
char *hilink_get_auto_bi_rsa_cipher(void);

/* ��ȡAC �����ӿں��� */
unsigned char *hilink_get_auto_ac(void);

/*
 * �޸ķ���ǰ�ֶ�ֵ
 * svcIdΪ�����ID��payloadΪ���յ���Ҫ�޸ĵ�Json��ʽ���ֶ�����ֵ��lenΪpayload�ĳ���
 * ����0��ʾ����״ֵ̬�޸ĳɹ�������Ҫ�ײ��豸�����ϱ�����HiLink SDK�ϱ���
 * ����-101��ʾ��ñ��Ĳ�����Ҫ��
 * ����-111��ʾ����״ֵ̬�����޸��У��޸ĳɹ���ײ��豸���������ϱ���
 */
int hilink_put_char_state(const char *svcId, const char *payload, unsigned int len);

/*
 * ��ȡ�����ֶ�ֵ
 * svcId��ʾ����ID������ʵ�ָú���ʱ����Ҫ��svcId�����жϣ�
 * in��ʾ���յ���Json��ʽ���ֶ�����ֵ��
 * inLen��ʾ���յ���in�ĳ��ȣ�
 * out��ʾ��������ֶ�ֵ���ݵ�ָ��,�ڴ��ɳ��̿��٣�ʹ����ɺ���Hilink Device SDK�ͷţ�
 * outLen��ʾ��ȡ����payload�ĳ��ȣ�
 * ����0��ʾ����״̬�ֶ�ֵ��ȡ�ɹ������ط�0��ʾ��ȡ����״̬�ֶ�ֵ���ɹ���
 */
int hilink_get_char_state(const char *svcId, const char *in, unsigned int inLen, char **out, unsigned int *outLen);

/*
 * ��ȡ�豸sn��
 * ����len��ʾsn����󳤶�,39�ֽ�
 * ����sn��ʾ�豸sn
 * snָ����ַ�������Ϊ0ʱ��ʹ���豸mac��ַ��Ϊsn
 */
void HilinkGetDeviceSn(unsigned int len, char *sn);

/*
 * ��ȡ�豸��ذ汾��
 * ����0��ʾ�汾�Ż�ȡ�ɹ�������������ʾ�汾�Ż�ȡʧ��
 * ע�⣬�˽ӿ�ΪHiLink�ڲ����ú���
 */
int getDeviceVersion(char **firmwareVer, char **softwareVer, char **hardwareVer);

/*
 * ��ȡSoftAp����PIN��
 * ����ֵΪ8λ����PIN��, ����-1��ʾʹ��HiLink SDK��Ĭ��PIN��
 * �ýӿ����豸������ʵ��
 */
int HiLinkGetPinCode(void);

/*
 * ��ѯ��ǰ�豸�����Ա�ʶ
 * ����0Ϊ�������豸������1Ϊ�����豸
 */
int HILINK_IsSensitiveDevice(void);

/*
 * ֪ͨ�豸��״̬
 * status��ʾ�豸��ǰ��״̬
 * ע�⣬�˺������豸���̸��ݲ�Ʒҵ��ѡ����ʵ��
 */
void hilink_notify_devstatus(int status);

/*
 * ʵ��ģ������ǰ���豸����
 * flagΪ0��ʾHiLink SDK �߳̿��Ź�����ģ������; Ϊ1��ʾAPPɾ���豸����ģ������
 * ����0��ʾ����ɹ�, ϵͳ����������ʹ��Ӳ����; ����1��ʾ����ɹ�, ϵͳ����������ʹ��������;
 * ���ظ�ֵ��ʾ����ʧ��, ϵͳ��������
 * ע�⣬�˺������豸����ʵ�֣���APPɾ���豸����ģ������ʱ���豸��������ط���0������ᵼ��ɾ���豸�쳣
 */
int hilink_process_before_restart(int flag);

/*
 * ��ȡ�豸�����룬��֪ͨAPP
 * status��ʾ�Ƿ��͹��ϣ�0��ʾ�����ͣ�1��ʾ���ͣ�code��ʾ������
 * ����0��ʾ�ɹ������ط�0ʧ��
 */
int get_faultDetection_state(int *status, int *code);

#ifdef __cplusplus
}
#endif
#endif