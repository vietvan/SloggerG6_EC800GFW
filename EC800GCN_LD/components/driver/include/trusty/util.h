/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef TRUSTY_UTIL_H_
#define TRUSTY_UTIL_H_

#include "trusty/sysdeps.h"

/* Returns the basename of |str|. This is defined as the last path
 * component, assuming the normal POSIX separator '/'. If there are no
 * separators, returns |str|.
 */
const char* trusty_basename(const char* str);

bool is_power_of_2(unsigned long n);

#define MIN(x,y)      ((x) < (y) ? (x) : (y))

#define TRUSTY_STRINGIFY(x)      #x
#define TRUSTY_TO_STRING(x)      TRUSTY_STRINGIFY(x)

#define TRUSTY_STR         "trusty driver:"
#define TRUSTY_OS_STR      "trusty os:"
#define TRUSTY_TRCID       0

#define TIPC_ENABLE_DEBUG

/*
 * Aborts the program if @expr is false.
 *
 * This has no effect unless TIPC_ENABLE_DEBUG is defined.
 */
#ifdef TIPC_ENABLE_DEBUG
#define trusty_assert(expr)                                                        \
    do {                                                                           \
        if (!(expr)) {                                                             \
            trusty_fatal(OSI_LOGPAR_S, "%s assert fail: " #expr "\n", TRUSTY_STR); \
        }                                                                          \
    } while (0)
#else
#define trusty_assert(expr)
#endif

/*
 * Prints message and calls trusty_abort.
 */
#define trusty_fatal(partype, fmt, ...)                           \
    do {                                                          \
            OSI_LOGXE(OSI_LOGPAR_SSS, TRUSTY_TRCID, "%s %s%s",    \
		      TRUSTY_STR,                                 \
	              trusty_basename(__FILE__),                  \
		      ":"TRUSTY_TO_STRING(__LINE__) " FATAL ");   \
            OSI_LOGXE(partype, TRUSTY_TRCID, fmt, ##__VA_ARGS__); \
            trusty_abort();                                       \
    } while (0)

/*
 * Prints error message.
 */
#define trusty_error(partype, fmt, ...)                           \
    do {                                                          \
            OSI_LOGXE(OSI_LOGPAR_SSS, TRUSTY_TRCID, "%s %s%s",    \
		      TRUSTY_STR,                                 \
	              trusty_basename(__FILE__),                  \
		      ":"TRUSTY_TO_STRING(__LINE__) " ERROR ");   \
            OSI_LOGXE(partype, TRUSTY_TRCID, fmt, ##__VA_ARGS__); \
    } while (0)

/*
 * Prints info message.
 */
#define trusty_info(partype, fmt, ...)                            \
    do {                                                          \
            OSI_LOGXI(OSI_LOGPAR_SSS, TRUSTY_TRCID, "%s %s%s",    \
		      TRUSTY_STR,                                 \
	              trusty_basename(__FILE__),                  \
		      ":"TRUSTY_TO_STRING(__LINE__) " INFO ");    \
            OSI_LOGXI(partype, TRUSTY_TRCID, fmt, ##__VA_ARGS__); \
    } while (0)
#define trusty_os_info(partype, fmt, ...)                         \
    do {                                                          \
            OSI_LOGXI(partype, TRUSTY_TRCID, fmt, ##__VA_ARGS__); \
    } while (0)

/*
 * Prints debug message.
 *
 * This has no effect unless TIPC_ENABLE_DEBUG and OSI_LOG_LEVEL_DEBUG is defined.
 */
#ifdef TIPC_ENABLE_DEBUG
#define trusty_debug(partype, fmt, ...)                           \
    do {                                                          \
            OSI_LOGXD(OSI_LOGPAR_SSS, TRUSTY_TRCID, "%s %s%s",    \
		      TRUSTY_STR,                                 \
	              trusty_basename(__FILE__),                  \
		      ":"TRUSTY_TO_STRING(__LINE__) " DEBUG ");   \
            OSI_LOGXD(partype, TRUSTY_TRCID, fmt, ##__VA_ARGS__); \
    } while (0)
#else
#define trusty_debug(partype, fmt, ...)
#endif

#endif /* TRUSTY_UTIL_H_ */
