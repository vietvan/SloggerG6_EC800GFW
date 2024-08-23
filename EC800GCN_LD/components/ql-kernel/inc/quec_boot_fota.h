/**  @file
  ql_boot_fota.h

  @brief
  This file is used to define boot spi flash api for different Quectel Project.

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
17/12/2020        Fei         Init version
=================================================================*/


#ifndef QL_BOOT_FOTA_H
#define QL_BOOT_FOTA_H

#include "quec_boot_pff.h"
#include <stdint.h>
#include "ql_api_common.h"

#ifdef __cplusplus
extern "C" {
#endif
#define QL_FILE_MAX_PATH_LEN         (172)

#define QL_FOTA_ERRCODE_BASE (QL_COMPONENT_FOTA<<16)

typedef enum
{
	QUEC_BOOT_FOTA_SUCCESS 					= 0,
	QUEC_BOOT_FOTA_MEM_ERR					= 1|QL_FOTA_ERRCODE_BASE, //动态申请内存错误
	QUEC_BOOT_FOTA_PACK_NAME_ERR			= 2|QL_FOTA_ERRCODE_BASE, //fota更新包名字错误,使用默认fota包文件
	QUEC_BOOT_FOTA_PACK_SIZE_ERR 			= 3|QL_FOTA_ERRCODE_BASE, //fota更新包大小错误
	QUEC_BOOT_FOTA_PACK_READ_ERR			= 4|QL_FOTA_ERRCODE_BASE, //读fota更新包操作错误
	QUEC_BOOT_FOTA_PACK_CRC_ERR				= 5|QL_FOTA_ERRCODE_BASE, //fota更新包CRC校验错误
	
}quec_boot_errcode_fota_e;


/*****************************************************************
* Function: quec_boot_fota_init
*
* Description:
* FOTA initialization operations
* 
* Parameters:
*	NULL
*
* Return:
* 	NULL
*
*****************************************************************/
void quec_boot_fota_init();

/*****************************************************************
* Function: quec_boot_fota_pack_verify
*
* Description:
* FOTA PACK verify operations
* 
* Parameters:
*	NULL
*
* Return:
* 	NULL
*
*****************************************************************/
quec_boot_errcode_fota_e quec_boot_fota_pack_verify(void);

quec_boot_fs_type_e quec_check_path_type(const char *path,int *offset);

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_BOOT_FOTA_H */




