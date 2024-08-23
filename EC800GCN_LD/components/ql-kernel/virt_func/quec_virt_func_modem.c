/**  @file
  quec_virt_func_modem.c

  @brief
  This file is used to run virt function.

*/

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
#include "quec_atresp.h"
#include "ql_uart_internal.h"
#include "quec_cust_feature.h"
#include "quec_common.h"
#include "quec_at_param.h"
#include "at_param.h"
#include "ql_api_osi.h"
#include "quec_at_engine.h"
#include "quec_log.h"
//#include "quec_rtos.h"
#include "ql_api_osi.h"
#include "ql_api_usbnet.h"
#include "ql_api_stk.h"
#include "ql_api_vsim_adapt.h"
#include "at_command.h"


#define QUEC_VIRT_MODEM_FUNC_LOG(msg, ...)  custom_log("VIRT_MFUNC", msg, ##__VA_ARGS__)

//usbnet
#ifndef CONFIG_QUEC_PROJECT_FEATURE_USBNET
__attribute__((weak)) bool quec_usbnet_sendevent(uint32_t nMsg, uint32_t nParam1, uint8_t nParam2, uint8_t nParam3, uint16_t nParam4)
{
	return false;
}

__attribute__((weak)) ql_task_t *quec_get_usbnet_threadID(void)
{
	return NULL;
}

__attribute__((weak)) ql_usbnet_errcode_e ql_usbnet_get_type(ql_usbnet_type_e *usbnet_type)
{
	return QL_USBNET_EXECUTE_ERR;
}

__attribute__((weak)) ql_usbnet_errcode_e ql_usbnet_set_type(ql_usbnet_type_e usbnet_type)
{
	return QL_USBNET_EXECUTE_ERR;
}

__attribute__((weak)) void quec_exec_qnetdevctl_cmd(ql_at_cmd_t *cmd)
{
	return;
}
#endif/*CONFIG_QUEC_PROJECT_FEATURE_USBNET*/


//pbk
#ifndef CONFIG_QUEC_PROJECT_FEATURE_PBK
__attribute__((weak)) int quec_pbk_process_output_name(uint8_t *dest, uint8_t destlen, uint8_t cscs_charset, uint8_t *source, uint8_t sourcelen)
{
    return 0;
}
__attribute__((weak)) void quec_pbk_init(uint8_t nSim)
{
    return;
};
__attribute__((weak)) int quec_pbk_add_lastdialling_phone(uint8_t nSim, const char *phone_num, uint16_t len)
{
    return 0;
};
#endif/*CONFIG_QUEC_PROJECT_FEATURE_PBK*/

//stk
#ifndef CONFIG_QUEC_PROJECT_FEATURE_STK
__attribute__((weak)) void quec_stk_sim_lost(uint8_t nSim)
{
    return;
}

__attribute__((weak)) void quec_stk_replace_profile(void)
{
    return;
}

__attribute__((weak)) bool quec_stk_is_cmd_in_register_table(uint8_t cmd)
{
    return false;
}

__attribute__((weak)) void quec_stk_no_proactive(uint8_t nSim)
{
    return;
}

__attribute__((weak)) bool quec_stk_sat_plugin_take_over(const osiEvent_t *event)
{
    return false;
}
#endif/*CONFIG_QUEC_PROJECT_FEATURE_STK*/

#ifndef CONFIG_QUEC_PROJECT_FEATURE_STK_AT
__attribute__((weak)) void quec_exec_qstk_cmd(atCommand_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, ERR_AT_CME_INVALID_COMMAND_LINE, NULL, 1);
    return;
}

__attribute__((weak)) void quec_exec_qstkpd_cmd(atCommand_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, ERR_AT_CME_INVALID_COMMAND_LINE, NULL, 1);
    return;
}

__attribute__((weak)) void quec_exec_qstkstate_cmd(atCommand_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, ERR_AT_CME_INVALID_COMMAND_LINE, NULL, 1);
    return;
}

__attribute__((weak)) void quec_exec_qstkgi_cmd(atCommand_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, ERR_AT_CME_INVALID_COMMAND_LINE, NULL, 1);
    return;
}

__attribute__((weak)) void quec_exec_qstkrsp_cmd(atCommand_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, ERR_AT_CME_INVALID_COMMAND_LINE, NULL, 1);
    return;
}
#endif /* CONFIG_QUEC_PROJECT_FEATURE_STK_AT */

