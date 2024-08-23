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

#ifndef _CORR_H_
#define _CORR_H_

// Auto generated by dtools(see dtools.txt for its version).
// Don't edit it manually!

#define REG_CORR_BASE (0x19000800)

typedef volatile struct
{
    uint32_t corr_para;   // 0x00000000
    uint32_t corr_start;  // 0x00000004
    uint32_t corr_out;    // 0x00000008
    uint32_t corr_max;    // 0x0000000c
    uint32_t corr_sum;    // 0x00000010
    uint32_t corr_int_en; // 0x00000014
    uint32_t int_flag;    // 0x00000018
} HWP_CORR_T;

#define hwp_corr ((HWP_CORR_T *)REG_ACCESS_ADDRESS(REG_CORR_BASE))

// corr_para
typedef union {
    uint32_t v;
    struct
    {
        uint32_t corr_idnum : 4;   // [3:0]
        uint32_t corr_reclen : 12; // [15:4]
        uint32_t corr_loclen : 9;  // [24:16]
        uint32_t __31_25 : 7;      // [31:25]
    } b;
} REG_CORR_CORR_PARA_T;

// corr_start
typedef union {
    uint32_t v;
    struct
    {
        uint32_t corr_start : 1; // [0]
        uint32_t __31_1 : 31;    // [31:1]
    } b;
} REG_CORR_CORR_START_T;

// corr_out
typedef union {
    uint32_t v;
    struct
    {
        uint32_t corr_id : 4;   // [3:0], read only
        uint32_t corr_pos : 12; // [15:4], read only
        uint32_t corr_pp : 1;   // [16], read only
        uint32_t __31_17 : 15;  // [31:17]
    } b;
} REG_CORR_CORR_OUT_T;

// corr_max
typedef union {
    uint32_t v;
    struct
    {
        uint32_t corr_max : 24; // [23:0], read only
        uint32_t __31_24 : 8;   // [31:24]
    } b;
} REG_CORR_CORR_MAX_T;

// corr_int_en
typedef union {
    uint32_t v;
    struct
    {
        uint32_t corr_int_en : 1; // [0]
        uint32_t __31_1 : 31;     // [31:1]
    } b;
} REG_CORR_CORR_INT_EN_T;

// int_flag
typedef union {
    uint32_t v;
    struct
    {
        uint32_t int_flag : 1; // [0], write clear
        uint32_t __31_1 : 31;  // [31:1]
    } b;
} REG_CORR_INT_FLAG_T;

// corr_para
#define CORR_CORR_IDNUM(n) (((n)&0xf) << 0)
#define CORR_CORR_RECLEN(n) (((n)&0xfff) << 4)
#define CORR_CORR_LOCLEN(n) (((n)&0x1ff) << 16)

// corr_start
#define CORR_CORR_START (1 << 0)

// corr_out
#define CORR_CORR_ID(n) (((n)&0xf) << 0)
#define CORR_CORR_POS(n) (((n)&0xfff) << 4)
#define CORR_CORR_PP (1 << 16)

// corr_max
#define CORR_CORR_MAX(n) (((n)&0xffffff) << 0)

// corr_int_en
#define CORR_CORR_INT_EN (1 << 0)

// int_flag
#define CORR_INT_FLAG (1 << 0)

#endif // _CORR_H_
