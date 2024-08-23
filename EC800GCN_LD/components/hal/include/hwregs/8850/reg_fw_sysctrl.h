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

#ifndef _REG_FW_SYSCTRL_H_
#define _REG_FW_SYSCTRL_H_

// Auto generated by dtools(see dtools.txt for its version).
// Don't edit it manually!

#define REG_REG_FW_SYSCTRL_BASE (0x51305000)

typedef volatile struct
{
    uint32_t reg_rd_ctrl_0;         // 0x00000000
    uint32_t reg_rd_ctrl_1;         // 0x00000004
    uint32_t reg_wr_ctrl_0;         // 0x00000008
    uint32_t reg_wr_ctrl_1;         // 0x0000000c
    uint32_t bit_ctrl_addr_array0;  // 0x00000010
    uint32_t bit_ctrl_addr_array1;  // 0x00000014
    uint32_t bit_ctrl_addr_array2;  // 0x00000018
    uint32_t bit_ctrl_addr_array3;  // 0x0000001c
    uint32_t bit_ctrl_addr_array4;  // 0x00000020
    uint32_t bit_ctrl_addr_array5;  // 0x00000024
    uint32_t bit_ctrl_addr_array6;  // 0x00000028
    uint32_t bit_ctrl_addr_array7;  // 0x0000002c
    uint32_t bit_ctrl_addr_array8;  // 0x00000030
    uint32_t bit_ctrl_addr_array9;  // 0x00000034
    uint32_t bit_ctrl_addr_array10; // 0x00000038
    uint32_t bit_ctrl_addr_array11; // 0x0000003c
    uint32_t bit_ctrl_addr_array12; // 0x00000040
    uint32_t bit_ctrl_addr_array13; // 0x00000044
    uint32_t bit_ctrl_addr_array14; // 0x00000048
    uint32_t bit_ctrl_addr_array15; // 0x0000004c
    uint32_t bit_ctrl_array0;       // 0x00000050
    uint32_t bit_ctrl_array1;       // 0x00000054
    uint32_t bit_ctrl_array2;       // 0x00000058
    uint32_t bit_ctrl_array3;       // 0x0000005c
    uint32_t bit_ctrl_array4;       // 0x00000060
    uint32_t bit_ctrl_array5;       // 0x00000064
    uint32_t bit_ctrl_array6;       // 0x00000068
    uint32_t bit_ctrl_array7;       // 0x0000006c
    uint32_t bit_ctrl_array8;       // 0x00000070
    uint32_t bit_ctrl_array9;       // 0x00000074
    uint32_t bit_ctrl_array10;      // 0x00000078
    uint32_t bit_ctrl_array11;      // 0x0000007c
    uint32_t bit_ctrl_array12;      // 0x00000080
    uint32_t bit_ctrl_array13;      // 0x00000084
    uint32_t bit_ctrl_array14;      // 0x00000088
    uint32_t bit_ctrl_array15;      // 0x0000008c
} HWP_REG_FW_SYSCTRL_T;

#define hwp_regFwSysctrl ((HWP_REG_FW_SYSCTRL_T *)REG_ACCESS_ADDRESS(REG_REG_FW_SYSCTRL_BASE))

