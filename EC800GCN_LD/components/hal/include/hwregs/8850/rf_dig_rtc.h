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

#ifndef _RF_DIG_RTC_H_
#define _RF_DIG_RTC_H_

// Auto generated by dtools(see dtools.txt for its version).
// Don't edit it manually!

#define REG_RF_DIG_RTC_SET_OFFSET (256)
#define REG_RF_DIG_RTC_CLR_OFFSET (512)

#define REG_RF_DIG_RTC_BASE (0x50034000)

typedef volatile struct
{
    uint32_t reg_00_reg;     // 0x00000000
    uint32_t reg_18_reg;     // 0x00000004
    uint32_t reg_1c_reg;     // 0x00000008
    uint32_t reg_c4_reg;     // 0x0000000c
    uint32_t reg_c8_reg;     // 0x00000010
    uint32_t reg_cc_reg;     // 0x00000014
    uint32_t reg_d0_reg;     // 0x00000018
    uint32_t reg_d4_reg;     // 0x0000001c
    uint32_t reg_d8_reg;     // 0x00000020
    uint32_t reg_dc_reg;     // 0x00000024
    uint32_t reg_e0_reg;     // 0x00000028
    uint32_t __44[53];       // 0x0000002c
    uint32_t reg_00_reg_set; // 0x00000100
    uint32_t reg_18_reg_set; // 0x00000104
    uint32_t reg_1c_reg_set; // 0x00000108
    uint32_t reg_c4_reg_set; // 0x0000010c
    uint32_t reg_c8_reg_set; // 0x00000110
    uint32_t reg_cc_reg_set; // 0x00000114
    uint32_t reg_d0_reg_set; // 0x00000118
    uint32_t reg_d4_reg_set; // 0x0000011c
    uint32_t reg_d8_reg_set; // 0x00000120
    uint32_t reg_dc_reg_set; // 0x00000124
    uint32_t reg_e0_reg_set; // 0x00000128
    uint32_t __300[53];      // 0x0000012c
    uint32_t reg_00_reg_clr; // 0x00000200
    uint32_t reg_18_reg_clr; // 0x00000204
    uint32_t reg_1c_reg_clr; // 0x00000208
    uint32_t reg_c4_reg_clr; // 0x0000020c
    uint32_t reg_c8_reg_clr; // 0x00000210
    uint32_t reg_cc_reg_clr; // 0x00000214
    uint32_t reg_d0_reg_clr; // 0x00000218
    uint32_t reg_d4_reg_clr; // 0x0000021c
    uint32_t reg_d8_reg_clr; // 0x00000220
    uint32_t reg_dc_reg_clr; // 0x00000224
    uint32_t reg_e0_reg_clr; // 0x00000228
} HWP_RF_DIG_RTC_T;

#define hwp_rfDigRtc ((HWP_RF_DIG_RTC_T *)REG_ACCESS_ADDRESS(REG_RF_DIG_RTC_BASE))

// reg_00_reg
typedef union {
    uint32_t v;
    struct
    {
        uint32_t pu_xtal_ana_sel_src : 1; // [0]
        uint32_t pu_xtal_reg : 1;         // [1]
        uint32_t xtal_sel_vrtc_vdig : 1;  // [2]
        uint32_t xtal_sel_vref_vdig : 1;  // [3]
        uint32_t step_offset_update : 1;  // [4]
        uint32_t __31_5 : 27;             // [31:5]
    } b;
} REG_RF_DIG_RTC_REG_00_REG_T;

// reg_18_reg
typedef union {
    uint32_t v;
    struct
    {
        uint32_t step_offset_lp : 8;     // [7:0]
        uint32_t step_offset_normal : 8; // [15:8]
        uint32_t __31_16 : 16;           // [31:16]
    } b;
} REG_RF_DIG_RTC_REG_18_REG_T;

