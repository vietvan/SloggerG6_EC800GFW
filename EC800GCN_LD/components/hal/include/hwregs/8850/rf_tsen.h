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

#ifndef _RF_TSEN_H_
#define _RF_TSEN_H_

// Auto generated by dtools(see dtools.txt for its version).
// Don't edit it manually!

#define REG_RF_TSEN_SET_OFFSET (1024)
#define REG_RF_TSEN_CLR_OFFSET (2048)

#define REG_RF_TSEN_BASE (0x5003b000)

typedef volatile struct
{
    uint32_t tst_tsen_bist_cfg;          // 0x00000000
    uint32_t tst_tsen_bist_time_sel;     // 0x00000004
    uint32_t tst_bist_sel;               // 0x00000008
    uint32_t tst_bist_res;               // 0x0000000c
    uint32_t tst_tsen_c0_test_res0;      // 0x00000010
    uint32_t tst_tsen_c0_test_res1;      // 0x00000014
    uint32_t tst_tsen_c0_test_res2;      // 0x00000018
    uint32_t tst_tsen_c0_test_res3;      // 0x0000001c
    uint32_t tst_tsen_c1_test_res0;      // 0x00000020
    uint32_t tst_tsen_c1_test_res1;      // 0x00000024
    uint32_t tst_tsen_c1_test_res2;      // 0x00000028
    uint32_t tst_tsen_c1_test_res3;      // 0x0000002c
    uint32_t tst_tsen_c2_test_res0;      // 0x00000030
    uint32_t tst_tsen_c2_test_res1;      // 0x00000034
    uint32_t tst_tsen_c2_test_res2;      // 0x00000038
    uint32_t tst_tsen_c2_test_res3;      // 0x0000003c
    uint32_t tst_tsen_c3_test_res0;      // 0x00000040
    uint32_t tst_tsen_c3_test_res1;      // 0x00000044
    uint32_t tst_tsen_c3_test_res2;      // 0x00000048
    uint32_t tst_tsen_c3_test_res3;      // 0x0000004c
    uint32_t __80[236];                  // 0x00000050
    uint32_t tst_tsen_bist_cfg_set;      // 0x00000400
    uint32_t tst_tsen_bist_time_sel_set; // 0x00000404
    uint32_t tst_bist_sel_set;           // 0x00000408
    uint32_t __1036[253];                // 0x0000040c
    uint32_t tst_tsen_bist_cfg_clr;      // 0x00000800
    uint32_t tst_tsen_bist_time_sel_clr; // 0x00000804
    uint32_t tst_bist_sel_clr;           // 0x00000808
} HWP_RF_TSEN_T;

#define hwp_rfTsen ((HWP_RF_TSEN_T *)REG_ACCESS_ADDRESS(REG_RF_TSEN_BASE))

// tst_tsen_bist_cfg
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __0_0 : 1;                // [0]
        uint32_t tst_tsen_bist_cfg3 : 3;   // [3:1]
        uint32_t tst_tsen_bist_cfg2 : 3;   // [6:4]
        uint32_t tst_tsen_bist_cfg1 : 3;   // [9:7]
        uint32_t tst_tsen_bist_cfg0 : 3;   // [12:10]
        uint32_t tst_tsen_bist_bypass : 1; // [13]
        uint32_t __31_14 : 18;             // [31:14]
    } b;
} REG_RF_TSEN_TST_TSEN_BIST_CFG_T;

// tst_tsen_bist_time_sel
typedef union {
    uint32_t v;
    struct
    {
        uint32_t tst_tsen_bist_en : 1;            // [0]
        uint32_t tst_tsen_bist_time_sel_cfg0 : 2; // [2:1]
        uint32_t tst_tsen_bist_time_sel_cfg1 : 2; // [4:3]
        uint32_t tst_tsen_bist_time_sel_cfg2 : 2; // [6:5]
        uint32_t tst_tsen_bist_time_sel_cfg3 : 2; // [8:7]
        uint32_t tst_tsen_bist_code_sel : 1;      // [9]
        uint32_t tst_tsen_bist_code_in : 3;       // [12:10]
        uint32_t __31_13 : 19;                    // [31:13]
    } b;
} REG_RF_TSEN_TST_TSEN_BIST_TIME_SEL_T;

