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

#ifndef _HAL_CHIP_H_
#define _HAL_CHIP_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "hal_config.h"
#include "osi_compiler.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HAL_CHIP_ID_UNKNOWN (0xffff0000)
#define HAL_CHIP_ID_8955 (0x89550000)
#define HAL_CHIP_ID_8909 (0x89090000)
#define HAL_CHIP_ID_8910 (0x89100000)
#define HAL_CHIP_ID_8811 (0x88110000)

#define HAL_FREQ_16K (16384)
#define HAL_FREQ_32K (32768)
#define HAL_FREQ_1M (1000000)
#define HAL_FREQ_2M (2000000)
#define HAL_FREQ_6P5M (6500000)
#define HAL_FREQ_13M (13 * 1000000)
#define HAL_FREQ_20M (20 * 1000000)
#define HAL_FREQ_26M (26 * 1000000)
#define HAL_FREQ_39M (39 * 1000000)
#define HAL_FREQ_45M (45 * 1000000)
#define HAL_FREQ_52M (52 * 1000000)
#define HAL_FREQ_57M (57 * 1000000)
#define HAL_FREQ_62M (62 * 1000000)
#define HAL_FREQ_69M (69 * 1000000)
#define HAL_FREQ_78M (78 * 1000000)
#define HAL_FREQ_89M (89 * 1000000)
#define HAL_FREQ_104M (104 * 1000000)
#define HAL_FREQ_113M (113 * 1000000)
#define HAL_FREQ_125M (125 * 1000000)
#define HAL_FREQ_139M (139 * 1000000)
#define HAL_FREQ_156M (156 * 1000000)
#define HAL_FREQ_178M (178 * 1000000)
#define HAL_FREQ_208M (208 * 1000000)
#define HAL_FREQ_250M (250 * 1000000)
#define HAL_FREQ_312M (312 * 1000000)

#define HAL_NAME_DMA OSI_MAKE_TAG('D', 'M', 'A', ' ')
#define HAL_NAME_SIM OSI_MAKE_TAG('S', 'I', 'M', ' ')
#define HAL_NAME_XCV OSI_MAKE_TAG('X', 'C', 'V', ' ')
#define HAL_NAME_CALIB OSI_MAKE_TAG('C', 'A', 'L', 'I')
#define HAL_NAME_PAL OSI_MAKE_TAG('P', 'A', 'L', ' ')
#define HAL_NAME_USB OSI_MAKE_TAG('U', 'S', 'B', ' ')
#define HAL_NAME_NETCARD OSI_MAKE_TAG('N', 'E', 'T', 'C')
#define HAL_NAME_CAM OSI_MAKE_TAG('C', 'A', 'M', ' ')
#define HAL_NAME_PWM OSI_MAKE_TAG('P', 'W', 'M', ' ')

#define REG_APPLY_TYPE_END 0
#define REG_APPLY_TYPE_UDELAY 1

#define REG_APPLY_WRITE(address, value) (uint32_t)(address), (uint32_t)(value)
#define REG_APPLY_UDELAY(us) (uint32_t)(REG_APPLY_TYPE_UDELAY), (uint32_t)(us)
#define REG_APPLY_END REG_APPLY_TYPE_END

#define HAL_FLASH_OFFSET(address) ((unsigned)(address)&0xffffff)
#define HAL_FLASH_DEVICE_NAME(address) HAL_CHIP_FLASH_DEVICE_NAME((uintptr_t)(address))

#define HAL_ADDR_IS_ADI(address) HAL_CHIP_ADDR_IS_ADI((uintptr_t)(address))
#define HAL_ADDR_IS_SRAM(address) HAL_CHIP_ADDR_IS_SRAM((uintptr_t)(address))
#define HAL_ADDR_IS_RAM(address) HAL_CHIP_ADDR_IS_RAM((uintptr_t)(address))
#define HAL_ADDR_IS_SRAM_RAM(address) HAL_CHIP_ADDR_IS_SRAM_RAM((uintptr_t)(address))

