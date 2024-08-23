/**  @file
  ql_api_spi_nand_flash.h

  @brief
  This file is used to define bt api for different Quectel Project.

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


#ifndef QL_API_SPI_NAND_FLASH_H
#define QL_API_SPI_NAND_FLASH_H
#include "ql_api_common.h"
#include "ql_api_spi.h"
#include "ql_api_spi_flash.h"

#ifdef __cplusplus
extern "C" {
#endif


/*========================================================================
 *  Variable Definition
 *========================================================================*/
 
#define QL_SPI_NAND_PAGE_SIZE                       2048
#define QL_SPI_NAND_SPARE_SIZE                      64
#define QL_SPI_NAND_PAGE_SPARE_SIZE                 (QL_SPI_NAND_PAGE_SIZE+QL_SPI_NAND_SPARE_SIZE)

/****************************  error code about ql spi flash ***************************/
typedef ql_errcode_spi_flash_e ql_errcode_spi_nand_e ;
typedef ql_spi_flash_config_s ql_spi_nand_config_s ;

typedef enum
{
    QL_NAND_FLASH_STATUS_1 = 0,                     //Status Register-1
    QL_NAND_FLASH_STATUS_2,                         //Status Register-2
    QL_NAND_FLASH_STATUS_3,                         //Status Register-3
    QL_NAND_FLASH_STATUS_4,                         //Status Register-4
}ql_spi_nand_status_reg_e;

typedef enum
{
    QL_NAND_FLASH_ID_16BIT = 0,               //Manufacturer ID(8bit) + device ID(8bit)
    QL_NAND_FLASH_ID_24BIT,                   //Manufacturer ID(8bit) + device ID(16bit)
}ql_spi_nand_id_type_e;

typedef enum
{
    QL_NAND_TYPE_STANDARD_SPI = 0,   //Standard SPI: SCLK, CS#, SI, SO 
    QL_NAND_TYPE_QUAD_SPI,           //Quad SPI: SCLK, CS#, SIO0, SIO1, SIO2, SIO3
    QL_NAND_TYPE_DUAL_SPI,           //Dual SPI: SCLK, CS#, SIO0, SIO1
}ql_nand_spi_type_e;

typedef struct
{
    ql_nand_spi_type_e     (*nand_type)(void);
    ql_errcode_spi_nand_e  (*nand_init)(void* nand_config);
    ql_errcode_spi_nand_e  (*read_devid)(ql_spi_port_e port, unsigned  char *mid,ql_spi_nand_id_type_e mid_type);
    ql_errcode_spi_nand_e  (*read_status)(ql_spi_port_e port, ql_spi_nand_status_reg_e reg, unsigned char *status);
    ql_errcode_spi_nand_e  (*write_page_spare)(ql_spi_port_e port, unsigned int page_addr, unsigned short column_addr, unsigned char *data, int len);
    ql_errcode_spi_nand_e  (*read_page_spare)(ql_spi_port_e port, unsigned int page_addr, unsigned short column_addr, unsigned char *data, int len);
    ql_errcode_spi_nand_e  (*erase_block)(ql_spi_port_e port, unsigned int page_addr);
    ql_errcode_spi_nand_e  (*reset)(ql_spi_port_e port);
} ql_nand_ops_t;
/*========================================================================
 *  function Definition
 *========================================================================*/
/*****************************************************************
* Function: ql_spi_nand_init
*
* Description:
*   初始化NAND FLASH
* 
* Parameters:
*   port     [in]  SPI总线选择
*   spiclk   [in]  SPI时钟选择
*
* Return:ql_errcode_spi_nand_e
*
*****************************************************************/
ql_errcode_spi_nand_e ql_spi_nand_init(ql_spi_port_e port, ql_spi_clk_e spiclk);

/*****************************************************************
* Function: ql_spi_nand_init_ext
*
* Description:
*   扩展初始化NAND FLASH，与ql_spi_nand_init有更多的设置选项
* 
* Parameters:
*   nand_config   [in]  SPI配置参数
*
* Return:ql_errcode_spi_nand_e
*
*****************************************************************/
ql_errcode_spi_nand_e ql_spi_nand_init_ext(ql_spi_nand_config_s nand_config);

/*****************************************************************
* Function: ql_spi_nand_read_devid_ex
*
* Description:
*   read flash devie id
* 
* Parameters:
*   port     [in]  SPI bus select
*   mid      [out] manufacturer id
*   mid_type [in]  manufacturer id type
*
* Return:ql_errcode_spi_nand_e
*
*****************************************************************/
ql_errcode_spi_nand_e ql_spi_nand_read_devid_ex(ql_spi_port_e port, unsigned  char *mid,ql_spi_nand_id_type_e mid_type);

