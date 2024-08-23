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

#ifndef _PMIC_WDT_H_
#define _PMIC_WDT_H_

// Auto generated by dtools(see dtools.txt for its version).
// Don't edit it manually!

#define REG_PMIC_WDT_BASE (0x51108480)

typedef volatile struct
{
    uint32_t wdg_load_low;         // 0x00000000
    uint32_t wdg_load_high;        // 0x00000004
    uint32_t wdg_ctrl;             // 0x00000008
    uint32_t wdg_int_clr;          // 0x0000000c
    uint32_t wdg_int_raw;          // 0x00000010
    uint32_t wdg_irq_mask;         // 0x00000014
    uint32_t wdg_cnt_low;          // 0x00000018
    uint32_t wdg_cnt_high;         // 0x0000001c
    uint32_t wdg_lock;             // 0x00000020
    uint32_t wdg_cnt_read_low;     // 0x00000024
    uint32_t wdg_cnt_read_high;    // 0x00000028
    uint32_t wdg_irq_value_low;    // 0x0000002c
    uint32_t wdg_irq_value_high;   // 0x00000030
    uint32_t wdg_load_higher;      // 0x00000034
    uint32_t wdg_cnt_higher;       // 0x00000038
    uint32_t wdg_cnt_read_higher;  // 0x0000003c
    uint32_t wdg_irq_value_higher; // 0x00000040
} HWP_PMIC_WDT_T;

#define hwp_pmicWdt ((HWP_PMIC_WDT_T *)REG_ACCESS_ADDRESS(REG_PMIC_WDT_BASE))

// wdg_load_low
typedef union {
    uint32_t v;
    struct
    {
        uint32_t wdg_ld_value_low : 16; // [15:0]
        uint32_t __31_16 : 16;          // [31:16]
    } b;
} REG_PMIC_WDT_WDG_LOAD_LOW_T;

// wdg_load_high
typedef union {
    uint32_t v;
    struct
    {
        uint32_t wdg_ld_value_high : 16; // [15:0]
        uint32_t __31_16 : 16;           // [31:16]
    } b;
} REG_PMIC_WDT_WDG_LOAD_HIGH_T;

// wdg_ctrl
typedef union {
    uint32_t v;
    struct
    {
        uint32_t wdg_irq_en : 1; // [0]
        uint32_t wdg_open : 1;   // [1]
        uint32_t wdg_new : 1;    // [2]
        uint32_t wdg_rst_en : 1; // [3]
        uint32_t __31_4 : 28;    // [31:4]
    } b;
} REG_PMIC_WDT_WDG_CTRL_T;

// wdg_int_clr
typedef union {
    uint32_t v;
    struct
    {
        uint32_t wdg_irq_clr : 1; // [0]
        uint32_t __2_1 : 2;       // [2:1]
        uint32_t wdg_rst_clr : 1; // [3]
        uint32_t __31_4 : 28;     // [31:4]
    } b;
} REG_PMIC_WDT_WDG_INT_CLR_T;

// wdg_int_raw
typedef union {
    uint32_t v;
    struct
    {
        uint32_t wdg_irq_raw : 1;    // [0], read only
        uint32_t __2_1 : 2;          // [2:1]
        uint32_t wdg_rst_raw : 1;    // [3], read only
        uint32_t apb_wr_ld_busy : 1; // [4], read only
        uint32_t __31_5 : 27;        // [31:5]
    } b;
} REG_PMIC_WDT_WDG_INT_RAW_T;

// wdg_irq_mask
typedef union {
    uint32_t v;
    struct
    {
        uint32_t wdg_irq_mask : 1; // [0], read only
        uint32_t __31_1 : 31;      // [31:1]
    } b;
} REG_PMIC_WDT_WDG_IRQ_MASK_T;

// wdg_cnt_low
typedef union {
    uint32_t v;
    struct
    {
        uint32_t wdg_cnt_low : 16; // [15:0], read only
        uint32_t __31_16 : 16;     // [31:16]
    } b;
} REG_PMIC_WDT_WDG_CNT_LOW_T;

// wdg_cnt_high
typedef union {
    uint32_t v;
    struct
    {
        uint32_t wdg_cnt_high : 16; // [15:0], read only
        uint32_t __31_16 : 16;      // [31:16]
    } b;
} REG_PMIC_WDT_WDG_CNT_HIGH_T;

// wdg_lock
typedef union {
    uint32_t v;
    struct
    {
        uint32_t wdg_lock : 16; // [15:0]
        uint32_t __31_16 : 16;  // [31:16]
    } b;
} REG_PMIC_WDT_WDG_LOCK_T;

// wdg_cnt_read_low
typedef union {
    uint32_t v;
    struct
    {
        uint32_t wdg_cnt_read_low : 16; // [15:0], read only
        uint32_t __31_16 : 16;          // [31:16]
    } b;
} REG_PMIC_WDT_WDG_CNT_READ_LOW_T;

