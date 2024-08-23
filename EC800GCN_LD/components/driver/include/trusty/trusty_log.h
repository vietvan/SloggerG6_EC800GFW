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
#ifndef TRUSTY_LOG_H_
#define TRUSTY_LOG_H_

struct log_rb {
    volatile uint32_t alloc;
    volatile uint32_t put;
    uint32_t sz;
    volatile char data[0];
} __packed;

enum {
    TRUSTY_CALL_PREPARE,
    TRUSTY_CALL_RETURNED,
    TRUSTY_CALL_PANIC
};

#define SMC_SC_SHARED_LOG_VERSION      SMC_STDCALL_NR(SMC_ENTITY_LOGGING, 0)
#define SMC_SC_SHARED_LOG_ADD          SMC_STDCALL_NR(SMC_ENTITY_LOGGING, 1)
#define SMC_SC_SHARED_LOG_RM           SMC_STDCALL_NR(SMC_ENTITY_LOGGING, 2)

#define TRUSTY_LOG_API_VERSION      1

void print_trusty_memlog(void);
int trusty_log_init(void);

#endif /* TRUSTY_LOG_H_ */
