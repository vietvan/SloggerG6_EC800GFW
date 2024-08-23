/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: ϵͳ�����������������ӿ�(���豸����ʵ��)
 */
#ifndef HILINK_OPEN_SYS_ADAPTER_H
#define HILINK_OPEN_SYS_ADAPTER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void *HiLinkMutex;

typedef void *HiLinkSemaphore;

typedef void *(*TaskEntryFunc)(unsigned int arg);

typedef struct {
    TaskEntryFunc funTaskEntry;
    unsigned short taskPrio;
    void *arg;
    unsigned int stackSize;
    const char *name;
    unsigned short resved;
} TaskInitParam;

/*
 * IoT OS �����߳̽ӿ�
 * ����ֵ: ����0 �ɹ�, ���� ʧ��
 */
int HILINK_CreateTask(const void *handle, TaskInitParam *initParam);

/*
 * IoT OS ɾ���߳̽ӿ�
 * �������handle ɾ���߳̾��
 * ����0 �ɹ�, ���� ʧ��
 */
int HILINK_DeleteTask(const void *handle);

/*
 * IoT OS ������
 * ������� HiLinkMutex *mutex  ���ش�������
 * ����0 �ɹ�, ���� ʧ��
 */
int HILINK_MutexCreate(HiLinkMutex *mutex);

/*
 * IoT OS ����
 * �������HiLinkMutex *mutex ��������Դ
 * ����0 �ɹ�, ���� ʧ��
 */
int HILINK_MutexLock(HiLinkMutex *mutex);

/*
 * IoT OS �����ӿں���
 * �������HiLinkMutex *mutex
 * ����0 �ɹ�, ���� ʧ��
 */
int HILINK_MutexUnlock(HiLinkMutex *mutex);

/*
 * IoT OS ɾ����
 * �������HiLinkMutex *mexut  ɾ����
 * ����0 �ɹ�, ���� ʧ��
 */
int HILINK_MutexDestroy(HiLinkMutex *mutex);

/*
 * �����ź���
 * handle��ָ���ź����Ķ���countָ���ź���ֵ�Ĵ�С
 * ����0��ʾ�ɹ�, ����ֵ��ʾʧ��
 */
int HILINK_SemaphoreCreate(HiLinkSemaphore *handle, int count);

/*
 * ���ź�����ֵ��1
 * msָ�����ź���Ϊ0�޷���1�������������ʱ������
 * ����0��ʾ�ɹ�, ����ֵ��ʾʧ��
 */
int HILINK_SemaphoreTimedWait(HiLinkSemaphore handle, int ms);

/*
 * ���ź�����ֵ��1
 * ����0��ʾ�ɹ�, ����ֵ��ʾʧ��
 */
int HILINK_SemaphorePost(HiLinkSemaphore handle);

/* �����ź��� */
void HILINK_SemaphoreDestroy(HiLinkSemaphore handle);

/*
 * IoT OS ��ȡ��ǰʱ��
 * ���������ʾ��ǰ�豸����������ʱ�� ��λ����
 * ����0 �ɹ�������ʧ��
 */
int HILINK_GetCurrTime(unsigned long *ms);

/*
 * ��ȡ��ǰ�����̵߳��̺߳�
 * ע��:freeRTOS��mico2.0���ص��ǵ�ǰ�̵߳�Handle��Contiki��֧�ָýӿ�.
 */
unsigned long HILINK_GetCurrentTaskId(void);

/*
 * ��ȡ�����������ʵ�ָýӿ�
 * ���ģ����Ӳ����Դ����Ҫ����HILINK_RegisterRandomEntropyע��Ӳ�����Դ��Ȼ�����HILINK_SecRandom���ذ�ȫ�����
 * ���ģ��û��Ӳ����Դ���򳧼����������������ͬʱ��Ҫ��Ϊ����
 */
int HILINK_Rand(void);

#ifdef __cplusplus
}
#endif

#endif
