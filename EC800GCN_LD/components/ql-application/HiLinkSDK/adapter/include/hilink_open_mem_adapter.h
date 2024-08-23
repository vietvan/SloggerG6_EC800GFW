/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: ϵͳ������ڴ�ӿ�(���豸����ʵ��)
 */
#ifndef HILINK_OPEN_MEM_ADAPTER_H
#define HILINK_OPEN_MEM_ADAPTER_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * �����ڴ�ռ�
 * ����size��ʾ�����ڴ�ռ��С
 * ���������ڴ�ռ�ָ��
 */
void *HILINK_Malloc(unsigned int size);

/*
 * �ͷ��ڴ�ռ�
 * �������pt��ʾ�ͷ��ڴ�ռ�ָ��
 */
void HILINK_Free(void *pt);

/*
 * �ڴ�Ƚ�
 * ����0��ʾbuf1��buf2ָ����ڴ���������ͬ, ����0��ʾbuf1�е����ݴ���buf2�е�����
 */
int HILINK_Memcmp(const void *buf1, const void *buf2, unsigned int len);

#ifdef __cplusplus
}
#endif

#endif
