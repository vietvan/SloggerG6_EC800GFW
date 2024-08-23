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

#include "ql_api_osi.h"
#include "ql_api_bt.h"
#include "ql_api_bt_spp.h"

#include "ql_log.h"
#include "bt_spp_demo.h"

#define QL_BT_SPP_LOG_LEVEL         	QL_LOG_LEVEL_INFO
#define QL_BT_SPP_LOG(msg, ...)			QL_LOG(QL_BT_SPP_LOG_LEVEL, "ql_bt_spp_DEMO", msg, ##__VA_ARGS__)
#define QL_BT_SPP_LOG_PUSH(msg, ...)	QL_LOG_PUSH("ql_bt_spp_DEMO", msg, ##__VA_ARGS__)

#define QL_BT_DEMO_ADDR_MAX_SIZE                    17
ql_task_t bt_demo_task = NULL;

ql_bt_inquiry_type_e bt_inquiry_type = QL_BT_SERVICE_ALL;
ql_bt_addr_s bt_connection_addr = {{0x66, 0xCA, 0xC9, 0xA2, 0x3E, 0x38}};

extern ql_errcode_bt_e ql_bt_demo_start();
extern ql_errcode_bt_e ql_bt_demo_stop();
extern ql_errcode_bt_e ql_bt_demo_get_state();
extern ql_errcode_bt_e ql_bt_demo_get_local_name();
extern ql_errcode_bt_e ql_bt_demo_set_local_name();
extern ql_errcode_bt_e ql_bt_demo_get_scanmde();
extern ql_errcode_bt_e ql_be_demo_set_scanmode();

ql_errcode_bt_e ql_bt_demo_start_inquiry()
{
    ql_errcode_bt_e ret;
    
    ret = ql_classic_bt_start_inquiry(bt_inquiry_type);
    if (ret == QL_BT_SUCCESS)
    {
        QL_BT_SPP_LOG("sucess");
    }
    else 
    {
        QL_BT_SPP_LOG("error=%x", ret);
    }

    return ret;
}

ql_errcode_bt_e ql_bt_demo_cancel_inquiry()
{
    ql_errcode_bt_e ret;
    
    ret = ql_classic_bt_cancel_inquiry();
    if (ret == QL_BT_SUCCESS)
    {
        QL_BT_SPP_LOG("sucess");
    }
    else 
    {
        QL_BT_SPP_LOG("error=%x", ret);
    }

    return ret;
}

static ql_errcode_bt_e ql_bt_demo_connect()
{
    ql_errcode_bt_e ret;
    ql_bt_addr_s connect_info;

    connect_info.addr[0] = 0x66;
    connect_info.addr[1] = 0xCA;
    connect_info.addr[2] = 0xC9;
    connect_info.addr[3] = 0xA2;
    connect_info.addr[4] = 0x3E;
    connect_info.addr[5] = 0x38;
    
    ret = ql_bt_spp_connect(connect_info);
    if (ret == QL_BT_SUCCESS)
    {
        QL_BT_SPP_LOG("sucess");
    }
    else
    {
        QL_BT_SPP_LOG("error=%x", ret);
    }

    return ret;
}

static ql_errcode_bt_e ql_bt_demo_disconnect()
{
    ql_errcode_bt_e ret;
    
    ret = ql_bt_spp_disconnect();
    if (ret == QL_BT_SUCCESS)
    {
        QL_BT_SPP_LOG("sucess");
    }
    else
    {
        QL_BT_SPP_LOG("error=%x", ret);
    }

    return ret;
}


static ql_errcode_bt_e ql_bt_demo_spp_send_data()
{
    ql_errcode_bt_e ret;
    uint8 spp_info[10]="123456789";
    
    ret = ql_bt_spp_data_send(spp_info,9);
    if (ret == QL_BT_SUCCESS)
    {
        QL_BT_SPP_LOG("sucess");
    }
    else
    {
        QL_BT_SPP_LOG("error=%x", ret);
    }

    return ret;
}

void ql_bt_notify_cb(void *ind_msg_buf, void *ctx)
{
    ql_event_t *ql_event = NULL;
    
    if (ind_msg_buf == NULL)
    {
        return ;
        
    }

    ql_event = (ql_event_t *)ind_msg_buf;
    if (ql_event->id != 0)
    {
        ql_rtos_event_send(bt_demo_task,ql_event);
    }
}

