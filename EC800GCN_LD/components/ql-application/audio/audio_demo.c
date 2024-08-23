/*================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ql_api_osi.h"

#include "ql_log.h"
#include "audio_demo.h"
#include "ql_osi_def.h"
#include "ql_audio.h"
#include "ql_fs.h"
#include "ql_i2c.h"
#include "quec_pin_index.h"
#include "ql_gpio.h"

#define QL_AUDIO_LOG_LEVEL	            QL_LOG_LEVEL_INFO
#define QL_AUDIO_LOG(msg, ...)			QL_LOG(QL_AUDIO_LOG_LEVEL, "ql_audio", msg, ##__VA_ARGS__)
#define QL_AUDIO_LOG_PUSH(msg, ...)		QL_LOG_PUSH("ql_AUDIO", msg, ##__VA_ARGS__)

#if !defined(audio_demo_no_err)
	#define audio_demo_no_err(x, action, str)																		\
			do                                                                                                  \
			{                                                                                                   \
				if(x != 0)                                                                        				\
				{                                                                                               \
					QL_AUDIO_LOG(str);																			\
					{action;}																					\
				}                                                                                               \
			} while( 1==0 )
#endif

/* for headset_det */
#ifdef QL_APP_FEATURE_HEADSET_DET
#define QL_HEADSET_DETECT_PIN              QUEC_PIN_DNAME_GPIO_8
#define QL_HEADSET_DETECT_GPIO             QUEC_GPIO_DNAME_GPIO_8
#define QL_HEADSET_DETECT_FUNC_GPIO        0
#define QL_HEADSET_DETECT_DEBOUNCE_TIME    300    //unit: ms

#define DEMO_HEADSET_DETECT_PLUG_IN          1
#define DEMO_HEADSET_DETECT_PLUG_OUT         0

ql_task_t headset_det_task = NULL;
ql_timer_t headset_det_debounce_timer = NULL;
#endif

#define ID_RIFF 	0x46464952
#define ID_WAVE		0x45564157
#define ID_FMT  	0x20746d66
#define FORMAT_PCM 	1

#define CHECK_AUDIO_CORRECT     0
#define CHECK_AUDIO_INVALID_PARAMETER       1
#define CHECK_AUDIO_WAV_ERR     2
#define CHECK_AUDIO_MP3_ERR     3
#define CHECK_AUDIO_AMR_ERR     4

#define	QL_APP_EXT_CODEC_FLAG		0  //0: use default register config  1: use extcodec_cfg cb,Customer control codec config

struct wav_header{
	unsigned int riff_id;
	unsigned int riff_sz;
	unsigned int riff_fmt;
	unsigned int fmt_id;
	unsigned int fmt_sz;
	unsigned short audio_format;
	unsigned short num_channels;
	unsigned int sample_rate;
	unsigned int byte_rate;
	unsigned short block_align;
	unsigned short bits_per_sample;
	unsigned int data_id;
	unsigned int data_sz;
};

typedef struct
{
	PCM_HANDLE_T recorder;
	PCM_HANDLE_T player;
}ql_demo_poc_t;

#ifdef QL_APP_FEATURE_AUDIO_RECORD
static uint8 *pcm_buffer = NULL;
static uint  pcm_data_size = 0;
#endif
static bool  ring_tone_start = 0;
ql_task_t ql_play_task = NULL;

static int play_callback(char *p_data, int len, enum_aud_player_state state)
{
	if(state == AUD_PLAYER_START)
	{
		QL_AUDIO_LOG("player start run");
	}
	else if(state == AUD_PLAYER_FINISHED)
	{
		QL_AUDIO_LOG("player stop run");
	}
	else
	{
		QL_AUDIO_LOG("type is %d", state);
	}

	return QL_AUDIO_SUCCESS;
}

#ifdef QL_APP_FEATURE_AUDIO_RECORD

static int record_callback(char *p_data, int len, enum_aud_record_state state)
{	
	if(state == AUD_RECORD_START)
	{
		QL_AUDIO_LOG("recorder start run");
	}
	else if(state == AUD_RECORD_CLOSE)
	{
		QL_AUDIO_LOG("recorder stop run");
	}
    else if(state == AUD_RECORD_CALL_INT)
    {
        QL_AUDIO_LOG("recorder int from ring/call");
    }
	else if(state == AUD_RECORD_DATA)
	{
		if(len <= 0)
			return -1;

		if(pcm_data_size > RECORD_BUFFER_MAX){
			return -1;
		}
		else{
			memcpy(pcm_buffer+pcm_data_size, p_data, len);
			pcm_data_size += len;
		}
	}

	return QL_AUDIO_SUCCESS;
}
#endif

