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

#ifndef _F8_H_
#define _F8_H_

// Auto generated by dtools(see dtools.txt for its version).
// Don't edit it manually!

#define REG_F8_BASE (0x12000000)

typedef volatile struct
{
    uint32_t f8_up_conf;       // 0x00000000
    uint32_t f8_up_group_addr; // 0x00000004
    uint32_t f8_up_group_cnt;  // 0x00000008
    uint32_t f8_up_status;     // 0x0000000c
    uint32_t f8_dp_conf;       // 0x00000010
    uint32_t f8_dp_group_addr; // 0x00000014
    uint32_t f8_dp_group_cnt;  // 0x00000018
    uint32_t f8_dp_status;     // 0x0000001c
    uint32_t f9_conf;          // 0x00000020
    uint32_t f9_group_addr;    // 0x00000024
    uint32_t f9_status;        // 0x00000028
    uint32_t f9_result;        // 0x0000002c
    uint32_t f8_cmd_conf;      // 0x00000030
    uint32_t f8_cmd_addr;      // 0x00000034
    uint32_t f8_cmd_status;    // 0x00000038
    uint32_t status_sr;        // 0x0000003c
} HWP_F8_T;

#define hwp_f8 ((HWP_F8_T *)REG_ACCESS_ADDRESS(REG_F8_BASE))

// f8_up_conf
typedef union {
    uint32_t v;
    struct
    {
        uint32_t f8_start : 1;  // [0]
        uint32_t f8_irq_en : 1; // [1]
        uint32_t f8_ar_sel : 3; // [4:2]
        uint32_t __31_5 : 27;   // [31:5]
    } b;
} REG_F8_F8_UP_CONF_T;

// f8_up_status
typedef union {
    uint32_t v;
    struct
    {
        uint32_t f8_up_stat : 1; // [0], write clear
        uint32_t __31_1 : 31;    // [31:1]
    } b;
} REG_F8_F8_UP_STATUS_T;

// f8_dp_conf
typedef union {
    uint32_t v;
    struct
    {
        uint32_t f8_start : 1;  // [0]
        uint32_t f8_irq_en : 1; // [1]
        uint32_t f8_ar_sel : 3; // [4:2]
        uint32_t __31_5 : 27;   // [31:5]
    } b;
} REG_F8_F8_DP_CONF_T;

// f8_dp_status
typedef union {
    uint32_t v;
    struct
    {
        uint32_t f8_dp_stat : 1; // [0], write clear
        uint32_t __31_1 : 31;    // [31:1]
    } b;
} REG_F8_F8_DP_STATUS_T;

// f9_conf
typedef union {
    uint32_t v;
    struct
    {
        uint32_t f9_start : 1;  // [0]
        uint32_t f9_irq_en : 1; // [1]
        uint32_t f9_ar_sel : 2; // [3:2]
        uint32_t r_cmd_cnt : 2; // [5:4]
        uint32_t w_cmd_cnt : 2; // [7:6]
        uint32_t __31_8 : 24;   // [31:8]
    } b;
} REG_F8_F9_CONF_T;

// f9_status
typedef union {
    uint32_t v;
    struct
    {
        uint32_t f9_stat : 1; // [0], write clear
        uint32_t __31_1 : 31; // [31:1]
    } b;
} REG_F8_F9_STATUS_T;

// f8_cmd_conf
typedef union {
    uint32_t v;
    struct
    {
        uint32_t f8_start : 1;  // [0]
        uint32_t f8_irq_en : 1; // [1]
        uint32_t f8_ar_sel : 3; // [4:2]
        uint32_t __31_5 : 27;   // [31:5]
    } b;
} REG_F8_F8_CMD_CONF_T;

// f8_cmd_status
typedef union {
    uint32_t v;
    struct
    {
        uint32_t f8_cmd_stat : 1; // [0], write clear
        uint32_t __31_1 : 31;     // [31:1]
    } b;
} REG_F8_F8_CMD_STATUS_T;

// status_sr
typedef union {
    uint32_t v;
    struct
    {
        uint32_t f8_dp_status : 1;  // [0], write clear
        uint32_t f8_up_status : 1;  // [1], write clear
        uint32_t f8_cmd_status : 1; // [2], write clear
        uint32_t f9_status : 1;     // [3], write clear
        uint32_t __31_4 : 28;       // [31:4]
    } b;
} REG_F8_STATUS_SR_T;

// f8_up_conf
#define F8_F8_START (1 << 0)
#define F8_F8_IRQ_EN (1 << 1)
#define F8_F8_AR_SEL(n) (((n)&0x7) << 2)

// f8_up_status
#define F8_F8_UP_STAT (1 << 0)

// f8_dp_conf
#define F8_F8_START (1 << 0)
#define F8_F8_IRQ_EN (1 << 1)
#define F8_F8_AR_SEL(n) (((n)&0x7) << 2)

// f8_dp_status
#define F8_F8_DP_STAT (1 << 0)

// f9_conf
#define F8_F9_START (1 << 0)
#define F8_F9_IRQ_EN (1 << 1)
#define F8_F9_AR_SEL(n) (((n)&0x3) << 2)
#define F8_R_CMD_CNT(n) (((n)&0x3) << 4)
#define F8_W_CMD_CNT(n) (((n)&0x3) << 6)

// f9_status
#define F8_F9_STAT (1 << 0)

// f8_cmd_conf
#define F8_F8_START (1 << 0)
#define F8_F8_IRQ_EN (1 << 1)
#define F8_F8_AR_SEL(n) (((n)&0x7) << 2)

// f8_cmd_status
#define F8_F8_CMD_STAT (1 << 0)

// status_sr
#define F8_F8_DP_STATUS (1 << 0)
#define F8_F8_UP_STATUS (1 << 1)
#define F8_F8_CMD_STATUS (1 << 2)
#define F8_F9_STATUS (1 << 3)

#endif // _F8_H_