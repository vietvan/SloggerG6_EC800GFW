/*================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/

/************************************************************************************************************
  此demo将tts的api，与audio的播放函数一起进行封装，封装后的ql_tts_play，ql_tts_init，ql_tts_deinit与我司
  ASR系列的TTS播放函数类似，直接调用ql_tts_init+ql_tts_play即可以开始播放，播放完成后，调用ql_tts_deinit
  函数释放TTS资源。用户可参考tts_demo.c中的方法，也可使用本demo中的方法进行TTS播放
  注意：由于audio的播放器只有一个，因此此demo与tts_demo.c同一时间只能有一个运行

  用户如需将TTS库放到内核，则在target.config中将 CONFIG_QUEC_PROJECT_FEATURE_TTS_IN_KERNEL 置为 y,
  并修改分区，将内核分区增大250k左右，其余的内容不变
*************************************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tts_demo.h"
#include "ql_api_osi.h"
#include "ql_api_tts.h"
#include "ql_log.h"
#include "ql_osi_def.h"
#include "ql_audio.h"
#include "ql_fs.h"
#include "ql_api_dev.h"

/* 
	1. 不同的TTS资源文件，对应的播放效果不同。其中中文资源不能用来播英文单词，单词会以字母的方式播出; 英文资源也不能用来播中文。默认使用
	   中文16k TTS资源

	2. 若使用16k中文TTS资源，且TTS资源文件预置到内置flash中，则不需要修改json脚本(脚本默认已选择预置16k中文资源，且预置到内置flash)，只需要调用
	   ql_tts_engine_init函数即可完成初始化，不需要关注以下描述
	
	3. 所有的资源文件均在components\ql-config\download\prepack下，其中:
	   英文16k资源文件名为: "quectel_tts_resource_english_16k.bin"
	   中文8k资源文件为："quectel_tts_resource_chinese_8k.bin"
	   中文16k资源文件为："quectel_pcm_resource.bin"

	4. 预置文件时，请将json脚本中的"file"固定为"/qsfs/quectel_pcm_resource.bin"(预置资源文件到内置flash), 或
	   "/ext/qsfs/quectel_pcm_resource.bin"(预置到外置6线spi flash中), 并修改"local_file"来选择上传哪个资源文件，如下述示例.
	   若不使用中文16k资源，则需要使用"ql_tts_engine_init_ex"函数，将配置结构体中的"resource"变量设置为需要使用的资源;
	   若将资源文件预置到外置6线spi flash，需要将"position"变量设置为 POSIT_EFS

	   当TTS资源文件预置在内置Flash时，针对需要FOTA升级的情况，新版本SDK中默认将该文件进行拆分为多个子文件进行预置！
	   外置存储时可以不用拆分。
	   
	5. 使用英文16k TTS资源播放时，需要1.45M的RAM空间，因此要注意RAM空间是否充足； 选择中文16k TTS资源文件时，需要620k的RAM空间； 选择中文
	   8k资源时，需要570kRAM空间


预置文件示例：
	1. 预置16k中文TTS资源文件到内部flash(默认):
		"files": [
			{
				"file": "/qsfs/quectel_pcm_resource.bin",
				"local_file": "quectel_pcm_resource.bin"
			}
		]
	2. 预置16k英文TTS资源文件到内部flash(以"/qsfs/quectel_pcm_resource.bin"为文件系统路径)
		"files": [
			{
				"file": "/qsfs/quectel_pcm_resource.bin",
				"local_file": "quectel_tts_resource_english_16k.bin"
			}
		]
	3. 预置8K中文TTS资源文件到内部flash(以"/qsfs/quectel_pcm_resource.bin"为文件系统路径)
		"files": [
			{
				"file": "/qsfs/quectel_pcm_resource.bin",
				"local_file": "quectel_tts_resource_chinese_8k.bin"
			}
		]
	4.  (1)预置16k英文TTS资源到外置6线spi flash(以/ext/qsfs/quectel_pcm_resource.bin"为文件系统路径)
		"files": [
			{
				"file": "/ext/qsfs/quectel_pcm_resource.bin",
				"local_file": "quectel_tts_resource_english_16k.bin"
			}
		]
		(2)需要把boot_fdl_dnld.c文件的bool fdlDnldStart(fdlEngine_t *fdl, unsigned devtype)，
			6线flash部分的#if 0打开为1（CONFIG_QUEC_PROJECT_FEATURE_SPI6_EXT_NOR_SFFS部分）;
		(3)在target.config中，CONFIG_QUEC_PROJECT_FEATURE_SPI6_EXT_NOR_SFFS打开，CONFIG_QUEC_PROJECT_FEATURE_SPI4_EXT_NOR关闭
		
	5.  (1)预置16k中文TTS资源文件到外置4线flash
		"files": [
			{
				"file": "/ext4n/qsfs/quectel_pcm_resource.bin",
				"local_file": "quectel_pcm_resource.bin"
			}
		]
		(2)需要把boot_fdl_dnld.c文件的bool fdlDnldStart(fdlEngine_t *fdl, unsigned devtype)，
			4线flash部分的#if 0打开为1（CONFIG_QUEC_PROJECT_FEATURE_SPI4_EXT_NOR_SFFS部分）;
		(3)在target.config中，CONFIG_QUEC_PROJECT_FEATURE_SPI4_EXT_NOR_SFFS打开，CONFIG_QUEC_PROJECT_FEATURE_SPI6_EXT_NOR关闭
*/

