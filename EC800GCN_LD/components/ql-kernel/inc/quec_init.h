/**  @file
  quec_init.h

  @brief
  This file is used to define version information for different Quectel Project.

*/

/*================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
/*=================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN              WHO         WHAT, WHERE, WHY
------------     -------     -------------------------------------------------------------------------------

=================================================================*/


#ifndef QUEC_INIT_H
#define QUEC_INIT_H

#include "quec_proj_config.h"

#ifdef __cplusplus
extern "C" {
#endif


void quec_startup(uint32 stacksize);
void quec_init_thread(void *arg);

void quec_net_app_init(void);
void quec_net_app_startup(void);

void quec_cfw_poweron(void);

void quec_init_cfw_wait(uint32_t timeout);
void quec_init_cfw_release(void);

void quec_fs_dir_check(void);

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QUEC_INIT_H */