enum
{
    HAL_CLK_DIV1P0 = 2,
    HAL_CLK_DIV1P5 = 3,
    HAL_CLK_DIV2P0 = 4,
    HAL_CLK_DIV2P5 = 5,
    HAL_CLK_DIV3P0 = 6,
    HAL_CLK_DIV3P5 = 7,
    HAL_CLK_DIV4P0 = 8,
    HAL_CLK_DIV4P5 = 9,
    HAL_CLK_DIV5P0 = 10,
    HAL_CLK_DIV5P5 = 11,
    HAL_CLK_DIV6P0 = 12,
    HAL_CLK_DIV7P0 = 14,
    HAL_CLK_DIV8P0 = 16,
    HAL_CLK_DIV12P0 = 24,
    HAL_CLK_DIV16P0 = 32,
    HAL_CLK_DIV24P0 = 48,
};

enum
{
    HAL_METAL_ID_U01 = 1,
    HAL_METAL_ID_U02,
    HAL_METAL_ID_U03,
    HAL_METAL_ID_U04,
};

enum
{
    HAL_METAL_ID_AA = 1,
    HAL_METAL_ID_AB,
    HAL_METAL_ID_AC,
    HAL_METAL_ID_AD,
};

#define HAL_METAL_BEFORE(metal, n) ((metal) < (n))
#define HAL_METAL_FROM(metal, n) ((metal) >= (n))

enum
{
    HAL_VCORE_USER_SYS,
    HAL_VCORE_USER_VOC,
    HAL_VCORE_USER_COUNT,
};

enum
{
    HAL_VCORE_LEVEL_LOW,
    HAL_VCORE_LEVEL_MEDIUM,
    HAL_VCORE_LEVEL_HIGH,
    HAL_VCORE_LEVEL_COUNT,
};

enum
{
    HAL_VCORE_MODE_LDO,
    HAL_VCORE_MODE_DCDC
};

typedef enum
{
    PM0_POWER_MODE_POWERDOWN,
    PM0_POWER_MODE_POWERUP,
    PM0_POWER_MODE_LOWPOWER
} pm0PowerMode_t;

enum
{
    HAL_POWER_DCDC_GEN = OSI_MAKE_TAG('D', 'C', 'D', 'C'),
    HAL_POWER_CORE = OSI_MAKE_TAG('C', 'O', 'R', 'E'),
    HAL_POWER_BUCK_PA = OSI_MAKE_TAG('D', 'C', 'P', 'A'),
    HAL_POWER_SPIMEM = OSI_MAKE_TAG('S', 'P', 'I', 'M'),
    HAL_POWER_MEM = OSI_MAKE_TAG('M', 'E', 'M', ' '),
    HAL_POWER_AVDD18 = OSI_MAKE_TAG('A', 'V', '1', '8'),
    HAL_POWER_RF15 = OSI_MAKE_TAG('R', 'F', '1', '5'),
    HAL_POWER_WCN = OSI_MAKE_TAG('W', 'C', 'N', ' '),
    HAL_POWER_DDR12 = OSI_MAKE_TAG('D', 'D', 'R', '1'),
    HAL_POWER_CAMD = OSI_MAKE_TAG('C', 'A', 'M', 'D'),
    HAL_POWER_CAMA = OSI_MAKE_TAG('C', 'A', 'M', 'A'),
    HAL_POWER_USB = OSI_MAKE_TAG('U', 'S', 'B', ' '),
    HAL_POWER_LCD = OSI_MAKE_TAG('L', 'C', 'D', ' '),
    HAL_POWER_SD = OSI_MAKE_TAG('S', 'D', ' ', ' '),
    HAL_POWER_SIM0 = OSI_MAKE_TAG('S', 'I', 'M', '0'),
    HAL_POWER_SIM1 = OSI_MAKE_TAG('S', 'I', 'M', '1'),
    HAL_POWER_VIBR = OSI_MAKE_TAG('V', 'I', 'B', 'R'),
    HAL_POWER_DCXO = OSI_MAKE_TAG('D', 'C', 'X', 'O'),
    HAL_POWER_VDD28 = OSI_MAKE_TAG('V', 'D', 'D', '2'),
    HAL_POWER_VIO18 = OSI_MAKE_TAG('V', 'I', 'O', '1'),
    HAL_POWER_VIO33 = OSI_MAKE_TAG('V', 'I', 'O', '3'),
    HAL_POWER_KEYLED = OSI_MAKE_TAG('K', 'E', 'Y', 'L'),
    HAL_POWER_BACK_LIGHT = OSI_MAKE_TAG('B', 'A', 'C', 'K'),
    HAL_POWER_ANALOG = OSI_MAKE_TAG('A', 'N', 'A', 'L'),
    HAL_POWER_CAMFLASH = OSI_MAKE_TAG('C', 'A', 'M', 'F'),
    HAL_POWER_VBAT_RF = OSI_MAKE_TAG('V', 'B', 'R', 'F'),
    HAL_POWER_DCDC_VRF = OSI_MAKE_TAG('D', 'C', 'R', 'F'),
    HAL_POWER_RF12 = OSI_MAKE_TAG('R', 'F', '1', '2'),
    HAL_POWER_LP18 = OSI_MAKE_TAG('L', 'P', '1', '8'),
    HAL_POWER_EMM = OSI_MAKE_TAG('E', 'M', 'M', ' '),
    HAL_POWER_CP = OSI_MAKE_TAG('C', 'P', ' ', ' '),
    HAL_POWER_VPA = OSI_MAKE_TAG('V', 'P', 'A', ' '),
#ifdef CONFIG_QUEC_PROJECT_FEATURE_GPIO
    HAL_POWER_NONE = OSI_MAKE_TAG('N', 'O', 'N', 'E'),
#endif
};