#define	QL_TTS_LANGUAGE_USE_ENGLISH			0

/*0:tts库在内置flash，1:tts库在六线flash，2：tts库在四线flash*/
#define QL_TTS_LOCATION   0

#define QL_TTS_LOG_LEVEL	            QL_LOG_LEVEL_INFO
#define QL_TTS_LOG(msg, ...)			QL_LOG(QL_TTS_LOG_LEVEL, "ql_app_tts", msg, ##__VA_ARGS__)
#define QL_TTS_LOG_PUSH(msg, ...)		QL_LOG_PUSH("ql_app_tts", msg, ##__VA_ARGS__)

#if !defined(tts_demo_no_err)
	#define tts_demo_no_err(x, action, str)																		\
			do                                                                                                  \
			{                                                                                                   \
				if(x != 0)                                                                        				\
				{                                                                                               \
					QL_TTS_LOG(str);																			\
					{action;}																					\
				}                                                                                               \
			} while( 1==0 )
#endif

typedef struct
{
	ql_tts_encoding_e encode;
	char 			  *str;
	uint			  len;
}tts_demo_play_info_t;

/*===========================================================================
 * Variate
 ===========================================================================*/
PCM_HANDLE_T ql_player = NULL;
PCM_HANDLE_T ql_recorder = NULL;

ql_task_t  ql_tts_demo_task2 = NULL;
ql_queue_t ql_tts_demo_queue = NULL;
/*===========================================================================
 * Functions
 ===========================================================================*/
int userCallback(void *param, int param1, int param2, int param3, int data_len, const void *pcm_data)
{
	int err;

	err = ql_pcm_write(ql_player, (void *)pcm_data, data_len);
	if(err <= 0)
	{
		QL_TTS_LOG("write data to PCM player failed");
		return -1;
	}	
    return 0;
}

void ql_tts_thread_demo_2(void *param)
{
    int err = 0;
	tts_demo_play_info_t info = {0};
		
	ql_set_audio_path_earphone();
	ql_aud_set_volume(QL_AUDIO_PLAY_TYPE_LOCAL, AUDIOHAL_SPK_VOL_11);

    poc_demo_test();
    
	while(1)
	{	
		err = ql_rtos_queue_wait(ql_tts_demo_queue, (uint8 *)&info, sizeof(tts_demo_play_info_t), QL_WAIT_FOREVER);
		tts_demo_no_err(err, continue, "invalid queue");
		
		if(!info.str || !info.len){
			QL_TTS_LOG("invalid tts string");
			continue;
		}
        
        ql_pcm_poc_init_ex();
        
        err = ql_tts_init(userCallback);
        tts_demo_no_err(err, goto exit, "tts init failed"); 

#if !QL_TTS_LANGUAGE_USE_ENGLISH //使用英文TTS库不需要设置编码
		err = ql_tts_set_config_param(QL_TTS_CONFIG_ENCODING, info.encode);
		tts_demo_no_err(err, goto exit, "config tts failed");
#endif

        //设置使用数字播报
        //err = ql_tts_set_config_param(QL_TTS_CONFIG_DIGITS,TTS_DIGIT_NUMBER);
		//tts_demo_no_err(err, goto exit, "config tts failed");
        
		err = ql_tts_start(info.str, info.len);
		tts_demo_no_err(err, goto exit, "tts start failed");
        
    	while(ql_pcm_buffer_used(ql_player))  //in poc mode, player will not stop if not ql_aud_stop_poc_mode called
    	{
    		ql_rtos_task_sleep_ms(20); //wait the write buffer empty
    	}	
        
exit:
		if(info.str){
			free(info.str);
		}
	
		QL_TTS_LOG("tts done");
        ql_pcm_poc_deinit_ex();
	    ql_tts_deinit();
	}
}

