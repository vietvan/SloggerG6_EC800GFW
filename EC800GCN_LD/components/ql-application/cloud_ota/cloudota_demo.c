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



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "ql_api_osi.h"
#include "ql_api_nw.h"

#include "ql_log.h"
#include "ql_api_datacall.h"
#include "ql_cloud_ota.h"
#include "ql_power.h"
#define QL_CTSREG_LOG_LEVEL	                QL_LOG_LEVEL_INFO
#define cloudota_printf(msg, ...)			QL_LOG(QL_CTSREG_LOG_LEVEL, "ql_cloudota_DEMO", msg, ##__VA_ARGS__)
#define cloudota_printf_psuh(msg, ...)	    QL_LOG_PUSH("ql_cloudota_DEMO", msg, ##__VA_ARGS__)

static ql_task_t cloudota_task = NULL;

static void cloudota_cb(qlHttpFotaStart start, int progress)
{
	cloudota_printf("\r\n[cloudota_cb]start(%d) progress:%d \r\n", start, progress);
	
    switch(start)
    {
        case QL_HTTPFOTA_PUB_DOWN_FIRMWARE_SUCCESS:
            cloudota_printf("upgrade success\r\n");
            break;
        case QL_HTTPFOTA_PUB_DOWN_FIRMWARE_UPGRADE:
            cloudota_printf("reboot to upgrade\r\n");
            ql_power_reset(RESET_NORMAL);
            break;
       case QL_HTTPFOTA_PUB_GET_VERSION_FAIL:
            cloudota_printf("Get targetversion fail\r\n");
            break;
       default:
       		cloudota_printf("start=%d",start);
       		break;
    }       
}

static void cloudota_app_thread(void * arg)
{
	int ret = 0;
	int i = 0;
	int profile_idx = 1;
    ql_data_call_info_s info;
	char ip4_addr_str[16] = {0};
	uint8_t sim_id = 0;
	bool have_version = FALSE;
	char target_version[64] = {0};

	ql_rtos_task_sleep_s(10);
	cloudota_printf("==========cloudota demo start ==========");
	cloudota_printf("wait for network register done");

	while((ret = ql_network_register_wait(sim_id, 120)) != 0 && i < 10){
    	i++;
		ql_rtos_task_sleep_s(1);
	}
	if(ret == 0){
		i = 0;
		cloudota_printf("====network registered!!!!====");
	}else{
		cloudota_printf("====network register failure!!!!!====");
		goto exit;
	}

	ql_set_data_call_asyn_mode(sim_id, profile_idx, 0);

	cloudota_printf("===start data call====");
	ret=ql_start_data_call(sim_id, profile_idx, QL_PDP_TYPE_IP, "uninet", NULL, NULL, 0); 
	cloudota_printf("===data call result:%d", ret);
	if(ret != 0){
		cloudota_printf("====data call failure!!!!=====");
	}
	memset(&info, 0x00, sizeof(ql_data_call_info_s));
	
	ret = ql_get_data_call_info(sim_id, profile_idx, &info);
	if(ret != 0){
		cloudota_printf("ql_get_data_call_info ret: %d", ret);
		ql_stop_data_call(sim_id, profile_idx);
		goto exit;
	}
    cloudota_printf("info->profile_idx: %d", info.profile_idx);
	cloudota_printf("info->ip_version: %d", info.ip_version);
            
	cloudota_printf("info->v4.state: %d", info.v4.state); 
	inet_ntop(AF_INET, &info.v4.addr.ip, ip4_addr_str, sizeof(ip4_addr_str));
	cloudota_printf("info.v4.addr.ip: %s\r\n", ip4_addr_str);

	inet_ntop(AF_INET, &info.v4.addr.pri_dns, ip4_addr_str, sizeof(ip4_addr_str));
	cloudota_printf("info.v4.addr.pri_dns: %s\r\n", ip4_addr_str);

	inet_ntop(AF_INET, &info.v4.addr.sec_dns, ip4_addr_str, sizeof(ip4_addr_str));
	cloudota_printf("info.v4.addr.sec_dns: %s\r\n", ip4_addr_str);
	
	ql_cloudota_init();
	cloudota_printf("start check version\r\n");
	have_version = ql_check_cloudota_info(profile_idx,  target_version, 64, cloudota_cb);
	if(have_version == FALSE)
	{
		cloudota_printf("\r\nNot have new version\r\n");
		goto exit;
	}
	
	have_version = ql_cloudota_download(profile_idx, cloudota_cb);
	if(have_version != 0)
	{
		cloudota_printf("\r\nql_cloudota_download error\r\n");
		goto exit;
	}
exit:
   ql_rtos_task_delete(cloudota_task);	
  
   return;	
}


int ql_cloudota_app_init(void)
{
	QlOSStatus err = QL_OSI_SUCCESS;
	
    err = ql_rtos_task_create(&cloudota_task, 5*1024, APP_PRIORITY_ABOVE_NORMAL, "cloudota_app", cloudota_app_thread, NULL, 5);
	if(err != QL_OSI_SUCCESS)
    {
		cloudota_printf("ctsreg_app init failed");
	}

	return err;
}

