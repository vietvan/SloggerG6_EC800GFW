/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: OTA����ʵ�� (���豸����ʵ��)
 */
#include "hilink_open_ota_adapter.h"
#include "hilink_typedef.h"

/*
 * Flash��ʼ��
 * ����ֵ��trueʱ����ʾ��ʼ������
 * ����ֵ��falseʱ����ʾ��ʼ���쳣
 */
bool HILINK_OtaAdapterFlashInit(void)
{
    return true;
}

/*
 * �ж���Ҫ�����ķ���
 * ����ֵ��UPGRADE_FW_BIN1ʱ����ʾ�����̼�������1
 * ����ֵ��UPGRADE_FW_BIN2ʱ����ʾ�����̼�������2
 */
unsigned int HILINK_OtaAdapterGetUpdateIndex(void)
{
    return UPGRADE_FW_BIN1;
}

/*
 * ������Ҫ�����ķ���
 * size��ʾ��Ҫ�����ķ�����С
 * ����ֵ��ILINK_OKʱ����ʾ�����ɹ�
 * ����ֵ��HILINK_ERRORʱ����ʾ����ʧ��
 */
int HILINK_OtaAdapterFlashErase(unsigned int size)
{
    return 0;
}

/*
 * ��������д�������ķ���
 * buf��ʾ��д������
 * bufLen��ʾ��д�����ݵĳ���
 * ����ֵ��HILINK_OKʱ����ʾд��ɹ�
 * ����ֵ��HILINK_ERRORʱ����ʾд��ʧ��
 */
int HILINK_OtaAdapterFlashWrite(const unsigned char *buf, unsigned int bufLen)
{
    return 0;
}

/*
 * ��ȡ������������
 * offset��ʾ��дƫ��
 * buf��ʾ������ݵ��ڴ��ַ
 * bufLen��ʾ������ݵ��ڴ泤��
 * ����ֵ��HILINK_OKʱ����ʾ��ȡ�ɹ�
 * ����ֵ��HILINK_ERRORʱ����ʾ��ȡʧ��
 */
int HILINK_OtaAdapterFlashRead(unsigned int offset, unsigned char *buf, unsigned int bufLen)
{
    return 0;
}

/*
 * ������������
 * ����ֵ��trueʱ����ʾ��������
 * ����ֵ��falseʱ����ʾ�����쳣
 */
bool HILINK_OtaAdapterFlashFinish(void)
{
    return true;
}

/* ��ȡ����������󳤶� */
unsigned int HILINK_OtaAdapterFlashMaxSize(void)
{
    return 0;
}

/*
 * ���ݱ�־����ģ��
 * flag��ʾ������־
 * ��flag��RESTART_FLAG_NOWʱ����ʾֻ��MCU����ʱ��������
 * ��flag��RESTART_FLAG_LATERʱ����ʾ��ģ��ʱ�л�������������
 */
void HILINK_OtaAdapterRestart(int flag)
{
    return;
}