typedef enum
{
    HAL_SIM_VOLT_CLASS_A = 0, // 5v
    HAL_SIM_VOLT_CLASS_B = 1, // 3.3v
    HAL_SIM_VOLT_CLASS_C = 2, // 1.8v
    HAL_SIM_VOLT_OFF = 3,
} halSimVoltClass_t;

typedef enum
{
    SENSOR_VDD_3300MV = 3300,
    SENSOR_VDD_3000MV = 3000,
    SENSOR_VDD_2800MV = 2800,
    SENSOR_VDD_2500MV = 2500,
    SENSOR_VDD_1800MV = 1800,
    SENSOR_VDD_1500MV = 1500,
    SENSOR_VDD_1300MV = 1300,
    SENSOR_VDD_1200MV = 1200,
    SENSOR_VDD_CLOSED,
    SENSOR_VDD_UNUSED
} cameraVDD_t;

typedef enum
{
    WCN_VDD_1800MV = 1800,
    WCN_VDD_1700MV = 1700,
    WCN_VDD_1600MV = 1600,
    WCN_VDD_1500MV = 1500, //default
    WCN_VDD_1400MV = 1400,
    WCN_VDD_1300MV = 1300,
    WCN_VDD_1200MV = 1200,
    WCN_VDD_1100MV = 1100,
    WCN_VDD_UNUSED
} wcnVDD_t;

typedef enum
{
    POWER_LEVEL_3200MV = 3200,
    POWER_LEVEL_3100MV = 3100,
    POWER_LEVEL_3000MV = 3000,
    POWER_LEVEL_2900MV = 2900,
    POWER_LEVEL_2800MV = 2800,
    POWER_LEVEL_2700MV = 2700,
    POWER_LEVEL_2600MV = 2600,
    POWER_LEVEL_2500MV = 2500,
    POWER_LEVEL_2400MV = 2400,
    POWER_LEVEL_2300MV = 2300,
    POWER_LEVEL_2200MV = 2200,
    POWER_LEVEL_2100MV = 2100,
    POWER_LEVEL_2000MV = 2000,
    POWER_LEVEL_1900MV = 1900,
    POWER_LEVEL_1800MV = 1800,
    POWER_LEVEL_1700MV = 1700,
    POWER_LEVEL_1600MV = 1600,
    POWER_LEVEL_1500MV = 1500,
    POWER_LEVEL_1400MV = 1400,
    POWER_LEVEL_1300MV = 1300,
    POWER_LEVEL_1200MV = 1200,
    POWER_LEVEL_1100MV = 1100,
    POWER_LEVEL_UNUSED
} powerLevel_t;

typedef enum
{
    HAL_CLK32K_SRC_RC,
    HAL_CLK32K_SRC_XTAL32K,
    HAL_CLK32K_SRC_XTAL26M,
} halClk32kSource_t;

typedef enum
{
    PM2_POWER_MODE_POWERDOWN,
    PM2_POWER_MODE_LOWPOWER
} pm2PowerMode_t;

typedef enum
{
    PM3_POWER_MODE_POWERDOWN,
    PM3_POWER_MODE_LOWPOWER
} pm3PowerMode_t;

typedef enum
{
    HAL_CLOCK_INIT_BOOTLOADER,
    HAL_CLOCK_INIT_FDL,
    HAL_CLOCK_INIT_APP,
    HAL_CLOCK_INIT_OPENCPU,
    HAL_CLOCK_INIT_PM2BOOT,
} halClockInitCase_t;

