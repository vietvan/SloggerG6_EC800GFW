/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: ϵͳ������ַ����ӿ�(���豸����ʵ��)
 */
#ifndef HILINK_OPEN_STR_ADAPTER_H
#define HILINK_OPEN_STR_ADAPTER_H

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * ���ڼ����ַ����ĳ��ȣ�
 * �������char  *src �����㳤�ȵ��ַ�����
 * ����ֵΪ�ַ������ȡ�
 */
unsigned int HILINK_Strlen(const char *src);

/*
 * �������ַ���str�в����ַ�ch
 * ������� const strΪ�������ַ���
 * ������� ch�������ַ�
 * ����ֵNULLû�в��ҵ��ַ�����NULL ָ��ch��ָ��
 */
char *HILINK_Strchr(const char *str, int ch);

/*
 * ���ַ���str����������ַ�ch
 * �������str �ַ���
 * �������c�������ַ�
 * ����ֵNULLû�в��ҵ��ַ�����NULL ָ��ch��ָ��
 */
char *HILINK_Strrchr(const char *str, int c);

/*
 * ���ַ���ת����int��������
 * �������str ������Ҫת���� int �����ַ���
 * ���ַ���ת����int��������
 */
int HILINK_Atoi(const char *str);

/*
 * ��ʽ������ַ���
 * ������� formatΪ��ʽ��apΪ�ɱ����
 */
int HILINK_Printf(const char *format, const va_list ap);

/*
 * ��str1�в����Ƿ����str2
 * �������str1Ϊ������Ŀ�꣬str2Ҫ���Ҷ���
 * ����0 �ɹ�, ���� ʧ��
 */
char *HILINK_Strstr(char *str1, const char *str2);

/*
 * �Ƚ������ַ���str1��str2
 * �������str1Ŀ�괮1
 * �������str2Ŀ�괮2
 * �������lenΪ�Ƚϵĳ���
 * ����0 �ɹ�, ���� ʧ��
 */
int HILINK_Strncmp(const char *str1, const char *str2, unsigned int len);

#ifdef __cplusplus
}
#endif

#endif