// reg_rd_ctrl_0
typedef union {
    uint32_t v;
    struct
    {
        uint32_t aon_soft_rst_ctrl0_rd_sec : 1;      // [0]
        uint32_t clken_lte_rd_sec : 1;               // [1]
        uint32_t clken_lte_intf_rd_sec : 1;          // [2]
        uint32_t rstctrl_lte_rd_sec : 1;             // [3]
        uint32_t lte_autogate_mode_rd_sec : 1;       // [4]
        uint32_t lte_autogate_en_rd_sec : 1;         // [5]
        uint32_t lte_autogate_delay_num_rd_sec : 1;  // [6]
        uint32_t aon_lpc_ctrl_rd_sec : 1;            // [7]
        uint32_t aon_clock_en0_rd_sec : 1;           // [8]
        uint32_t aon_clock_en1_rd_sec : 1;           // [9]
        uint32_t aon_clock_auto_sel0_rd_sec : 1;     // [10]
        uint32_t aon_clock_auto_sel1_rd_sec : 1;     // [11]
        uint32_t aon_clock_auto_sel2_rd_sec : 1;     // [12]
        uint32_t aon_clock_auto_sel3_rd_sec : 1;     // [13]
        uint32_t aon_clock_force_en0_rd_sec : 1;     // [14]
        uint32_t aon_clock_force_en1_rd_sec : 1;     // [15]
        uint32_t aon_clock_force_en2_rd_sec : 1;     // [16]
        uint32_t aon_clock_force_en3_rd_sec : 1;     // [17]
        uint32_t aon_soft_rst_ctrl1_rd_sec : 1;      // [18]
        uint32_t mipi_csi_cfg_reg_rd_sec : 1;        // [19]
        uint32_t cfg_clk_uart2_rd_sec : 1;           // [20]
        uint32_t cfg_clk_uart3_rd_sec : 1;           // [21]
        uint32_t cfg_clk_debug_host_rd_sec : 1;      // [22]
        uint32_t rc_calib_ctrl_rd_sec : 1;           // [23]
        uint32_t rc_calib_th_val_rd_sec : 1;         // [24]
        uint32_t rc_calib_out_val_rd_sec : 1;        // [25]
        uint32_t emmc_slice_phy_ctrl_rd_sec : 1;     // [26]
        uint32_t dma_req_ctrl_rd_sec : 1;            // [27]
        uint32_t apt_trigger_sel_rd_sec : 1;         // [28]
        uint32_t ahb2ahb_ab_funcdma_ctrl_rd_sec : 1; // [29]
        uint32_t ahb2ahb_ab_funcdma_sts_rd_sec : 1;  // [30]
        uint32_t ahb2ahb_ab_dap_ctrl_rd_sec : 1;     // [31]
    } b;
} REG_REG_FW_SYSCTRL_REG_RD_CTRL_0_T;

// reg_rd_ctrl_1
typedef union {
    uint32_t v;
    struct
    {
        uint32_t ahb2ahb_ab_dap_sts_rd_sec : 1;            // [0]
        uint32_t ahb2axi_pub_ctrl_rd_sec : 1;              // [1]
        uint32_t ahb2axi_pub_sts_rd_sec : 1;               // [2]
        uint32_t axi2axi_pub_sts_0_rd_sec : 1;             // [3]
        uint32_t axi2axi_pub_sts_1_rd_sec : 1;             // [4]
        uint32_t ahb2ahb_ab_aon2lps_ctrl_rd_sec : 1;       // [5]
        uint32_t ahb2ahb_ab_aon2lps_sts_rd_sec : 1;        // [6]
        uint32_t ahb2ahb_ab_lps2aon_ctrl_rd_sec : 1;       // [7]
        uint32_t ahb2ahb_ab_lps2aon_sts_rd_sec : 1;        // [8]
        uint32_t sysctrl_reg0_rd_sec : 1;                  // [9]
        uint32_t plls_sts_rd_sec : 1;                      // [10]
        uint32_t cfg_aon_anti_hang_rd_sec : 1;             // [11]
        uint32_t cfg_aon_qos_rd_sec : 1;                   // [12]
        uint32_t aon_ahb_mtx_slice_autogate_en_rd_sec : 1; // [13]
        uint32_t dap_djtag_en_cfg_rd_sec : 1;              // [14]
        uint32_t lte_ahb2ahb_sync_cfg_rd_sec : 1;          // [15]
        uint32_t cfg_aon_io_core_ie_0_rd_sec : 1;          // [16]
        uint32_t cfg_aon_io_core_ie_1_rd_sec : 1;          // [17]
        uint32_t cfg_aon_io_core_ie_2_rd_sec : 1;          // [18]
        uint32_t cfg_aon_io_core_ie_3_rd_sec : 1;          // [19]
        uint32_t __31_20 : 12;                             // [31:20]
    } b;
} REG_REG_FW_SYSCTRL_REG_RD_CTRL_1_T;

