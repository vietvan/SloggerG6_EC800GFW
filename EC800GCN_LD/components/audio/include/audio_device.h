/* Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */

#ifndef _AUDIO_DEVICE_H_
#define _AUDIO_DEVICE_H_

#include "osi_compiler.h"
#include "osi_byte_buf.h"
#include "audio_types.h"
#include "audio_config.h"
#include "ql_api_common.h"

OSI_EXTERN_C_BEGIN

/**
 * \brief event type send from audio device for playback
 */
typedef enum
{
    /**
     * Playback is finished. This event will be sent after all samples are
     * output through audio device.
     */
    AUDEV_PLAY_EVENT_FINISH,
} audevPlayEvent_t;

/**
 * \brief player operations
 *
 * These operations will be called by audio device.
 */
typedef struct
{
    /**
     * \brief get audio frame
     *
     * When more data are needed, audio device will call \p get_frame
     * for audio frames.
     *
     * At the end of stream, callee should return empty audio frame
     * with \p AUFRAME_FLAG_END.
     *
     * \param param     player operation context
     * \param frame     audio frame, filled by callee
     * \param array     get data in array
     * \param size      empty size
     * \return
     *      get data size
     */
    int (*get_frame)(void *param, auFrame_t *frame, void *address, unsigned size);
    /**
     * \brief handle play event from audio device
     *
     * \param param     player operation context
     * \param event     audio device play event
     */
    void (*handle_event)(void *param, audevPlayEvent_t event);
} audevPlayOps_t;

/**
 * \brief event type send from audio device for voice call Stop
 */
typedef enum
{
    /**
     * Record is finished. This event will be sent before voice call Stop
     * audio device.
     */
    AUDEV_RECORD_EVENT_FINISH,
} audevRecordEvent_t;

/**
 * \brief recorder operations
 *
 * These operations will be called by audio device.
 */
typedef struct
{
    /**
     * \brief put audio frame, shoould be processed by recorder
     *
     * \param param     recorder operation contect
     * \param frame     audio frame to be recorded
     * \return
     *      - true on success
     *      - false on error
     */
    bool (*put_frame)(void *param, const auFrame_t *frame);
    /**
     * \brief handle record event from audio device
     *
     * \param param     recorder operation context
     * \param event     audio device record event
     */
    void (*handle_event)(void *param, audevRecordEvent_t event);

} audevRecordOps_t;

/**
 * \brief function type, override keytone detect push voice data
 */
typedef void (*ktdetectHook_t)(short *data, uint16_t SampleLen);

/**
 * \brief initialize audio device
 */
#ifndef CONFIG_QUEC_PROJECT_FEATURE_AUDIO
void audevInit(void);
#else
void audevInit(uint task_prio);
#endif

/**
 * \brief reset audio device settings to default value, and clear nv
 */
void audevResetSettings(void);

/**
 * \brief set audio output device
 *
 * \param dev       audio output device
 * \return
 *      - true on success
 *      - false on failed
 */
bool audevSetOutput(audevOutput_t dev);

/**
 * \brief get current audio output device
 *
 * \return
 *      - current audio output device
 */
audevOutput_t audevGetOutput(void);

/**
 * \brief set audio input device
 *
 * \param dev       audio input device
 * \return
 *      - true on success
 *      - false on failed
 */
bool audevSetInput(audevInput_t dev);

/**
 * \brief get current audio input device
 *
 * \return
 *      - current audio input device
 */
audevInput_t audevGetInput(void);

/**
 * \brief set voice call volume
 *
 * \p vol should in [0, 100]. 0 is the minimal volume, and 100 is the
 * maximum volume.
 *
 * It is possible that the supported volume levels is not 100. And then
 * the output is the same for multiple \p vol.
 *
 * \param vol       volume in [0, 100]
 * \return
 *      - true on success
 *      - false on invalid parameter, or failed
 */
bool audevSetVoiceVolume(unsigned vol);

/**
 * \brief get voice call volume
 *
 * \return
 *      - voice call volume
 */
unsigned audevGetVoiceVolume(void);

