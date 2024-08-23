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

#ifndef _GPT_LITE_H_
#define _GPT_LITE_H_

// Auto generated by dtools(see dtools.txt for its version).
// Don't edit it manually!

#define REG_AP_GPT_LITE_BASE (0x04809000)
#define REG_LPS_GPT_LITE_BASE (0x5150d000)

typedef volatile struct
{
    uint32_t cr;            // 0x00000000
    uint32_t smcr;          // 0x00000004
    uint32_t egr;           // 0x00000008
    uint32_t ccmr_oc;       // 0x0000000c
    uint32_t ccmr_ic;       // 0x00000010
    uint32_t ccer;          // 0x00000014
    uint32_t cnt;           // 0x00000018
    uint32_t psc;           // 0x0000001c
    uint32_t arr;           // 0x00000020
    uint32_t timer_ccr1_ic; // 0x00000024
    uint32_t timer_ccr2_ic; // 0x00000028
    uint32_t timer_ccr1_oc; // 0x0000002c
    uint32_t timer_ccr2_oc; // 0x00000030
    uint32_t isr;           // 0x00000034
    uint32_t irsr;          // 0x00000038
    uint32_t mask;          // 0x0000003c
    uint32_t clr;           // 0x00000040
} HWP_GPT_LITE_T;

#define hwp_apGptLite ((HWP_GPT_LITE_T *)REG_ACCESS_ADDRESS(REG_AP_GPT_LITE_BASE))
#define hwp_lpsGptLite ((HWP_GPT_LITE_T *)REG_ACCESS_ADDRESS(REG_LPS_GPT_LITE_BASE))

// cr
typedef union {
    uint32_t v;
    struct
    {
        uint32_t cen : 1;        // [0]
        uint32_t ckd : 2;        // [2:1]
        uint32_t udis : 1;       // [3]
        uint32_t opm : 1;        // [4]
        uint32_t dir : 1;        // [5]
        uint32_t cms : 2;        // [7:6]
        uint32_t arpe : 1;       // [8]
        uint32_t tri : 3;        // [11:9]
        uint32_t tri_cnt_en : 1; // [12]
        uint32_t __30_13 : 18;   // [30:13]
        uint32_t refclk_sel : 1; // [31]
    } b;
} REG_GPT_LITE_CR_T;

// smcr
typedef union {
    uint32_t v;
    struct
    {
        uint32_t sms : 3;     // [2:0]
        uint32_t __31_3 : 29; // [31:3]
    } b;
} REG_GPT_LITE_SMCR_T;

// egr
typedef union {
    uint32_t v;
    struct
    {
        uint32_t ug : 1;      // [0], read only
        uint32_t __31_1 : 31; // [31:1]
    } b;
} REG_GPT_LITE_EGR_T;

// ccmr_oc
typedef union {
    uint32_t v;
    struct
    {
        uint32_t cc1s : 2;     // [1:0]
        uint32_t oc1fe : 1;    // [2]
        uint32_t oc1pe : 1;    // [3]
        uint32_t oc1m : 3;     // [6:4]
        uint32_t oc1ce : 1;    // [7]
        uint32_t cc2s : 2;     // [9:8]
        uint32_t oc2fe : 1;    // [10]
        uint32_t oc2pe : 1;    // [11]
        uint32_t oc2m : 3;     // [14:12]
        uint32_t oc2ce : 1;    // [15]
        uint32_t __31_16 : 16; // [31:16]
    } b;
} REG_GPT_LITE_CCMR_OC_T;

// ccmr_ic
typedef union {
    uint32_t v;
    struct
    {
        uint32_t ic1psc : 2;   // [1:0]
        uint32_t ic1f : 4;     // [5:2]
        uint32_t __7_6 : 2;    // [7:6]
        uint32_t ic2psc : 2;   // [9:8]
        uint32_t ic2f : 4;     // [13:10]
        uint32_t __31_14 : 18; // [31:14]
    } b;
} REG_GPT_LITE_CCMR_IC_T;