typedef struct
{
    uintptr_t reg;
    uint32_t value;
} halRegValue_t;

typedef struct
{
    uintptr_t reg;
    uint32_t mask;
    uint32_t value;
} halRegMaskValue_t;

typedef struct
{
    unsigned num;
    unsigned denom;
} halFracDividerValue_t;

/**
 * \brief global variable for build revision
 *
 * Though it is constant, it is by design to put it in data section. It will
 * appear in blue screen dump when it is in data section.
 */
extern char gBuildRevision[];

/**
 * \brief read chip id
 *
 * The returned chip id is not the raw data of hardware registers, it is the
 * *logical* chip id.
 *
 * Due to the method to detect chip id is different among various chips, the
 * implementation depends on chip. When the hardware register is unexpected,
 * it returns \p HAL_CHIP_ID_UNKNOWN.
 *
 * \return  *logical* chip id
 */
uint32_t halGetChipId(void);

/**
 * \brief read chip metal id
 *
 * When the hardware register is unexpected, the returned value is the value
 * the latest metal id. For example, when current revision is \p U02, all
 * unexpected values will be regarded as \p U02.
 *
 * When it is needed to distinguish metal id, **never** use the raw hardware
 * register, and always use macros for metal id check. Examples:
 *
 * - HAL_METAL_BEFORE(metal, HAL_METAL_ID_U02): true for metal id *less than*
 *   U02, and not including U02.
 * - HAL_METAL_FROM(metal, HAL_METAL_ID_U02): true for metal id
 *   *larger or equal than* U02, and including U02.
 *
 * \return  *logical* metal id
 */
uint32_t halGetMetalId(void);

/**
 * \brief read chip A-DIE metal id
 *
 * Similar to \p halGetMetalId, just for the metal id of A-DIE.
 *
 * \return  *logical* metal id
 */
uint32_t halGetAdieMetalId(void);

/**
 * \brief bare metal boot initialization
 *
 * This will be called before run time initialization. Only the small piece of
 * codes are copied from flash into SRAM, namely *.sramboottext* section. All
 * *.data* and *.bss* sections are not initialized. Typically, it can access:
 *
 * - stack
 * - flash codes
 * - osiDebugEvent, osiDelayUS, halApplyRegisters, halApplyRegisterList
 */
void halChipPreBoot(void);

/**
 * \brief clock and memory initialization
 *
 * When this is called, only partial run time is initialized. All sections on
 * SRAM can be accessed, and sections on external RAM (PSRAM or DDR) can't be
 * accessed.
 *
 * It may be called in various senario, and it is possible there are
 * differences among them.
 *
 * \param whence caller secario
 */
void halClockInit(unsigned whence);

/**
 * \brief PSRAM/DDR initialization at cold boot
 */
void halRamInit(void);

/**
 * \brief PSRAM/DDR initialization at resume
 *
 * This is called when system is power off, power of PSRAM is kept. Also,
 * the content of RAM should be kept.
 */
void halRamWakeInit(void);

/**
 * \brief PSRAM/DDR suspend
 *
 * When PSRAM half sleep is supported, it will be processed inside.
 */
void halRamSuspend(void);

/**
 * \brief PSRAM/DDR suspend abort
 */
void halRamSuspendAbort(void);

/**
 * \brief batch write write registers, and executed on SRAM
 *
 * This function should be located in *.sramboottext* section, and the section
 * will be loaded to SRAM at the beginning of boot.
 *
 * The main purpose of this is to write registers which may affect memory
 * access. For example, it will make flash unaccessible to write some flash
 * controller registers.
 *
 * It use variadic variables. The variadic variable format is:
 *
 * - (address, value): write the value to specified register
 * - (REG_APPLY_TYPE_UDELAY, value): delay specified microseconds
 * - (REG_APPLY_TYPE_END): indicate the end of variadic variables
 *
 * \param [in] address  register address, or REG_APPLY_TYPE_UDELAY,
 *                      REG_APPLY_TYPE_END
 */
void halApplyRegisters(uint32_t address, ...);

/**
 * \brief batch write write registers, and executed on SRAM
 *
 * This function should be located in *.sramboottext* section, and the section
 * will be loaded to SRAM at the beginning of boot.
 *
 * It is similar to \p halApplyRegisters, just the parameters are located in
 * memory pointed by \p data.
 *
 * \param [in ] data    batch write parameter pointer
 */
