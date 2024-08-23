/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: ���ļ��ṩ��������صĽӿ�ʵ��ʾ��;
 *              ����������ع��ܵĽӿ���Ҫ�����߻��̽���ʵ�֣�HiLink SDK���յ������������ʱ����ö�Ӧ�ӿ�;
 *              �뿪������ϸ�Ķ��ļ��е�ע��˵�����ο����޸�ʵ�֡�
 */
#ifndef HILINK_OTA_H
#define HILINK_OTA_H

#ifdef __cplusplus
extern "C" {
#endif

// ��������
#define RETURN_OK               0
// û��MCU
#define RETURN_ERROR_NO_MCU     (-1)
// MCU������������
#define RETURN_ERROR            (-2)
// MCU����Ҫ����
#define RETURN_MCU_NO_NEED_OTA  (-3)
// ģ���������
#define MODULE_CAN_REBOOT       1
// ģ�鲻������
#define MODULE_CANNOT_REBOOT    0
// �ֶ�����
#define UPDATE_TYPE_MANUAL      0
// �Զ�����
#define UPDATE_TYPE_AUTO        1
// ��ʼ����MCU����
#define START_SEND_DATA         0
// ����MCU���ݽ���
#define STOP_SEND_DATA          1
// �쳣��ֹ
#define SEND_DATA_ERROR         2

typedef enum {
    SUB_DEV_UPDATE_SUCCESS = 0,
    SUB_DEV_UPDATE_FAIL
} SubDevUpdateStatus;

typedef void (*GetOtaVerCb)(const char *version);

/*
 * ��ȡMCU��ǰ�汾
 * version��ʾ�汾�ַ���
 * inLen��ʾ�����ַ�������
 * outLen��ʾ����ַ�������
 * ����ֵ��RETURN_OKʱ����ʾ��ȡ�ɹ�
 * ����ֵ��RETURN_ERROR_NO_MCUʱ����ʾû��MCU
 * ����ֵ��RETURN_ERRORʱ����ʾ��ȡʧ��
 * ע�⣺�����ȡ����MCU�İ汾���򲻶�MCU����������
 * ���鳧����MCU���������󣬻����������󣬾ͽ�MCU�İ汾�Ŵ��ݸ�ģ�飬ȷ��ģ����Ի�ȡ��MCU�İ汾��
 */
int HilinkGetMcuVersion(char *version, unsigned int inLen, unsigned int *outLen);

/*
 * ��ʼģ������
 * type��ʾ��������
 * ��type��UPDATE_TYPE_MANUALʱ����ʾ�����������������û�����������ֶ�����
 * ��type��UPDATE_TYPE_AUTOʱ����ʾ�������������Ǿ����û�ͬ����Զ�����
 * ����ֵ��RETURN_OKʱ����ʾ����ɹ���HiLink SDK����ʼ������������
 * ����ֵ��RETURN_ERRORʱ����ʾ�����ɹ���HiLink SDK����ֹ������������
 * ע�⣺���ֶ����������£�HiLink SDK�ڽ��յ��û�����������ָ��󣬽�ֱ�ӵ��ô˽ӿڣ�
 * ���Զ����������£���HiLink SDK�ڵ���HilinkGetRebootFlag�ӿڷ���ֵ��MODULE_CAN_REBOOTʱ��HiLink SDK�����ô˽ӿڡ�
 * ���̿��ڴ˽ӿ�����ɺ�����������صĴ���
 * ������10���ӵ�1Сʱ�����ʱ����һ���Ƿ����°汾��֮���Ե�ǰʱ��Ϊ��㣬23Сʱ��1Сʱ�����ֵ�����Լ���°汾��
 * ����û������Զ��������أ���⵽���°汾�����ǿ�������������£��ͽ����°汾�����أ�������ɺ��Զ�������
 * �Զ��������̿������賿���У���˳�����ʵ������������ع���ʱ��ȷ�������������ذ�װ�̼��������豸ʱ������û�����
 * Ӱ�죬���緢�������������ȡ�
 */
int HilinkOtaStartProcess(int type);

/*
 * ģ����������
 * status��ʾ�������
 * ��status��100ʱ����ʾ�����ɹ�
 * ��status����100ʱ����ʾ����ʧ��
 * ����ֵ��RETURN_OKʱ����ʾ����ɹ���HiLink SDK����������־���л���������־
 * ����ֵ����RETURN_OKʱ����ʾ�����ɹ���HiLink SDK����ֹ������������
 * ע�⣺HiLink SDK�ڽ��̼�д�뵽OTA����������������У��ͨ���󣬽����ó�������Ĵ˽ӿڣ�
 * ���̿��ڴ˽ӿ�����ɺ�����������صĴ���
 * ������10���ӵ�1Сʱ�����ʱ����һ���Ƿ����°汾��֮���Ե�ǰʱ��Ϊ��㣬23Сʱ��1Сʱ�����ֵ�����Լ���°汾��
 * ����û������Զ��������أ���⵽���°汾�����ǿ�������������£��ͽ����°汾�����أ�������ɺ��Զ�������
 * �Զ��������̿������賿���У���˳�����ʵ������������ع���ʱ��ȷ�������������ذ�װ�̼��������豸ʱ������û�����
 * Ӱ�죬���緢�������������ȣ����������Ƿ�Ϊ�Զ������ɲο��ӿ�HilinkOtaStartProcess�Ĳ���type��������
 */
int HilinkOtaEndProcess(int status);

/*
 * �ж�ģ���Ƿ�����������������
 * ����ֵ��MODULE_CAN_REBOOTʱ����ʾģ���������������������HiLink SDK����ʼ�Զ��������̡�
 * ����ֵ��MODULE_CANNOT_REBOOTʱ����ʾģ�鲻������������������HiLink SDK�������б����Զ��������̡�
 * ע�⣺���û�ͬ���豸�����Զ�����������£�HiLink SDK���ô˽ӿڻ�ȡ�豸��ǰҵ��״̬�£�ģ���Ƿ�������������������ı�־��
 * ֻ�е��豸����ҵ�����״̬ʱ���ӿڲſ��Է���MODULE_CAN_REBOOT��
 * ���豸����ҵ��ǿ���״̬ʱ���ӿڷ���MODULE_CANNOT_REBOOT��
 */
int HilinkGetRebootFlag();

/*
 * HiLink SDK���ó�������Ĵ˽ӿ�֪ͨMCU�̼������״̬
 * flag��ʾ�������̱�־
 * ��flag��START_SEND_DATAʱ����ʾ֪ͨģ�鼴����ʼ����MCU�̼����ݰ�
 * ��flag��STOP_SEND_DATAʱ����ʾ֪ͨģ��������MCU�̼����ѷ������
 * ��flag��SEND_DATA_ERRORʱ����ʾ֪ͨģ�鱾��MCU�̼������쳣��ֹ
 * len��ʾMCU�̼����Ĵ�С
 * type��ʾ��������
 * ��type��UPDATE_TYPE_MANUALʱ����ʾ�����������������û�����������ֶ�����
 * ��type��UPDATE_TYPE_AUTOʱ����ʾ�������������Ǿ����û�ͬ����Զ�����
 * ����ֵ��RETURN_OKʱ����ʾ����ɹ���HiLink SDK�������������������
 * ����ֵ��RETURN_ERRORʱ����ʾ����ʧ�ܣ�HiLink SDK����ֹ����MCU��������
 * ע�⣺��flag��STOP_SEND_DATAʱ���˽ӿ��践��MCU��̼������Ľ������flag������ֵʱ���践�ؽӿڽ��յ�����Ϣ��Ĵ�������
 * ������10���ӵ�1Сʱ�����ʱ����һ���Ƿ����°汾��֮���Ե�ǰʱ��Ϊ��㣬23Сʱ��1Сʱ�����ֵ�����Լ���°汾��
 * ����û������Զ��������أ���⵽���°汾�����ǿ�������������£��ͽ����°汾�����أ�������ɺ��Զ�������
 * �Զ��������̿������賿���У���˳�����ʵ������������ع���ʱ��ȷ�������������ذ�װ�̼��������豸ʱ������û�����
 * Ӱ�죬���緢�������������ȡ�
 */
int HilinkNotifyOtaStatus(int flag, unsigned int len, unsigned int type);

/*
 * HiLink SDK���ó�������Ĵ˽ӿ�֪ͨ���̷���MCU�̼��ļ�����
 * data��ʾ���͵�����
 * len��ʾ���͵����ݵĳ���
 * offset��ʾ���͵�������ʼλ������������̼�����ƫ����
 * �˽ӿ���Ҫ����MCU�����ⲿ�����ݵĴ�����
 * ����ֵ��RETURN_OKʱ��ģ�齫֪ͨ��������ȷ���͸�MCU����MCU��ȷ�����͵����ݣ�HiLink SDK���������������������
 * ����ֵ��RETURN_ERRORʱ��ģ���MCUδ����������֪ͨ��MCU�Ĺ̼��ļ����ݣ�HiLink SDK����ֹ����MCU�̼���������
 */
int HilinkNotifyOtaData(const unsigned char *data, unsigned int len, unsigned int offset);

/*
 * ��ȡ�豸�����ļ�����·��
 * filePath��ʾ�豸�����ļ�����·��
 * len��ʾ�豸�����ļ�����·���ĳ���
 * ����ֵ��RETURN_OKʱ����ʾ��ȡ�ɹ�
 * ����ֵ��RETURN_ERRORʱ����ʾ��ȡʧ��
 * ע�⣺Linuxϵͳ�Ͱ�׿ϵͳ��Ҫ����˽ӿڣ���������ϵͳ��������˽ӿڡ�
 * HiLink SDK���ص��豸�����ļ��豣�����豸�ϵ�ĳ���ļ�·���£�
 * ���ļ�·�������ɳ��������ṩ�����硰/var/update.bin����
 */
int HILINK_GetUpdateFilePath(char *filePath, unsigned int len);

/*
 * ��������˽ӿ��ṩ�豸���ܹ�����������ļ�������С��
 * ���⵱HiLink SDK���ص������ļ���С���ڷ�����Сʱ����������ʧ�ܡ�
 * ע�⣺Linuxϵͳ�Ͱ�׿ϵͳ��Ҫ����˽ӿڣ���������ϵͳ��������˽ӿڡ�
 */
unsigned int HILINK_GetMaxUpdateFileSize(void);

/*
 * ���������װ
 * ����ֵ��RETURN_OKʱ����ʾ�����ɹ�
 * ����ֵ��RETURN_ERRORʱ����ʾ����ʧ��
 * ע�⣺Linuxϵͳ�Ͱ�׿ϵͳ��Ҫ����˽ӿڣ���������ϵͳ��������˽ӿڡ�
 * HiLink SDK����°汾�ļ����������̺��ɳ���ʵ�������װ���̡�
 * HiLink SDK���ô˽ӿں����˳��������̣����ɳ��̵İ�װ�������������İ�װ��
 */
int HILINK_StartSoftwareIntall(void);

/*
 * ��ȡ�������豸�ĵ�ǰ�汾
 * version����ͬһproIdƷ�����Ͱ汾��
 * ����ֵ��RETURN_OKʱ����ʾ��ȡ�ɹ�����֮ʧ��
 */
int HILINK_GetBrgSubDevVersion(const char *proId, unsigned int proIdLen,
    char *version, unsigned int versionLen);

/*
 * HiLink SDK���ó�������Ĵ˽ӿ�֪ͨ�����������豸��hota���°汾��
 * version��ʾͬһproIdƷ������°汾��
 * ����ֵ��RETURN_OKʱ����ʾ��ȡ�ɹ�����֮ʧ��
 */
int HILINK_NotifyBrgSubDevNewVersion(const char *proId, unsigned int proIdLen, const char *version,
    unsigned int versionLen);

/*
 * HiLink SDK���ó�������Ĵ˽ӿ�֪ͨ���̷����������豸�̼�����
 * data��ʾ���͵�����
 * len��ʾ���͵����ݵĳ���
 * offset��ʾ���͵�������ʼλ������������̼�����ƫ����
 * ����ֵ��RETURN_OKʱ����ʾ���ճɹ�����֮ʧ��
 */
int HILINK_NotifyBrgSubDevOtaData(const char *proId, unsigned int proIdLen,
    const unsigned char *data, unsigned int len, unsigned int offset);
/*
 * HiLink SDK���ó�������Ĵ˽ӿ�֪ͨ�������豸�̼������״̬
 * flag��ʾ�������̱�־
 * ��flag��START_SEND_DATAʱ����ʾ֪ͨģ�鼴����ʼ�����������豸�̼����ݰ�
 * ��flag��STOP_SEND_DATAʱ����ʾ֪ͨģ���������������豸�̼����ѷ������
 * ��flag��SEND_DATA_ERRORʱ����ʾ֪ͨģ�鱾���������豸�̼������쳣��ֹ
 * len��ʾMCU�̼����Ĵ�С
 * type��ʾ��������
 * ��type��UPDATE_TYPE_MANUALʱ����ʾ�����������������û�����������ֶ�����
 * ��type��UPDATE_TYPE_AUTOʱ����ʾ�������������Ǿ����û�ͬ����Զ�����
 * ����ֵ��RETURN_OKʱ����ʾ����ɹ���HiLink SDK�������������������
 * ����ֵ��RETURN_ERRORʱ����ʾ����ʧ�ܣ�HiLink SDK����ֹ�����������豸��������
 * ע��:��flag��STOP_SEND_DATAʱ���˽ӿ��践���������豸��̼������Ľ������flag������ֵʱ���践�ؽӿڽ��յ�����Ϣ��Ĵ�������
 * ������10���ӵ�1Сʱ�����ʱ����һ���Ƿ����°汾������û������Զ��������أ���⵽���°汾�����ǿ�������������£�
 * �ͽ����°汾�����أ�������ɺ��Զ�������
 */
int HILINK_NotifyBrgSubDevOtaStatus(const char *proId, unsigned int proIdLen, int flag,
    unsigned int len, unsigned int type);
/*
 * �ϱ���ģ���������豸����������
 * progress�Ϸ�ֵ��[0,100]������5%�����ϱ�������Ƶ���ϱ���
 * ע��:�ýӿ�ֻ����֧�����豸������SDK�汾�е���
 */
void HILINK_ReportBrgSubDevUpdateProgress(const char *proId, unsigned int proIdLen, int progress);
/*
 * �ϱ���ģ���������豸������״̬
 * �ϱ���״̬��SubDevUpdateStatusö����
 * �����ǳɹ�����ʧ�ܣ�����Ҫȷ����������������״̬�ϱ���ģ�顣
 */
void HILINK_ReportBrgSubDevUpdateStatus(const char *proId, unsigned int proIdLen, SubDevUpdateStatus status);

/*
 * �����豸��ota�°汾���
 * ��⵽�İ汾��sdk��ͨ��cb�ص������ϱ����������Ҫ��ȡ�汾�ţ���ɽ�cb�ÿա�
 * �ϱ���version���Ϊ�գ���δ�ɹ���ȡ���°汾��
 * ע��:1���������Ƶ�����һ��һ��
 * 2���״�ͨ���ֻ������ɹ����ܵ��ã��������Ƴɹ�������5s�����
 */
void HILINK_TrigOtaVersionCheck(GetOtaVerCb cb);
#ifdef __cplusplus
}
#endif

#endif