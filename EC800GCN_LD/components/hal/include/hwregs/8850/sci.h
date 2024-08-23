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

#ifndef _SCI_H_
#define _SCI_H_

// Auto generated by dtools(see dtools.txt for its version).
// Don't edit it manually!

#define REG_SCI1_BASE (0x14000000)
#define REG_SCI2_BASE (0x14001000)

typedef volatile struct
{
    uint32_t sci_config;     // 0x00000000
    uint32_t status;         // 0x00000004
    uint32_t data;           // 0x00000008
    uint32_t clkdiv_reg;     // 0x0000000c
    uint32_t rxcnt_reg;      // 0x00000010
    uint32_t times;          // 0x00000014
    uint32_t ch_filt_reg;    // 0x00000018
    uint32_t dbg;            // 0x0000001c
    uint32_t int_cause;      // 0x00000020
    uint32_t int_clr;        // 0x00000024
    uint32_t int_mask;       // 0x00000028
    uint32_t pa_clk_stop_en; // 0x0000002c
    uint32_t pa_status;      // 0x00000030
} HWP_SCI_T;

#define hwp_sci1 ((HWP_SCI_T *)REG_ACCESS_ADDRESS(REG_SCI1_BASE))
#define hwp_sci2 ((HWP_SCI_T *)REG_ACCESS_ADDRESS(REG_SCI2_BASE))

// sci_config
typedef union {
    uint32_t v;
    struct
    {
        uint32_t enable : 1;         // [0]
        uint32_t parity : 1;         // [1]
        uint32_t perf : 1;           // [2]
        uint32_t filter_disable : 1; // [3]
        uint32_t clockstop : 1;      // [4]
        uint32_t autostop_en_h : 1;  // [5]
        uint32_t msbh_lsbl : 1;      // [6]
        uint32_t lli : 1;            // [7]
        uint32_t pegen_len : 1;      // [8]
        uint32_t parity_en : 1;      // [9]
        uint32_t stop_level : 1;     // [10]
        uint32_t __15_11 : 5;        // [15:11]
        uint32_t arg_h : 1;          // [16]
        uint32_t afd_en_h : 1;       // [17]
        uint32_t tx_resend_en_h : 1; // [18]
        uint32_t __19_19 : 1;        // [19]
        uint32_t reset : 1;          // [20]
        uint32_t dly_sel : 1;        // [21]
        uint32_t in_avg_en : 1;      // [22]
        uint32_t __23_23 : 1;        // [23]
        uint32_t par_chk_offset : 6; // [29:24]
        uint32_t __31_30 : 2;        // [31:30]
    } b;
} REG_SCI_SCI_CONFIG_T;

// status
typedef union {
    uint32_t v;
    struct
    {
        uint32_t rxdata_rdy : 1;     // [0], read only
        uint32_t tx_fifo_rdy : 1;    // [1], read only
        uint32_t format_det : 1;     // [2], read only
        uint32_t arg_det : 1;        // [3], read only
        uint32_t reset_det : 1;      // [4], read only
        uint32_t clk_rdy_h : 1;      // [5], read only
        uint32_t clk_off : 1;        // [6], read only
        uint32_t __7_7 : 1;          // [7]
        uint32_t rx_err : 1;         // [8], read only
        uint32_t tx_err : 1;         // [9], read only
        uint32_t rxoverflow : 1;     // [10], read only
        uint32_t txoverflow : 1;     // [11], read only
        uint32_t __29_12 : 18;       // [29:12]
        uint32_t autostop_state : 2; // [31:30], read only
    } b;
} REG_SCI_STATUS_T;

// data
typedef union {
    uint32_t v;
    struct
    {
        uint32_t data_in : 8;  // [7:0]
        uint32_t data_out : 8; // [7:0], read only
        uint32_t __31_8 : 24;  // [31:8]
    } b;
} REG_SCI_DATA_T;

// clkdiv_reg
typedef union {
    uint32_t v;
    struct
    {
        uint32_t clkdiv : 9;           // [8:0]
        uint32_t baud_x8_en : 1;       // [9]
        uint32_t rx_clk_cnt_limit : 5; // [14:10]
        uint32_t clk_tst : 1;          // [15]
        uint32_t clkdiv_16 : 8;        // [23:16]
        uint32_t maindiv : 6;          // [29:24]
        uint32_t clk_out_inv : 1;      // [30]
        uint32_t clk_inv : 1;          // [31]
    } b;
} REG_SCI_CLKDIV_REG_T;

