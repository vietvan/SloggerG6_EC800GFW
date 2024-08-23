/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: jsonͷ�ļ������Ҳ���ʵ�֣�ֱ�ӵ���
 */
#ifndef HILINK_CJSON_H
#define HILINK_CJSON_H

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

/*
 * ���ַ�����ʽ��json���ݽ���Ϊjson�ṹ�����͵�����
 * ����NULL-����ʧ��,��NULL-�����ɹ�������ֵΪjson�ṹ��ָ��
 */
void *hilink_json_parse(const char *value);

/*
 * ��ȡjson�ṹ���е��ַ���value
 * ����NULL-��ȡʧ��,��NULL-��ȡ�ɹ�������ֵΪ�ַ���value�׵�ַ
 */
char *hilink_json_get_string_value(const void *object, const char *name, unsigned int *len);

/*
 * ��ȡjson�ṹ���е���ֵvalue
 * ����0-�ɹ�,-1-ʧ��
 */
int hilink_json_get_number_value(const void *object, const char *name, int *value);

/*
 * ��ȡjson�ṹ���е�object
 * ����NULL-����ʧ��,��NULL-�����ɹ�������ֵΪjson object�ṹ��ָ��
 */
void *hilink_json_get_object(const void *object, const char *name);

/*
 * ��ȡjson�ṹ���е�arrayԪ��
 * ����NULL-����ʧ��, ��NULL-�����ɹ�������ֵΪjson object�ṹ��ָ��
 */
void *hilink_json_get_arrayitem(const void *array, int item);

/*
 * ��ȡjson�ṹ���е�array�Ĵ�С
 * ����Ԫ������
 */
int hilink_json_get_arraysize(const void *array);

/* �ͷ�json�ṹ����ռ�õĿռ� */
void hilink_json_delete(void *object);

/* json�������ַ���������0 */
void hilink_json_clear_all_string(void *object);

#ifdef __cplusplus
}
#endif /* __cpluscplus */

#endif