// reg_wr_ctrl_0
typedef union {
    uint32_t v;
    struct
    {
        uint32_t aon_soft_rst_ctrl0_wr_sec : 1;      // [0]
        uint32_t clken_lte_wr_sec : 1;               // [1]
        uint32_t clken_lte_intf_wr_sec : 1;          // [2]
        uint32_t rstctrl_lte_wr_sec : 1;             // [3]
        uint32_t lte_autogate_mode_wr_sec : 1;       // [4]
        uint32_t lte_autogate_en_wr_sec : 1;         // [5]
        uint32_t lte_autogate_delay_num_wr_sec : 1;  // [6]
        uint32_t aon_lpc_ctrl_wr_sec : 1;            // [7]
        uint32_t aon_clock_en0_wr_sec : 1;           // [8]
        uint32_t aon_clock_en1_wr_sec : 1;           // [9]
        uint32_t aon_clock_auto_sel0_wr_sec : 1;     // [10]
        uint32_t aon_clock_auto_sel1_wr_sec : 1;     // [11]
        uint32_t aon_clock_auto_sel2_wr_sec : 1;     // [12]
        uint32_t aon_clock_auto_sel3_wr_sec : 1;     // [13]
        uint32_t aon_clock_force_en0_wr_sec : 1;     // [14]
        uint32_t aon_clock_force_en1_wr_sec : 1;     // [15]
        uint32_t aon_clock_force_en2_wr_sec : 1;     // [16]
        uint32_t aon_clock_force_en3_wr_sec : 1;     // [17]
        uint32_t aon_soft_rst_ctrl1_wr_sec : 1;      // [18]
        uint32_t mipi_csi_cfg_reg_wr_sec : 1;        // [19]
        uint32_t cfg_clk_uart2_wr_sec : 1;           // [20]
        uint32_t cfg_clk_uart3_wr_sec : 1;           // [21]
        uint32_t cfg_clk_debug_host_wr_sec : 1;      // [22]
        uint32_t rc_calib_ctrl_wr_sec : 1;           // [23]
        uint32_t rc_calib_th_val_wr_sec : 1;         // [24]
        uint32_t rc_calib_out_val_wr_sec : 1;        // [25]
        uint32_t emmc_slice_phy_ctrl_wr_sec : 1;     // [26]
        uint32_t dma_req_ctrl_wr_sec : 1;            // [27]
        uint32_t apt_trigger_sel_wr_sec : 1;         // [28]
        uint32_t ahb2ahb_ab_funcdma_ctrl_wr_sec : 1; // [29]
        uint32_t ahb2ahb_ab_funcdma_sts_wr_sec : 1;  // [30]
        uint32_t ahb2ahb_ab_dap_ctrl_wr_sec : 1;     // [31]
    } b;
} REG_REG_FW_SYSCTRL_REG_WR_CTRL_0_T;

// reg_wr_ctrl_1
typedef union {
    uint32_t v;
    struct
    {
        uint32_t ahb2ahb_ab_dap_sts_wr_sec : 1;            // [0]
        uint32_t ahb2axi_pub_ctrl_wr_sec : 1;              // [1]
        uint32_t ahb2axi_pub_sts_wr_sec : 1;               // [2]
        uint32_t axi2axi_pub_sts_0_wr_sec : 1;             // [3]
        uint32_t axi2axi_pub_sts_1_wr_sec : 1;             // [4]
        uint32_t ahb2ahb_ab_aon2lps_ctrl_wr_sec : 1;       // [5]
        uint32_t ahb2ahb_ab_aon2lps_sts_wr_sec : 1;        // [6]
        uint32_t ahb2ahb_ab_lps2aon_ctrl_wr_sec : 1;       // [7]
        uint32_t ahb2ahb_ab_lps2aon_sts_wr_sec : 1;        // [8]
        uint32_t sysctrl_reg0_wr_sec : 1;                  // [9]
        uint32_t plls_sts_wr_sec : 1;                      // [10]
        uint32_t cfg_aon_anti_hang_wr_sec : 1;             // [11]
        uint32_t cfg_aon_qos_wr_sec : 1;                   // [12]
        uint32_t aon_ahb_mtx_slice_autogate_en_wr_sec : 1; // [13]
        uint32_t dap_djtag_en_cfg_wr_sec : 1;              // [14]
        uint32_t lte_ahb2ahb_sync_cfg_wr_sec : 1;          // [15]
        uint32_t cfg_aon_io_core_ie_0_wr_sec : 1;          // [16]
        uint32_t cfg_aon_io_core_ie_1_wr_sec : 1;          // [17]
        uint32_t cfg_aon_io_core_ie_2_wr_sec : 1;          // [18]
        uint32_t cfg_aon_io_core_ie_3_wr_sec : 1;          // [19]
        uint32_t __31_20 : 12;                             // [31:20]
    } b;
} REG_REG_FW_SYSCTRL_REG_WR_CTRL_1_T;

