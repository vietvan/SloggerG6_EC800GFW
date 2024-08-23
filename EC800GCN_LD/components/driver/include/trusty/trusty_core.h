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

#ifndef TRUSTY_CORE_H_
#define TRUSTY_CORE_H_

struct trusty_state {
    char *version_str;
    struct trusty_dev *tdev;
};

extern struct trusty_state *ts;
extern osiMutex_t *smc_lock;

const char *trusty_version_str_get(struct trusty_state *s);
int trusty_core_init(void);

#endif /* TRUSTY_CORE_H_ */