// tst_bist_sel
typedef union {
    uint32_t v;
    struct
    {
        uint32_t tsen_adc_rst_sel_src : 1;  // [0]
        uint32_t tsen_adc_ch_sel_src : 1;   // [1]
        uint32_t tsen_adc_ch_sel_pad : 1;   // [2]
        uint32_t pad_ie_tsx_clk : 1;        // [3]
        uint32_t pad_oe_tsx_clk : 1;        // [4]
        uint32_t pad_ie_tsx_data : 1;       // [5]
        uint32_t pad_oe_tsx_data : 1;       // [6]
        uint32_t pad_ie_tsx_adc_ch_sel : 1; // [7]
        uint32_t pad_oe_tsx_adc_ch_sel : 1; // [8]
        uint32_t pad_ie_osc_clk : 1;        // [9]
        uint32_t pad_oe_osc_clk : 1;        // [10]
        uint32_t pad_ie_osc_data : 1;       // [11]
        uint32_t pad_oe_osc_data : 1;       // [12]
        uint32_t __31_13 : 19;              // [31:13]
    } b;
} REG_RF_TSEN_TST_BIST_SEL_T;

// tst_bist_res
typedef union {
    uint32_t v;
    struct
    {
        uint32_t tst_tsen_bist_done : 1; // [0], read only
        uint32_t tsen_bist_code : 3;     // [3:1], read only
        uint32_t __31_4 : 28;            // [31:4]
    } b;
} REG_RF_TSEN_TST_BIST_RES_T;

// tst_tsen_c0_test_res0
typedef union {
    uint32_t v;
    struct
    {
        uint32_t tst_tsen_c0_res0 : 16; // [15:0], read only
        uint32_t __31_16 : 16;          // [31:16]
    } b;
} REG_RF_TSEN_TST_TSEN_C0_TEST_RES0_T;

// tst_tsen_c0_test_res1
typedef union {
    uint32_t v;
    struct
    {
        uint32_t tst_tsen_c0_res1 : 16; // [15:0], read only
        uint32_t __31_16 : 16;          // [31:16]
    } b;
} REG_RF_TSEN_TST_TSEN_C0_TEST_RES1_T;

// tst_tsen_c0_test_res2
typedef union {
    uint32_t v;
    struct
    {
        uint32_t tst_tsen_c0_res2 : 16; // [15:0], read only
        uint32_t __31_16 : 16;          // [31:16]
    } b;
} REG_RF_TSEN_TST_TSEN_C0_TEST_RES2_T;

// tst_tsen_c0_test_res3
typedef union {
    uint32_t v;
    struct
    {
        uint32_t tst_tsen_c0_res3 : 16; // [15:0], read only
        uint32_t __31_16 : 16;          // [31:16]
    } b;
} REG_RF_TSEN_TST_TSEN_C0_TEST_RES3_T;

// tst_tsen_c1_test_res0
typedef union {
    uint32_t v;
    struct
    {
        uint32_t tst_tsen_c1_res0 : 16; // [15:0], read only
        uint32_t __31_16 : 16;          // [31:16]
    } b;
} REG_RF_TSEN_TST_TSEN_C1_TEST_RES0_T;

// tst_tsen_c1_test_res1
typedef union {
    uint32_t v;
    struct
    {
        uint32_t tst_tsen_c1_res1 : 16; // [15:0], read only
        uint32_t __31_16 : 16;          // [31:16]
    } b;
} REG_RF_TSEN_TST_TSEN_C1_TEST_RES1_T;

// tst_tsen_c1_test_res2
typedef union {
    uint32_t v;
    struct
    {
        uint32_t tst_tsen_c1_res2 : 16; // [15:0], read only
        uint32_t __31_16 : 16;          // [31:16]
    } b;
} REG_RF_TSEN_TST_TSEN_C1_TEST_RES2_T;