// rxcnt_reg
typedef union {
    uint32_t v;
    struct
    {
        uint32_t rxcnt : 10;      // [9:0]
        uint32_t __30_10 : 21;    // [30:10]
        uint32_t clk_persist : 1; // [31]
    } b;
} REG_SCI_RXCNT_REG_T;

// times
typedef union {
    uint32_t v;
    struct
    {
        uint32_t chguard : 8;         // [7:0]
        uint32_t turnaroundguard : 4; // [11:8]
        uint32_t __15_12 : 4;         // [15:12]
        uint32_t wi : 8;              // [23:16]
        uint32_t tx_pert : 8;         // [31:24]
    } b;
} REG_SCI_TIMES_T;

// ch_filt_reg
typedef union {
    uint32_t v;
    struct
    {
        uint32_t ch_filt : 8; // [7:0]
        uint32_t __31_8 : 24; // [31:8]
    } b;
} REG_SCI_CH_FILT_REG_T;

// dbg
typedef union {
    uint32_t v;
    struct
    {
        uint32_t fifo_rx_clr : 1; // [0]
        uint32_t fifo_tx_clr : 1; // [1]
        uint32_t __31_2 : 30;     // [31:2]
    } b;
} REG_SCI_DBG_T;

// int_cause
typedef union {
    uint32_t v;
    struct
    {
        uint32_t rx_done : 1;         // [0], read only
        uint32_t rx_half : 1;         // [1], read only
        uint32_t wwt_timeout : 1;     // [2], read only
        uint32_t extra_rx : 1;        // [3], read only
        uint32_t resend_ovfl : 1;     // [4], read only
        uint32_t arg_end : 1;         // [5], read only
        uint32_t sci_dma_tx_done : 1; // [6], read only
        uint32_t sci_dma_rx_done : 1; // [7], read only
        uint32_t __31_8 : 24;         // [31:8]
    } b;
} REG_SCI_INT_CAUSE_T;

// int_clr
typedef union {
    uint32_t v;
    struct
    {
        uint32_t rx_done : 1;         // [0], write clear
        uint32_t rx_half : 1;         // [1], write clear
        uint32_t wwt_timeout : 1;     // [2], write clear
        uint32_t extra_rx : 1;        // [3], write clear
        uint32_t resend_ovfl : 1;     // [4], write clear
        uint32_t arg_end : 1;         // [5], write clear
        uint32_t sci_dma_tx_done : 1; // [6], write clear
        uint32_t sci_dma_rx_done : 1; // [7], write clear
        uint32_t __31_8 : 24;         // [31:8]
    } b;
} REG_SCI_INT_CLR_T;

// int_mask
typedef union {
    uint32_t v;
    struct
    {
        uint32_t rx_done : 1;         // [0]
        uint32_t rx_half : 1;         // [1]
        uint32_t wwt_timeout : 1;     // [2]
        uint32_t extra_rx : 1;        // [3]
        uint32_t resend_ovfl : 1;     // [4]
        uint32_t arg_end : 1;         // [5]
        uint32_t sci_dma_tx_done : 1; // [6]
        uint32_t sci_dma_rx_done : 1; // [7]
        uint32_t __31_8 : 24;         // [31:8]
    } b;
} REG_SCI_INT_MASK_T;

// sci_config
#define SCI_ENABLE (1 << 0)
#define SCI_PARITY_EVEN_PARITY (0 << 1)
#define SCI_PARITY_ODD_PARITY (1 << 1)
#define SCI_PERF (1 << 2)
#define SCI_FILTER_DISABLE (1 << 3)
#define SCI_CLOCKSTOP (1 << 4)
#define SCI_AUTOSTOP_EN_H (1 << 5)
#define SCI_MSBH_LSBL (1 << 6)
#define SCI_LLI (1 << 7)
#define SCI_PEGEN_LEN (1 << 8)
#define SCI_PARITY_EN (1 << 9)
#define SCI_STOP_LEVEL (1 << 10)
#define SCI_ARG_H (1 << 16)
#define SCI_AFD_EN_H (1 << 17)
#define SCI_TX_RESEND_EN_H (1 << 18)
#define SCI_RESET (1 << 20)
#define SCI_DLY_SEL (1 << 21)
#define SCI_IN_AVG_EN (1 << 22)
#define SCI_PAR_CHK_OFFSET(n) (((n)&0x3f) << 24)