#ifndef CONFIG_QUEC_PROJECT_FEATURE_BIP
__attribute__((weak)) ql_stk_errcode_e ql_stk_get_bip_config(ql_stk_bip_config_s *config)
{
    if (NULL == config)
    {
        return QL_STK_INVALID_PARAM_ERR;
    }

    config->mode = QL_STK_BIP_CONFIG_MODE_DISABLE;
    config->cid = 1;
    config->status_report = false;
    return QL_STK_SUCCESS;
}
#endif /*CONFIG_QUEC_PROJECT_FEATURE_BIP*/

//vsim_adapt
#ifndef CONFIG_QUEC_PROJECT_FEATURE_VSIM_ADAPT
bool set_vsim_in_poweron;
ql_vsim_adapt_callback ql_vsim_adapt_cb = NULL;
#endif

//volte
#ifndef CONFIG_QUEC_PROJECT_FEATURE_VOLTE
__attribute__((weak)) void quec_volte_init(void)
{
    return;
};

__attribute__((weak)) int ql_volte_user_agent_init(void)
{
    return 0;
};

__attribute__((weak)) int ql_volte_usage_set(uint8_t nSim, uint8_t setting)
{
    return 0;
};

__attribute__((weak)) int ql_volte_vdp_set(uint8_t nSim, uint8_t setting)
{
    return 0;
};

//audio voice
#ifndef CONFIG_AUDIO_VOICE_ENABLE
__attribute__((weak)) int vsi_amrnb_enc_state_mem_require()
{
    return 0;
}
__attribute__((weak)) int vsi_amrnb_enc_init(void *vsi_amrnb_enc_state, int dtx)
{
    return 0;
}
__attribute__((weak)) int vsi_amrnb_enc_process(void *vsi_amrnb_enc_state,short *input_pcm, unsigned char *output, unsigned int type, unsigned int format)
{
    return 0;
}
__attribute__((weak)) int vsi_amrnb_enc_close(void *vsi_amrnb_enc_state)
{
    return 0;
}
__attribute__((weak)) int vsi_amrnb_dec_process(void *vsi_amrnb_dec_state,unsigned char *input, short *output_pcm,unsigned int type, unsigned int format)
{
    return 0;
}
__attribute__((weak)) int vsi_amrnb_dec_init(void *vsi_amrnb_dec_state)
{
    return 0;
}
__attribute__((weak)) int vsi_amrnb_dec_state_mem_require()
{
    return 0;
}
__attribute__((weak)) int vsi_amrnb_dec_close(void *vsi_amrnb_dec_state)
{
    return 0;
}

__attribute__((weak)) int vsi_amrwb_enc_init(void *vsi_amrwb_enc_state, int dtx)
{
    return 0;
}

__attribute__((weak)) int vsi_amrwb_enc_process(void *vsi_amrwb_enc_state,unsigned char *input_pcm, unsigned char *output,unsigned int mode,unsigned int format)
{
    return 0;
}

__attribute__((weak)) int vsi_amrwb_enc_state_mem_require()
{
    return 0;
}

__attribute__((weak)) int vsi_amrwb_enc_close(void *vsi_amrwb_enc_state)
{
    return 0;
}
__attribute__((weak)) int vsi_amrwb_dec_init(void *vsi_amrwb_dec_state)
{
    return 0;
}
__attribute__((weak)) int vsi_amrwb_dec_state_mem_require()
{
    return 0;
}

__attribute__((weak)) int vsi_amrwb_dec_process(void *vsi_amrwb_dec_state,unsigned char *input, short *output_pcm,unsigned int format)
{
    return 0;
}
__attribute__((weak)) int vsi_amrwb_dec_close(void *vsi_amrwb_dec_state)
{
    return 0;
}
#endif

//sms
#ifndef CONFIG_QUEC_PROJECT_FEATURE_SMS
__attribute__((weak)) int quec_sms_init_me_size(void)
{
  return 0;
}

__attribute__((weak)) int ql_sms_get_me_erase_flag(bool *meEraseFlag)
{
  return 0;
}
#endif
#endif/*CONFIG_QUEC_PROJECT_FEATURE_VOLTE*/