/*****************************************************************
* Function: ql_spi_nand_read_devid
*
* Description:
*   read flash devie id
* 
* Parameters:
*   port     [in]  SPI bus select
*   mid      [out] manufacturer id
*
* Return:ql_errcode_spi_nand_e
*
*****************************************************************/
ql_errcode_spi_nand_e ql_spi_nand_read_devid(ql_spi_port_e port, unsigned  char *mid);

/*****************************************************************
* Function: ql_spi_nand_read_page_spare
*
* Description:
*   read page(2048 byte)+spare area(64 byte)
* 
* Parameters:
*   port            [in]  SPI bus select
*   page_addr       [in]  page address
*   column_addr     [in]  column address
*   data            [out] data read buffer
*   len             [in] data read length
*
* Return:ql_errcode_spi_nand_e
*
*****************************************************************/
ql_errcode_spi_nand_e ql_spi_nand_read_page_spare(ql_spi_port_e port, unsigned int page_addr, unsigned short column_addr, unsigned char *data, int len);

/*****************************************************************
* Function: ql_spi_nand_write_page_spare
*
* Description:
*   write page(2048 byte)+spare area(64 byte)
* 
* Parameters:
*   port            [in]  SPI bus select
*   page_addr       [in]  page address
*   column_addr     [in]  column address
*   data            [in] data read buffer
*   len             [in] data read length
*
* Return:ql_errcode_spi_nand_e
*
*****************************************************************/
ql_errcode_spi_nand_e ql_spi_nand_write_page_spare(ql_spi_port_e port, unsigned int page_addr, unsigned short column_addr, unsigned char *data, int len);

/*****************************************************************
* Function: ql_spi_nand_write_spare
*
* Description:
*   write spare area(64 byte)
* 
* Parameters:
*   port            [in]  SPI bus select
*   page_addr       [in]  page address
*   data            [out] data read buffer
*   len             [in] data read length
*
* Return:ql_errcode_spi_nand_e
*
*****************************************************************/
ql_errcode_spi_nand_e ql_spi_nand_write_spare(ql_spi_port_e port, unsigned int page_addr, unsigned char *data, int len);

/*****************************************************************
* Function: ql_spi_nand_read_status
*
* Description:
*   read status register value
* 
* Parameters:
*   port            [in]  SPI bus select
*   reg             [in]  FLASH status register
*   status          [out] status register value
*
* Return:ql_errcode_spi_nand_e
*
*****************************************************************/
ql_errcode_spi_nand_e ql_spi_nand_read_status(ql_spi_port_e port, ql_spi_nand_status_reg_e reg, unsigned char *status);

/*****************************************************************
* Function: ql_spi_nand_write_status
*
* Description:
*   read status register value
* 
* Parameters:
*   port            [in]  SPI bus select
*   reg             [in]  FLASH status register
*   status          [in]  status register value
*
* Return:ql_errcode_spi_nand_e
*
*****************************************************************/
ql_errcode_spi_nand_e ql_spi_nand_write_status(ql_spi_port_e port, ql_spi_nand_status_reg_e reg, unsigned char status);

/*****************************************************************
* Function: ql_spi_nand_erase_block
*
* Description:
*   erase block(128k):D8H
* 
* Parameters:
*   port            [in]  SPI bus selelct
*   page_addr       [in]  page address
*
* Return:ql_errcode_spi_nand_e
*
*****************************************************************/
ql_errcode_spi_nand_e ql_spi_nand_erase_block(ql_spi_port_e port, unsigned int page_addr);

/*****************************************************************
* Function: ql_spi_nand_reset
*
* Description:
*   reset flash device:FFH
* 
* Parameters:
*   port            [in]  SPI bus selelct
*
* Return:ql_errcode_spi_nand_e
*
*****************************************************************/
ql_errcode_spi_nand_e ql_spi_nand_reset(ql_spi_port_e port);



typedef enum
{
    //SPI6_NAND_FLASH_CLK_31MHZ  = 0x0, //8850上不支持该频率，与8910有差异
    //SPI6_NAND_FLASH_CLK_41MHZ  = 0x0, //8850上不支持该频率，与8910有差异
    SPI6_NAND_FLASH_CLK_62MHZ  = 0x1,
    SPI6_NAND_FLASH_CLK_71MHZ  = 0x3,
    SPI6_NAND_FLASH_CLK_83MHZ  = 0x5,
    SPI6_NAND_FLASH_CLK_91MHZ  = 0x6,
    SPI6_NAND_FLASH_CLK_100MHZ = 0x7,
    SPI6_NAND_FLASH_CLK_111MHZ = 0x8,
    SPI6_NAND_FLASH_CLK_125MHZ = 0x9,
    //以下为8850支持，但8910不支持的
    SPI6_NAND_FLASH_CLK_142MHZ = 0xa,  
    SPI6_NAND_FLASH_CLK_166MHZ = 0xb,  
    SPI6_NAND_FLASH_CLK_200MHZ = 0xc,  //8850默认为该频率
    //8850内置flash不支持以下时钟频率
    //SPI6_NAND_FLASH_CLK_250MHZ = 0xd,
    //SPI6_NAND_FLASH_CLK_333MHZ = 0xe,
    SPI6_NAND_FLASH_CLK_500MHZ = 0xf,
    SPI6_NAND_FLASH_CLK_INVALID = 0x10,
}ql_spi6_nand_flash_clk_e;

