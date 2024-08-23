/**  @file
  quec_boot_pff.h

  @brief
  This file is used to define boot uart api for different Quectel Project.

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
24/01/2021        Neo         Init version
=================================================================*/

#ifndef QUEC_BOOT_PFF_H
#define QUEC_BOOT_PFF_H

#include "quec_proj_config.h"

#ifdef __cplusplus
extern "C" {
#endif


/*===========================================================================
 * Macro Definition
 ===========================================================================*/


/*===========================================================================
 * Enum
 ===========================================================================*/
typedef enum
{
    QUEC_BOOT_FS_NONE            = -1,

    QUEC_BOOT_SFFS_EXT           = 0,
	QUEC_BOOT_SFFS_SPI4EXTNOR	 = 1,
	QUEC_BOOT_FAT_SDMMC 		 = 2,
	QUEC_BOOT_FAT_EXNAND_FLASH,
	QUEC_BOOT_FAT_EXTNOR_FLASH
}quec_boot_fs_type_e;

typedef enum
{
	QL_BOOT_FAT_OPEN_READ_ONLY		= 0, 	/* Open in read-only mode */
	QL_BOOT_FAT_OPEN_CREATE			= 1,	/* Open in create mode    */
											/* if the file already exists, it will be overwritten*/
}ql_boot_fat_open_mode_e;

/* the partition number of mount    */
typedef enum
{
	QL_BOOT_FAT_MOUNT_PARTITION_1	= 0,	/* partition 1 */
	QL_BOOT_FAT_MOUNT_PARTITION_2	= 1,	/* partition 2 */
	QL_BOOT_FAT_MOUNT_PARTITION_MAX	= 2,	
}ql_boot_fat_mount_pt_e;

/*===========================================================================
 * Functions declaration
 ===========================================================================*/
void quec_get_real_path(const char* path,char** realpath);

int ql_boot_fat_mount(ql_boot_fat_mount_pt_e pt);
int ql_boot_fat_open(ql_boot_fat_mount_pt_e pt,const char* path, ql_boot_fat_open_mode_e mode);
int ql_boot_fat_read(ql_boot_fat_mount_pt_e pt,void* buff, unsigned int size);
int ql_boot_fat_file_size(ql_boot_fat_mount_pt_e pt);
int ql_boot_fat_lseek(ql_boot_fat_mount_pt_e pt,unsigned int ofs);
long ql_boot_fat_tell(ql_boot_fat_mount_pt_e pt);
#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RW
int ql_boot_fat_mount_ex(ql_boot_fat_mount_pt_e pt);
int ql_boot_fat_open_ex(ql_boot_fat_mount_pt_e pt,const char* path, ql_boot_fat_open_mode_e mode);
int ql_boot_fat_read_ex(ql_boot_fat_mount_pt_e pt,void* buff, unsigned int size);
int ql_boot_fat_close_ex(ql_boot_fat_mount_pt_e pt);
int ql_boot_fat_file_size_ex(ql_boot_fat_mount_pt_e pt);
int ql_boot_fat_write_ex(ql_boot_fat_mount_pt_e pt,const void* buff, unsigned int size);
int ql_boot_fat_lseek_ex(ql_boot_fat_mount_pt_e pt,unsigned int ofs);
int ql_boot_fat_is_mount_ex(ql_boot_fat_mount_pt_e pt);
#endif


#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QUEC_BOOT_PFF_H */


