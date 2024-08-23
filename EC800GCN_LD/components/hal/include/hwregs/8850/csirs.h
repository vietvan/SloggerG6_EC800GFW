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

#ifndef _CSIRS_H_
#define _CSIRS_H_

// Auto generated by dtools(see dtools.txt for its version).
// Don't edit it manually!

#define REG_CSIRS_BASE (0x18b00000)

typedef volatile struct
{
    uint32_t csi_start;            // 0x00000000
    uint32_t csi_cfg_nxt;          // 0x00000004
    uint32_t csi_ri_threshold_nxt; // 0x00000008
    uint32_t csi_code_index1_nxt;  // 0x0000000c
    uint32_t csi_code_index2_nxt;  // 0x00000010
    uint32_t csi_inten_nxt;        // 0x00000014
    uint32_t csi_cinit1_nxt;       // 0x00000018
    uint32_t csi_cinit2_nxt;       // 0x0000001c
    uint32_t csi_intf;             // 0x00000020
    uint32_t csi_sw_stop;          // 0x00000024
    uint32_t csi_sw_stop_flag;     // 0x00000028
    uint32_t csi_ri_rpt;           // 0x0000002c
    uint32_t csi_pmi_rpt;          // 0x00000030
    uint32_t csi_rx1_sig_rpt;      // 0x00000034
    uint32_t csi_rx2_sig_rpt;      // 0x00000038
    uint32_t csi_rx1_noise_rpt;    // 0x0000003c
    uint32_t csi_rx2_noise_rpt;    // 0x00000040
    uint32_t csi_cfg_cur;          // 0x00000044
    uint32_t csi_ri_threshold_cur; // 0x00000048
    uint32_t csi_code_index1_cur;  // 0x0000004c
    uint32_t csi_code_index2_cur;  // 0x00000050
    uint32_t csi_inten_cur;        // 0x00000054
    uint32_t csi_cinit1_cur;       // 0x00000058
    uint32_t csi_cinit2_cur;       // 0x0000005c
    uint32_t __96[8168];           // 0x00000060
    uint32_t rs_fh_mem1;           // 0x00008000
    uint32_t __32772[1023];        // 0x00008004
    uint32_t rs_fh_mem2;           // 0x00009000
    uint32_t __36868[1023];        // 0x00009004
    uint32_t hls_mem1;             // 0x0000a000
    uint32_t __40964[2047];        // 0x0000a004
    uint32_t out_mem;              // 0x0000c000
} HWP_CSIRS_T;

#define hwp_csirs ((HWP_CSIRS_T *)REG_ACCESS_ADDRESS(REG_CSIRS_BASE))

// csi_start
typedef union {
    uint32_t v;
    struct
    {
        uint32_t csi_en : 1;        // [0]
        uint32_t dma_start_en : 1;  // [1]
        uint32_t data_drive_en : 1; // [2]
        uint32_t __31_3 : 29;       // [31:3]
    } b;
} REG_CSIRS_CSI_START_T;

// csi_cfg_nxt
typedef union {
    uint32_t v;
    struct
    {
        uint32_t tx_num : 2;      // [1:0]
        uint32_t sub_nrb : 4;     // [5:2]
        uint32_t total_nrb : 7;   // [12:6]
        uint32_t old_ri_ind : 1;  // [13]
        uint32_t ri_en : 1;       // [14]
        uint32_t pmi_en : 1;      // [15]
        uint32_t ri_sel : 1;      // [16]
        uint32_t sw_ri : 1;       // [17]
        uint32_t ls_en : 1;       // [18]
        uint32_t csi_crs_ind : 1; // [19]
        uint32_t fh_bit_sel : 5;  // [24:20]
        uint32_t cp : 1;          // [25]
        uint32_t __31_26 : 6;     // [31:26]
    } b;
} REG_CSIRS_CSI_CFG_NXT_T;

// csi_ri_threshold_nxt
typedef union {
    uint32_t v;
    struct
    {
        uint32_t th1_cfg : 15; // [14:0]
        uint32_t __15_15 : 1;  // [15]
        uint32_t th2_cfg : 15; // [30:16]
        uint32_t __31_31 : 1;  // [31]
    } b;
} REG_CSIRS_CSI_RI_THRESHOLD_NXT_T;

// csi_code_index1_nxt
typedef union {
    uint32_t v;
    struct
    {
        uint32_t code_index1_mask1 : 16; // [15:0]
        uint32_t code_index1_mask2 : 16; // [31:16]
    } b;
} REG_CSIRS_CSI_CODE_INDEX1_NXT_T;

