
/*================================================================
  Copyright (c) 2021, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/
    
/**
 ******************************************************************************
 * @file    quec_gnss_handle.h
 * @author  lambert.zhao
 * @version V1.0.0
 * @date    2020/11/25
 * @brief   This file contains the quec_gnss_handle functions's declaration
 ******************************************************************************
 */

#ifndef QUEC_GNSS_HANDLE_H_
#define QUEC_GNSS_HANDLE_H_
#ifdef __cplusplus 
extern "C" {
#endif

#include "quec_proj_config.h"
#include "ql_api_osi.h"
#include "ql_gnss.h"
#include "at_engine.h"


/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define QUEC_GNSS_LOCK_TIMEOUT     10000   //10sec
#define QUEC_GNSS_PAM_API_PASS     1       //pam.h api函数返回值,执行成功则返回 1

/*===========================================================================
 * Typedef
 ===========================================================================*/



 /*===========================================================================
 * Enum
 ===========================================================================*/
typedef enum
{
    QUEC_GNSS_PAM_PARAM_HOT = 0,     //热启动     对应 unisoc api参数
    QUEC_GNSS_PAM_PARAM_COLD,        //冷启动
    QUEC_GNSS_PAM_PARAM_WARM,        //温启动
    QUEC_GNSS_PAM_PARAM_FACTORY,     //工厂模式,不依赖任何辅助定位数据
    QUEC_GNSS_PAM_PARAM_MAX
}quec_gnss_pam_startmode_e;

typedef enum
{
    QUEC_AGPS_STATE_START_IND = 0,      //开始AGPS(开始注册网络)
    QUEC_AGPS_STATE_REG_NW_IND,         //已经注网通知
    QUEC_AGPS_STATE_DATACALL_IND,       //已经datacall拨号通知
    QUEC_AGPS_STATE_MAX
}quec_gnss_agps_state_e;

/*===========================================================================
 * data struct
===========================================================================*/
typedef struct
{
    bool bFirstOpenGnss;
    bool bNeedUpdatefirm;
    bool bHslRetryProcess;
    bool bGnssResume;
    bool bGnsslocTimer;
    uint8_t simid;
    uint8_t start_mode;
    uint8_t satellite_type;
    uint8_t coexist_lte_gnss;
    uint16_t gnss_sleep;
    uint16_t lte_sleep;
    uint16_t output_format;
    uint32_t time_interval;
    uint32_t fixcount;
}quec_gnss_loc_t;

typedef struct
{
    ql_GnssState gnss_state;
    quec_gnss_loc_t loc;
    ql_gnss_coexist_callback gnss_cb;
    ql_mutex_t lock;
}quec_gnss_op_info_t;

typedef struct
{
    uint8_t priority;       //lte gnss coexist priority        
}quec_coexist_cfg_t;

typedef struct              
{
    quec_gps_config confg;
    loc_calendar_time_s_type time;
    unsigned char start_type;
    nmeasrc_sentences_cfg nmea_handle_cfg;
    quec_gps_outport_config outport;
    bool gpsautoflag;
    quec_agps_cfg_t agps;
    quec_coexist_cfg_t coexist_cfg;
    char device_information[100];
    ql_mutex_t lock;
}quec_gps_handle;          //此结构体中成员可以任意添加存储

#ifndef CONFIG_QL_OPEN_EXPORT_PKG
typedef struct
{
    bool is_at;                     //at回复flag
    ql_dispatch_t	*engine;        //at回复的dispatch
    ql_mutex_t lock;
}quec_gnss_at_resp_t;
#endif

/*===========================================================================
 * Functions declaration
 ===========================================================================*/
extern quec_gnss_op_info_t gnss_operation_param;
extern quec_gps_handle quec_gps_config_param;
int quec_gnss_param_lock(ql_mutex_t mutexRef);
int quec_gnss_param_unlock(ql_mutex_t mutexRef);
void quec_gnss_notify_cb(const ql_event_t *event);
int gpsautocheck(void);
int quec_gnss_get_version_info(void);
void quec_gnss_power_onff(bool status);
int quec_gnss_restart_req(ql_gnss_startmode_e start_mode);
void quec_gnss_pam_cfw_hsl_abort_ind(const ql_event_t *event);                  //LTE协议栈通知停止gnss功能
void quec_gnss_pam_cfw_hsl_recovery_ind(const ql_event_t *event);               //LTE协议栈通知恢复gnss功能
int quec_gps_handle_set(void);
int quec_gps_handle_get(void);
int ql_gnss_location_info_get(ql_gnss_output_info_ind_t * param);
struct nmea_s* nmea_parse(char *sentence, int length, int check_checksum);
int nmea_value_update(struct nmea_s *nmea, ql_gnss_data_t *gps_data);
int ql_get_gnss_info(ql_gnss_data_t *data);
char *strptime(const char *buf, const char *fmt, struct tm *tm);
#ifdef CONFIG_QUEC_PROJECT_FEATURE_GNSS_AT
void nmea_write_pqgsv_addsystemid(unsigned char * nmea_sentence,int len);
int quec_gnss_set_at_sync_resp_param(void *ctx);
void quec_gnss_at_resp_timeout_cb(ql_at_cmd_t *cmd); 
#endif

#ifdef __cplusplus 
}
#endif
#endif /* !QUEC_GNSS_HANDLE_H_ */