// tst_tsen_c1_test_res3
typedef union {
    uint32_t v;
    struct
    {
        uint32_t tst_tsen_c1_res3 : 16; // [15:0], read only
        uint32_t __31_16 : 16;          // [31:16]
    } b;
} REG_RF_TSEN_TST_TSEN_C1_TEST_RES3_T;

// tst_tsen_c2_test_res0
typedef union {
    uint32_t v;
    struct
    {
        uint32_t tst_tsen_c2_res0 : 16; // [15:0], read only
        uint32_t __31_16 : 16;          // [31:16]
    } b;
} REG_RF_TSEN_TST_TSEN_C2_TEST_RES0_T;

// tst_tsen_c2_test_res1
typedef union {
    uint32_t v;
    struct
    {
        uint32_t tst_tsen_c2_res1 : 16; // [15:0], read only
        uint32_t __31_16 : 16;          // [31:16]
    } b;
} REG_RF_TSEN_TST_TSEN_C2_TEST_RES1_T;

// tst_tsen_c2_test_res2
typedef union {
    uint32_t v;
    struct
    {
        uint32_t tst_tsen_c2_res2 : 16; // [15:0], read only
        uint32_t __31_16 : 16;          // [31:16]
    } b;
} REG_RF_TSEN_TST_TSEN_C2_TEST_RES2_T;

// tst_tsen_c2_test_res3
typedef union {
    uint32_t v;
    struct
    {
        uint32_t tst_tsen_c2_res3 : 16; // [15:0], read only
        uint32_t __31_16 : 16;          // [31:16]
    } b;
} REG_RF_TSEN_TST_TSEN_C2_TEST_RES3_T;

// tst_tsen_c3_test_res0
typedef union {
    uint32_t v;
    struct
    {
        uint32_t tst_tsen_c3_res0 : 16; // [15:0], read only
        uint32_t __31_16 : 16;          // [31:16]
    } b;
} REG_RF_TSEN_TST_TSEN_C3_TEST_RES0_T;

// tst_tsen_c3_test_res1
typedef union {
    uint32_t v;
    struct
    {
        uint32_t tst_tsen_c3_res1 : 16; // [15:0], read only
        uint32_t __31_16 : 16;          // [31:16]
    } b;
} REG_RF_TSEN_TST_TSEN_C3_TEST_RES1_T;

// tst_tsen_c3_test_res2
typedef union {
    uint32_t v;
    struct
    {
        uint32_t tst_tsen_c3_res2 : 16; // [15:0], read only
        uint32_t __31_16 : 16;          // [31:16]
    } b;
} REG_RF_TSEN_TST_TSEN_C3_TEST_RES2_T;

// tst_tsen_c3_test_res3
typedef union {
    uint32_t v;
    struct
    {
        uint32_t tst_tsen_c3_res3 : 16; // [15:0], read only
        uint32_t __31_16 : 16;          // [31:16]
    } b;
} REG_RF_TSEN_TST_TSEN_C3_TEST_RES3_T;

// tst_tsen_bist_cfg
#define RF_TSEN_TST_TSEN_BIST_CFG3(n) (((n)&0x7) << 1)
#define RF_TSEN_TST_TSEN_BIST_CFG2(n) (((n)&0x7) << 4)
#define RF_TSEN_TST_TSEN_BIST_CFG1(n) (((n)&0x7) << 7)
#define RF_TSEN_TST_TSEN_BIST_CFG0(n) (((n)&0x7) << 10)
#define RF_TSEN_TST_TSEN_BIST_BYPASS (1 << 13)

// tst_tsen_bist_time_sel
#define RF_TSEN_TST_TSEN_BIST_EN (1 << 0)
#define RF_TSEN_TST_TSEN_BIST_TIME_SEL_CFG0(n) (((n)&0x3) << 1)
#define RF_TSEN_TST_TSEN_BIST_TIME_SEL_CFG1(n) (((n)&0x3) << 3)
#define RF_TSEN_TST_TSEN_BIST_TIME_SEL_CFG2(n) (((n)&0x3) << 5)
#define RF_TSEN_TST_TSEN_BIST_TIME_SEL_CFG3(n) (((n)&0x3) << 7)
#define RF_TSEN_TST_TSEN_BIST_CODE_SEL (1 << 9)
#define RF_TSEN_TST_TSEN_BIST_CODE_IN(n) (((n)&0x7) << 10)