/**
 * \brief set play volume
 *
 * \p vol should in [0, 100]. 0 is the minimal volume, and 100 is the
 * maximum volume.
 *
 * It is possible that the supported volume levels is not 100. And then
 * the output is the same for multiple \p vol.
 *
 * \param vol       volume in [0, 100]
 * \return
 *      - true on success
 *      - false on invalid parameter, or failed
 */
bool audevSetPlayVolume(unsigned vol);

/**
 * \brief get play volume
 *
 * \return
 *      - play volume
 */
unsigned audevGetPlayVolume(void);

/**
 * \brief mute or unmute audio output device
 *
 * This setting won't be stored nv, the initial value is always unmute.
 *
 * \param mute      true for mute output
 * \return
 *      - true on success
 *      - false on invalid parameter, or failed
 */
bool audevSetOutputMute(bool mute);

/**
 * \brief check whether audio output device is muted
 *
 * \return
 *      - true if audio output device is muted
 *      - false if not
 */
bool audevIsOutputMute(void);

/**
 * \brief mute/unmute uplink during voice call
 *
 * When the current voice call is finished, the property will be kept.
 * That is, when uplink is muted for one voice call, uplink will be still
 * muted at the next voice call.
 *
 * This setting won't be stored nv, the initial value is always unmute.
 *
 * \param mute      true for mute uplink of voice call
 * \return
 *      - true on success
 *      - false on failed
 */
bool audevSetVoiceUplinkMute(bool mute);

/**
 * \brief whether uplink during voice call is muted
 *
 * \return
 *      - true if voice call uplink is muted
 *      - false if voice call uplink is unmuted
 */
bool audevIsVoiceUplinkMute(void);

/**
 * \brief set record sample rate
 *
 * default value is 8000
 * when set value to 16000,record file format and sample rate
 * should be handled yourself
 *
 * \param samplerate should be 8000 or 16000.
 * \return
 *      - true on success
 *      - false on invalid parameter, or failed
 */
bool audevSetRecordSampleRate(uint32_t samplerate);

/**
 * \brief get record sample rate
 *
 * \return
 * 	 - sample rate
 */
uint32_t audevGetRecordSampleRate(void);

/**
 * \brief set local record sample time interval
 *
 * default value is 20 ms
 * max value is 20 ms,min value is 5 ms
 *
 * \param time_ms should be 5 or 10 or 20.
 * \return
 *      - true on success
 *      - false on invalid parameter, or failed
 */
bool audevSetRecordSampleInterval(uint8_t time_ms);

/**
 * \brief get record sample time interval
 *
 * \return
 * 	 - sample time interval
 */
uint8_t audevGetRecordSampleInterval(void);

/*
 *\brief Set Poc Dump mode enable
 *
 *\param en		true for enable Poc Dump
 *
 *\return 
 *      - true on success
 *      - false on invalid parameter, or failed
 */
bool audevPocDumpEnable(bool en);

/**
 * \brief check whether keytone detect is enabled
 *
 * \return
 *      - true if keytone detect is enabled
 *      - false if disabled
 */
bool audevIsKtdetectEnable(void);

/**
 * \brief enable or disable keytone detect function
 *
 * \param en      true for enable keytone detect
 * \return
 *      - true on success
 *      - false on invalid parameter, or failed
 */
bool audevSetKtdetectEnable(bool en);

/**
 * \brief register hook function for keytone detect get raw voice data
 *
 * \param func the process function when ap receive raw voice data 8K mono 16bit
 */
void audevSetKtdetectCB(ktdetectHook_t func);

#ifdef CONFIG_AUDIO_IPC_SET_TRIGGER_MODE_ENABLE
/**
 * \brief set local player Ipc trigger mode
 *
 * default mode 0,when sharemem reach empty,ipc trigger irq
 * customer mode 1,ipc trigger irq in every codec irq
 *
 * \param tmode should be 0 or 1.
 * \return
 *      - true on success
 *      - false on invalid parameter, or failed
 */
bool audevSetPlayerIpcTriggerMode(uint32_t tmode);