static void ql_audio_demo_thread(void *param)
{
	QL_AUDIO_LOG("enter audio demo"); 
#if QL_APP_EXT_CODEC_FLAG
    ql_ext_codec_cb_init();
#endif    
	//test_pcm();
	//test_mp3();
	//test_wav();
	//test_amr();
	//test_amr_stream();
#ifdef QL_APP_FEATURE_AUDIO_RECORD
	//test_record_file();
	//test_record_stream();
    //test_poc_full_duplex();
    //test_poc_half_duplex();
#endif

	QL_AUDIO_LOG("test done, exit audio demo");
	ql_rtos_task_delete(NULL);
}

static int check_wav_file(QFILE fd)
{
    int ret = 0;
    struct wav_header hdr;
    ret = ql_fseek(fd, 0, SEEK_SET);
    if(ret < 0)
    {
        return -1;
    }

    ret = ql_fread(&hdr, sizeof(hdr), 1, fd);
    if(ret < sizeof(hdr))
    {
        ql_fseek(fd, 0, SEEK_SET);
        return -1;
    }

    if( (hdr.riff_id != ID_RIFF) || (hdr.riff_fmt != ID_WAVE)
    || (hdr.fmt_id != ID_FMT) )
    {
        return -1;
    }

    if(hdr.audio_format != FORMAT_PCM)
    {
        return -1;
    }
    return 0; 
}

static int check_mp3_file(QFILE fd)
{
    int64 ret = 0;
    char head[10] = {0};
    ret = ql_fseek(fd, 0, SEEK_SET);
    if(ret < 0)
    {
        return -1;
    }

    ret = ql_fread(head, sizeof(head), 1, fd);
    if(ret < sizeof(head))
    {
        ql_fseek(fd, 0, SEEK_SET);
        return -1;
    }
    if(strncmp(head, "ID3", 3) == 0)
    {
        return 0;
    }
    else if( (head[0] == 0xFF) && (head[1] & 0xF0) == 0xF0 )
    {
        return 0;
    }

    return 1;
}

static int check_amr_file(QFILE fd)
{
    return 0;
}

//Whether the audio format is correct?
int check_audio_format(char *fname)
{
    int err = CHECK_AUDIO_CORRECT;
    if (fname == NULL)
        return CHECK_AUDIO_INVALID_PARAMETER;

    char *dot = strrchr(fname, '.');
    if (dot == NULL)
        return CHECK_AUDIO_INVALID_PARAMETER;

    QFILE fd = ql_fopen(fname, "r");
    if(fd<0)
    {
        return CHECK_AUDIO_INVALID_PARAMETER;
    }
    
    if(!check_wav_file(fd))
    {
        if (strcasecmp(dot, ".wav") != 0)
        {
            err = CHECK_AUDIO_WAV_ERR;
        }
    }
    else if(!check_mp3_file(fd))
    {
        if (strcasecmp(dot, ".mp3") != 0)
        {
            err = CHECK_AUDIO_MP3_ERR;
        }
    }
    else if(!check_amr_file(fd))
    {
        if (strcasecmp(dot, ".amr") != 0)
        {
            err = CHECK_AUDIO_AMR_ERR;
        }
    }
    
    ql_fclose(fd);
    return err;
}

static void ql_audio_play_thread(void *ctx)
{
	int err = 0;
	ql_event_t event = {0};

	while(1)
	{
		err = ql_event_try_wait(&event);
		audio_demo_no_err(err, continue, "wait event failed");
        
		switch(event.id)
		{
		case QL_AUDIO_RINGTONE_PLAY:
			do
			{
				err = ql_aud_play_file_start("ring_tone.mp3", QL_AUDIO_PLAY_TYPE_LOCAL, NULL);
				if(err)
				{
					ring_tone_start = FALSE;
					break;
				}			
				ql_aud_wait_play_finish(QL_WAIT_FOREVER);		
			}while(ring_tone_start);
		break;
		}
	}	
}