// ccer
typedef union {
    uint32_t v;
    struct
    {
        uint32_t cc1e : 1;    // [0]
        uint32_t cc1p : 1;    // [1]
        uint32_t cc2e : 1;    // [2]
        uint32_t cc2p : 1;    // [3]
        uint32_t __31_4 : 28; // [31:4]
    } b;
} REG_GPT_LITE_CCER_T;

// cnt
typedef union {
    uint32_t v;
    struct
    {
        uint32_t cnt_value : 16; // [15:0], read only
        uint32_t __31_16 : 16;   // [31:16]
    } b;
} REG_GPT_LITE_CNT_T;

// psc
typedef union {
    uint32_t v;
    struct
    {
        uint32_t psc_value : 16; // [15:0]
        uint32_t __31_16 : 16;   // [31:16]
    } b;
} REG_GPT_LITE_PSC_T;

// arr
typedef union {
    uint32_t v;
    struct
    {
        uint32_t arr_value : 16; // [15:0]
        uint32_t __31_16 : 16;   // [31:16]
    } b;
} REG_GPT_LITE_ARR_T;

// timer_ccr1_ic
typedef union {
    uint32_t v;
    struct
    {
        uint32_t timer_ccr1_capture : 16; // [15:0], read only
        uint32_t __31_16 : 16;            // [31:16]
    } b;
} REG_GPT_LITE_TIMER_CCR1_IC_T;

// timer_ccr2_ic
typedef union {
    uint32_t v;
    struct
    {
        uint32_t timer_ccr2_capture : 16; // [15:0], read only
        uint32_t __31_16 : 16;            // [31:16]
    } b;
} REG_GPT_LITE_TIMER_CCR2_IC_T;

// timer_ccr1_oc
typedef union {
    uint32_t v;
    struct
    {
        uint32_t timer_ccr1_compare : 16; // [15:0]
        uint32_t __31_16 : 16;            // [31:16]
    } b;
} REG_GPT_LITE_TIMER_CCR1_OC_T;

// timer_ccr2_oc
typedef union {
    uint32_t v;
    struct
    {
        uint32_t timer_ccr2_compare : 16; // [15:0]
        uint32_t __31_16 : 16;            // [31:16]
    } b;
} REG_GPT_LITE_TIMER_CCR2_OC_T;

// isr
typedef union {
    uint32_t v;
    struct
    {
        uint32_t compare_int : 4;  // [3:0], read only
        uint32_t __11_4 : 8;       // [11:4]
        uint32_t capture_int : 4;  // [15:12], read only
        uint32_t __29_16 : 14;     // [29:16]
        uint32_t slave_trig : 1;   // [30], read only
        uint32_t event_update : 1; // [31], read only
    } b;
} REG_GPT_LITE_ISR_T;

// irsr
typedef union {
    uint32_t v;
    struct
    {
        uint32_t compare_int : 4;  // [3:0], read only
        uint32_t __11_4 : 8;       // [11:4]
        uint32_t capture_int : 4;  // [15:12], read only
        uint32_t __29_16 : 14;     // [29:16]
        uint32_t slave_trig : 1;   // [30], read only
        uint32_t event_update : 1; // [31], read only
    } b;
} REG_GPT_LITE_IRSR_T;

// mask
typedef union {
    uint32_t v;
    struct
    {
        uint32_t compare_int : 4;  // [3:0]
        uint32_t __11_4 : 8;       // [11:4]
        uint32_t capture_int : 4;  // [15:12]
        uint32_t __29_16 : 14;     // [29:16]
        uint32_t slave_trig : 1;   // [30]
        uint32_t event_update : 1; // [31]
    } b;
} REG_GPT_LITE_MASK_T;

// clr
typedef union {
    uint32_t v;
    struct
    {
        uint32_t compare_int : 4;  // [3:0], read only
        uint32_t __11_4 : 8;       // [11:4]
        uint32_t capture_int : 4;  // [15:12], read only
        uint32_t __29_16 : 14;     // [29:16]
        uint32_t slave_trig : 1;   // [30], read only
        uint32_t event_update : 1; // [31], read only
    } b;
} REG_GPT_LITE_CLR_T;

