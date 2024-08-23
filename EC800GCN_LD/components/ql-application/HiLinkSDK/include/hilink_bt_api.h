/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: ����SDK APIͷ�ļ�
 */
#ifndef HILINK_BT_API_H
#define HILINK_BT_API_H

#include "ble_cfg_net_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/* �㲥��󳤶� */
#define ADV_VALUE_MAX_LEN 31

/* ��������Э��ջ��־ */
#define BLE_DEINIT_FALG 0x1234ABCD

typedef enum {
    HILINK_BT_SDK_STATUS_SVC_RUNNING = 0, /* �������� */
    HILINK_BT_SDK_STATUS_DEINIT, /* ע�� */
    HILINK_BT_SDK_STATUS_NAME_SET_ABNORM, /* �������������쳣 */
    HILINK_BT_SDK_STATUS_DISCOVER_MODE_SET_ABNORM, /* �����ɷ���ģʽ�����쳣 */
    HILINK_BT_SDK_STATUS_REG_APP_ABNORM, /* ע��BLEӦ���쳣 */
    HILINK_BT_SDK_STATUS_SVC_CREATE_ABNORM, /* ���񴴽��쳣 */
    HILINK_BT_SDK_STATUS_CHAR_ADD_ABNORM, /* ��������쳣 */
    HILINK_BT_SDK_STATUS_DESC_ADD_ABNORM, /* ��������쳣 */
    HILINK_BT_SDK_STATUS_SVC_START_ABNORM, /* ���������쳣 */
    HILINK_BT_SDK_STATUS_ADV_PARA_SET_ABNORM, /* �㲥���������쳣 */
    HILINK_BT_SDK_STATUS_ADV_DATA_SET_ABNORM, /* �㲥���������쳣 */
    HILINK_BT_SDK_STATUS_ADV_START_ABNORM, /* �㲥�����쳣 */
} HILINK_BT_SdkStatus;

/* GATTS char����ȡֵ */
typedef enum {
    HILINK_BT_CHAR_PROP_WRITE = 0x08,
    HILINK_BT_CHAR_PROP_READ = 0x02,
    HILINK_BT_CHAR_PROP_NOTIFY = 0x10,
    HILINK_BT_CHAR_PROP_INDICATE = 0x20
} HILINK_BT_CharProperty;

/* GATTS charȨ��ȡֵ */
typedef enum {
    HILINK_BT_CHAR_PERM_READ  = 0x01,
    HILINK_BT_CHAR_PERM_READ_ENCRYPTED = 0x02,
    HILINK_BT_CHAR_PERM_READ_ENCRYPTED_MITM = 0x04,
    HILINK_BT_CHAR_PERM_WRITE = 0x10,
    HILINK_BT_CHAR_PERM_WRITE_ENCRYPTED = 0x20,
    HILINK_BT_CHAR_PERM_WRITE_ENCRYPTED_MITM = 0x40,
    HILINK_BT_CHAR_PERM_WRITE_SIGNED = 0x80,
    HILINK_BT_CHAR_PERM_WRITE_SIGNED_MITM = 0x100,
} HILINK_BT_CharPermission;

/* GATTS desc����ȡֵ */
typedef enum {
    HILINK_BT_DESC_PERM_WRITE = 0x01,
    HILINK_BT_DESC_PERM_READ = 0x02
} HILINK_BT_DescPermission;

/* ����ֵ����: ���ͺ����� */
typedef enum {
    HILINK_BT_CMD_DATA_TYPE_INT,
    HILINK_BT_CMD_DATA_TYPE_STR,
} HILINK_BT_CmdDataType;

/* hilink����Ӧ�ò����ݱ������� */
typedef enum {
    HILINK_BT_CMD_DATA_MODE_TLV = 0x00, /* TLV��ʽ: ���ͱ���ռ�õĿռ� */
    HILINK_BT_CMD_DATA_MODE_JSON = 0x01 /* JSON��ʽ: ��չ�Ը��ã�Ĭ�ϸ�ʽ */
} HILINK_BT_CmdDataMode;

/* ��������SDK״̬�ص��������� */
typedef void (*HILINK_BT_SdkEventCallBack)(HILINK_BT_SdkStatus event, const void *param);

/* �����������ݻص��������� */
typedef int (*HILINK_BT_SendBtDataCallback)(const unsigned char *buf, unsigned int len);

/* �Զ���gatt������¼��ص� */
typedef int (*HILINK_BT_GattReadCallback)(unsigned char *out, unsigned int *outLen);

/* �Զ���gatt����д�¼��ص� */
typedef int (*HILINK_BT_GattWriteCallback)(const unsigned char *in, unsigned int inLen);