// bit_ctrl_addr_array0
typedef union {
    uint32_t v;
    struct
    {
        uint32_t bit_ctrl_addr_array0 : 12; // [11:0]
        uint32_t __31_12 : 20;              // [31:12]
    } b;
} REG_REG_FW_SYSCTRL_BIT_CTRL_ADDR_ARRAY0_T;

// bit_ctrl_addr_array1
typedef union {
    uint32_t v;
    struct
    {
        uint32_t bit_ctrl_addr_array1 : 12; // [11:0]
        uint32_t __31_12 : 20;              // [31:12]
    } b;
} REG_REG_FW_SYSCTRL_BIT_CTRL_ADDR_ARRAY1_T;

// bit_ctrl_addr_array2
typedef union {
    uint32_t v;
    struct
    {
        uint32_t bit_ctrl_addr_array2 : 12; // [11:0]
        uint32_t __31_12 : 20;              // [31:12]
    } b;
} REG_REG_FW_SYSCTRL_BIT_CTRL_ADDR_ARRAY2_T;

// bit_ctrl_addr_array3
typedef union {
    uint32_t v;
    struct
    {
        uint32_t bit_ctrl_addr_array3 : 12; // [11:0]
        uint32_t __31_12 : 20;              // [31:12]
    } b;
} REG_REG_FW_SYSCTRL_BIT_CTRL_ADDR_ARRAY3_T;

// bit_ctrl_addr_array4
typedef union {
    uint32_t v;
    struct
    {
        uint32_t bit_ctrl_addr_array4 : 12; // [11:0]
        uint32_t __31_12 : 20;              // [31:12]
    } b;
} REG_REG_FW_SYSCTRL_BIT_CTRL_ADDR_ARRAY4_T;

// bit_ctrl_addr_array5
typedef union {
    uint32_t v;
    struct
    {
        uint32_t bit_ctrl_addr_array5 : 12; // [11:0]
        uint32_t __31_12 : 20;              // [31:12]
    } b;
} REG_REG_FW_SYSCTRL_BIT_CTRL_ADDR_ARRAY5_T;

// bit_ctrl_addr_array6
typedef union {
    uint32_t v;
    struct
    {
        uint32_t bit_ctrl_addr_array6 : 12; // [11:0]
        uint32_t __31_12 : 20;              // [31:12]
    } b;
} REG_REG_FW_SYSCTRL_BIT_CTRL_ADDR_ARRAY6_T;

// bit_ctrl_addr_array7
typedef union {
    uint32_t v;
    struct
    {
        uint32_t bit_ctrl_addr_array7 : 12; // [11:0]
        uint32_t __31_12 : 20;              // [31:12]
    } b;
} REG_REG_FW_SYSCTRL_BIT_CTRL_ADDR_ARRAY7_T;

// bit_ctrl_addr_array8
typedef union {
    uint32_t v;
    struct
    {
        uint32_t bit_ctrl_addr_array8 : 12; // [11:0]
        uint32_t __31_12 : 20;              // [31:12]
    } b;
} REG_REG_FW_SYSCTRL_BIT_CTRL_ADDR_ARRAY8_T;

// bit_ctrl_addr_array9
typedef union {
    uint32_t v;
    struct
    {
        uint32_t bit_ctrl_addr_array9 : 12; // [11:0]
        uint32_t __31_12 : 20;              // [31:12]
    } b;
} REG_REG_FW_SYSCTRL_BIT_CTRL_ADDR_ARRAY9_T;

// bit_ctrl_addr_array10
typedef union {
    uint32_t v;
    struct
    {
        uint32_t bit_ctrl_addr_array10 : 12; // [11:0]
        uint32_t __31_12 : 20;               // [31:12]
    } b;
} REG_REG_FW_SYSCTRL_BIT_CTRL_ADDR_ARRAY10_T;

