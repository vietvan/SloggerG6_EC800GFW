/**  @file
  ql_api_bt_commom.h

  @brief
  This file is used to define bt api commom for different Quectel Project.
  ql_api_bt.h can use this common define.

*/

/*================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
/*=================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN              WHO         WHAT, WHERE, WHY
------------     -------     -------------------------------------------------------------------------------
=================================================================*/


#ifndef QL_API_BT_COMMON_H
#define QL_API_BT_COMMON_H
#include "ql_api_common.h"

/*========================================================================
 *  Variable Definition
 *========================================================================*/
typedef enum
{
    QL_BT_SUCCESS                   =   0,
    //
    QL_BT_ERROR                 =   1 | (QL_COMPONENT_BSP_BT << 16),
    QL_BT_PENDING,
    QL_BT_BUSY_ERR,
    QL_BT_NO_RESOURCE_ERR,
    QL_BT_NOT_FOUND_ERR,
    QL_BT_DEVICE_NOT_FOUND_ERR,
    QL_BT_CONNECTION_FAILED_ERR,
    QL_BT_TIMEOUT_ERR,
    QL_BT_NO_CONNECTION_ERR,
    QL_BT_INVALID_PARM_ERR,
    QL_BT_NOT_SUPPORTED_ERR,
    QL_BT_CANCELLED_ERR,
    QL_BT_IN_PROGRESS_ERR,
    QL_BT_RESTRICTED_ERR,
    QL_BT_INVALID_HANDLE_ERR,
    QL_BT_PACKET_TOO_SMALL_ERR,
    QL_BT_NO_PAIR_ERR,
    QL_BT_FAILED_ERR,
    QL_BT_DISCONNECT_ERR,
    QL_BT_NO_CONNECT_ERR,
    QL_BT_IN_USE_ERR,
    QL_BT_MEDIA_BUSY_ERR,
    QL_BT_OFF_ERR,

    //蓝牙协议栈返回的错误
    QL_BT_SYS_FAIL_ERR,                     //操作失败
    QL_BT_SYS_DONE_ERR,                     //处于完成状态
    QL_BT_SYS_PENDING_ERR,                  //处于等待状态
    QL_BT_SYS_INVALID_PARAM_ERR,            //输入的参数无效
    QL_BT_SYS_ISECFAIL_ERR,                 //安全连接失败
    QL_BT_SYS_FLOWCTRL_ERR,                 //处于流控状态
    QL_BT_SYS_NORESOURCES_ERR,              //资源不足
    QL_BT_SYS_UNSUPPORTED_ERR,              //设备或能力不支持
    QL_BT_SYS_HW_ERR,                       //蓝牙硬件错误
    QL_BT_SYS_HOST_ERR,                     //蓝牙主机错误
    QL_BT_SYS_UNKOWN_ERR,                   //蓝牙未知错误
    QL_BT_SYS_NOT_READY_ERR,                //处于未就绪状态
    QL_BT_SYS_READY_ERR,                    //处于就绪状态
    QL_BT_SYS_AUTHORISE_FAIL_ERR,           //授权失败
    QL_BT_SYS_AUTHEN_FAIL_ERR,              //认证失败
    QL_BT_SYS_ENCRYPT_FAIL_ERR,             //加密失败
    QL_BT_SYS_TIMEOUT_ERR,                  //操作超时
    QL_BT_SYS_PROTOCOL_ERR,                 //协议错误
    QL_BT_SYS_DISALLOWED_ERR,               //操作不允许
    QL_BT_SYS_BUFF_SAMLL_ERR,               //蓝牙协议栈buffer太小
    QL_BT_SYS_DISCONNECT_ERR,               //处于未连接
    QL_BT_SYS_MORE_ERR,                     //更多错误

    QL_BT_REGISTER_MSG_ERR,
    QL_BT_DISCONNECT,
    QL_BT_ALREADY_CONNECTED_ERR,            //已经建立了连接
    QL_BT_ALREADY_STARTED_ERR,
    QL_BT_ALREADY_STOPED_ERR,
    QL_BT_NOT_START_ERR,
    QL_BT_NOT_STOP_ERR,
    QL_BT_PRAMA_TYPE_ERR,
    QL_BT_PRAMA_LENGTH_ERR,
    QL_BT_PRAMA_DATA_ERR,                   //BT/BLE input parameter error
    QL_BT_PRAMA_SIZE_ERR,
    QL_BT_PRAMA_NULL_ERR,
    QL_BT_NOT_INQURY_ERR,
    QL_BT_SPP_NOT_CONNECT_ERR,
    QL_BT_SPP_NOT_SUPPORT_ERR,
    QL_BT_GET_PAIR_INFO_ERR,
    QL_BT_REMVOE_PAIR_INFO_ERR,
    QL_BT_GET_WHITELIST_ERR,
    QL_BT_MEM_ADDR_NULL_ERR,
    QL_BT_GET_FILE_ADDR_ERR,
    QL_BT_WIFI_USING_ERR,
    QL_BT_NOT_INIT_ERR,
    QL_BT_NOT_REGISTER_CB_ERR,
    QL_BT_TASK_NOT_EXIT_ERR,
    QL_BT_IS_USING_ERR,
    QL_BT_ALREADY_REGISTERED_ERR,
    QL_BT_SERVICE_ALREADY_EXITED_ERR,
    QL_BT_SERVICE_NOT_EXITED_ERR,
    QL_BT_SERVICE_NOT_CLEAN_ERR,                    //服务没有清除，service not clean
    QL_BT_CHARA_ALREADY_EXITED_ERR,
    QL_BT_CHARA_NOT_EXITED_ERR,
    QL_BT_NOT_CONN_ID_ERR,
    QL_BLE_PUBLIC_ADDR_NOT_USED_ERR,                //cannot use ble public address
    QL_BLE_CID_ERR,                                 //没有对应cid的通道
    QL_BLE_HANDLE_ERR,                              //使用了错误的句柄
    QL_BLE_PROP_ERR,                                //属性错误
    QL_BLE_WRITE_NAME_FILE_ERR,                     //写蓝牙名称文件错误
    QL_BLE_READ_NAME_FILE_ERR,                      //读蓝牙名称文件错误
    QL_BLE_READ_ADDR_FILE_ERR,                      //read mac address file err
    QL_BTE_IN_UPGRAD_MOAE_ERR,                      //蓝牙正在升级错误
    //bt filter error
    QL_BT_FILTER_PARAM_ERR = 500 | (QL_COMPONENT_BSP_BT << 16),             //bt filter:param error
}ql_errcode_bt_e;

