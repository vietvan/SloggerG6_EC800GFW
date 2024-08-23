﻿/**  @file
  bt_hfp_demo.h

  @brief
  This file is used to define bt hfp demo for different Quectel Project.

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


#ifndef BT_HFP_DEMO_H
#define BT_HFP_DEMO_H


#ifdef __cplusplus
extern "C" {
#endif


/*========================================================================
 *  Variable Definition
 *========================================================================*/
#define BT_BLE_DEMO_TASK_PRIO           12
#define BT_BLE_DEMO_TASK_STACK_SIZE     8*1024
#define BT_BLE_DEMO_TASK_EVENT_CNT      16


/*========================================================================
 *  function Definition
 *========================================================================*/
QlOSStatus ql_bt_hfp_demo_init(void);

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* BT_HFP_DEMO_H */