int ql_tts_init(pUserCallback mCallback)
{	
	tts_param_t tts_param = {0};
	
	if(!mCallback){
		return QL_TTS_INVALID_PARAM;
	}

	if(!ql_tts_is_running())
	{

#if  !QL_TTS_LANGUAGE_USE_ENGLISH
		tts_param.resource = TTS_RESOURCE_16K_CN;  //使用中文16k资源
#else
		tts_param.resource = TTS_RESOURCE_16K_EN;  //使用英文16k资源
#endif


#if		QL_TTS_LOCATION==1   //使用的tts库在外部6线flash
		tts_param.position = POSIT_EFS;      
#elif	QL_TTS_LOCATION==2   //使用的tts库在外部4线flash
		tts_param.position = POSIT_EXNSFFS;
#else                        //默认使用的tts库在内置flash
		tts_param.position = POSIT_INTERNAL_FS;   
#endif

		//int err = ql_tts_engine_init(userCallback); //若使用默认的中文16k资源，且资源文件预置到内置flash, 则直接调用ql_tts_engine_init即可
		int	err = ql_tts_engine_init_ex(userCallback, &tts_param);
		tts_demo_no_err(err, return err, "tts session begain failed");
	}
	else
	{
		QL_TTS_LOG("tts is running");
		return QL_TTS_DEVICE_BUSY;
	}
	return QL_TTS_SUCCESS;
}

int ql_tts_deinit(void)
{
	int err = 0;

	err = ql_tts_end();
	tts_demo_no_err(err, return err, "tts end failed");

	return err;
}

int ql_tts_play(ql_tts_encoding_e encoding, const char* string, uint len)
{
	tts_demo_play_info_t param = {0};
	int err = 0;

	if(encoding < QL_TTS_GBK || encoding > QL_TTS_UCS2 || !string || !len)
	{
		QL_TTS_LOG("invalid param");
		return QL_TTS_INVALID_PARAM;
	}

	param.encode = encoding;
	param.len = len;
	param.str = calloc(1, len);
	if(!param.str)
	{
		QL_TTS_LOG("tts no memory");
		return QL_TTS_NO_MEMORY;
	}
	
	memcpy(param.str, string, len);
	
	err = ql_rtos_queue_release(ql_tts_demo_queue, sizeof(tts_demo_play_info_t), (uint8 *)&param, 0);
	if(err)
	{
		free(param.str);
	}

	return err;
}

int ql_tts_play_english(const char* string, uint len)
{
	tts_demo_play_info_t param = {0};
	int err = 0;

	if(!string || !len)
	{
		QL_TTS_LOG("invalid param");
		return QL_TTS_INVALID_PARAM;
	}

	param.len = len;
	param.str = calloc(1, len);
	if(!param.str)
	{
		QL_TTS_LOG("tts no memory");
		return QL_TTS_NO_MEMORY;
	}

	memcpy(param.str, string, len);
	err = ql_rtos_queue_release(ql_tts_demo_queue, sizeof(tts_demo_play_info_t), (uint8 *)&param, 0);
	if(err)
	{
		free(param.str);
	}
	return err;
}

void ql_tts_demo2_init(void)
{
    uint8_t err = QL_OSI_SUCCESS;
	
	err = ql_rtos_queue_create(&ql_tts_demo_queue, sizeof(tts_demo_play_info_t), 10);
	if (err != QL_OSI_SUCCESS)
    {
        QL_TTS_LOG("TTS queue created failed");
		return;
    }
	
    err = ql_rtos_task_create(&ql_tts_demo_task2, QL_TTS_TASK_STACK, QL_TTS_TASK_PRIO-1, "ql_tts_task", ql_tts_thread_demo_2, NULL, 1);
    if (err != QL_OSI_SUCCESS)
    {
    	ql_rtos_queue_delete(ql_tts_demo_queue);
		ql_tts_demo_queue = NULL;
        QL_TTS_LOG("TTS demo task2 created failed");
		return;
    }

#if !QL_TTS_LANGUAGE_USE_ENGLISH	//播放中文TTS
	char *str1 = "支付宝收款 12345元";
	char *str2 = "您已超速, 请减速";
	char *str3 = "条形码为: 2 2 1 9 8 3 3 6 4 5 2 3 8 8"; //空格代表以号码的方式播报
	uint16 ucs_str[8] = {0x6B22, 0x8FCE, 0x4F7F, 0x7528, 0x79FB, 0x8FDC, 0x6A21, 0x5757}; //欢迎使用移远模块

	ql_tts_play(QL_TTS_UTF8, str1, strlen(str1));
	ql_tts_play(QL_TTS_UTF8, str2, strlen(str2));
	ql_tts_play(QL_TTS_UTF8, str3, strlen(str3));
	ql_tts_play(QL_TTS_UCS2, (const char *)ucs_str, sizeof(ucs_str));
#else //播放英文TTS
	char *str_eng = "The price of the shirt is $50, and the price of the computer is $1200";
	ql_tts_play_english(str_eng, strlen(str_eng));
#endif
	
}

