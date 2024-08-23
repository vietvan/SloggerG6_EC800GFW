/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: ������Ϣ��д�����ӿ� (���豸����ʵ��)
 */
#include "hilink_open_config_adapter.h"
#include "hilink_typedef.h"

/*
 * ��ʼ�����ñ������
 * ����0��ʾ�ɹ�������-1��ʾʧ��
 */
int HILINK_InitConfig(void)
{
    return 0;
}

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
int HILINK_WriteConfig(enum HILINK_ConfigType configType, const unsigned char *buf, unsigned int len)
{
    //int ret;  //Quectel larson.li ��������δʹ����ʱע�͵�

    if (buf == NULL) {
        return -1;
    }

    switch (configType) {
        case HILINK_RUNNING_CONFIG:
            /* HiLink������������ */
            break;
        case HILINK_TIMER_CONFIG:
            /* HiLink��ʱ�����������ݣ���Ҫ��ʱ������Ҫ���� */
            break;
        case HILINK_BRIDGE_CONFIG:
            /* HiLink���豸�������ݣ���ǰΪ���豸ʱ��Ҫ���� */
            break;
        case HILINK_HKS_CONFIG:
            /* �˵��˰�ȫhichain�����Ҫ�洢���ݣ���Ҫ�˵��˹���ʱ��Ҫ���� */
            break;
        case HILINK_URL_CONFIG:
            /* HiLink�����������ݣ���Ҫ����������Ҫ���� */
            break;
        case HILINK_URL_BAK_CONFIG:
            /* HiLink���������������ݣ���Ҫ��������������Ҫ���� */
            break;
        case HILINK_BRG_SUBDEV_NODE:
            /* HiLink�������豸�ڵ��������ݣ�ʹ��meshͨ�����ع��ܵ���Ҫ���� */
            break;
        case HILINK_BRG_SUBDEV_MAP:
            /* HiLink�������豸map�������ݣ�ʹ��meshͨ�����ع��ܵ���Ҫ���� */
            break;
        default:
            return -1;
    }

    return 0;
}

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
int HILINK_ReadConfig(enum HILINK_ConfigType configType, unsigned char *buf, unsigned int len)
{
    if (buf == NULL) {
        return -1;
    }

    switch (configType) {
        case HILINK_RUNNING_CONFIG:
            /* HiLink������������ */
            break;
        case HILINK_TIMER_CONFIG:
            /* HiLink��ʱ�����������ݣ���Ҫ��ʱ������Ҫ���� */
            break;
        case HILINK_BRIDGE_CONFIG:
            /* HiLink���豸�������ݣ���ǰΪ���豸ʱ��Ҫ���� */
            break;
        case HILINK_HKS_CONFIG:
            /* �˵��˰�ȫhichain�����Ҫ�洢���ݣ���Ҫ�˵��˹���ʱ��Ҫ���� */
            break;
        case HILINK_URL_CONFIG:
            /* HiLink�����������ݣ���Ҫ����������Ҫ���� */
            break;
        case HILINK_URL_BAK_CONFIG:
            /* HiLink���������������ݣ���Ҫ��������������Ҫ���� */
            break;
        case HILINK_BRG_SUBDEV_NODE:
            /* HiLink�������豸�ڵ��������ݣ�ʹ��meshͨ�����ع��ܵ���Ҫ���� */
            break;
        case HILINK_BRG_SUBDEV_MAP:
            /* HiLink�������豸map�������ݣ�ʹ��meshͨ�����ع��ܵ���Ҫ���� */
            break;
        default:
            return -1;
    }

    return 0;
}