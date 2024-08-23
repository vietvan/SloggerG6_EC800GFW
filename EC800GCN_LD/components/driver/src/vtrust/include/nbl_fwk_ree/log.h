// Copyright 2018 GoldenRiver Technology Co., Ltd. All rights reserved.
#ifndef NBL_LOG_H_
#define NBL_LOG_H_

/// \nbl_group{ nbl_fwk_ree }

/**
 * \file
 * \brief Write log into system.
 * If in Android, use android log.
 * If in Linux, use printf functions.
 */

#if __ANDROID_API__ // In Android env.

#include <android/log.h>

#define NBL_LOGE(...)  (__android_log_print(ANDROID_LOG_ERROR,   LOG_TAG, __VA_ARGS__))
#define NBL_LOGW(...)  (__android_log_print(ANDROID_LOG_WARN,    LOG_TAG, __VA_ARGS__))
#define NBL_LOGI(...)  (__android_log_print(ANDROID_LOG_INFO,    LOG_TAG, __VA_ARGS__))
#define NBL_LOGD(...)  (__android_log_print(ANDROID_LOG_DEBUG,   LOG_TAG, __VA_ARGS__))
#define NBL_LOGV(...)  (__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__))

#define NBL_LOG_ASSERT(cond)                                                                  \
    do {                                                                                      \
        if (!(cond)) {                                                                        \
            __android_log_assert(#cond, LOG_TAG, "ASSERT: %s:%d: '%s'\n", __func__, __LINE__, \
                                 #cond);                                                      \
        }                                                                                     \
    } while (0)

#define NBL_LOG_ASSERT_MSG(cond, fmt, ...)                                                        \
    do {                                                                                          \
        if (!(cond)) {                                                                            \
            __android_log_assert(#cond, LOG_TAG, "ASSERT: %s:%d: '%s': " fmt, __func__, __LINE__, \
                                 #cond, ##__VA_ARGS__);                                           \
        }                                                                                         \
    } while (0)

#else // Not "__ANDROID_API__"

#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NBL_LOGE(fmt, ...) 	\
	trusty_printf("E:%s: %s:%d: " fmt, LOG_TAG, __func__, __LINE__,  ## __VA_ARGS__)

#define NBL_LOGW(fmt, ...) 	\
	trusty_printf("W:%s: %s:%d: " fmt, LOG_TAG, __func__, __LINE__,  ## __VA_ARGS__)

#define NBL_LOGI(fmt, ...) \
    trusty_printf("I:%s: %s:%d: " fmt, LOG_TAG, __func__, __LINE__,  ## __VA_ARGS__)

#if 0 // Debug and Verbose are NOT display.
#define NBL_LOGD(fmt, ...) \
    trusty_printf("D:%s: %s:%d: " fmt, LOG_TAG, __func__, __LINE__,  ## __VA_ARGS__)
#define NBL_LOGV(fmt, ...) \
    trusty_printf("V:%s: %s:%d: " fmt, LOG_TAG, __func__, __LINE__,  ## __VA_ARGS__)
#else
#define NBL_LOGD(fmt, ...)
#define NBL_LOGV(fmt, ...)
#endif

extern void trusty_printf(const char* format, ...);

#define NBL_LOG_ASSERT(cond)                                                        \
    do {                                                                            \
        if (!(cond)) {                                                              \
            trusty_printf("ASSERT:%s: %s:%d: '%s'\n", LOG_TAG, __func__, __LINE__, #cond); \
            abort();                                                                \
        }                                                                           \
    } while (0)

#define NBL_LOG_ASSERT_MSG(cond, fmt, ...)                                             \
    do {                                                                               \
        if (!(cond)) {                                                                 \
            trusty_printf("ASSERT:%s: %s:%d: '%s': " fmt, LOG_TAG, __func__, __LINE__, #cond, \
                   ##__VA_ARGS__);                                                     \
            abort();                                                                   \
        }                                                                              \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif // __ANDROID_API__

#endif /* NBL_LOG_H_ */