// wdg_cnt_read_high
typedef union {
    uint32_t v;
    struct
    {
        uint32_t wdg_cnt_read_high : 16; // [15:0], read only
        uint32_t __31_16 : 16;           // [31:16]
    } b;
} REG_PMIC_WDT_WDG_CNT_READ_HIGH_T;

// wdg_irq_value_low
typedef union {
    uint32_t v;
    struct
    {
        uint32_t wdg_irq_value_low : 16; // [15:0]
        uint32_t __31_16 : 16;           // [31:16]
    } b;
} REG_PMIC_WDT_WDG_IRQ_VALUE_LOW_T;

// wdg_irq_value_high
typedef union {
    uint32_t v;
    struct
    {
        uint32_t wdg_irq_value_high : 16; // [15:0]
        uint32_t __31_16 : 16;            // [31:16]
    } b;
} REG_PMIC_WDT_WDG_IRQ_VALUE_HIGH_T;

// wdg_load_higher
typedef union {
    uint32_t v;
    struct
    {
        uint32_t wdg_ld_value_higher : 16; // [15:0]
        uint32_t __31_16 : 16;             // [31:16]
    } b;
} REG_PMIC_WDT_WDG_LOAD_HIGHER_T;

// wdg_cnt_higher
typedef union {
    uint32_t v;
    struct
    {
        uint32_t wdg_cnt_higher : 16; // [15:0], read only
        uint32_t __31_16 : 16;        // [31:16]
    } b;
} REG_PMIC_WDT_WDG_CNT_HIGHER_T;

// wdg_cnt_read_higher
typedef union {
    uint32_t v;
    struct
    {
        uint32_t wdg_cnt_read_higher : 16; // [15:0], read only
        uint32_t __31_16 : 16;             // [31:16]
    } b;
} REG_PMIC_WDT_WDG_CNT_READ_HIGHER_T;

// wdg_irq_value_higher
typedef union {
    uint32_t v;
    struct
    {
        uint32_t wdg_irq_value_higher : 16; // [15:0]
        uint32_t __31_16 : 16;              // [31:16]
    } b;
} REG_PMIC_WDT_WDG_IRQ_VALUE_HIGHER_T;

// wdg_load_low
#define PMIC_WDT_WDG_LD_VALUE_LOW(n) (((n)&0xffff) << 0)

// wdg_load_high
#define PMIC_WDT_WDG_LD_VALUE_HIGH(n) (((n)&0xffff) << 0)

// wdg_ctrl
#define PMIC_WDT_WDG_IRQ_EN (1 << 0)
#define PMIC_WDT_WDG_OPEN (1 << 1)
#define PMIC_WDT_WDG_NEW (1 << 2)
#define PMIC_WDT_WDG_RST_EN (1 << 3)

// wdg_int_clr
#define PMIC_WDT_WDG_IRQ_CLR (1 << 0)
#define PMIC_WDT_WDG_RST_CLR (1 << 3)

// wdg_int_raw
#define PMIC_WDT_WDG_IRQ_RAW (1 << 0)
#define PMIC_WDT_WDG_RST_RAW (1 << 3)
#define PMIC_WDT_APB_WR_LD_BUSY (1 << 4)

// wdg_irq_mask
#define PMIC_WDT_WDG_IRQ_MASK (1 << 0)

// wdg_cnt_low
#define PMIC_WDT_WDG_CNT_LOW(n) (((n)&0xffff) << 0)

// wdg_cnt_high
#define PMIC_WDT_WDG_CNT_HIGH(n) (((n)&0xffff) << 0)

// wdg_lock
#define PMIC_WDT_WDG_LOCK(n) (((n)&0xffff) << 0)

// wdg_cnt_read_low
#define PMIC_WDT_WDG_CNT_READ_LOW(n) (((n)&0xffff) << 0)

// wdg_cnt_read_high
#define PMIC_WDT_WDG_CNT_READ_HIGH(n) (((n)&0xffff) << 0)

// wdg_irq_value_low
#define PMIC_WDT_WDG_IRQ_VALUE_LOW(n) (((n)&0xffff) << 0)

// wdg_irq_value_high
#define PMIC_WDT_WDG_IRQ_VALUE_HIGH(n) (((n)&0xffff) << 0)

// wdg_load_higher
#define PMIC_WDT_WDG_LD_VALUE_HIGHER(n) (((n)&0xffff) << 0)

// wdg_cnt_higher
#define PMIC_WDT_WDG_CNT_HIGHER(n) (((n)&0xffff) << 0)

// wdg_cnt_read_higher
#define PMIC_WDT_WDG_CNT_READ_HIGHER(n) (((n)&0xffff) << 0)

// wdg_irq_value_higher
#define PMIC_WDT_WDG_IRQ_VALUE_HIGHER(n) (((n)&0xffff) << 0)

#endif // _PMIC_WDT_H_