void halApplyRegisterList(const uint32_t *data);

/**
 * \brief calculate 32bits divider
 *
 * The divider is: output = (input * num) / denom
 * - [9:0]: num
 * - [29:16]: denom
 *
 * It is used in UART divider calculation.
 *
 * \param input     input clock frequency
 * \param output    output clock frequency
 * \return
 *      - caclulated divider
 *      - 0 on failed
 */
unsigned halCalcDivider32(unsigned input, unsigned output);

/**
 * \brief calculate 24bits divider
 *
 * The divider is: output = (input * num) / denom
 * - [9:0]: num
 * - [23:10]: denom
 *
 * It is used in UART divider calculation.
 *
 * \param input     input clock frequency
 * \param output    output clock frequency
 * \return
 *      - caclulated divider
 *      - 0 on failed
 */
unsigned halCalcDivider24(unsigned input, unsigned output);

/**
 * \brief calculate fraction divider
 *
 * The divider is: output = (input * num) / denom
 *
 * \param input input clock frequency
 * \param output output clock frequency, can't be larger than \p input
 * \param num_bits bits of num
 * \param denom_bits bits of denom
 * \return
 *      - caclulated divider
 *      - (0, 0) on failed
 */
halFracDividerValue_t halCalcFracDivider(unsigned input, unsigned output,
                                         unsigned num_bits,
                                         unsigned denom_bits);

/**
 * \brief output frequency of 24bits divider
 *
 * \param input     input clock frequency
 * \param divider   divider setting
 * \result  output frequency
 */
unsigned halDivider24OutFreq(unsigned input, unsigned divider);

/**
 * \brief calculate 20bits divider
 *
 * The divider is: output = input / (div * set)
 * - [15:0]: div - 1, div >= 2
 * - [19:16]: set - 1, set >= 6
 *
 * It is better to choose larger set. And it is used in ARM UART divider
 * calculation.
 *
 * \param input     input clock frequency
 * \param output    output clock frequency
 * \return
 *      - caclulated divider
 *      - 0 on failed
 */
unsigned halCalcDivider20(unsigned input, unsigned output);

/**
 * \brief output frequency of 20bits divider
 *
 * \param input     input clock frequency
 * \param divider   divider setting
 * \result  output frequency
 */
unsigned halDivider20OutFreq(unsigned input, unsigned divider);

/**
 * \brief calculate half divider
 *
 * Half divider is 4 bits, and the real divider is:
 *  1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5
 *  5.0, 5.5, 6.0, 7.0, 8.0, 12.0, 16.0, 24.0
 *
 * \return half divider setting
 */
unsigned halCalHalfDivider(unsigned input, unsigned output);

/**
 * \brief output frequency of half divider
 *
 * \param input     input clock frequency
 * \param divider   divider setting
 * \result  output frequency
 */
unsigned halHalfDividerOutFreq(unsigned input, unsigned divider);

/**
 * \brief power on external flash
 *
 * The implementation may be empty. It should be implemented according to
 * board connection and flash type.
 *
 * When external flash isn't supported, or CONFIG_BOARD_WITH_EXT_FLASH is
 * not defined, it should be implemented as empty.
 */
void halPmuExtFlashPowerOn(void);

/**
 * \brief select clk_32k source
 * \param src clk_32k source
 * \return
 *      - true on success
 *      - false on error, invalid parameter
 */
bool halSelectClk32KSource(halClk32kSource_t src);

/**
 * \brief get clk_32k source by read register
 * \return clk_32k source
 */
halClk32kSource_t halGetClk32kSource(void);

/**
 * \brief prepare PSM module
 *
 * This is called before PSM sleep or power down. PSM counter should be
 * started here. The PSM counter will be used to measure PSM duration.
 */
void halPmuPsmPrepare(void);

/**
 * \brief read boot mode magic in pmic
 *
 * To distinguish various boot mode, magic word will be written in pmic
 * register, which will be always power on. This will read it.
 *
 * Boot mode magic will be cleared in application boot. So, caller should
 * make sure it isn't cleared at reading.
 *
 * \return magic word in always on boot mode register
 */
uint32_t halPmuReadBootMode(void);

/**
 * \brief write boot mode magic to pmic
 *
 * \param mode boot mode magic word
 */
void halPmuWriteBootMode(uint32_t mode);

/**
 * \brief write calibration boot mode magic to pwr
 *
 * \param calibration mode boot mode magic word
 */