// bit_ctrl_addr_array11
typedef union {
    uint32_t v;
    struct
    {
        uint32_t bit_ctrl_addr_array11 : 12; // [11:0]
        uint32_t __31_12 : 20;               // [31:12]
    } b;
} REG_REG_FW_SYSCTRL_BIT_CTRL_ADDR_ARRAY11_T;

// bit_ctrl_addr_array12
typedef union {
    uint32_t v;
    struct
    {
        uint32_t bit_ctrl_addr_array12 : 12; // [11:0]
        uint32_t __31_12 : 20;               // [31:12]
    } b;
} REG_REG_FW_SYSCTRL_BIT_CTRL_ADDR_ARRAY12_T;

// bit_ctrl_addr_array13
typedef union {
    uint32_t v;
    struct
    {
        uint32_t bit_ctrl_addr_array13 : 12; // [11:0]
        uint32_t __31_12 : 20;               // [31:12]
    } b;
} REG_REG_FW_SYSCTRL_BIT_CTRL_ADDR_ARRAY13_T;

// bit_ctrl_addr_array14
typedef union {
    uint32_t v;
    struct
    {
        uint32_t bit_ctrl_addr_array14 : 12; // [11:0]
        uint32_t __31_12 : 20;               // [31:12]
    } b;
} REG_REG_FW_SYSCTRL_BIT_CTRL_ADDR_ARRAY14_T;

// bit_ctrl_addr_array15
typedef union {
    uint32_t v;
    struct
    {
        uint32_t bit_ctrl_addr_array15 : 12; // [11:0]
        uint32_t __31_12 : 20;               // [31:12]
    } b;
} REG_REG_FW_SYSCTRL_BIT_CTRL_ADDR_ARRAY15_T;

// reg_rd_ctrl_0
#define REG_FW_SYSCTRL_AON_SOFT_RST_CTRL0_RD_SEC (1 << 0)
#define REG_FW_SYSCTRL_CLKEN_LTE_RD_SEC (1 << 1)
#define REG_FW_SYSCTRL_CLKEN_LTE_INTF_RD_SEC (1 << 2)
#define REG_FW_SYSCTRL_RSTCTRL_LTE_RD_SEC (1 << 3)
#define REG_FW_SYSCTRL_LTE_AUTOGATE_MODE_RD_SEC (1 << 4)
#define REG_FW_SYSCTRL_LTE_AUTOGATE_EN_RD_SEC (1 << 5)
#define REG_FW_SYSCTRL_LTE_AUTOGATE_DELAY_NUM_RD_SEC (1 << 6)
#define REG_FW_SYSCTRL_AON_LPC_CTRL_RD_SEC (1 << 7)
#define REG_FW_SYSCTRL_AON_CLOCK_EN0_RD_SEC (1 << 8)
#define REG_FW_SYSCTRL_AON_CLOCK_EN1_RD_SEC (1 << 9)
#define REG_FW_SYSCTRL_AON_CLOCK_AUTO_SEL0_RD_SEC (1 << 10)
#define REG_FW_SYSCTRL_AON_CLOCK_AUTO_SEL1_RD_SEC (1 << 11)
#define REG_FW_SYSCTRL_AON_CLOCK_AUTO_SEL2_RD_SEC (1 << 12)
#define REG_FW_SYSCTRL_AON_CLOCK_AUTO_SEL3_RD_SEC (1 << 13)
#define REG_FW_SYSCTRL_AON_CLOCK_FORCE_EN0_RD_SEC (1 << 14)
#define REG_FW_SYSCTRL_AON_CLOCK_FORCE_EN1_RD_SEC (1 << 15)
#define REG_FW_SYSCTRL_AON_CLOCK_FORCE_EN2_RD_SEC (1 << 16)
#define REG_FW_SYSCTRL_AON_CLOCK_FORCE_EN3_RD_SEC (1 << 17)
#define REG_FW_SYSCTRL_AON_SOFT_RST_CTRL1_RD_SEC (1 << 18)
#define REG_FW_SYSCTRL_MIPI_CSI_CFG_REG_RD_SEC (1 << 19)
#define REG_FW_SYSCTRL_CFG_CLK_UART2_RD_SEC (1 << 20)
#define REG_FW_SYSCTRL_CFG_CLK_UART3_RD_SEC (1 << 21)
#define REG_FW_SYSCTRL_CFG_CLK_DEBUG_HOST_RD_SEC (1 << 22)
#define REG_FW_SYSCTRL_RC_CALIB_CTRL_RD_SEC (1 << 23)
#define REG_FW_SYSCTRL_RC_CALIB_TH_VAL_RD_SEC (1 << 24)
#define REG_FW_SYSCTRL_RC_CALIB_OUT_VAL_RD_SEC (1 << 25)
#define REG_FW_SYSCTRL_EMMC_SLICE_PHY_CTRL_RD_SEC (1 << 26)
#define REG_FW_SYSCTRL_DMA_REQ_CTRL_RD_SEC (1 << 27)
#define REG_FW_SYSCTRL_APT_TRIGGER_SEL_RD_SEC (1 << 28)
#define REG_FW_SYSCTRL_AHB2AHB_AB_FUNCDMA_CTRL_RD_SEC (1 << 29)
#define REG_FW_SYSCTRL_AHB2AHB_AB_FUNCDMA_STS_RD_SEC (1 << 30)
#define REG_FW_SYSCTRL_AHB2AHB_AB_DAP_CTRL_RD_SEC (1 << 31)