#ifdef QL_APP_FEATURE_AUDIO_RECORD 
void test_poc_full_duplex(void)
{
	QL_PCM_CONFIG_T config = {0};
	int err = 0, cnt_read=0, cnt_write;
	char *buffer = NULL;
	static ql_demo_poc_t *demo_poc = NULL;

	config.channels = 1;  //单声道
	config.samplerate = 16000;

	ql_set_audio_path_earphone();
	ql_aud_set_volume(QL_AUDIO_PLAY_TYPE_VOICE, AUDIOHAL_SPK_VOL_6);  //POC mode use the param of voice call
	
	buffer = calloc(1, 1024);
	audio_demo_no_err(!buffer, return, "no memory");

	demo_poc = calloc(1, sizeof(ql_demo_poc_t));
	audio_demo_no_err(!demo_poc, return, "no memory");

	demo_poc->recorder = ql_aud_pcm_open(&config, QL_AUDIO_FORMAT_PCM, QL_PCM_BLOCK_FLAG|QL_PCM_READ_FLAG, QL_PCM_POC);
	audio_demo_no_err(!demo_poc->recorder, goto exit, "player created failed");

	demo_poc->player = ql_aud_pcm_open(&config, QL_AUDIO_FORMAT_PCM, QL_PCM_BLOCK_FLAG|QL_PCM_WRITE_FLAG, QL_PCM_POC);
	audio_demo_no_err(!demo_poc->player, goto exit, "player created failed");

	err = ql_aud_start_poc_mode(QL_POC_TYPE_FULL_DUPLEX);
	audio_demo_no_err(err, goto exit, "player created failed");

	while(1)
	{
		memset(buffer, 0, 1024);
	
		cnt_read = ql_pcm_read(demo_poc->recorder, buffer, 640);
		audio_demo_no_err((cnt_read<=0), goto exit, "read data failed");

		cnt_write = ql_pcm_write(demo_poc->player, buffer, cnt_read);
		audio_demo_no_err((cnt_write!=cnt_read), goto exit, "read data failed");	
	}

exit:
	if(buffer)
	{
		free(buffer);
	}

	ql_aud_stop_poc_mode();  //users must call "ql_aud_stop_poc_mode" then call "ql_pcm_close", otherwise may leed to some errors

	if(demo_poc)
	{
		if(demo_poc->player)
		{
			ql_pcm_close(demo_poc->player);
		}	
			
		if(demo_poc->recorder)
		{
			ql_pcm_close(demo_poc->recorder);
		}
		
		free(demo_poc);	
	}
}

void test_poc_half_duplex(void)
{
	PCM_HANDLE_T recorder = NULL;
	PCM_HANDLE_T player = NULL;

	QL_PCM_CONFIG_T config;
	void *data = NULL;
	int size, total_size = 0, cnt=0, write_cnt=0;

	config.channels = 1;  //单声道
	config.samplerate = 16000;
	config.amrwb_param.amrwb_mode = AMRWB_MODE_2385;

	data = calloc(1, 200*1024);
	if(data == NULL)
	{
		goto exit;	
	}

	ql_set_audio_path_earphone();
	ql_aud_set_volume(QL_AUDIO_PLAY_TYPE_VOICE, AUDIOHAL_SPK_VOL_6);  //POC mode use the param of voice call

	recorder = ql_aud_pcm_open(&config, QL_AUDIO_FORMAT_PCM, QL_PCM_BLOCK_FLAG|QL_PCM_READ_FLAG, QL_PCM_POC);
	audio_demo_no_err(!recorder, goto exit, "recorder created failed");

	player = ql_aud_pcm_open(&config, QL_AUDIO_FORMAT_PCM, QL_PCM_BLOCK_FLAG|QL_PCM_WRITE_FLAG, QL_PCM_POC);
	audio_demo_no_err(!player, goto exit, "player created failed");
	
	int err = ql_aud_start_poc_mode(QL_POC_TYPE_HALF_DUPLEX);
	audio_demo_no_err(err, goto exit, "poc mode start failed");

	err = ql_aud_poc_switch(QL_POC_MODE_REC);
	audio_demo_no_err(err, goto exit, "poc mode switch failed");

	//start record
	while(total_size < 200*1024)
	{
		size = ql_pcm_read(recorder, data+total_size, 1024);

		if(size <= 0)
		{
			break;
		}
		
		total_size += size;
	}

	err = ql_aud_poc_switch(QL_POC_MODE_PLAY);
	audio_demo_no_err(err, goto exit, "poc mode switch failed");

	while(write_cnt < total_size)
	{
		if(total_size - write_cnt > PACKET_WRITE_MAX_SIZE)	//单次最多可写入 PACKET_WRITE_MAX_SIZE 字节
		{
			cnt = ql_pcm_write(player, data+write_cnt, PACKET_WRITE_MAX_SIZE);
		}
		else
		{
			cnt = ql_pcm_write(player, data+write_cnt, total_size - write_cnt);
		}
		if(cnt <= 0)
		{
			QL_AUDIO_LOG("write failed");
			goto exit;	
		}
		write_cnt += cnt;
	}

	while(ql_pcm_buffer_used(player))  //in poc mode, player will not stop if not ql_aud_stop_poc_mode called
	{
		ql_rtos_task_sleep_ms(20); //wait the write buffer empty
	}	
	ql_rtos_task_sleep_ms(200);
	
exit:
	ql_aud_stop_poc_mode();  //users must call "ql_aud_stop_poc_mode" then call "ql_pcm_close", otherwise may leed to some errors
	ql_pcm_close(recorder);
	ql_pcm_close(player);
	
	if(data)
	{
		free(data);
	}
	QL_AUDIO_LOG("test done");
}
#endif