/**
 * \brief get player Ipc trigger mode
 *
 * \return
 * 	 - tmode
 */
uint32_t audevGetPlayerIpcTriggerMode(void);
#endif

/**
 * \brief set call mode mic gain
 *
 * \param mode should in 0,5. 0:VOICECALLNB,5:VOICECALLWB
 * \param path should in [0, 5]. 0:Handset,1:Handfree,2:Handset4P,3:Handset3P,4:BTHS,5:BTHSNrec
 * \p anaGain should in [0, 7].0:0dB,1:3dB,2:6dB,3:12dB,4:18dB,5:24dB,6:30dB,7:36dB
 * \p adcGain should in [0,15].0:Mute,1:-12dB,2:-10dB,...,6:-2dB,7:0dB,...,15:16dB
 *
 * \param anaGain  Input analog gain
 * \param adcGain  Input ADC gain.
 * \return
 *      - true set success
 *      - false set invalid parameter, or failed
 */
bool audevSetMicGain(uint8_t mode, uint8_t path, uint16_t anaGain, uint16_t adcGain);

/**
 * \brief get call mode mic gain
 *
 * \p mode should in 0,5. 0:VOICECALLNB,5:VOICECALLWB
 * \p path should in [0, 5]. 0:Handset,1:Handfree,2:Handset4P,3:Handset3P,4:BTHS,5:BTHSNrec
 * \p anaGain should in [0, 7].0:0dB,1:3dB,2:6dB,3:12dB,4:18dB,5:24dB,6:30dB,7:36dB
 * \p adcGain should in [0,15].0:Mute,1:-12dB,2:-10dB,...,6:-2dB,7:0dB,...,15:16dB
 *
 * \return
 * 	 - anaGain ,adcGain
 */
bool audevGetMicGain(uint8_t mode, uint8_t path, uint16_t *anaGain, uint16_t *adcGain);

/**
 * \brief set record mode mic gain
 *
 * \param mode should in 2,4. 2:MUSICRECORD,4:FMRECORD
 * \param path should in [1, 3]. 1:Handfree,2:Handset4P,3:Handset3P
 * \param anaGain should in [0, 7].0:0dB,1:3dB,2:6dB,3:12dB,4:18dB,5:24dB,6:30dB,7:36dB
 * \param adcGain should in [0,15].0:Mute,1:-12dB,2:-10dB,...,6:-2dB,7:0dB,...,15:16dB
 * \return
 *      - true set success
 *      - false set invalid parameter, or failed
 */
bool audevSetRecordMicGain(uint8_t mode, uint8_t path, uint16_t anaGain, uint16_t adcGain);

/**
 * \brief get call mode mic gain
 *
 * \param mode should in 2,4. 2:MUSICRECORD,4:FMRECORD
 * \param path should in [1, 3]. 1:Handfree,2:Handset4P,3:Handset3P
 * \param anaGain should in [0, 7].0:0dB,1:3dB,2:6dB,3:12dB,4:18dB,5:24dB,6:30dB,7:36dB
 * \param adcGain should in [0,15].0:Mute,1:-12dB,2:-10dB,...,6:-2dB,7:0dB,...,15:16dB
 * \return
 * 	 - anaGain ,adcGain
 */
bool audevGetRecordMicGain(uint8_t mode, uint8_t path, uint16_t *anaGain, uint16_t *adcGain);

/**
 * \brief set call mode sidetone gain
 *
 * \param mode should in 0,5. 0:VOICECALLNB,5:VOICECALLWB
 * \param path should in [0, 5]. 0:Handset,1:Handfree,2:Handset4P,3:Handset3P,4:BTHS,5:BTHSNrec
 * \param level should in [0, 15]. match with call vol0-vol15
 * \param sideGain should in [-39, 6].-39:mute,-36:-36dB,-33:-33dB...,-3:-3dB,0:0dB,3:3dB,6:6dB
 * \return
 *      - true set success
 *      - false set invalid parameter, or failed
 */
bool audevSetSideToneGain(uint8_t mode, uint8_t path, int8_t sideGain);