// reg_rd_ctrl_1
#define REG_FW_SYSCTRL_AHB2AHB_AB_DAP_STS_RD_SEC (1 << 0)
#define REG_FW_SYSCTRL_AHB2AXI_PUB_CTRL_RD_SEC (1 << 1)
#define REG_FW_SYSCTRL_AHB2AXI_PUB_STS_RD_SEC (1 << 2)
#define REG_FW_SYSCTRL_AXI2AXI_PUB_STS_0_RD_SEC (1 << 3)
#define REG_FW_SYSCTRL_AXI2AXI_PUB_STS_1_RD_SEC (1 << 4)
#define REG_FW_SYSCTRL_AHB2AHB_AB_AON2LPS_CTRL_RD_SEC (1 << 5)
#define REG_FW_SYSCTRL_AHB2AHB_AB_AON2LPS_STS_RD_SEC (1 << 6)
#define REG_FW_SYSCTRL_AHB2AHB_AB_LPS2AON_CTRL_RD_SEC (1 << 7)
#define REG_FW_SYSCTRL_AHB2AHB_AB_LPS2AON_STS_RD_SEC (1 << 8)
#define REG_FW_SYSCTRL_SYSCTRL_REG0_RD_SEC (1 << 9)
#define REG_FW_SYSCTRL_PLLS_STS_RD_SEC (1 << 10)
#define REG_FW_SYSCTRL_CFG_AON_ANTI_HANG_RD_SEC (1 << 11)
#define REG_FW_SYSCTRL_CFG_AON_QOS_RD_SEC (1 << 12)
#define REG_FW_SYSCTRL_AON_AHB_MTX_SLICE_AUTOGATE_EN_RD_SEC (1 << 13)
#define REG_FW_SYSCTRL_DAP_DJTAG_EN_CFG_RD_SEC (1 << 14)
#define REG_FW_SYSCTRL_LTE_AHB2AHB_SYNC_CFG_RD_SEC (1 << 15)
#define REG_FW_SYSCTRL_CFG_AON_IO_CORE_IE_0_RD_SEC (1 << 16)
#define REG_FW_SYSCTRL_CFG_AON_IO_CORE_IE_1_RD_SEC (1 << 17)
#define REG_FW_SYSCTRL_CFG_AON_IO_CORE_IE_2_RD_SEC (1 << 18)
#define REG_FW_SYSCTRL_CFG_AON_IO_CORE_IE_3_RD_SEC (1 << 19)