/*
目前建议使用的值:如果需要使用其他的freq需要抓波形调整参数,尤其是sample_delay参数,
如果设置不对会导致读/写数据不正确。
 ----------------------------------------------------------------------------
| 复用的pin脚--GPIO number:GPIO18-23                                             |
 ----------------------------------------------------------------------------
| freq   |          clk                   | clk_div  |  drv   | sample_delay |
| 40M    |  SPI6_NAND_FLASH_CLK_200MHZ    |   5      |  0x2   |       2/3    |
| 50M    |  SPI6_NAND_FLASH_CLK_200MHZ    |   4      |  0x2   |       2/3    |
| 83.3M  |  SPI6_NAND_FLASH_CLK_500MHZ    |   6      |  0x2   |       4/6    |
 ----------------------------------------------------------------------------

 ----------------------------------------------------------------------------
| 复用的pin脚--SPI_LCD                                                           |
 ----------------------------------------------------------------------------
| freq   |          clk                   | clk_div  |  drv   | sample_delay |
| 40M    |  SPI6_NAND_FLASH_CLK_200MHZ    |   5      |  0x7   |       2/3    |
| 50M    |  SPI6_NAND_FLASH_CLK_200MHZ    |   4      |  0x7   |       2/3    |
| 83.3M  |  SPI6_NAND_FLASH_CLK_500MHZ    |   6      |  0x7   |       4/5    |
 ----------------------------------------------------------------------------

 ----------------------------------------------------------------------------
| 复用的pin脚--SDMMC                                                             |
 ----------------------------------------------------------------------------
| freq   |          clk                   | clk_div  |  drv   | sample_delay |
| 40M    |  SPI6_NAND_FLASH_CLK_200MHZ    |   5      |  0x7   |       2/3    |
| 50M    |  SPI6_NAND_FLASH_CLK_200MHZ    |   4      |  0x7   |       2/3    |
| 83.3M  |  SPI6_NAND_FLASH_CLK_500MHZ    |   6      |  0x7   |       4/6    |
 ----------------------------------------------------------------------------
*/
typedef struct
{
    ql_spi_port_e port;                  //SPI6总线选择,此参数无意义
    ql_spi6_nand_flash_clk_e clk;        //SPI6时钟源配置,默认SPI6_NAND_FLASH_CLK_200MHZ
    uint8_t clk_div;                     //时钟分频,默认5,freq =200M/5=40M
    uint8_t quad_mode;                   //1--Quad SPII: SCLK, CS#, SIO0, SIO1, SIO2, SIO3  0--Dual SPI: SCLK, CS#, SIO0, SIO1
    uint8_t sample_delay;                //延时采样,此值与clk和驱动能力相关,需要配置波形设置(主要用于调节数据的采样点在时钟的有效位置,尽可能的在中间),一般频率越高此值越大
    uint8_t isslow;                      //是否使用低速率时钟，1--low  0--fast；仅8910有效
} ql_spi6_nand_config_s;

/*========================================================================
 *  function Definition
 *========================================================================*/
/*****************************************************************
* Function: ql_spi6_nand_init
*
* Description:
*   初始化NAND FLASH
* 
* Parameters:
*   port     [in]  SPI6总线选择,此参数预留,目前没有实际意义
*   clk      [in]  SPI6时钟源选择
*   clk_div  [in]  SPI6时钟分频
*
* Return:ql_errcode_spi_nand_e
*
*****************************************************************/
ql_errcode_spi_nand_e ql_spi6_nand_init(ql_spi_port_e port, ql_spi6_nand_flash_clk_e clk, uint8_t clk_div);

/*****************************************************************
* Function: ql_spi6_nand_init_ext
*
* Description:
*   扩展初始化NAND FLASH，与ql_spi_nand_init有更多的设置选项
* 
* Parameters:
*   config   [in]  SPI6配置参数
*
* Return:ql_errcode_spi_nand_e
*
*****************************************************************/
ql_errcode_spi_nand_e ql_spi6_nand_init_ext(ql_spi6_nand_config_s config);