// cr
#define GPT_LITE_CEN (1 << 0)
#define GPT_LITE_CKD(n) (((n)&0x3) << 1)
#define GPT_LITE_UDIS (1 << 3)
#define GPT_LITE_OPM (1 << 4)
#define GPT_LITE_DIR (1 << 5)
#define GPT_LITE_CMS(n) (((n)&0x3) << 6)
#define GPT_LITE_ARPE (1 << 8)
#define GPT_LITE_TRI(n) (((n)&0x7) << 9)
#define GPT_LITE_TRI_CNT_EN (1 << 12)
#define GPT_LITE_REFCLK_SEL (1 << 31)

// smcr
#define GPT_LITE_SMS(n) (((n)&0x7) << 0)

// egr
#define GPT_LITE_UG (1 << 0)

// ccmr_oc
#define GPT_LITE_CC1S(n) (((n)&0x3) << 0)
#define GPT_LITE_OC1FE (1 << 2)
#define GPT_LITE_OC1PE (1 << 3)
#define GPT_LITE_OC1M(n) (((n)&0x7) << 4)
#define GPT_LITE_OC1CE (1 << 7)
#define GPT_LITE_CC2S(n) (((n)&0x3) << 8)
#define GPT_LITE_OC2FE (1 << 10)
#define GPT_LITE_OC2PE (1 << 11)
#define GPT_LITE_OC2M(n) (((n)&0x7) << 12)
#define GPT_LITE_OC2CE (1 << 15)

// ccmr_ic
#define GPT_LITE_IC1PSC(n) (((n)&0x3) << 0)
#define GPT_LITE_IC1F(n) (((n)&0xf) << 2)
#define GPT_LITE_IC2PSC(n) (((n)&0x3) << 8)
#define GPT_LITE_IC2F(n) (((n)&0xf) << 10)

// ccer
#define GPT_LITE_CC1E (1 << 0)
#define GPT_LITE_CC1P (1 << 1)
#define GPT_LITE_CC2E (1 << 2)
#define GPT_LITE_CC2P (1 << 3)

// cnt
#define GPT_LITE_CNT_VALUE(n) (((n)&0xffff) << 0)

// psc
#define GPT_LITE_PSC_VALUE(n) (((n)&0xffff) << 0)

// arr
#define GPT_LITE_ARR_VALUE(n) (((n)&0xffff) << 0)

// timer_ccr1_ic
#define GPT_LITE_TIMER_CCR1_CAPTURE(n) (((n)&0xffff) << 0)

// timer_ccr2_ic
#define GPT_LITE_TIMER_CCR2_CAPTURE(n) (((n)&0xffff) << 0)

// timer_ccr1_oc
#define GPT_LITE_TIMER_CCR1_COMPARE(n) (((n)&0xffff) << 0)

// timer_ccr2_oc
#define GPT_LITE_TIMER_CCR2_COMPARE(n) (((n)&0xffff) << 0)

// isr
#define GPT_LITE_COMPARE_INT(n) (((n)&0xf) << 0)
#define GPT_LITE_CAPTURE_INT(n) (((n)&0xf) << 12)
#define GPT_LITE_SLAVE_TRIG (1 << 30)
#define GPT_LITE_EVENT_UPDATE (1 << 31)

// irsr
#define GPT_LITE_COMPARE_INT(n) (((n)&0xf) << 0)
#define GPT_LITE_CAPTURE_INT(n) (((n)&0xf) << 12)
#define GPT_LITE_SLAVE_TRIG (1 << 30)
#define GPT_LITE_EVENT_UPDATE (1 << 31)

// mask
#define GPT_LITE_COMPARE_INT(n) (((n)&0xf) << 0)
#define GPT_LITE_CAPTURE_INT(n) (((n)&0xf) << 12)
#define GPT_LITE_SLAVE_TRIG (1 << 30)
#define GPT_LITE_EVENT_UPDATE (1 << 31)

// clr
#define GPT_LITE_COMPARE_INT(n) (((n)&0xf) << 0)
#define GPT_LITE_CAPTURE_INT(n) (((n)&0xf) << 12)
#define GPT_LITE_SLAVE_TRIG (1 << 30)
#define GPT_LITE_EVENT_UPDATE (1 << 31)

#endif // _GPT_LITE_H_