void ql_audio_app_init(void)
{
	QlOSStatus err = QL_OSI_SUCCESS;
	ql_task_t ql_audio_task = NULL;
	
    QL_AUDIO_LOG("audio demo enter");
	
    err = ql_rtos_task_create(&ql_audio_task, 4096*2, APP_PRIORITY_NORMAL, "ql_audio", ql_audio_demo_thread, NULL, 5);
	if(err != QL_OSI_SUCCESS)
    {
		QL_AUDIO_LOG("audio task create failed");
	}

	err = ql_rtos_task_create(&ql_play_task, 4096, APP_PRIORITY_NORMAL, "ql_audio", ql_audio_play_thread, NULL, 2);
	if(err != QL_OSI_SUCCESS)
    {
		QL_AUDIO_LOG("audio task create failed");
	}
}

void test_amr_stream(void)
{
	PCM_HANDLE_T PCM = NULL;
	QL_PCM_CONFIG_T config;
	void *data = NULL;
	int total_size = 0, cnt=0, write_cnt=0;

	config.channels = 1;  //单声道
	config.samplerate = 16000;
	config.amrwb_param.amrwb_mode = AMRWB_MODE_2385;

#ifdef QL_APP_FEATURE_AUDIO_RECORD	
    int size=0;

	PCM = ql_aud_pcm_open(&config, QL_AUDIO_FORMAT_AMRWB, QL_PCM_BLOCK_FLAG|QL_PCM_READ_FLAG, QL_PCM_LOCAL);
	if(PCM == NULL)
	{
		QL_AUDIO_LOG("open pcm failed");
		goto exit;		
	}
	data = malloc(100*1024);
	if(data == NULL)
	{
		goto exit;	
	}

	QL_AUDIO_LOG("start read");

//start record
	while(total_size < 10*1024)
	{
		size = ql_pcm_read(PCM, data+total_size, 41);

		if(size <= 0)
		{
			break;
		}
		
		total_size += size;
	}	

	QL_AUDIO_LOG("exit record");

	if(total_size <= 0)
	{
		QL_AUDIO_LOG("read pcm failed");
		goto exit;			
	}
	QL_AUDIO_LOG("size is %d", total_size);

	if(ql_pcm_close(PCM) != 0)
	{
		QL_AUDIO_LOG("close pcm failed");
		goto exit;			
	}
	PCM = NULL;
#endif

	PCM = ql_aud_pcm_open(&config, QL_AUDIO_FORMAT_AMRWB, QL_PCM_BLOCK_FLAG|QL_PCM_WRITE_FLAG, QL_PCM_LOCAL);
	if(PCM == NULL)
	{
		QL_AUDIO_LOG("open pcm failed");
		goto exit;		
	}

	while(write_cnt < total_size)
	{
		if(total_size - write_cnt > PACKET_WRITE_MAX_SIZE)  //单次最多可写 PACKET_WRITE_MAX_SIZE 字节
		{
			cnt = ql_pcm_write(PCM, data+write_cnt, PACKET_WRITE_MAX_SIZE);
		}
		else
		{
			cnt = ql_pcm_write(PCM, data+write_cnt, total_size - write_cnt);
		}
		if(cnt <= 0)
		{
			QL_AUDIO_LOG("write failed");
			goto exit;	
		}
		write_cnt += cnt;
	}

	ql_aud_data_done();
	ql_aud_wait_play_finish(QL_WAIT_FOREVER);
	
	QL_AUDIO_LOG("play finish");	

exit:
	if(PCM != NULL)
	{
		ql_pcm_close(PCM);
	}
	if(data != NULL)
	{
		free(data);
		data = NULL;
	}
}

