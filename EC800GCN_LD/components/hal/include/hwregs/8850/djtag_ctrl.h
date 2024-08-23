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

#ifndef _DJTAG_CTRL_H_
#define _DJTAG_CTRL_H_

// Auto generated by dtools(see dtools.txt for its version).
// Don't edit it manually!

#define REG_DJTAG_CTRL_BASE (0x5150e000)

typedef volatile struct
{
    uint32_t djtag_ir_len;                // 0x00000000
    uint32_t ap_pwr_ctrl;                 // 0x00000004
    uint32_t djtag_ir;                    // 0x00000008
    uint32_t djtag_dr;                    // 0x0000000c
    uint32_t dr_pause_recov;              // 0x00000010
    uint32_t djtag_rnd_en;                // 0x00000014
    uint32_t djtag_upd_dr;                // 0x00000018
    uint32_t djtag_dap_mux_ctrl_soft_rst; // 0x0000001c
} HWP_DJTAG_CTRL_T;

#define hwp_djtagCtrl ((HWP_DJTAG_CTRL_T *)REG_ACCESS_ADDRESS(REG_DJTAG_CTRL_BASE))

// djtag_ir_len
typedef union {
    uint32_t v;
    struct
    {
        uint32_t djtag_ir_len : 6; // [5:0]
        uint32_t __31_6 : 26;      // [31:6]
    } b;
} REG_DJTAG_CTRL_DJTAG_IR_LEN_T;

// ap_pwr_ctrl
typedef union {
    uint32_t v;
    struct
    {
        uint32_t djtag_dr_len : 16; // [15:0]
        uint32_t __31_16 : 16;      // [31:16]
    } b;
} REG_DJTAG_CTRL_AP_PWR_CTRL_T;

// dr_pause_recov
typedef union {
    uint32_t v;
    struct
    {
        uint32_t djtag_dr_pause_recov : 1; // [0]
        uint32_t __31_1 : 31;              // [31:1]
    } b;
} REG_DJTAG_CTRL_DR_PAUSE_RECOV_T;

// djtag_rnd_en
typedef union {
    uint32_t v;
    struct
    {
        uint32_t djtag_rnd_en : 1; // [0]
        uint32_t __31_1 : 31;      // [31:1]
    } b;
} REG_DJTAG_CTRL_DJTAG_RND_EN_T;

// djtag_dap_mux_ctrl_soft_rst
typedef union {
    uint32_t v;
    struct
    {
        uint32_t djtag_dap_mux_ctrl_soft_rst : 1; // [0]
        uint32_t __31_1 : 31;                     // [31:1]
    } b;
} REG_DJTAG_CTRL_DJTAG_DAP_MUX_CTRL_SOFT_RST_T;

// djtag_ir_len
#define DJTAG_CTRL_DJTAG_IR_LEN(n) (((n)&0x3f) << 0)

// ap_pwr_ctrl
#define DJTAG_CTRL_DJTAG_DR_LEN(n) (((n)&0xffff) << 0)

// dr_pause_recov
#define DJTAG_CTRL_DJTAG_DR_PAUSE_RECOV (1 << 0)

// djtag_rnd_en
#define DJTAG_CTRL_DJTAG_RND_EN (1 << 0)

// djtag_dap_mux_ctrl_soft_rst
#define DJTAG_CTRL_DJTAG_DAP_MUX_CTRL_SOFT_RST (1 << 0)

#endif // _DJTAG_CTRL_H_
