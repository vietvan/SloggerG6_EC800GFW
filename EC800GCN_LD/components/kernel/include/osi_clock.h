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

#ifndef _OSI_CLOCK_H_
#define _OSI_CLOCK_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief callback function for clk_sys change
 *
 * The callback is platform dependent. Typical usage is for hardware module,
 * whose clock is divided from clk_sys. So, when clk_sys is changed, it is
 * needed to update the divider from clk_sys.
 *
 * It is recommended to add callback only in hardware driver, and divider
 * change is needed.
 *
 * To ensure atomic, the callback will be called with interrupt disabled.
 * So, the callback should be as fast as possible, even trace inside
 * callback is discouraged.
 *
 * \param [in] sysClkFreq   new clk_sys to be changed
 * \return  hardware module working frequency
 */
typedef uint32_t (*osiSysClkChangeCallback_t)(uint32_t sysClkFreq);

/**
 * \brief clk_sys change callback registry
 */
typedef struct
{
    uint32_t tag; ///< name tag
    //! @cond Doxygen_Suppress
    uint32_t dummy0;
    uint32_t dummy1;
    uint32_t dummy2;
    //! @endcond
} osiSysClkCallbackRegistry_t;

/**
 * \brief clk_sys constraint registry
 */
typedef struct
{
    uint32_t tag; ///< name tag
    //! @cond Doxygen_Suppress
    uint32_t dummy0;
    uint32_t dummy1;
    uint32_t dummy2;
    uint32_t dummy3;
    uint32_t dummy4;
    //! @endcond
} osiClockConstrainRegistry_t;

/**
 * \brief clock management module initialization
 */
void osiClockManInit(void);

/**
 * \brief clock management start
 *
 * At initialization, clock management will just record clock constrains. Only
 * after this is called, clock management will start to change system clock
 * based on clock constrains.
 */
void osiClockManStart(void);

/**
 * \brief register clk_sys change callback
 *
 * When \p cb is NULL, it is the same as \p osiUnregisterSysClkChangeCallback(r).
 *
 * \param [in] r        callback registry, must be valid
 * \param [in] cb       callback function
 */
void osiRegisterSysClkChangeCallback(osiSysClkCallbackRegistry_t *r, osiSysClkChangeCallback_t cb);

/**
 * \brief unregister clk_sys change callback
 *
 * \param [in] r        callback registry, must be valid
 */
void osiUnregisterSysClkChangeCallback(osiSysClkCallbackRegistry_t *r);

/**
 * \brief initialize clock constrain
 *
 * \param [in] r        clock constrain registry, must be valid
 * \param [in] tag      tag name of the clock constrain
 */
void osiClockConstrainInit(osiClockConstrainRegistry_t *r, uint32_t tag);

/**
 * \brief set hardware minimal clock constrain
 *
 * It is not needed to specify \p freq to be supported by hardware divider.
 * The **closest** supported hardware divider will be chosen.
 *
 * When \p freq is too high to be supported by hardware, the highest
 * supported frequency will be chosen.
 *
 * After this call, the actual clk_sys may be different with \p freq:
 * - When there are higher frequency requests, the actual clk_sys frequency
 *   may be higher than \p freq.
 * - When the closest supported frequency is lower, the actual clk_sys
 *   frequency may be lower than \p freq.
 * - When \p freq is too high, clk_sys will be set to be the highest
 *   supported frequency.
 *
 * \param [in] r        clock constrain registry, must be valid
 * \param [in] freq     minimum clk_sys frequency
 */
void osiRequestSysClk(osiClockConstrainRegistry_t *r, uint32_t freq);

/**
 * \brief set hardware maximum clock constrain
 *
 * This is for special purpose. In some cases, some hardware can only work
 * when sys_clk doesn't exceed a certain frequency. This can be called to
 * set the limit.
 *
 * This will have higher priority than \p osiRequestSysClk. For example,
 * module A call \p osiRequestSysClk to request minimal frequency 200MHz,
 * and module B call \p osiRequestMaxSysClk to limit the maximum frequency
 * 100MHz, sys_clk will be changed to 100MHz though it doesn't satisfy
 * the requirement of module A.
 *
 * \param [in] r        clock constrain registry, must be valid
 * \param [in] freq     maximum clk_sys frequency
 */
void osiRequestMaxSysClk(osiClockConstrainRegistry_t *r, uint32_t freq);

/**
 * \brief set hardware minimal clock constrain, without specified frequency
 *
 * Besides it will prevent system sleep, it will prevent clk_sys related
 * power consumption optimization.
 *
 * \param [in] r        clock constrain registry, must be valid
 */
void osiRequestSysClkActive(osiClockConstrainRegistry_t *r);

/**
 * \brief set software minimal clock constrain
 *
 * It is similar to \p osiRequestSysClk. However, it won't prevent system
 * sleep or clk_sys related power consumption optimization.
 *
 * \param [in] r        clock constrain registry, must be valid
 * \param [in] freq     minimum clk_sys frequency
 */
void osiRequestPerfClk(osiClockConstrainRegistry_t *r, uint32_t freq);

/**
 * \brief unset software or hardware minimal clock constrain
 *
 * \param [in] r        clock constrain registry, must be valid
 */
void osiReleaseClk(osiClockConstrainRegistry_t *r);

/**
 * \brief whether slow down sys clock is allowed
 *
 * \return
 *      - true if allowed
 *      - false if not allowed
 */
bool osiIsSlowSysClkAllowed(void);

/**
 * \brief reapply clk_sys constrains
 *
 * It is possible that some constrains aren't satisfied immediately,
 * especially to decrease clk_sys. So, it may be needed to reapply
 * constrains. Usually, it is called in idle thread.
 *
 * It should be called by system only.
 */
void osiReapplySysClk(void);

/**
 * \brief dump clock constrain information to memory
 *
 * It is for debug only. The data format of timer information dump is
 * not stable, and may change. When \p mem is NULL, it will return the
 * estimated dump size.
 *
 * \param mem       memory for clock constrain information dump
 * \param size      provided memory size
 * \return
 *      - dump memory size
 *      - -1 if memory size of not enough
 */
int osiClockConstrainDump(void *mem, unsigned size);

#ifdef __cplusplus
}
#endif
#endif