// csi_code_index2_nxt
typedef union {
    uint32_t v;
    struct
    {
        uint32_t code_index2_mask1 : 16; // [15:0]
        uint32_t code_index2_mask2 : 16; // [31:16]
    } b;
} REG_CSIRS_CSI_CODE_INDEX2_NXT_T;

// csi_inten_nxt
typedef union {
    uint32_t v;
    struct
    {
        uint32_t csi_inten : 1;  // [0]
        uint32_t __3_1 : 3;      // [3:1]
        uint32_t phy_factor : 4; // [7:4]
        uint32_t __31_8 : 24;    // [31:8]
    } b;
} REG_CSIRS_CSI_INTEN_NXT_T;

// csi_cinit1_nxt
typedef union {
    uint32_t v;
    struct
    {
        uint32_t cinit0 : 31; // [30:0]
        uint32_t __31_31 : 1; // [31]
    } b;
} REG_CSIRS_CSI_CINIT1_NXT_T;

// csi_cinit2_nxt
typedef union {
    uint32_t v;
    struct
    {
        uint32_t cinit1 : 31; // [30:0]
        uint32_t __31_31 : 1; // [31]
    } b;
} REG_CSIRS_CSI_CINIT2_NXT_T;

// csi_intf
typedef union {
    uint32_t v;
    struct
    {
        uint32_t csi_intf : 1;   // [0], write clear
        uint32_t __3_1 : 3;      // [3:1]
        uint32_t phy_factor : 4; // [7:4], write clear
        uint32_t __31_8 : 24;    // [31:8]
    } b;
} REG_CSIRS_CSI_INTF_T;

// csi_sw_stop
typedef union {
    uint32_t v;
    struct
    {
        uint32_t sw_stop_en : 1;   // [0]
        uint32_t sw_pause_en : 1;  // [1]
        uint32_t sw_pause_way : 1; // [2]
        uint32_t __31_3 : 29;      // [31:3]
    } b;
} REG_CSIRS_CSI_SW_STOP_T;

// csi_sw_stop_flag
typedef union {
    uint32_t v;
    struct
    {
        uint32_t sw_stop_flag : 1;  // [0], write clear
        uint32_t sw_pause_flag : 1; // [1], write clear
        uint32_t __31_2 : 30;       // [31:2]
    } b;
} REG_CSIRS_CSI_SW_STOP_FLAG_T;

// csi_ri_rpt
typedef union {
    uint32_t v;
    struct
    {
        uint32_t ri_total_rpt : 1; // [0], read only
        uint32_t __31_1 : 31;      // [31:1]
    } b;
} REG_CSIRS_CSI_RI_RPT_T;

// csi_pmi_rpt
typedef union {
    uint32_t v;
    struct
    {
        uint32_t pmi_total_rpt : 8; // [7:0], read only
        uint32_t __31_8 : 24;       // [31:8]
    } b;
} REG_CSIRS_CSI_PMI_RPT_T;

// csi_rx1_sig_rpt
typedef union {
    uint32_t v;
    struct
    {
        uint32_t rx1_sig_rpt : 28; // [27:0], read only
        uint32_t __31_28 : 4;      // [31:28]
    } b;
} REG_CSIRS_CSI_RX1_SIG_RPT_T;

// csi_rx2_sig_rpt
typedef union {
    uint32_t v;
    struct
    {
        uint32_t rx2_sig_rpt : 28; // [27:0], read only
        uint32_t __31_28 : 4;      // [31:28]
    } b;
} REG_CSIRS_CSI_RX2_SIG_RPT_T;

// csi_rx1_noise_rpt
typedef union {
    uint32_t v;
    struct
    {
        uint32_t rx1_noise_rpt : 30; // [29:0], read only
        uint32_t __31_30 : 2;        // [31:30]
    } b;
} REG_CSIRS_CSI_RX1_NOISE_RPT_T;

// csi_rx2_noise_rpt
typedef union {
    uint32_t v;
    struct
    {
        uint32_t rx2_noise_rpt : 30; // [29:0], read only
        uint32_t __31_30 : 2;        // [31:30]
    } b;
} REG_CSIRS_CSI_RX2_NOISE_RPT_T;

// csi_cfg_cur
typedef union {
    uint32_t v;
    struct
    {
        uint32_t tx_num : 2;      // [1:0]
        uint32_t sub_nrb : 4;     // [5:2]
        uint32_t total_nrb : 7;   // [12:6]
        uint32_t old_ri_ind : 1;  // [13]
        uint32_t ri_en : 1;       // [14]
        uint32_t pmi_en : 1;      // [15]
        uint32_t ri_sel : 1;      // [16]
        uint32_t sw_ri : 1;       // [17]
        uint32_t ls_en : 1;       // [18]
        uint32_t csi_crs_ind : 1; // [19]
        uint32_t fh_bit_sel : 5;  // [24:20]
        uint32_t cp : 1;          // [25]
        uint32_t __31_26 : 6;     // [31:26]
    } b;
} REG_CSIRS_CSI_CFG_CUR_T;

