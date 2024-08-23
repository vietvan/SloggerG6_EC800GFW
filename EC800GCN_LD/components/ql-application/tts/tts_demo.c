/*================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/

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
		
	5.  (1)预置16k中文TTS资源文件到外部4线flash
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
#define	QL_TTS_LANGUAGE_ENGLISH			0

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


/*===========================================================================
 * Variate
 ===========================================================================*/

PCM_HANDLE_T tts_player = NULL;
ql_task_t ql_tts_demo_task = NULL;

/*===========================================================================
 * Functions
 ===========================================================================*/

int userCallback(void *param, int param1, int param2, int param3, int data_len, const void *pcm_data)
{
	int err;

	err = ql_pcm_write(tts_player, (void *)pcm_data, data_len);
	if(err <= 0)
	{
		QL_TTS_LOG("write data to PCM player failed");
		return -1;
	}	
    return 0;
}


void ql_tts_thread(void *param)
{
    int err = 0, cnt = 0, len=0;
	QL_PCM_CONFIG_T config = {1, 16000, 0};
	tts_param_t tts_param = {0};
	
	ql_set_audio_path_earphone();
	ql_aud_set_volume(QL_AUDIO_PLAY_TYPE_LOCAL, AUDIOHAL_SPK_VOL_11);

	tts_player = ql_pcm_open(&config, QL_PCM_BLOCK_FLAG|QL_PCM_WRITE_FLAG);
	tts_demo_no_err(!tts_player, goto exit, "create pcm_player failed");

#if  !QL_TTS_LANGUAGE_ENGLISH
	tts_param.resource = TTS_RESOURCE_16K_CN;
#else
	tts_param.resource = TTS_RESOURCE_16K_EN;
#endif

#if		QL_TTS_LOCATION==1   //使用的tts库在外部6线flash
		tts_param.position = POSIT_EFS;      
#elif	QL_TTS_LOCATION==2   //使用的tts库在外部4线flash
		tts_param.position = POSIT_EXNSFFS;
#else                        //默认使用的tts库在内置flash
		tts_param.position = POSIT_INTERNAL_FS;   
#endif

  //err = ql_tts_engine_init(userCallback); //若使用默认的中文16k资源，且资源文件预置到内置flash, 则直接调用ql_tts_engine_init即可
	err = ql_tts_engine_init_ex(userCallback, &tts_param);
	tts_demo_no_err(err, goto exit, "tts session begain failed");
		
	while(1)
	{	
#if  !QL_TTS_LANGUAGE_ENGLISH	//英文资源文件无法播中文TTS
		if(cnt == 0)  //play utf8 encoding
		{
			char *tts_str = "֧支付宝收款123456789.000元";        //utf-8 encoding
			len = strlen(tts_str);
			ql_tts_set_config_param(QL_TTS_CONFIG_ENCODING,QL_TTS_UTF8);
            ql_tts_set_config_param(QL_TTS_CONFIG_DGAIN, 0);////set tts  digtal  gain

			err = ql_tts_start((const char *)tts_str, len);
			tts_demo_no_err(err, goto exit, "tts start failed");
		}
		else if(cnt == 1) //play ucs2 encoding
		{
			uint16 tts_str[8] = {0x6B22, 0x8FCE, 0x4F7F, 0x7528, 0x79FB, 0x8FDC, 0x6A21, 0x5757}; //欢迎使用移远模块
			len = sizeof(tts_str);
			ql_tts_set_config_param(QL_TTS_CONFIG_ENCODING,QL_TTS_UCS2);
            ql_tts_set_config_param(QL_TTS_CONFIG_DGAIN, 0);//set tts  digtal  gain

			err = ql_tts_start((const char *)tts_str, len);
			tts_demo_no_err(err, goto exit, "tts start failed");
		}
#else
		if(cnt == 2)
		{
			char *str_eng = "The price of the shirt is $50, and the price of the computer is $1200";
			len = strlen(str_eng);
			
			err = ql_tts_start((const char *)str_eng, len);
			tts_demo_no_err(err, goto exit, "tts start failed");
		}
#endif

		ql_rtos_task_sleep_ms(1000);							
		if(cnt == 2){
			goto exit;
		}
		else{
			cnt++;
			continue;
		}
	}

	exit:
		ql_aud_data_done();
		ql_aud_wait_play_finish(QL_WAIT_FOREVER);
		ql_tts_end();
	
		if(tts_player){
			ql_pcm_close(tts_player);
			tts_player = NULL;
		}		
		QL_TTS_LOG("tts done");
		ql_rtos_task_delete(NULL);
}



void ql_tts_demo1_init(void)
{
    uint8_t err = QL_OSI_SUCCESS;
      
    err = ql_rtos_task_create(&ql_tts_demo_task, QL_TTS_TASK_STACK, QL_TTS_TASK_PRIO, "ql_tts_task", ql_tts_thread, NULL, 1);
    if (err != QL_OSI_SUCCESS)
    {
        QL_TTS_LOG("TTS demo task created failed");
    }
}

