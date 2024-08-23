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

#ifndef TRUSTY_SYSDEPS_H_
#define TRUSTY_SYSDEPS_H_

/*
 * Change these includes to match your platform to bring in the equivalent
 * types available in a normal C runtime. At least things like uint64_t,
 * uintptr_t, and bool (with |false|, |true| keywords) must be present.
 */
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "osi_api.h"

/*
 * These attribute macros may need to be adjusted if not using gcc or clang.
 */
#define TRUSTY_ATTR_PACKED                  __attribute__((packed))
#define TRUSTY_ATTR_NO_RETURN               __attribute__((noreturn))
#define TRUSTY_ATTR_SENTINEL                __attribute__((__sentinel__))
#define TRUSTY_ATTR_WARN_UNUSED_RESULT      __attribute__((warn_unused_result))

#define PAGE_SHIFT      12
#define PAGE_SIZE       (1 << PAGE_SHIFT)

/*
 * Struct containing attributes for memory to be shared with secure size.
 */
struct ns_mem_page_info {
    uint64_t attr;
};

struct trusty_dev;

/*
 * Lock/unlock mutex. These can be safely empty in a single threaded environment.
 *
 */
void trusty_lock(osiMutex_t *lock);
void trusty_unlock(osiMutex_t *lock);
/*
 * Disable/enable IRQ interrupts and save/restore @state
 */
void trusty_local_irq_disable(unsigned long* state);
void trusty_local_irq_restore(unsigned long* state);
/*
 * Put in standby state waiting for interrupt.
 *
 * @dev: Trusty device initialized with trusty_dev_init
 * @event_poll: If true this function is entered after trusty_ipc_poll_for_event
 *              returned TRUSTY_EVENT_NONE. In this case this call needs to
 *              return without waiting for another interrupt if a secondary CPU
 *              (any CPU other than the CPU that is waiting for a message) has
 *              entered idle since the last such call. If there is only one CPU
 *              calling into trusty this argument can be ignored.
 */
void trusty_idle(struct trusty_dev* dev, bool event_poll);
/*
 * Aborts the program or reboots the device.
 */
void trusty_abort(void);
/*
 * Print a formatted string. @format must point to a NULL-terminated string, and
 * is followed by arguments to be printed.
 */
void trusty_printf(const char* format, ...);
/*
 * Copy @n bytes from @src to @dest.
 */
void* trusty_memcpy(void* dest, const void* src, size_t n);
/*
 * Set @n bytes starting at @dest to @c. Returns @dest.
 */
void* trusty_memset(void* dest, const int c, size_t n);
/*
 * Compare s1 to s2 with @n bytes. Returns 0 if s1 equal to s2, otherwise non-zero.
 */
size_t trusty_memcmp(void* s1, void* s2, size_t n);
/*
 * Copy string from @src to @dest, including the terminating NULL byte.
 *
 * The size of the array at @dest should be long enough to contain the string
 * at @src, and should not overlap in memory with @src.
 */
char* trusty_strcpy(char* dest, const char* src);
/*
 * Returns the length of @str, excluding the terminating NULL byte.
 */
size_t trusty_strlen(const char* str);
/*
 * Compare two strings, returns zero if strings are identical, else returns
 * integer less than or greater than zero respectively if @str1 if less than
 * or greater than @str2.
 */
int trusty_strcmp(const char* str1, const char* str2);
/*
 * Allocate @n elements of size @size. Initializes memory to 0, returns pointer
 * to it.
 */
void* trusty_calloc(size_t n, size_t size) TRUSTY_ATTR_WARN_UNUSED_RESULT;
/*
 * Free memory at @addr allocated with trusty_calloc.
 */
void trusty_free(void* addr);
/*
 * Allocate @count contiguous pages to be shared with secure side.
 *
 * Returns:   vaddr of allocated memory
 */
void* trusty_alloc_pages(unsigned count) TRUSTY_ATTR_WARN_UNUSED_RESULT;
/*
 * Free @count pages at @vaddr allocated by trusty_alloc_pages
 */
void trusty_free_pages(void* vaddr, unsigned count);

#endif /* TRUSTY_SYSDEPS_H_ */