// csi_ri_threshold_cur
typedef union {
    uint32_t v;
    struct
    {
        uint32_t th1_cfg : 15; // [14:0]
        uint32_t __15_15 : 1;  // [15]
        uint32_t th2_cfg : 15; // [30:16]
        uint32_t __31_31 : 1;  // [31]
    } b;
} REG_CSIRS_CSI_RI_THRESHOLD_CUR_T;

// csi_code_index1_cur
typedef union {
    uint32_t v;
    struct
    {
        uint32_t code_index1_mask1 : 16; // [15:0]
        uint32_t code_index1_mask2 : 16; // [31:16]
    } b;
} REG_CSIRS_CSI_CODE_INDEX1_CUR_T;

// csi_code_index2_cur
typedef union {
    uint32_t v;
    struct
    {
        uint32_t code_index2_mask1 : 16; // [15:0]
        uint32_t code_index2_mask2 : 16; // [31:16]
    } b;
} REG_CSIRS_CSI_CODE_INDEX2_CUR_T;

// csi_inten_cur
typedef union {
    uint32_t v;
    struct
    {
        uint32_t csi_inten : 1; // [0]
        uint32_t __31_1 : 31;   // [31:1]
    } b;
} REG_CSIRS_CSI_INTEN_CUR_T;

// csi_cinit1_cur
typedef union {
    uint32_t v;
    struct
    {
        uint32_t cinit0 : 31; // [30:0]
        uint32_t __31_31 : 1; // [31]
    } b;
} REG_CSIRS_CSI_CINIT1_CUR_T;

// csi_cinit2_cur
typedef union {
    uint32_t v;
    struct
    {
        uint32_t cinit1 : 31; // [30:0]
        uint32_t __31_31 : 1; // [31]
    } b;
} REG_CSIRS_CSI_CINIT2_CUR_T;

// rs_fh_mem1
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __3_0 : 4;         // [3:0]
        uint32_t rs_fh_mem1_2 : 12; // [15:4]
        uint32_t __19_16 : 4;       // [19:16]
        uint32_t rs_fh_mem1_1 : 12; // [31:20]
    } b;
} REG_CSIRS_RS_FH_MEM1_T;

// rs_fh_mem2
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __3_0 : 4;         // [3:0]
        uint32_t rs_fh_mem2_2 : 12; // [15:4]
        uint32_t __19_16 : 4;       // [19:16]
        uint32_t rs_fh_mem2_1 : 12; // [31:20]
    } b;
} REG_CSIRS_RS_FH_MEM2_T;

// hls_mem1
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __3_0 : 4;       // [3:0]
        uint32_t hls_mem1_2 : 12; // [15:4]
        uint32_t __19_16 : 4;     // [19:16]
        uint32_t hls_mem1_1 : 12; // [31:20]
    } b;
} REG_CSIRS_HLS_MEM1_T;

// csi_start
#define CSIRS_CSI_EN (1 << 0)
#define CSIRS_DMA_START_EN (1 << 1)
#define CSIRS_DATA_DRIVE_EN (1 << 2)

// csi_cfg_nxt
#define CSIRS_TX_NUM(n) (((n)&0x3) << 0)
#define CSIRS_SUB_NRB(n) (((n)&0xf) << 2)
#define CSIRS_TOTAL_NRB(n) (((n)&0x7f) << 6)
#define CSIRS_OLD_RI_IND (1 << 13)
#define CSIRS_RI_EN (1 << 14)
#define CSIRS_PMI_EN (1 << 15)
#define CSIRS_RI_SEL (1 << 16)
#define CSIRS_SW_RI (1 << 17)
#define CSIRS_LS_EN (1 << 18)
#define CSIRS_CSI_CRS_IND (1 << 19)
#define CSIRS_FH_BIT_SEL(n) (((n)&0x1f) << 20)
#define CSIRS_CP (1 << 25)

// csi_ri_threshold_nxt
#define CSIRS_TH1_CFG(n) (((n)&0x7fff) << 0)
#define CSIRS_TH2_CFG(n) (((n)&0x7fff) << 16)

