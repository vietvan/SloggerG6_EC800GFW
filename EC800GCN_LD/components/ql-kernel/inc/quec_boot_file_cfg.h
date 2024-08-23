/**  @file
  quec_boot_file_config.h

  @brief
  This file is used to define boot file system api for different Quectel Project.

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

#ifndef QUEC_BOOT_FILE_CONFIG_H
#define QUEC_BOOT_FILE_CONFIG_H

#include "fs_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CONFIG_FS_SPI6_EXT_NOR_SFFS_MOUNT_POINT     "/ext"         //SPI6 EXT NOR  FLASH,  mount sffs  file system
#define CONFIG_FS_SPI4_EXT_NAND_FAT_MOUNT_POINT     "/exnand"      //SPI4 EXT NAND FLASH,  mount fatfs file system
#define CONFIG_FS_SPI4_EXT_NOR_FAT_MOUNT_POINT      "/exnor"       //SPI4 EXT NOR  FLASH,  mount fatfs file system
#define CONFIG_FS_SPI4_EXT_NOR_SFFS_MOUNT_POINT     "/ext4n"       //SPI4 EXT NOR  FLASH,  mount sffs  file system
#ifndef CONFIG_FS_SDCARD_MOUNT_POINT
#define CONFIG_FS_SDCARD_MOUNT_POINT                "/sdcard0"
#endif

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QUEC_BOOT_FILE_CONFIG_H */


