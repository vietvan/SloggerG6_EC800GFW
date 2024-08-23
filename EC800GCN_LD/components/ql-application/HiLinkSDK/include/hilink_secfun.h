/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: HiLink��ȫ���������Ҳ���ʵ�֣�ֱ�ӵ���
 */
#ifndef HILINK_SECFUN_H
#define HILINK_SECFUN_H

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

/*
 * ����Ŀ�Ļ�����Ϊ�ض�ֵ
 * ����0��ʾ�ɹ�, ��0��ʾʧ��
 * ע�⣺1��ȷ��count <= dest��ָ����ڴ������С��
 *       2������ӿ����Ѿ��ṩ����������ʹ�ð�ȫ�����ӿڣ�
 *       �Ѿ���������TMG�������Ϊ�˰汾���ݣ����Լ����ṩ��
 */
int hilink_memset_s(void *dest, unsigned int destMax, int c, unsigned int count);

/*
 * ����Դ�����������ݵ�Ŀ�Ļ�����
 * ����0��ʾ�ɹ�, ��0��ʾʧ��
 * ע�⣺1��ȷ��count <= destMax��ָ����ڴ������С
 *       2��ȷ��dest��srcָ����ڴ�û���ص�����
 *       3������ӿ����Ѿ��ṩ����������ʹ�ð�ȫ�����ӿڣ�
 *       �Ѿ���������TMG�������Ϊ�˰汾���ݣ����Լ����ṩ��
 */
int hilink_memcpy_s(void *dest, unsigned int destMax, const void *src, unsigned int count);

/*
 * ����ָ�����ȵ�Դ�ַ�����Ŀ�Ļ�����
 * ����0��ʾ�ɹ�, ��0��ʾʧ��
 * ע�⣺1��ȷ��strDest��ָ�ڴ�ռ��㹻���ɿ������ַ�����
 *       2������ӿ����Ѿ��ṩ����������ʹ�ð�ȫ�����ӿڣ�
 *       �Ѿ���������TMG�������Ϊ�˰汾���ݣ����Լ����ṩ��
 */
int hilink_strncpy_s(char *strDest, unsigned int destMax, const char *strSrc, unsigned int count);

/*
 * ��ָ�����ȵ�Դ�ַ������ӵ�Ŀ���ַ�������
 * ����0��ʾ�ɹ�, ��0��ʾʧ��
 * ע�⣺1��ȷ��strDest��ָ�ڴ�ռ��㹻����strDestԭ���ַ���׷�ӵ�strSrc�ַ�����
 *       2������ӿ����Ѿ��ṩ����������ʹ�ð�ȫ�����ӿڣ�
 *       �Ѿ���������TMG�������Ϊ�˰汾���ݣ����Լ����ṩ��
 */
int hilink_strncat_s(char *strDest, unsigned int destMax, const char *strSrc, unsigned int count);

/*
 * �����ݸ�ʽ�������Ŀ�Ļ�����
 * ����-1��ʾʧ��, ������ʾʵ��д��strDest���ֽ���
 * ע�⣺1��strDestҪ���㹻�Ŀռ����ɸ�ʽ�����ַ�����
 *       2������ӿ����Ѿ��ṩ����������ʹ�ð�ȫ�����ӿڣ�
 *       �Ѿ���������TMG�������Ϊ�˰汾���ݣ����Լ����ṩ��
 */
int hilink_sprintf_s(char *strDest, unsigned int destMax, const char *format, ...);

/*
 * �����ݰ���ָ�����ȸ�ʽ�������Ŀ�Ļ�����
 * ����-1��ʾʧ��, ������ʾ��д��strDest���ֽ���
 * ע�⣺����ӿ����Ѿ��ṩ����������ʹ�ð�ȫ�����ӿڣ�
 *       �Ѿ���������TMG�������Ϊ�˰汾���ݣ����Լ����ṩ��
 */
int hilink_snprintf_s(char *strDest, unsigned int destMax, unsigned int count, const char *format, ...);

#ifdef __cplusplus
}
#endif /* __cpluscplus */

#endif