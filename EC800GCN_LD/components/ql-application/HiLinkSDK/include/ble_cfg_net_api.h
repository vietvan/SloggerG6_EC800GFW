/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: BLE��������SDK APIͷ�ļ�
 */

#ifndef BLE_CFG_NET_API_H
#define BLE_CFG_NET_API_H

#ifdef __cplusplus
extern "C" {
#endif

/* WIFI��Ϣ���Ⱥ궨�� */
#define WIFI_SSID_MAX_LEN 32
#define WIFI_PWD_MAX_LEN 64
#define WIFI_PSK_LEN 32
#define WIFI_BSSID_LEN 6

#define BLE_UUID_LEN 16

/* �û����͵��������� */
typedef enum {
    NETCFG_DATA,
    CUSTOM_DATA,
    DATA_TYPE_BUTT
} BLE_DataType;

/* �������Ͷ��� */
typedef enum {
    ATTR_TYPE_SERVICE = 0,
    ATTR_TYPE_CHAR,
    ATTR_TYPE_CHAR_VALUE,
    ATTR_TYPE_CHAR_CLIENT_CONFIG,
    ATTR_TYPE_CHAR_USER_DESC,
} BLE_AttrType;

/* UUID���ȶ��� */
typedef enum {
    UUID_TYPE_NULL = 0,
    UUID_TYPE_16_BIT,
    UUID_TYPE_32_BIT,
    UUID_TYPE_128_BIT,
} BLE_UuidType;

/* BLE��������״̬���� */
typedef enum {
    CFG_NET_PROCESS_SUCCESS = 0x00,
    CFG_NET_BLE_CONNECT,
    CFG_NET_BLE_DIS_CONNECT,
    CFG_NET_SPEKE_SUCCESS,
    CFG_NET_PROCESS_START,
    CFG_NET_RECEIVE_PARA,
    CFG_NET_WIFI_CONNECT,
    CFG_NET_FAIL_UNKUNOWN = 0x100,
    CFG_NET_FAIL_WIFI_SSID,
    CFG_NET_FAIL_WIFI_PWD
} BLE_CfgNetStatus;

/* �ֻ��ഫ������WIFI��Ϣ����ʽ���ö̾�������ʽ */
typedef struct {
    unsigned char ssid[WIFI_SSID_MAX_LEN + 1];
    unsigned char pwd[WIFI_PWD_MAX_LEN + 1];
    unsigned char psk[WIFI_PSK_LEN + 1];
    unsigned char bssid[WIFI_BSSID_LEN + 1];
    unsigned char ssidLen;
    unsigned char pwdLen;
    unsigned char pskLen;
    unsigned char bssidLen;
    int authMode;
    int wifiInfoSrc;
    int channelNumber;
} BLE_WifiInfo;

/*
 * ��ȡ�豸PIN�뺯������
 * pincode: ���pin��Ļ�����
 * size: �������ĳ���
 * len: ���ص�pin��ʵ�ʳ���
 */
typedef int (*BLE_GetDevPinCode)(unsigned char *pinCode, unsigned int size, unsigned int *len);

/*
 * ��ȡ�豸��Ϣ�������ͣ�len�������Ҳ�ǳ��Σ���δ���buff���������ȣ����δ����ȡ���豸��Ϣʵ�ʳ���
 * ��ʽҪ��{"productId":"%s", "sn":"%s", "vendor":"%s"}
 */
typedef int (*BLE_GetDeviceInfo)(unsigned char *devInfo, unsigned int *len);

/*
 * ����������Ϣ��������
.* ���ݸ�ʽ��{"ssid":"%s","password":"%s","devId":"%s","psk":"%s","code":"%s","random":"%s","vendorData":"%s"}
 */
typedef int (*BLE_SetCfgNetInfo)(const unsigned char *netInfo, unsigned int len);

/* �����û����ݺ������� */
typedef int (*BLE_RcvCustomData)(unsigned char *buff, unsigned int len);

/* ��������״̬���������� */
typedef int (*BLE_CfgNetProcess)(BLE_CfgNetStatus status);

/* BLE GATT������������� */
typedef int (*BLE_GattRead)(unsigned char *buff, unsigned int *len);

/* BLE GATT����д�������� */
typedef int (*BLE_GattWrite)(const unsigned char *buff, unsigned int len);

/* BLE GATT����ָʾ�������� */
typedef int (*BLE_GattIndicate)(unsigned char *buff, unsigned int len);

/* BLE GATT�ص�����ָ�� */
typedef struct {
    BLE_GattRead readCb;
    BLE_GattWrite writeCb;
    BLE_GattIndicate indicateCb;
} BLE_GattOperateFunc;

/* BLE���ò��� */
typedef struct {
    int isBlePair;
    int isDeinitBleStack;
    int data1; /* Ϊ�������ò���Ԥ�����ݲ�ʹ�� */
    int data2; /* Ϊ�������ò���Ԥ�����ݲ�ʹ�� */
    int data3; /* Ϊ�������ò���Ԥ�����ݲ�ʹ�� */
} BLE_ConfPara;

/* BLE GATT���� */
typedef struct {
    BLE_AttrType attrType;
    unsigned int permission;
    BLE_UuidType uuidType;
    unsigned char uuid[BLE_UUID_LEN];
    unsigned char *value;
    unsigned char valLen;
    unsigned char properties;
    BLE_GattOperateFunc func;
} BLE_GattAttr;

/* GATT����(����service�����¹ҵ�ȫ��characteristics��descriptions) */
typedef struct {
    unsigned int attrNum;
    BLE_GattAttr *attrList;
} BLE_GattService;

/* GATT�б�(�������services�ͷ��ص�handle) */
typedef struct {
    unsigned int num;
    BLE_GattService *service;
    int *handle;
} BLE_GattList;

/* GATT����б� */
typedef struct {
    unsigned int num;
    int *handle;
} BLE_GattHandleList;

/* BLE�Ĺ㲥���ݺ�ɨ��Ӧ������ */
typedef struct {
    unsigned char *advData;
    unsigned int advDataLen;
    unsigned char *rspData;
    unsigned int rspDataLen;
} BLE_AdvData;

/* BLE�Ĺ㲥���� */
typedef struct {
    unsigned char advType;
    unsigned char discMode;
    unsigned char connMode;
    unsigned int minInterval;
    unsigned int maxInterval;
    unsigned int channelMap;
    unsigned int timeout;
    int txPower;
} BLE_AdvPara;

/* �㲥���������� */
typedef struct {
    BLE_AdvPara *advPara;
    BLE_AdvData *advData;
} BLE_AdvInfo;

/* BLE��ʼ������ */
typedef struct {
    BLE_ConfPara *confPara;
    BLE_AdvInfo *advInfo;
    BLE_GattList *gattList;
} BLE_InitPara;

/* BLE�����ص����� */
typedef struct {
    BLE_GetDevPinCode getDevPinCodeCb;
    BLE_GetDeviceInfo getDeviceInfoCb;
    BLE_SetCfgNetInfo setCfgNetInfoCb;
    BLE_RcvCustomData rcvCustomDataCb;
    BLE_CfgNetProcess cfgNetProcessCb;
} BLE_CfgNetCb;

/* BLE������Դ���룺BLEЭ��ջ�����������ص������ҽ� */
int BLE_CfgNetInit(BLE_InitPara *para, BLE_CfgNetCb *cb);

/*
 * BLE������Դע���������ص���������BLEЭ��ջ����
 * flagΪ0��ֻ���ٿ��ƺ͵����̣߳�flagΪ1��������Э��ջ���ú�����������
 */
int BLE_CfgNetDeInit(const BLE_GattHandleList *handleList, unsigned int flag);

/* BLE�����㲥���ƣ���������㲥ʱ�䣬0:ֹͣ��0xFFFFFFFF:һֱ�㲥���������㲥ָ��ʱ���ֹͣ����λ�� */
int BLE_CfgNetAdvCtrl(unsigned int advSecond);

/*
 * ���¹㲥������������ɺ������BLE_CfgNetAdvCtrl�����㲥
 * �����ֵʱ������hilink����Ĺ㲥
 */
int BLE_CfgNetAdvUpdate(const BLE_AdvInfo *advInfo);

/* BLE�����Ͽ����ӣ���ֹ��������ʱ��ռ��BLE���� */
int BLE_CfgNetDisConnect(void);

/* BLE�����û����ݣ��û����ݷ��ͣ�����ջص���������ʹ�� */
int BLE_SendCustomData(BLE_DataType dataType, const unsigned char *buff, unsigned int len);

#ifdef __cplusplus
}
#endif
#endif