void halPmuWriteNbCalibBootMode(uint32_t mode);

/**
 * \brief read boot mode magic in pwr
 *
 * \return calibration boot mode register
 */
uint32_t halPmuReadNbCalibBootMode();

/**
 * \brief write RTC epoch time in milliseconds
 *
 * \p ms should be inside bwteen \p CONFIG_KERNEL_MIN_UTC_SECOND and
 * \p CONFIG_KERNEL_MAX_UTC_SECOND.
 *
 * \param ms epoch time in milliseconds
 * \return
 *      - true on success
 *      - false on fail, invalid parameter
 */
bool halPmuWriteRtc(int64_t ms);

/**
 * \brief read RTC epoch time in milliseconds
 *
 * The returned epoch time may be outside of valid range. The calibration
 * ratio of clk32k is compensated inside.
 *
 * \return epoch time in milliseconds
 */
int64_t halPmuReadRtc(void);

/**
 * \brief read raw RTC epoch time in milliseconds
 *
 * Comparing \p halPmuReadRtcRaw, the calibration ratio of clk32k isn't
 * compensated. Typical usage is for debug only.
 *
 * \return epoch time in milliseconds
 */
int64_t halPmuReadRtcRaw(void);

/**
 * \brief unset (disable) RTC alarm
 */
void halPmuUnsetAlarm(void);

/**
 * \brief set (enable) RTC alarm epoch time in milliseconds
 *
 * \p ms should be inside valid range.
 *
 * It doesn't check whether \p ms is in reasonable future of current RTC
 * epoch time. Caller should check it.
 *
 * \param ms alarm epoch time
 * \return
 *      - true on success
 *      - false on fail, invalid parameter
 */
bool halPmuSetAlarm(int64_t ms);

/**
 * \brief enter psm sleep (if supported)
 *
 * On success, this won't return. It will only return on fail. Typical
 * reason is there are wake source during entering psm sleep.
 *
 * \param wake_uptime expected wakeup time, related to uptime.
 *          \p INT64_MAX for sleep forever.
 */
void halPsmSleep(int64_t wake_uptime);

/**
 * \brief enter PM2 cold boot sleep (if supported)
 *
 * \param wake_uptime expected wakeup time, related to uptime.
 *          \p INT64_MAX for sleep forever.
 */
void halPm2BootSleep(int64_t wake_uptime);

/**
 * \brief system shutdown into various mode
 *
 * It will never return. \p wake_uptime will only be used in
 * \p OSI_SHUTDOWN_POWER_OFF.
 *
 * Caller should ensure \p mode is supported. For all unsupported mode,
 * it will ba handled as \p OSI_SHUTDOWN_RESET.
 *
 * \param mode shutdown mode, refer to \p osiShutdownMode_t
 * \param wake_uptime expected wakeup time, related to uptime.
 *          \p INT64_MAX for power off forever.
 */
OSI_NO_RETURN void halShutdown(int mode, int64_t wake_uptime);

/**
 * \brief enable or disable PMU wake source
 *
 * Refer to \p HAL_RESUME_SRC_xxx for wake source definition. And this
 * only change PMU wake sources.
 *
 * \param sources one or more wakeup sources
 * \param enable true to enable wakeup
 */
void halPmuSetWakeupEnable(unsigned sources, bool enable);

/**
 * \brief enable or disable device clock
 *
 * It is only operates the clock gate, not consider the clock tree. On
 * different platforms, the valid \p name are different.
 *
 * Ussually, not all hardware clock gate are exposed to this API.
 *
 * \param name hardware module name
 * \param enable true for enable clock, false for disable clock
 */
void halSetDeviceClockEnable(unsigned name, bool enable);

/**
 * \brief configure sys_wdt
 *
 * sys_wdt is used to generate interrupt, and system will come to blue
 * screen on this interrupt. It won't reset system, and pmic_wdt will
 * reset system.
 *
 * The main purpose is to monitor system hang, for example, dead loop in
 * ISR. And context will be saved in blue screen.
 *
 * It should be called before \p halSysWdtStart.
 *
 * In 8910, SYS_IRQ_ID_TIMER_1_OS will be configured as FIQ, and it is
 * used for sys_wdt. In 8811, hwp_sysWdt is used.
 *
 * \param intr_ms timeout to generate sys_wdt interrupt
 */
void halSysWdtConfig(unsigned intr_ms);