#define QL_BT_MAC_ADDRESS_SIZE              6
#define QL_BLE_ADV_SCAN_DATA_SIZE           31                              //BLE advertise and scan data max length
#define QL_BLE_EXP_ADV_SCAN_DATA_SIZE       255                             //BLE expand advertise and scan data max length

//bt/ble address
typedef struct
{
    unsigned char addr[QL_BT_MAC_ADDRESS_SIZE];
} ql_bt_addr_s;

//connection parameter
typedef struct
{
    unsigned short conn_id;                         //connection id
    unsigned short max_interval;                    //最大的间隔，间隔：1.25ms，取值范围：6-3200，时间范围：7.5ms~4s
    unsigned short min_interval;                    //最小的间隔，间隔：1.25ms，取值范围：6-3200，时间范围：7.5ms~4s
    unsigned short latency;                         //从机忽略连接状态事件的时间。需满足：（1+latecy)*max_interval*2*1.25<timeout*10
    unsigned short timeout;                         //没有交互，超时断开时间，间隔：10ms，取值范围：10-3200，时间范围：100ms~32s
}ql_ble_update_conn_infos_s;

//ble gatt uiid information
#define QL_BLE_LONG_UUID_SIZE                   16
typedef enum 
{
    QL_BLE_GATT_LONG_UUID = 0,                      //长UUID：128bit
    QL_BLE_GATT_SHORT_UUID,                         //短UUID：16bit
}ql_ble_gatt_uuid_state_e;

typedef struct
{
    ql_ble_gatt_uuid_state_e uuid_type;
    unsigned char uuid_l[QL_BLE_LONG_UUID_SIZE];
    unsigned short uuid_s; 
}ql_ble_gatt_uuid_s;

#define QL_BT_BLE_NAME_MAX_SIZE             22      //now is 22,will be 29 in future

typedef enum
{
    QL_BT_BLE_NAME_CODE_UTF8 = 0,                   //code utf8
    QL_BT_BLE_NAME_CODE_GBK ,                       //code GBK
}ql_bt_ble_name_code_type_e;
//bt/ble local name
typedef struct
{                             
    unsigned char name[QL_BT_BLE_NAME_MAX_SIZE+1];  //蓝牙名称实际长度，最大长度为22.end with \0
    ql_bt_ble_name_code_type_e code_type;
}ql_bt_ble_local_name_s;

//BLE IBEACON信息
typedef struct
{
    unsigned char uuid_l[QL_BLE_LONG_UUID_SIZE];
    unsigned short major;
    unsigned short minor;
}ql_ble_ibeacon_cfg_s;

//bt/ble connection information
typedef enum 
{
    QL_BT_BLE_CONN_DISCONNECTED = 0,                       //ble is disconnected
    QL_BT_BLE_CONN_CONNECTED,                              //ble is connected
}ql_bt_ble_connection_state_e;

typedef struct
{
    unsigned short conn_id;
    ql_bt_ble_connection_state_e conn_state;
    ql_bt_addr_s addr;
}ql_bt_ble_connection_info_s;


typedef struct
{
    ql_bt_addr_s addr;
    unsigned char rssi;
    unsigned char device_name[QL_BT_BLE_NAME_MAX_SIZE+1];
    unsigned int  device_class;
    unsigned char status;
}ql_bt_inquiry_info_s;

typedef struct
{
    ql_bt_addr_s addr;
    unsigned char device_name[QL_BT_BLE_NAME_MAX_SIZE+1];
    unsigned char name_len;
    unsigned int  value;
    unsigned char state;
}ql_bt_bond_info_s;


/*========================================================================
 *  function Definition
 *========================================================================*/

#endif /* QL_API_BT_COMMON_H */

