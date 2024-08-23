/*================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ql_api_osi.h"
#include "ql_log.h"
#include "usb_demo.h"

#define QL_USB_LOG_LEVEL	            QL_LOG_LEVEL_INFO
#define QL_USB_LOG(msg, ...)			QL_LOG(QL_USB_LOG_LEVEL, "ql_usb", msg, ##__VA_ARGS__)
#define QL_USB_LOG_PUSH(msg, ...)		QL_LOG_PUSH("ql_usb", msg, ##__VA_ARGS__)

#define QL_USB_MASS_STORAGE	0 //验证mass storage功能打开此宏

static void ql_usb_demo_thread(void *param)
{
	QL_USB_LOG("enter usb demo");

#if QL_USB_MASS_STORAGE
	ql_usb_msc_cfg_t msc_cfg = {0};
/*
	配置映射模块的内置flash/sd卡/6线flash等存储器到PC端, 模块作为虚拟U盘
	注意：
		  1. 在target.config中,打开CONFIG_QUEC_PROJECT_FEATURE_USB_MASS_STORAGE宏
		  2. 映射SD卡需要开机时挂载SD卡,使用demo验证可以打开sd卡的demo(ql_sdmmc_app_init)去挂载
		  3. 映射外置6线flash需要开机时挂载6线flash文件系统,使用demo验证可以打开6线flash的demo(ql_spi6_ext_nor_flash_demo_init)去挂载
		  4. 烧录代码后第一次开机进app可能会比较慢,如果sd卡/外置flash在app挂载,则烧录后第一次开机可能虚拟不出sd卡/外置flash,后续开机不会无法映射
		  5. 如存储器未被正常映射,可在log中搜索 "mtp_scan result", 该log为usb扫描存储设备结束,看下是不是存储设备的挂载
		     动作在扫描结束之后; 如果是可以试下将挂载sd卡/6线flash的动作提前
		  6. 最多支持同时映射2个存储器
*/
	msc_cfg.msc_device = QL_USB_MSC_SDCARD; //如需映射2个存储器,可用"|"连接,如同时映射外置6线flash和sd卡: QL_USB_MSC_EFS | QL_USB_MSC_SDCARD
	ql_usb_set_enum_mode(QL_USB_ENUM_MASS_STORAGE);	//配置USB枚举模式为usb存储设备,同时支持usbat,usbmodem,usb aplog等端口,重启生效
	ql_usb_msc_config_set(&msc_cfg); //配置usb mass storage功能,重启生效
#endif

/*
	使用默认的端口组合,配置枚举哪些USB端口,也可以直接在quec_usb_serial_create中配置,该函数开源在app_start.c中
	ql_usb_set_enum_mode(QL_USB_ENUM_USBNET_COM);
*/
	ql_rtos_task_delete(NULL);
}

void ql_usb_app_init(void)
{
	QlOSStatus err = QL_OSI_SUCCESS;
	ql_task_t ql_usb_task = NULL;
	
    QL_USB_LOG("usb demo enter");
	
    err = ql_rtos_task_create(&ql_usb_task, QL_USB_TASK_STACK, APP_PRIORITY_NORMAL, "ql_usb_demo", ql_usb_demo_thread, NULL, 5);
	if(err != QL_OSI_SUCCESS)
    {
		QL_USB_LOG("usb task create failed");
	}
}


