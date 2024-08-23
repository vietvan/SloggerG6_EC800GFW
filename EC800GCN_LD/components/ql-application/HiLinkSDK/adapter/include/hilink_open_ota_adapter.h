/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: OTA�����ӿ�(���豸����ʵ��)
 */
#ifndef HILINK_OPEN_OTA_ADAPTER_H
#define HILINK_OPEN_OTA_ADAPTER_H

#include "hilink_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* OTA��������1 */
#ifndef UPGRADE_FW_BIN1
#define UPGRADE_FW_BIN1 0x00
#endif

/* OTA��������2 */
#ifndef UPGRADE_FW_BIN2
#define UPGRADE_FW_BIN2 0x01
#endif

/* ֻ��MCU����ʱ�������� */
#ifndef RESTART_FLAG_NOW
#define RESTART_FLAG_NOW 0x01
#endif

/* ��ģ��ʱ�л������������� */
#ifndef RESTART_FLAG_LATER
#define RESTART_FLAG_LATER 0x02
#endif

/*
 * Flash��ʼ��
 * ����ֵ��trueʱ����ʾ��ʼ������
 * ����ֵ��falseʱ����ʾ��ʼ���쳣
 */
bool HILINK_OtaAdapterFlashInit(void);

/*
 * �ж���Ҫ�����ķ���
 * ����ֵ��UPGRADE_FW_BIN1ʱ����ʾ�����̼�������1
 * ����ֵ��UPGRADE_FW_BIN2ʱ����ʾ�����̼�������2
 */
unsigned int HILINK_OtaAdapterGetUpdateIndex(void);

/*
 * ������Ҫ�����ķ���
 * size��ʾ��Ҫ�����ķ�����С
 * ����ֵ��0ʱ����ʾ�����ɹ�
 * ����ֵ��-1ʱ����ʾ����ʧ��
 */
int HILINK_OtaAdapterFlashErase(unsigned int size);

/*
 * ��������д�������ķ���
 * buf��ʾ��д������
 * bufLen��ʾ��д�����ݵĳ���
 * ����ֵ��0ʱ����ʾд��ɹ�
 * ����ֵ��-1ʱ����ʾд��ʧ��
 */
int HILINK_OtaAdapterFlashWrite(const unsigned char *buf, unsigned int bufLen);

/*
 * ��ȡ������������
 * offset��ʾ��дƫ��
 * buf��ʾ������ݵ��ڴ��ַ
 * bufLen��ʾ������ݵ��ڴ泤��
 * ����ֵ��0ʱ����ʾ��ȡ�ɹ�
 * ����ֵ��-1ʱ����ʾ��ȡʧ��
 */
int HILINK_OtaAdapterFlashRead(unsigned int offset, unsigned char *buf, unsigned int bufLen);

/*
 * ������������
 * ����ֵ��trueʱ����ʾ��������
 * ����ֵ��falseʱ����ʾ�����쳣
 */
bool HILINK_OtaAdapterFlashFinish(void);

/* ��ȡ����������󳤶� */
unsigned int HILINK_OtaAdapterFlashMaxSize(void);

/*
 * ���ݱ�־����ģ��
 * flag��ʾ������־
 * ��flag��RESTART_FLAG_NOWʱ����ʾֻ��MCU����ʱ��������
 * ��flag��RESTART_FLAG_LATERʱ����ʾ��ģ��ʱ�л�������������
 */
void HILINK_OtaAdapterRestart(int flag);

#ifdef __cplusplus
}
#endif

#endif