// csi_code_index1_nxt
#define CSIRS_CODE_INDEX1_MASK1(n) (((n)&0xffff) << 0)
#define CSIRS_CODE_INDEX1_MASK2(n) (((n)&0xffff) << 16)

// csi_code_index2_nxt
#define CSIRS_CODE_INDEX2_MASK1(n) (((n)&0xffff) << 0)
#define CSIRS_CODE_INDEX2_MASK2(n) (((n)&0xffff) << 16)

// csi_inten_nxt
#define CSIRS_CSI_INTEN (1 << 0)
#define CSIRS_PHY_FACTOR(n) (((n)&0xf) << 4)

// csi_cinit1_nxt
#define CSIRS_CINIT0(n) (((n)&0x7fffffff) << 0)

// csi_cinit2_nxt
#define CSIRS_CINIT1(n) (((n)&0x7fffffff) << 0)

// csi_intf
#define CSIRS_CSI_INTF (1 << 0)
#define CSIRS_PHY_FACTOR(n) (((n)&0xf) << 4)

// csi_sw_stop
#define CSIRS_SW_STOP_EN (1 << 0)
#define CSIRS_SW_PAUSE_EN (1 << 1)
#define CSIRS_SW_PAUSE_WAY (1 << 2)

// csi_sw_stop_flag
#define CSIRS_SW_STOP_FLAG (1 << 0)
#define CSIRS_SW_PAUSE_FLAG (1 << 1)

// csi_ri_rpt
#define CSIRS_RI_TOTAL_RPT (1 << 0)

// csi_pmi_rpt
#define CSIRS_PMI_TOTAL_RPT(n) (((n)&0xff) << 0)

// csi_rx1_sig_rpt
#define CSIRS_RX1_SIG_RPT(n) (((n)&0xfffffff) << 0)

// csi_rx2_sig_rpt
#define CSIRS_RX2_SIG_RPT(n) (((n)&0xfffffff) << 0)

// csi_rx1_noise_rpt
#define CSIRS_RX1_NOISE_RPT(n) (((n)&0x3fffffff) << 0)

// csi_rx2_noise_rpt
#define CSIRS_RX2_NOISE_RPT(n) (((n)&0x3fffffff) << 0)

// csi_cfg_cur
#define CSIRS_TX_NUM(n) (((n)&0x3) << 0)
#define CSIRS_SUB_NRB(n) (((n)&0xf) << 2)
#define CSIRS_TOTAL_NRB(n) (((n)&0x7f) << 6)
#define CSIRS_OLD_RI_IND (1 << 13)
#define CSIRS_RI_EN (1 << 14)
#define CSIRS_PMI_EN (1 << 15)
#define CSIRS_RI_SEL (1 << 16)
#define CSIRS_SW_RI (1 << 17)
#define CSIRS_LS_EN (1 << 18)
#define CSIRS_CSI_CRS_IND (1 << 19)
#define CSIRS_FH_BIT_SEL(n) (((n)&0x1f) << 20)
#define CSIRS_CP (1 << 25)

// csi_ri_threshold_cur
#define CSIRS_TH1_CFG(n) (((n)&0x7fff) << 0)
#define CSIRS_TH2_CFG(n) (((n)&0x7fff) << 16)

// csi_code_index1_cur
#define CSIRS_CODE_INDEX1_MASK1(n) (((n)&0xffff) << 0)
#define CSIRS_CODE_INDEX1_MASK2(n) (((n)&0xffff) << 16)

// csi_code_index2_cur
#define CSIRS_CODE_INDEX2_MASK1(n) (((n)&0xffff) << 0)
#define CSIRS_CODE_INDEX2_MASK2(n) (((n)&0xffff) << 16)

// csi_inten_cur
#define CSIRS_CSI_INTEN (1 << 0)

// csi_cinit1_cur
#define CSIRS_CINIT0(n) (((n)&0x7fffffff) << 0)

// csi_cinit2_cur
#define CSIRS_CINIT1(n) (((n)&0x7fffffff) << 0)

// rs_fh_mem1
#define CSIRS_RS_FH_MEM1_2(n) (((n)&0xfff) << 4)
#define CSIRS_RS_FH_MEM1_1(n) (((n)&0xfff) << 20)

// rs_fh_mem2
#define CSIRS_RS_FH_MEM2_2(n) (((n)&0xfff) << 4)
#define CSIRS_RS_FH_MEM2_1(n) (((n)&0xfff) << 20)

// hls_mem1
#define CSIRS_HLS_MEM1_2(n) (((n)&0xfff) << 4)
#define CSIRS_HLS_MEM1_1(n) (((n)&0xfff) << 20)

#endif // _CSIRS_H_
