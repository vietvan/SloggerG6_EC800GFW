/*================================================================
Build all
./build_all new EC800GCN_LD SLoggerS6V001

build app only
./build_all r EC800GCN_LD SLoggerS6V001

xoa du an
./build_all clean

//programmer app
..\UpgradeDownload_R25.20.3901\Bin\CmdDloader.exe -pac target\EC800GCN_LD_SLOGGERS6V001\app\SLOGGERS6V001_APP.pac -port 18
//full programmer
..\UpgradeDownload_R25.20.3901\Bin\CmdDloader.exe -pac target\EC800GCN_LD_SLOGGERS6V001\8850XM_cat1_open_SLOGGERS6V001_merge.pac -port 18

//tạo fota thay doi nho

//Ver 0.1: đẩy dữ liệu lên MQTT hiveMQ test

//tao full fota app
tools\win32\dtools.exe fotacreate2 --single-pac bb.pac,setting\fota8910.xml output.pack -d v



SLoggerS6/866207071305126/cfg/sensor
=================================================================*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ql_app_feature_config.h"
#include "ql_api_osi.h"
#include "ql_log.h"
#include "ql_pin_cfg.h"
#include "nw_demo.h"
#include "gpio_demo.h"
#include "gpio_int_demo.h"
#include "datacall_demo.h"
#include "osi_demo.h"
#include "ql_dev_demo.h"
#include "adc_demo.h"
#include "led_cfg_demo.h"
#include "ql_sim_demo.h"
#include "power_demo.h"
#include "ql_api_dev.h"
#include"ql_uart.h"
#include "mqtt_demo.h"
#include "cJSON.h"
#include "ql_mqttclient.h"
#include "ql_api_datacall.h"

#define FW_VERSION 0.1
#define QL_INIT_LOG_LEVEL	QL_LOG_LEVEL_INFO
#define QL_INIT_LOG(msg, ...)			QL_LOG(QL_INIT_LOG_LEVEL, "ql_INIT", msg, ##__VA_ARGS__)
#define QL_INIT_LOG_PUSH(msg, ...)	QL_LOG_PUSH("ql_INIT", msg, ##__VA_ARGS__)
#define USE_LOG
//////////////////////biến lưu id task
ql_task_t ql_main1 = NULL;
ql_task_t ql_main2 = NULL;
ql_task_t mqtt_task = NULL;

char m[1000];
char message_sendMQTT[1000];
int len,len_message_sendMQTT;
/////////////////////////define MQTT
#define MQTT_CLIENT_QUECTEL_URL                  "mqtt://broker.hivemq.com:1883"
// #define MQTT_CLIENT_QUECTEL_URL                  "mqtt://giamsatnhietdo.vn:1883"
// #define MQTT_CLIENT_QUECTEL_URL                  "mqtt://183.80.84.154:1883"

#define MQTT_CLIENT_IDENTITY        "Ec200U_test"
#define MQTT_CLIENT_USER            ""
#define MQTT_CLIENT_PASS            ""
static ql_sem_t  mqtt_semp;
static int  mqtt_connected = 0;
char topic_gui[100];
char topic_sensor[100];
char topic_nhan[100];
char topic_cau_hinh[100];
mqtt_client_t  mqtt_cli;

void uart_log_init(void)
{
	#ifdef  USE_LOG
    ql_uart_config_s uart_cfg = {0};
    unsigned char data[] = "EC800G - Test1\r\n";
    uart_cfg.baudrate = QL_UART_BAUD_115200;
    uart_cfg.flow_ctrl = QL_FC_NONE;
    uart_cfg.data_bit = QL_UART_DATABIT_8;
    uart_cfg.stop_bit = QL_UART_STOP_1;
    uart_cfg.parity_bit = QL_UART_PARITY_NONE;

    ql_uart_set_dcbconfig(QL_USB_PORT_AT, &uart_cfg);
	ql_uart_open(QL_USB_PORT_AT);

	ql_uart_write(QL_USB_PORT_AT, data, strlen((char *)data));
	#endif
}
void log_print(char *data)
{
	#ifdef  USE_LOG
	char mm[1000];
	int length;
	length = sprintf(mm,"LOG: %s\r\n",data);
	ql_uart_write(QL_USB_PORT_AT, (unsigned char *)mm, length);
	#endif
}
///
static void mqtt_state_exception_cb(mqtt_client_t *client)
{
	log_print("mqtt session abnormal disconnect");
	mqtt_connected = 0;
}

static void mqtt_connect_result_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_e status)
{
	sprintf(m,"status: %d", status);
	log_print(m);
	if(status == 0){
		mqtt_connected = 1;
	}
	ql_rtos_semaphore_release(mqtt_semp);
}

static void mqtt_requst_result_cb(mqtt_client_t *client, void *arg,int err)
{
	sprintf(m,"err: %d", err);
	log_print(m);
	ql_rtos_semaphore_release(mqtt_semp);
}
static void mqtt_inpub_data_cb(mqtt_client_t *client, void *arg, int pkt_id, const char *topic, const unsigned char *payload, unsigned short payload_len)
{
    //  char buff[1000];
	sprintf(m,"topic: %s", topic);
	log_print(m);
	sprintf(m,"payload: %s %d", payload,payload_len);
	log_print(m);

	//kiểm tra nếu topic là topic nhận thì mới xử lý
    if(strcmp(topic_nhan,topic) == 0)
    {
		sprintf(m,"dung toppic nhan du lieu");
        log_print(m);

		// cJSON *pJsonRoot = cJSON_Parse((char*)payload);
		// cJSON *cmd = cJSON_GetObjectItem(pJsonRoot,"cmd");

		//nếu cmd tồn tại thì mới thực hiện
    }
    
    
}

static void mqtt_disconnect_result_cb(mqtt_client_t *client, void *arg,int err)
{
	sprintf(m,"err: %d", err);
	log_print(m);
	ql_rtos_semaphore_release(mqtt_semp);
}

static void mqtt_app_thread(void * arg)
{
    char imei[100]={0};
	int ret = 0;
	int i = 0;
	int profile_idx = 1;
    ql_data_call_info_s info;
	char ip4_addr_str[16] = {0};
	uint8_t nSim = 0;
	uint16_t sim_cid;
    struct mqtt_connect_client_info_t  client_info = {0};
    uint16_t dem = 0;
	ql_rtos_semaphore_create(&mqtt_semp, 0);
	ql_rtos_task_sleep_s(10);

    //get imei
    ql_dev_get_imei(imei,30,0);
    // log_print("imei:");
    // log_print(imei);
	//tạo topic gửi data
    strcpy(topic_cau_hinh,"SLoggerS6/");
    strcat(topic_cau_hinh,imei);
    strcat(topic_cau_hinh,"/cfg/setting");

    strcpy(topic_nhan,"SLoggerS6/");
    strcat(topic_nhan,imei);
    strcat(topic_nhan,"/cfg/cmd");

	//tạo topic nhận data
    strcpy(topic_gui,"SLoggerS6/");
    strcat(topic_gui,imei);
    strcat(topic_gui,"/get/response");

	//tạo topic gửi sensor
    strcpy(topic_sensor,"SLoggerS6/");
    strcat(topic_sensor,imei);
    strcat(topic_sensor,"/cfg/sensor");

    sprintf(m,"topic nhan cmd %s", topic_nhan);
    log_print(m);
    sprintf(m,"topic gui %s", topic_gui);
    log_print(m);
    sprintf(m,"topic cau hinh %s", topic_cau_hinh);
    log_print(m);
    sprintf(m,"topic sensor %s", topic_sensor);
    log_print(m);
    
 while(1)
 { 
	log_print("========== mqtt demo start ==========");
	log_print("wait for network register done");
		
	while((ret = ql_network_register_wait(nSim, 120)) != 0 && i < 10){
    	i++;
		ql_rtos_task_sleep_s(1);
	}
	if(ret == 0){
		i = 0;
		log_print("====network registered!!!!====");
	}else{
		log_print("====network register failure!!!!!====");
		goto exit;
	}

	ql_set_data_call_asyn_mode(nSim, profile_idx, 0);

	log_print("===start data call====");
	ret=ql_start_data_call(nSim, profile_idx, QL_PDP_TYPE_IP, "uninet", NULL, NULL, 0); 
    sprintf(m,"===data call result:%d", ret);
    log_print(m);
	if(ret != 0)
    {
		log_print("====data call failure!!!!=====");
	}
	memset(&info, 0x00, sizeof(ql_data_call_info_s));
	
	ret = ql_get_data_call_info(nSim, profile_idx, &info);
	if(ret != 0)
    {
		sprintf(m,"ql_get_data_call_info ret: %d", ret);
		log_print(m);
		ql_stop_data_call(nSim, profile_idx);
		goto exit;
	}
    sprintf(m,"info->profile_idx: %d", info.profile_idx);
    log_print(m);
    sprintf(m,"info->ip_version: %d", info.ip_version);
    log_print(m);
    sprintf(m,"info->v4.state: %d", info.v4.state);
    log_print(m); 
    inet_ntop(AF_INET, &info.v4.addr.ip, ip4_addr_str, sizeof(ip4_addr_str));
    sprintf(m,"info.v4.addr.ip: %s\r\n", ip4_addr_str);
    log_print(m);
	inet_ntop(AF_INET, &info.v4.addr.pri_dns, ip4_addr_str, sizeof(ip4_addr_str));
    sprintf(m,"info.v4.addr.pri_dns: %s\r\n", ip4_addr_str);
    log_print(m);
	inet_ntop(AF_INET, &info.v4.addr.sec_dns, ip4_addr_str, sizeof(ip4_addr_str));
    sprintf(m,"info.v4.addr.sec_dns: %s\r\n", ip4_addr_str);
    log_print(m);

	
	while(1)
	{	

		int ret = MQTTCLIENT_SUCCESS;
		sprintf(m,"==============mqtt_client_test[%d]================\n",1);
		log_print(m);

		if(QL_DATACALL_SUCCESS != ql_bind_sim_and_profile(nSim, profile_idx, &sim_cid))
		{
			log_print("nSim or profile_idx is invalid!!!!");
			break;
		}
		
		if(ql_mqtt_client_init(&mqtt_cli, sim_cid) != MQTTCLIENT_SUCCESS){
			log_print("mqtt client init failed!!!!");
			break;
		}
;
		sprintf(m,"mqtt_cli:%d", mqtt_cli);
		log_print(m);


		client_info.keep_alive = 60;
		client_info.clean_session = 1;
		client_info.will_qos = 0;
		client_info.will_retain = 0;
		client_info.will_topic = NULL;
		client_info.will_msg = NULL;
		client_info.client_id = imei;
		client_info.client_user = MQTT_CLIENT_USER;
		client_info.client_pass = MQTT_CLIENT_PASS;

//        log_print("connect ssl %d onenet mode %d",case_id,is_user_onenet);
		client_info.ssl_cfg = NULL;
		ret = ql_mqtt_connect(&mqtt_cli, MQTT_CLIENT_QUECTEL_URL , mqtt_connect_result_cb, NULL, (const struct mqtt_connect_client_info_t *)&client_info, mqtt_state_exception_cb);

		if(ret  == MQTTCLIENT_WOUNDBLOCK)
        {
			log_print("====wait connect result");
			ql_rtos_semaphore_wait(mqtt_semp, QL_WAIT_FOREVER);
			if(mqtt_connected == 0){
				log_print("====ql_mqtt_client_deinit");
				ql_mqtt_client_deinit(&mqtt_cli);
				break;
			}
		}
        else
        {
            sprintf(m,"===mqtt connect failed ,ret = %d",ret);
            log_print(m);
			break;
		}

		ql_mqtt_set_inpub_callback(&mqtt_cli, mqtt_inpub_data_cb, NULL);
		if(ql_mqtt_sub_unsub(&mqtt_cli, topic_nhan, 1, mqtt_requst_result_cb,NULL, 1) == MQTTCLIENT_WOUNDBLOCK)
        {
			log_print("======wait subscrible result");
			ql_rtos_semaphore_wait(mqtt_semp, QL_WAIT_FOREVER);
		}
		if(ql_mqtt_sub_unsub(&mqtt_cli, topic_cau_hinh, 1, mqtt_requst_result_cb,NULL, 1) == MQTTCLIENT_WOUNDBLOCK)
        {
			log_print("======wait subscrible result");
			ql_rtos_semaphore_wait(mqtt_semp, QL_WAIT_FOREVER);
		}
		ql_rtos_task_sleep_ms(200);
		do
		{		
            cJSON *pRoot = cJSON_CreateObject();
			//nếu null thì return làm việc khác
			if (pRoot == NULL)
			{
				ql_rtos_task_sleep_ms(100);
			}
			else
			{
				cJSON_AddNumberToObject(pRoot,"version",FW_VERSION);
				cJSON_AddNumberToObject(pRoot,"dem",dem);
                char *payload_send = cJSON_Print(pRoot);
				len_message_sendMQTT = sprintf(message_sendMQTT,"%s",payload_send);
				if(ql_mqtt_publish(&mqtt_cli, topic_sensor, message_sendMQTT, len_message_sendMQTT, 0, 0, mqtt_requst_result_cb,NULL) == MQTTCLIENT_WOUNDBLOCK){
					// log_print("======wait publish result");
					ql_rtos_semaphore_wait(mqtt_semp, QL_WAIT_FOREVER);
				}

				cJSON_free((void*)payload_send);
				cJSON_Delete(pRoot);
				ql_rtos_task_sleep_ms(1000);
                dem++;
			}
            
			
		}
		while(mqtt_connected == 1);
        if(mqtt_connected == 1 && ql_mqtt_disconnect(&mqtt_cli, mqtt_disconnect_result_cb, NULL) == MQTTCLIENT_WOUNDBLOCK){
            log_print("=====wait disconnect result");
            ql_rtos_semaphore_wait(mqtt_semp, QL_WAIT_FOREVER);
        }

		sprintf(m,"==============mqtt_client_test[%d] end=======%x=========\n",1,mqtt_cli);
		log_print(m);
		ql_mqtt_client_deinit(&mqtt_cli);
		mqtt_connected = 0;

	}
	
exit:
log_print("Exit app mqtt");
 }
	
    ql_rtos_semaphore_delete(mqtt_semp);
    ql_rtos_task_delete(mqtt_task);	

   return;	
}

static void main1(void *param)
{
    while(1)
    {
		ql_rtos_task_sleep_ms(100);

    }
}
static void main2(void *param)
{
    while(1)
    {
		ql_rtos_task_sleep_ms(100);

    }
}
int appimg_enter(void *param)
{
	QlOSStatus err = QL_OSI_SUCCESS;
    sprintf(m,"init demo enter: %s @ %s", QL_APP_VERSION, QL_APP_BUILD_RELEASE_TYPE);
	log_print(m);
    ql_dev_cfg_wdt(1);
    ql_log_set_port(0);
    ql_quec_trace_enable(0);
    uart_log_init();

    err = ql_rtos_task_create(&ql_main1, 1024*16, APP_PRIORITY_NORMAL, "ql_main1", main1, NULL, 1);
    err = ql_rtos_task_create(&ql_main2, 1024*16, APP_PRIORITY_NORMAL, "ql_main2", main2, NULL, 1);
    err = ql_rtos_task_create(&mqtt_task, 16*1024, 23, "mqtt_app", mqtt_app_thread, NULL, 1);
    return err;
}

void appimg_exit(void)
{
    QL_INIT_LOG("init demo exit");
}