// reg_wr_ctrl_0
#define REG_FW_SYSCTRL_AON_SOFT_RST_CTRL0_WR_SEC (1 << 0)
#define REG_FW_SYSCTRL_CLKEN_LTE_WR_SEC (1 << 1)
#define REG_FW_SYSCTRL_CLKEN_LTE_INTF_WR_SEC (1 << 2)
#define REG_FW_SYSCTRL_RSTCTRL_LTE_WR_SEC (1 << 3)
#define REG_FW_SYSCTRL_LTE_AUTOGATE_MODE_WR_SEC (1 << 4)
#define REG_FW_SYSCTRL_LTE_AUTOGATE_EN_WR_SEC (1 << 5)
#define REG_FW_SYSCTRL_LTE_AUTOGATE_DELAY_NUM_WR_SEC (1 << 6)
#define REG_FW_SYSCTRL_AON_LPC_CTRL_WR_SEC (1 << 7)
#define REG_FW_SYSCTRL_AON_CLOCK_EN0_WR_SEC (1 << 8)
#define REG_FW_SYSCTRL_AON_CLOCK_EN1_WR_SEC (1 << 9)
#define REG_FW_SYSCTRL_AON_CLOCK_AUTO_SEL0_WR_SEC (1 << 10)
#define REG_FW_SYSCTRL_AON_CLOCK_AUTO_SEL1_WR_SEC (1 << 11)
#define REG_FW_SYSCTRL_AON_CLOCK_AUTO_SEL2_WR_SEC (1 << 12)
#define REG_FW_SYSCTRL_AON_CLOCK_AUTO_SEL3_WR_SEC (1 << 13)
#define REG_FW_SYSCTRL_AON_CLOCK_FORCE_EN0_WR_SEC (1 << 14)
#define REG_FW_SYSCTRL_AON_CLOCK_FORCE_EN1_WR_SEC (1 << 15)
#define REG_FW_SYSCTRL_AON_CLOCK_FORCE_EN2_WR_SEC (1 << 16)
#define REG_FW_SYSCTRL_AON_CLOCK_FORCE_EN3_WR_SEC (1 << 17)
#define REG_FW_SYSCTRL_AON_SOFT_RST_CTRL1_WR_SEC (1 << 18)
#define REG_FW_SYSCTRL_MIPI_CSI_CFG_REG_WR_SEC (1 << 19)
#define REG_FW_SYSCTRL_CFG_CLK_UART2_WR_SEC (1 << 20)
#define REG_FW_SYSCTRL_CFG_CLK_UART3_WR_SEC (1 << 21)
#define REG_FW_SYSCTRL_CFG_CLK_DEBUG_HOST_WR_SEC (1 << 22)
#define REG_FW_SYSCTRL_RC_CALIB_CTRL_WR_SEC (1 << 23)
#define REG_FW_SYSCTRL_RC_CALIB_TH_VAL_WR_SEC (1 << 24)
#define REG_FW_SYSCTRL_RC_CALIB_OUT_VAL_WR_SEC (1 << 25)
#define REG_FW_SYSCTRL_EMMC_SLICE_PHY_CTRL_WR_SEC (1 << 26)
#define REG_FW_SYSCTRL_DMA_REQ_CTRL_WR_SEC (1 << 27)
#define REG_FW_SYSCTRL_APT_TRIGGER_SEL_WR_SEC (1 << 28)
#define REG_FW_SYSCTRL_AHB2AHB_AB_FUNCDMA_CTRL_WR_SEC (1 << 29)
#define REG_FW_SYSCTRL_AHB2AHB_AB_FUNCDMA_STS_WR_SEC (1 << 30)
#define REG_FW_SYSCTRL_AHB2AHB_AB_DAP_CTRL_WR_SEC (1 << 31)