/* ��ȡ����mac��ַ�Ļص����� */
typedef int (*HILINK_BT_GetBtMacCallBack)(const unsigned char *buf, unsigned int len);

/* ��Ʒ��Ϣ����ṹ�� */
typedef struct {
    char *manuName;
    char *devName;
    char *productId;
    char *sn;
    char *mac;
    char *subProductId;
} HILINK_BT_DevInfo;

/* ��Ʒ���������ṹ�� */
typedef struct {
    unsigned char attrIdx;
    char *attr;
    HILINK_BT_CmdDataType dataType;
    int (*putFunc)(const void *data, unsigned int len);
    int (*getFunc)(void *buf, unsigned int *bufLen, unsigned int len);
} HILINK_BT_AttrInfo;

/* ��Ʒ���ܶ���ṹ�� */
typedef struct {
    unsigned char svcIdx;
    char *service;
    int (*putFunc)(const void *svc, const unsigned char *in, unsigned int inLen,
        unsigned char **out, unsigned int *outLen);
    int (*getFunc)(const void *svc, const unsigned char *in, unsigned int inLen,
        unsigned char **out, unsigned int *outLen);
    unsigned char attrNum;
    HILINK_BT_AttrInfo *attrInfo;
} HILINK_BT_SvcInfo;

/* ��ƷProfile����ṹ�� */
typedef struct {
    unsigned int svcNum;
    HILINK_BT_SvcInfo *svcInfo;
} HILINK_BT_Profile;

/* ����gatt character���� */
typedef struct {
    char *descUuid;
    /* gatt����������дȨ�ޣ�ȡֵ��HILINK_BT_DescPermission���͵ĳ�Ա������ó� */
    unsigned int descPermission;
} HILINK_BT_GattProfileDesc;

/* ����gatt character */
typedef struct {
    char *charUuid;
    /* gatt charȨ��:ȡֵ��HILINK_BT_CharPermission���͵ĳ�Ա������ó� */
    unsigned int charPermission;
    /* gatt char����:ȡֵ��HILINK_BT_CharProperty���͵ĳ�Ա������ó� */
    unsigned int charProperty;
    HILINK_BT_GattReadCallback readFunc;
    HILINK_BT_GattWriteCallback writeFunc;
    HILINK_BT_GattProfileDesc *desc;
    unsigned char descNum;
} HILINK_BT_GattProfileChar;

/* ����gatt ���� */
typedef struct {
    char *svcUuid;
    int isPrimary;
    HILINK_BT_GattProfileChar *character;
    unsigned char charNum;
} HILINK_BT_GattProfileSvc;

/* �����Զ�������gatt�����б� */
typedef struct {
    HILINK_BT_GattProfileSvc *service;
    unsigned char serviceNum;
} HILINK_BT_GattServiceList;

/* ���ñ���ص��ṹ�� */
typedef struct {
    int (*createItem)(const char *name, unsigned int size);
    int (*readItem)(const char *name, unsigned char *buf, unsigned int len);
    int (*writeItem)(const char *name, const unsigned char *buf, unsigned int len);
    int (*deleteItem)(const char *name);
} HILINK_BT_ConfigInterface;

/* ��ȡ�㲥���ݽṹ�� */
typedef struct {
    unsigned int advSvcDataLen;
    unsigned char advSvcData[ADV_VALUE_MAX_LEN];
    unsigned int advRspDataLen;
    unsigned char advRspData[ADV_VALUE_MAX_LEN];
} HILINK_BT_AdvertiseData;

/* ����Ӧ�ò����ģʽ */
int HILINK_BT_SetEncodeMode(HILINK_BT_CmdDataMode mode);

/* ��ѯӦ�ò����ģʽ */
HILINK_BT_CmdDataMode HILINK_BT_GetEncodeMode(void);

/* ��ʼ������HiLink Bluetooth SDK */
int HILINK_BT_Init(const HILINK_BT_Profile *profile);

/* ����HiLink BT SDK��������HiLinkЭ��ջ */
int HILINK_BT_Process(void);

/*
 * ����HiLink Bluetooth SDK
 * flagΪ0��ֻ���ٿ��ƺ͵����̣߳�flagΪ1��������Э��ջ���ú�����������
 */
int HILINK_BT_DeInit(unsigned int flag);

/* ���HiLink������Ϣservice��Ϣ */
int HILINK_BT_AddHiLilnkService(const HILINK_BT_SvcInfo *serviceArray, unsigned int serviceNum);

/* ֪ͨ����״̬ */
int HILINK_BT_ReportServiceState(const void *service, const void *buf, unsigned int len);

/* ֪ͨ����״̬ */
int HILINK_BT_ReportAttrState(const void *svc, const void *attr, const void *buf, unsigned int len);