void poc_demo_test(void)
{
	void *data = NULL;
	int size, total_size = 0, cnt=0, write_cnt=0;

    ql_pcm_poc_init_ex();
    QL_TTS_LOG("##poc start");
	ql_pcm_record_init_ex();
    
	data = malloc(100*1024);
	if(data == NULL)
	{
		goto exit;	
	}

	QL_TTS_LOG("start read");

//start record
	while(total_size < 80*1024)
	{
		size = ql_pcm_record_ex(data+total_size, 640);

		if(size <= 0)
		{
			break;
		}
		
		total_size += size;
	}
	ql_pcm_record_deinit_ex();
	QL_TTS_LOG("exit record");

	if(total_size <= 0)
	{
		QL_TTS_LOG("read pcm failed");
		goto exit;			
	}
	QL_TTS_LOG("size is %d", total_size);

	while(write_cnt < total_size)
	{
		if(total_size - write_cnt > PACKET_WRITE_MAX_SIZE)  //单次最多可写 PACKET_WRITE_MAX_SIZE 字节
		{
			cnt = ql_pcm_play_ex(data+write_cnt, PACKET_WRITE_MAX_SIZE);
		}
		else
		{
			cnt = ql_pcm_play_ex(data+write_cnt, total_size - write_cnt);
		}
		if(cnt <= 0)
		{
			QL_TTS_LOG("write failed");
			goto exit;	
		}
		write_cnt += cnt;
	}
	while(ql_pcm_buffer_used(ql_player))  //in poc mode, player will not stop if not ql_aud_stop_poc_mode called
	{
		ql_rtos_task_sleep_ms(20); //wait the write buffer empty
	}	
    
    ql_pcm_play_stop_ex();
exit:
	if(data != NULL)
	{
		free(data);
		data = NULL;
	}
    ql_pcm_poc_deinit_ex();

}

/*************************************************** Audio API 封装 ***************************************************************************/
void ql_pcm_poc_init_ex(void)
{
    QL_PCM_CONFIG_T pcm_config = {1, 16000, 0};

	if(ql_recorder == NULL)
	{
		ql_recorder = ql_aud_pcm_open(&pcm_config, QL_AUDIO_FORMAT_PCM, QL_PCM_BLOCK_FLAG|QL_PCM_READ_FLAG, QL_PCM_POC);
		if(ql_recorder == NULL)
		{
            return;
		}	
	}
    
	if(ql_player == NULL)
	{
		ql_player = ql_aud_pcm_open(&pcm_config, QL_AUDIO_FORMAT_PCM, QL_PCM_BLOCK_FLAG|QL_PCM_WRITE_FLAG, QL_PCM_POC);
		if(ql_player == NULL)
		{
            return;
		}
	}    
    
	ql_aud_start_poc_mode(QL_POC_TYPE_HALF_DUPLEX);
	ql_aud_poc_switch(QL_POC_MODE_PLAY);	
}


int ql_pcm_play_ex(uint8_t *data, uint32_t count)
{	
	return ql_pcm_write(ql_player, data, count);
}

void ql_pcm_play_stop_ex(void)
{
	ql_pcm_buffer_reset(ql_player);
}

void ql_pcm_record_init_ex(void)
{
	ql_aud_poc_switch(QL_POC_MODE_REC);
}

void ql_pcm_record_deinit_ex(void)
{
	ql_aud_poc_switch(QL_POC_MODE_PLAY);
}

int ql_pcm_record_ex(void *data, uint32_t count)
{
	int cnt_read;
	if(ql_recorder)
	{
		cnt_read = ql_pcm_read(ql_recorder, data, count);		
	}
	else
    {
		cnt_read = -1;
	}

    return cnt_read;
}
void ql_pcm_poc_deinit_ex(void)
{

    ql_aud_stop_poc_mode();
    if(ql_player != NULL)
    {
        ql_pcm_close(ql_player);
        ql_player = NULL;
    }
    if(ql_recorder != NULL)
    {
        ql_pcm_close(ql_recorder);
        ql_recorder = NULL;
    }

}