static ql_errcode_bt_e ql_bt_spp_event()
{
    ql_event_t test_event = {0};
    ql_errcode_bt_e ret = QL_BT_SUCCESS;
    QL_BT_STATUS status;
    if (ql_event_try_wait(&test_event) == 0)
	{
        if (test_event.id == 0)
        {
            return ret;
        }
        status = (QL_BT_STATUS)(test_event.param1);
        switch (test_event.id)
        {
            case QUEC_BT_START_STATUS_IND:
            {
                if (QL_BT_STATUS_SUCCESS == status)
                {
                    QL_BT_SPP_LOG("start sucess");
                    ret = ql_bt_demo_get_state();
                    if (ret == QL_BT_SUCCESS)
                    {  
                        ql_be_demo_set_scanmode();

                        ret = ql_bt_demo_set_local_name();
                        if (ret != QL_BT_SUCCESS)
                        {
                            goto QL_BT_SPP_CLIENT_STOP;
                        }
                        else 
                        {
                            ql_bt_demo_start_inquiry();
                        }    
                    }
                    else 
                    {
                        goto QL_BT_SPP_CLIENT_STOP;
                    }    
                }
                else
                {
                    QL_BT_SPP_LOG("start failed");
                }
            }
            break;
            case QUEC_BT_STOP_STATUS_IND:
            {
                if (QL_BT_STATUS_SUCCESS == status)
                {
                    QL_BT_SPP_LOG("stop sucess");
                    
                }
                else
                {
                    QL_BT_SPP_LOG("stop failed");
                }
            }
            break;
            case QUEC_BT_INQUIRY_IND:
            {
                if (QL_BT_STATUS_SUCCESS == status)
                {
                    QL_BT_SPP_LOG("inqury sucess");
                    ql_bt_info_s *pdev_info = NULL;
                    unsigned char addr_string[QL_BT_DEMO_ADDR_MAX_SIZE + 1] = {0};
                    unsigned char str_ptr[256];

                    void *pData = (void *)(test_event.param2);
                    if (pData == NULL)
                    {
                        QL_BT_SPP_LOG("inqury pData is NULL");
                        break;
                    }
                    pdev_info = (ql_bt_info_s *)pData;
                    memset(addr_string,0x00,sizeof(addr_string));
                    sprintf((char *)addr_string, "%02x:%02x:%02x:%02x:%02x:%02x", pdev_info->addr.addr[0], pdev_info->addr.addr[1], pdev_info->addr.addr[2], pdev_info->addr.addr[3], pdev_info->addr.addr[4], pdev_info->addr.addr[5]);
                    memset(str_ptr,0x00,sizeof(str_ptr));
                    sprintf((char *)str_ptr, "%s%s,%s,%x", "INQ:", addr_string, (char *)pdev_info->name, pdev_info->dev_class);
                    QL_BT_SPP_LOG("inqury str_ptr=%s",str_ptr);  
                }
                else
                {
                    QL_BT_SPP_LOG("inqury failed");
                    goto QL_BT_SPP_CLIENT_STOP;
                }
            }
            break;
            case QUEC_BT_INQUIRY_END_IND:
            {
                if (QL_BT_STATUS_SUCCESS == status)
                {
                    QL_BT_SPP_LOG("inqury end sucess");
                    ql_bt_demo_cancel_inquiry();
                    ql_bt_demo_connect();
                }
                else
                {
                    QL_BT_SPP_LOG("inqury end failed");
                    goto QL_BT_SPP_CLIENT_STOP; 
                }
            }
            break;
            case QUEC_BT_SPP_CONNECT_IND:
            {
                if (QL_BT_STATUS_SUCCESS == status)
                {
                    ret = ql_bt_demo_spp_send_data();
                    if (ret != QL_BT_SUCCESS)
                    {
                        goto QL_BT_SPP_CLIENT_STOP;
                    }
                }
                else
                {
                    QL_BT_SPP_LOG("connect failed");
                    goto QL_BT_SPP_CLIENT_STOP; 
                }
            }
            break;
            case QUEC_BT_SPP_REVDATA_IND:
            {
                if (QL_BT_STATUS_SUCCESS == status)
                {
                    QL_BT_SPP_LOG("spp recv data sucess");
                    void *pData = (void *)(test_event.param2);
                    if (pData == NULL)
                    {
                        QL_BT_SPP_LOG("inqury pData is NULL");
                        break;
                    }
                    
                    ret = ql_bt_demo_spp_send_data();
                    if (ret != QL_BT_SUCCESS)
                    {
                        goto QL_BT_SPP_CLIENT_STOP;
                    }
                    ql_bt_demo_disconnect();
                }
                else
                {
                    QL_BT_SPP_LOG("spp send data failed");
                    goto QL_BT_SPP_CLIENT_STOP;
                }
            }
            break;
            case QUEC_BT_SPP_DISCONNECT_IND:
            {
                if (QL_BT_STATUS_SUCCESS == status)
                {
                    QL_BT_SPP_LOG("disconnect sucess");
                    goto QL_BT_SPP_CLIENT_STOP;   
                }
                else
                {
                    QL_BT_SPP_LOG("disconnect failed");
                    goto QL_BT_SPP_CLIENT_STOP;
                }
            }
            break;
            default:
            break;
        }
        return ret;
QL_BT_SPP_CLIENT_STOP:
        ql_bt_demo_stop();
	}
    return ret;
}

void ql_bt_spp_demo_task_pthread(void *ctx)
{
	QlOSStatus err = 0;
    ql_errcode_bt_e ret = QL_BT_SUCCESS;
    
    ret = ql_bt_spp_init(ql_bt_notify_cb);
    if (ret != QL_BT_SUCCESS)
    {
        goto QL_BT_SPP_SERVER_NOT_INIT_EIXT;
    }

    ret = ql_bt_demo_start();
    if (ret != QL_BT_SUCCESS)
    {
        goto QL_BT_SPP_SERVER_INIT_EIXT;
    }
    
	while(1)
	{
        ql_bt_spp_event(); 
        //if (ret != QL_BT_SUCCESS)
        //{
        //    break;
        //}
	}
QL_BT_SPP_SERVER_INIT_EIXT:  
	ql_bt_spp_release();
QL_BT_SPP_SERVER_NOT_INIT_EIXT:
	err = ql_rtos_task_delete(NULL);
	if(err != QL_OSI_SUCCESS)
	{
		QL_BT_SPP_LOG("task deleted failed");
	}
}



QlOSStatus ql_bt_spp_demo_init(void)
{	
	QlOSStatus err = QL_OSI_SUCCESS;

    QL_BT_SPP_LOG("enter ql_bt_spp_demo_init");
	err = ql_rtos_task_create(&bt_demo_task, BT_BLE_DEMO_TASK_STACK_SIZE, BT_BLE_DEMO_TASK_PRIO, "ql_spp_demo", ql_bt_spp_demo_task_pthread, NULL, BT_BLE_DEMO_TASK_EVENT_CNT);
	
    return err;
}