#ifdef QL_APP_FEATURE_AUDIO_RECORD
void test_record_stream(void)
{
	ql_aud_config config = {0};
	int cnt = 0, total_cnt=0, err;
	config.amrwb_param.amrwb_mode = AMRWB_MODE_2385;
    
	pcm_buffer = malloc(RECORD_BUFFER_MAX);
	if(!pcm_buffer){
		return;
	}
	
/*	录音    */	
	if(ql_aud_record_stream_start_ex(&config, QL_REC_TYPE_MIC, QL_AUDIO_FORMAT_AMRWB, record_callback))
	{
		QL_AUDIO_LOG("record fail");
		goto exit;
	}
    
	ql_rtos_task_sleep_s(5);   //record 5s	
	ql_aud_record_stop();

	if(pcm_data_size <= 0){
		QL_AUDIO_LOG("data invalid");
		goto exit;			
	}

/*	读取录音文件用于播放，此处也可调用	ql_aud_play_file_start,或者ql_pcm_open+ql_pcm_write去播放    				  */
	ql_set_audio_path_speaker();
	ql_set_volume(TEST_PLAY_VOLUME);
    

	while(total_cnt < pcm_data_size)
	{
		if(pcm_data_size - total_cnt > PACKET_WRITE_MAX_SIZE)  //单次最多可播放 PACKET_WRITE_MAX_SIZE 字节
		{
			cnt = PACKET_WRITE_MAX_SIZE;   
			err = ql_aud_play_stream_start(QL_AUDIO_FORMAT_AMRWB, pcm_buffer+total_cnt, cnt, QL_AUDIO_PLAY_TYPE_LOCAL, play_callback);
		}
		else
		{
			cnt = pcm_data_size - total_cnt;
			err = ql_aud_play_stream_start(QL_AUDIO_FORMAT_AMRWB, pcm_buffer+total_cnt, cnt, QL_AUDIO_PLAY_TYPE_LOCAL, play_callback);
		}
		
		if(err < 0)
		{
			QL_AUDIO_LOG("start failed");
			goto exit;
		}
		else
		{
			QL_AUDIO_LOG("play %d bytes, total %d", cnt, total_cnt);
			total_cnt += cnt;
		}
	}

	ql_aud_data_done();	
	ql_aud_wait_play_finish(QL_WAIT_FOREVER);
	ql_aud_player_stop();
	
	QL_AUDIO_LOG("test successful");

exit:

	if(pcm_buffer){
		free(pcm_buffer);
		pcm_buffer = NULL;
		pcm_data_size = 0;
	}
}

void test_record_file(void)
{
	ql_aud_config config = {0};
	config.samplerate = 8000;
    ql_aud_adc_cfg adc_cfg={0};
    adc_cfg.adc_gain = QL_ADC_GAIN_LEVEL_12;
    ql_aud_dac_cfg dac_cfg={0};

    dac_cfg.dac_gain = 0xff;//32db

	if(ql_aud_record_file_start(TEST_RECORD_WAV_NAME, &config, QL_REC_TYPE_MIC, NULL) != QL_AUDIO_SUCCESS)
	{
		QL_AUDIO_LOG("record failed");
		return;
	}
	QL_AUDIO_LOG("record start");
	ql_rtos_task_sleep_s(3);   //record 3s
    ql_aud_set_adc_gain(&adc_cfg);
    ql_rtos_task_sleep_s(3);   //record 3s

	ql_aud_record_stop();
	QL_AUDIO_LOG("record finish, start play");
    ql_aud_set_dac_gain(&dac_cfg);

	ql_set_audio_path_speaker();
	if(ql_aud_play_file_start(TEST_RECORD_WAV_NAME, QL_AUDIO_PLAY_TYPE_LOCAL, play_callback))
	{
		QL_AUDIO_LOG("play failed");
		return;
	}

	ql_aud_wait_play_finish(QL_WAIT_FOREVER);
	ql_aud_player_stop();
	QL_AUDIO_LOG("test successful");
}
#endif
void test_mp3(void)
{
    //检测MP3文件格式是否正确,防止误将其他格式的文件命名为MP3格式
    //int err = check_audio_format(TEST_MP3_FILE_NAME);//路径为客户预置音频文件放置的路径
    //if(err)
    //{
        //QL_AUDIO_LOG("err = %d",err);
        //return;
    //}
    
	if(ql_aud_play_file_start(TEST_MP3_FILE_NAME, QL_AUDIO_PLAY_TYPE_LOCAL, play_callback))
	{
		QL_AUDIO_LOG("play failed");
		return;
	}
	ql_aud_wait_play_finish(QL_WAIT_FOREVER);

	if(ql_aud_play_file_start(TEST_MP3_FILE_NAME, QL_AUDIO_PLAY_TYPE_LOCAL, play_callback))
	{
		QL_AUDIO_LOG("play failed");
		return;
	}
	ql_aud_wait_play_finish(QL_WAIT_FOREVER);

	ql_aud_player_stop(); //播放结束，释放播放资源
	
	QL_AUDIO_LOG("test mp3 successful");
}


void test_wav(void)
{
	int cnt = 0;

    //检测WAV文件格式是否正确,防止误将其他格式的文件命名为WAV格式
    //int err = check_audio_format(TEST_WAV_FILE_NAME);//路径为客户预置音频文件放置的路径
    //if(err)
    //{
        //QL_AUDIO_LOG("err = %d",err);
        //return;
    //}

	if(ql_aud_play_file_start(TEST_WAV_FILE_NAME, QL_AUDIO_PLAY_TYPE_LOCAL, play_callback))
	{
		QL_AUDIO_LOG("play failed");
		return;
	}
	ql_aud_wait_play_finish(QL_WAIT_FOREVER);
	QL_AUDIO_LOG("play %d times ok", ++cnt);

	if(ql_aud_play_file_start(TEST_WAV_FILE_NAME, QL_AUDIO_PLAY_TYPE_LOCAL, play_callback))
	{
		QL_AUDIO_LOG("play failed");
		return;
	}
	ql_aud_wait_play_finish(QL_WAIT_FOREVER);
	
	QL_AUDIO_LOG("play %d times ok", ++cnt);
	
	ql_aud_player_stop(); //播放结束，释放播放资源
	QL_AUDIO_LOG("test wav successful");
}