#define SCI_PARITY_V_EVEN_PARITY (0)
#define SCI_PARITY_V_ODD_PARITY (1)

// status
#define SCI_RXDATA_RDY (1 << 0)
#define SCI_TX_FIFO_RDY (1 << 1)
#define SCI_FORMAT_DET (1 << 2)
#define SCI_ARG_DET (1 << 3)
#define SCI_RESET_DET (1 << 4)
#define SCI_CLK_RDY_H (1 << 5)
#define SCI_CLK_OFF (1 << 6)
#define SCI_RX_ERR (1 << 8)
#define SCI_TX_ERR (1 << 9)
#define SCI_RXOVERFLOW (1 << 10)
#define SCI_TXOVERFLOW (1 << 11)
#define SCI_AUTOSTOP_STATE(n) (((n)&0x3) << 30)
#define SCI_AUTOSTOP_STATE_STARTUP_PHASE (0 << 30)
#define SCI_AUTOSTOP_STATE_AUTO_ON (1 << 30)
#define SCI_AUTOSTOP_STATE_SHUTDOWN_PHASE (2 << 30)
#define SCI_AUTOSTOP_STATE_CLOCK_OFF (3 << 30)

#define SCI_AUTOSTOP_STATE_V_STARTUP_PHASE (0)
#define SCI_AUTOSTOP_STATE_V_AUTO_ON (1)
#define SCI_AUTOSTOP_STATE_V_SHUTDOWN_PHASE (2)
#define SCI_AUTOSTOP_STATE_V_CLOCK_OFF (3)

// data
#define SCI_DATA_IN(n) (((n)&0xff) << 0)
#define SCI_DATA_OUT(n) (((n)&0xff) << 0)

// clkdiv_reg
#define SCI_CLKDIV(n) (((n)&0x1ff) << 0)
#define SCI_BAUD_X8_EN (1 << 9)
#define SCI_RX_CLK_CNT_LIMIT(n) (((n)&0x1f) << 10)
#define SCI_CLK_TST (1 << 15)
#define SCI_CLKDIV_16(n) (((n)&0xff) << 16)
#define SCI_MAINDIV(n) (((n)&0x3f) << 24)
#define SCI_CLK_OUT_INV (1 << 30)
#define SCI_CLK_INV (1 << 31)

// rxcnt_reg
#define SCI_RXCNT(n) (((n)&0x3ff) << 0)
#define SCI_CLK_PERSIST (1 << 31)

// times
#define SCI_CHGUARD(n) (((n)&0xff) << 0)
#define SCI_TURNAROUNDGUARD(n) (((n)&0xf) << 8)
#define SCI_WI(n) (((n)&0xff) << 16)
#define SCI_TX_PERT(n) (((n)&0xff) << 24)

// ch_filt_reg
#define SCI_CH_FILT(n) (((n)&0xff) << 0)

// dbg
#define SCI_FIFO_RX_CLR (1 << 0)
#define SCI_FIFO_TX_CLR (1 << 1)

// int_cause
#define SCI_RX_DONE (1 << 0)
#define SCI_RX_HALF (1 << 1)
#define SCI_WWT_TIMEOUT (1 << 2)
#define SCI_EXTRA_RX (1 << 3)
#define SCI_RESEND_OVFL (1 << 4)
#define SCI_ARG_END (1 << 5)
#define SCI_SCI_DMA_TX_DONE (1 << 6)
#define SCI_SCI_DMA_RX_DONE (1 << 7)

// int_clr
#define SCI_RX_DONE (1 << 0)
#define SCI_RX_HALF (1 << 1)
#define SCI_WWT_TIMEOUT (1 << 2)
#define SCI_EXTRA_RX (1 << 3)
#define SCI_RESEND_OVFL (1 << 4)
#define SCI_ARG_END (1 << 5)
#define SCI_SCI_DMA_TX_DONE (1 << 6)
#define SCI_SCI_DMA_RX_DONE (1 << 7)

// int_mask
#define SCI_RX_DONE (1 << 0)
#define SCI_RX_HALF (1 << 1)
#define SCI_WWT_TIMEOUT (1 << 2)
#define SCI_EXTRA_RX (1 << 3)
#define SCI_RESEND_OVFL (1 << 4)
#define SCI_ARG_END (1 << 5)
#define SCI_SCI_DMA_TX_DONE (1 << 6)
#define SCI_SCI_DMA_RX_DONE (1 << 7)

#endif // _SCI_H_
