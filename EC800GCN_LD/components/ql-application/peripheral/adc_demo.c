﻿
/*================================================================
  Copyright (c) 2021, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/
    
/*=================================================================

						EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN			  WHO		  WHAT, WHERE, WHY
------------	 -------	 -------------------------------------------------------------------------------

=================================================================*/


/*===========================================================================
 * include files
 ===========================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "osi_api.h"
#include "ql_log.h"
#include "ql_adc.h"
#include "adc_demo.h"
#include "ql_api_osi.h"

/*===========================================================================
 *Definition
 ===========================================================================*/
#define QL_ADCDEMO_LOG_LEVEL			QL_LOG_LEVEL_INFO
#define QL_ADCDEMO_LOG(msg, ...)		QL_LOG(QL_ADCDEMO_LOG_LEVEL, "ql_ADC", msg, ##__VA_ARGS__)
#define QL_ADCDEMO_LOG_PUSH(msg, ...)	QL_LOG_PUSH("ql_ADCDEMO", msg, ##__VA_ARGS__)

#define QL_ADC_TASK_STACK_SIZE     		1024
#define QL_ADC_TASK_PRIO          	 	APP_PRIORITY_NORMAL
#define QL_ADC_TASK_EVENT_CNT      		5

/*===========================================================================
 * Variate
 ===========================================================================*/

/*===========================================================================
 * Functions
 ===========================================================================*/
ql_errcode_adc_e ql_adc_demo_test(  void)
{
	int adc_value = 0;

	ql_adc_get_volt(QL_ADC0_CHANNEL, &adc_value);
	QL_ADCDEMO_LOG("ADC[0]: adc_value=[%d]", adc_value );

	ql_adc_get_volt(QL_ADC1_CHANNEL, &adc_value);
	QL_ADCDEMO_LOG("ADC[1]: adc_value=[%d]", adc_value );

#ifndef QL_APP_PROJECT_DEF_EC800G
#ifndef QL_APP_PROJECT_DEF_EG800G
	ql_adc_get_volt(QL_ADC2_CHANNEL, &adc_value);
	QL_ADCDEMO_LOG("ADC[2]: adc_value=[%d]", adc_value );
	
	ql_adc_get_volt(QL_ADC3_CHANNEL, &adc_value);
	QL_ADCDEMO_LOG("ADC[3]: adc_value=[%d]", adc_value );
#endif
#endif

    //obtain raw data in order to improve the speed, customers need to optimize the accuracy themselves
    ql_adc_get_volt_raw(QL_ADC0_CHANNEL, QL_ADC_SCALE_0, &adc_value);
    QL_ADCDEMO_LOG("RAW ADC[0]: adc_value=[%d]", adc_value );

    ql_adc_get_volt_raw(QL_ADC1_CHANNEL, QL_ADC_SCALE_0, &adc_value);
    QL_ADCDEMO_LOG("RAW ADC[1]: adc_value=[%d]", adc_value );

#ifndef QL_APP_PROJECT_DEF_EC800G
#ifndef QL_APP_PROJECT_DEF_EG800G
    ql_adc_get_volt_raw(QL_ADC2_CHANNEL, QL_ADC_SCALE_0, &adc_value);
    QL_ADCDEMO_LOG("RAW ADC[2]: adc_value=[%d]", adc_value );
	
    ql_adc_get_volt_raw(QL_ADC3_CHANNEL, QL_ADC_SCALE_0, &adc_value);
    QL_ADCDEMO_LOG("RAW ADC[3]: adc_value=[%d]", adc_value );
#endif
#endif

	return 0;
}

static void ql_adc_demo_thread(void *param)
{
    ql_rtos_task_sleep_s(1);

    /*Show test results*/
    ql_adc_demo_test();
    ql_rtos_task_delete(NULL);
}

void ql_adc_app_init(void)
{
	QlADCStatus err = QL_ADC_SUCCESS;
	ql_task_t adc_task = NULL;
	
	err = ql_rtos_task_create(&adc_task, QL_ADC_TASK_STACK_SIZE, QL_ADC_TASK_PRIO, "adc DEMO", ql_adc_demo_thread, NULL, QL_ADC_TASK_EVENT_CNT);
	if (err != QL_ADC_SUCCESS)
	{
		QL_ADCDEMO_LOG("adc demo task created failed");
	}
}