void test_amr(void)
{
	int cnt = 0;

    //检测AMR文件格式是否正确,防止误将其他格式的文件命名为AMR格式
    //int err = check_audio_format(TEST_AMR_FILE_NAME);//路径为客户预置音频文件放置的路径
    //if(err)
    //{
        //QL_AUDIO_LOG("err = %d",err);
        //return;
    //}
    
	if(ql_aud_play_file_start(TEST_AMR_FILE_NAME, QL_AUDIO_PLAY_TYPE_LOCAL, play_callback))
	{
		QL_AUDIO_LOG("play failed");
	}
	ql_aud_wait_play_finish(QL_WAIT_FOREVER);
	QL_AUDIO_LOG("play %d times ok", ++cnt);

	if(ql_aud_play_file_start(TEST_AMR_FILE_NAME, QL_AUDIO_PLAY_TYPE_LOCAL, play_callback))
	{
		QL_AUDIO_LOG("play failed");
	}
	ql_aud_wait_play_finish(QL_WAIT_FOREVER);
	QL_AUDIO_LOG("play %d times ok", ++cnt);

	ql_aud_player_stop(); //播放结束，释放播放资源
	QL_AUDIO_LOG("test wav successful");
}

void test_pcm(void)
{
	PCM_HANDLE_T PCM = NULL;
	QL_PCM_CONFIG_T config;
	void *data = NULL;
	int size=0, write_cnt=0, cnt=0;
    ql_aud_dac_cfg dac_cfg={0};
    dac_cfg.dac_gain = 0xff;//32db

	config.channels = 1; //单声道
	config.samplerate = 8000;

#ifdef QL_APP_FEATURE_AUDIO_RECORD 	
	PCM = ql_pcm_open(&config, QL_PCM_BLOCK_FLAG|QL_PCM_READ_FLAG);
	if(PCM == NULL)
	{
		QL_AUDIO_LOG("open pcm failed");
		goto exit;		
	}
	data = malloc(50*1024);
	if(data == NULL)
	{
		goto exit;	
	}

	QL_AUDIO_LOG("start read");
	size = ql_pcm_read(PCM, data, 50*1024);
	if(size <= 0)
	{
		QL_AUDIO_LOG("read pcm failed");
		goto exit;			
	}
	QL_AUDIO_LOG("size is %d", size);

	if(ql_pcm_close(PCM) != 0)
	{
		QL_AUDIO_LOG("close pcm failed");
		goto exit;			
	}
	PCM = NULL;
#endif
	ql_set_audio_path_speaker();
	
	PCM = ql_pcm_open(&config, QL_PCM_BLOCK_FLAG|QL_PCM_WRITE_FLAG);
	if(PCM == NULL)
	{
		QL_AUDIO_LOG("open pcm failed");
		goto exit;		
	}

	QL_AUDIO_LOG("start write");
    ql_aud_set_dac_gain(&dac_cfg);

	while(write_cnt < size)
	{
		if(size - write_cnt > PACKET_WRITE_MAX_SIZE)  //单次最多可播放 PACKET_WRITE_MAX_SIZE 字节
		{
			cnt = ql_pcm_write(PCM, data+write_cnt, PACKET_WRITE_MAX_SIZE);
		}
		else
		{
			cnt = ql_pcm_write(PCM, data+write_cnt, size - write_cnt);
		}
		if(cnt <= 0)
		{
			QL_AUDIO_LOG("write failed");
			goto exit;	
		}
		write_cnt += cnt;
	}

	ql_aud_data_done();
	ql_aud_wait_play_finish(QL_WAIT_FOREVER);

	QL_AUDIO_LOG("play done");

	
	if(ql_pcm_close(PCM) != 0)
	{
		QL_AUDIO_LOG("close pcm failed");
		goto exit;			
	}
	PCM = NULL;

	QL_AUDIO_LOG("play finish");	

exit:
	if(PCM != NULL)
	{
		ql_pcm_close(PCM);
	}
	if(data != NULL)
	{
		free(data);
		data = NULL;
	}
}

