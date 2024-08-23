/**  @file
  quec_boot_uart.h

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

#ifndef QUEC_BOOT_SDMMC_H
#define QUEC_BOOT_SDMMC_H

#include "quec_boot_pff.h"
#include "quec_proj_config.h"


#ifdef __cplusplus
extern "C" {
#endif


/*===========================================================================
 * Macro Definition
 ===========================================================================*/

#define TCARD_SECTOR_SIZE           (512)
#define TCARD_CACHE_SECTOR_COUNT    (64)
#define SDMMC_OPT_BLOCK_COUNT       (1)

#define QUEC_ECHO_ENBLE              0
#define QUEC_MINI_SD_INIT            1  //剪裁boot空间

#define QL_BOOT_SDMMC_CLK_FREQ_SDHC  50000000  //50MHz
#define QL_BOOT_SDMMC_CLK_FREQ_SD    25000000  //25MHz

/*===========================================================================
 * Enum
 ===========================================================================*/

typedef struct
{
	uint32_t sdmmc_clk;			//sdmmc时钟频率,400000 <= clk <= 50000000,默认25MHz
	uint32_t power_mv;			//sdmmc电压,SD卡默认3.2v,emmc默认1.8v
} ql_boot_sdmmc_cfg_t;

/*===========================================================================
 * Functions declaration
 ===========================================================================*/
 //sdmmc1
#if (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_SDMMC)
int quec_boot_sdmmc_init(ql_boot_fat_mount_pt_e pt);
int quec_boot_sdmmc_destroy();

#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RW
int quec_boot_sdmmc_init_ex(ql_boot_fat_mount_pt_e pt);
#endif

#elif (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_EMMC)
int quec_boot_emmc_init(ql_boot_fat_mount_pt_e pt);
int quec_boot_emmc_destroy();

#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RW
int quec_boot_emmc_init_ex(ql_boot_fat_mount_pt_e pt);
#endif

//sdmmc2
#elif (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_SDMMC2)
int quec_boot_sdmmc2_init(ql_boot_fat_mount_pt_e pt);
int quec_boot_sdmmc2_destroy();

#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RW
int quec_boot_sdmmc2_init_ex(ql_boot_fat_mount_pt_e pt);
#endif

#elif (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_EMMC2)
int quec_boot_emmc2_init(ql_boot_fat_mount_pt_e pt);
int quec_boot_emmc2_destroy();

#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RW
int quec_boot_emmc2_init_ex(ql_boot_fat_mount_pt_e pt);
#endif

#endif

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QUEC_BOOT_SDMMC_H */


