/*
* Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
* Description: OS������ַ����ӿ�(���豸����ʵ��)
*/
#include "hilink_open_str_adapter.h"
#include <string.h>
#include "hilink_typedef.h"

/*
 * ���ڼ����ַ����ĳ��ȣ�
 * �������char  *src �����㳤�ȵ��ַ�����
 * ����ֵΪ�ַ������ȡ�
 */
unsigned int HILINK_Strlen(const char *src)
{
    return 0;
}

/*
 * �������ַ���str�в����ַ�ch
 * ������� const strΪ�������ַ���
 * ������� ch�������ַ�
 * ����ֵNULLû�в��ҵ��ַ�����NULL ָ��ch��ָ��
 */
char *HILINK_Strchr(const char *str, int ch)
{
    return NULL;
}

/*
 * ���ַ���str����������ַ�ch
 * �������str �ַ���
 * �������c�������ַ�
 * ����ֵNULLû�в��ҵ��ַ�����NULL ָ��ch��ָ��
 */
char *HILINK_Strrchr(const char *str, int c)
{
    return NULL;
}

/*
 * ���ַ���ת����int��������
 * �������str ������Ҫת���� int �����ַ���
 * ���ַ���ת����int��������
 */
int HILINK_Atoi(const char *str)
{
    return 0;
}

/*
 * ��ʽ������ַ���
 * ������� format ��ʽ��apΪ�ɱ����
 */
int HILINK_Printf(const char *format, va_list ap)
{
    return 0;
}

/*
 * �Ƚ������ַ���str1��str2
 * �������str1Ŀ�괮1
 * �������str2Ŀ�괮2
 * �������lenΪ�Ƚϵĳ���
 * ����0 �ɹ�, ���� ʧ��
 */
int HILINK_Strncmp(const char *str1, const char *str2, unsigned int len)
{
    return 0;
}