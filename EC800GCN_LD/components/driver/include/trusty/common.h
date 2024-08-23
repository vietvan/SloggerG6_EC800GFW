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

#ifndef TRUSTY_COMMON_H_
#define TRUSTY_COMMON_H_

int32_t trusty_fast_call32(struct trusty_dev* dev,
                                  uint32_t smcnr,
                                  uint32_t a0,
                                  uint32_t a1,
                                  uint32_t a2);

int32_t trusty_std_call32(struct trusty_dev* dev,
                                 uint32_t smcnr,
                                 uint32_t a0,
                                 uint32_t a1,
                                 uint32_t a2);

int trusty_init_api_version(struct trusty_dev* dev);

int trusty_dev_nop(struct trusty_dev* dev);

#endif /* TRUSTY_COMMON_H_ */