/* ������������ */
int HILINK_BT_ProcessBtData(const unsigned char *buf, unsigned int len);

/* ע���������ݷ��ͽӿ� */
int HILINK_BT_RegisterBtDataSendCallback(HILINK_BT_SendBtDataCallback callback);

/* ��ѯ�������ݷ��ͽӿ� */
HILINK_BT_SendBtDataCallback HILINK_BT_GetBtDataSendCallback(void);

/* ��ʼ����������mac��ַ�ĺ��� */
int HILINK_BT_BtMacSendInit(HILINK_BT_GetBtMacCallBack callback);

/* ��ѯ����mac��ַ�ķ��ͽӿ� */
HILINK_BT_GetBtMacCallBack HILINK_BT_GetBtMacSendCallback(void);

/* ��������SDK�¼������� */
int HILINK_BT_SetSdkEventCallback(HILINK_BT_SdkEventCallBack callback);

/*
 * ����BLE�����������
 * ���connNum�ķ�ΧΪ[1,10]
 * �������������Ϊ10������10����10��ִ��
 * ��С������Ϊ1��С��1��1��ִ��
 * �������øýӿڣ�Ĭ�����������Ϊ1
 */
void HILINK_BT_SetMaxConnNum(int connNum);

/* ��ѯ����SDK����������� */
int HILINK_BT_GetMaxConnNum(void);

/* �������SDK�Զ���gatt���� */
int HILINK_BT_SetGattProfile(HILINK_BT_GattServiceList *gattServiceList);

/* ע�����ñ���ص�������HiLink Bluetooth SDK���������øú�������Ĭ��ʹ��HiLink Bluetooth SDK��������ʵ�� */
int HILINK_BT_RegisterConfigInterface(const HILINK_BT_ConfigInterface *interface);

/* �����㲥 */
int HILINK_BT_StartAdvertise(void);

/* ֹͣ�㲥 */
int HILINK_BT_StopAdvertise(void);

/* �ϱ������������� */
int HILINK_BT_IndicateSvcCharData(const char *svcUuid, const char *charUuid, const char *buf, unsigned int len);

/*
 * ��ȡ�豸�������ǿ���źŷ��书��ǿ�ȣ���ǿ��λ�õ�ȷ���Լ����ʲ��Է�
 * ��������hilink��֤�����������ֹ������ü����Է���ָ���ĵ���powerΪ����
 * ����λdbm�������豸�������ǿ�ź�ǿ��ֵ��������̲���ʹ�������������ֹ�
 * �ܣ��ӿ�ֱ�ӷ�-1�������Ҫʹ�������������֣���ӿڷ���0�����輰ʱ��Ч����
 * ����HILINK_BT_StartAdvertise()���������㲥
 */
int HILINK_BT_GetDevSurfacePower(char *power);

/*
 * ��ȡ�豸�����ͺţ����ȹ̶������ֽ�
 * subProdIdΪ�������ͺŵĻ�������lenΪ�������ĳ���
 * �����Ʒ���������ͺţ����������ֽ����ͺţ�����'\0'����, ����0
 * û�ж������ͺţ��򷵻�-1
 * �ýӿ����豸������ʵ��
 * ��hilink sdk��ͬ���壬˫ģ��ģʽֻ��һ�ݣ����ṩ�����������ң��ݲ�����̹淶����
 */
int HILINK_GetSubProdId(char *subProdId, int len);

/*
 * ��ȡ�豸SN
 * ��hilink sdk��ͬ���壬˫ģ��ģʽֻ��һ�ݣ����ṩ�����������ң��ݲ�����̹淶����
 */
void HilinkGetDeviceSn(unsigned int len, char *sn);

/* ��ȡ����SDK�豸�����Ϣ */
HILINK_BT_DevInfo *HILINK_BT_GetDevInfo(void);

/* ��ȡ���������й㲥���� */
int HILINK_BT_GetAdvertiseData(HILINK_BT_AdvertiseData *advertiseData);

/* ��ȡBLE����ע��Ļص����� */
BLE_CfgNetCb *GetBleCfgNetCallback(void);

/* �Ͽ���������ʱ���ͷ�ͨ��ռ����Դ */
void HILINK_BT_DisconnectFreeResource(void);

/*
 * ��������linuxϵͳ��д�ļ���·��,·�����Ȳ��ܳ���33���ֽ�,����������
 * Ҫ��洢·���Ƿ���ʧ�Եģ��汾������Ӱ���·���ļ�
 */
void HILINK_BT_SetConfigDirPath(const char *configDirPath);

#ifdef __cplusplus
}
#endif
#endif
