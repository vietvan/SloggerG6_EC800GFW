
/*================================================================
  Copyright (c) 2021, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/
    


#ifndef QL_API_BT_SPP_H
#define QL_API_BT_SPP_H


#ifdef __cplusplus
extern "C" {
#endif

#include "ql_api_bt_common.h"


typedef struct
{
    int state;                      //different from evnet type
    ql_bt_addr_s addr;              //remote address
}ql_bt_spp_event_info_t;

typedef struct
{
    uint8 *buf;
    uint16 dataLen;
} ql_bt_spp_data_info_t;

/*****************************************************************
* Function: ql_bt_spp_init
*
* Description:
*   初始化BT SPP并注册回调函数。初始化会设置BT的mac地址
* 
* Parameters:
*   bt_cb           [in]    回调函数
*
* Return:ql_errcode_bt_e
*
*****************************************************************/
ql_errcode_bt_e ql_bt_spp_init(ql_bt_callback bt_cb);

/*****************************************************************
* Function: ql_bt_spp_connect_req
*
* Description:
*   
* 
* Parameters:
*   bdAddr           [in]    设备地址
*
* Return:ql_errcode_bt_e
*
*****************************************************************/
ql_errcode_bt_e ql_bt_spp_connect(ql_bt_addr_s bdAddr);

/*****************************************************************
* Function: ql_bt_spp_disconnect_req
*
* Description:
*   
* 
* Parameters:
*   
*
* Return:ql_errcode_bt_e
*
*****************************************************************/
ql_errcode_bt_e ql_bt_spp_disconnect(void);

/*****************************************************************
* Function: ql_bt_spp_data_send_req
*
* Description:
*   
* 
* Parameters:
*   
*
* Return:ql_errcode_bt_e
*
*****************************************************************/
ql_errcode_bt_e ql_bt_spp_data_send(uint8 *src_buf, uint16 data_len);

/*****************************************************************
* Function: ql_bt_spp_release
*
* Description:
*   BT SPP释放资源
* 
* Parameters:
*
* Return:ql_errcode_bt_e
*
*****************************************************************/
ql_errcode_bt_e ql_bt_spp_release(void);


#ifdef __cplusplus
    } /*"C" */
#endif
    
#endif /* QL_API_BT_SPP_H */



