// reg_1c_reg
typedef union {
    uint32_t v;
    struct
    {
        uint32_t lp_mode_h_reg : 1;       // [0]
        uint32_t lp_mode_h_dr : 1;        // [1]
        uint32_t change_reg_flag_reg : 1; // [2]
        uint32_t change_reg_flag_dr : 1;  // [3]
        uint32_t lp_mode_en_reg : 1;      // [4]
        uint32_t lp_mode_en_dr : 1;       // [5]
        uint32_t lp_mode_delay : 2;       // [7:6]
        uint32_t __31_8 : 24;             // [31:8]
    } b;
} REG_RF_DIG_RTC_REG_1C_REG_T;

// reg_c4_reg
typedef union {
    uint32_t v;
    struct
    {
        uint32_t xtal_osc_ibit_n : 4;        // [3:0]
        uint32_t xtal_osc_ibit_l : 4;        // [7:4]
        uint32_t xtal26m_pwadc_en : 1;       // [8]
        uint32_t xtal26m_pllcal_en : 1;      // [9]
        uint32_t xtal26m_interface_en : 1;   // [10]
        uint32_t xtal26m_plls1_en : 1;       // [11]
        uint32_t xtal26m_plls2_en : 1;       // [12]
        uint32_t enable_clk_26m : 1;         // [13]
        uint32_t enable_clk_26m_lp_uart : 1; // [14]
        uint32_t __31_15 : 17;               // [31:15]
    } b;
} REG_RF_DIG_RTC_REG_C4_REG_T;

// reg_c8_reg
typedef union {
    uint32_t v;
    struct
    {
        uint32_t xtal_fixi_bit_n : 6; // [5:0]
        uint32_t xtal_fixi_bit_l : 6; // [11:6]
        uint32_t xtal_cfix_bit_n : 1; // [12]
        uint32_t xtal_cfix_bit_l : 1; // [13]
        uint32_t __31_14 : 18;        // [31:14]
    } b;
} REG_RF_DIG_RTC_REG_C8_REG_T;

// reg_cc_reg
typedef union {
    uint32_t v;
    struct
    {
        uint32_t xdrv_reg_bit : 4;        // [3:0]
        uint32_t xtal_reg_bit : 4;        // [7:4]
        uint32_t xdrv_aux1_power_bit : 3; // [10:8]
        uint32_t __31_11 : 21;            // [31:11]
    } b;
} REG_RF_DIG_RTC_REG_CC_REG_T;

// reg_d0_reg
typedef union {
    uint32_t v;
    struct
    {
        uint32_t xtal_capbank_bit_n : 8; // [7:0]
        uint32_t xtal_capbank_bit_l : 8; // [15:8]
        uint32_t __31_16 : 16;           // [31:16]
    } b;
} REG_RF_DIG_RTC_REG_D0_REG_T;

// reg_d4_reg
typedef union {
    uint32_t v;
    struct
    {
        uint32_t rtc_reser_l : 16; // [15:0]
        uint32_t __31_16 : 16;     // [31:16]
    } b;
} REG_RF_DIG_RTC_REG_D4_REG_T;

// reg_d8_reg
typedef union {
    uint32_t v;
    struct
    {
        uint32_t rtc_reser_n : 16; // [15:0]
        uint32_t __31_16 : 16;     // [31:16]
    } b;
} REG_RF_DIG_RTC_REG_D8_REG_T;

// reg_dc_reg
typedef union {
    uint32_t v;
    struct
    {
        uint32_t psm_sw_cnt_l : 16; // [15:0]
        uint32_t __31_16 : 16;      // [31:16]
    } b;
} REG_RF_DIG_RTC_REG_DC_REG_T;

// reg_e0_reg
typedef union {
    uint32_t v;
    struct
    {
        uint32_t psm_sw_cnt_h : 16; // [15:0]
        uint32_t __31_16 : 16;      // [31:16]
    } b;
} REG_RF_DIG_RTC_REG_E0_REG_T;