/**
 * \brief enable or disable sys_wdt
 *
 * In rare cases, it is known that system will be busy for very long time,
 * and there are no opportunity to call \p halSysWdtStart in low priority
 * thread, this can be called to disable sys_wdt.
 *
 * Typically, it will be called to disable sys_wdt in unittests.
 *
 * \param enable true for enable sys_wdt, false for disable
 */
void halSysWdtSetEnable(bool enable);

/**
 * \brief start or restart sys_wdt
 *
 * On restart, sys_wdt counting will be cleared.
 */
void halSysWdtStart(void);

/**
 * \brief stop sys_wdt
 */
void halSysWdtStop(void);

/**
 * \brief get the supported real frequency by the request frequency
 *
 * There are reasons that the real frequency will be different from request
 * frequency:
 * - The request frequency is higher than maximum supported frequency.
 * - The divider can't provide exact frequency.
 *
 * \param freq the request frequency
 * \return the real supported frequency
 */
uint32_t halSysClkSupportedFreq(uint32_t freq);

/**
 * \brief change sys_clk frequency
 *
 * \param freq the request frequency
 */
void halChangeSysClk(uint32_t freq);

/**
 * \brief get clk_sys frequency by read registers
 *
 * \return clk_sys frequency
 */
unsigned halGetSysClkFreq(void);

/**
 * \brief save blue screen registers
 *
 * Typically, some of blue screen registers are save in blue screen entry
 * assembly. And the remaining will be saved inside.
 */
void halSaveBlueScreenReg(void);

/**
 * \brief set warm boot entry
 * \param entry warm boot entry, NULL for clear
 */
void halSetWarmBootEntry(void *entry);

/**
 * \brief convert anti-rollback efuse value to integer version
 *
 * \param verbits efuse anti-rollback value
 * \return version in [0, 64]
 */
unsigned halFromEfuseAntiRollback64(const uint32_t verbits[2]);

/**
 * \brief convert integer version to anti-rollback efuse value
 *
 * \param ver version number
 * \param verbits output efuse anti-rollback value
 * \return
 *      - true on success
 *      - false on fail, invalie version number
 */
bool halToEfuseAntiRollback64(unsigned ver, uint32_t verbits[2]);

uint32_t halFreqToPllDivider(uint32_t freq);
void hal_SysSetBBClock(uint32_t freq);
void hal_SysStartBcpu(void *bcpu_main, void *stack_start_addr);

void halIspiInit(void);
uint16_t halPmuRead(volatile uint32_t *reg);
void halPmuWrite(volatile uint32_t *reg, uint16_t value);
uint16_t halAbbRead(volatile uint32_t *reg);
void halAbbWrite(volatile uint32_t *reg, uint16_t value);

void halPmuInit(unsigned whence);

uint32_t halGetVcoreMode(void);
uint32_t halGetVcoreLevel(void);
uint8_t halRequestVcoreRegs(uint32_t id, uint32_t level, uint16_t *addr, uint16_t *value);

#if defined(CONFIG_SOC_8910) || defined(CONFIG_SOC_8850)
bool halPmuSwitchPower(uint32_t id, bool enabled, bool lp_enabled);
#endif

#if defined(CONFIG_SOC_8850)
bool halPmuSwitchPowerPm2(uint32_t id, bool pm2);
#endif

#ifdef CONFIG_SOC_8811
bool halPmuSwitchPower(uint32_t id, pm0PowerMode_t pm0_mode, pm2PowerMode_t pm2_mode, pm3PowerMode_t pm3_mode);
#endif
bool halPmuSetPowerLevel(uint32_t id, uint32_t mv);
bool halPmuSetCamFlashLevel(uint8_t level);

void halPmuEnterPm1(void);
void halPmuAbortPm1(void);
void halPmuExitPm1(void);

void halPmuEnterPm2(void);
void halPmuAbortPm2(void);
void halPmuExitPm2(void);

bool halPmuSetSimVolt(int idx, halSimVoltClass_t volt);
bool halPmuSelectSim(int idx);

void halPmuSet7sReset(bool enable);

uint32_t GetabnormalCauses(void);

#if defined(CONFIG_SOC_8910)
#include "8910/hal_chip_8910.h"
#elif defined(CONFIG_SOC_8811)
#include "8811/hal_chip_8811.h"
#elif defined(CONFIG_SOC_8850)
#include "8850/hal_chip_8850.h"
#endif

#ifdef __cplusplus
}
#endif
#endif
