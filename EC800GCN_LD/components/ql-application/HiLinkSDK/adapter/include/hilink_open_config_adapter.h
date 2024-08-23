/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: ���������ӿ� (���豸����ʵ��)
 */
#ifndef HILINK_OPEN_CONFIG_ADAPTER_H
#define HILINK_OPEN_CONFIG_ADAPTER_H

#ifdef __cplusplus
extern "C" {
#endif

/* HiLink������������ */
enum HILINK_ConfigType {
    HILINK_RUNNING_CONFIG = 0,  /* HiLink������������ */
    HILINK_TIMER_CONFIG = 1,    /* HiLink��ʱ���������� */
    HILINK_BRIDGE_CONFIG = 2,   /* ���豸���ò��� */
    HILINK_HKS_CONFIG = 3,      /* �˵��˰�ȫhichain�������ļ� */
    HILINK_URL_CONFIG = 4,      /* Url�������� */
    HILINK_URL_BAK_CONFIG = 5,  /* Url���ñ������� */
    HILINK_BRG_SUBDEV_NODE = 6, /* �������豸�ڵ����� */
    HILINK_BRG_SUBDEV_MAP = 7,  /* �������豸map���� */
};

/*
 * ��ʼ�����ñ������
 * ����0��ʾ�ɹ�������-1��ʾʧ��
 */
int HILINK_InitConfig(void);

/*
 * ����HiLink������Ϣ
 * configType��ʾHiLink��������
 *     HILINK_RUNNING_CONFIG HiLink�����������ݣ���������
 *     HILINK_TIMER_CONFIG   HiLink��ʱ�����������ݣ���Ҫ��ʱ����ʱ����
 *     HILINK_BRIDGE_CONFIG  ���豸���ò�������ǰΪ���豸ʱ��Ҫ����
 *     HILINK_HKS_CONFIG     �˵��˰�ȫhichain�����Ҫ�洢���ݣ���Ҫ�˵��˹���ʱ��Ҫ����
 * buf��ʾHiLink������Ϣbuf��ַ
 * len��ʾHiLink������Ϣ����
 * ����0��ʾ�ɹ�������-1��ʾʧ��
 */
int HILINK_WriteConfig(enum HILINK_ConfigType configType, const unsigned char *buf, unsigned int len);

/*
 * ��ȡHiLink������Ϣ
 * configType��ʾHiLink��������
 *     HILINK_RUNNING_CONFIG HiLink������������
 *     HILINK_TIMER_CONFIG   HiLink��ʱ�����������ݣ���Ҫ��ʱ����ʱ����
 *     HILINK_BRIDGE_CONFIG  ���豸���ò�������ǰΪ���豸ʱ��Ҫ����
 *     HILINK_HKS_CONFIG     �˵��˰�ȫhichain�����Ҫ�洢���ݣ���Ҫ�˵��˹���ʱ��Ҫ����
 * buf��ʾHiLink������Ϣbuf��ַ
 * len��ʾHiLink������Ϣ����
 * ����0��ʾ�ɹ�������-1��ʾʧ��
 */
int HILINK_ReadConfig(enum HILINK_ConfigType configType, unsigned char *buf, unsigned int len);

#ifdef __cplusplus
}
#endif
#endif