#ifdef QL_APP_FEATURE_HEADSET_DET
void _ql_headset_det_debounce_callback(void *ctx)
{
    if( headset_det_debounce_timer == NULL || ql_rtos_timer_is_running(headset_det_debounce_timer) )
    {
        return;
    }
    ql_rtos_timer_start(headset_det_debounce_timer, QL_HEADSET_DETECT_DEBOUNCE_TIME, 1);
    QL_AUDIO_LOG("headset timer start");
}

static void _ql_headset_detect_callback(void *ctx)
{
    ql_LvlMode headsetdet_value;
    ql_event_t event;

    ql_rtos_timer_stop(headset_det_debounce_timer);

    event.id = QUEC_HEADSET_DET_EVENT_IND;

    ql_gpio_get_level(QL_HEADSET_DETECT_GPIO, &headsetdet_value);
    if( headsetdet_value == LVL_LOW )       //detect plug_in
    {
        event.param1 = DEMO_HEADSET_DETECT_PLUG_IN;
    }
    else/* headsetdet_value == LVL_HIGH */  //detect plug_out
    {
        event.param1 = DEMO_HEADSET_DETECT_PLUG_OUT;
    }

    ql_rtos_event_send(headset_det_task, &event);
}

int ql_headset_det_pin_init(void)
{
    int err;

    err = ql_pin_set_func(QL_HEADSET_DETECT_PIN, QL_HEADSET_DETECT_FUNC_GPIO);
    if( err != QL_GPIO_SUCCESS )
    {
        QL_AUDIO_LOG("headset_det err = %d", err);
        return err;
    }

    return 0;
}

int ql_headset_det_interrupt_init(void)
{
    int err;

    err = ql_int_register(QL_HEADSET_DETECT_GPIO, EDGE_TRIGGER, DEBOUNCE_EN, EDGE_BOTH, PULL_UP, _ql_headset_det_debounce_callback, NULL);
    if( err != QL_GPIO_SUCCESS )
    {
        QL_AUDIO_LOG("headset_det err = %d", err);
        return err;
    }

    ql_int_enable(QL_HEADSET_DETECT_GPIO);

    return 0;
}

static void ql_headset_det_demo_thread(void *param)
{
    QL_AUDIO_LOG("headset_det demo thread enter, param 0x%x", param);

    ql_event_t event;

    if( ql_headset_det_pin_init() != 0 )
    {
        QL_AUDIO_LOG("headset_det pin init failed");
        ql_rtos_task_delete(NULL);
    }

    if( ql_headset_det_interrupt_init() != 0 )
    {
        QL_AUDIO_LOG("headset_det interrupt init failed");
        ql_rtos_task_delete(NULL);
    }

    ql_LvlMode headsetdet_value;

    ql_gpio_get_level(QL_HEADSET_DETECT_GPIO, &headsetdet_value);
    if( headsetdet_value == LVL_LOW )       //detect plug_in
    {
        QL_AUDIO_LOG("start up headset plug_in");
        ql_set_audio_path_earphone();
    }
    else/* headsetdet_value == LVL_HIGH */  //detect plug_out
    {
        QL_AUDIO_LOG("start up headset plug_out");
        ql_set_audio_path_speaker();
    }

    while(1)
    {
        if( ql_event_try_wait(&event) != 0 )
        {
            continue;
        }

        if( event.id == QUEC_HEADSET_DET_EVENT_IND )
        {
            if( event.param1 == DEMO_HEADSET_DETECT_PLUG_IN )
            {
                QL_AUDIO_LOG("headset_det detect plug_in");
                ql_aud_player_pause();
                ql_set_audio_path_earphone();
                ql_aud_player_resume();
            }
            else if( event.param1 == DEMO_HEADSET_DETECT_PLUG_OUT )
            {
                QL_AUDIO_LOG("headset_det detect plug_out");
                ql_aud_player_pause();
                ql_set_audio_path_speaker();
                ql_aud_player_resume();
            }
        }
    }

    ql_rtos_task_delete(NULL);
}

void ql_headset_det_app_init(void)
{
    QlOSStatus err = QL_OSI_SUCCESS;

    err = ql_rtos_task_create(&headset_det_task, 1024, APP_PRIORITY_NORMAL, "ql_headset_demo", ql_headset_det_demo_thread, NULL, 1);
    if( err != QL_OSI_SUCCESS )
    {
        QL_AUDIO_LOG("headset_det demo task created failed");
    }

    err = ql_rtos_timer_create(&headset_det_debounce_timer, headset_det_task, _ql_headset_detect_callback, NULL);
    if( err != QL_OSI_SUCCESS )
    {
        QL_AUDIO_LOG("headset_det demo timer created failed");
    }

}
#endif

#if QL_APP_EXT_CODEC_FLAG

#define QL_CUR_IIC_CLK_PIN           QUEC_PIN_DNAME_I2C_M2_SCL
#define QL_CUR_IIC_SDA_PIN           QUEC_PIN_DNAME_I2C_M2_SDA

