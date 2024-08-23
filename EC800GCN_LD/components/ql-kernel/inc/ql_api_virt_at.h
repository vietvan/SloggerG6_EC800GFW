/**  @file
  ql_api_virt_at.h

  @brief
  This file is used to define virt at interface for different Quectel Project.

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
26/01/2021        Neo         Init version
=================================================================*/


#ifndef _QL_API_VIRT_AT_H_
#define _QL_API_VIRT_AT_H_

#include "ql_api_common.h"

#ifdef __cplusplus
	 extern "C" {
#endif


/*===========================================================================
 * Macro Definition
 *===========================================================================*/
#define QL_VIRT_AT_ERRCODE_BASE (QL_COMPONENT_BSP_VIRT_AT<<16)


/*========================================================================
 *  Enumeration Definition
 *========================================================================*/
/**
 * virt at errcode
 */
typedef enum
{
    QL_VIRT_AT_SUCCESS = QL_SUCCESS,

    QL_VIRT_AT_INVALID_PARAM_ERR                 = 1|QL_VIRT_AT_ERRCODE_BASE,
    QL_VIRT_AT_EXECUTE_ERR,			//虚拟AT操作失败(内存不足等)
    QL_VIRT_AT_OPEN_REPEAT_ERR,		//虚拟AT通道重复打开
    QL_VIRT_AT_OPEN_DEVICE_ERR,		//虚拟AT通道打开失败
    QL_VIRT_AT_CREATE_PIPE_ERR,		//虚拟AT创建FIFO失败
    QL_VIRT_AT_PUSH_LIST_ERR,		//暂未使用
    QL_VIRT_AT_CREATE_MUTEX_ERR,	//虚拟创建互斥锁失败
    QL_VIRT_AT_MUTEX_TIMEOUT_ERR,	//虚拟AT互斥锁超时
    QL_VIRT_AT_CREATE_DISPATCH_ERR,	//虚拟AT创建数据结构时失败
    QL_VIRT_AT_NOT_INIT,			//虚拟AT功能未初始化
    QL_VIRT_AT_TX_OVERFOLW,			//虚拟AT  的TX FIFO空间不足
}ql_errcode_virt_at_e;

/**
 * virtual at channel enum
 */
typedef enum 
{
    QL_VIRT_AT_PORT_0,
	QL_VIRT_AT_PORT_1,
	QL_VIRT_AT_PORT_2,
	QL_VIRT_AT_PORT_3,
    QL_VIRT_AT_PORT_4,
    QL_VIRT_AT_PORT_5,
    QL_VIRT_AT_PORT_6,
    QL_VIRT_AT_PORT_7,
	QL_VIRT_AT_PORT_8,
	QL_VIRT_AT_PORT_9,    
	QL_VIRT_AT_PORT_MAX,
}ql_virt_at_port_number_e;

/*===========================================================================
 * Functions declaration
 ===========================================================================*/
/*****************************************************************
* Description: VIRT AT 回调函数，用于接收AT命令返回
* 
* Parameters:
*   ind_type    [in]    事件类型，VIRT      AT RX只有一种事件。 
*   size        [in]    数据大小
*
*****************************************************************/
typedef void (*ql_virt_at_callback)(uint32 ind_type, uint32 size);


/*****************************************************************
* Function: ql_virt_at_read
*
* Description: VIRT AT读数据
* 
* Parameters:
*   port	    [in] 	虚拟AT通道 
* 	data	    [out] 	读取的数据 
*   data_len    [in]    要读取的数据长度
*
* Return:
*   < 0         错误码,详见ql_errcode_virt_at_e
*	other 	    实际读取到的数据长度
*
*****************************************************************/
int ql_virt_at_read(ql_virt_at_port_number_e port, unsigned char *data, unsigned int data_len);

/*****************************************************************
* Function: ql_virt_at_write
*
* Description: VIRT AT写数据
* 
* Parameters: 
*   port	    [in] 	虚拟AT通道 
* 	data	    [in] 	要写入的数据 
*   data_len    [in]    要写入的数据长度
*
* Return:
*   < 0         错误码,详见ql_errcode_virt_at_e
*	other 	    实际写入的数据长度
*
*****************************************************************/
int ql_virt_at_write(ql_virt_at_port_number_e port, unsigned char *data, unsigned int data_len);


/*****************************************************************
* Function: ql_virt_at_close
*
* Description: 关闭 VIRT AT通道
* 
* Parameters: 
*   port	    [in] 	虚拟AT通道 
*
* Return:
* 	0			关闭成功。
*	other 	    错误码。
*
*****************************************************************/
ql_errcode_virt_at_e ql_virt_at_close(ql_virt_at_port_number_e port);

/*****************************************************************
* Function: ql_virt_at_open
*
* Description: 打开 VIRT AT通道,注册回调函数
* 
* Parameters:
*   port	    [in] 	虚拟AT通道
*   virt_at_cb	[in] 	回调函数 
*
* Return:
* 	0			打开成功。
*	other 	    错误码。
*
*****************************************************************/
ql_errcode_virt_at_e ql_virt_at_open(ql_virt_at_port_number_e port, ql_virt_at_callback virt_at_cb);

/*****************************************************************
* Function: ql_virt_at_read_aviable
*
* Description: 获取虚拟AT通道中的可读字节数
* 
* Parameters:
*   port	    [in] 	虚拟AT通道
*
* Return:
* 	>=0			可读字节数
*	other 	    错误码。
*
*****************************************************************/
int ql_virt_at_read_aviable(ql_virt_at_port_number_e port);

/*****************************************************************
* Function: ql_virt_at_write_aviable
*
* Description: 获取虚拟AT通道中的可写字节数
* 
* Parameters:
*   port	    [in] 	虚拟AT通道
*
* Return:
* 	>=0			可写字节数
*	other 	    错误码。
*
*****************************************************************/
int ql_virt_at_write_aviable(ql_virt_at_port_number_e port);



#ifdef __cplusplus
	 } /*"C" */
#endif
 
#endif /* _QL_API_VIRT_AT_H_ */