/**
 * \brief get call mode sidetone gain
 *
 * \param mode should in 0,5. 0:VOICECALLNB,5:VOICECALLWB
 * \param path should in [0, 5]. 0:Handset,1:Handfree,2:Handset4P,3:Handset3P,4:BTHS,5:BTHSNrec
 * \param level should in [0, 15]. match with call vol0-vol15
 * \param sideGain should in [-39, 6].-39:mute,-36:-36dB,-33:-33dB...,-3:-3dB,0:0dB,3:3dB,6:6dB
 * \return
 * 	 - sideGain -39:mute,-36:-36dB,-33:-33dB...,-3:-3dB,0:0dB,3:3dB,6:6dB
 */
bool audevGetSideToneGain(uint8_t mode, uint8_t path, int8_t *sideGain);

/**
 * \brief start voice for call
 *
 * \return
 *      - true on success
 *      - false on failed
 */
bool audevStartVoice(void);

/**
 * \brief stop voice for call
 *
 * \return
 *      - true on success
 *      - false on failed
 */
bool audevStopVoice(void);

/**
 * \brief restart voice call
 *
 * \return
 *      - true on success
 *      - false on failed
 */
bool audevRestartVoice(void);

/**
 * \brief start voice loopback
 *
 * \return
 *      - true on success
 *      - false on failed
 */
bool audevStartLoopbackVoice(audevOutput_t outdev, audevInput_t indev, unsigned volume);

/**
 * \brief stop voice loopback
 *
 * \return
 *      - true on success
 *      - false on failed
 */
bool audevStopLoopbackVoice(void);

/**
 * switch on/off voice data dump
 *  */
void audevSetVoiceDump(bool flag);

/**
 * \brief play a tone
 *
 * This will play a tone, and output to current audio output device.
 * This can't be used to send tone during voice call.
 *
 * This won't wait the specified tone duration finish, that is, it is
 * asynchronuous.
 *
 * Tone will be played with current output device.
 *
 * \param tone          tone to be played
 * \param duration      tone duration in milliseconds
 * \return
 *      - true on success
 *      - false on failed
 */
bool audevPlayTone(audevTone_t tone, unsigned duration);

/**
 * \brief stop tone play
 *
 * \return
 *      - true on success
 *      - false on failed
 */
bool audevStopTone(void);

/**
 * \brief start poc mode
 *
 * we need create a poc type audioplayer and a poc type audiorecorder before call this api
 * \param duplexmode true:full duplex mode record send and play recv false:half duplex mode play recv
 * \return
 *      - true on success
 *      - false on failed
 */
bool audevStartPocMode(bool duplexmode);

/**
 * \brief stop poc mode
 *
 * \return
 *      - true on success
 *      - false on failed
 */
bool audevStopPocMode(void);

/**
 * \brief switch poc half duplex mode
 *
 * \param mode  1:half duplex mode record send 2:half duplex mode play recv
 * \return
 *      - true switch success
 *      - false switch failed
 */
bool audevPocModeSwitch(uint8_t mode);

/**
 * \brief start PCM output
 *
 * Only stream information in \p frame will be used.
 *
 * \param play_ops      operations will be called by audio device
 * \param play_ctx      operation context
 * \param frame         audio frame, only stream information will be used
 * \return
 *      - true on success
 *      - false on failed
 */
bool audevStartPlay(const audevPlayOps_t *play_ops, void *play_ctx,
                    const auFrame_t *frame);

/**
 * \brief start PCM output
 *
 * Only stream information in \p frame will be used.
 *
 * \param type          audio playing type
 * \param play_ops      operations will be called by audio device
 * \param play_ctx      operation context
 * \param frame         audio frame, only stream information will be used
 * \return
 *      - true on success
 *      - false on failed
 */
bool audevStartPlayV2(audevPlayType_t type, const audevPlayOps_t *play_ops, void *play_ctx,
                      const auFrame_t *frame);

/**
 * \brief stop PCM output
 *
 * \return
 *      - true on success
 *      - false on failed
 */
bool audevStopPlay(void);