typedef struct
{
    uint8_t addr;
    uint8_t data;
} extcodecReg_t;

//This is an example for ES8311.You can modify them.
static  extcodecReg_t g_codecInitRegList[] =
{
        {0x45, 0x00},
        {0x01, 0x30},
        {0x02, 0xa0}, //MCLK prediv:6-1  mult:1,8K
        {0x03, 0x10}, //single speed
        {0x16, 0x24}, //ADC
        {0x04, 0x20}, //DAC_OSR  
        {0x05, 0x00}, //DIV_ADCCLK DIV_DACCLK
        {0x06, 0x15}, //DIV_BCK:4,
        {0x07, 0x05}, //256LRCK=MCLK  salve:inactive
        {0x08, 0xff}, //256LRCK=MCLK  salve:inactive
        {0x09, 0x0c}, //SDP_IN:IIS 16BIT
        {0x0a, 0x0c}, //SDP_OUT:IIS 16BIT
        {0x0b, 0x00}, //Power UP STAGE
        {0x0c, 0x00}, //Power UP STAGE
        {0x10, 0x1f}, //AVDD:3V3
        {0x11, 0x7f}, //internal use
        {0x00, 0x80},
        {0x0d, 0x01},
        {0x01, 0xdf},
        {0x12, 0x02},
        {0x31, 0x60},
        {0x32, 0x00},
};
static  extcodecReg_t g_codecCloseRegList[] =
{
        {0x0E, 0xFF}, //POWER DOWN ANALOG PGA/ADC
        {0x12, 0x02}, //POWER DOWN DAC
        {0x14, 0x00}, //NO INPUT,PGA:0DB
        {0x0D, 0xFA}, //POWER DOWN CIRCUITS
        {0x01, 0x00}, //clk off
};
static  extcodecReg_t g_codecrecRegList[] =
{
        {0x0e,0x02},
        {0x0f,0x44},
        {0x15,0x10},
        {0x1b,0x05},
        {0x1c,0x65},
        {0x0f,0x01},
        {0x44,0x08},
        {0x02,0x10},
        {0x14,0x17},
        {0x17,0xbf},

};
static  extcodecReg_t g_codecplayRegList[] =
{
        {0x0e,0x62},
        {0x0f,0x01},
        {0x44,0x08},
        {0x02,0x10},
        {0x13,0x00},
        {0x31,0x00},
        {0x12,0x00},
        {0x32,0xbf},
        {0x37,0x08},
};

void es8311_WriteRegList(extcodecReg_t *regList, uint16_t len)
{
    uint16_t regCount;
    ql_errcode_i2c_e ret=0;
    uint8 retry_count = 5;

    for (regCount = 0; regCount < len; regCount++)
    {
        do 
        {
            ret = ql_I2cWrite(i2c_2, ES8311_I2C_SLAVE_ADDR, regList[regCount].addr, &regList[regCount].data, 1);
            if (ret)
            {
                 QL_AUDIO_LOG("I2C write failed");
            }
            else
            {
                break;
            }        
        } while (--retry_count);
    }

}

void _ql_extcodec_stage_cfg_cb(QL_EXT_CODEC_STAGE_E stage)
{
    switch(stage)
    {
        case QL_EXT_CODEC_INIT:
                /*set i2c2 pin function*/
            ql_pin_set_func(QL_CUR_IIC_CLK_PIN, 0);
            ql_pin_set_func(QL_CUR_IIC_SDA_PIN, 0);
            ql_I2cInit(i2c_2, STANDARD_MODE);
            es8311_WriteRegList(g_codecInitRegList,sizeof(g_codecInitRegList) / sizeof(extcodecReg_t));
        break;

        case QL_EXT_CODEC_REC:
            es8311_WriteRegList(g_codecrecRegList,sizeof(g_codecrecRegList) / sizeof(extcodecReg_t));
        break;

        case QL_EXT_CODEC_PLAY:
            es8311_WriteRegList(g_codecplayRegList,sizeof(g_codecplayRegList) / sizeof(extcodecReg_t));

        break;
        case QL_EXT_CODEC_DEINIT:
            es8311_WriteRegList(g_codecCloseRegList,sizeof(g_codecCloseRegList) / sizeof(extcodecReg_t));
            ql_I2cRelease(i2c_2);
        break;
        default:
        break;      
    }
}


void ql_ext_codec_cb_init(void)
{
    ql_extcodec_info_t quec_extcodec_info;
    quec_extcodec_info.extcodec_enable=true;
    quec_extcodec_info.extcodec_cfg=_ql_extcodec_stage_cfg_cb;
    ql_aud_ext_codec_cfg(&quec_extcodec_info);
}


#endif