/*****************************************************************
* Function: ql_spi6_nand_read_devid_ex
*
* Description:
*   read flash devie id
* 
* Parameters:
*   port     [in]  SPI6 bus select,this parameter is reserved
*   mid      [out] manufacturer id
*   mid_type [in]  manufacturer id type
*
* Return:ql_errcode_spi_nand_e
*
*****************************************************************/
ql_errcode_spi_nand_e ql_spi6_nand_read_devid_ex(ql_spi_port_e port, unsigned  char *mid,ql_spi_nand_id_type_e mid_type);

/*****************************************************************
* Function: ql_spi6_nand_read_devid
*
* Description:
*   read flash devie id
* 
* Parameters:
*   port     [in]  SPI6 bus select,this parameter is reserved
*   mid      [out] manufacturer id
*
* Return:ql_errcode_spi_nand_e
*
*****************************************************************/
ql_errcode_spi_nand_e ql_spi6_nand_read_devid(ql_spi_port_e port, unsigned  char *mid);

/*****************************************************************
* Function: ql_spi6_nand_read_page_spare
*
* Description:
*   read page(2048 byte)+spare area(64 byte)
* 
* Parameters:
*   port            [in]  SPI6 bus select,this parameter is reserved
*   page_addr       [in]  page address
*   column_addr     [in]  column address
*   data            [out] data read buffer
*   len             [in] data read length
*
* Return:ql_errcode_spi_nand_e
*
*****************************************************************/
ql_errcode_spi_nand_e ql_spi6_nand_read_page_spare(ql_spi_port_e port, unsigned int page_addr, unsigned short column_addr, unsigned char *data, int len);

/*****************************************************************
* Function: ql_spi6_nand_write_page_spare
*
* Description:
*   write page(2048 byte)+spare area(64 byte)
* 
* Parameters:
*   port            [in]  SPI6 bus select,this parameter is reserved
*   page_addr       [in]  page address
*   column_addr     [in]  column address
*   data            [in] data read buffer
*   len             [in] data read length
*
* Return:ql_errcode_spi_nand_e
*
*****************************************************************/
ql_errcode_spi_nand_e ql_spi6_nand_write_page_spare(ql_spi_port_e port, unsigned int page_addr, unsigned short column_addr, unsigned char *data, int len);

/*****************************************************************
* Function: ql_spi6_nand_write_spare
*
* Description:
*   write spare area(64 byte)
* 
* Parameters:
*   port            [in]  SPI6 bus select,this parameter is reserved
*   page_addr       [in]  page address
*   data            [out] data read buffer
*   len             [in] data read length
*
* Return:ql_errcode_spi_nand_e
*
*****************************************************************/
ql_errcode_spi_nand_e ql_spi6_nand_write_spare(ql_spi_port_e port, unsigned int page_addr, unsigned char *data, int len);

/*****************************************************************
* Function: ql_spi6_nand_read_status
*
* Description:
*   read status register value
* 
* Parameters:
*   port            [in]  SPI6 bus select,this parameter is reserved
*   reg             [in]  FLASH status register
*   status          [out] status register value
*
* Return:ql_errcode_spi_nand_e
*
*****************************************************************/
ql_errcode_spi_nand_e ql_spi6_nand_read_status(ql_spi_port_e port, ql_spi_nand_status_reg_e reg, unsigned char *status);

/*****************************************************************
* Function: ql_spi6_nand_write_status
*
* Description:
*   read status register value
* 
* Parameters:
*   port            [in]  SPI6 bus select,this parameter is reserved
*   reg             [in]  FLASH status register
*   status          [in]  status register value
*
* Return:ql_errcode_spi_nand_e
*
*****************************************************************/
ql_errcode_spi_nand_e ql_spi6_nand_write_status(ql_spi_port_e port, ql_spi_nand_status_reg_e reg, unsigned char status);

/*****************************************************************
* Function: ql_spi6_nand_erase_block
*
* Description:
*   erase block(128k):D8H
* 
* Parameters:
*   port            [in]  SPI6 bus select,this parameter is reserved
*   page_addr       [in]  page address
*
* Return:ql_errcode_spi_nand_e
*
*****************************************************************/
ql_errcode_spi_nand_e ql_spi6_nand_erase_block(ql_spi_port_e port, unsigned int page_addr);

/*****************************************************************
* Function: ql_spi6_nand_reset
*
* Description:
*   reset flash device:FFH
* 
* Parameters:
*   port            [in]  SPI6 bus select,this parameter is reserved
*
* Return:ql_errcode_spi_nand_e
*
*****************************************************************/
ql_errcode_spi_nand_e ql_spi6_nand_reset(ql_spi_port_e port);

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_API_SPI_NAND_FLASH_H */



