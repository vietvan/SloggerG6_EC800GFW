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

#ifndef TRUSTY_LIST_H_
#define TRUSTY_LIST_H_

#include "trusty/sysdeps.h"
#include "cmsis_core.h"

#define POISON_POINTER_DELTA      0
#define LIST_POISON1              ((void *)0x100 + POISON_POINTER_DELTA)
#define LIST_POISON2              ((void *)0x200 + POISON_POINTER_DELTA)

#define __force

struct hlist_node {
    struct hlist_node *next, **pprev;
};

struct hlist_head {
    struct hlist_node *first;
};

#undef offsetof
#define offsetof(TYPE, MEMBER)      ((size_t) &((TYPE *)0)->MEMBER)
/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:        the pointer to the member.
 * @type:       the type of the container struct this is embedded in.
 * @member:     the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) ({               \
    const typeof( ((type *)0)->member ) *__mptr = (ptr); \
    (type *)( (char *)__mptr - offsetof(type,member) );})

#define hlist_entry(ptr, type, member)      container_of(ptr,type,member)
#define hlist_entry_safe(ptr, type, member)              \
    ({ typeof(ptr) ____ptr = (ptr);                      \
    ____ptr ? hlist_entry(____ptr, type, member) : NULL; \
    })

static __always_inline void __write_once_size(volatile void *p, void *res, int size)
{
    switch (size) {
        case 1: *(volatile uint8_t *)p = *(uint8_t *)res; break;
        case 2: *(volatile uint16_t *)p = *(uint16_t *)res; break;
        case 4: *(volatile uint32_t *)p = *(uint32_t *)res; break;
        case 8: *(volatile uint64_t *)p = *(uint64_t *)res; break;
        default:
	    __DSB();
            __builtin_memcpy((void *)p, (const void *)res, size);
	    __DSB();
    }
}

#define WRITE_ONCE(x, val)                         \
({                                                 \
    union { typeof(x) __val; char __c[1]; } __u =  \
    { .__val = (__force typeof(x)) (val) };        \
    __write_once_size(&(x), __u.__c, sizeof(x));   \
    __u.__val;                                     \
})

static inline void __hlist_del(struct hlist_node *n)
{
    struct hlist_node *next = n->next;
    struct hlist_node **pprev = n->pprev;

    WRITE_ONCE(*pprev, next);
    if (next)
        next->pprev = pprev;
}

static inline void hlist_del(struct hlist_node *n)
{
    __hlist_del(n);
    n->next = LIST_POISON1;
    n->pprev = LIST_POISON2;
}

static inline void hlist_add_head(struct hlist_node *n, struct hlist_head *h)
{
    struct hlist_node *first = h->first;

    n->next = first;
    if (first)
        first->pprev = &n->next;

    WRITE_ONCE(h->first, n);
    n->pprev = &h->first;
}

/**
 * hlist_for_each_entry	- iterate over list of given type
 * @pos:	the type * to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the hlist_node within the struct.
 */
#define hlist_for_each_entry(pos, head, member)                         \
    for (pos = hlist_entry_safe((head)->first, typeof(*(pos)), member); \
    pos;							        \
    pos = hlist_entry_safe((pos)->member.next, typeof(*(pos)), member))

/**
 * hlist_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:	the type * to use as a loop cursor.
 * @n:		another &struct hlist_node to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the hlist_node within the struct.
 */
#define hlist_for_each_entry_safe(pos, n, head, member)               \
    for (pos = hlist_entry_safe((head)->first, typeof(*pos), member); \
    pos && ({ n = pos->member.next; 1; });			      \
    pos = hlist_entry_safe(n, typeof(*pos), member))

#endif /* TRUSTY_LIST_H_ */