// reg_wr_ctrl_1
#define REG_FW_SYSCTRL_AHB2AHB_AB_DAP_STS_WR_SEC (1 << 0)
#define REG_FW_SYSCTRL_AHB2AXI_PUB_CTRL_WR_SEC (1 << 1)
#define REG_FW_SYSCTRL_AHB2AXI_PUB_STS_WR_SEC (1 << 2)
#define REG_FW_SYSCTRL_AXI2AXI_PUB_STS_0_WR_SEC (1 << 3)
#define REG_FW_SYSCTRL_AXI2AXI_PUB_STS_1_WR_SEC (1 << 4)
#define REG_FW_SYSCTRL_AHB2AHB_AB_AON2LPS_CTRL_WR_SEC (1 << 5)
#define REG_FW_SYSCTRL_AHB2AHB_AB_AON2LPS_STS_WR_SEC (1 << 6)
#define REG_FW_SYSCTRL_AHB2AHB_AB_LPS2AON_CTRL_WR_SEC (1 << 7)
#define REG_FW_SYSCTRL_AHB2AHB_AB_LPS2AON_STS_WR_SEC (1 << 8)
#define REG_FW_SYSCTRL_SYSCTRL_REG0_WR_SEC (1 << 9)
#define REG_FW_SYSCTRL_PLLS_STS_WR_SEC (1 << 10)
#define REG_FW_SYSCTRL_CFG_AON_ANTI_HANG_WR_SEC (1 << 11)
#define REG_FW_SYSCTRL_CFG_AON_QOS_WR_SEC (1 << 12)
#define REG_FW_SYSCTRL_AON_AHB_MTX_SLICE_AUTOGATE_EN_WR_SEC (1 << 13)
#define REG_FW_SYSCTRL_DAP_DJTAG_EN_CFG_WR_SEC (1 << 14)
#define REG_FW_SYSCTRL_LTE_AHB2AHB_SYNC_CFG_WR_SEC (1 << 15)
#define REG_FW_SYSCTRL_CFG_AON_IO_CORE_IE_0_WR_SEC (1 << 16)
#define REG_FW_SYSCTRL_CFG_AON_IO_CORE_IE_1_WR_SEC (1 << 17)
#define REG_FW_SYSCTRL_CFG_AON_IO_CORE_IE_2_WR_SEC (1 << 18)
#define REG_FW_SYSCTRL_CFG_AON_IO_CORE_IE_3_WR_SEC (1 << 19)

// bit_ctrl_addr_array0
#define REG_FW_SYSCTRL_BIT_CTRL_ADDR_ARRAY0(n) (((n)&0xfff) << 0)

// bit_ctrl_addr_array1
#define REG_FW_SYSCTRL_BIT_CTRL_ADDR_ARRAY1(n) (((n)&0xfff) << 0)

// bit_ctrl_addr_array2
#define REG_FW_SYSCTRL_BIT_CTRL_ADDR_ARRAY2(n) (((n)&0xfff) << 0)

// bit_ctrl_addr_array3
#define REG_FW_SYSCTRL_BIT_CTRL_ADDR_ARRAY3(n) (((n)&0xfff) << 0)

// bit_ctrl_addr_array4
#define REG_FW_SYSCTRL_BIT_CTRL_ADDR_ARRAY4(n) (((n)&0xfff) << 0)

// bit_ctrl_addr_array5
#define REG_FW_SYSCTRL_BIT_CTRL_ADDR_ARRAY5(n) (((n)&0xfff) << 0)

// bit_ctrl_addr_array6
#define REG_FW_SYSCTRL_BIT_CTRL_ADDR_ARRAY6(n) (((n)&0xfff) << 0)

// bit_ctrl_addr_array7
#define REG_FW_SYSCTRL_BIT_CTRL_ADDR_ARRAY7(n) (((n)&0xfff) << 0)

// bit_ctrl_addr_array8
#define REG_FW_SYSCTRL_BIT_CTRL_ADDR_ARRAY8(n) (((n)&0xfff) << 0)

// bit_ctrl_addr_array9
#define REG_FW_SYSCTRL_BIT_CTRL_ADDR_ARRAY9(n) (((n)&0xfff) << 0)

// bit_ctrl_addr_array10
#define REG_FW_SYSCTRL_BIT_CTRL_ADDR_ARRAY10(n) (((n)&0xfff) << 0)

// bit_ctrl_addr_array11
#define REG_FW_SYSCTRL_BIT_CTRL_ADDR_ARRAY11(n) (((n)&0xfff) << 0)

// bit_ctrl_addr_array12
#define REG_FW_SYSCTRL_BIT_CTRL_ADDR_ARRAY12(n) (((n)&0xfff) << 0)

// bit_ctrl_addr_array13
#define REG_FW_SYSCTRL_BIT_CTRL_ADDR_ARRAY13(n) (((n)&0xfff) << 0)

// bit_ctrl_addr_array14
#define REG_FW_SYSCTRL_BIT_CTRL_ADDR_ARRAY14(n) (((n)&0xfff) << 0)

// bit_ctrl_addr_array15
#define REG_FW_SYSCTRL_BIT_CTRL_ADDR_ARRAY15(n) (((n)&0xfff) << 0)

#endif // _REG_FW_SYSCTRL_H_