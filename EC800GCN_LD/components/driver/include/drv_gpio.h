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

#ifndef _DRV_GPIO_H_
#define _DRV_GPIO_H_

#include "osi_api.h"

#include "quec_common.h"

#ifdef __cplusplus
extern "C" {
#endif

struct drvGpio;

/**
 * opaque data struct for GPIO instance
 */
typedef struct drvGpio drvGpio_t;

/**
 * GPIO interrupt callback
 *
 * It will be called when GPIO interrupt occurs. Due to it is executed in
 * ISR, the callback should follow ISR programming guides.
 */
typedef void (*drvGpioIntrCB_t)(void *ctx);

typedef enum
{
    DRV_GPIO_INPUT,
    DRV_GPIO_OUTPUT
} drvGpioMode_t;

/**
 * GPIO configuration
 */
typedef struct
{
    drvGpioMode_t mode; ///< GPIO mode
    bool out_level;     ///< level to be set for GPIO output
    bool intr_enabled;  ///< interrupt enabled, only for GPIO input
    bool intr_level;    ///< true for level interrupt, false for edge interrupt
    bool debounce;      ///< debounce enabled
    bool rising;        ///< rising edge or level high interrupt enabled
    bool falling;       ///< falling edge or level low interrupt enabled
} drvGpioConfig_t;

/**
 * @brief GPIO module initialization
 *
 * It just initialize GPIO module, and won't touch any GPIO. It should be
 * called before any \p drvGpioOpen.
 */
void drvGpioInit(void);

/**
 * @brief open a GPIO
 *
 * IOMUX will be set to GPIO mode at open.
 *
 * Each GPIO sould be opened only once. When it is already opened, this
 * API will return NULL.
 *
 * If the specified GPIO can't support the specified mode, this API will
 * return NULL.
 *
 * If the specified GPIO can't support input interrupt, and interrupt is
 * enabled, this API will return NULL.
 *
 * The returned instance is dynamic allocated, caller should free it after
 * \p drvGpioClose is called.
 *
 * @param id    GPIO id. The range may be different among chips.
 * @param cfg   GPIO configuration
 * @param cb    callback at interrupt
 * @param cb_ctx    context pointer for callback
 * @return
 *      - GPIO instance pointer
 *      - NULL if parameter is invalid
 */
drvGpio_t *drvGpioOpen(uint32_t id, const drvGpioConfig_t *cfg, drvGpioIntrCB_t cb, void *cb_ctx);

/**
 * @brief close a GPIO
 *
 * IOMUX will be kept, and the GPIO will be set to input mode.
 *
 * @param d     GPIO instance
 */
void drvGpioClose(drvGpio_t *d);

/**
 * @brief reconfigure the opened GPIO
 *
 * When the configuration is invalid, the current configuration will
 * be kept.
 *
 * @param d     GPIO instance
 * @param cfg   GPIO configuration
 * @return
 *      - true if GPIO configuration is valid
 *      - false if GPIO configuration is invalid
 */
bool drvGpioReconfig(drvGpio_t *d, const drvGpioConfig_t *cfg);

/**
 * @brief read the level of GPIO
 *
 * It can be called for both GPIO input and output. For GPIO output,
 * it is the level set by software.
 *
 * @param d     GPIO instance
 * @return
 *      - true for level high
 *      - false for level low
 */
bool drvGpioRead(drvGpio_t *d);

/**
 * @brief write level for GPIO
 *
 * When it is called for GPIO input, it will do nothing.
 *
 * When it is called for GPIO output, set the output level.
 *
 * @param d     GPIO instance
 * @param level GPIO level to be set,
 *              - true for level high
 *              - false for level low
 */
void drvGpioWrite(drvGpio_t *d, bool level);

/**
 * @brief GPIO suspend callback
 *
 * Explicit GPIO suspend and resume call will be used, in case the order
 * of GPIO suspend/resume is sensitive.
 *
 * @param mode suspend mode
 */
void drvGpioSuspend(osiSuspendMode_t mode);

/**
 * @brief GPIO resume callback
 *
 * Explicit GPIO suspend and resume call will be used, in case the order
 * of GPIO suspend/resume is sensitive.
 *
 * @param mode suspend mode
 * @param source wakeup source
 */
void drvGpioResume(osiSuspendMode_t mode, uint32_t source);

#ifdef CONFIG_QUEC_PROJECT_FEATURE_GPIO
void quec_drvGpioOpen(uint32_t id, drvGpioIntrCB_t cb, void *cb_ctx);
void quec_drvGpioClose(uint32_t id);
void quec_drvGpioInit(uint32_t gpio_set1, uint32_t gpio_set2);
#endif

#ifdef __cplusplus
}
#endif
#endif