// tst_bist_sel
#define RF_TSEN_TSEN_ADC_RST_SEL_SRC (1 << 0)
#define RF_TSEN_TSEN_ADC_CH_SEL_SRC (1 << 1)
#define RF_TSEN_TSEN_ADC_CH_SEL_PAD (1 << 2)
#define RF_TSEN_PAD_IE_TSX_CLK (1 << 3)
#define RF_TSEN_PAD_OE_TSX_CLK (1 << 4)
#define RF_TSEN_PAD_IE_TSX_DATA (1 << 5)
#define RF_TSEN_PAD_OE_TSX_DATA (1 << 6)
#define RF_TSEN_PAD_IE_TSX_ADC_CH_SEL (1 << 7)
#define RF_TSEN_PAD_OE_TSX_ADC_CH_SEL (1 << 8)
#define RF_TSEN_PAD_IE_OSC_CLK (1 << 9)
#define RF_TSEN_PAD_OE_OSC_CLK (1 << 10)
#define RF_TSEN_PAD_IE_OSC_DATA (1 << 11)
#define RF_TSEN_PAD_OE_OSC_DATA (1 << 12)

// tst_bist_res
#define RF_TSEN_TST_TSEN_BIST_DONE (1 << 0)
#define RF_TSEN_TSEN_BIST_CODE(n) (((n)&0x7) << 1)

// tst_tsen_c0_test_res0
#define RF_TSEN_TST_TSEN_C0_RES0(n) (((n)&0xffff) << 0)

// tst_tsen_c0_test_res1
#define RF_TSEN_TST_TSEN_C0_RES1(n) (((n)&0xffff) << 0)

// tst_tsen_c0_test_res2
#define RF_TSEN_TST_TSEN_C0_RES2(n) (((n)&0xffff) << 0)

// tst_tsen_c0_test_res3
#define RF_TSEN_TST_TSEN_C0_RES3(n) (((n)&0xffff) << 0)

// tst_tsen_c1_test_res0
#define RF_TSEN_TST_TSEN_C1_RES0(n) (((n)&0xffff) << 0)

// tst_tsen_c1_test_res1
#define RF_TSEN_TST_TSEN_C1_RES1(n) (((n)&0xffff) << 0)

// tst_tsen_c1_test_res2
#define RF_TSEN_TST_TSEN_C1_RES2(n) (((n)&0xffff) << 0)

// tst_tsen_c1_test_res3
#define RF_TSEN_TST_TSEN_C1_RES3(n) (((n)&0xffff) << 0)

// tst_tsen_c2_test_res0
#define RF_TSEN_TST_TSEN_C2_RES0(n) (((n)&0xffff) << 0)

// tst_tsen_c2_test_res1
#define RF_TSEN_TST_TSEN_C2_RES1(n) (((n)&0xffff) << 0)

// tst_tsen_c2_test_res2
#define RF_TSEN_TST_TSEN_C2_RES2(n) (((n)&0xffff) << 0)

// tst_tsen_c2_test_res3
#define RF_TSEN_TST_TSEN_C2_RES3(n) (((n)&0xffff) << 0)

// tst_tsen_c3_test_res0
#define RF_TSEN_TST_TSEN_C3_RES0(n) (((n)&0xffff) << 0)

// tst_tsen_c3_test_res1
#define RF_TSEN_TST_TSEN_C3_RES1(n) (((n)&0xffff) << 0)

// tst_tsen_c3_test_res2
#define RF_TSEN_TST_TSEN_C3_RES2(n) (((n)&0xffff) << 0)

// tst_tsen_c3_test_res3
#define RF_TSEN_TST_TSEN_C3_RES3(n) (((n)&0xffff) << 0)

#endif // _RF_TSEN_H_