/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: ģ��ϵͳ������ڴ�ӿں���(���豸����ʵ��)
 */
#include "hilink_open_mem_adapter.h"
#include <stdlib.h>

/*
 * �����ڴ�ռ�
 * ����size��ʾ�����ڴ�ռ��С
 * ���������ڴ�ռ�ָ��
 */
void *HILINK_Malloc(unsigned int size)
{
    return NULL;
}

/*
 * �ͷ��ڴ�ռ�
 * �������pt��ʾ�ͷ��ڴ�ռ�ָ��
 */
void HILINK_Free(void *pt)
{
    return;
}

/*
 * �ڴ�Ƚ�
 * ����0��ʾbuf1��buf2ָ����ڴ���������ͬ, ����0��ʾbuf1�е����ݴ���buf2�е�����
 */
int HILINK_Memcmp(const void *buf1, const void *buf2, unsigned int len)
{
    return 0;
}