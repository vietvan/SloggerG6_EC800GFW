/**  @file
  ql_sdmmc.h

  @brief
  quectel sdmmc info interface definition.

*/
/*================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/

/*=================================================================

						EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN			  WHO		  WHAT, WHERE, WHY
------------	 -------	 -------------------------------------------------------------------------------

=================================================================*/


#ifndef QL_SDMMC_H
#define QL_SDMMC_H


#ifdef __cplusplus
extern "C" {
#endif

#include "ql_api_common.h"

/*===========================================================================
 * Macro Definition
 ===========================================================================*/
 
#define QL_SDMMC_ERRCODE_BASE (QL_COMPONENT_STORAGE_SDEMMC<<16)
/*========================================================================
*  Enumeration Definition
*========================================================================*/

/****************************  error code about SDMMC    ***************************/
typedef enum
{
	QL_SDMMC_SUCCESS  						= QL_SUCCESS,
	QL_SDMMC_INVALID_PARAM_ERR				= 1|QL_SDMMC_ERRCODE_BASE,
	QL_SDMMC_ADDR_NULL_ERR					= 3|QL_SDMMC_ERRCODE_BASE,
    QL_SDMMC_INIT_ERR                       = 4|QL_SDMMC_ERRCODE_BASE,
	QL_SDMMC_MOUNT_ERR						= 5|QL_SDMMC_ERRCODE_BASE,
	QL_SDMMC_MKFS_ERR 						= 10|QL_SDMMC_ERRCODE_BASE,
    QL_SDMMC_SET_CLK_ERR                    = 11|QL_SDMMC_ERRCODE_BASE,	
    QL_SDMMC_WRITE_ERR                      = 12|QL_SDMMC_ERRCODE_BASE,
    QL_SDMMC_READ_ERR                       = 13|QL_SDMMC_ERRCODE_BASE,
    QL_SDMMC_DET_ERR                        = 14|QL_SDMMC_ERRCODE_BASE,    
    QL_SDMMC_MEM_ILLEGAL_ERR                = 20|QL_SDMMC_ERRCODE_BASE, //the *buffer of writer or read isn't from SRAM/RAM.       
    QL_SDMMC_ADDR_ALIGNED_ERR               = 21|QL_SDMMC_ERRCODE_BASE, 
    QL_SDMMC_SIZE_ALIGNED_ERR               = 22|QL_SDMMC_ERRCODE_BASE,     
    QL_SDMMC_BLOCK_NUM_OVER_ERR             = 23|QL_SDMMC_ERRCODE_BASE,    
	QL_SDMMC_LOCK_ERR 						= 24|QL_SDMMC_ERRCODE_BASE,

    QL_SDMMC_CREATE_ERR                     = 30|QL_SDMMC_ERRCODE_BASE,     
    QL_SDMMC_OPEN_ERR                       = 31|QL_SDMMC_ERRCODE_BASE, 
    QL_SDMMC_CLOSE_ERR                      = 32|QL_SDMMC_ERRCODE_BASE,
    QL_SDMMC_OPERATION_NOT_ALLOWED          = 33|QL_SDMMC_ERRCODE_BASE,//if sdmmc is opened through mount,sdmmc write/read is not allowed

    //errcode for multi partition    
    QL_SDMMC_MULTI_PARTITON_NUM_ERR         = 40|QL_SDMMC_ERRCODE_BASE,     
    QL_SDMMC_MULTI_PARTITON_FDISK_ERR       = 41|QL_SDMMC_ERRCODE_BASE,
    QL_SDMMC_UMOUNT_ERR                     = 42|QL_SDMMC_ERRCODE_BASE,
    QL_SDMMC_MOUNT_ALL_ERR                  = 43|QL_SDMMC_ERRCODE_BASE,
	QL_SDMMC_UMOUNT_ALL_ERR                 = 44|QL_SDMMC_ERRCODE_BASE,
	QL_SDMMC_MKFS_ALL_ERR                   = 45|QL_SDMMC_ERRCODE_BASE,
    QL_SDMMC_MULTI_PARTITON_OTHER_ERR       = 46|QL_SDMMC_ERRCODE_BASE,
} ql_errcode_sdmmc_e;

/*===========================================================================
 * Struct
 ===========================================================================*/
typedef struct
{
    uint8_t  mid;           //Manufacturer ID
    uint8_t  pnm[6];        //Product name
    uint8_t  psn[4];        //Product serial number
    uint32_t blknum;        //block count
    uint32_t blksize;       //block size
} ql_sdmmc_hw_info_t;

/*===========================================================================
 * Functions declaration
 ===========================================================================*/

/*****************************************************************
* Function: ql_sdmmc_mkfs
*
* Description:   format sd card in FM_FAT32
* 
* Parameters:
* 	opt	  		[in] 	input   FM_FAT32(0x02)
*
* Return:
* 	QL_SDMMC_SUCCESS	succeed
*	OTHER ERRCODE		failure
*
*****************************************************************/
ql_errcode_sdmmc_e ql_sdmmc_mkfs(uint8_t opt);

/*****************************************************************
* Function: ql_sdmmc_umount
*
* Description:   Uninitialize the sdmmc driver,umount sdcard/emmc
* 
* Parameters:	void
*
* Return:
* 	QL_SDMMC_SUCCESS		succeed
*
*****************************************************************/
ql_errcode_sdmmc_e ql_sdmmc_umount(void);

/*****************************************************************
* Function: ql_sdmmc_mount
*
* Description:   Initialize the sdmmc driver,and mount sdcard/emmc
* 
* Parameters:	void
*
* Return:
* 	QL_SDMMC_SUCCESS	succeed
*	QL_SDMMC_MOUNT_ERR	failure
*
*****************************************************************/
ql_errcode_sdmmc_e ql_sdmmc_mount(void);

/*****************************************************************
* Function: ql_sdmmc_is_format
*
* Description:   Whether the SD card/emmc is formatted
*                This function will be mounted once if it has not been mounted before,
*                so it is recommended to call it during a mount failure.
*                如果前面没有mount过，这函数将会先mount一次，所以建议在mount失败里调用
* 
* Parameters:   void
*
* Return:
* 	true	    is formatted
*	false		is not formatted
*
*****************************************************************/
bool ql_sdmmc_is_format(void);

/*****************************************************************
* Function: ql_sdmmc_is_mount
*
* Description:   Whether the SD card/emmc is mounted
* 
* Parameters:   void
*
* Return:
* 	true	    is mounted
*	false		is not mounted
*
*****************************************************************/
bool ql_sdmmc_is_mount(void);

/*****************************************************************
* Function: ql_sdmmc_set_clk 
* 
* Description: set sdmmc clk freq,can use either the mount file system or not
*
* Parameters:  
*   freq           [in]  The sdmmc clk freq for setting
*                        400000 <= freq <= 50000000
* Return:
* 	QL_SDMMC_SUCCESS	succeed
*	OTHER ERRCODE		failure
*****************************************************************/
ql_errcode_sdmmc_e ql_sdmmc_set_clk(uint32_t freq);

/*****************************************************************
* Function: ql_sdmmc_write
*
* Description: Write blocks of data to sdmmc,only be used when there is no mount file system
*
* Parameters:
*   block_number   [in]  block_number Start address of the SD memory block for writing
*   buffer         [in]  buffer Pointer to the block of data to write
*                        Must be 32 bytes (not 32 bits) aligned.
*   size           [in]  size Number of bytes to write
*                        Must be sector size aligned
* Return:
* 	QL_SDMMC_SUCCESS	succeed
*	OTHER ERRCODE		failure
*****************************************************************/
ql_errcode_sdmmc_e ql_sdmmc_write(uint32_t block_number, const void *buffer, uint32_t size);

/*****************************************************************
* Function: ql_sdmmc_read
*
* Description: Read blocks of data from sdmmc,only be used when there is no mount file system 
*
* Parameters:
*   block_number   [in]  block_number Start address of the SD memory block for reading
*   buffer         [out] Pointer to the block of data to read
*                        Must be 32 bytes (not 32 bits) aligned.
*   size           [in]  Number of bytes to read
*                        Must be sector size aligned
* Return:
* 	QL_SDMMC_SUCCESS	succeed
*	OTHER ERRCODE		failure
*****************************************************************/
ql_errcode_sdmmc_e ql_sdmmc_read(uint32_t block_number, void *buffer, uint32_t size);

/*****************************************************************
* Function: ql_sdmmc_get_block_number
*
* Description: get sdmmc block number,can use either the mount file system or not
*
* Parameters:

* Return:  block number
*    
*****************************************************************/
uint32_t ql_sdmmc_get_block_number(void);

/*****************************************************************
* Function: ql_sdmmc_open
*
* Description: only open sdmmc driver,not mount fatfs file system.
*              If the file system has been mounted, it will be umount 
*
* Parameters:

* Return:
* 	QL_SDMMC_SUCCESS	succeed
*	OTHER ERRCODE		failure    
*****************************************************************/
ql_errcode_sdmmc_e ql_sdmmc_open(void);

/*****************************************************************
* Function: ql_sdmmc_close
*
* Description: close sdmmc driver,while umount the file system
*              if the file system has been mounted.
*
* Parameters:

* Return:
* 	QL_SDMMC_SUCCESS	succeed
*	OTHER ERRCODE		failure   
*****************************************************************/
ql_errcode_sdmmc_e ql_sdmmc_close(void);

/*****************************************************************
* Function: ql_sdmmc_get_hw_info
*
* Description: get information about sdmmc,can use either the mount file system or not
*
* Parameters:
*   info         [out] Pointer to info data to read
*
* Return:
* 	QL_SDMMC_SUCCESS	succeed
*	OTHER ERRCODE		failure   
*****************************************************************/
ql_errcode_sdmmc_e ql_sdmmc_get_hw_info(ql_sdmmc_hw_info_t *info);

/*****************************************************************
* Function: ql_sdmmc_sdio1_vdd_off
*
* Description: close sdio1 vdd，Use this function with caution.
*
* 此函数一般不要调用，如果一定需要使用请注意以下使用注意事项。
* 使用注意事项：
* 1、此函数只针对SD卡的VCC3.3V是由SDIO_VDD控制的用户，如果是由GPIO控制的，请先通过GPIO关闭SD
*    卡的供电再调用此函数。
* 2、调用此函数之前，一定要先反初始化SD卡(如果是调用ql_sdmmc_mount初始化SD卡并挂载文件系统的，
*    要调用ql_sdmmc_umount反初始化SD卡和文件系统；如果是调用ql_sdmmc_open初始化SD卡的，要调用
*    ql_sdmmc_close反初始化SD卡)。否则下次调用ql_sdmmc_mount/ql_sdmmc_open时，会返回成功，
*    但是SD卡不能使用。
* 3、调用此函数后SD卡断电，所以下次使用时需要重新调用ql_sdmmc_mount/ql_sdmmc_open初始化
*    SD卡。
* 4、调用此函数之前，如果不关闭以新建方式打开的文件，文件会丢失；如果是只打开已创建的文件，不关
*    闭，新写进去的内容会丢失，原来的内容不变。但是之前打开文件返回的fd文件句柄，不可再使用。
* 5、如果在休眠中使用此函数，要注意使用方式：
*    >>以下方案是不可取的：
*    ql_enter_sleep_cb进入休眠回调函数中ql_fclose(fd);ql_sdmmc_umount()；ql_sdmmc_sdio1_vdd_off()；
*    ql_exit_sleep_cb退出休眠回调函数中，ql_sdmmc_mount；ql_fopen
*    这样的逻辑会大大的损耗sd卡的使用寿命，因为调用ql_fclose就会把缓存和文件系统信息写到sd卡中。 
*    >>建议：
*    在使用sd卡的时候才ql_sdmmc_mount，ql_fopen，读写，ql_fclose，并加休眠锁。因为ql_sdmmc_mount
*    需要一定的时间，所以在调用mount和读写sd卡的线程要添加休眠锁ql_lpm_wakelock_create，
*    但是有可能会影响休眠。如果不加休眠锁，会在mount过程中再次进入休眠，导致mount失败。
*
* Parameters:
*
* Return:
*  
*****************************************************************/
void ql_sdmmc_sdio1_vdd_off(void);


/*****************************************************************
                 SD卡多分区 API
*****************************************************************/

typedef enum
{
    QL_SDMMC_PARTITION_NUM_1   = 1,            //1--分区1 
    QL_SDMMC_PARTITION_NUM_2   = 2,            //2--分区2
    QL_SDMMC_PARTITION_NUM_MAX ,
    QL_SDMMC_PARTITION_NUM_ALL = 0xff          //所有分区    
}ql_sdmmc_partition_num_e;

typedef struct
{
    ql_sdmmc_partition_num_e partnum;        //分区号
    int64 partsize;                          //分区大小，单位：Mbyte   
}ql_sdmmc_part_info_t;

/*****************************************************************
* Function: ql_sdmmc_fdisk_ex
*
* Description:   sdcard/EMMC partition, format and mount all partitions
* 
* Parameters:
*
* 	part_info[]	  	[in] 	Partition information,
*                           分区信息，最后一个分区大小会自动调整为剩余容量
*                           注意：实际容量会比卡上标识的容量小一点点，所以最后一个分区可以填小一点，会自动调整为剩余容量
*                           文件系统是有最小扇区数量要求的，最小为1024个扇区，所以分区容量不能太小
*                           ex:part_info[] = {
*                               {QL_SDMMC_PARTITION_NUM_1,1000},
*                               {QL_SDMMC_PARTITION_NUM_2,1000}                           
*                           }
*
* Return:
* 	QL_SDMMC_SUCCESS	succeed
*	OTHER ERRCODE		failure
*
*****************************************************************/
ql_errcode_sdmmc_e ql_sdmmc_fdisk_ex(ql_sdmmc_part_info_t part_info[]);

/*****************************************************************
* Function: ql_sdmmc_mkfs_ex
*
* Description:   format sd card partition.
*                This function automatically adjusts the formatting type 
*                based on the number of partition sectors.
*                此函数将会根据分区扇区数量自动调整格式化类型,
*                如果总扇区数小于66130格式化为FM_FAT，大于格式化为FM_FAT32
* 
* Parameters:
*
*   part        [in]    partition number,Currently only two partitions are supported
*                       if QL_SDMMC_PARTITION_NUM_ALL，umount all partitions
* 	opt	  		[in] 	FM_FAT(0x01)(FAT12/FAT16) FM_FAT32(0x02)
*                       根据扇区的数量判断格式化为哪种格式：1024-8220：FM_FAT(FAT12)
*                       8229-4190430：FM_FAT(FAT16)   66130-67048580：FM_FAT32(0x02)
*
* Return:
* 	QL_SDMMC_SUCCESS	succeed
*	OTHER ERRCODE		failure
*
*****************************************************************/
ql_errcode_sdmmc_e ql_sdmmc_mkfs_ex(ql_sdmmc_partition_num_e part,uint8_t opt);

/*****************************************************************
* Function: ql_sdmmc_mount_ex
*
* Description:   Initialize the sdmmc driver,and mount partition
* 
* Parameters:
*
*   part        [in]    partition number,Currently only two partitions are supported
*                       if QL_SDMMC_PARTITION_NUM_ALL，mount all partitions
*
* Return:
* 	QL_SDMMC_SUCCESS	succeed
*	OTHER ERRCODE		failure
*
*****************************************************************/
ql_errcode_sdmmc_e ql_sdmmc_mount_ex(ql_sdmmc_partition_num_e part);

/*****************************************************************
* Function: ql_sdmmc_umount_ex
*
* Description:   umount partition,if all partitions are in umount state,Uninitialize the sdmmc driver
* 
* Parameters:
*
*   part        [in]    partition number,Currently only two partitions are supported 
*                       if QL_SDMMC_PARTITION_NUM_ALL，umount all partitions
*
* Return:
* 	QL_SDMMC_SUCCESS	succeed
*	OTHER ERRCODE		failure
*
*****************************************************************/
ql_errcode_sdmmc_e ql_sdmmc_umount_ex(ql_sdmmc_partition_num_e part);

/*****************************************************************
* Function: ql_sdmmc_is_fdisk_ex
*
* Description:   Whether the SD card/emmc is partitioned
* 
* Parameters:   void
*
* Return:
* 	true	    is partitioned
*	false		is not partitioned
*
*****************************************************************/
bool ql_sdmmc_is_fdisk_ex(void);

/*****************************************************************
* Function: ql_sdmmc_is_format_ex
*
* Description:   Whether the SD card/emmc is formatted.
*                This function will be mounted once if it has not been mounted before,
*                so it is recommended to call it during a mount failure.
*                如果前面没有mount过，这函数将会先mount一次，所以建议在mount失败里调用
*
* Parameters:
*
*   part        [in]    partition number,Currently only two partitions are supported 
*                       QL_SDMMC_PARTITION_NUM_ALL cannot be used in this function.
*
* Return:
* 	true	    is formatted
*	false		is not formatted
*
*****************************************************************/
bool ql_sdmmc_is_format_ex(ql_sdmmc_partition_num_e part);

/*****************************************************************
* Function: ql_sdmmc_is_mount_ex
*
* Description:   Whether the SD card/emmc is mounted
* 
* Parameters:
*
*   part        [in]    partition number,Currently only two partitions are supported
*                       QL_SDMMC_PARTITION_NUM_ALL cannot be used in this function.
*
* Return:
* 	true	    is mounted
*	false		is not mounted
*
*****************************************************************/
bool ql_sdmmc_is_mount_ex(ql_sdmmc_partition_num_e part);


#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_SDMMC_H */