// reg_00_reg
#define RF_DIG_RTC_PU_XTAL_ANA_SEL_SRC (1 << 0)
#define RF_DIG_RTC_PU_XTAL_REG (1 << 1)
#define RF_DIG_RTC_XTAL_SEL_VRTC_VDIG (1 << 2)
#define RF_DIG_RTC_XTAL_SEL_VREF_VDIG (1 << 3)
#define RF_DIG_RTC_STEP_OFFSET_UPDATE (1 << 4)

// reg_18_reg
#define RF_DIG_RTC_STEP_OFFSET_LP(n) (((n)&0xff) << 0)
#define RF_DIG_RTC_STEP_OFFSET_NORMAL(n) (((n)&0xff) << 8)

// reg_1c_reg
#define RF_DIG_RTC_LP_MODE_H_REG (1 << 0)
#define RF_DIG_RTC_LP_MODE_H_DR (1 << 1)
#define RF_DIG_RTC_CHANGE_REG_FLAG_REG (1 << 2)
#define RF_DIG_RTC_CHANGE_REG_FLAG_DR (1 << 3)
#define RF_DIG_RTC_LP_MODE_EN_REG (1 << 4)
#define RF_DIG_RTC_LP_MODE_EN_DR (1 << 5)
#define RF_DIG_RTC_LP_MODE_DELAY(n) (((n)&0x3) << 6)

// reg_c4_reg
#define RF_DIG_RTC_XTAL_OSC_IBIT_N(n) (((n)&0xf) << 0)
#define RF_DIG_RTC_XTAL_OSC_IBIT_L(n) (((n)&0xf) << 4)
#define RF_DIG_RTC_XTAL26M_PWADC_EN (1 << 8)
#define RF_DIG_RTC_XTAL26M_PLLCAL_EN (1 << 9)
#define RF_DIG_RTC_XTAL26M_INTERFACE_EN (1 << 10)
#define RF_DIG_RTC_XTAL26M_PLLS1_EN (1 << 11)
#define RF_DIG_RTC_XTAL26M_PLLS2_EN (1 << 12)
#define RF_DIG_RTC_ENABLE_CLK_26M (1 << 13)
#define RF_DIG_RTC_ENABLE_CLK_26M_LP_UART (1 << 14)

// reg_c8_reg
#define RF_DIG_RTC_XTAL_FIXI_BIT_N(n) (((n)&0x3f) << 0)
#define RF_DIG_RTC_XTAL_FIXI_BIT_L(n) (((n)&0x3f) << 6)
#define RF_DIG_RTC_XTAL_CFIX_BIT_N (1 << 12)
#define RF_DIG_RTC_XTAL_CFIX_BIT_L (1 << 13)

// reg_cc_reg
#define RF_DIG_RTC_XDRV_REG_BIT(n) (((n)&0xf) << 0)
#define RF_DIG_RTC_XTAL_REG_BIT(n) (((n)&0xf) << 4)
#define RF_DIG_RTC_XDRV_AUX1_POWER_BIT(n) (((n)&0x7) << 8)

// reg_d0_reg
#define RF_DIG_RTC_XTAL_CAPBANK_BIT_N(n) (((n)&0xff) << 0)
#define RF_DIG_RTC_XTAL_CAPBANK_BIT_L(n) (((n)&0xff) << 8)

// reg_d4_reg
#define RF_DIG_RTC_RTC_RESER_L(n) (((n)&0xffff) << 0)

// reg_d8_reg
#define RF_DIG_RTC_RTC_RESER_N(n) (((n)&0xffff) << 0)

// reg_dc_reg
#define RF_DIG_RTC_PSM_SW_CNT_L(n) (((n)&0xffff) << 0)

// reg_e0_reg
#define RF_DIG_RTC_PSM_SW_CNT_H(n) (((n)&0xffff) << 0)

#endif // _RF_DIG_RTC_H_
