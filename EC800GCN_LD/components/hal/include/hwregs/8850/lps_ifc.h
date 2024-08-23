/* Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */

#ifndef _LPS_IFC_H_
#define _LPS_IFC_H_

// Auto generated by dtools(see dtools.txt for its version).
// Don't edit it manually!

#define LPS_NB_BITS_ADDR (32)
#define LPS_IFC_ADDR_ALIGN (0)
#define LPS_IFC_TC_LEN (23)
#define LPS_IFC_STD_CHAN_NB (2)
#define LPS_IFC_RFSPI_CHAN (0)
#define LPS_IFC_AIF_CHAN (0)
#define LPS_IFC_DBG_CHAN (0)

typedef enum
{
    DMA_ID_TX_UART1 = 0,
    DMA_ID_RX_UART1 = 1,
} LPS_IFC_REQUEST_IDS_T;

#define REG_LPS_IFC_BASE (0x5170e000)

typedef volatile struct
{
    uint32_t get_ch;           // 0x00000000
    uint32_t dma_status;       // 0x00000004
    uint32_t debug_status;     // 0x00000008
    uint32_t ifc_sec;          // 0x0000000c
    struct                     // 0x00000010
    {                          //
        uint32_t control;      // 0x00000000
        uint32_t status;       // 0x00000004
        uint32_t start_addr;   // 0x00000008
        uint32_t tc;           // 0x0000000c
        uint32_t tc_threshold; // 0x00000010
    } std_ch[2];               //
} HWP_LPS_IFC_T;

#define hwp_lpsIfc ((HWP_LPS_IFC_T *)REG_ACCESS_ADDRESS(REG_LPS_IFC_BASE))

// get_ch
typedef union {
    uint32_t v;
    struct
    {
        uint32_t ch_to_use : 5; // [4:0], read only
        uint32_t __31_5 : 27;   // [31:5]
    } b;
} REG_LPS_IFC_GET_CH_T;

// dma_status
typedef union {
    uint32_t v;
    struct
    {
        uint32_t ch_enable : 2; // [1:0], read only
        uint32_t __15_2 : 14;   // [15:2]
        uint32_t ch_busy : 2;   // [17:16], read only
        uint32_t __31_18 : 14;  // [31:18]
    } b;
} REG_LPS_IFC_DMA_STATUS_T;

// debug_status
typedef union {
    uint32_t v;
    struct
    {
        uint32_t dbg_status : 1; // [0], read only
        uint32_t __31_1 : 31;    // [31:1]
    } b;
} REG_LPS_IFC_DEBUG_STATUS_T;

// ifc_sec
typedef union {
    uint32_t v;
    struct
    {
        uint32_t std_ch_reg_sec : 2; // [1:0]
        uint32_t __15_2 : 14;        // [15:2]
        uint32_t std_ch_dma_sec : 2; // [17:16]
        uint32_t __31_18 : 14;       // [31:18]
    } b;
} REG_LPS_IFC_IFC_SEC_T;

// control
typedef union {
    uint32_t v;
    struct
    {
        uint32_t enable : 1;           // [0]
        uint32_t disable : 1;          // [1]
        uint32_t ch_rd_hw_exch : 1;    // [2]
        uint32_t ch_wr_hw_exch : 1;    // [3]
        uint32_t autodisable : 1;      // [4]
        uint32_t size : 1;             // [5]
        uint32_t __7_6 : 2;            // [7:6]
        uint32_t req_src : 5;          // [12:8]
        uint32_t __15_13 : 3;          // [15:13]
        uint32_t flush : 1;            // [16]
        uint32_t max_burst_length : 2; // [18:17]
        uint32_t __31_19 : 13;         // [31:19]
    } b;
} REG_LPS_IFC_CONTROL_T;

// status
typedef union {
    uint32_t v;
    struct
    {
        uint32_t enable : 1;     // [0], read only
        uint32_t __3_1 : 3;      // [3:1]
        uint32_t fifo_empty : 1; // [4], read only
        uint32_t __31_5 : 27;    // [31:5]
    } b;
} REG_LPS_IFC_STATUS_T;

// tc
typedef union {
    uint32_t v;
    struct
    {
        uint32_t tc : 23;     // [22:0]
        uint32_t __31_23 : 9; // [31:23]
    } b;
} REG_LPS_IFC_TC_T;

// tc_threshold
typedef union {
    uint32_t v;
    struct
    {
        uint32_t tc_threshold : 23; // [22:0]
        uint32_t __31_23 : 9;       // [31:23]
    } b;
} REG_LPS_IFC_TC_THRESHOLD_T;

// get_ch
#define LPS_IFC_CH_TO_USE(n) (((n)&0x1f) << 0)

// dma_status
#define LPS_IFC_CH_ENABLE(n) (((n)&0x3) << 0)
#define LPS_IFC_CH_BUSY(n) (((n)&0x3) << 16)

// debug_status
#define LPS_IFC_DBG_STATUS (1 << 0)

// ifc_sec
#define LPS_IFC_STD_CH_REG_SEC(n) (((n)&0x3) << 0)
#define LPS_IFC_STD_CH_DMA_SEC(n) (((n)&0x3) << 16)

// control
#define LPS_IFC_ENABLE (1 << 0)
#define LPS_IFC_DISABLE (1 << 1)
#define LPS_IFC_CH_RD_HW_EXCH (1 << 2)
#define LPS_IFC_CH_WR_HW_EXCH (1 << 3)
#define LPS_IFC_AUTODISABLE (1 << 4)
#define LPS_IFC_SIZE (1 << 5)
#define LPS_IFC_REQ_SRC(n) (((n)&0x1f) << 8)
#define LPS_IFC_REQ_SRC_DMA_ID_TX_UART1 (0 << 8)
#define LPS_IFC_REQ_SRC_DMA_ID_RX_UART1 (1 << 8)
#define LPS_IFC_FLUSH (1 << 16)
#define LPS_IFC_MAX_BURST_LENGTH(n) (((n)&0x3) << 17)

#define LPS_IFC_REQ_SRC_V_DMA_ID_TX_UART1 (0)
#define LPS_IFC_REQ_SRC_V_DMA_ID_RX_UART1 (1)

// status
#define LPS_IFC_ENABLE (1 << 0)
#define LPS_IFC_FIFO_EMPTY (1 << 4)

// start_addr
#define LPS_IFC_START_ADDR(n) (((n)&0xffffffff) << 0)

// tc
#define LPS_IFC_TC(n) (((n)&0x7fffff) << 0)

// tc_threshold
#define LPS_IFC_TC_THRESHOLD(n) (((n)&0x7fffff) << 0)

#endif // _LPS_IFC_H_