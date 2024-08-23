/**  @file
  audio_demo.h

  @brief
  This file is used to define audio demo for different Quectel Project.

*/

/*================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/

#ifndef TTS_DEMO_H
#define TTS_DEMO_H


#ifdef __cplusplus
extern "C" {
#endif

#include "ql_api_tts.h"

/*========================================================================
 *  Variable Definition
 *========================================================================*/
#define QL_TTS_TASK_PRIO     25
#define QL_TTS_TASK_STACK    8*1024

/*========================================================================
 *  function Definition
 *========================================================================*/
void poc_demo_test(void);
void ql_tts_demo1_init(void);
void ql_tts_demo2_init(void);
int ql_tts_init(pUserCallback mCallback);//TTS 初始化
int ql_tts_deinit(void);//TTS去初始化，释放资源
int ql_tts_play(ql_tts_encoding_e mode, const char* string, uint len);
void ql_pcm_poc_init_ex(void);//开机后调用，只需初始化一次
int ql_pcm_play_ex(uint8_t *data, uint32_t count);//播放音频数据
void ql_pcm_play_stop_ex(void);     //中止当前播放内容
void ql_pcm_record_init_ex(void);//打开录音模式
int ql_pcm_record_ex(void *data, uint32_t count);//获取录音数据
void ql_pcm_record_deinit_ex(void);//关闭录音,切换到播放模式
void ql_pcm_poc_deinit_ex(void);//POC去初始化




#ifdef __cplusplus
} /*"C" */
#endif

#endif /* AUDIO_DEMO_H */