/**
 * \brief sustainable time of audio output buffer
 *
 * There may exist multiple output buffer in the audio output pipeline.
 * This will estimate the sustainable time in milliseconds based on buffer
 * size in output buffer.
 *
 * \return
 *      - sustainable time in milliseconds
 */
int audevPlayRemainTime(void);

/**
 * \brief start audio recording
 *
 * \param type          audio recording type
 * \param rec_ops       operations will called by audio device
 * \param rec_ctx       operation context
 * \return
 *      - true on success
 *      - false on failed
 */
bool audevStartRecord(audevRecordType_t type, const audevRecordOps_t *rec_ops, void *rec_ctx);

/**
 * \brief stop audio recording
 *
 * \return
 *      - true on success
 *      - false on failed
 */
bool audevStopRecord(void);

/**
 * \brief restart voice call
 *
 * \return
 *		- true on success
 *		- false on failed
 */
bool audevRestartVoiceRecord(void);

/**
 * \brief start audio loopback test
 *
 * In loopback test mode, input is fedback to output.
 *
 * \param outdev        output device for test mode
 * \param indev         input device for test mode
 * \param volume        volume
 * \return
 *      - true on success
 *      - false on failed
 */
bool audevStartLoopbackTest(audevOutput_t outdev, audevInput_t indev, unsigned volume);

/**
 * \brief stop audio audio loopback test
 *
 * \return
 *      - true on success
 *      - false on failed
 */
bool audevStopLoopbackTest(void);

/**
 * \brief start test mode PCM play
 *
 * This is test mode play, and should only be called for test purpose. It is
 * not for normal playback. In 8910, the size of \p buf must be 704.
 *
 * At return \p pcm can be freed or overwritten.
 *
 * \param outdev        output device
 * \param buf           PCM data with size
 * \return
 *      - true on success
 *      - false on failed
 */
bool audevStartPlayTest(audevOutput_t outdev, const osiBufSize32_t *buf);

/**
 * \brief stop test mode PCM play
 *
 * This is test mode play, and should only be called for test purpose. It is
 * not for normal playback.
 *
 * \return
 *      - true on success
 *      - false on failed
 */
bool audevStopPlayTest(void);

/**
 * Close mic and speaker
 */
void audSetLdoVB(uint32_t en);

/**
 * Get audCodec Open status
 */
bool audIsCodecOpen(void);

/**
 * headset plug depop
 */
void audHeadsetdepop_en(bool en, uint8_t mictype);

/**
 * \brief start bt voice for bt speaker
 *
 * \return
 *      - true on success
 *      - false on failed
 */
bool audevBtVoiceStart(void);

/**
 * \brief stop bt voice for bt speaker
 *
 * \return
 *      - true on success
 *      - false on failed
 */
bool audevBtVoiceStop(void);

/**
 * \brief set audio bt voice work mode
 *
 * \param workmode  btworkmode
 * \return
 *      - true on success
 *      - false on failed
 */
bool audevBtVoiceSetWorkMode(auBtVoiceWorkMode_t workmode);

/**
 *
 * set BBAT mode for CP
 *
 */
void audHeadSetBBATMode(uint8_t mictype, bool en);

/**
 * \brief get current audio bt voice work mode
 *
 * \return
 *      - current audio bt voice work mode
 */
auBtVoiceWorkMode_t audevBtVoiceGetWorkMode(void);

/**
 *
 * set BBAT mode for CP
 *
 */
void audHeadSetBBATMode(uint8_t mictype, bool en);

void audSetLoopbackGain(uint8_t itf, int outAlg, int outAna, int outDac, int outReserv, uint8_t inputType, int inAna, int inAdc, uint8_t *result);

#if defined(CONFIG_SOC_8850) && !defined(CONFIG_AUDIO_ENABLE)
void audevBMSetGpio(void);
#endif
#if defined(CONFIG_SOC_8850) && defined(CONFIG_AUDIO_VOICE_ENABLE)
bool audevPlayVoiceTone(uint8_t ToneType, uint16_t Duration);
#endif

OSI_EXTERN_C_END
#endif
