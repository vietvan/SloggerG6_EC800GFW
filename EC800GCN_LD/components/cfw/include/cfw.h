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
/**
 *@file cfw.h
 *@author RDA
 *@email
 *@version
 *@data
  */
#ifndef _CFW_H_
#define _CFW_H_

#include <stdint.h>
#include <stdbool.h>

#include "cfw_cc.h"
#include "cfw_config.h"
#include "quec_proj_config.h"
#include "cfw_sms.h"
#include "cfw_utility.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CONFIG_SOC_6760
typedef struct
{
    char string[4];
    uint32_t seq;
    uint32_t time;
    uint16_t MsgId;
    uint16_t MsgLen;
    uint8_t payload[0];
} CFW_NW_PSSTATSINDLTE_INFO;
#endif

typedef struct _PM_BATTERY_INFO
{
    uint8_t nBatteryFlag;
    uint8_t nBatteryLifePercent;
    uint16_t nBatteryRealTimeVoltage;
    uint16_t nBatteryVoltage;
    uint16_t nBatteryCurrent;
    uint16_t nBatteryTemperature;
    uint8_t padding[2];
    uint32_t nBatteryLifeTime;
    uint32_t nBatteryFullLifeTime;
} PM_BATTERY_INFO;

typedef struct _CFW_EVENT
{
    uint32_t nEventId;
    uint32_t nParam1;
    uint32_t nParam2;
    uint8_t nFlag;
    uint8_t nType;
    uint16_t nUTI;
} CFW_EVENT;
typedef struct _CFW_NV_PLMN
{
    uint8_t plmn[9];
} CFW_NV_PLMN;

typedef uint32_t (*PFN_CFW_EV_PORC)(
    CFW_EVENT *pEvent,
    void *pUserData);

typedef enum
{
    CFW_IMS_SUSPEND_ALL = 0,
    CFW_IMS_SUSPEND_NORMAL,
    CFW_IMS_SUSPEND_EMREGENCY,
    CFW_IMS_SUSPEND_ENUM_FILL = 0x7fffffff
} CFW_IMS_SUSPEND_TYPE;

//#ifdef CFW_VOLTE_SUPPORT //quectel modify
#define CFW_TELNUMBER_TYPE_INVALID 0 ///< No valid information in parameter <number>.
#define CFW_TELNUMBER_TYPE_URI 1     ///< Number in <number> according to URI including the prefix specifying the URI type
#define CFW_TELNUMBER_TYPE_NORMAL 2  ///< Number
#define CFW_IMS_LOCATION_STRING_SZ 20

typedef struct
{
    uint8_t nLongiSize;
    uint8_t nLatiSize;
    uint8_t Longitude[CFW_IMS_LOCATION_STRING_SZ];
    uint8_t Latitude[CFW_IMS_LOCATION_STRING_SZ];
} CFW_IMS_LOCINFO;
//#endif  //quectel modify

typedef struct
{
    uint8_t emerg_categ_present;
    uint8_t emerg_categ_value;
} CFW_CcEmergCateg;

typedef struct _OPER_DEFAULT_APN_INFO
{
    const uint8_t OperatorId[6];
    const char *Defaultapn;
} OPER_DEFAULT_APN_INFO;

typedef struct _OPER_DEFAULT_APN_INFO_V2
{
    uint8_t OperatorId[6];
    uint8_t Defaultapn[10];
} OPER_DEFAULT_APN_INFO_V2;

typedef struct
{
    //UINT16 mode;//1:gsm/2:lte
    uint16_t band;
    uint16_t band_indicator;
    uint32_t channel; //dlarfcn
    uint16_t bw;
    int16_t pwr; //pcl
    uint16_t TxRxmode;
    uint16_t TxRx;
    uint16_t onoff;
    uint16_t reserved[3];
} CFW_NW_NST_TX_PARA_CONFIG;

#define BAL_PROC_DEFAULT 0
#define BAL_PROC_COMPLETE 1
#define BAL_PROC_CONTINUE 2
#define BAL_PROC_NOT_CONSUME 4
#define BAL_PROC_DONT_FREE_MEM 8 ///<  Tell CFW don't free the EVETN struct.
#define BAL_PROC_UNKOWN_ERR 0

#define API_SIM_EF_ECC 41
#define API_USIM_EF_ECC 107

/*! \brief The CFW_EventName function get eventname by event id
\param [in] id  EVENT ID
\return
EventName
*/

int32_t CFW_StartNstMode(CFW_SIM_ID nSimID);
int32_t CFW_StopNstMode(CFW_SIM_ID nSimID);
int32_t CFW_NstConfig(CFW_NW_NST_TX_PARA_CONFIG *nst_para_config, CFW_SIM_ID nSimID);
int32_t CFW_NstGetrssi(void);
const char *CFW_EventName(
    uint32_t id);
/*! \brief The CFW_SetEventProc function installs an application-defined event procedure for CFW response events or indicator events.

\param [in] nServiceType Identifier of CFW Service Type. This parameter can be one of the following value:
\rst
+--------------------+-----------------------------------------------------+
|    nServiceType    |                     Description                     |
+====================+=====================================================+
| CFW_SRV_NW_EV_RSP  | Indicate the response event of net work  management |
+--------------------+-----------------------------------------------------+
| CFW_SRV_CC_EV_RSP  | Indicate the response event of CC management events |
+--------------------+-----------------------------------------------------+
| CFW_SRV_SS_EV_RSP  | Indicate the response event of SS management events |
+--------------------+-----------------------------------------------------+
| CFW_SRV_SIM_EV_RSP | Indicate the response event of SIM management       |
+--------------------+-----------------------------------------------------+
| CFW_SRV_SMS_EV_RSP | Indicate the response event of SMS management       |
+--------------------+-----------------------------------------------------+
| CFW_SRV_EV_IND     | Indicate the indication event.                      |
+--------------------+-----------------------------------------------------+
\endrst
\param [in] pUserData Pointer to user data. 用户的数据指针,该参数会传入注册的EventProc 函数去处理.
\param [in] pfnEvProc Pointer to the function to be notified when the CFW notification event occur, see EventProc for more. 事件处理函数,当CFW事件到来时会调用该注册的函数.

\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+-------------------------+--------------------------------------------+
|       Error Code        |                Description                 |
+=========================+============================================+
| ERR_CFW_INVALID_SERVICE | The input parameter nServiceId is invalid. |
+-------------------------+--------------------------------------------+
\endrst
*/
uint32_t CFW_SetEventProc(
    uint8_t nServiceId,
    void *pUserData,
    PFN_CFW_EV_PORC pfnRspEvProc);

uint32_t CFW_BalInit(void);
/*! \brief The CFW_SetEventProc function installs an application-defined event procedure for CFW response events or indicator events.

\param [in] nServiceType  Identifier of CFW Service Type. This parameter can be one of the following value:
\rst
+--------------------+-----------------------------------------------------+
|    nServiceType    |                     Description                     |
+====================+=====================================================+
| CFW_SRV_NW_EV_RSP  | Indicate the response event of net work  management |
+--------------------+-----------------------------------------------------+
| CFW_SRV_CC_EV_RSP  | Indicate the response event of CC management events |
+--------------------+-----------------------------------------------------+
| CFW_SRV_SS_EV_RSP  | Indicate the response event of SS management events |
+--------------------+-----------------------------------------------------+
| CFW_SRV_SIM_EV_RSP | Indicate the response event of SIM management       |
+--------------------+-----------------------------------------------------+
| CFW_SRV_SMS_EV_RSP | Indicate the response event of SMS management       |
+--------------------+-----------------------------------------------------+
| CFW_SRV_EV_IND     | Indicate the indication event.                      |
+--------------------+-----------------------------------------------------+
\endrst
\param [in] pUserData Pointer to user data. 用户的数据指针,该参数会传入注册的EventProc 函数去处理.
\param [in] pfnEvProc Pointer to the function to be notified when the CFW notification event occur, see EventProc for more. 事件处理函数,当CFW事件到来时会调用该注册的函数.

\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+-------------------------+--------------------------------------------+
|       Error Code        |                Description                 |
+=========================+============================================+
| ERR_CFW_INVALID_SERVICE | The input parameter nServiceId is invalid. |
+-------------------------+--------------------------------------------+
\endrst
*/
uint32_t CFW_BalSetEventPorc(
    void *pUserData,
    PFN_CFW_EV_PORC pfnProc);

/*! \brief The CFW_IsFreeUTI function checks the specific UTI is free or not.

\param [in] nUTI  Specify the UTI to check.
\param [in] nServiceId  Identifier of CFW module. This parameter can be one of the following value:
\rst
+-------------------+---------------------------------------------------+
|    nServiceId     |                    Description                    |
+===================+===================================================+
| CFW_NW_SRV_ID     | Indicate the Network management.                  |
+-------------------+---------------------------------------------------+
| CFW_SIM_SRV_ID    | Indicate the SIM management.                      |
+-------------------+---------------------------------------------------+
| CFW_CM_SRV_ID     | Indicate the Call related SS and Call management. |
+-------------------+---------------------------------------------------+
| CFW_SS_SRV_ID     | Indicate the Call Indented SS management.         |
+-------------------+---------------------------------------------------+
| CFW_SMS_MO_SRV_ID | Indicate the SMS management.                      |
+-------------------+---------------------------------------------------+
| CFW_SMS_MT_SRV_ID | Indicate the SIM management.                      |
+-------------------+---------------------------------------------------+
\endrst

\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+----------------------------+-------------------------------------------------------------------+
|         Error Code         |                            Description                            |
+============================+===================================================================+
| ERR_CFW_NOT_EXIST_FREE_UTI | The specified input nUTI is no a available free UTI in the system |
+----------------------------+-------------------------------------------------------------------+
\endrst
*/
uint32_t CFW_IsFreeUTI(
    uint16_t nUTI,
    uint32_t nServiceId);
/*! \brief The CFW_GetFreeUTI function retrieves the free UTI.

\param [in] pUTI Pointer to uint8_t type to retrieve the free UTI.
\param [in] nServiceId  Identifier of CFW module. This parameter can be one of the following value:
\rst
+-------------------+---------------------------------------------------+
|    nServiceId     |                    Description                    |
+===================+===================================================+
| CFW_NW_SRV_ID     | Indicate the Network management.                  |
+-------------------+---------------------------------------------------+
| CFW_SIM_SRV_ID    | Indicate the SIM management.                      |
+-------------------+---------------------------------------------------+
| CFW_CM_SRV_ID     | Indicate the Call related SS and Call management. |
+-------------------+---------------------------------------------------+
| CFW_SS_SRV_ID     | Indicate the Call Indented SS management.         |
+-------------------+---------------------------------------------------+
| CFW_SMS_MO_SRV_ID | Indicate the SMS management.                      |
+-------------------+---------------------------------------------------+
| CFW_SMS_MT_SRV_ID | Indicate the SIM management.                      |
+-------------------+---------------------------------------------------+
\endrst

\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+----------------------------+------------------------------------------+
|         Error Code         |               Description                |
+============================+==========================================+
| ERR_CFW_INVALID_PARAMETER  | The input parameter is invalid.          |
+----------------------------+------------------------------------------+
| ERR_CFW_NOT_EXIST_FREE_UTI | No any available free UTI in the system. |
+----------------------------+------------------------------------------+
\endrst
*/
uint32_t CFW_GetFreeUTI(
    uint32_t nServiceId,
    uint8_t *pUTI);

bool CFW_BlockEvent(
    uint32_t event);

bool CFW_UnBlockEvent(
    uint32_t event);
/*! \brief The CFW_IsServiceAvailable function detects wether the service is available.

\param [in] nServiceId  Identifier of CFW module. This parameter can be one of the following value:
\rst
+-------------------+---------------------------------------------------+
|    nServiceId     |                    Description                    |
+===================+===================================================+
| CFW_NW_SRV_ID     | Indicate the Network management.                  |
+-------------------+---------------------------------------------------+
| CFW_SIM_SRV_ID    | Indicate the SIM management.                      |
+-------------------+---------------------------------------------------+
| CFW_CM_SRV_ID     | Indicate the Call related SS and Call management. |
+-------------------+---------------------------------------------------+
| CFW_SS_SRV_ID     | Indicate the Call Indented SS management.         |
+-------------------+---------------------------------------------------+
| CFW_SMS_MO_SRV_ID | Indicate the SMS management.                      |
+-------------------+---------------------------------------------------+
| CFW_SMS_MT_SRV_ID | Indicate the SIM management.                      |
+-------------------+---------------------------------------------------+
\endrst
\param [out] pAvailable Pointer to uint8_t type to detect the specified service is available. This parameter can be one of the following value returned:
\rst
+-----------------+-------------+
|   nServiceId    | Description |
+=================+=============+
| CFW_NW_PS_START |             |
+-----------------+-------------+
| CFW_SIM_INIT_OK | SAT         |
+-----------------+-------------+
| CFW_SIM_INIT_OK |             |
+-----------------+-------------+
\endrst

\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+---------------------------+-------------------------------------------+
|        Error Code         |                Description                |
+===========================+===========================================+
| ERR_CFW_INVALID_PARAMETER | The input parameter pAvailable is NULL    |
+---------------------------+-------------------------------------------+
| ERR_CFW_INVALID_SERVICE   | The input parameter nServiceId is invalid |
+---------------------------+-------------------------------------------+
\endrst
*/
uint32_t CFW_IsServiceAvailable(
    uint32_t nServiceId,
    uint8_t *pAvailable);
/*! \brief The CFW_SendReqEvent function sends the request message to CFW layer.

\param [in] nEventId Specify the identifier of the request event to CFW layer.
\param [in] nParam1 The request event associated parameter.
\param [in] nParam2 The request event associated parameter.
\param [in] nUTI    Specify the UTI (Unique Transaction Identifier) operation, which support multi-application in parallel. This parameter will
be return in notification event (see below). You also can get a free UTI by CFW_GetFreeUTI function if you application is a dynamical
module. This parameter should be less than 255 for all upper layer application.

\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+-------------------------+--------------------------------------------+
|       Error Code        |                Description                 |
+=========================+============================================+
| ERR_CFW_INVALID_SERVICE | The input parameter nServiceId is invalid. |
+-------------------------+--------------------------------------------+
\endrst
*/
uint32_t CFW_SendReqEvent(
    uint32_t nEventId,
    uint32_t nParam1,
    uint32_t nParam2,
    uint8_t nUTI);

#define CFW_CONTROL_CMD_POWER_ON 0
#define CFW_CONTROL_CMD_POWER_OFF 1
#define CFW_CONTROL_CMD_RESTART 2
#define CFW_CONTROL_CMD_RESET 3
#define CFW_CONTROL_CMD_RF_OFF 4

typedef struct _CFW_SPN_INFO
{
    uint8_t nSPNameLen;
    uint8_t nSPNDisplayFlag;
    uint8_t nSpnName[17];
} CFW_SPN_INFO;

void CFW_nvmWriteStatic(CFW_SIM_ID nSimID);

uint32_t CFW_ShellControl(
    uint8_t nCommand);
uint32_t CFW_ShellControl_V2(uint16_t nUTI, uint8_t nCommand);
uint32_t CFW_ISRNotifySimStatus(uint8_t iSlotStatus, CFW_SIM_ID nSimID);
//
// Configuration(CFG)  Management
//

// General Configuration
/*! \brief The CFW_CfgSetPhoneFunctionality function sets the level of functionality.

\param [in] nLevel Specify the functionality level. This parameter can be one of the following value:
\rst
+--------+--------------------------------------------------------+
| nLevel |                      Description                       |
+========+========================================================+
|        | Minimum functionality (SLEEP mode).The GSM module will |
| 0      | disconnects any call in progress.The SLEEP mode starts |
|        | after remaining network activities are terminated.     |
+--------+--------------------------------------------------------+
| 1      | Full functionality.                                    |
+--------+--------------------------------------------------------+
| 2      | Disable transmit RF circuits only.                     |
+--------+--------------------------------------------------------+
| 3      | Disable receive RF circuits only.                      |
+--------+--------------------------------------------------------+
| 4      | Disable both transmit and receive RF circuits          |
+--------+--------------------------------------------------------+
|        | Reserved for other manufacturer-defined states between |
| 5-127  | minimum and full functionality.                        |
+--------+--------------------------------------------------------+
\endrst
\param [in] nRst Specify the functionality level. This parameter can be one of the following value:
\rst
+------+----------------------------------------------------------------+
| nRst |                          Description                           |
+======+================================================================+
|      | The <rst> parameter can only be used if +CFUN=1.Due to the     |
| 0    | command syntax, you need to enter <fun>, followed by <rst>,    |
|      | where <fun> is only a placeholder and has no effect.           |
+------+----------------------------------------------------------------+
|      | ME resets and restarts to full functionality. After reset and  |
|      | restart, PIN 1 authentication is necessary. Therefore, you are |
| 1    | required to use AT+CPIN again.If autobauding is enabled it is  |
|      | recommended to wait 3 to 5 seconds before entering the first   |
|      | AT command.                                                    |
+------+----------------------------------------------------------------+
\endrst

\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+---------------------------+---------------------------------+
|        Error Code         |           Description           |
+===========================+=================================+
| ERR_CFW_INVALID_PARAMETER | The input parameter is invalid. |
+---------------------------+---------------------------------+
| ERR_CME_SIM_PIN_REQUIRED  | SIM Request PIN                 |
+---------------------------+---------------------------------+
\endrst
*/
uint32_t CFW_CfgSetPhoneFunctionality(
    uint8_t nLevel,
    uint8_t nRst);
/*! \brief The CFW_CfgGetPhoneFunctionality function gets the level of functionality.

\param [out] pLevel  Specify the functionality level. This parameter can be one of the following value:
\rst
+--------+--------------------------------------------------------+
| nLevel |                      Description                       |
+========+========================================================+
|        | Minimum functionality (SLEEP mode).The GSM module will |
| 0      | disconnects any call in progress.The SLEEP mode starts |
|        | after remaining network activities are terminated.     |
+--------+--------------------------------------------------------+
| 1      | Full functionality.                                    |
+--------+--------------------------------------------------------+
| 2      | Disable transmit RF circuits only.                     |
+--------+--------------------------------------------------------+
| 3      | Disable receive RF circuits only.                      |
+--------+--------------------------------------------------------+
| 4      | Disable both transmit and receive RF circuits          |
+--------+--------------------------------------------------------+
|        | Reserved for other manufacturer-defined states between |
| 5-127  | minimum and full functionality.                        |
+--------+--------------------------------------------------------+
\endrst

\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+---------------------------+---------------------------------+
|        Error Code         |           Description           |
+===========================+=================================+
| ERR_CFW_INVALID_PARAMETER | The input parameter is invalid. |
+---------------------------+---------------------------------+
\endrst
*/
uint32_t CFW_CfgGetPhoneFunctionality(
    uint8_t *pLevel);
/*! \brief The CFW_CfgStoreUserProfile function stores current configuration to user defined profile.

\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+---------------------------+---------------------------------+
|        Error Code         |           Description           |
+===========================+=================================+
| ERR_CFW_INVALID_PARAMETER | The input parameter is invalid. |
+---------------------------+---------------------------------+
\endrst
*/
uint32_t CFW_CfgStoreUserProfile(void);
/*! \brief The CFW_CfgGetUserProfile function get current stores configuration from user defined profile.

\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+---------------------------+---------------------------------+
|        Error Code         |           Description           |
+===========================+=================================+
| ERR_CFW_INVALID_PARAMETER | The input parameter is invalid. |
+---------------------------+---------------------------------+
\endrst
*/
uint32_t CFW_CfgGetUserProfile(void);
/*! \brief The CFW_CfgRestoreFactoryProfile function restores current configuration to user defined profile.

\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+---------------------------+---------------------------------+
|        Error Code         |           Description           |
+===========================+=================================+
| ERR_CFW_INVALID_PARAMETER | The input parameter is invalid. |
+---------------------------+---------------------------------+
\endrst
*/
uint32_t CFW_CfgRestoreFactoryProfile(void);

#define CFW_CHSET_UCS2 0
#define CFW_CHSET_GSM 1

/*! \brief function informs TA which character set <chset> is used by the TE. TA is then able to convert character strings correctly between TE and ME
character sets.

\param [out] chset    Specify the character set, this parameter can be one of the following value:
\rst
+----------------+-------+-------------------------------------------------------------------------------------------------+
|     chset      | value |                                           Description                                           |
+================+=======+=================================================================================================+
| CFW_CHSET_UCS2 | 0     | 16-bit universal multiple-octet coded character set (ISO/IEC10646 [32]);                        |
|                |       | UCS2 character strings are converted to hexadecimal numbers from 0000 to FFFF;                  |
|                |       | e.g."004100620063" equals three 16-bit characters with decimal values 65, 98 and 99, $(AT R97)$ |
+----------------+-------+-------------------------------------------------------------------------------------------------+
| CFW_CHSET_GSM  | 1     | GSM default alphabet (GSM 03.38 subclause 6.2.1);                                               |
|                |       | Note:This setting may cause software flow control problems due to values of XON/XOFF characters |
+----------------+-------+-------------------------------------------------------------------------------------------------+
\endrst
\return \a ERR_SUCCESS on success, or error code
                    - \a    ERR_CFW_INVALID_PARAMETER  The input parameter(nFac) is invalid.
*/
uint32_t CFW_CfgGetCharacterSet(
    uint8_t *chset);

/*! \brief function informs TA which character set <chset> is used by the TE. TA is then able to convert character strings correctly between TE and ME
character sets.

\param [in] chset    Specify the character set, this parameter can be one of the following value:
\rst
+----------------+-------+-------------------------------------------------------------------------------------------------+
|     chset      | value |                                           Description                                           |
+================+=======+=================================================================================================+
| CFW_CHSET_UCS2 | 0     | 16-bit universal multiple-octet coded character set (ISO/IEC10646 [32]);                        |
|                |       | UCS2 character strings are converted to hexadecimal numbers from 0000 to FFFF;                  |
|                |       | e.g."004100620063" equals three 16-bit characters with decimal values 65, 98 and 99, $(AT R97)$ |
+----------------+-------+-------------------------------------------------------------------------------------------------+
| CFW_CHSET_GSM  | 1     | GSM default alphabet (GSM 03.38 subclause 6.2.1);                                               |
|                |       | Note:This setting may cause software flow control problems due to values of XON/XOFF characters |
+----------------+-------+-------------------------------------------------------------------------------------------------+
\endrst
\return \a ERR_SUCCESS on success, or error code
                    - \a    ERR_CFW_INVALID_PARAMETER  The input parameter(nFac) is invalid.
*/
uint32_t CFW_CfgSetCharacterSet(
    uint8_t chset);

typedef struct _CFW_IND_EVENT_INFO
{
    uint32_t bc : 3;
    uint32_t sq : 3;
    uint32_t nw_srv : 1;
    uint32_t sms_receive : 1;
    uint32_t call : 1;
    uint32_t roam : 1;
    uint32_t sms_full : 1;
    uint32_t sounder : 1;
    uint32_t bit_err : 8;
} CFW_IND_EVENT_INFO;

typedef enum
{
    /// Chip product ID
    CFW_SYS_CHIP_PROD_ID,

    /// Chip bond ID
    CFW_SYS_CHIP_BOND_ID,

    /// Chip metal ID
    CFW_SYS_CHIP_METAL_ID,

    /// Chip ID whole
    CFW_SYS_CHIP_ID_WHOLE,

    CFW_SYS_CHIP_ID_ENUM_FILL = 0x7fffffff

} CFW_SYS_CHIP_ID_T;

/*! \brief function sets the specified indicator event to report.

\param [in] ind   Identifier of CFW Service. This parameter can be any combination of the following value:
\rst
+-------------------------------+-----------------------------------+----------------------------------------------------------------------------------------------+
|              ind              |        Corresponding event        |                                         Description                                          |
+===============================+===================================+==============================================================================================+
| CFW_CFG_IND_BC_EVENT          | EV_PM_BC_IND                      | battery charge level report, see PM_GetBatteryInfo for more.                                 |
+-------------------------------+-----------------------------------+----------------------------------------------------------------------------------------------+
| CFW_CFG_IND_SQ_EVENT          | EV_CFW_NW_SIGNAL_QUALITY_IND      | signal quality report                                                                        |
+-------------------------------+-----------------------------------+----------------------------------------------------------------------------------------------+
| CFW_CFG_IND_NW_EVENT          | EV_CFW_NW_REG_STATUS_IND,type = 0 | Network service availability                                                                 |
+-------------------------------+-----------------------------------+----------------------------------------------------------------------------------------------+
| CFW_CFG_IND_SOUNDER_EVENT     | EV_CFW_CC_CALL_INFO_IND           | sounder activity (0-1)                                                                       |
+-------------------------------+-----------------------------------+----------------------------------------------------------------------------------------------+
| CFW_CFG_IND_SMS_RECEIVE_EVENT | EV_CFW_NEW_SMS_IND , type=0       | new message received.                                                                        |
+-------------------------------+-----------------------------------+----------------------------------------------------------------------------------------------+
| CFW_CFG_IND_CALL_EVENT        | EV_CFW_CC_CALL_INFO_IND           | call in progress                                                                             |
+-------------------------------+-----------------------------------+----------------------------------------------------------------------------------------------+
| CFW_CFG_IND_VOX_EVENT         | TODO...                           | transmit activated by voice activity                                                         |
+-------------------------------+-----------------------------------+----------------------------------------------------------------------------------------------+
| CFW_CFG_IND_ROAM_EVENT        | EV_CFW_NW_REG_STATUS_IND,type = 1 | roaming indicator                                                                            |
+-------------------------------+-----------------------------------+----------------------------------------------------------------------------------------------+
| CFW_CFG_IND_SMS_FULL_EVENT    | EV_CFW_SMS_INFO_IND               | a short message memory storage in the MT has become full, or memory locations are available. |
+-------------------------------+-----------------------------------+----------------------------------------------------------------------------------------------+
| CFW_CFG_IND_BIT_ERR_EVENT     | EV_CFW_NW_SIGNAL_QUALITY_IND      | signal bit error report                                                                      |
+-------------------------------+-----------------------------------+----------------------------------------------------------------------------------------------+
\endrst
\return \a ERR_SUCCESS on success, or error code
                    - \a    ERR_CFW_INVALID_PARAMETER  The input parameter(nFac) is invalid.
*/
uint32_t CFW_CfgSetIndicatorEvent(
    uint16_t ind,
    CFW_SIM_ID nSimID);

/*! \brief function retrieves the specified indicator event.

\param [out] ind   Identifier of CFW Service. This parameter can be any combination of the following value:
\rst
+-------------------------------+-----------------------------------+----------------------------------------------------------------------------------------------+
|              ind              |        Corresponding event        |                                         Description                                          |
+===============================+===================================+==============================================================================================+
| CFW_CFG_IND_BC_EVENT          | EV_PM_BC_IND                      | battery charge level report, see PM_GetBatteryInfo for more.                                 |
+-------------------------------+-----------------------------------+----------------------------------------------------------------------------------------------+
| CFW_CFG_IND_SQ_EVENT          | EV_CFW_NW_SIGNAL_QUALITY_IND      | signal quality report                                                                        |
+-------------------------------+-----------------------------------+----------------------------------------------------------------------------------------------+
| CFW_CFG_IND_NW_EVENT          | EV_CFW_NW_REG_STATUS_IND,type = 0 | Network service availability                                                                 |
+-------------------------------+-----------------------------------+----------------------------------------------------------------------------------------------+
| CFW_CFG_IND_SOUNDER_EVENT     | EV_CFW_CC_CALL_INFO_IND           | sounder activity (0-1)                                                                       |
+-------------------------------+-----------------------------------+----------------------------------------------------------------------------------------------+
| CFW_CFG_IND_SMS_RECEIVE_EVENT | EV_CFW_NEW_SMS_IND , type=0       | new message received.                                                                        |
+-------------------------------+-----------------------------------+----------------------------------------------------------------------------------------------+
| CFW_CFG_IND_CALL_EVENT        | EV_CFW_CC_CALL_INFO_IND           | call in progress                                                                             |
+-------------------------------+-----------------------------------+----------------------------------------------------------------------------------------------+
| CFW_CFG_IND_VOX_EVENT         | TODO...                           | transmit activated by voice activity                                                         |
+-------------------------------+-----------------------------------+----------------------------------------------------------------------------------------------+
| CFW_CFG_IND_ROAM_EVENT        | EV_CFW_NW_REG_STATUS_IND,type = 1 | roaming indicator                                                                            |
+-------------------------------+-----------------------------------+----------------------------------------------------------------------------------------------+
| CFW_CFG_IND_SMS_FULL_EVENT    | EV_CFW_SMS_INFO_IND               | a short message memory storage in the MT has become full, or memory locations are available. |
+-------------------------------+-----------------------------------+----------------------------------------------------------------------------------------------+
| CFW_CFG_IND_BIT_ERR_EVENT     | EV_CFW_NW_SIGNAL_QUALITY_IND      | signal bit error report                                                                      |
+-------------------------------+-----------------------------------+----------------------------------------------------------------------------------------------+
\endrst
\return \a ERR_SUCCESS on success, or error code
                    - \a    ERR_CFW_INVALID_PARAMETER  The input parameter(nFac) is invalid.
*/
uint32_t CFW_CfgGetIndicatorEvent(
    uint16_t *ind,
    CFW_SIM_ID nSimID);

/*! \brief function sets the indication related information.

\param [in] pIndInfo    Pointer to CFW_IND_EVENT_INFO structure to retrieve the indicator information.
\param [in] nSimID      SIM ID
\return \a ERR_SUCCESS on success, or error code
                    - \a    ERR_CFW_INVALID_PARAMETER  The input parameter(nFac) is invalid.
*/
uint32_t CFW_CfgSetIndicatorEventInfo(
    CFW_IND_EVENT_INFO *pIndInfo,
    CFW_SIM_ID nSimID);

/*! \brief function retrieves the indication set by CFW_CfgSetIndicatorEvent

\param [out] pIndInfo    Pointer to CFW_IND_EVENT_INFO structure to retrieve the indicator information.
\param [in]  nSimID      SIM ID
\return \a ERR_SUCCESS on success, or error code
                    - \a    ERR_CFW_INVALID_SERVICE The input parameter nServiceId is invalid.
*/
uint32_t CFW_CfgGetIndicatorEventInfo(
    CFW_IND_EVENT_INFO *pIndInfo,
    CFW_SIM_ID nSimID);

uint32_t CFW_SetQualReport(
    bool bStart,
    CFW_SIM_ID nSimID);

uint32_t CFW_CfgSetFirstPowerOnFlag(
    bool bFPO);

uint32_t CFW_CfgCheckFirstPowerOnFlag(
    bool *pbFPO);

/*! \brief function retrieves the presentation of the event reporting.

\param [out] pMer  Pointer to UINT16 to contain the presentation of the event reporting. This parameter can be one of the following value:bit0-bit1 : use CFW_CFG_CMER_MODE(mer) to get mode.
- \a  bit0-bit1 : use CFW_CFG_CMER_MODE(mer) to get mode.
\rst
+------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------+
| mode value |                                                                          Description                                                                          |
+============+===============================================================================================================================================================+
| 0          | buffer unsolicited result codes in the TA; if TA result code buffer is full, codes can be buffered in some other place or the oldest ones can be discarded    |
+------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 1          | discard unsolicited result codes when TA TE link is reserved (e.g. in on line data mode); otherwise forward them directly to the TE                           |
+------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 2          | buffer unsolicited result codes in the TA when TA TE link is reserved (e.g. in on line data mode) and flush them to the TE after reservation;                 |
|            | otherwise forward them directly to the TE                                                                                                                     |
+------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 3          | forward unsolicited result codes directly to the TE; TA TE link specific inband technique used to embed result codes and data when TA is in on line data mode |
+------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------+
\endrst
- \a  bit 2-bit3: use CFW_CFG_CMER_KEYP(mer) to get keyp.
\rst
+------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Keyp value |                                                                         Description                                                                          |
+============+==============================================================================================================================================================+
| 0          | no keypad event reporting                                                                                                                                    |
+------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 1          | keypad event reporting using result code +CKEV: <key>,<press>. <key> indicates the key (refer IRA values defined in table in subclause "Keypad control       |
|            | +CKPD") and <press> if the key is pressed or released (1 for pressing and 0 for releasing). Only those key pressings, which are not caused by +CKPD shall be |
|            | indicated by the TA to the TE.                                                                                                                               |
+------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 2          | keypad event reporting using result code +CKEV: <key>,<press>. All key pressings shall be directed from TA to TE.                                            |
+------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------+
\endrst
- \a  bit4-bit5:use CFW_CFG_CMER_DISP(mer) to get Disp.
\rst
+------------+------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Disp value |                                                                        Description                                                                         |
+============+============================================================================================================================================================+
| 0          | no keypad event reporting                                                                                                                                  |
+------------+------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 1          | display event reporting using result code +CDEV: <elem>,<text>. <elem> indicates the element order number (as specified for +CDIS) and <text> is the new   |
|            | value of text element. Only those display events, which are not caused by +CDIS shall be indicated by the TA to the TE. Character set used in <text> is as |
|            | specified by command Select TE Character Set +CSCS                                                                                                         |
+------------+------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 2          | display event reporting using result code +CDEV: <elem>,<text>. All display events shall be directed from TA to TE. Character set used in <text> is as     |
|            | specified by command Select TE Character Set +CSCS                                                                                                         |
+------------+------------------------------------------------------------------------------------------------------------------------------------------------------------+
\endrst
- \a  bit6-bit7:use CFW_CFG_CMER_IND(mer) to get Ind.
\rst
+-----------+--------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Ind value |                                                                         Description                                                                          |
+===========+==============================================================================================================================================================+
| 0         | no indicator event reporting                                                                                                                                 |
+-----------+--------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 1         | indicator event reporting using result code +CIEV: <ind>,<value>. <ind> indicates the indicator order number (as specified for +CIND) and <value> is the new |
|           | value of indicator. Only those indicator events, which are not caused by +CIND shall be indicated by the TA to the TE                                        |
+-----------+--------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 2         | indicator event reporting using result code +CIEV: <ind>,<value>. All indicator events shall be directed from TA to TE                                       |
+-----------+--------------------------------------------------------------------------------------------------------------------------------------------------------------+
\endrst
- \a  bit8-bit9:use CFW_CFG_CMER_BFR(mer) to get bfr.
\rst
+-----------+--------------------------------------------------------------------------------------------------------------------------------------------------------+
| Bfr value |                                                                      Description                                                                       |
+===========+========================================================================================================================================================+
| 0         | TA buffer of unsolicited result codes defined within this command is cleared when <mode> 1...3 is entered                                              |
+-----------+--------------------------------------------------------------------------------------------------------------------------------------------------------+
| 1         | TA buffer of unsolicited result codes defined within this command is flushed to the TE when <mode> 1...3 is entered (OK response shall be given before |
|           | flushing the codes)                                                                                                                                    |
+-----------+--------------------------------------------------------------------------------------------------------------------------------------------------------+
\endrst
\return \a ERR_SUCCESS on success, or error code
                    - \a    ERR_CFW_INVALID_PARAMETER  The input parameter(nFac) is invalid.
*/
uint32_t CFW_CfgGetEventReport(
    uint16_t *pMer);

/*! \brief function controls the presentation of the event reporting.

\param [in]  pMer   Specify the presentation of the event reporting, you can pass CFW_CFG_CMER(mode,keyp,disp,ind,bfr) to this parameter.
\return \a ERR_SUCCESS on success, or error code
                    - \a    ERR_CFW_INVALID_PARAMETER  The input parameter is NULL.
*/
uint32_t CFW_CfgSetEventReport(
    uint16_t pMer);

/*! \brief function sets the result code  present mode of the phone.

\param [in]  nMode   Specify the result code present mode(0-1).
\return \a ERR_SUCCESS on success, or error code
                    - \a    ERR_CFW_INVALID_SERVICE The input parameter nServiceId is invalid.
*/
uint32_t CFW_CfgSetResultCodePresentMode(
    uint8_t nMode);

/*! \brief function sets the result code format of the phone.

\param [in]  nMode   Specify the format mode(0-1).
\return \a ERR_SUCCESS on success, or error code
                    - \a    ERR_CFW_INVALID_PARAMETER  The input parameter is NULL.
*/
uint32_t CFW_CfgSetResultCodeFormatMode(
    uint8_t nMode);

/*! \brief  function retrieves the result code present mode of the phone.

\param [out]  nMode   Pointer to the result code present mode.
\return \a ERR_SUCCESS on success, or error code
                    - \a    ERR_CFW_INVALID_PARAMETER  The input parameter is NULL.
*/
uint32_t CFW_CfgGetResultCodePresentMode(
    uint8_t *pMode);

/*! \brief  function retrieves the result code format mode of the phone.

\param [out]  nMode   Pointer to the result code format mode.
\return \a ERR_SUCCESS on success, or error code
                    - \a    ERR_CFW_INVALID_PARAMETER  The input parameter is NULL.
*/
uint32_t CFW_CfgGetResultCodeFormatMode(
    uint8_t *pMode);

/*! \brief  function retrieves product identification information.

\param [out]  pIdText   Pointer to character string of product identification information ended with 0.
\return \a ERR_SUCCESS on success, or error code
                    - \a    ERR_CFW_INVALID_PARAMETER  The input parameter is NULL.
*/
uint32_t CFW_CfgGetProductIdentification(
    uint8_t *pIdText);

/*! \brief  function retrieves an extended error report of the reason for the last call release and location.

\param [out]  pErrReport   Pointer to PSTR type to retrieve the error string shown in Location ID table.
\return \a ERR_SUCCESS on success, or error code
                    - \a    ERR_CFW_INVALID_PARAMETER  The input parameter is NULL.
*/
uint32_t CFW_CfgGetExtendedErrorV0(
    const char **pErrReport);

/*! \brief  function set an extended error report of the reason for the last call release and location.

\param [in]  pErrReport   Specify the error code of Location ID.
\return \a ERR_SUCCESS on success, or error code
                    - \a    ERR_CFW_INVALID_PARAMETER  The input parameter is NULL.
*/
uint32_t CFW_CfgSetExtendedErrorV0(
    uint32_t nErrorCode);

/*! \brief  function controls the presentation of the result codes.

\param [in]  nMode   Specify the mode. This parameter can be one of the following value:
\rst
+-------+-----------------------------------------------------------------------------+
| nMode |                                 Description                                 |
+=======+=============================================================================+
| 0     | DCE does not echo characters during command state and online command state. |
+-------+-----------------------------------------------------------------------------+
| 1     | DCE echoes characters during command state and online command state.        |
+-------+-----------------------------------------------------------------------------+
\endrst
\return \a ERR_SUCCESS on success, or error code
                    - \a    ERR_CFW_INVALID_PARAMETER  The input parameter is NULL.
*/
uint32_t CFW_CfgSetEchoMode(
    uint8_t nMode);

/*! \brief  function controls the presentation of the result codes.

\param [out]  nMode   Specify the mode. This parameter can be one of the following value:
\rst
+-------+-----------------------------------------------------------------------------+
| nMode |                                 Description                                 |
+=======+=============================================================================+
| 0     | DCE does not echo characters during command state and online command state. |
+-------+-----------------------------------------------------------------------------+
| 1     | DCE echoes characters during command state and online command state.        |
+-------+-----------------------------------------------------------------------------+
\endrst
\return \a ERR_SUCCESS on success, or error code
                    - \a    ERR_CFW_INVALID_PARAMETER  The input parameter is NULL.
*/
uint32_t CFW_CfgGetEchoMode(
    uint8_t *nMode);

typedef struct _CFW_EXTENDED_ERROR
{
    uint8_t locationId;
    uint8_t reason;
    uint8_t ss_reason;
    uint8_t padding;
} CFW_EXTENDED_ERROR;

/*! \brief  function retrieves an extended error report of the reason for the last call release and location.

\param [in]  pExtendedError   Pointer to CFW_EXTENDED_ERROR structure to contain the extended error.
\return \a ERR_SUCCESS on success, or error code
                    - \a    ERR_CFW_INVALID_PARAMETER  The input parameter is NULL.
*/
uint32_t CFW_CfgGetExtendedErrorV1(
    CFW_EXTENDED_ERROR *pExtendedError);

/*! \brief  function retrieves the error information.

\param [out]  pLevel   Specify the error level to report. This parameter can be one of the following value:
\rst
+-------+------------------------------------------------------+
| nMode |                     Description                      |
+=======+======================================================+
| 0     | disable result code (only 'ERROR' will be displayed) |
+-------+------------------------------------------------------+
| 1     | enable result code and use numeric values            |
+-------+------------------------------------------------------+
| 2     | enable result code and use verbose values            |
+-------+------------------------------------------------------+
\endrst
\return \a ERR_SUCCESS on success, or error code
                    - \a    ERR_CFW_INVALID_PARAMETER  The input parameter is NULL.
*/
uint32_t CFW_CfgGetErrorReport(
    uint8_t *pLevel);

/*! \brief  function controls the presentation of the result codes.

\param [in]  nLevel   Specify the error level to report. This parameter can be one of the following value:
\rst
+-------+------------------------------------------------------+
| nMode |                     Description                      |
+=======+======================================================+
| 0     | disable result code (only 'ERROR' will be displayed) |
+-------+------------------------------------------------------+
| 1     | enable result code and use numeric values            |
+-------+------------------------------------------------------+
| 2     | enable result code and use verbose values            |
+-------+------------------------------------------------------+
\endrst
\return \a ERR_SUCCESS on success, or error code
                    - \a    ERR_CFW_INVALID_PARAMETER  The input parameter is NULL.
*/
uint32_t CFW_CfgSetErrorReport(
    uint8_t nLevel);

uint32_t CFW_CfgDeviceSwithOff(void);

uint32_t CFW_CfgSetConnectResultFormatCode(
    uint8_t nValue);

uint32_t CFW_CfgGetConnectResultFormatCode(
    uint8_t *nValue);

uint32_t CFW_CfgSetSoftReset(
    uint8_t cause);

uint32_t CFW_CfgGetSoftReset(
    uint8_t *cause);

uint32_t CFW_CfgSetResetCause(
    uint8_t cause);

uint32_t CFW_CfgGetResetCause(
    uint8_t *cause);

typedef enum
{

    CFW_SIM_ABSENT = 0x00,
    CFW_SIM_NORMAL = 0x01,
    CFW_SIM_TEST = 0x02,
    CFW_SIM_ABNORMAL = 0x03,
    CFW_SIM_STATUS_END = 0x04,
    CFW_SIM_TYPE_SOFT = 0x05,
    CFW_SIM_STATUS_ENUM_FILL = 0x7fffffff
} CFW_SIM_STATUS;

void CFW_SetSimStatus(
    CFW_SIM_ID nSimID,
    CFW_SIM_STATUS nSimStatus);

CFW_SIM_STATUS CFW_GetSimStatus(
    CFW_SIM_ID nSimID);

uint8_t CFW_GetSimPHYType(
    CFW_SIM_ID nSimID);

bool CFW_GetSimType(
    CFW_SIM_ID nSimID);

/*! \close the sim.

The result should be returned through the following \a EV_CFW_SIM_CLOSE_RSP event:

\rst
+------+-------+---------+---------+-------------+
| UTI  | nType | nParam1 | nParam2 | Description |
+======+=======+=========+=========+=============+
| nUTI | 0     | 0       | 0       | OK          |
+------+-------+---------+---------+-------------+
\endrst

\param [in] nUTI    UTI operation
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_SimClose(
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

uint32_t CFW_GetStackSimFileID(
    uint16_t n3GppFileID,
    uint16_t EFPath,
    CFW_SIM_ID nSimID);

uint32_t CFW_GetStackSimFileIDByPath_V2(
    uint16_t n3GppFileID,
    uint8_t *EFTotalPath,
    uint8_t iEfPathLength,
    CFW_SIM_ID nSimID);

uint32_t CFW_SimReadBinary(
    uint8_t nFileId,
    uint8_t nOffset,
    uint8_t nBytesToRead,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

uint32_t CFW_SimReadElementary(
    uint8_t nFileId,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

uint32_t CFW_SimUpdateBinary(
    uint8_t nFileId,
    uint8_t nOffset,
    uint8_t *pData,
    uint8_t nBytesToWrite,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

uint32_t CFW_SimReadRecord(
    uint8_t nFileID,
    uint8_t nRecordNum,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

uint32_t CFW_SimUpdateRecord(
    uint8_t nFileID,
    uint8_t nRecordNum,
    uint8_t nRecordSize,
    uint8_t *pData,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

uint32_t CFW_SimUpdateRecordAllParam(
    uint8_t nFileID,
    uint8_t nRecordNum,
    uint8_t nRecordSize,
    uint8_t nRecordMode,
    uint8_t *pData,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

uint32_t CFW_SimReadRecordWithLen(
    uint8_t nFileID,
    uint8_t nRecordNum,
    uint16_t nLen,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

uint32_t CFW_SimReadRecordAllParam(
    uint8_t nFileID,
    uint8_t nRecordNum,
    uint8_t nRecordMode,
    uint16_t nLen,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

uint32_t CFW_SimGetFileStatus(
    uint8_t nFileID,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

uint32_t CFW_SimReadFiles(
    uint8_t *pSimFile,
    uint8_t nSimFile,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

//SIM Switch
uint32_t CFW_CfgSetSimSwitch(
    uint8_t n);

uint32_t CFW_CfgGetSimSwitch(
    uint8_t *n);

uint32_t CFW_SwitchDualSimSlot(
    uint8_t nSimSlot);

void SimSwitch(
    uint8_t SimNum);

uint32_t CFW_SimGetOperationTimes();

// PBK Configuration
#define CFW_PBK_SIM 0x01
#define CFW_PBK_ME 0x02

#define CFW_PBK_ME_MISSED 0x1
#define CFW_PBK_ME_RECEIVED 0x2
#define CFW_PBK_SIM_FIX_DIALLING 0x4
#define CFW_PBK_SIM_LAST_DIALLING 0x8
#define CFW_PBK_ME_LAST_DIALLING 0x10
#define CFW_PBK_SIM_BARRED_DIALLING 0x11
#define CFW_PBK_SIM_SERVICE_DIALLING 0x12
#define CFW_PBK_ON 0x20
#define CFW_PBK_EN 0x40
#define CFW_PBK_SDN 0x80

#define CFW_PBK_AUTO 0x00
#define CFW_PBK_ALL 0x00

// Pbk Option
#define CFW_PBK_OPTION_BASIC 0x00
#define CFW_PBK_OPTION_All 0x01
#define CFW_PBK_OPTION_BASIC_NAME_DESCEND_ORDER 0x04
#define CFW_PBK_OPTION_BASIC_NAME_ASCEND_ORDER 0x08
#define CFW_PBK_OPTION_All_NAME_DESCEND_ORDER 0x10
#define CFW_PBK_OPTION_All_NAME_ASCEND_ORDER 0x20

/*! \brief The CFW_GetSimAdnId function to get the Sim AdnId
\param [in]  nSimID  SIMID
\return
Sim AdnId
*/
uint8_t CFW_GetSimAdnId(CFW_SIM_ID nSimID);

typedef struct _CFW_PBK_STRORAGE_INFO
{
    uint16_t index;
    uint8_t iNumberLen;
    uint8_t txtLen;
    uint8_t Bitmap[63];
} CFW_PBK_STRORAGE_INFO;

uint32_t CFW_CfgGetPbkStrorageInfo(
    CFW_PBK_STRORAGE_INFO *pStorageInfo);

uint32_t CFW_CfgSetPbkStrorageInfo(
    CFW_PBK_STRORAGE_INFO *pStorageInfo);

// NW Configuration

#define CFW_STORE_MAX_COUNT 33
typedef struct //this must be the same with api_NwStoreListInd_t
{
    uint8_t Plmn[3];
    uint8_t nCount;
    uint16_t ArfcnL[CFW_STORE_MAX_COUNT];
} CFW_StoredPlmnList;

typedef struct _CFW_SIMSTATUS
{
    bool UsimFlag;
} CFW_SIMSTATUS;

typedef struct _CFW_SIM_PHY_TYPE
{
    uint8_t iSimPhycialType;
} CFW_SIM_PHY_TYPE;

typedef struct _NW_SIM_INFO
{
    CFW_StoredPlmnList p_StoreList[CFW_SIM_COUNT];
    uint8_t SimStatus; // 0: sim; 1: Usim
} CFW_SIM_INFO;

/*! \brief function retrieves the operator names by operator identifier.

\param [in]   nStorage    Specify the current phonebook memory storage. This parameter can be one of the following value:
\rst
+----------------+-------+----------------------------------------------+
| Memory Storage | Value |                   nParam2                    |
+================+=======+==============================================+
| CFW_PBK_SIM    | 0x01  | SIM phonebook (storage depends on SIM Card). |
+----------------+-------+----------------------------------------------+
| CFW_PBK_ME     | 0x02  | ME Phonebook.                                |
+----------------+-------+----------------------------------------------+
| CFW_PBK_ON     | 0x20  | SIM (or ME) own numbers (MSISDNs) list.      |
+----------------+-------+----------------------------------------------+
| CFW_PBK_EN     | 0x40  | Emergency Call storage                       |
+----------------+-------+----------------------------------------------+
\endrst
\param [in]   nSimID      SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_CfgSetPbkStorage(
    uint8_t nStorage,
    CFW_SIM_ID nSimID);

/*! \brief function retrieves the operator names by operator identifier.

\param [out]   nStorage    Specify the current phonebook memory storage. This parameter can be one of the following value:
\rst
+----------------+-------+----------------------------------------------+
| Memory Storage | Value |                   nParam2                    |
+================+=======+==============================================+
| CFW_PBK_SIM    | 0x01  | SIM phonebook (storage depends on SIM Card). |
+----------------+-------+----------------------------------------------+
| CFW_PBK_ME     | 0x02  | ME Phonebook.                                |
+----------------+-------+----------------------------------------------+
| CFW_PBK_ON     | 0x20  | SIM (or ME) own numbers (MSISDNs) list.      |
+----------------+-------+----------------------------------------------+
| CFW_PBK_EN     | 0x40  | Emergency Call storage                       |
+----------------+-------+----------------------------------------------+
\endrst
\param [in]   nSimID      SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_CfgGetPbkStorage(
    uint8_t *nStorage,
    CFW_SIM_ID nSimID);

uint32_t CFW_CfgSetStoredPlmnList(
    CFW_StoredPlmnList *pPlmnL,
    CFW_SIM_ID nSimId);

uint32_t CFW_CfgGetStoredPlmnList(
    CFW_StoredPlmnList *pPlmnL,
    CFW_SIM_ID nSimId);

uint32_t CFW_CfgNwSetLockedFrequencyBand(
    uint8_t nBand,
    CFW_SIM_ID nSimID);

uint32_t CFW_CfgNwGetLockedFrequencyBand(
    uint8_t *pBand,
    CFW_SIM_ID nSimID);

uint32_t CFW_CfgNwSetFrequencyBand(
    uint8_t nBand,
    CFW_SIM_ID nSimID);

uint32_t CFW_CfgNwGetFrequencyBand(
    uint8_t *pBand,
    CFW_SIM_ID nSimID);

uint32_t CFW_CfgNwGetNetWorkMode(
    uint8_t *pMode,
    CFW_SIM_ID nSimID);

uint32_t CFW_CfgNwSetNetWorkMode(
    uint8_t nMode,
    CFW_SIM_ID nSimID);

/*! \brief The CFW_SetCDACValue function set the cdac value
\param [in]  nCdac  cdac value from 0 ~ 255
\param [in]  bSaveFlag  1 save to flash, 0 donot save to flash
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_SetCDACValue(uint8_t nCdac, bool bSaveFlag);

/*! \brief The CFW_GetCDACValue function get the cdac value
\param [out]  pCdac  cdac value from 0 ~ 255
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_GetCDACValue(uint8_t *pCdac);

/*! \brief function retrieves the network status presentation mode.

\param [out] pCmd    Specify the how to control the network status report, this parameter can be one of the following value:
\rst
+-------+---------------------------------------------------------------------------------------------------------------+
| value |                                                  Description                                                  |
+=======+===============================================================================================================+
| 0     | disable URCs                                                                                                  |
+-------+---------------------------------------------------------------------------------------------------------------+
| 1     | enable URC +CREG:<stat> to report status of network registration                                              |
+-------+---------------------------------------------------------------------------------------------------------------+
| 2     | enable URC +CREG:<stat>[,<lac>,<ci>] to report status of network registration including location information. |
|       | Please note that optional parameters will not be displayed during call.                                       |
+-------+---------------------------------------------------------------------------------------------------------------+
\endrst

\param [in]  nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_CfgGetNwStatus(
    uint8_t *pCmd,
    CFW_SIM_ID nSimID);

/*! \brief function sets the network status presentation mode.

\param [in] n        Specify the how to conctrol the network status report, this parameter can be one of the following value:
\rst
+-------+---------------------------------------------------------------------------------------------------------------------+
| value |                                                     Description                                                     |
+=======+=====================================================================================================================+
| 0     | disable to report the network status information.                                                                   |
+-------+---------------------------------------------------------------------------------------------------------------------+
| 1     | enable send EV_CFW_NW_REG_STATUS_IND event to report status of network registration                                 |
+-------+---------------------------------------------------------------------------------------------------------------------+
| 2     | enable send EV_CFW_NW_REG_STATUS_IND event to report status of network registration including location information. |
+-------+---------------------------------------------------------------------------------------------------------------------+
\endrst

\param [in]  nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_CfgSetNwStatus(
    uint8_t n,
    CFW_SIM_ID nSimID);

/*! \brief function retrieves the operator names according to the input index (based on zero) .

\param [out] pOperatorId    Specify the operator identifier in digital format,if the ID length is 5,the array should terminated with 0x0F. For example,the id is 0X04,0X06,0X00,0X00,0X00,0X0F,and its corresponding full name is "China Mobile"
\param [out] pOperatorName  Pointer to the buffer to hold the operator name in ASCII string format with zero-terminated
\param [in]  nIndex         Specify the based-zero index(0 - 31).
\return \a ERR_SUCCESS on success, or error code
                - \a ERR_CFW_INVALID_PARAMETER  (The input parameter is NULL)
                - \a ERR_CFW_NOT_EXIST_OPERATOR_ID  (Don't find any operator identifier in the system)
                - \a ERR_CFG_PARAM_OUT_RANGR
*/
uint32_t CFW_CfgNwGetOperatorInfo(
    uint8_t **pOperatorId,
    uint8_t **pOperatorName,
    uint32_t nIndex);

/*! \brief function retrieves the T3302 value.

\return \a T3302 value
*/
uint32_t CFW_GetT3302(CFW_SIM_ID nSimID);

/*! \brief function set T3302 value.

\param [in]  value T3302 value.
\return \a ERR_SUCCESS on success
*/
uint32_t CFW_SetT3302(uint32_t value, CFW_SIM_ID nSimID);

/*! \brief function retrieves the RRC release value.

\return \a RRC release value
*/
uint32_t CFW_GetRRCRel(CFW_SIM_ID nSimID);

/*! \brief function set the RRC release value.

\param [in]  value RRC release value.
\return \a ERR_SUCCESS on success
*/
uint32_t CFW_SetRRCRel(uint32_t value, CFW_SIM_ID nSimID);

/*! \brief function retrieves the RRC release value.

\param [in]  value sim index.
\return \a RRC release value
*/
uint32_t CFW_GetRRCRelEx(
    CFW_SIM_ID nSimID);

/*! \brief function set the RRC release value.

\param [in]  value RRC release value.
\param [in]  sim index.
\return \a ERR_SUCCESS on success
*/
uint32_t CFW_SetRRCRelEx(
    uint32_t value,
    CFW_SIM_ID nSimID);

uint16_t CFW_GetDrxCyc(
    CFW_SIM_ID nSimID);

/*! \brief function get the electric fence cell flag.

\return \a  electric fence  cell flag value
*/
uint32_t CFW_GetEleFence(
    CFW_SIM_ID nSimID);
/*! \brief function set the electric fence cell flag.

\param [in] electric fence cell flag value(0: Enable the electric fence; 1: Disable the electric fence).
\return \a ERR_SUCCESS on success
*/
uint32_t CFW_SetEleFence(
    uint8_t value,
    CFW_SIM_ID nSimID);
/*! \brief function get the electric fence cell timer.

\return \a  electric fence  cell timer value
*/
uint32_t CFW_GetEleFenceTimer(
    CFW_SIM_ID nSimID);
/*! \brief function set the electric fence cell timer.

\param [in] electric fence cell timer value.
\return \a ERR_SUCCESS on success
*/
uint32_t CFW_SetEleFenceTimer(
    uint8_t value,
    CFW_SIM_ID nSimID);

/*! \brief function retrieves the operator ID by operator name.

\param [out] pOperatorId    Specify the operator identifier in digital format,if the ID length is 5,the array should terminated with 0x0F. For example,the id is 0X04,0X06,0X00,0X00,0X00,0X0F,and its corresponding full name is "China Mobile"
\param [in]  format         Out the pOperatorName format.
\param [in]  pOperatorName  Pointer to the buffer to hold the operator name in ASCII string format with zero-terminated
\return \a ERR_SUCCESS on success, or error code
                - \a ERR_CFW_INVALID_PARAMETER  (The input parameter is NULL)
                - \a ERR_CFW_NOT_EXIST_OPERATOR_NAME  (Don't find any specific operator name  in the system)
                - \a ERR_CFG_PARAM_OUT_RANGR
*/
uint32_t CFW_CfgNwGetOperatorId(
    uint8_t **pOperatorId,
    uint8_t format,
    uint8_t *pOperatorName);

uint32_t CFW_CfgNwGetOperatorIdWithIndex(
    uint8_t pOperatorId[6],
    uint8_t *pOperatorName,
    uint8_t format,
    uint32_t *nIndex);

/*! \brief function retrieves the operator names by operator identifier.

\param [in]   pOperatorId    Specify the operator identifier in digital format,if the ID length is 5,the array should terminated with 0x0F. For example,the id is 0X04,0X06,0X00,0X00,0X00,0X0F,and its corresponding full name is "China Mobile"
\param [out]  pOperatorName       Pointer to the buffer to hold the operator long name in ASCII string format with zero-terminated
\param [out]  pOperatorShortName  Pointer to the buffer to hold the operator short name in ASCII string format with zero-terminated
\return \a ERR_SUCCESS on success, or error code
                - \a ERR_CFW_INVALID_PARAMETER  (The input parameter is NULL)
                - \a ERR_CFW_NOT_EXIST_OPERATOR_ID  (Don't find any operator identifier in the system)
                - \a ERR_CFG_PARAM_OUT_RANGR
*/
uint32_t CFW_CfgNwGetOperatorName(
    uint8_t pOperatorId[6],
    uint8_t **pOperatorName,
    uint8_t **pOperatorShortName);

uint32_t CFW_CfgSetNwOperDispFormat(
    uint8_t nFormat);

uint32_t CFW_CfgGetNwOperDispFormat(
    uint8_t *nFormat);

//if gsmGprsOnly is 1,we will not register for GSM-only network.
uint32_t CFW_GetGsmGprsOnly(
    CFW_SIM_ID nSimID);

uint32_t CFW_SetGsmGprsOnly(
    uint8_t value, CFW_SIM_ID nSimID);

// CM Configuration

uint32_t CFW_CfgSetSSN(
    uint8_t nCSSI,
    uint8_t nCSSU,
    CFW_SIM_ID nSimID);

uint32_t CFW_CfgGetSSN(
    uint8_t *pCSSI,
    uint8_t *pCSSU,
    CFW_SIM_ID nSimID);

/*! \brief   function enables or disables the presentation of the CLI (Calling Line Identification).

\param [in]  nClir This parameter can be one of the following value:
\rst
+-------+----------------------------------------------------------------------------------+
| nClir |                                   Description                                    |
+=======+==================================================================================+
| 0     | presentation indicator is used according to the subscription of the CLIR service |
+-------+----------------------------------------------------------------------------------+
| 1     | CLIR invocation                                                                  |
+-------+----------------------------------------------------------------------------------+
| 2     | CLIR suppression                                                                 |
+-------+----------------------------------------------------------------------------------+
\endrst
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
                    - \a    ERR_CFW_INVALID_PARAMETER  The input parameter is NULL.
*/
uint32_t CFW_CfgSetClir(
    uint8_t nClir,
    CFW_SIM_ID nSimID);

/*! \brief   function queries the setting of restriction of the CLI (Calling Line Identification).

\param [out]  n  Pointer to UINT8 to show the settings for outgoing calls, this parameter can be on of the following value:
\rst
+-----+----------------------------------------------------------------------------------+
|  n  |                                   Description                                    |
+=====+==================================================================================+
| 0   | presentation indicator is used according to the subscription of the CLIR service |
+-----+----------------------------------------------------------------------------------+
| 1   | CLIR invocation                                                                  |
+-----+----------------------------------------------------------------------------------+
| 2   | CLIR suppression                                                                 |
+-----+----------------------------------------------------------------------------------+
\endrst
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
                    - \a    ERR_CFW_INVALID_PARAMETER  The input parameter is NULL.
*/
uint32_t CFW_CfgGetClir(
    uint8_t *n,
    CFW_SIM_ID nSimID);

/*! \brief   function enables or disables the presentation of the CLI (Calling Line Identification).

\param [in] nEnable 1 indicate presentation of the CLI; and 0 means don't presentation of the CLI.
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
                    - \a    ERR_CFW_INVALID_PARAMETER  The input parameter is NULL.
*/
uint32_t CFW_CfgSetClip(
    uint8_t nEnable,
    CFW_SIM_ID nSimID);

/*! \brief   function enables or disables the presentation of the CLI (Calling Line Identification).

\param [out]  n
\rst
+-----+--------------------------------------+
|  n  |             Description              |
+=====+======================================+
| 0   | means don't presentation of the CLI. |
+-----+--------------------------------------+
| 1   | indicate presentation of the CLI.    |
+-----+--------------------------------------+
\endrst
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
                    - \a    ERR_CFW_INVALID_PARAMETER  The input parameter is NULL.
*/
uint32_t CFW_CfgGetClip(
    uint8_t *n,
    CFW_SIM_ID nSimID);

/*! \brief   function enables or disables a calling subscriber to get the connected line identity (COL) of the called party after setting up a mobile originated call.
See 3GPP TS 22.081 [3] for more.

\param [in]  n shows the result code presentation status. This parameter can be one of the following value:
\rst
+-----+--------------------------+
|  n  |       Description        |
+=====+==========================+
| 0   | Disable to show the COLP |
+-----+--------------------------+
| 1   | Enable to show the COLP  |
+-----+--------------------------+
\endrst
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
                    - \a    ERR_CFW_INVALID_PARAMETER  The input parameter is NULL.
*/
uint32_t CFW_CfgSetColp(
    uint8_t n,
    CFW_SIM_ID nSimID);

/*! \brief   function retrieves the enables or disables setting of  the connected line identity (COL) of the called party after setting up a mobile originated call.
See 3GPP TS 22.081 [3] for more.

\param [out]  n Point to UINT8 to hold the result code presentation status. This parameter can be one of the following value:
\rst
+-----+--------------------------+
|  n  |       Description        |
+=====+==========================+
| 0   | Disable to show the COLP |
+-----+--------------------------+
| 1   | Enable to show the COLP  |
+-----+--------------------------+
\endrst
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
                    - \a    ERR_CFW_INVALID_PARAMETER  The input parameter is NULL.
*/
uint32_t CFW_CfgGetColp(
    uint8_t *n,
    CFW_SIM_ID nSimID);

/*! \brief function sets the presentations status of call waiting in the MT/TA:

\param [in]   n  sets/shows the result code presentation status in the MT/TA, If you don't need this parameter, set this parameter to 0xFF,
the system will select the default value to zero. (sets/shows the result code presentation status in the MT/TA):
\param [in]   nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
                - \a ERR_CFW_INVALID_PARAMETER  (The input parameter is NULL)
*/
uint32_t CFW_CfgSetCallWaiting(
    uint8_t n,
    CFW_SIM_ID nSimID);

/*! \brief function sets the presentations status of call waiting in the MT/TA:

\param [out]   n  sets/shows the result code presentation status in the MT/TA, If you don't need this parameter, set this parameter to 0xFF,
the system will select the default value to zero. (sets/shows the result code presentation status in the MT/TA):
\param [in]    nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
                - \a ERR_CFW_INVALID_PARAMETER  (The input parameter is NULL)
*/
uint32_t CFW_CfgGetCallWaiting(
    uint8_t *n,
    CFW_SIM_ID nSimID);

uint32_t CFW_CfgGetIMSI(
    uint8_t pIMSI[15],
    CFW_SIM_ID nSimID);

uint32_t CFW_CfgSetIMSI(
    uint8_t pIMSI[15],
    CFW_SIM_ID nSimID);

typedef enum
{
    CFW_CDMA_IMSI_NOT_INIT,
    CFW_CDMA_IMSI_NOT_EXIST,
    CFW_CDMA_IMSI_OK,
} CFW_CDMA_IMSI_STATUS;

uint32_t CFW_CfgGetCdmaImsi(
    uint8_t pCdmaImsi[10],
    CFW_SIM_ID nSimID);

#ifdef LTE_SUPPORT
uint32_t CFW_NwSetEpsMode(
    uint8_t eps_mode,
    CFW_SIM_ID nSimID);
#endif

uint32_t CFW_NwSetMaxAttFailForIratReq(
    uint8_t nMaxNum,
    CFW_SIM_ID nSimID);

uint8_t CFW_NwIsUseISIM(
    char *plmnStr);

uint32_t CFW_CfgSetToneDuration(
    uint8_t nToneDuration);

uint32_t CFW_CfgGetToneDuration(
    uint8_t *pToneDuration);

uint32_t CFW_CfgSetIncomingCallResultMode(
    uint8_t nMode);

uint32_t CFW_CfgGetIncomingCallResultMode(
    uint8_t *nMode);

// Device Configuration
typedef struct _CFW_AUD_OUT_PARAMETER
{
    uint8_t outBbcGain;
    uint8_t outStep;
    uint16_t sideTone;
    uint16_t outCalibrate[4];
} CFW_AUD_OUT_PARAMETER;

uint32_t CFW_CfgSetAudioOutputParam(
    CFW_AUD_OUT_PARAMETER *pAudOutParam);

uint32_t CFW_CfgGetAudioOutputParam(
    CFW_AUD_OUT_PARAMETER *pAudOutParam);

uint32_t CFW_CfgSetAudioAudioMode(
    uint8_t nAudioMode);

uint32_t CFW_CfgGetAudioAudioMode(
    uint8_t *pAudioMode);

uint32_t CFW_CfgSetLoudspeakerVolumeLevel(
    uint8_t nVolumeLevel);

uint32_t CFW_CfgGetLoudspeakerVolumeLevel(
    uint8_t *pVolumeLevel);

uint32_t CFW_CfgGetLoudspeakerVolumeLevelRange(
    uint8_t *pMinVolumeLevel,
    uint8_t *pMaxVolumeLevel);

//
// NW Service
//
#define CFW_NW_STATUS_NOTREGISTERED_NOTSEARCHING 0 // Not registered and not searching.
#define CFW_NW_STATUS_REGISTERED_HOME 1            //Registered and in the home area.
#define CFW_NW_STATUS_NOTREGISTERED_SEARCHING 2    //Not registered and searching for a new operator.
#define CFW_NW_STATUS_REGISTRATION_DENIED 3        //Registration denied.
#define CFW_NW_STATUS_UNKNOW 4                     //Unknown registration.
#define CFW_NW_STATUS_REGISTERED_ROAMING 5

#ifdef LTE_SUPPORT
#define CFW_NW_SMS_ONLY_HOME 6
#define CFW_NW_SMS_ONLY_ROAMING 7
#define CFW_NW_EMER_SVCE_ONLY 8
#define CFW_NW_CSFB_NOT_PREFER_HOME 9
#define CFW_NW_CSFB_NOT_PREFER_ROAMING 10
#endif

#define CFW_NW_AUTOMATIC_MODE 0
#define CFW_NW_MANUAL_MODE 1
#define CFW_NW_MANUAL_AUTOMATIC_MODE 4

#define CFW_NW_OPERATOR_STATUS_UNKNOW 0    //unknown
#define CFW_NW_OPERATOR_STATUS_AVAILABLE 1 //available
#define CFW_NW_OPERATOR_STATUS_CURRENT 2   //current
#define CFW_NW_OPERATOR_STATUS_FORBIDDEN 3 //forbidden

#define CFW_RAT_AUTO 0
#define CFW_RAT_GSM_ONLY 2
#define CFW_RAT_LTE_ONLY 4
#define CFW_RAT_NBIOT_ONLY 8

#define CFW_IPV4 0x01
#define CFW_IPV6 0x02
#define CFW_IPV4V6 0x03
#define CFW_PDP_EMPTY 0x04

#define DATA_PDP_CID 5

#define MAX_PCSCF_ADDR_NUMBER 6
//#ifdef CFW_VOLTE_SUPPORT //quectel modify

#define IMS_PDP_CID 11
#define IMS_PDP_APN "ims"

#define IMS_EMERGENCY_PDP_CID 9

#define CFW_IMSPDPACTIVE_MAX_COUNT (3)

typedef enum
{
    IMS_PDP_STATUS_IDLE,
    IMS_PDP_STATUS_CONNECT,
    IMS_PDP_STATUS_ACTIVE,
    IMS_PDP_STATUS_DISCONNECT,
    IMS_PDP_STATUS_WAIT_RETRY,
    IMS_PDP_STATUS_STOP,
    IMS_PDP_STATUS_ENUM_FILL = 0x7fffffff
} CFW_IMS_PDP_STATUS;

typedef enum
{
    IMS_NW_LTE_VOICE_NOT_SUPPORT = 0x00,
    IMS_NW_CS_VOICE_ONLY,
    IMS_NW_CS_VOICE_PREFER_PS_VOICE_SEC,
    IMS_NW_PS_VOICE_PREFER_CS_VOICE_SEC,
    IMS_NW_PS_VOICE_ONLY,
    IMS_NW_VOICE_PREFERENCE_MAX,
    IMS_NW_VOICE_ENUM_FILL = 0x7fffffff
} CFW_IMS_NW_VOICE_TYOE;
//#ifndef CFW_VOLTE_HANDSHAKE_SUPPORT
//#define CFW_VOLTE_HANDSHAKE_SUPPORT
//#endif
#ifdef CFW_VOLTE_HANDSHAKE_SUPPORT

typedef enum
{
    HANDSHAKE_IDLE,
    HANDSHAKE_GOING,
    HANDSHAKE_SUCCESS,
    HANDSHAKE_FAIL,
    HANDSHAKE_RETRY,
    HANDSHAKE_STOP,
    HANDSHAKE_ENUM_FILL = 0x7fffffff
} CFW_IMS_HANDSHAKE_STATE;
extern uint8_t g_HandShakeRetryCount[];
extern uint8_t g_HandShakeMaxRetryCount[];
#endif

extern uint8_t gCfwImsPdpActiveCount[];
extern uint8_t gCfwImsRegetPcscf[];

uint32_t CFW_ImsInit(CFW_SIM_ID nSimID);
uint32_t CFW_ImsEnable(uint8_t isEmergency, uint16_t nUTI, CFW_SIM_ID nSimID);
uint32_t CFW_ImsDisable(uint8_t isEmergency, uint16_t nUTI, CFW_SIM_ID nSimID);
uint32_t CFW_ImsSuspend(CFW_SIM_ID nSimID, CFW_IMS_SUSPEND_TYPE nType);
uint32_t CFW_ImsResume(CFW_SIM_ID nSimID);
#ifdef CFW_VOLTE_HANDSHAKE_SUPPORT
uint32_t CFW_ImsHandShakeStartReq(uint32_t ReqSrc, CFW_SIM_ID nSimID);
uint32_t CFW_ImsHandShakeReqSrc(CFW_SIM_ID nSimID);
uint32_t CFW_ImsHandShakeEndReq(CFW_SIM_ID nSimID);
uint32_t CFW_ImsSetHandShankeRetryTimer(uint32_t nElapse, uint8_t nMaxRetry, CFW_SIM_ID nSimID);
#endif
uint32_t CFW_ImsSetDapsCallback(uint8_t nCid, CFW_SIM_ID nSimID);
uint32_t CFW_ImsSetVolte(uint8_t bSet, uint16_t nUTI, CFW_SIM_ID nSimID);
uint32_t CFW_ImsSetCevdp(uint8_t setting, uint16_t nUTI, CFW_SIM_ID nSimID);
uint32_t CFW_ImsSetTerminalMode(uint8_t setting, uint16_t nUTI, CFW_SIM_ID nSimID);
uint8_t CFW_ImsGetTerminalMode(CFW_SIM_ID nSimID);
uint32_t CFW_ImsSetSrvccMode(uint8_t ueSrvcc, uint16_t nUTI, CFW_SIM_ID nSimID);
uint8_t CFW_ImsGetSrvccMode(CFW_SIM_ID nSimID);
bool CFW_ImsIsSet(CFW_SIM_ID nSimID);
uint8_t CFW_ImsFreeUti(void);
uint32_t CFW_ImsActImsPdp(uint16_t nUTI, CFW_SIM_ID nSimID);
uint32_t CFW_ImsDoSavedEmcDail(void);
uint32_t CFW_ImsSetPdpStatus(uint8_t nStatus, uint8_t isEmergency, CFW_SIM_ID nSimID);
uint8_t CFW_ImsGetPdpStatus(uint8_t isEmergency, CFW_SIM_ID nSimID);
uint32_t CFW_ImsCheckStatusAfterCc(uint8_t nCsImsFlag, CFW_SIM_ID nSimID);
uint32_t CFW_SsSetClip(uint8_t set, uint16_t nUTI, CFW_SIM_ID nSimID);
uint32_t CFW_SsSetClir(uint8_t set, uint16_t nUTI, CFW_SIM_ID nSimID);
uint32_t CFW_SsSetColp(uint8_t set, uint16_t nUTI, CFW_SIM_ID nSimID);
bool CFW_ImsIsRegistered(uint8_t isEmergency, CFW_SIM_ID nSimID);

//#endif //quectel modify

//24008 10.5.3.13, listContent is copy start from octet 3
typedef struct _CFW_EMC_NUM_LIST_INFO
{
    uint8_t plmn[3];
    uint8_t lenOfNumList;
    uint8_t listContent[48];
} CFW_NW_EMC_NUM_LIST_INFO;

typedef struct _CFW_NW_NETWORK_INFO
{
    uint8_t nLocalTimeZoneLen;
    uint8_t nLocalTimeZone;
    uint8_t nUniversalTimeZoneLen;
    uint8_t nUniversalTimeZone[7];
    uint8_t nDaylightSavingTimeLen;
    uint8_t nDaylightSavingTime;
    uint8_t nNwkFullNameLen;
    uint8_t nNwkShortNameLen;
    uint8_t nNwkNames[247];
} CFW_NW_NETWORK_INFO;

typedef struct _CFW_NW_MBS_CELL_INFO
{
    uint16_t nArfcn; // Abs Freq Number
    uint8_t nBsic;   // Base Station Code
    uint8_t nCellId[2];
    uint8_t nLai[5];
} CFW_NW_MBS_CELL_INFO;

typedef struct _CFW_NW_JAMMING_DETECT_GET_IND
{
    uint8_t nMode;    /*1: Jamming Detect Enabled; 0: Jamming Detect Disabled*/
    uint8_t nJamming; /*Jamming Status, 1 means Jamming detected*/
} CFW_NW_JAMMING_DETECT_GET_IND;

#define CFW_MAX_JAMMING_FREQ_NUM (40)

typedef struct
{
    uint32_t freq;
    int16_t rssi;
    uint16_t reserved;
} CFW_jammingFreqRssi_t;

typedef struct
{
    uint8_t mode; /*API_RAT_GSM or  API_RAT_LTE*/
    uint8_t reserved;
    uint16_t freqNum;
    CFW_jammingFreqRssi_t jammingFreqRssi[CFW_MAX_JAMMING_FREQ_NUM];
} CFW_JAMMING_RSSI_ENQUIRY_RSP;

typedef struct _CFW_NW_QUAL_INFO
{
    uint8_t nRssi;
    uint8_t nRxQual;
    int8_t iRsrp;
    int8_t iRsrq;
    int16_t nRssidBm;
    int8_t Sinr;
} CFW_NW_QUAL_INFO;

typedef struct _CFW_NW_OPERATOR_INFO
{
    uint8_t nOperatorId[6];
    uint8_t nStatus;
    uint8_t nRat;
} CFW_NW_OPERATOR_INFO;

typedef struct _CFW_NW_OPERATOR_INFO_V2
{
    CFW_NW_OPERATOR_INFO nOperatorInfo;
    int16_t nRsrp;
} CFW_NW_OPERATOR_INFO_V2;
#define NW_MAX_HNBNAME 48
typedef struct _CFW_NW_STATUS_INFO
{
    uint8_t nStatus;
    uint8_t nCause;          //only use PS no service
    uint8_t PsType;          // 1: GPRS 2: EDGE 3: UMTS 4: NB-IoT 3: WB-EUTRAN(eg.CATM)
    uint8_t nCsStatusChange; //add for if CS status have changed,we will set the TRUE.
    uint8_t nAreaCode[5];
    uint8_t nCurrRat;
    uint8_t access;
#if (defined LTE_NBIOT_SUPPORT) || (defined LTE_SUPPORT)
    uint8_t nCellId[4]; //cell id in umts is bitstring(28), so 4 bytes is necessary.
    uint8_t cause_type;
    uint8_t reject_cause;
    uint8_t activeTime;
    uint8_t periodicTau;
    uint8_t nwSupportedCiot; //[0-3]
    uint8_t edrxEnable;      //0 off, 1 on
    uint8_t edrxPtw;         //[0-15]
    uint8_t edrxValue;       //[0-15]
    uint8_t ConnMode;
    uint16_t phyCellId;
    uint32_t frequency;
#else
    uint8_t nCellId[2];
#endif
    uint16_t bsic;
    uint16_t gsmFreq;
    uint8_t StatusCause; //network reject cause for PS of GSM
#ifdef CSG_SUPPORT
    uint8_t cellTypeId;
    uint8_t csgTypeInd;
    uint8_t hnbNameLength;
    uint32_t csgId;
    uint8_t hnbName[NW_MAX_HNBNAME];
#endif
//#ifdef CFW_VOLTE_SUPPORT //quectel modify
    uint8_t nIMSVoPS;
    uint8_t nImsPCSCFStatus;
    uint8_t EMCBS;
//#endif //quectel modify
} CFW_NW_STATUS_INFO;

typedef struct
{
    /*
    0	idle
    1	connected
    */
    uint8_t ConnMode;
    /*
    0   UTRAN URA_PCH state
    1   UTRAN Cell_PCH state
    2   UTRAN Cell_FACH state
    3   UTRAN Cell_DCH state
    4   GERAN CS connected state
    5   GERAN PS connected state
    6   GERAN CS and PS connected state
    7   E-UTRAN connected state
    */
    uint8_t State;
    /*
    0	Indicates usage of radio access of type GERAN, see 3GPP TS 45.001 [146].
    1	Indicates usage of radio access of type UTRAN TDD, see 3GPP TS 25.212 [144].
    2	Indicates usage of radio access of type UTRAN FDD, see 3GPP TS 25.212 [144].
    3	Indicates usage of radio access of type E-UTRAN TDD, see 3GPP TS 36.300 [145].
    4	Indicates usage of radio access of type E-UTRAN FDD, see 3GPP TS 36.300 [145].
    */
    uint8_t Access;

    uint8_t rr_L1_ConnectState;
    uint16_t Arfcn;
    uint8_t Bsic;
    uint8_t Lai[5];
    uint8_t Rac;
    uint8_t CellId[2];
} CFW_NW_CONN_STATUS_INFO;

#define CFW_BLACK_WHITE_PLMN_NUM 5
typedef struct
{
    uint8_t nMode; //0:enable white list; 1:enable black lsit; 2: disalbe black white lsit
    uint8_t ltePlmnNum;
    uint8_t ltePlmnList[CFW_BLACK_WHITE_PLMN_NUM * 6];
    uint8_t gsmPlmnNum;
    uint8_t gsmPlmnList[CFW_BLACK_WHITE_PLMN_NUM * 6];
} CFW_BLACK_WHITE_LIST;

uint32_t CFW_NwSetBlackWhiteList(CFW_BLACK_WHITE_LIST *pBlackWhiteList, CFW_SIM_ID nSimID);

#define NW_MAX_FREQ 20
#ifdef CSG_SUPPORT

typedef struct
{
    uint8_t nCellTypeId;
    uint8_t csgStatus;
    uint8_t csgTypeFile;
    uint8_t csgTypeInd;
    uint8_t hnbNameLength;
    uint32_t csgId;
    uint8_t hnbName[NW_MAX_HNBNAME];
} NW_CSG_CELL_INFO;
typedef struct
{
    uint8_t nIndex; //maybe not?
    uint8_t nPlmnNb;
    uint8_t nLteCellNb;
    uint8_t searchMode; /*1:freq scan; 2:csg cell info; other:normal user plmn search*/
    uint8_t nPlmnL[NW_MAX_FREQ * 3];
    uint8_t nRestr[NW_MAX_FREQ];
    uint8_t nRat[NW_MAX_FREQ];
    NW_CSG_CELL_INFO nCellInfo[NW_MAX_FREQ];
} NW_CSG_LIST_INFO;
uint32_t CFW_NwGetCsgOperators(uint16_t nUTI, CFW_SIM_ID nSimID);
uint32_t CFW_NwCSGSetRegistration(uint8_t nOperatorId[6], uint8_t nMode,
                                  uint8_t nRat, uint32_t nCsgId, uint16_t nUTI, CFW_SIM_ID nSimID);
uint32_t CFW_NwSetCsgMode(uint8_t nMode, CFW_SIM_ID nSimID);
uint32_t CFW_NwGetCsgMode(CFW_SIM_ID nSimID);
#endif

typedef struct
{
    uint32_t nArfcn;
    uint8_t nTac[2];
    uint16_t nPci;
    int16_t nRSRP;
    int16_t nRSRQ;
    int16_t nRSSI;
    uint16_t nBand;
    uint16_t nSINR;
    uint32_t nCellId;
    uint8_t nPlmnId[3];
} NW_FREQ_SCAN_CELL_INFO;
typedef struct
{
    uint32_t nArfcn;
    uint8_t nLac[2];
    int16_t nRSSI;
    uint16_t nBand;
    uint16_t nCellId[2];
    uint8_t nPlmnId[3];
    uint16_t nBsic;
} NW_GSM_FREQ_SCAN_CELL_INFO;

typedef struct
{
    uint8_t nCount; // 0: LTE 1: GSM
    uint8_t nPlmnNb;
    uint8_t nLteCellNb;
    uint8_t nGsmCellNb;
    uint8_t nPlmnL[NW_MAX_FREQ * 3];
    uint8_t nRestr[NW_MAX_FREQ];
    uint8_t nRat[NW_MAX_FREQ];
    NW_FREQ_SCAN_CELL_INFO *pCellInfo;    //nCellInfo[NW_MAX_FREQ]
    NW_GSM_FREQ_SCAN_CELL_INFO *pGsmCell; //nGsmCell[NW_MAX_FREQ]
    uint8_t searchMode;                   /*1:freq scan; other:normal user plmn search*/
    uint8_t padding[3];
} NW_FREQ_SCAN_INFO;

#ifndef CONFIG_QUEC_PROJECT_FEATURE
typedef struct
{
    uint32_t nArfcn;
    uint8_t nTac[2];
    uint16_t nPci;
    int16_t nRSRP;
    int16_t nRSRQ;
    int16_t nRSSI;
    uint16_t nBand;
    uint16_t nSINR;
    uint32_t nCellId;
    uint8_t nPlmnId[3];
} NW_FREQ_SCAN_CELL_ALL_INFO;

typedef struct
{
    uint32_t nArfcn;
    uint8_t nLac[2];
    int16_t nRSSI;
    uint16_t nBand;
    uint16_t nCellId[2];
    uint8_t nPlmnId[3];
    uint16_t nBsic;
} NW_GSM_FREQ_SCAN_CELL_ALL_INFO;

typedef struct
{
    uint8_t nCount; // 0: LTE 1: GSM
    uint8_t nPlmnNb;
    uint8_t nLteCellNb;
    uint8_t nGsmCellNb;
    uint8_t nPlmnL[NW_MAX_FREQ * 3];
    uint8_t nRestr[NW_MAX_FREQ];
    uint8_t nRat[NW_MAX_FREQ];
    NW_FREQ_SCAN_CELL_ALL_INFO *pCellInfo;    //nCellInfo[NW_MAX_FREQ]
    NW_GSM_FREQ_SCAN_CELL_ALL_INFO *pGsmCell; //nGsmCell[NW_MAX_FREQ]
    uint8_t searchMode;                       /*1:freq scan; other:normal user plmn search*/
    uint8_t padding[3];
} NW_FREQ_SCAN_ALL_INFO;

typedef struct
{
    uint32_t nArfcn;
    uint8_t nLac[2];
    int16_t nRSSI;
    uint16_t nBand;
    uint16_t nCellId[2];
    uint8_t nPlmnId[3];
    uint16_t nBsic;
    uint8_t c1;
    bool isgprsSupport;
} NW_GSM_FREQ_SCAN_CELL_ALL_INFO_V2;

typedef struct
{
    uint8_t nCount; // 0: LTE 1: GSM
    uint8_t nPlmnNb;
    uint8_t nLteCellNb;
    uint8_t nGsmCellNb;
    uint8_t nPlmnL[NW_MAX_FREQ * 3];
    uint8_t nRestr[NW_MAX_FREQ];
    uint8_t nRat[NW_MAX_FREQ];
    NW_FREQ_SCAN_CELL_ALL_INFO *pCellInfo;       //nCellInfo[NW_MAX_FREQ]
    NW_GSM_FREQ_SCAN_CELL_ALL_INFO_V2 *pGsmCell; //nGsmCell[NW_MAX_FREQ]
    uint8_t searchMode;                          /*1:freq scan; other:normal user plmn search*/
    uint8_t padding[3];
} NW_FREQ_SCAN_ALL_INFO_V2;
#endif

#define CFW_MAX_BLACK_LTE_CELL_NUM 10
#define CFW_MAX_BLACK_CELL_NUM 10
typedef struct _CFW_CellDescLte
{
    uint32_t nFrequency;
    uint16_t nPhyCellId;
    uint16_t nReserved;
} CFW_CellDescLte;

typedef struct _CFW_CellDescGsm
{
    uint16_t nArfcn;
    uint8_t nLai[5];
    uint8_t nBsic;
} CFW_CellDescGsm;

typedef struct
{
    uint16_t nLteBlackCellNum;
    uint16_t nGsmBlackcellnum;
    CFW_CellDescLte nLteBlackCellList[CFW_MAX_BLACK_LTE_CELL_NUM];
    CFW_CellDescGsm nGsmBlackCellList[CFW_MAX_BLACK_CELL_NUM];
} CFW_BlackCellList;

typedef struct
{
    uint8_t nMode;
    uint16_t nLteBlackCellNum;
    uint16_t nGsmBlackcellnum;
    CFW_CellDescLte nLteBlackCellList[CFW_MAX_BLACK_LTE_CELL_NUM];
    CFW_CellDescGsm nGsmBlackCellList[CFW_MAX_BLACK_CELL_NUM];
} CFW_BlackCellList_V2;

typedef struct
{
    uint8_t BlackListCnt;
    uint16_t Seconds;
} CFW_BlackListCntReq;

typedef struct
{
    uint8_t cellNum;
    uint8_t reserved[3];
    CFW_CellDescLte cellList[CFW_MAX_BLACK_LTE_CELL_NUM];
} CFW_NW_BLKLIST_LTE_INFO;

uint32_t CFW_GetGsmBlkList(uint16_t nUTI, CFW_SIM_ID nSimID);
uint32_t CFW_GetLteBlkList(uint16_t nUTI, CFW_SIM_ID nSimID);

uint32_t CFW_NWSetRat(uint8_t nRat, CFW_SIM_ID nSimID);
uint8_t CFW_NWGetRat(CFW_SIM_ID nSimID);
uint32_t CFW_NWSetStackRat(uint8_t nRat, CFW_SIM_ID nSimID);
uint8_t CFW_NWGetStackRat(CFW_SIM_ID nSimID);
uint32_t CFW_NwSetNetMode(uint8_t nNetMode, CFW_SIM_ID nSimID);
uint32_t CFW_NwSetFTA(uint8_t nFta, CFW_SIM_ID nSimID);
uint8_t CFW_NwGetFTA(CFW_SIM_ID nSimID);
uint32_t CFW_NwSetAutoAttachFlag(uint8_t flag, CFW_SIM_ID nSimID);
uint8_t CFW_NwGetAutoAttachFlag(CFW_SIM_ID nSimID);
uint32_t CFW_NwSetnSecurityUsedFlag(uint8_t nSecurityUsedFlag, CFW_SIM_ID nSimID);
uint32_t CFW_GprsIpDnsInfo(uint8_t nCid, CFW_SIM_ID nSimID);
uint32_t CFW_GprsClrIpDns(void);
uint32_t CFW_NwSetBlackList(CFW_BlackCellList nBlackCellList, CFW_SIM_ID nSimID);
uint32_t CFW_NwSetBlackList_V2(CFW_BlackCellList_V2 nBlackCellList, CFW_SIM_ID nSimID);
uint32_t CFW_nvGetBlackListEnable(CFW_SIM_ID nSimID);
uint8_t CFW_nvSetBlackListEnable(uint8_t BlacklistEnable, CFW_SIM_ID nSimID);
uint32_t CFW_SetJammingDetectFlag(uint8_t nFlag, CFW_SIM_ID nSimID);
uint32_t CFW_SetJammingDetectParam(uint8_t nFlag, int16_t nLteThreshold, int16_t nGsmThreshold, CFW_SIM_ID nSimID);
uint32_t CFW_SetJammingDetectParamEx(uint8_t bEnable, uint8_t detectPeriod, int16_t rssiThresholdLte, int16_t rsrpThreshold, int16_t rsrqThreshold, int16_t rssiThreshold, int16_t snrThreshold, uint16_t minCh, CFW_SIM_ID nSimID);
uint32_t CFW_NwJammingRssiEnquiry(uint8_t nMode, uint16_t nUTI, CFW_SIM_ID nSimID);
uint32_t CFW_GetJammingDetect(uint16_t nUTI, CFW_SIM_ID nSimID);
uint32_t CFW_NwSetBlackListTimer(CFW_BlackListCntReq nBlackListCnt, CFW_SIM_ID nSimID);
#ifdef CONFIG_SOC_8811
uint8_t CFW_NwGetAutoAtt(CFW_SIM_ID nSimID);
#endif
/*! \brief function retrieves information about, or the count of, available operators.

The result should be returned through the EV_CFW_NW_GET_AVAIL_OPERATOR_RSP event:
\rst
+------+-------+---------------------------------------------------------------------------------------+---------------------------+-------------+
| UTI  | nType |                                        nParam1                                        |          nParam2          | Description |
+======+=======+=======================================================================================+===========================+=============+
| nUTI | 0x00  | Pointer to the CFW_NW_OPERATOR_INFO structure array to hold the operator information. | the count array structure | OK          |
+------+-------+---------------------------------------------------------------------------------------+---------------------------+-------------+
| nUTI | 0xF0  | See below                                                                             | 0                         | Error       |
+------+-------+---------------------------------------------------------------------------------------+---------------------------+-------------+
\endrst
when the nType is 0xF0, and the nParam1 indicate the error code which listed below:
\rst
+-------------------------------------------+---------------------------------------------------------------------+
|                  nParam1                  |                               nParam2                               |
+===========================================+=====================================================================+
| ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED | Network is not provided temporarily, maybe network is initializing. |
+-------------------------------------------+---------------------------------------------------------------------+
| ERR_CME_UNKNOWN                           | Other error.                                                        |
+-------------------------------------------+---------------------------------------------------------------------+
\endrst

\param [in] nUTI    UTI operation
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_NwGetAvailableOperators(
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

/*! \brief function retrieves information about, or the count of, available operators.

The result should be returned through the EV_CFW_NW_GET_AVAIL_OPERATOR_RSP event:
\rst
+------+-------+------------------------------------------------------------------------------------------+---------------------------+-------------+
| UTI  | nType |                                         nParam1                                          |          nParam2          | Description |
+======+=======+==========================================================================================+===========================+=============+
| nUTI | 0x00  | Pointer to the CFW_NW_OPERATOR_INFO_V2 structure array to hold the operator information. | the count array structure | OK          |
+------+-------+------------------------------------------------------------------------------------------+---------------------------+-------------+
| nUTI | 0xF0  | See below                                                                                | 0                         | Error       |
+------+-------+------------------------------------------------------------------------------------------+---------------------------+-------------+
\endrst
when the nType is 0xF0, and the nParam1 indicate the error code which listed below:
\rst
+-------------------------------------------+---------------------------------------------------------------------+
|                  nParam1                  |                               nParam2                               |
+===========================================+=====================================================================+
| ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED | Network is not provided temporarily, maybe network is initializing. |
+-------------------------------------------+---------------------------------------------------------------------+
| ERR_CME_UNKNOWN                           | Other error.                                                        |
+-------------------------------------------+---------------------------------------------------------------------+
\endrst

\param [in] nUTI    UTI operation
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_NwGetAvailableOperators_V2(
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

/*! \brief function retrieves information about the current operator.

\param [out] OperatorId     Specify the current operator/PLMN (Public Land Mobile Network) compose of MCC (Mobile country code) and MNC (Mobile network code) in BCD format (non-compressed format). If the length of the operator Id is 5,
the last byte of the operator Id should be 0x0F.
\param [out] pMode          Pointer to UINT8 type to contain the mode which shown below:
\rst
+------+------------------------------------------------------------------------------+
| mode |                                 Description                                  |
+======+==============================================================================+
| 0    | automatic mode                                                               |
+------+------------------------------------------------------------------------------+
| 1    | manual operator selection name field must be present, format can only be = 2 |
+------+------------------------------------------------------------------------------+
\endrst

nOperatorId Point to array of UINT8 type to contain the operator identifier, for example china mobile:

nOperatorId[0] = 0x04;

nOperatorId[1] = 0x06;

nOperatorId[2] = 0x00; nOperatorId[3] = 0x00;

nOperatorId[4] = 0x00; nOperatorId[5] = 0x0F;

nOperatorId[0-2] is the MCC, and nOperatorId[3-5] is the MNC

\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_NwGetCurrentOperator(
    uint8_t OperatorId[6],
    uint8_t *pMode,
    CFW_SIM_ID nSimID);

uint32_t CFW_NwGetImei(
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

uint32_t CFW_SimGetOperatorName(
    uint8_t pOperatorId[6],
    uint16_t nUTI,
    CFW_SIM_ID nSimID);
/*! \brief function retrieves the selected network carrier signal level and Bit error rate.

\param [out] pSignalLevel     Specify the network the signal level values shown below:
\rst
+---------------------+--------------------------------+
| signal level values |      Signal level in dbm       |
+=====================+================================+
| 0                   | -113 dBm or less(<= -113 dBm)  |
+---------------------+--------------------------------+
| 1                   | -111 dBm                       |
+---------------------+--------------------------------+
| 2 to 30             | -109 dBm to -53 dBm            |
+---------------------+--------------------------------+
| 31                  | -51 dBm or greater(>= -51 dBm) |
+---------------------+--------------------------------+
| 99                  | Unknown or undetectable        |
+---------------------+--------------------------------+
\endrst
\param [out] pBitError        Specify the Bit error rate shown below:
\rst
+----------------+-------------------------------+
| Bit error rate |            Meaning            |
+================+===============================+
| 0..7           | -113 dBm or less(<= -113 dBm) |
+----------------+-------------------------------+
| 99             | -111 dBm                      |
+----------------+-------------------------------+
\endrst
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_NwGetSignalQuality(
    uint8_t *pSignalLevel,
    uint8_t *pBitError,
    CFW_SIM_ID nSimID);

uint32_t CFW_NwGetLteSignalQuality(
    uint8_t *pSignalLevel,
    uint8_t *pBitError,
    CFW_SIM_ID nSimID);
uint32_t CFW_NwGetQualReport(
    CFW_NW_QUAL_INFO *pQualReport,
    CFW_SIM_ID nSimID);

/*! \brief function retrieves the status of GSM network.

\param [out] pStatusInfo    Pointer to CFW_NW_STATUS_INFO structure to contain the network status information.
\param [in]  nSimID         SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_NwGetStatus(
    CFW_NW_STATUS_INFO *pStatusInfo,
    CFW_SIM_ID nSimID);

#define NW_SET_AUTO_RAT 0xFF

/*! \brief function use to choose whether the GSM network operator is to be selected automatically or manually.

The result should be returned through the EV_CFW_NW_SET_REGISTRATION_RSP event:
\rst
+------+-------+-----------+-----------+-----------------------------------+
| UTI  | nType |  nParam1  |  nParam2  |            Description            |
+======+=======+===========+===========+===================================+
| nUTI | 0x00  | See below | see below | this function perform completely. |
+------+-------+-----------+-----------+-----------------------------------+
| nUTI | 0xF0  | See below | see below | Error                             |
+------+-------+-----------+-----------+-----------------------------------+
\endrst

nParam1: Specify the operator /PLMN (Public Land Mobile Network) compose of MCC (Mobile country code) and MNC (Mobile network code) in BCD format (non-compressed format).
If the length of the operator Id is 5, the last byte of the operator Id should be 0x0F.

nParam2: Pointer to UINT8 type to contain the mode which shown below:
\rst
+-----------------------+------------------------------------------------------------------------------+
|         mode          |                                 Description                                  |
+=======================+==============================================================================+
| CFW_NW_AUTOMATIC_MODE | automatic mode                                                               |
+-----------------------+------------------------------------------------------------------------------+
| CFW_NW_MANUAL_MODE    | manual operator selection name field must be present, format can only be = 2 |
+-----------------------+------------------------------------------------------------------------------+
\endrst

when the nType is 0xF0, and the nParam1 indicate the error code which listed below:
\rst
+-------------------------------------------------+------------------------------------------+
|                     Param1                      |               Description                |
+=================================================+==========================================+
| ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED       | Network is not provided temporarily.     |
+-------------------------------------------------+------------------------------------------+
| ERR_CME_NO_NETWORK_SERVICE                      | no network service.                      |
+-------------------------------------------------+------------------------------------------+
| ERR_CME_MEMORY_NOT_ALLOWED_EMERGENCY_CALLS_ONLY | Network not allowed emergency calls only |
+-------------------------------------------------+------------------------------------------+
| ERR_CME_UNKNOWN                                 | Other error.                             |
+-------------------------------------------------+------------------------------------------+
\endrst

\param [in] nOperatorId    Specify the identifier of the network operator to register, this parameter should be not terminated with zero and the size of operator id should be 6 bytes. This parameter can be ignored when mode=0,
in this case this function don't care this parameter at all.
\param [in] nMode          Specify the GSM network registration mode to select which shown below:
\param [in] nRat           The rat LTE or GSM
\param [in] nUTI    UTI operation
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_NwSetRegistration(
    uint8_t nOperatorId[6],
    uint8_t nMode,
    uint8_t nRat,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

typedef enum
{
    CFW_ALL_DETACH = 0x00,      ///< Detach PS and Detach CS
    CFW_ALL_ATTACH,             ///< Doing all attach (CS and PS)
    CFW_PS_ATTACH,              ///< PS attach
    CFW_PS_DETACH,              ///< PS detach
    CFW_INIT_STATE = 0xffffffff ///didn't attach and detach
} CFW_ATT_DETACH_STATUS;

/*! \brief function manually deregister from network and remain unregistered until mode 0,1,4 is selected.

The result should be returned through the EV_CFW_NW_DEREGISTER_RSP event:
\rst
+------+-------+------------+---------+-------------+
| UTI  | nType |  nParam1   | nParam2 | Description |
+======+=======+============+=========+=============+
| nUTI | 0x00  | 0          | 0       | OK          |
+------+-------+------------+---------+-------------+
| nUTI | 0xF0  | Error code | 0       | Error       |
+------+-------+------------+---------+-------------+
\endrst

\param [in] nUTI    UTI operation
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_NwDeRegister(
    uint16_t nUTI,
    CFW_SIM_ID nSimID);
uint32_t CFW_DetachCSAndPS(
    uint16_t nUTI,
    CFW_SIM_ID nSimID);
uint32_t CFW_AttachCSAndPS(
    uint16_t nUTI,
    CFW_SIM_ID nSimID);
uint32_t CFW_NwGetAllDetach(
    uint8_t *nAttDetach,
    CFW_SIM_ID nSimID);

uint32_t CFW_NwGetHNBName(
    uint8_t *pLength,
    uint8_t *pHNBName,
    CFW_SIM_ID nSimID);

typedef enum
{
    CFW_DISABLE_COMM = 0x00, ///< don't start
    CFW_ENABLE_COMM,         ///<  start stack
    CFW_CHECK_COMM,
    CFW_COMM_MODE_ENUM_FILL = 0x7fffffff
} CFW_COMM_MODE;

//nMode: 0 : CFW_DISABLE_COMM; 1: CFW_ENABLE_COMM; 2: CFW_CHECK_COMM
uint32_t CFW_SetComm(
    CFW_COMM_MODE nMode,
    uint8_t nStorageFlag,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

uint32_t CFW_GetComm(
    CFW_COMM_MODE *pMode,
    CFW_SIM_ID nSimID);

uint32_t CFW_CfgNwSetFM(
    CFW_COMM_MODE bMode,
    CFW_SIM_ID nSimID);

uint32_t CFW_CfgNwSetCgclassType(
    uint8_t nCgclassType,
    CFW_SIM_ID nSimID);

uint32_t CFW_CfgNwGetCgclassType(
    uint8_t *nCgclassType,
    CFW_SIM_ID nSimID);

/*! \brief function changes the frequency band.

The result should be returned through the EV_CFW_NW_SET_BAND_RSP event which shown below:
\rst
+-------+-----------+---------+-------------+
| nType |  nParam1  | nParam2 | Description |
+=======+===========+=========+=============+
| 0x00  | nBand     | 0       | OK          |
+-------+-----------+---------+-------------+
| 0xF0  | see below | 0       | Error       |
+-------+-----------+---------+-------------+
\endrst

When the nType is 0xF0, and the nParam1 indicate the error code which listed below:
\rst
+-------------------------------------------------+------------------------------------------+
|                     Param1                      |               Description                |
+=================================================+==========================================+
| ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED       | Network is not provided temporarily.     |
+-------------------------------------------------+------------------------------------------+
| ERR_CME_NO_NETWORK_SERVICE                      | no network service.                      |
+-------------------------------------------------+------------------------------------------+
| ERR_CME_MEMORY_NOT_ALLOWED_EMERGENCY_CALLS_ONLY | Network not allowed emergency calls only |
+-------------------------------------------------+------------------------------------------+
| ERR_CME_UNKNOWN                                 | Other error.                             |
+-------------------------------------------------+------------------------------------------+
\endrst

\param [in] nBand   Specify the frequency band to active.
\param [in] nUTI    UTI operation
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_NwSetFrequencyBand(
    uint8_t nBand,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

/*! \brief function abort the construction of the list of surrounding PLMNs retrieved.

The result should be returned through the EV_CFW_NW_ABORT_LIST_OPERATORS_RSP event:
\rst
+------+-------+------------+---------+-------------+
| nUTI | nType |  nParam1   | nParam2 | Description |
+======+=======+============+=========+=============+
| nUTI | 0x00  | 0          | 0       | OK          |
+------+-------+------------+---------+-------------+
| nUTI | 0xF0  | ERROR code | 0       | Error       |
+------+-------+------------+---------+-------------+
\endrst

\param [in] nUTI    UTI operation
\param [in] nSimID  SIM ID

\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+-------------------------------------------------+-----------------------------------------------------------+
|                     Param1                      |                        Description                        |
+=================================================+===========================================================+
| ERR_CFW_UTI_IS_BUSY                             | The UTI is busy, maybe the UTI is used by other instance. |
+-------------------------------------------------+-----------------------------------------------------------+
| ERR_CME_MEMORY_NOT_ALLOWED_EMERGENCY_CALLS_ONLY | Network not allowed emergency calls only                  |
+-------------------------------------------------+-----------------------------------------------------------+
| ERR_CME_UNKNOWN                                 | Other error.                                              |
+-------------------------------------------------+-----------------------------------------------------------+
\endrst
*/
uint32_t CFW_NwAbortListOperators(
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

bool CFW_GetNWTimerOutFlag(CFW_SIM_ID nSimID);
void CFW_SetNWTimerOutFlag(bool bClear, CFW_SIM_ID nSimID);
//Please use these two interfaces together;first call ScanInfo,then get call ScanEnd.
//Please use the same UTI.
uint32_t CFW_GetFreqScanInfo(uint16_t nUTI, CFW_SIM_ID nSimID);
uint32_t CFW_GetFreqScanEnd(uint16_t nUTI, CFW_SIM_ID nSimID);
uint32_t CFW_GetFreqScanAllInfo(uint16_t nUTI, CFW_SIM_ID nSimID);
uint32_t CFW_NwSetCSMode(uint8_t nMode, CFW_SIM_ID nSimID);

#define CFW_NW_BAND_GSM_450 (1 << 0)
#define CFW_NW_BAND_GSM_480 (1 << 1)
#define CFW_NW_BAND_GSM_900P (1 << 2)
#define CFW_NW_BAND_GSM_900E (1 << 3)
#define CFW_NW_BAND_GSM_900R (1 << 4)
#define CFW_NW_BAND_GSM_850 (1 << 5)
#define CFW_NW_BAND_DCS_1800 (1 << 6)
#define CFW_NW_BAND_PCS_1900 (1 << 7)

//
// CM Service
//

#define CFW_CM_MODE_VOICE 0
#define CFW_CM_MODE_DATA 1
#define CFW_CM_MODE_FAX 2

//Call direction
#define CFW_DIRECTION_MOBILE_ORIGINATED 0
#define CFW_DIRECTION_MOBILE_TERMINATED 1

//Call state
#define CFW_CM_STATUS_ACTIVE 0
#define CFW_CM_STATUS_HELD 1
#define CFW_CM_STATUS_DIALING 2  ///< MO CALL)
#define CFW_CM_STATUS_ALERTING 3 ///< (MO CALL)
#define CFW_CM_STATUS_INCOMING 4 ///< (MT CALL)
#define CFW_CM_STATUS_WAITING 5  ///< (MT call)
#define CFW_CM_STATUS_RELEASED 7
//#ifdef CFW_VOLTE_SUPPORT  //quectel modify
#define CFW_CM_STATUS_HANDSHAKE 8
//#endif //quectel modify

#define TEL_NUMBER_STRING_MAX_LEN 30
#define TEL_NUMBER_STRING_MAX_LEN_EX 30
typedef struct _CFW_SPEECH_CALL_IND
{
    CFW_TELNUMBER TelNumber;
    uint8_t nCode;
    uint8_t nCLIValidity;
    uint8_t nDisplayType;
    uint8_t nIndex;
    uint8_t calling_uri[TEL_NUMBER_STRING_MAX_LEN + 1];
} CFW_SPEECH_CALL_IND;

typedef struct _CFW_CC_CURRENT_CALL_INFO
{
    CFW_TELNUMBER dialNumber;
    bool multiparty;
    uint8_t idx;
    uint8_t direction;
    uint8_t status;
    uint8_t mode;
    uint8_t calling_uri[TEL_NUMBER_STRING_MAX_LEN + 1];
    uint8_t padding[3];
} CFW_CC_CURRENT_CALL_INFO;

typedef struct _CFW_CC_CURRENT_CALL_INFO_V2
{
    CFW_TELNUMBER dialNumber;
    bool multiparty;
    uint8_t idx;
    uint8_t direction;
    uint8_t status;
    uint8_t mode;
    uint8_t calling_uri[TEL_NUMBER_STRING_MAX_LEN_EX + 1];
    uint8_t padding[3];
} CFW_CC_CURRENT_CALL_INFO_V2;

//Remove CPP_KEYWORD_SUPPORT  by lixp for MMI compilation issue at 20090512.

typedef struct _CFW_SS_QUERY_CALLFORWARDING_RSP
{
    CFW_TELNUMBER nNumber;
    uint8_t nStatus;
    uint8_t nClass;
    uint8_t nReason;
    uint8_t nTime;
} CFW_SS_QUERY_CALLFORWARDING_RSP;
typedef struct _CFW_SS_SET_CALLFORWARDING_INFO
{
    uint8_t nReason;
    uint8_t nMode;
    uint8_t nClass;
    uint8_t nTime;
    CFW_DIALNUMBER_V2 nNumber;
} CFW_SS_SET_CALLFORWARDING_INFO;

typedef struct _CFW_SS_SET_CALLFORWARDING_RSP_INFO
{
    uint8_t nReason;
    uint8_t nMode;
    uint8_t nClass;
    uint8_t nTime;
    CFW_DIALNUMBER nNumber;
} CFW_SS_SET_CALLFORWARDING_RSP_INFO;
/*! \brief The function is used to accept an incoming voice call.

The result should be returned through the following \a EV_CFW_CC_ACCEPT_SPEECH_CALL_RSP event:

\rst
+------+-------+---------------------+----------------------------------------------+---------------+
| UTI  | nType |       nParam1       |                   nParam2                    |  Description  |
+======+=======+=====================+==============================================+===============+
| nUTI | 0x0F  | 0                   | 0                                            | OK            |
+------+-------+---------------------+----------------------------------------------+---------------+
|      |       |                     | HIUINT16(nParam2) = 0;                       |               |
| nUTI | 0x10  | ranging from 1 to 7 | LOUINT16(nParam2)=                           | MO Release    |
|      |       |                     | CFW_CC_CAUSE_RECOVERY_ON_TIMER_EXPIRY        | (Time expire) |
+------+-------+---------------------+----------------------------------------------+---------------+
|      |       |                     | HIUINT16(nParam2)=Call identification number |               |
|      |       |                     | LOUINT16(nParam2)=                           |               |
| nUTI | 0xF0  | Network cause       | CFW_CC_CAUSE_NORMAL_RELEASE or               | Error         |
|      |       |                     | CFW_CC_CAUSE_LOW_LAYER_FAIL                  |               |
+------+-------+---------------------+----------------------------------------------+---------------+
\endrst

\param [in] nSimID  SIM ID
\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+-------------------------------------------+--------------------------------------+
|                Error Code                 |             Description              |
+===========================================+======================================+
| ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED | There is no any incoming voice call. |
+-------------------------------------------+--------------------------------------+
| ERR_CME_MEMORY_FULL                       | No any more memory to use.           |
+-------------------------------------------+--------------------------------------+
\endrst
*/
uint32_t CFW_CcAcceptSpeechCall(
    CFW_SIM_ID nSimId);
/*! \brief The function is as well as CFW_CcAcceptSpeechCallEx.
*/
uint32_t CFW_CcAcceptSpeechCallEx(
    CFW_SIM_ID nSimId);
/*! \brief The function is used to get the status of current calls.
\param [in] nSimID  SIM ID
\return \a CC_STATE
*/
uint32_t CFW_CcGetCallStatus(
    CFW_SIM_ID nSimId);
/*! \brief The function is used to get the number of current calls.
\param [in] nSimID  SIM ID
\return \a number of current calls
*/
uint8_t CFW_GetCcCount(
    CFW_SIM_ID nSimID);
/*! \brief The function Gets a list of current calls or the count of them.
\param [out] CallInfo[7]  Specifies the CFW_CC_CURRENT_CALL_INFO structure array that holds the current speech call information.
\param [out] pCnt  Point to uint8_t type to retrieve the count of CallInfo.
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_CcGetCurrentCall(
    CFW_CC_CURRENT_CALL_INFO CallInfo[7],
    uint8_t *pCnt,
    CFW_SIM_ID nSimId);

/*! \brief The function Gets a list of current calls or the count of them.
\param [out] CallInfo[7]  Specifies the CFW_CC_CURRENT_CALL_INFO structure array that holds the current speech call information.
\param [out] pCnt  Point to uint8_t type to retrieve the count of CallInfo.
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_CcGetCurrentCallV2(
    CFW_CC_CURRENT_CALL_INFO_V2 CallInfo[7],
    uint8_t *pCnt,
    CFW_SIM_ID nSimId);

/*! \brief The function places all active calls, if any, on hold and accept another (incoming, waiting, or held) call, if any.

The result should be returned asynchronously through the EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP event:

\rst
+------+-------+-------------------+-------------------+-----------------+
| UTI  | nType |      nParam1      |      nParam2      |   Description   |
+======+=======+===================+===================+=================+
|      |       |                   |                   | Perform         |
| nUTI | 0x00  | 1 to 7.           | 0                 | successfully.   |
+------+-------+-------------------+-------------------+-----------------+
|      |       |                   |                   |                 |
| nUTI | 0x01  | 1 to 7.           | 0                 | release call    |
|      |       |                   |                   | successfully.   |
+------+-------+-------------------+-------------------+-----------------+
|      |       |                   |                   | anwser call     |
| nUTI | 0x0F  | 1 to 7.           | 0                 | successfully.   |
+------+-------+-------------------+-------------------+-----------------+
|      |       |                   |                   | MO Release(i.e. |
| nUTI | 0x10  | 1 to 7.           | 0                 | Time expire)    |
+------+-------+-------------------+-------------------+-----------------+
| nUTI | 0xF0  | Network Error cod | CC local error    | Error           |
+------+-------+-------------------+-------------------+-----------------+
|      |       |                   | HIUINT16=number   |                 |
|      |       | HIUINT16=ss_code  | HIUINT8(LOUINT16) |                 |
| nUTI | 0xF1  | LOUINT16=         | =problemTag       | Error           |
|      |       | ss_operations     | LOUINT8(LOUINT16) |                 |
|      |       |                   | =CRSS Error       |                 |
+------+-------+-------------------+-------------------+-----------------+
\endrst

the EV_CFW_CC_CALL_SWAP_RSP event:

\rst
+------+-------+---------+---------+---------------+
| UTI  | nType | nParam1 | nParam2 |  Description  |
+======+=======+=========+=========+===============+
|      |       |         |         | Perform       |
| nUTI | 0x00  | 1 to 7. | 0       | successfully. |
+------+-------+---------+---------+---------------+
\endrst

\param [in] nCmd   Specify the command which you can select a value below:
\rst
+------------------------+--------------------------------------------------------------+
|      nCmd, nIndex      |                         Description                          |
+========================+==============================================================+
|                        | Terminate all held calls;dor set User Determined User Busy   |
| nCmd=0, nIndex=0xFF    | for a waiting call,reject the waiting call                   |
+------------------------+--------------------------------------------------------------+
|                        | Terminate all active calls (if any) and accept the other     |
| nCmd=1, nIndex=0xFF    | call(waiting call or held call).This command isn't available |
|                        | when there is only one incoming call                         |
+------------------------+--------------------------------------------------------------+
| nCmd=1, nIndex=x(1-7)  | Terminate the call specified by nIndex (nIndex= 1-7).        |
+------------------------+--------------------------------------------------------------+
|                        | Place all active calls on hold(if any)and accept the         |
| nCmd=2, nIndex=0xFF    | other call (waiting call or held call) as the active         |
|                        | the waiting call will be accepted as active call at first).  |
+------------------------+--------------------------------------------------------------+
|                        | Place all active calls except call nIndex(nIndex= 1-7) on    |
| nCmd= 2, nIndex=x(1-7) | hold.                                                        |
+------------------------+--------------------------------------------------------------+
| nCmd=3, nIndex=0xFF    | Add the held call to the active calls.                       |
+------------------------+--------------------------------------------------------------+
\endrst
\param [in] nIndex   Specify the associated parameter see nCmd above.
\param [in] nSimId  SIM ID
\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+-------------------------------------------+---------------------------------+
|                Error Code                 |           Description           |
+===========================================+=================================+
| ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED | There is no voice call.         |
+-------------------------------------------+---------------------------------+
| ERR_CFW_INVALID_PARAMETER                 | The input parameter is invalid. |
+-------------------------------------------+---------------------------------+
\endrst
*/
uint32_t CFW_CcCallHoldMultiparty(
    uint8_t nCmd,
    uint8_t nIndex,
    CFW_SIM_ID nSimId);
uint32_t CFW_CcCallHoldMultiparty_V2(uint16_t nUTI, uint8_t nCmd, uint8_t nIndex, CFW_SIM_ID nSimId);

#ifdef LTE_SUPPORT
/*! \brief The function disconnects the index x call in progress, such as voice calls or CSD data calls.

The result should be returned asynchronously through the  EV_CFW_CC_RELEASE_CALL_RSP event:

\rst
+------+---------+-------------------+-------------------+----------------------+
| UTI  |  nType  |      nParam1      |      nParam2      |     Description      |
+======+=========+===================+===================+======================+
|      |         |                   |                   |                      |
| nUTI | 0x10/30 | 1 to 7.           | Network Error cod | OK                   |
+------+---------+-------------------+-------------------+----------------------+
| nUTI | 0x20    | 0                 | 0                 | When no any call,    |
|      |         |                   |                   | this function called |
+------+---------+-------------------+-------------------+----------------------+
|      |         |                   | HIUINT16=number   |                      |
| nUTI | 0xF0    | Network Error cod | LOUINT16 =        | Error                |
|      |         |                   | network casue     |                      |
+------+---------+-------------------+-------------------+----------------------+
\endrst

\param [in] nIndex  the index call
\param [in] nSimID  SIM ID
\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+-------------------------------------------+-------------------------------------+
|                Error Code                 |             Description             |
+===========================================+=====================================+
| ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED | Temporarily operate is not allowed. |
+-------------------------------------------+-------------------------------------+
\endrst
*/
uint32_t CFW_CcReleaseCallX(
    uint8_t nIndex,
    CFW_SIM_ID nSimId);

uint32_t CFW_CcReleaseCallX_V2(
    uint16_t nUTI,
    uint8_t nIndex,
    uint8_t cause,
    CFW_SIM_ID nSimId);
#endif
/*! \brief The function initiates a voice call.

The result should be returned asynchronously through the EV_CFW_CC_INITIATE_SPEECH_CALL_RSP event:

\rst
+------+-------+------------------------+----------------------+------------------+
| UTI  | nType |        nParam1         |       nParam2        |   Description    |
+======+=======+========================+======================+==================+
|      |       |                        |                      | call connect     |
| nUTI | 0x00  | 0                      | 0                    | successfully.    |
+------+-------+------------------------+----------------------+------------------+
|      |       | point CFW_TELNUMBER to |                      |                  |
| nUTI | 0x01  | hold the telephone     | 0                    | call connect     |
|      |       | number                 |                      | successfully.    |
+------+-------+------------------------+----------------------+------------------+
|      |       |                        | HIUINT16=0           | Error MO Release |
| nUTI | 0x10  | 1 to 7.                | LOUINT16=Network err |                  |
+------+-------+------------------------+----------------------+------------------+
|      |       |                        | HIUINT16=cc number   |                  |
| nUTI | 0xF0  | Network Error cod      | LOUINT16=local err   | Error            |
+------+-------+------------------------+----------------------+------------------+
\endrst

\param [in] pDialNumber   Pointer to the CFW_DIALNUMBER structure to dial
\param [in] nUTI    Specify the UTI (Unique Transaction Identifier) operation, which support multi-application in parallel. This parameter will
be return in notification event (see below). You also can get a free UTI by CFW_GetFreeUTI function if you application is a dynamical
module. This parameter should be less than 255 for all upper layer application.
\param [in] nSimID  SIM ID
\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+------------------------------+------------------------------------------------+
|          Error Code          |                  Description                   |
+==============================+================================================+
| ERR_CFW_INVALID_PARAMETER    | The input parameter is NULL                    |
+------------------------------+------------------------------------------------+
| ERR_CME_DIAL_STRING_TOO_LONG | The input dial number string is tool long(>40) |
+------------------------------+------------------------------------------------+
| ERR_CFW_NOT_EXIST_FREE_UTI   | No any available free UTI in the system.       |
+------------------------------+------------------------------------------------+
| ERR_CME_MEMORY_FULL          | No any more memory to use.                     |
+------------------------------+------------------------------------------------+
\endrst
*/
uint32_t CFW_CcInitiateSpeechCall(
    CFW_DIALNUMBER *pDialNumber,
    uint16_t nUTI,
    CFW_SIM_ID nSimId);
/*! \brief The function initiates a voice call.

The result should be returned asynchronously through the EV_CFW_CC_INITIATE_SPEECH_CALL_RSP event:

\rst
+------+-------+------------------------+----------------------+------------------+
| UTI  | nType |        nParam1         |       nParam2        |   Description    |
+======+=======+========================+======================+==================+
|      |       |                        |                      | call connect     |
| nUTI | 0x00  | 0                      | 0                    | successfully.    |
+------+-------+------------------------+----------------------+------------------+
|      |       | point CFW_TELNUMBER to |                      |                  |
| nUTI | 0x01  | hold the telephone     | 0                    | call connect     |
|      |       | number                 |                      | successfully.    |
+------+-------+------------------------+----------------------+------------------+
|      |       |                        | HIUINT16=0           | Error MO Release |
| nUTI | 0x10  | 1 to 7.                | LOUINT16=Network err |                  |
+------+-------+------------------------+----------------------+------------------+
|      |       |                        | HIUINT16=cc number   |                  |
| nUTI | 0xF0  | Network Error cod      | LOUINT16=local err   | Error            |
+------+-------+------------------------+----------------------+------------------+
\endrst

\param [in] pDialNumber   Pointer to the CFW_DIALNUMBER_V2 structure to dial
\param [in] nUTI    Specify the UTI (Unique Transaction Identifier) operation, which support multi-application in parallel. This parameter will
be return in notification event (see below). You also can get a free UTI by CFW_GetFreeUTI function if you application is a dynamical
module. This parameter should be less than 255 for all upper layer application.
\param [in] nSimID  SIM ID
\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+------------------------------+------------------------------------------------+
|          Error Code          |                  Description                   |
+==============================+================================================+
| ERR_CFW_INVALID_PARAMETER    | The input parameter is NULL                    |
+------------------------------+------------------------------------------------+
| ERR_CME_DIAL_STRING_TOO_LONG | The input dial number string is tool long(>40) |
+------------------------------+------------------------------------------------+
| ERR_CFW_NOT_EXIST_FREE_UTI   | No any available free UTI in the system.       |
+------------------------------+------------------------------------------------+
| ERR_CME_MEMORY_FULL          | No any more memory to use.                     |
+------------------------------+------------------------------------------------+
\endrst
*/

uint32_t CFW_CcInitiateSpeechCall_V2(
    CFW_DIALNUMBER_V2 *pDialNumber,
    uint16_t nUTI,
    CFW_SIM_ID nSimId);
/*! \brief The function initiates a voice call.

The result should be returned asynchronously through the EV_CFW_CC_INITIATE_SPEECH_CALL_RSP event:

\rst
+------+-------+------------------------+----------------------+------------------+
| UTI  | nType |        nParam1         |       nParam2        |   Description    |
+======+=======+========================+======================+==================+
|      |       |                        |                      | call connect     |
| nUTI | 0x00  | 0                      | 0                    | successfully.    |
+------+-------+------------------------+----------------------+------------------+
|      |       | point CFW_TELNUMBER to |                      |                  |
| nUTI | 0x01  | hold the telephone     | 0                    | call connect     |
|      |       | number                 |                      | successfully.    |
+------+-------+------------------------+----------------------+------------------+
|      |       |                        | HIUINT16=0           | Error MO Release |
| nUTI | 0x10  | 1 to 7.                | LOUINT16=Network err |                  |
+------+-------+------------------------+----------------------+------------------+
|      |       |                        | HIUINT16=cc number   |                  |
| nUTI | 0xF0  | Network Error cod      | LOUINT16=local err   | Error            |
+------+-------+------------------------+----------------------+------------------+
\endrst

\param [in]  pDialNumber   Pointer to the CFW_DIALNUMBER structure to dial
\param [out] pIndex  the index of the setup call
\param [in]  nUTI    Specify the UTI (Unique Transaction Identifier) operation, which support multi-application in parallel. This parameter will
be return in notification event (see below). You also can get a free UTI by CFW_GetFreeUTI function if you application is a dynamical
module. This parameter should be less than 255 for all upper layer application.
\param [in] nSimID  SIM ID
\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+------------------------------+------------------------------------------------+
|          Error Code          |                  Description                   |
+==============================+================================================+
| ERR_CFW_INVALID_PARAMETER    | The input parameter is NULL                    |
+------------------------------+------------------------------------------------+
| ERR_CME_DIAL_STRING_TOO_LONG | The input dial number string is tool long(>40) |
+------------------------------+------------------------------------------------+
| ERR_CFW_NOT_EXIST_FREE_UTI   | No any available free UTI in the system.       |
+------------------------------+------------------------------------------------+
| ERR_CME_MEMORY_FULL          | No any more memory to use.                     |
+------------------------------+------------------------------------------------+
\endrst
*/
uint32_t CFW_CcInitiateSpeechCallEx(
    CFW_DIALNUMBER *pDialNumber,
    uint8_t *pIndex,
    uint16_t nUTI,
    CFW_SIM_ID nSimId);
/*! \brief The function initiates a voice call.

The result should be returned asynchronously through the EV_CFW_CC_INITIATE_SPEECH_CALL_RSP event:

\rst
+------+-------+------------------------+----------------------+------------------+
| UTI  | nType |        nParam1         |       nParam2        |   Description    |
+======+=======+========================+======================+==================+
|      |       |                        |                      | call connect     |
| nUTI | 0x00  | 0                      | 0                    | successfully.    |
+------+-------+------------------------+----------------------+------------------+
|      |       | point CFW_TELNUMBER to |                      |                  |
| nUTI | 0x01  | hold the telephone     | 0                    | call connect     |
|      |       | number                 |                      | successfully.    |
+------+-------+------------------------+----------------------+------------------+
|      |       |                        | HIUINT16=0           | Error MO Release |
| nUTI | 0x10  | 1 to 7.                | LOUINT16=Network err |                  |
+------+-------+------------------------+----------------------+------------------+
|      |       |                        | HIUINT16=cc number   |                  |
| nUTI | 0xF0  | Network Error cod      | LOUINT16=local err   | Error            |
+------+-------+------------------------+----------------------+------------------+
\endrst

\param [in]  pDialNumber   Pointer to the CFW_DIALNUMBER_V2 structure to dial
\param [out] pIndex  the index of the setup call
\param [in]  nUTI    Specify the UTI (Unique Transaction Identifier) operation, which support multi-application in parallel. This parameter will
be return in notification event (see below). You also can get a free UTI by CFW_GetFreeUTI function if you application is a dynamical
module. This parameter should be less than 255 for all upper layer application.
\param [in] nSimID  SIM ID
\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+------------------------------+------------------------------------------------+
|          Error Code          |                  Description                   |
+==============================+================================================+
| ERR_CFW_INVALID_PARAMETER    | The input parameter is NULL                    |
+------------------------------+------------------------------------------------+
| ERR_CME_DIAL_STRING_TOO_LONG | The input dial number string is tool long(>40) |
+------------------------------+------------------------------------------------+
| ERR_CFW_NOT_EXIST_FREE_UTI   | No any available free UTI in the system.       |
+------------------------------+------------------------------------------------+
| ERR_CME_MEMORY_FULL          | No any more memory to use.                     |
+------------------------------+------------------------------------------------+
\endrst
*/
uint32_t CFW_CcInitiateSpeechCallEx_V2(
    CFW_DIALNUMBER_V2 *pDialNumber,
    uint8_t *pIndex,
    uint16_t nUTI,
    CFW_SIM_ID nSimId);
/*! \brief The function calls the emergency service
\param [in] pNumber  Pointer to uint8_t to hold the number in GSM BCD format.
\param [in] nSize    The size of telephone number, the MAX. value of this parameter is 2.
\param [in] nSimID   SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_CcEmcDial(
    uint8_t *pNumber,
    uint8_t nSize,
    CFW_SIM_ID nSimId);
/*! \brief The function calls the emergency service
\param [in] pNumber  Pointer to uint8_t to hold the number in GSM BCD format.
\param [in] nDialNumberSize    The size of telephone number, the MAX. value of this parameter is 2.
\param [out] pIndex  the index of the setup call
\param [in] nSimID   SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_CcEmcDialEx(
    uint8_t *pDialNumber,
    uint8_t nDialNumberSize,
    uint8_t *pIndex,
    CFW_SIM_ID nSimId);

uint32_t CFW_CcEmcDialEx_V2(
    uint8_t *pDialNumber,
    uint8_t nDialNumberSize,
    uint8_t *pIndex,
    CFW_CcEmergCateg emergCateg,
    CFW_SIM_ID nSimId);
/*! \brief The function disconnects any call in progress, such as voice calls or CSD data calls.

The result should be returned asynchronously through the  EV_CFW_CC_RELEASE_CALL_RSP event:

\rst
+------+---------+-------------------+-------------------+----------------------+
| UTI  |  nType  |      nParam1      |      nParam2      |     Description      |
+======+=========+===================+===================+======================+
|      |         |                   |                   |                      |
| nUTI | 0x10/30 | 1 to 7.           | Network Error cod | OK                   |
+------+---------+-------------------+-------------------+----------------------+
| nUTI | 0x20    | 0                 | 0                 | When no any call,    |
|      |         |                   |                   | this function called |
+------+---------+-------------------+-------------------+----------------------+
|      |         |                   | HIUINT16=number   |                      |
| nUTI | 0xF0    | Network Error cod | LOUINT16 =        | Error                |
|      |         |                   | network casue     |                      |
+------+---------+-------------------+-------------------+----------------------+
\endrst

\param [in] nSimID  SIM ID
\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+-------------------------------------------+-------------------------------------+
|                Error Code                 |             Description             |
+===========================================+=====================================+
| ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED | Temporarily operate is not allowed. |
+-------------------------------------------+-------------------------------------+
\endrst
*/
uint32_t CFW_CcReleaseCall(
    CFW_SIM_ID nSimId);

uint32_t CFW_CcReleaseCall_V2(uint16_t nUTI,
                              CFW_SIM_ID nSimId);
/*! \brief The function disconnects any call in progress, such as voice calls or CSD data calls.

The result should be returned asynchronously through the  EV_CFW_CC_RELEASE_CALL_RSP event:

\rst
+------+-------+------------------+--------------------+-------------+
| UTI  | nType |     nParam1      |      nParam2       | Description |
+======+=======+==================+====================+=============+
|      |       |                  |                    |             |
| nUTI | 0x10  | 1 to 7.          | Network Error cod  | OK          |
+------+-------+------------------+--------------------+-------------+
|      |       |                  | HIUINT16=cc number |             |
| nUTI | 0xF0  | network casue:16 | LOUINT16 =         | Error       |
|      |       |                  | network casue      |             |
+------+-------+------------------+--------------------+-------------+
\endrst

\param [in] nSimID  SIM ID
\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+-------------------------------------------+-------------------------------------+
|                Error Code                 |             Description             |
+===========================================+=====================================+
| ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED | Temporarily operate is not allowed. |
+-------------------------------------------+-------------------------------------+
\endrst
*/
uint32_t CFW_CcRejectCall(
    CFW_SIM_ID nSimId);
/*! \brief The function sends DTMF tones over the GSM network.
\param [in] iTone    A single ASCII character in the set of 0-9, #, *, and A-D to send.
\param [in] nSimID   SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_CcPlayTone(
    int8_t iTone,
    CFW_SIM_ID nSimId);
/*! \brief The function stops to send DTMF tones over the GSM network.
\param [in] nSimID   SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_CcStopTone(
    CFW_SIM_ID nSimId);

uint32_t CFW_CcSetCsfbmtFlag(uint8_t nFlag, CFW_SIM_ID nSim);
uint32_t CFW_CcGetCsfbmtFlag(CFW_SIM_ID nSim);

#ifdef AT_EXT_CONF_SUPPORT
uint32_t CFW_CcGetMptyNum(
    uint8_t *pMptyCnt,
    CFW_SIM_ID nSimId);

uint32_t CFW_CcExtConfCallDial(
    CFW_DIAL_EXT_CONF_CT *pDialNumber,
    uint8_t *pIndex,
    uint16_t nUTI,
    CFW_SIM_ID nSimId);

uint32_t CFW_CcExtConfCallAdd(
    CFW_DIAL_EXT_CONF_CT *pDialNumber,
    uint16_t nUTI,
    CFW_SIM_ID nSimId);
#endif

/*! \brief The CFW_SsQueryCallWaiting function interrogates the status of call waiting.

The result should be returned through the EV_CFW_SS_QUERY_CALL_WAITING_RSP event:

\rst
+-------+---------------------------------------------+----------------------------------+------------------+
| nType |                   nParam1                   |             nParam2              |   Description    |
+=======+=============================================+==================================+==================+
|       | Bit0:VOICE,Bit1:DATA,BIT2:FAX,BIT3:SMS...   | bit0 = 1: indicate VOICE class   |                  |
|       | the status                                  | bit1 = 1: indicate data class    |                  |
| 0x00  | bit0 = 0: VOICE is not active               | bit2 = 1: indicate fax class     | OK               |
|       | bit0 = 1: VOICE is active    ...            | bit3 = 1: indicate sms class     |                  |
+-------+---------------------------------------------+----------------------------------+------------------+
|       | HIUINT16= ss_code(ss_CW = 0x41)             | HIUINT16(nParam2)=0              |                  |
| 0xFA  | LOUINT16=ss_operations(SS_Interrogate = 14) | LOUINT16(nParam2)=LocalErrorCode | LocalError       |
+-------+---------------------------------------------+----------------------------------+------------------+
|       | HIUINT16= ss_code(ss_CW = 0x41)             | HIUINT16(nParam2)=0              | Nwk returned a   |
| 0xFB  | LOUINT16=ss_operations(SS_Interrogate = 14) | LOUINT16(nParam2)=NWErrorCode    | return error     |
+-------+---------------------------------------------+----------------------------------+------------------+
|       | HIUINT16= ss_code(ss_CW = 0x41)             | HIUINT16(nParam2)=ProblemCodeTag | NW returned a    |
| 0xFC  | LOUINT16=ss_operations(SS_Interrogate = 14) | LOUINT16(nParam2)=ProblemCode    | reject component |
+-------+---------------------------------------------+----------------------------------+------------------+
|       | HIUINT16= ss_code(ss_CW = 0x41)             | HIUINT16(nParam2)=0              | Nwk returned a   |
| 0xFD  | LOUINT16=ss_operations(SS_Interrogate = 14) | LOUINT16(nParam2)=causevalue     | Cause  error     |
+-------+---------------------------------------------+----------------------------------+------------------+
\endrst

\param [in] nClass Specify the sum of class to represent the class information which show below: See MMI Service Code For more
\rst
+-------+------------------------------------+
| Class |            Description             |
+=======+====================================+
| 11    | Telephony, voice service.          |
+-------+------------------------------------+
| 13    | Facsimile services                 |
+-------+------------------------------------+
| 16    | Short Message Services             |
+-------+------------------------------------+
| 20    | All bearer services, data service. |
+-------+------------------------------------+
\endrst
\param [in] nUTI    Specify the UTI (Unique Transaction Identifier) operation, which support multi-application in parallel. This parameter will
be return in notification event (see below). You also can get a free UTI by CFW_GetFreeUTI function if you application is a dynamical
module. This parameter should be less than 255 for all upper layer application.
\param [in] nSimID  SIM ID
\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+----------------------------+------------------------------------------------------------------+
|         Error Code         |                           Description                            |
+============================+==================================================================+
| ERR_CFW_INVALID_PARAMETER  | The input parameter(nIndex) is invalid.                          |
+----------------------------+------------------------------------------------------------------+
| ERR_CFW_NOT_EXIST_FREE_UTI | No any available free UTI in the system                          |
+----------------------------+------------------------------------------------------------------+
|                            | Indicates the event could not be immediately placed in the queue |
| ERR_COS_QUEUE_FULL         | because there was not enough space available, so this function   |
|                            | fail to send event to CFW.                                       |
+----------------------------+------------------------------------------------------------------+
| ERR_CME_MEMORY_FULL        | No any more memory to allocate                                   |
+----------------------------+------------------------------------------------------------------+
| ERR_CME_UNKNOWN            | Unknown error occur.                                             |
+----------------------------+------------------------------------------------------------------+
\endrst
*/
uint32_t CFW_SsQueryCallWaiting(
    uint8_t nClass,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

/*! \brief The CFW_SsSetCallWaiting function controls the Call Waiting supplementary service according to 3GPP TS 22.083 [5].

The result should be returned through the  EV_CFW_SS_SET_CALL_WAITING_RSP event:

\rst
+-------+---------------------------------+----------------------------------+------------------+
| nType |             nParam1             |             nParam2              |   Description    |
+=======+=================================+==================================+==================+
|       | mode = 1:active                 |                                  |                  |
| 0x00  | mode = 0:deactive               | 0                                | OK               |
+-------+---------------------------------+----------------------------------+------------------+
|       | HIUINT16= ss_code(ss_CW = 0x41) |                                  |                  |
|       | LOUINT16=ss_operations          | HIUINT16(nParam2)=0              |                  |
| 0xFA  | (mode = 1:SS_Activate = 12,     | LOUINT16(nParam2)=LocalErrorCode | LocalError       |
|       | mode = 0:SS_Deactivate = 13)    |                                  |                  |
+-------+---------------------------------+----------------------------------+------------------+
|       | HIUINT16= ss_code(ss_CW = 0x41) |                                  |                  |
|       | LOUINT16=ss_operations          | HIUINT16(nParam2)=0              | Nwk returned a   |
| 0xFB  | (mode = 1:SS_Activate = 12,     | LOUINT16(nParam2)=NWErrorCode    | return error     |
|       | mode = 0:SS_Deactivate = 13)    |                                  |                  |
+-------+---------------------------------+----------------------------------+------------------+
|       | HIUINT16= ss_code(ss_CW = 0x41) |                                  |                  |
|       | LOUINT16=ss_operations          | HIUINT16(nParam2)=ProblemCodeTag | NW returned a    |
| 0xFC  | (mode = 1:SS_Activate = 12,     | LOUINT16(nParam2)=ProblemCode    | reject component |
|       | mode = 0:SS_Deactivate = 13)    |                                  |                  |
+-------+---------------------------------+----------------------------------+------------------+
|       | HIUINT16= ss_code(ss_CW = 0x41) |                                  |                  |
|       | LOUINT16=ss_operations          | HIUINT16(nParam2)=0              | Nwk returned a   |
| 0xFD  | (mode = 1:SS_Activate = 12,     | LOUINT16(nParam2)=causevalue     | Cause IE         |
|       | mode = 0:SS_Deactivate = 13)    |                                  |                  |
+-------+---------------------------------+----------------------------------+------------------+
\endrst
\param [in] nMode  Specify the value to disable(0)/enable(1) call waiting.
\param [in] nClass Specify the sum of class to represent the class information which show below: See MMI Service Code For more
\rst
+-------+------------------------------------+
| Class |            Description             |
+=======+====================================+
| 11    | Telephony, voice service.          |
+-------+------------------------------------+
| 13    | Facsimile services                 |
+-------+------------------------------------+
| 16    | Short Message Services             |
+-------+------------------------------------+
| 20    | All bearer services, data service. |
+-------+------------------------------------+
\endrst
\param [in] nUTI    Specify the UTI (Unique Transaction Identifier) operation, which support multi-application in parallel. This parameter will
be return in notification event (see below). You also can get a free UTI by CFW_GetFreeUTI function if you application is a dynamical
module. This parameter should be less than 255 for all upper layer application.
\param [in] nSimID  SIM ID
\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+----------------------------+------------------------------------------------------------------+
|         Error Code         |                           Description                            |
+============================+==================================================================+
| ERR_CFW_INVALID_PARAMETER  | The input parameter(nIndex) is invalid.                          |
+----------------------------+------------------------------------------------------------------+
| ERR_CFW_NOT_EXIST_FREE_UTI | No any available free UTI in the system                          |
+----------------------------+------------------------------------------------------------------+
|                            | Indicates the event could not be immediately placed in the queue |
| ERR_COS_QUEUE_FULL         | because there was not enough space available, so this function   |
|                            | fail to send event to CFW.                                       |
+----------------------------+------------------------------------------------------------------+
| ERR_CME_UNKNOWN            | Unknown error occur.                                             |
+----------------------------+------------------------------------------------------------------+
\endrst
*/
uint32_t CFW_SsSetCallWaiting(
    uint8_t nMode,
    uint8_t nClass,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

/*! \brief The CFW_SsSetCallForwarding function sets the call forwarding number and conditions.

The result should be returned through the EV_CFW_SS_SET_CALL_FORWARDING_RSP event

\rst
+-------+------------------------------------------+----------------------------------+------------------+
| nType |                 nParam1                  |             nParam2              |   Description    |
+=======+==========================================+==================================+==================+
|       | Point to CFW_SS_SET_CALLFORWARDING_INFO  |                                  |                  |
| 0x00  | structure array referenced by nParam1.   | 0                                | OK               |
+-------+------------------------------------------+----------------------------------+------------------+
|       | HIUINT16= ss_code(ss_CFA = 0x20,ss_CFU = |                                  |                  |
|       | 0x21, ss_CFC = 0x28, ss_CFB = 0x29,      |                                  |                  |
|       | ss_CFNRY = 0x2A, ss_CFNRC = 0x2B)        |                                  |                  |
|       | LOUINT16=ss_operations                   | HIUINT16(nParam2)=0              |                  |
| 0xFA  | (mode = 1:SS_Activate = 12,              | LOUINT16(nParam2)=LocalErrorCode | LocalError       |
|       | mode = 0:SS_Deactivate = 13)             |                                  |                  |
+-------+------------------------------------------+----------------------------------+------------------+
|       | HIUINT16= ss_code(ss_CFA = 0x20,ss_CFU = |                                  |                  |
|       | 0x21, ss_CFC = 0x28, ss_CFB = 0x29,      |                                  |                  |
|       | ss_CFNRY = 0x2A, ss_CFNRC = 0x2B)        |                                  |                  |
|       | LOUINT16=ss_operations                   | HIUINT16(nParam2)=0              | Nwk returned a   |
| 0xFB  | (mode = 1:SS_Activate = 12,              | LOUINT16(nParam2)=NWErrorCode    | return error     |
|       | mode = 0:SS_Deactivate = 13)             |                                  |                  |
+-------+------------------------------------------+----------------------------------+------------------+
|       | HIUINT16= ss_code(ss_CFA = 0x20,ss_CFU = |                                  |                  |
|       | 0x21, ss_CFC = 0x28, ss_CFB = 0x29,      |                                  |                  |
|       | ss_CFNRY = 0x2A, ss_CFNRC = 0x2B)        |                                  |                  |
|       | LOUINT16=ss_operations                   | HIUINT16(nParam2)=ProblemCodeTag | NW returned a    |
| 0xFC  | (mode = 1:SS_Activate = 12,              | LOUINT16(nParam2)=ProblemCode    | reject component |
|       | mode = 0:SS_Deactivate = 13)             |                                  |                  |
+-------+------------------------------------------+----------------------------------+------------------+
|       | HIUINT16= ss_code(ss_CFA = 0x20,ss_CFU = |                                  |                  |
|       | 0x21, ss_CFC = 0x28, ss_CFB = 0x29,      |                                  |                  |
|       | ss_CFNRY = 0x2A, ss_CFNRC = 0x2B)        |                                  |                  |
|       | LOUINT16=ss_operations                   | HIUINT16(nParam2)=0              | Nwk returned a   |
| 0xFD  | (mode = 1:SS_Activate = 12,              | LOUINT16(nParam2)=causevalue     | Cause IE         |
|       | mode = 0:SS_Deactivate = 13)             |                                  |                  |
+-------+------------------------------------------+----------------------------------+------------------+
\endrst

\param [in] pCallForwarding  Pointer to an array of CFW_SS_SET_CALLFORWARDING_INFO structure to contain the forwarding number and conditions.
\param [in] nUTI    Specify the UTI (Unique Transaction Identifier) operation, which support multi-application in parallel. This parameter will
be return in notification event (see below). You also can get a free UTI by CFW_GetFreeUTI function if you application is a dynamical
module. This parameter should be less than 255 for all upper layer application.
\param [in] nSimID  SIM ID
\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+----------------------------+------------------------------------------------------------------+
|         Error Code         |                           Description                            |
+============================+==================================================================+
| ERR_CFW_INVALID_PARAMETER  | The input parameter(nIndex) is invalid.                          |
+----------------------------+------------------------------------------------------------------+
| ERR_CFW_NOT_EXIST_FREE_UTI | No any available free UTI in the system                          |
+----------------------------+------------------------------------------------------------------+
|                            | Indicates the event could not be immediately placed in the queue |
| ERR_COS_QUEUE_FULL         | because there was not enough space available, so this function   |
|                            | fail to send event to CFW.                                       |
+----------------------------+------------------------------------------------------------------+
| ERR_CME_UNKNOWN            | Unknown error occur.                                             |
+----------------------------+------------------------------------------------------------------+
| ERR_CME_MEMORY_FULL        | No any more memory to allocate                                   |
+----------------------------+------------------------------------------------------------------+
\endrst
*/
uint32_t CFW_SsSetCallForwarding(
    CFW_SS_SET_CALLFORWARDING_INFO *pCallForwarding,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

/*! \brief The CFW_SsQueryCallForwarding function interrogates the call forwarding number and conditions.

The result should be returned through the EV_CFW_SS_QUERY_CALL_FORWARDING_RSP event:

\rst
+-------+------------------------------------------+----------------------------------+------------------+
| nType |                 nParam1                  |             nParam2              |   Description    |
+=======+==========================================+==================================+==================+
|       | Point to CFW_SS_QUERY_CALLFORWARDING_RSP |                                  |                  |
| 0x00  | structure array referenced by nParam1.   | The counter of structure array   | OK               |
+-------+------------------------------------------+----------------------------------+------------------+
|       | HIUINT16= ss_code(ss_CFA = 0x20,ss_CFU = |                                  |                  |
|       | 0x21, ss_CFC = 0x28, ss_CFB = 0x29,      |                                  |                  |
|       | ss_CFNRY = 0x2A, ss_CFNRC = 0x2B)        |                                  |                  |
|       | LOUINT16=ss_operations                   | HIUINT16(nParam2)=0              |                  |
| 0xFA  | (mode = 1:SS_Activate = 12,              | LOUINT16(nParam2)=LocalErrorCode | LocalError       |
|       | mode = 0:SS_Deactivate = 13              |                                  |                  |
|       | mode = 3:SS_Deactivate = 10              |                                  |                  |
|       | mode = 4:SS_Deactivate = 11)             |                                  |                  |
+-------+------------------------------------------+----------------------------------+------------------+
|       | HIUINT16= ss_code(ss_CFA = 0x20,ss_CFU = |                                  |                  |
|       | 0x21, ss_CFC = 0x28, ss_CFB = 0x29,      |                                  |                  |
|       | ss_CFNRY = 0x2A, ss_CFNRC = 0x2B)        |                                  |                  |
|       | LOUINT16=ss_operations                   | HIUINT16(nParam2)=0              | Nwk returned a   |
| 0xFB  | (mode = 1:SS_Activate = 12,              | LOUINT16(nParam2)=NWErrorCode    | return error     |
|       | mode = 0:SS_Deactivate = 13              |                                  |                  |
|       | mode = 3:SS_Deactivate = 10              |                                  |                  |
|       | mode = 4:SS_Deactivate = 11)             |                                  |                  |
+-------+------------------------------------------+----------------------------------+------------------+
|       | HIUINT16= ss_code(ss_CFA = 0x20,ss_CFU = |                                  |                  |
|       | 0x21, ss_CFC = 0x28, ss_CFB = 0x29,      |                                  |                  |
|       | ss_CFNRY = 0x2A, ss_CFNRC = 0x2B)        |                                  |                  |
|       | LOUINT16=ss_operations                   | HIUINT16(nParam2)=ProblemCodeTag | NW returned a    |
| 0xFC  | (mode = 0:SS_Activate = 12,              | LOUINT16(nParam2)=ProblemCode    | reject component |
|       | mode = 0:SS_Deactivate = 13              |                                  |                  |
|       | mode = 3:SS_Deactivate = 10              |                                  |                  |
|       | mode = 4:SS_Deactivate = 11)             |                                  |                  |
+-------+------------------------------------------+----------------------------------+------------------+
|       | HIUINT16= ss_code(ss_CFA = 0x20,ss_CFU = |                                  |                  |
|       | 0x21, ss_CFC = 0x28, ss_CFB = 0x29,      |                                  |                  |
|       | ss_CFNRY = 0x2A, ss_CFNRC = 0x2B)        |                                  |                  |
|       | LOUINT16=ss_operations                   | HIUINT16(nParam2)=0              | Nwk returned a   |
| 0xFD  | (mode = 1:SS_Activate = 12,              | LOUINT16(nParam2)=causevalue     | Cause IE         |
|       | mode = 0:SS_Deactivate = 13              |                                  |                  |
|       | mode = 3:SS_Deactivate = 10              |                                  |                  |
|       | mode = 4:SS_Deactivate = 11)             |                                  |                  |
+-------+------------------------------------------+----------------------------------+------------------+
\endrst

\param [in] nReason Specify one of the forwarding reasons to interrogate. It can be any one of the following values:
\rst
+----------------------------------------+-------+--------------------------------------+
|           Forwarding Reason            | Value |             Description              |
+========================================+=======+======================================+
| CFW_CM_FORWARDING_REASON_UNCONDITIONAL | 0     | Forward unconditionally.             |
+----------------------------------------+-------+--------------------------------------+
| CFW_CM_FORWARDING_REASON_MOBILEBUSY    | 1     | Forward if the mobile phone is busy  |
+----------------------------------------+-------+--------------------------------------+
| CFW_CM_FORWARDING_REASON_NOREPLY       | 2     | Forward if there is no answer        |
+----------------------------------------+-------+--------------------------------------+
| CFW_CM_FORWARDING_REASON_NOTREACHABLE  | 3     | Forward if the number is unreachable |
+----------------------------------------+-------+--------------------------------------+
\endrst
\param [in] nClass Specify the sum of class to represent the class information which show below: See MMI Service Code For more
\rst
+-------+------------------------------------+
| Class |            Description             |
+=======+====================================+
| 11    | Telephony, voice service.          |
+-------+------------------------------------+
| 13    | Facsimile services                 |
+-------+------------------------------------+
| 16    | Short Message Services             |
+-------+------------------------------------+
| 20    | All bearer services, data service. |
+-------+------------------------------------+
\endrst
\param [in] nUTI    Specify the UTI (Unique Transaction Identifier) operation, which support multi-application in parallel. This parameter will
be return in notification event (see below). You also can get a free UTI by CFW_GetFreeUTI function if you application is a dynamical
module. This parameter should be less than 255 for all upper layer application.
\param [in] nSimID  SIM ID
\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+----------------------------+------------------------------------------------------------------+
|         Error Code         |                           Description                            |
+============================+==================================================================+
| ERR_CFW_INVALID_PARAMETER  | The input parameter(nIndex) is invalid.                          |
+----------------------------+------------------------------------------------------------------+
| ERR_CFW_NOT_EXIST_FREE_UTI | No any available free UTI in the system                          |
+----------------------------+------------------------------------------------------------------+
|                            | Indicates the event could not be immediately placed in the queue |
| ERR_COS_QUEUE_FULL         | because there was not enough space available, so this function   |
|                            | fail to send event to CFW.                                       |
+----------------------------+------------------------------------------------------------------+
| ERR_CME_UNKNOWN            | Unknown error occur.                                             |
+----------------------------+------------------------------------------------------------------+
| ERR_CME_MEMORY_FULL        | No any more memory to allocate                                   |
+----------------------------+------------------------------------------------------------------+
\endrst
*/
uint32_t CFW_SsQueryCallForwarding(
    uint8_t nReason,
    uint8_t nClass,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

#define CFW_STY_FAC_TYPE_AO 0x414F ///< BAOC (Bar All Outgoing Calls)
#define CFW_STY_FAC_TYPE_OI 0x4F49 ///< BOIC (Bar Outgoing International Calls)
#define CFW_STY_FAC_TYPE_OX 0x4F58 ///< BOIC-exHC (Bar Outgoing International Calls except to Home Country)
#define CFW_STY_FAC_TYPE_AI 0x4149 ///< BAIC (Bar All Incoming Calls)
#define CFW_STY_FAC_TYPE_IR 0x4952 ///< BIC-Roam (Bar Incoming Calls when Roaming outside the home country)
#define CFW_STY_FAC_TYPE_AB 0x4142 ///< All Barring services (only applicable for mode=0)
#define CFW_STY_FAC_TYPE_AG 0x4147 ///< All outGoing barring services (only applicable for mode=0)
#define CFW_STY_FAC_TYPE_AC 0x4143 ///< All inComing barring services (only applicable for mode=0)

#define CFW_CM_MODE_UNLOCK 0 ///< unlock
#define CFW_CM_MODE_LOCK 1   ///< lock

#define CFW_SS_FORWARDING_REASON_UNCONDITIONAL 0          ///< Forward unconditionally.
#define CFW_SS_FORWARDING_REASON_MOBILEBUSY 1             ///< Forward if the mobile phone is busy
#define CFW_SS_FORWARDING_REASON_NOREPLY 2                ///< Forward if there is no answer
#define CFW_SS_FORWARDING_REASON_NOTREACHABLE 3           ///< Forward if the number is unreachable.
#define CFW_SS_FORWARDING_REASON_ALL_CALLFORWARDING 4     ///< All call forwarding reasons(includes reasons 0, 1, 2 and 3).
#define CFW_SS_FORWARDING_REASON_ALL_CONDCALLFORWARDING 5 ///< All conditional call forwarding reasons(includes reasons 1, 2 and 3).
//#ifdef CFW_VOLTE_SUPPORT //quectel modify
#define CFW_SS_FORWARDING_REASON_COMMUNICATION_DEFLECT 6 ///< communication deflection
#define CFW_SS_FORWARDING_REASON_NOT_LOGGED_IN 7         ///< communication forwarding on not logged-in
//#endif //quectel modify
#define CFW_SS_FORWARDING_MODE_DISABLE 0
#define CFW_SS_FORWARDING_MODE_ENABLE 1
#define CFW_SS_FORWARDING_MODE_QUERY 2
#define CFW_SS_FORWARDING_MODE_REGISTRATION 3
#define CFW_SS_FORWARDING_MODE_ERASURE 4

/*! \brief The CFW_SsSetFacilityLock function locks, unlocks a MT or a network facility such as call barring.

The result should be returned through the EV_CFW_SS_SET_FACILITY_LOCK_RSP event:

\rst
+-------+------------------------------------+---------------------+------------------+
| nType |              nParam1               |       nParam2       |   Description    |
+=======+====================================+=====================+==================+
|       | HIUINT16 (nParam1)=fac             | HIUINT16=class      |                  |
| 0x00  | LOUINT16(nParam1)=mode             | LOUINT16=status     | OK               |
+-------+------------------------------------+---------------------+------------------+
|       | HIUINT16= ss_code                  |                     |                  |
|       | (ss_AllBarringSS=0x90,             |                     |                  |
|       | ss_BarringOfOutgoingCalls=0x91,    |                     |                  |
|       | ss_BAOC=0x92,ss_BOIC=0x93,         |                     |                  |
|       | ss_BOIC_ExHC = 0x94,               |                     |                  |
|       | ss_BarringOfIncomingCalls=0x99,    | HIUINT16(nParam2)=0 |                  |
| 0xFA  | ss_BAIC = 0x9A,ss_BIC_ROAM = 0x9B) | LOUINT16(nParam2)=  | LocalError       |
|       | LOUINT16 (nParam1)=ss_operations   | LocalErrorCode      |                  |
|       | (mode = 1:SS_Activate = 12,        |                     |                  |
|       | mode = 0:SS_Deactivate = 13)       |                     |                  |
+-------+------------------------------------+---------------------+------------------+
|       | HIUINT16= ss_code                  |                     |                  |
|       | (ss_AllBarringSS=0x90,             |                     |                  |
|       | ss_BarringOfOutgoingCalls=0x91,    |                     |                  |
|       | ss_BAOC=0x92,ss_BOIC=0x93,         |                     |                  |
|       | ss_BOIC_ExHC = 0x94,               |                     |                  |
|       | ss_BarringOfIncomingCalls=0x99,    | HIUINT16(nParam2)=0 | Nwk returned a   |
| 0xFB  | ss_BAIC = 0x9A,ss_BIC_ROAM = 0x9B) | LOUINT16(nParam2)=  | return error     |
|       | LOUINT16 (nParam1)=ss_operations   | NWErrorCode         |                  |
|       | (mode = 1:SS_Activate = 12,        |                     |                  |
|       | mode = 0:SS_Deactivate = 13)       |                     |                  |
+-------+------------------------------------+---------------------+------------------+
|       | HIUINT16= ss_code                  |                     |                  |
|       | (ss_AllBarringSS=0x90,             |                     |                  |
|       | ss_BarringOfOutgoingCalls=0x91,    |                     |                  |
|       | ss_BAOC=0x92,ss_BOIC=0x93,         |                     |                  |
|       | ss_BOIC_ExHC = 0x94,               | HIUINT16(nParam2)=  |                  |
|       | ss_BarringOfIncomingCalls=0x99,    | ProblemCodeTag      | NW returned a    |
| 0xFC  | ss_BAIC = 0x9A,ss_BIC_ROAM = 0x9B) | LOUINT16(nParam2)=  | reject component |
|       | LOUINT16 (nParam1)=ss_operations   | ProblemCode         |                  |
|       | (mode = 1:SS_Activate = 12,        |                     |                  |
|       | mode = 0:SS_Deactivate = 13)       |                     |                  |
+-------+------------------------------------+---------------------+------------------+
|       | HIUINT16= ss_code                  |                     |                  |
|       | (ss_AllBarringSS=0x90,             |                     |                  |
|       | ss_BarringOfOutgoingCalls=0x91,    |                     |                  |
|       | ss_BAOC=0x92,ss_BOIC=0x93,         |                     |                  |
|       | ss_BOIC_ExHC = 0x94,               |                     |                  |
|       | ss_BarringOfIncomingCalls=0x99,    | HIUINT16(nParam2)=0 | Nwk returned a   |
| 0xFD  | ss_BAIC = 0x9A,ss_BIC_ROAM = 0x9B) | LOUINT16(nParam2)=  | Cause IE         |
|       | LOUINT16 (nParam1)=ss_operations   | causevalue          |                  |
|       | (mode = 1:SS_Activate = 12,        |                     |                  |
|       | mode = 0:SS_Deactivate = 13)       |                     |                  |
+-------+------------------------------------+---------------------+------------------+
\endrst

\param [in] nFac Specify the facility to check its status. The facility type can be one of the following value:
\rst
+---------------------+--------+---------------------------------------------------------------------+
|    Facility type    | Value  |                             Description                             |
+=====================+========+=====================================================================+
| CFW_STY_FAC_TYPE_AO | 0x414F | BAOC (Bar All Outgoing Calls)                                       |
+---------------------+--------+---------------------------------------------------------------------+
| CFW_STY_FAC_TYPE_OI | 0x4F49 | BOIC (Bar Outgoing International Cal                                |
+---------------------+--------+---------------------------------------------------------------------+
| CFW_STY_FAC_TYPE_OX | 0x4F58 | BOIC-exHC (Bar Outgoing International Calls except to Home Country) |
+---------------------+--------+---------------------------------------------------------------------+
| CFW_STY_FAC_TYPE_AI | 0x4149 | BAIC (Bar All Incoming Calls)                                       |
+---------------------+--------+---------------------------------------------------------------------+
| CFW_STY_FAC_TYPE_IR | 0x4952 | BIC-Roam (Bar Incoming Calls when Roaming outside the home country) |
+---------------------+--------+---------------------------------------------------------------------+
| CFW_STY_FAC_TYPE_AB | 0x4142 | All Barring services (applicable only for nMode=0)                  |
+---------------------+--------+---------------------------------------------------------------------+
| CFW_STY_FAC_TYPE_AG | 0x4147 | All outGoing barring services (applicable only for nMode=0)         |
+---------------------+--------+---------------------------------------------------------------------+
| CFW_STY_FAC_TYPE_AC | 0x4143 | All inComing barring services (applicable only for nMode=0)         |
+---------------------+--------+---------------------------------------------------------------------+
\endrst
\param [in] pBufPwd  Point to the buffer containing the password in ASCII string format. If you don't care password, set this pBufPwd to NULL.
\param [in] nPwdSize The size of pBufPwdin bytes. If pBufPwd is NULL, this parameter is ignored by this function.
\param [in] nClass Specify the sum of class to represent the class information which show below: See MMI Service Code For more
\rst
+-------+------------------------------------+
| Class |            Description             |
+=======+====================================+
| 11    | Telephony, voice service.          |
+-------+------------------------------------+
| 13    | Facsimile services                 |
+-------+------------------------------------+
| 16    | Short Message Services             |
+-------+------------------------------------+
| 20    | All bearer services, data service. |
+-------+------------------------------------+
\endrst
\param [in] nMode  Specify the mode which show below:
\rst
+--------------------+-------+-----------------+
|       nMode        | Value |   Description   |
+====================+=======+=================+
| CFW_CM_MODE_UNLOCK | 0     | unlock,deactive |
+--------------------+-------+-----------------+
| CFW_CM_MODE_LOCK   | 1     | lock,active     |
+--------------------+-------+-----------------+
\endrst
\param [in] nUTI    Specify the UTI (Unique Transaction Identifier) operation, which support multi-application in parallel. This parameter will
be return in notification event (see below). You also can get a free UTI by CFW_GetFreeUTI function if you application is a dynamical
module. This parameter should be less than 255 for all upper layer application.
\param [in] nSimID  SIM ID
\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+-------------------------------+------------------------------------------------------------------+
|          Error Code           |                           Description                            |
+===============================+==================================================================+
| ERR_CFW_INVALID_PARAMETER     | The input parameter(nIndex) is invalid.                          |
+-------------------------------+------------------------------------------------------------------+
|                               | This function don't complete, and you execute the  same          |
| ERR_CME_OPERATION_NOT_ALLOWED | operations such as call this function again.                     |
+-------------------------------+------------------------------------------------------------------+
| ERR_CFW_NOT_EXIST_FREE_UTI    | No any available free UTI in the system                          |
+-------------------------------+------------------------------------------------------------------+
|                               | Indicates the event could not be immediately placed in the queue |
| ERR_COS_QUEUE_FULL            | because there was not enough space available, so this function   |
|                               | fail to send event to CFW.                                       |
+-------------------------------+------------------------------------------------------------------+
| ERR_CME_UNKNOWN               | Unknown error occur.                                             |
+-------------------------------+------------------------------------------------------------------+
| ERR_CME_MEMORY_FULL           | No any more memory to allocate                                   |
+-------------------------------+------------------------------------------------------------------+
\endrst
*/
uint32_t CFW_SsSetFacilityLock(
    uint16_t nFac,
    uint8_t *pBufPwd,
    uint8_t nPwdSize,
    uint8_t nClass,
    uint8_t nMode,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

/*! \brief The CFW_SsQueryFacilityLock function retrieves the status of supplementary Service about Call barring.

The result should be returned through the the EV_CFW_SS_QUERY_FACILITY_LOCK_RSP event:

\rst
+-------+------------------------------------+---------------------+------------------+
| nType |              nParam1               |       nParam2       |   Description    |
+=======+====================================+=====================+==================+
|       | Specify the facility               | HIUINT16=class      |                  |
| 0x00  | see nFac parameter below.          | LOUINT16=status     | OK               |
+-------+------------------------------------+---------------------+------------------+
|       | HIUINT16= ss_code                  |                     |                  |
|       | (ss_AllBarringSS=0x90,             |                     |                  |
|       | ss_BarringOfOutgoingCalls=0x91,    |                     |                  |
|       | ss_BAOC=0x92,ss_BOIC=0x93,         |                     |                  |
|       | ss_BOIC_ExHC = 0x94,               |                     |                  |
|       | ss_BarringOfIncomingCalls=0x99,    | HIUINT16(nParam2)=0 |                  |
| 0xFA  | ss_BAIC = 0x9A,ss_BIC_ROAM = 0x9B) | LOUINT16(nParam2)=  | LocalError       |
|       | LOUINT16 (nParam1)=ss_operations   | LocalErrorCode      |                  |
|       | (SS_GetCallBarringPassword = 18)   |                     |                  |
+-------+------------------------------------+---------------------+------------------+
|       | HIUINT16= ss_code                  |                     |                  |
|       | (ss_AllBarringSS=0x90,             |                     |                  |
|       | ss_BarringOfOutgoingCalls=0x91,    |                     |                  |
|       | ss_BAOC=0x92,ss_BOIC=0x93,         |                     |                  |
|       | ss_BOIC_ExHC = 0x94,               |                     |                  |
|       | ss_BarringOfIncomingCalls=0x99,    | HIUINT16(nParam2)=0 | Nwk returned a   |
| 0xFB  | ss_BAIC = 0x9A,ss_BIC_ROAM = 0x9B) | LOUINT16(nParam2)=  | return error     |
|       | LOUINT16 (nParam1)=ss_operations   | NWErrorCode         |                  |
|       | (SS_GetCallBarringPassword = 18)   |                     |                  |
+-------+------------------------------------+---------------------+------------------+
|       | HIUINT16= ss_code                  |                     |                  |
|       | (ss_AllBarringSS=0x90,             |                     |                  |
|       | ss_BarringOfOutgoingCalls=0x91,    |                     |                  |
|       | ss_BAOC=0x92,ss_BOIC=0x93,         |                     |                  |
|       | ss_BOIC_ExHC = 0x94,               | HIUINT16(nParam2)=  |                  |
|       | ss_BarringOfIncomingCalls=0x99,    | ProblemCodeTag      | NW returned a    |
| 0xFC  | ss_BAIC = 0x9A,ss_BIC_ROAM = 0x9B) | LOUINT16(nParam2)=  | reject component |
|       | LOUINT16 (nParam1)=ss_operations   | ProblemCode         |                  |
|       | (SS_GetCallBarringPassword = 18)   |                     |                  |
+-------+------------------------------------+---------------------+------------------+
|       | HIUINT16= ss_code                  |                     |                  |
|       | (ss_AllBarringSS=0x90,             |                     |                  |
|       | ss_BarringOfOutgoingCalls=0x91,    |                     |                  |
|       | ss_BAOC=0x92,ss_BOIC=0x93,         |                     |                  |
|       | ss_BOIC_ExHC = 0x94,               |                     |                  |
|       | ss_BarringOfIncomingCalls=0x99,    | HIUINT16(nParam2)=0 | Nwk returned a   |
| 0xFD  | ss_BAIC = 0x9A,ss_BIC_ROAM = 0x9B) | LOUINT16(nParam2)=  | Cause IE         |
|       | LOUINT16 (nParam1)=ss_operations   | causevalue          |                  |
|       | (SS_GetCallBarringPassword = 18)   |                     |                  |
+-------+------------------------------------+---------------------+------------------+
\endrst

\param [in] nFac Specify the facility to check its status. The facility type can be one of the following value:
\rst
+---------------------+--------+---------------------------------------------------------------------+
|    Facility type    | Value  |                             Description                             |
+=====================+========+=====================================================================+
| CFW_STY_FAC_TYPE_AO | 0x414F | BAOC (Bar All Outgoing Calls)                                       |
+---------------------+--------+---------------------------------------------------------------------+
| CFW_STY_FAC_TYPE_OI | 0x4F49 | BOIC (Bar Outgoing International Cal                                |
+---------------------+--------+---------------------------------------------------------------------+
| CFW_STY_FAC_TYPE_OX | 0x4F58 | BOIC-exHC (Bar Outgoing International Calls except to Home Country) |
+---------------------+--------+---------------------------------------------------------------------+
| CFW_STY_FAC_TYPE_AI | 0x4149 | BAIC (Bar All Incoming Calls)                                       |
+---------------------+--------+---------------------------------------------------------------------+
| CFW_STY_FAC_TYPE_IR | 0x4952 | BIC-Roam (Bar Incoming Calls when Roaming outside the home country) |
+---------------------+--------+---------------------------------------------------------------------+
\endrst
\param [in] nClass Specify the sum of class to represent the class information which show below: See MMI Service Code For more
\rst
+-------+------------------------------------+
| Class |            Description             |
+=======+====================================+
| 11    | Telephony, voice service.          |
+-------+------------------------------------+
| 13    | Facsimile services                 |
+-------+------------------------------------+
| 16    | Short Message Services             |
+-------+------------------------------------+
| 20    | All bearer services, data service. |
+-------+------------------------------------+
\endrst
\param [in] nUTI    Specify the UTI (Unique Transaction Identifier) operation, which support multi-application in parallel. This parameter will
be return in notification event (see below). You also can get a free UTI by CFW_GetFreeUTI function if you application is a dynamical
module. This parameter should be less than 255 for all upper layer application.
\param [in] nSimID  SIM ID
\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+----------------------------+-----------------------------------------+
|         Error Code         |               Description               |
+============================+=========================================+
| ERR_CFW_INVALID_PARAMETER  | The input parameter(nIndex) is invalid. |
+----------------------------+-----------------------------------------+
| ERR_CFW_NOT_EXIST_FREE_UTI | No any available free UTI in the system |
+----------------------------+-----------------------------------------+
| ERR_CME_MEMORY_FULL        | No any more memory to allocate          |
+----------------------------+-----------------------------------------+
\endrst
*/
uint32_t CFW_SsQueryFacilityLock(
    uint16_t nFac,
    uint8_t nClass,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

/*! \brief The CFW_SsChangePassword function unlocks the call barring.

The result should be returned through the EV_CFW_SS_CHANGE_PWD_RSP event:

\rst
+-------+-----------------------------------+---------------------+------------------+
| nType |              nParam1              |       nParam2       |   Description    |
+=======+===================================+=====================+==================+
| 0x00  | 0                                 | 0                   | OK               |
+-------+-----------------------------------+---------------------+------------------+
|       | HIUINT16(nParam1)=                |                     |                  |
|       | ss_code(ss_AllBarringSS = 0x90)   |                     |                  |
|       | ss_BarringOfIncomingCalls=0x99,   | HIUINT16(nParam2)=0 |                  |
| 0xFA  | LOUINT16 (nParam1)=               | LOUINT16(nParam2)=  | LocalError       |
|       | ss_operations(SS_RegisterPassword | LocalErrorCode      |                  |
|       | = 17)                             |                     |                  |
+-------+-----------------------------------+---------------------+------------------+
|       | HIUINT16(nParam1)=                |                     |                  |
|       | ss_code(ss_AllBarringSS = 0x90)   |                     |                  |
|       | ss_BarringOfIncomingCalls=0x99,   | HIUINT16(nParam2)=0 | Nwk returned a   |
| 0xFB  | LOUINT16 (nParam1)=               | LOUINT16(nParam2)=  | return error     |
|       | ss_operations(SS_RegisterPassword | NWErrorCode         |                  |
|       | = 17)                             |                     |                  |
+-------+-----------------------------------+---------------------+------------------+
|       | HIUINT16(nParam1)=                |                     |                  |
|       | ss_code(ss_AllBarringSS = 0x90)   | HIUINT16(nParam2)=  |                  |
|       | ss_BarringOfIncomingCalls=0x99,   | ProblemCodeTag      | NW returned a    |
| 0xFC  | LOUINT16 (nParam1)=               | LOUINT16(nParam2)=  | reject component |
|       | ss_operations(SS_RegisterPassword | ProblemCode         |                  |
|       | = 17)                             |                     |                  |
+-------+-----------------------------------+---------------------+------------------+
|       | HIUINT16(nParam1)=                |                     |                  |
|       | ss_code(ss_AllBarringSS = 0x90)   |                     |                  |
|       | ss_BarringOfIncomingCalls=0x99,   | HIUINT16(nParam2)=0 | Nwk returned a   |
| 0xFD  | LOUINT16 (nParam1)=               | LOUINT16(nParam2)=  | Cause IE         |
|       | ss_operations(SS_RegisterPassword | causevalue          |                  |
|       | = 17)                             |                     |                  |
+-------+-----------------------------------+---------------------+------------------+
\endrst

\param [in] nFac Specify the facility(call barring) to check its status. The facility type can be one of the following value:
\rst
+---------------------+--------+---------------------------------------------------------------------+
|    Facility type    | Value  |                             Description                             |
+=====================+========+=====================================================================+
| CFW_STY_FAC_TYPE_AO | 0x414F | BAOC (Bar All Outgoing Calls)                                       |
+---------------------+--------+---------------------------------------------------------------------+
| CFW_STY_FAC_TYPE_OI | 0x4F49 | BOIC (Bar Outgoing International Cal                                |
+---------------------+--------+---------------------------------------------------------------------+
| CFW_STY_FAC_TYPE_OX | 0x4F58 | BOIC-exHC (Bar Outgoing International Calls except to Home Country) |
+---------------------+--------+---------------------------------------------------------------------+
| CFW_STY_FAC_TYPE_AI | 0x4149 | BAIC (Bar All Incoming Calls)                                       |
+---------------------+--------+---------------------------------------------------------------------+
| CFW_STY_FAC_TYPE_IR | 0x4952 | BIC-Roam (Bar Incoming Calls when Roaming outside the home country) |
+---------------------+--------+---------------------------------------------------------------------+
| CFW_STY_FAC_TYPE_AB | 0x4142 | All Barring services                                                |
+---------------------+--------+---------------------------------------------------------------------+
| CFW_STY_FAC_TYPE_AG | 0x4147 | All outGoing barring services                                       |
+---------------------+--------+---------------------------------------------------------------------+
| CFW_STY_FAC_TYPE_AC | 0x4143 | All inComing barring services                                       |
+---------------------+--------+---------------------------------------------------------------------+
\endrst
\param [in] pBufOldPwd Point to the buffer containing the password in ASCII string format for the facility. This parameter can be ignored
if no old password was allocated to the facility.Take into account that a password may have already been set by factory, or that the
service is subject to a password issued by the provider.if nFac = CFW_STY_FAC_TYPE_AO ..CFW_STY_FAC_TYPE_AC (barring) then network password(if needed)
\param [in] nOldPwdSize The size of pBufPwdin bytes. If pBufPwd is NULL, this parameter is ignored by this function.
\param [in] pBufNewPwd  Point to the buffer containing the new password in ASCII string format.
\param [in] nNewPwdSize The size of pBufPwdin bytes.
\param [in] nUTI    Specify the UTI (Unique Transaction Identifier) operation, which support multi-application in parallel. This parameter will
be return in notification event (see below). You also can get a free UTI by CFW_GetFreeUTI function if you application is a dynamical
module. This parameter should be less than 255 for all upper layer application.
\param [in] nSimID  SIM ID
\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+-------------------------------+---------------------------------------------------------+
|          Error Code           |                       Description                       |
+===============================+=========================================================+
| ERR_CFW_INVALID_PARAMETER     | The input parameter(nIndex) is invalid.                 |
+-------------------------------+---------------------------------------------------------+
|                               | This function don't complete, and you execute the  same |
| ERR_CME_OPERATION_NOT_ALLOWED | operations such as call this function again.            |
+-------------------------------+---------------------------------------------------------+
\endrst
*/
uint32_t CFW_SsChangePassword(
    uint16_t nFac,
    uint8_t *pBufOldPwd,
    uint8_t nOldPwdSize,
    uint8_t *pBufNewPwd,
    uint8_t nNewPwdSize,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

/*! \brief The CFW_SsQueryClip function querys the Calling line identification presentation

The result should be returned through the EV_CFW_SS_QUERY_CLIP_RSP event:
\rst
+-------+-----------------------------------+-------------------------------+------------------+
| nType |              nParam1              |            nParam2            |   Description    |
+=======+===================================+===============================+==================+
|       |                                   | Specify the parameter shows   |                  |
|       | 0 means don't presentation        | the subscriber CLIP service   |                  |
|       | of the CLIP                       | status in the network:        |                  |
| 0x00  | 1: indicate presentation          | 0:CLIP not provisioned        | Interrogation OK |
|       | of the CLIP                       | 1:CLIP provisioned in         |                  |
|       |                                   | 2.unknown                     |                  |
+-------+-----------------------------------+-------------------------------+------------------+
|       | HIUINT16(nParam1)=                |                               |                  |
|       | ss_code(ss_CLIP = 0x11)           | HIUINT16(nParam2)=0           |                  |
| 0xFA  | LOUINT16 (nParam1)=               | LOUINT16(nParam2)=            | LocalError       |
|       | ss_operations(SS_Interrogate= 14) | LocalErrorCode                |                  |
+-------+-----------------------------------+-------------------------------+------------------+
|       | HIUINT16(nParam1)=                |                               |                  |
|       | ss_code(ss_CLIP = 0x11)           | HIUINT16(nParam2)=0           | Nwk returned a   |
| 0xFB  | LOUINT16 (nParam1)=               | LOUINT16(nParam2)=NWErrorCode | return error     |
|       | ss_operations(SS_Interrogate= 14) |                               |                  |
+-------+-----------------------------------+-------------------------------+------------------+
|       | HIUINT16(nParam1)=                |                               |                  |
|       | ss_code(ss_CLIP = 0x11)           | HIUINT16(nParam2)=            |                  |
|       | LOUINT16 (nParam1)=               | ProblemCodeTag                | NW returned a    |
| 0xFC  | ss_operations(SS_Interrogate= 14) | LOUINT16(nParam2)=ProblemCode | reject component |
+-------+-----------------------------------+-------------------------------+------------------+
|       | HIUINT16(nParam1)=                |                               |                  |
|       | ss_code(ss_CLIP = 0x11)           | HIUINT16(nParam2)=0           | Nwk returned a   |
| 0xFD  | LOUINT16 (nParam1)=               | LOUINT16(nParam2)=causevalue  | Cause IE         |
|       | ss_operations(SS_Interrogate= 14) |                               |                  |
+-------+-----------------------------------+-------------------------------+------------------+
\endrst

\param [in] nUTI    Specify the UTI (Unique Transaction Identifier) operation, which support multi-application in parallel. This parameter will
be return in notification event (see below). You also can get a free UTI by CFW_GetFreeUTI function if you application is a dynamical
module. This parameter should be less than 255 for all upper layer application.
\param [in] nSimID  SIM ID
\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+----------------------------+------------------------------------------------------------------+
|         Error Code         |                           Description                            |
+============================+==================================================================+
| ERR_CFW_NOT_EXIST_FREE_UTI | No any available free UTI in the system                          |
+----------------------------+------------------------------------------------------------------+
|                            | Indicates the event could not be immediately placed in the queue |
| ERR_COS_QUEUE_FULL         | because there was not enough space available, so this function   |
|                            | fail to send event to CFW.                                       |
+----------------------------+------------------------------------------------------------------+
| ERR_CME_UNKNOWN            | Unknown error occur.                                             |
+----------------------------+------------------------------------------------------------------+
\endrst
*/

uint32_t CFW_SsQueryClip(
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

/*! \brief The CFW_SsQueryClir function allows to query the status which a calling subscriber enable or disable the presentation of the CLI to the called party when originating a call.

The result should be returned through one of the EV_CFW_SS_QUERY_CLIR_RSP event:
\rst
+-------+-----------------------------------+-------------------------------+------------------+
| nType |              nParam1              |            nParam2            |   Description    |
+=======+===================================+===============================+==================+
|       | Specify the parameter the         | Specify the parameter shows   |                  |
|       | adjustment for outgoing calls:    | the subscriber CLIR service   |                  |
|       | 0:Presentation indicator is used  | status in the network:        |                  |
|       | according  to the subscription of | 0:CLIR not provisioned        |                  |
| 0x00  | the CLIR service.                 | 1:CLIR provisioned in         | OK               |
|       | 1:CLIR invocation. Activate CLIR. | permanent mode 2.unknown      |                  |
|       | Disable presentation of own phon  | 3.temporary mode presentation |                  |
|       | number to called party.           | restricted 4.CLIR temporary   |                  |
|       |                                   | mode presentation allowed     |                  |
+-------+-----------------------------------+-------------------------------+------------------+
|       | HIUINT16(nParam1)=                |                               |                  |
|       | ss_code(ss_CLIR = 0x12)           | HIUINT16(nParam2)=0           |                  |
| 0xFA  | LOUINT16 (nParam1)=               | LOUINT16(nParam2)=            | LocalError       |
|       | ss_operations(SS_Interrogate= 14) | LocalErrorCode                |                  |
+-------+-----------------------------------+-------------------------------+------------------+
|       | HIUINT16(nParam1)=                |                               |                  |
|       | ss_code(ss_CLIR = 0x12)           | HIUINT16(nParam2)=0           | Nwk returned a   |
| 0xFB  | LOUINT16 (nParam1)=               | LOUINT16(nParam2)=NWErrorCode | return error     |
|       | ss_operations(SS_Interrogate= 14) |                               |                  |
+-------+-----------------------------------+-------------------------------+------------------+
|       | HIUINT16(nParam1)=                |                               |                  |
|       | ss_code(ss_CLIR = 0x12)           | HIUINT16(nParam2)=            |                  |
|       | LOUINT16 (nParam1)=               | ProblemCodeTag                | NW returned a    |
| 0xFC  | ss_operations(SS_Interrogate= 14) | LOUINT16(nParam2)=ProblemCode | reject component |
+-------+-----------------------------------+-------------------------------+------------------+
|       | HIUINT16(nParam1)=                |                               |                  |
|       | ss_code(ss_CLIR = 0x12)           | HIUINT16(nParam2)=0           | Nwk returned a   |
| 0xFD  | LOUINT16 (nParam1)=               | LOUINT16(nParam2)=causevalue  | Cause IE         |
|       | ss_operations(SS_Interrogate= 14) |                               |                  |
+-------+-----------------------------------+-------------------------------+------------------+
\endrst

\param [in] nUTI    Specify the UTI (Unique Transaction Identifier) operation, which support multi-application in parallel. This parameter will
be return in notification event (see below). You also can get a free UTI by CFW_GetFreeUTI function if you application is a dynamical
module. This parameter should be less than 255 for all upper layer application.
\param [in] nSimID  SIM ID
\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+----------------------------+------------------------------------------------------------------+
|         Error Code         |                           Description                            |
+============================+==================================================================+
| ERR_CFW_NOT_EXIST_FREE_UTI | No any available free UTI in the system                          |
+----------------------------+------------------------------------------------------------------+
|                            | Indicates the event could not be immediately placed in the queue |
| ERR_COS_QUEUE_FULL         | because there was not enough space available, so this function   |
|                            | fail to send event to CFW.                                       |
+----------------------------+------------------------------------------------------------------+
| ERR_CME_UNKNOWN            | Unknown error occur.                                             |
+----------------------------+------------------------------------------------------------------+
\endrst
*/
uint32_t CFW_SsQueryClir(
    uint16_t nUTI,
    CFW_SIM_ID nSimID);
/*! \brief The CFW_SsQueryColp function retrieves the setting of  the  Connected Line Identification Presentation (COLP) of the called party after setting up a mobile originated call. See 3GPP TS 22.081 [3] for more.

The result should be returned through the EV_CFW_SS_QUERY_COLP_RSP event:
\rst
+-------+-----------------------------------+-------------------------------+------------------+
| nType |              nParam1              |            nParam2            |   Description    |
+=======+===================================+===============================+==================+
|       |                                   | 0:COLP   not provisioned      |                  |
| 0x00  | 0: Disable to show the COLP       | 1:COLP  provisioned in        | Interrogation OK |
|       | 1: Enable  to show the COLP       | 2.unknown                     |                  |
+-------+-----------------------------------+-------------------------------+------------------+
|       | HIUINT16(nParam1)=                |                               |                  |
|       | ss_code(ss_COLP = 0x13)           | HIUINT16(nParam2)=0           |                  |
| 0xFA  | LOUINT16 (nParam1)=               | LOUINT16(nParam2)=            | LocalError       |
|       | ss_operations(SS_Interrogate= 14) | LocalErrorCode                |                  |
+-------+-----------------------------------+-------------------------------+------------------+
|       | HIUINT16(nParam1)=                |                               |                  |
|       | ss_code(ss_COLP = 0x13)           | HIUINT16(nParam2)=0           | Nwk returned a   |
| 0xFB  | LOUINT16 (nParam1)=               | LOUINT16(nParam2)=NWErrorCode | return error     |
|       | ss_operations(SS_Interrogate= 14) |                               |                  |
+-------+-----------------------------------+-------------------------------+------------------+
|       | HIUINT16(nParam1)=                |                               |                  |
|       | ss_code(ss_COLP = 0x13)           | HIUINT16(nParam2)=            |                  |
|       | LOUINT16 (nParam1)=               | ProblemCodeTag                | NW returned a    |
| 0xFC  | ss_operations(SS_Interrogate= 14) | LOUINT16(nParam2)=ProblemCode | reject component |
+-------+-----------------------------------+-------------------------------+------------------+
|       | HIUINT16(nParam1)=                |                               |                  |
|       | ss_code(ss_COLP = 0x13)           | HIUINT16(nParam2)=0           | Nwk returned a   |
| 0xFD  | LOUINT16 (nParam1)=               | LOUINT16(nParam2)=causevalue  | Cause IE         |
|       | ss_operations(SS_Interrogate= 14) |                               |                  |
+-------+-----------------------------------+-------------------------------+------------------+
\endrst

\param [in] nUTI    Specify the UTI (Unique Transaction Identifier) operation, which support multi-application in parallel. This parameter will
be return in notification event (see below). You also can get a free UTI by CFW_GetFreeUTI function if you application is a dynamical
module. This parameter should be less than 255 for all upper layer application.
\param [in] nSimID  SIM ID
\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+----------------------------+-----------------------------------------+
|         Error Code         |               Description               |
+============================+=========================================+
| ERR_CFW_INVALID_PARAMETER  | The input parameter(nIndex) is invalid. |
+----------------------------+-----------------------------------------+
| ERR_CFW_NOT_EXIST_FREE_UTI | No any available free UTI in the system |
+----------------------------+-----------------------------------------+
| ERR_CME_UNKNOWN            | Unknown error occur.                    |
+----------------------------+-----------------------------------------+
| ERR_CME_MEMORY_FULL        | No any more memory to allocate          |
+----------------------------+-----------------------------------------+
\endrst
*/
uint32_t CFW_SsQueryColp(
    uint16_t nUTI,
    CFW_SIM_ID nSimID);
/*! \brief The CFW_SsQueryColr function retrieves the status of the Connected Line Identification Restriction (COLR) of the called party after setting up a mobile originated call.

The result should be returned through the EV_CFW_SS_QUERY_COLR_RSP event:
\rst
+-------+-----------------------------------+-------------------------------+------------------+
| nType |              nParam1              |            nParam2            |   Description    |
+=======+===================================+===============================+==================+
|       | 0: means don't presentation       | 0:COLR   not provisioned      |                  |
| 0x00  | of the COLR. 1:indicate           | 1:COLR   provisioned in       | OK               |
|       | presentation of the COLR          | 2.unknown                     |                  |
+-------+-----------------------------------+-------------------------------+------------------+
|       | HIUINT16(nParam1)=                |                               |                  |
|       | ss_code(ss_COLR = 0x14)           | HIUINT16(nParam2)=0           |                  |
| 0xFA  | LOUINT16 (nParam1)=               | LOUINT16(nParam2)=            | LocalError       |
|       | ss_operations(SS_Interrogate= 14) | LocalErrorCode                |                  |
+-------+-----------------------------------+-------------------------------+------------------+
|       | HIUINT16(nParam1)=                |                               |                  |
|       | ss_code(ss_COLR = 0x14)           | HIUINT16(nParam2)=0           | Nwk returned a   |
| 0xFB  | LOUINT16 (nParam1)=               | LOUINT16(nParam2)=NWErrorCode | return error     |
|       | ss_operations(SS_Interrogate= 14) |                               |                  |
+-------+-----------------------------------+-------------------------------+------------------+
|       | HIUINT16(nParam1)=                |                               |                  |
|       | ss_code(ss_COLR = 0x14)           | HIUINT16(nParam2)=            |                  |
|       | LOUINT16 (nParam1)=               | ProblemCodeTag                | NW returned a    |
| 0xFC  | ss_operations(SS_Interrogate= 14) | LOUINT16(nParam2)=ProblemCode | reject component |
+-------+-----------------------------------+-------------------------------+------------------+
|       | HIUINT16(nParam1)=                |                               |                  |
|       | ss_code(ss_COLR = 0x14)           | HIUINT16(nParam2)=0           | Nwk returned a   |
| 0xFD  | LOUINT16 (nParam1)=               | LOUINT16(nParam2)=causevalue  | Cause IE         |
|       | ss_operations(SS_Interrogate= 14) |                               |                  |
+-------+-----------------------------------+-------------------------------+------------------+
\endrst

\param [in] nUTI    Specify the UTI (Unique Transaction Identifier) operation, which support multi-application in parallel. This parameter will
be return in notification event (see below). You also can get a free UTI by CFW_GetFreeUTI function if you application is a dynamical
module. This parameter should be less than 255 for all upper layer application.
\param [in] nSimID  SIM ID
\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+----------------------------+------------------------------------------------------------------+
|         Error Code         |                           Description                            |
+============================+==================================================================+
| ERR_CFW_NOT_EXIST_FREE_UTI | No any available free UTI in the system                          |
+----------------------------+------------------------------------------------------------------+
|                            | Indicates the event could not be immediately placed in the queue |
| ERR_COS_QUEUE_FULL         | because there was not enough space available, so this function   |
|                            | fail to send event to CFW.                                       |
+----------------------------+------------------------------------------------------------------+
| ERR_CME_UNKNOWN            | Unknown error occur.                                             |
+----------------------------+------------------------------------------------------------------+
\endrst
*/
uint32_t CFW_SsQueryColr(
    uint16_t nUTI,
    CFW_SIM_ID nSimID);
/*! \brief The CFW_SsSendUSSD function controls the Unstructured Supplementary Service Data (USSD) according to 3GPP TS 23.090. Both network and mobile initiated operations are supported

The result should be returned through the EV_CFW_SS_QUERY_CLIP_RSP event:
\rst
+-------+-------------------------------------+-------------------------------+------------------+
| nType |               nParam1               |            nParam2            |   Description    |
+=======+=====================================+===============================+==================+
|       | Specify the CFW_SS_USSD_IND_INFO    | Specify the parameter shows   |                  |
| 0x00  | structure.                          | Specify ss_code(ussd_v1=      | OK               |
|       | If nOption==2 nParam1 is NULL.      | 0x101,ussd_v2=0x100)          |                  |
+-------+-------------------------------------+-------------------------------+------------------+
|       | HIUINT16(nParam1)= ss_code          |                               |                  |
|       | (ussd_v1=0x101,ussd_v2=0x100)       | HIUINT16(nParam2)=0           |                  |
| 0xFA  | LOUINT16 (nParam1)=ss_operations    | LOUINT16(nParam2)=            | LocalError       |
|       | (ProcessUnstructuredSSData = 19,    |                               |                  |
|       | SS_ProcessUnstructuredSSRequest=59, |                               |                  |
|       | SS_UnstructuredSSRequest = 60,      |                               |                  |
|       | SS_UnstructuredSSNotify = 61)       |                               |                  |
+-------+-------------------------------------+-------------------------------+------------------+
|       | HIUINT16(nParam1)= ss_code          |                               |                  |
|       | (ussd_v1=0x101,ussd_v2=0x100)       | HIUINT16(nParam2)=0           | Nwk returned a   |
| 0xFB  | LOUINT16 (nParam1)=ss_operations    | LOUINT16(nParam2)=NWErrorCode | return error     |
|       | (ProcessUnstructuredSSData = 19,    |                               |                  |
|       | SS_ProcessUnstructuredSSRequest=59, |                               |                  |
|       | SS_UnstructuredSSRequest = 60,      |                               |                  |
|       | SS_UnstructuredSSNotify = 61)       |                               |                  |
+-------+-------------------------------------+-------------------------------+------------------+
|       | HIUINT16(nParam1)= ss_code          |                               |                  |
|       | (ussd_v1=0x101,ussd_v2=0x100)       | HIUINT16(nParam2)=            |                  |
|       | LOUINT16 (nParam1)=ss_operations    | ProblemCodeTag                | NW returned a    |
| 0xFC  | (ProcessUnstructuredSSData = 19,    | LOUINT16(nParam2)=ProblemCode | reject component |
|       | SS_ProcessUnstructuredSSRequest=59, |                               |                  |
|       | SS_UnstructuredSSRequest = 60,      |                               |                  |
|       | SS_UnstructuredSSNotify = 61)       |                               |                  |
+-------+-------------------------------------+-------------------------------+------------------+
|       | HIUINT16(nParam1)= ss_code          |                               |                  |
|       | (ussd_v1=0x101,ussd_v2=0x100)       | HIUINT16(nParam2)=0           | Nwk returned a   |
| 0xFD  | LOUINT16 (nParam1)=ss_operations    | LOUINT16(nParam2)=causevalue  | Cause IE         |
|       | (ProcessUnstructuredSSData = 19,    |                               |                  |
|       | SS_ProcessUnstructuredSSRequest=59, |                               |                  |
|       | SS_UnstructuredSSRequest = 60,      |                               |                  |
|       | SS_UnstructuredSSNotify = 61)       |                               |                  |
+-------+-------------------------------------+-------------------------------+------------------+
\endrst
\param [in] pUsdString Specify the string type USSD-string, this parameter should be encoded according to the parameter nDcs. If this parameter is NULL, network is not interrogated.(其他应用参见 nOption)
\param [in] nUsdStringSize Specify the string type USSD-string size in bytes, ranged from 1 to 160. If pUsdString is NULL, this parameter is ignored. (其他应用参见 nOption)
\param [in] nOption Specify the result code. This parameter can be one of the following value:
\rst
+-------+--------------------------------------------------------------------------------------+
| Value |                                     Description                                      |
+=======+======================================================================================+
|       | MS 返回 Error(nUsdStringSize标识 Errsource , pUsdString[0]标识具体的 ErrCode);       |
| 0     | USSD version 2 operation                                                             |
+-------+--------------------------------------------------------------------------------------+
|       | MS返回拒绝 (nUsdStringSize标识 ProblemCodeTag ,pUsdString[0]标识具体的 ProblemCode)  |
| 1     | USSD version 2 operation.                                                            |
+-------+--------------------------------------------------------------------------------------+
| 2     | terminate the session(release complete); USSD version 2 operation.                   |
+-------+--------------------------------------------------------------------------------------+
| 3     | MS发起 ussd 或返回网络发起的 ussd 操作;USSD version 2 operation.                     |
+-------+--------------------------------------------------------------------------------------+
|       |                                                                                      |
|       | MS 返回 Error(nUsdStringSize标识 Errsource, pUsdString[0]标识具体的 ErrCode );USSD   |
| 128   | version 1 operation.(The string must be encoded as an IA5 string, nDcs is ignored)   |
+-------+--------------------------------------------------------------------------------------+
|       | MS返回拒绝(nUsdStringSize标识 ProblemCodeTag, pUsdString[0]标识具体的 ProblemCode);  |
| 129   | USSD version 1 operationThe string must be encoded as an IA5 string, nDcs is ignored |
+-------+--------------------------------------------------------------------------------------+
|       |                                                                                      |
|       | terminate the session(release complete); USSD version 1 operation.                   |
| 130   | (The string must be encoded as an IA5 string, nDcs is ignored)                       |
+-------+--------------------------------------------------------------------------------------+
|       | MS发起ussd或返回网络发起的ussd操作; USSD version 1 operation.                        |
| 131   | (The string must be encoded as an IA5 string, nDcs is ignored)                       |
+-------+--------------------------------------------------------------------------------------+
\endrst
\param [in] nDcs   The USSD data coding scheme. Specify the 3GPP TS 23.038 [25] Cell Broadcast Data Coding Scheme in integer format.Specify the file in integer format.
\param [in] nUTI   Specify the UTI (Unique Transaction Identifier) operation, which support multi-application in parallel. This parameter will
be return in notification event (see below). You also can get a free UTI by CFW_GetFreeUTI function if you application is a dynamical
module. This parameter should be less than 255 for all upper layer application.
\param [in] nSimID  SIM ID
\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+-------------------------------+--------------------------------------------------------+
|          Error Code           |                      Description                       |
+===============================+========================================================+
| ERR_CFW_INVALID_PARAMETER     | The input parameter(nIndex) is invalid.                |
+-------------------------------+--------------------------------------------------------+
|                               | This function don't complete, and you execute the same |
| ERR_CME_OPERATION_NOT_ALLOWED | operations such as call this function again.           |
|                               | or there is some other SS operations exist.            |
+-------------------------------+--------------------------------------------------------+
| ERR_CFW_NOT_EXIST_FREE_UTI    | No any available free UTI in the system                |
+-------------------------------+--------------------------------------------------------+
| ERR_CME_UNKNOWN               | Unknown error occur.                                   |
+-------------------------------+--------------------------------------------------------+
\endrst
*/
uint32_t CFW_SsSendUSSD(
    uint8_t *pUsdString,
    uint8_t nUsdStringSize,
    uint8_t nOption,
    uint8_t nDcs,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

uint32_t CFW_SsSendUSSD_V2(
    uint8_t *pUsdString,
    uint8_t nUsdStringSize,
    uint8_t nOption,
    uint8_t nDcs,
    uint16_t nUTI,
    uint16_t nTI,
    CFW_SIM_ID nSimID);

typedef struct _CFW_SS_USSD_IND_INFO
{
    uint8_t *pUsdString;
    uint8_t nStingSize;
    uint8_t nOption;
    uint8_t nDcs;
    uint8_t padding;
} CFW_SS_USSD_IND_INFO;

typedef struct _CFW_SS_USSD_IND_INFO_V2
{
    uint8_t pUsdString[255];
    uint8_t nStingSize;
    uint8_t nOption;
    uint8_t nDcs;
    uint8_t padding;
} CFW_SS_USSD_IND_INFO_V2;

//
// SIM Service
//
typedef struct _CFW_SAT_RESPONSE
{
    uint8_t nSm_Act_prevStatus;
    uint8_t nSm_Act_currStatus;
    uint8_t nSAT_Mode;

    uint8_t nSm_Resp_prevStatus;
    uint8_t nSm_Resp_currStatus;

    uint8_t nSAT_CmdType;
    uint8_t nSAT_Status; ///< command status
    uint8_t nSAT_ItemID;
    uint8_t nSAT_InnerProcess;
    uint8_t nSAT_InputSch;  ///< text code scheme.
    uint16_t nSAT_InputLen; ///< text length
    uint32_t padding;       ///< DON'T remove this byte, because the next member nSAT_InputStr should be aligned 4 bytes, it'll convert to a pointer.
    uint8_t nSAT_InputStr[0];
} CFW_SAT_RESPONSE;

typedef struct _CFW_SAT_TERMINAL_ENVELOPE_CNF
{
    uint16_t SW;
    uint8_t Pid;
    uint8_t Dcs;
    uint8_t NbByte; // Nb of byte of the response
    uint8_t ResponseData[256];
} CFW_SAT_TERMINAL_ENVELOPE_CNF;

typedef struct _SAT_CHSTATUS_RSP
{
    uint8_t nChannelID; ///< 1..7
    uint8_t nStatus;    ///<  0: Link not established or PDP context not activated
    // 1: Link established or PDP context activated
    //
    uint8_t nType; ///< 00 : No further info can be given
    //05 : Link dropped
} SAT_CHSTATUS_RSP;

typedef struct _SAT_BASE_RSP ///< for proactive command 26
{
    uint8_t nComID;        ///< Command id
    uint8_t nComQualifier; ///< Command Qualifier
} SAT_BASE_RSP;

typedef struct _SAT_LOCALINFO_RSP
{
    uint8_t nMCC_MNC_Code[6]; ///< BCD code ,example:4,6,0,0,0,F for china mobile
    uint16_t nLocalAreaCode;  ///< Hex Local area code
    uint16_t nCellIdentify;   ///< Hex Cell identify
} SAT_LOCALINFO_RSP;

typedef struct _SAT_IMEI_RSP
{
    uint8_t nInterMobileId[8]; ///< BCD code,IMEI
} SAT_IMEI_RSP;

typedef struct _SAT_MEASURE_RSP
{
    uint8_t nMeasure[16]; ///< Net work measurement
} SAT_MEASURE_RSP;

typedef struct _SAT_DATETIME_RSP
{
    uint8_t nDateTimeZone[7]; ///< Date,Time,Zone
} SAT_DATETIME_RSP;

typedef struct _SAT_LANGUAGE_RSP
{
    uint8_t nLanguageCode[2]; ///< Language
} SAT_LANGUAGE_RSP;

typedef struct _SAT_TIMINGADV_RSP
{
    uint8_t nME_Status; ///< Me status
    uint8_t nTimingAdv; ///< Timing advance
} SAT_TIMINGADV_RSP;

typedef struct _CFW_SAT_SMS_RSP ///< for proactive command 0x13
{
    uint8_t nComID;
    uint8_t nComQualifier;
    uint16_t nAlphaLen;
    uint8_t *pAlphaStr;
    uint16_t nAddrLen;
    uint8_t nAddrType;
    uint8_t *pAddrStr;
    uint8_t nPDULen;
    uint8_t *pPDUStr;
} CFW_SAT_SMS_RSP;

typedef struct _CFW_SAT_SMS_RSP_V2 ///< for proactive command 0x13
{
    uint8_t nComID;
    uint8_t nComQualifier;
    uint16_t nAlphaLen;
    uint8_t pAlphaStr[CFW_SAT_MAX_ALPHA_STR_LEN];
    uint16_t nAddrLen;
    uint8_t nAddrType;
    uint8_t pAddrStr[CFW_SAT_MAX_ADDR_LEN];
    uint8_t nPDULen;
    uint8_t pPDUStr[CFW_MAX_SMS_PDU_LEN];
} CFW_SAT_SMS_RSP_V2;

typedef struct _CFW_SAT_DTEXT_RSP_V2 ///< for proactive command 0x21
{
    uint8_t nComID;
    uint8_t nComQualifier;
    uint8_t nTextLen;
    uint8_t nTextSch;
    uint8_t pTextStr[CFW_SAT_MAX_TEXT_STR_LEN];
} CFW_SAT_DTEXT_RSP_V2;

typedef struct _CFW_SAT_INPUT_RSP_V2 ///< for proactive command 0x23
{
    uint8_t nComID;
    uint8_t nComQualifier;

    uint8_t nTextLen;
    uint8_t nTextSch;
    uint8_t pTextStr[CFW_SAT_MAX_TEXT_STR_LEN];

    uint8_t nResMinLen;
    uint8_t nResMaxLen;

    uint8_t nDefTextLen;
    uint8_t nDefTextSch;
    uint8_t pDefTextStr[CFW_SAT_MAX_TEXT_STR_LEN];
} CFW_SAT_INPUT_RSP_V2;

typedef struct _CFW_SAT_INKEY_RSP_V2 ///< for proactive command 0x23
{
    uint8_t nComID;
    uint8_t nComQualifier;

    uint8_t nTextLen;
    uint8_t nTextSch;
    uint8_t pTextStr[CFW_SAT_MAX_TEXT_STR_LEN];
} CFW_SAT_INKEY_RSP_V2;

typedef struct _CFW_SAT_CALL_RSP_V2 ///< for proactive command 0x10
{
    uint8_t nComID;
    uint8_t nComQualifier;

    uint16_t nAlphaConfirmLen;
    uint8_t pAlphaConfirmStr[CFW_SAT_MAX_ALPHA_STR_LEN];

    uint8_t nAddrLen;
    uint8_t nAddrType;
    uint8_t pAddrStr[CFW_SAT_MAX_ADDR_LEN];

    uint8_t nCapabilityCfgLen;
    uint8_t pCapabilityCfgStr[CFW_SAT_MAX_TEXT_STR_LEN];

    uint8_t nSubAddrLen;
    uint8_t pSubAddrStr[CFW_SAT_MAX_ADDR_LEN];

    uint8_t nTuint;
    uint8_t nTinterval;

    uint16_t nAlphaSetupLen;
    uint8_t pAlphaSetupStr[CFW_SAT_MAX_ALPHA_STR_LEN];
} CFW_SAT_CALL_RSP_V2;

typedef struct _CFW_SAT_SS_RSP_V2 ///< for proactive command 0x10
{
    uint8_t nComID;
    uint8_t nComQualifier;

    uint8_t nAlphaLength;
    uint8_t nAlphaIdentifier[CFW_SAT_MAX_ALPHA_STR_LEN];

    uint8_t nIconQualifier;
    uint8_t nIconIdentifier;

    uint8_t nSSLength;
    uint8_t nTONNPI;
    uint8_t pSSStr[CFW_SAT_MAX_SS_STR_LEN];
} CFW_SAT_SS_RSP_V2;

typedef struct _CFW_SAT_USSD_RSP_V2 ///< for proactive command 0x10
{
    uint8_t nComID;
    uint8_t nComQualifier;

    uint8_t nAlphaLength;
    uint8_t nAlphaIdentifier[CFW_SAT_MAX_ALPHA_STR_LEN];

    uint8_t nIconQualifier;
    uint8_t nIconIdentifier;

    uint8_t nUSSDLength;
    uint8_t nCodeScheme;
    uint8_t pUSSDStr[CFW_SAT_MAX_USSD_STR_LEN];
} CFW_SAT_USSD_RSP_V2;

typedef struct _CFW_SAT_FILELIST_RSP_V2 ///< for proactive command 0x10
{
    uint8_t nComID;
    uint8_t nComQualifier;
    uint8_t nMode;
    uint8_t nNumFiles;
    uint8_t nFiles[CFW_SAT_MAX_FILE_LIST_LEN];
} CFW_SAT_FILELIST_RSP_V2;

typedef struct _CFW_SAT_PLAYTONE_RSP_V2
{
    uint8_t nComID;
    uint8_t nComQualifier;
    uint8_t TimeUnit;
    uint8_t TimeInterval;
    uint8_t Tone;
    uint8_t IconQualifier;
    uint8_t IconIdentifier;
    uint8_t AlphaIDLen;
    uint8_t AlphaIDString[CFW_SAT_MAX_ALPHA_STR_LEN];
} CFW_SAT_PLAYTONE_RSP_V2;

#define OPEN_CHANNEL_BEARER_CSD 01
#define OPEN_CHANNEL_BEARER_GPRS 02
#define OPEN_CHANNEL_BEARER_DEFAULT 03
typedef struct _CFW_SAT_OPENCHANNEL_RSP_V2 ///< for proactive command 0x15
{
    uint8_t nComID;
    uint8_t nComQualifier;

    uint8_t AlphaIdentifierLen;
    uint8_t AlphaIdentifier[CFW_SAT_MAX_ALPHA_STR_LEN];

    uint8_t IconQualifier;
    uint8_t IconIdentifier;

    uint8_t AddressType;
    uint8_t AddressLen;
    uint8_t Address[CFW_SAT_MAX_ADDR_LEN];

    uint8_t SubAddressLen;
    uint8_t SubAddress[CFW_SAT_MAX_ADDR_LEN];

    uint8_t Duration1Unit;
    uint8_t Duration1Interval;

    uint8_t Duration2Unit;
    uint8_t Duration2Interval;

    uint8_t BearerType;
    uint8_t BearerLen;
    uint8_t BearerDescription[CFW_SAT_MAX_BEARER_DESC_LEN];

    uint16_t BufferSize;

    uint8_t OtherAddressLen;
    uint8_t OtherAddressType;
    uint8_t OtherAddress[CFW_SAT_MAX_ADDR_LEN];

    uint8_t UserLoginLen;
    uint8_t UserLoginDcs;
    uint8_t UserLogin[CFW_SAT_MAX_TEXT_STR_LEN];

    uint8_t UserPasswordLen;
    uint8_t UserPasswordDcs;
    uint8_t UserPassword[CFW_SAT_MAX_TEXT_STR_LEN];

    uint8_t TransportLevelType;
    uint16_t TransportLevelPort;

    uint8_t DestAddressLen;
    uint8_t DestAddressType;
    uint8_t DestAddress[CFW_SAT_MAX_ADDR_LEN];

    uint8_t NetworkAccessNameLen;
    uint8_t NetworkAccessName[CFW_SAT_MAX_NW_ACCESS_NAME_LEN];
} CFW_SAT_OPENCHANNEL_RSP_V2;

typedef struct _CFW_SAT_CLOSECHANNEL_RSP_V2 ///< for proactive command 0x15
{
    uint8_t nComID;
    uint8_t nComQualifier;

    uint8_t AlphaIdentifierLen;
    uint8_t AlphaIdentifier[CFW_SAT_MAX_ALPHA_STR_LEN];

    uint8_t IconQualifier;
    uint8_t IconIdentifier;
} CFW_SAT_CLOSECHANNEL_RSP_V2;

typedef struct _CFW_SAT_RECEIVEDATA_RSP_V2 ///< for proactive command 0x15
{
    uint8_t nComID;
    uint8_t nComQualifier;

    uint8_t AlphaIdentifierLen;
    uint8_t AlphaIdentifier[CFW_SAT_MAX_ALPHA_STR_LEN];

    uint8_t IconQualifier;
    uint8_t IconIdentifier;

    uint8_t ChannelDataLength;
} CFW_SAT_RECEIVEDATA_RSP_V2;

typedef struct _CFW_SAT_SENDDATA_RSP_V2 ///< for proactive command 0x15
{
    uint8_t nComID;
    uint8_t nComQualifier;

    uint8_t AlphaIdentifierLen;
    uint8_t AlphaIdentifier[CFW_SAT_MAX_ALPHA_STR_LEN];

    uint8_t IconQualifier;
    uint8_t IconIdentifier;

    uint8_t ChannelDataLen;
    uint8_t ChannelDataString[CFW_SAT_MAX_CHANNEL_DATA_STR_LEN];
} CFW_SAT_SENDDATA_RSP_V2;

typedef struct _CFW_SAT_BROWSER_RSP_V2 ///< for proactive command 0x15
{
    uint8_t nComID;
    uint8_t nComQualifier;

    uint8_t nBrowserIdentity;

    uint8_t nURLLength;

    uint8_t nBearerLength;

    uint8_t nCodeScheme;
    uint8_t nTextLength; ///< Gateway/Proxy

    uint8_t nAlphaLength;

    uint8_t nIconQualifier;
    uint8_t nIconIdentifier;

    uint8_t nProvisioningFileRefNum;

    uint8_t nPading;
    uint8_t pURL[CFW_SAT_MAX_URL_LEN];
    uint8_t pListBearer[CFW_SAT_MAX_LIST_BEARER_LEN];
    uint8_t nTextString[CFW_SAT_MAX_TEXT_STR_LEN];
    uint8_t nAlphaIdentifier[CFW_SAT_MAX_ALPHA_STR_LEN];
    uint8_t pProvisioningFileRef[CFW_SAT_MAX_PROVISIONING_FILE_LEN];
} CFW_SAT_BROWSER_RSP_V2;

typedef struct _CFW_SAT_SEND_DTMF_RSP_V2 ///< for proactive command
{
    uint8_t nComID;
    uint8_t nComQualifier;
    uint8_t nAlphaLen;
    uint8_t pAlphaStr[CFW_SAT_MAX_ALPHA_STR_LEN];
    uint8_t nDTMFLen;
    uint8_t pDTMFStr[CFW_SAT_MAX_DTMF_STR_LEN];
} CFW_SAT_SEND_DTMF_RSP_V2;

typedef struct _CFW_SAT_PDU_RSP_V2 ///< for proactive command 0x15
{
    uint8_t nComID;
    uint8_t nComNum;
    uint8_t nComQualifier;
    uint8_t nLength;
    uint8_t pPDU[CFW_MAX_SMS_PDU_LEN];
} CFW_SAT_PDU_RSP_V2;

typedef struct _CFW_SAT_DTEXT_RSP ///< for proactive command 0x21
{
    uint8_t nComID;
    uint8_t nComQualifier;
    uint8_t nTextLen;
    uint8_t nTextSch;
    uint8_t *pTextStr;
} CFW_SAT_DTEXT_RSP;

typedef struct _CFW_SAT_POLL_INTERVAL_RSP ///< for proactive command 0x21
{
    uint8_t nComID;
    uint8_t nComQualifier;
    uint8_t nTimeUnit;
    uint8_t nTimeInterval;
} CFW_SAT_POLL_INTERVAL_RSP;

typedef struct _CFW_SAT_INPUT_RSP ///< for proactive command 0x23
{
    uint8_t nComID;
    uint8_t nComQualifier;

    uint8_t nTextLen;
    uint8_t nTextSch;
    uint8_t *pTextStr;

    uint8_t nResMinLen;
    uint8_t nResMaxLen;

    uint8_t nDefTextLen;
    uint8_t nDefTextSch;
    uint8_t *pDefTextStr;
} CFW_SAT_INPUT_RSP;

typedef struct _CFW_SAT_INKEY_RSP ///< for proactive command 0x23
{
    uint8_t nComID;
    uint8_t nComQualifier;

    uint8_t nTextLen;
    uint8_t nTextSch;
    uint8_t *pTextStr;
} CFW_SAT_INKEY_RSP;

typedef struct _CFW_SAT_CALL_RSP ///< for proactive command 0x10
{
    uint8_t nComID;
    uint8_t nComQualifier;

    uint16_t nAlphaConfirmLen;
    uint8_t *pAlphaConfirmStr;

    uint8_t nAddrLen;
    uint8_t nAddrType;
    uint8_t *pAddrStr;

    uint8_t nCapabilityCfgLen;
    uint8_t *pCapabilityCfgStr;

    uint8_t nSubAddrLen;
    uint8_t *pSubAddrStr;

    uint8_t nTuint;
    uint8_t nTinterval;

    uint16_t nAlphaSetupLen;
    uint8_t *pAlphaSetupStr;
} CFW_SAT_CALL_RSP;

typedef struct _CFW_SAT_SS_RSP ///< for proactive command 0x10
{
    uint8_t nComID;
    uint8_t nComQualifier;

    uint8_t nAlphaLength;
    uint8_t *nAlphaIdentifier;

    uint8_t nIconQualifier;
    uint8_t nIconIdentifier;

    uint8_t nSSLength;
    uint8_t nTONNPI;
    uint8_t *pSSStr;
} CFW_SAT_SS_RSP;

typedef struct _CFW_SAT_USSD_RSP ///< for proactive command 0x10
{
    uint8_t nComID;
    uint8_t nComQualifier;

    uint8_t nAlphaLength;
    uint8_t *nAlphaIdentifier;

    uint8_t nIconQualifier;
    uint8_t nIconIdentifier;

    uint8_t nUSSDLength;
    uint8_t nCodeScheme;
    uint8_t *pUSSDStr;
} CFW_SAT_USSD_RSP;

#define MAX_NUM_EVENT_LIST 11         ///<  11 type of events
typedef struct _CFW_SAT_EVENTLIST_RSP ///< for proactive command 0x10
{
    uint8_t nComID;
    uint8_t nComQualifier;
    uint8_t nNumEvent;
    uint8_t nEventList[MAX_NUM_EVENT_LIST];
} CFW_SAT_EVENTLIST_RSP;

typedef struct _CFW_SAT_FILELIST_RSP ///< for proactive command 0x10
{
    uint8_t nComID;
    uint8_t nComQualifier;
    uint8_t nMode;
    uint8_t nNumFiles;
    uint8_t nFiles[0];
} CFW_SAT_FILELIST_RSP;

typedef struct _CFW_SAT_PLAYTONE_RSP
{
    uint8_t nComID;
    uint8_t nComQualifier;
    uint8_t TimeUnit;
    uint8_t TimeInterval;
    uint8_t Tone;
    uint8_t IconQualifier;
    uint8_t IconIdentifier;
    uint8_t AlphaIDLen;
    union {
        uint8_t *AlphaIDString;
        uint8_t *pAlphaIDString;
    };
    uint8_t *pTextAttribute;
    uint8_t TextAttrLen;
    uint8_t FrameIdent;
} CFW_SAT_PLAYTONE_RSP;

#define OPEN_CHANNEL_BEARER_CSD 01
#define OPEN_CHANNEL_BEARER_GPRS 02
#define OPEN_CHANNEL_BEARER_DEFAULT 03
typedef struct _CFW_SAT_OPENCHANNEL_RSP ///< for proactive command 0x15
{
    uint8_t nComID;
    uint8_t nComQualifier;

    uint8_t AlphaIdentifierLen;
    uint8_t *AlphaIdentifier;

    uint8_t IconQualifier;
    uint8_t IconIdentifier;

    uint8_t AddressType;
    uint8_t AddressLen;
    uint8_t *Address;

    uint8_t SubAddressLen;
    uint8_t *SubAddress;

    uint8_t Duration1Unit;
    uint8_t Duration1Interval;

    uint8_t Duration2Unit;
    uint8_t Duration2Interval;

    uint8_t BearerType;
    uint8_t BearerLen;
    uint8_t *BearerDescription;

    uint16_t BufferSize;

    uint8_t OtherAddressLen;
    uint8_t OtherAddressType;
    uint8_t *OtherAddress;

    uint8_t UserLoginLen;
    uint8_t UserLoginDcs;
    uint8_t *UserLogin;

    uint8_t UserPasswordLen;
    uint8_t UserPasswordDcs;
    uint8_t *UserPassword;

    uint8_t TransportLevelType;
    uint16_t TransportLevelPort;

    uint8_t DestAddressLen;
    uint8_t DestAddressType;
    uint8_t *DestAddress;

    uint8_t NetworkAccessNameLen;
    uint8_t *NetworkAccessName;
} CFW_SAT_OPENCHANNEL_RSP;

typedef struct _CFW_SAT_CLOSECHANNEL_RSP ///< for proactive command 0x15
{
    uint8_t nComID;
    uint8_t nComQualifier;
    uint8_t nChannelID;

    uint8_t AlphaIdentifierLen;
    uint8_t *AlphaIdentifier;

    uint8_t IconQualifier;
    uint8_t IconIdentifier;
} CFW_SAT_CLOSECHANNEL_RSP;

typedef struct _CFW_SAT_RECEIVEDATA_RSP ///< for proactive command 0x15
{
    uint8_t nComID;
    uint8_t nComQualifier;

    uint8_t AlphaIdentifierLen;
    uint8_t *AlphaIdentifier;

    uint8_t IconQualifier;
    uint8_t IconIdentifier;

    uint8_t ChannelID;
    uint8_t ChannelDataLength;
} CFW_SAT_RECEIVEDATA_RSP;

typedef struct _CFW_SAT_SENDDATA_RSP ///< for proactive command 0x15
{
    uint8_t nComID;
    uint8_t nComQualifier;

    uint8_t AlphaIdentifierLen;
    uint8_t *AlphaIdentifier;

    uint8_t IconQualifier;
    uint8_t IconIdentifier;

    uint8_t ChannelID;
    uint8_t ChannelDataLen;
    uint8_t *ChannelDataString;
} CFW_SAT_SENDDATA_RSP;

typedef struct _CFW_SAT_GETCHANNELSTATUS_RSP ///< for proactive command 0x15
{
    uint8_t nComID;
    uint8_t nComQualifier;
} CFW_SAT_GETCHANNELSTATUS_RSP;

typedef struct _CFW_SAT_BROWSER_RSP ///< for proactive command 0x15
{
    uint8_t nComID;
    uint8_t nComQualifier;

    uint8_t nBrowserIdentity;

    uint8_t nURLLength;

    uint8_t nBearerLength;

    uint8_t nCodeScheme;
    uint8_t nTextLength; ///< Gateway/Proxy

    uint8_t nAlphaLength;

    uint8_t nIconQualifier;
    uint8_t nIconIdentifier;

    uint8_t nProvisioningFileRefNum;

    uint8_t nPading;
    uint8_t *pURL;
    uint8_t *pListBearer;
    uint8_t *nTextString;
    uint8_t *nAlphaIdentifier;
    uint8_t *pProvisioningFileRef[1];
} CFW_SAT_BROWSER_RSP;

typedef struct _CFW_SAT_TIMER_RSP ///< for proactive command 0x15
{
    uint8_t nComID;
    uint8_t nComQualifier;

    uint8_t nTimerID;

    uint8_t nHour;
    uint8_t nMinute;
    uint8_t nSecond;
} CFW_SAT_TIMER_RSP;

typedef struct _CFW_SAT_SEND_DTMF_RSP ///< for proactive command
{
    uint8_t nComID;
    uint8_t nComQualifier;
    uint8_t nAlphaLen;
    uint8_t *pAlphaStr;
    uint8_t nDTMFLen;
    uint8_t *pDTMFStr;
} CFW_SAT_SEND_DTMF_RSP;

typedef struct _CFW_SAT_LANG_NOTIFICATION_RSP
{
    uint8_t nComID;
    uint8_t nComQualifier;
    uint8_t nLanguage[2];
} CFW_SAT_LANG_NOTIFICATION_RSP;

typedef struct _CFW_SAT_BASE_RSP
{
    uint8_t nComID;
    uint8_t nComQualifier;
} CFW_SAT_BASE_RSP;

typedef struct _CFW_SAT_PDU_RSP ///< for proactive command 0x15
{
    uint8_t nComID;
    uint8_t nComNum;
    uint8_t nComQualifier;
    uint8_t nLength;
    uint8_t *pPDU;
} CFW_SAT_PDU_RSP;

typedef union {
    CFW_SAT_BASE_RSP uBase;
    CFW_SAT_SEND_DTMF_RSP uSendDtmp;
    CFW_SAT_TIMER_RSP uTimer;
    CFW_SAT_GETCHANNELSTATUS_RSP uGetChannelStatus;
    CFW_SAT_EVENTLIST_RSP uEventList;
    CFW_SAT_POLL_INTERVAL_RSP uInterval;
    CFW_SAT_PDU_RSP_V2 uPDUV2;
    CFW_SAT_SEND_DTMF_RSP_V2 uSendDtmfV2;
    CFW_SAT_BROWSER_RSP_V2 uBrowserV2;
    CFW_SAT_SENDDATA_RSP_V2 uSendDataV2;
    CFW_SAT_RECEIVEDATA_RSP_V2 uReceDataV2;
    CFW_SAT_CLOSECHANNEL_RSP_V2 uCloseChannelV2;
    CFW_SAT_OPENCHANNEL_RSP_V2 uOpenChannelV2;
    CFW_SAT_PLAYTONE_RSP_V2 uPlayToneV2;
    CFW_SAT_FILELIST_RSP_V2 uFileListV2;
    CFW_SAT_USSD_RSP_V2 uUssdV2;
    CFW_SAT_SS_RSP_V2 uSSV2;
    CFW_SAT_CALL_RSP_V2 uCallV2;
    CFW_SAT_INKEY_RSP_V2 uInkeyV2;
    CFW_SAT_INPUT_RSP_V2 uInputV2;
    CFW_SAT_DTEXT_RSP_V2 uDtextV2;
    CFW_SAT_SMS_RSP_V2 uSMSV2;
} CFW_SAT_RSP_UNION;

//These Macro will be add to system header files.

//This Macro definition is used for ParsSW1SW2 function.
#define ERR_CME_SIM_VERIFY_FAIL 0x20
#define ERR_CME_SIM_UNBLOCK_FAIL 0x21
#define ERR_CME_SIM_CONDITION_NO_FULLFILLED 0x22
#define ERR_CME_SIM_UNBLOCK_FAIL_NO_LEFT 0x23
#define ERR_CME_SIM_VERIFY_FAIL_NO_LEFT 0x24
#define ERR_CME_SIM_INVALID_PARAMETER 0x25
#define ERR_CME_SIM_UNKNOW_COMMAND 0x26
#define ERR_CME_SIM_WRONG_CLASS 0x27
#define ERR_CME_SIM_TECHNICAL_PROBLEM 0x28
#define ERR_CME_SIM_CHV_NEED_UNBLOCK 0x29

#define ERR_CME_SIM_NOEF_SELECTED 0x2A
#define ERR_CME_SIM_FILE_UNMATCH_COMMAND 0x2B
#define ERR_CME_SIM_CONTRADICTION_CHV 0x2C
#define ERR_CME_SIM_CONTRADICTION_INVALIDATION 0x2D
#define ERR_CME_SIM_MAXVALUE_REACHED 0x2E
#define ERR_CME_SIM_PATTERN_NOT_FOUND 0x2F
#define ERR_CME_SIM_FILEID_NOT_FOUND 0x30
#define ERR_CME_SIM_STK_BUSY 0x31
#define ERR_CME_SIM_UNKNOW 0x32
#define ERR_CME_SIM_PROFILE_ERROR 0x33

// WARNING
#define ERR_CME_SIM_NO_INFORMATION 0x34
#define ERR_CME_SIM_RETURN_DATA_CORRUPTED 0x35
#define ERR_CME_SIM_LESS_DATA 0x36
#define ERR_CME_SIM_SELECT_FILE_INVALIDATED 0x37
#define ERR_CME_SIM_SELECT_FILE_TERMINATION 0x38
#define ERR_CME_SIM_MORE_DATA 0x39
#define ERR_CME_SIM_MORE_DATA_PROACTIVE_COMMAND_PENDING 0x3A
#define ERR_CME_SIM_RESPONSE_DATA_AVAILABLE 0x3B

#define ERR_CME_SIM_MORE_DATA_EXPECTED 0x3C
#define ERR_CME_SIM_MORE_DATA_EXPECTED_PROACTIVE_COMMAND_PENDING 0x3D
#define ERR_CME_SIM_COMMAND_SUCCESS_VERIFICATION_FAIL 0x3E

//Execution errors
#define ERR_CME_SIM_MEMORY_PROBLEM 0x3F

// Functions in CLA not supported
#define ERR_CME_SIM_LOGIC_CHANNEL_NO_SUPPORTED 0x40
#define ERR_CME_SIM_SECURE_MESSAGING_NO_SUPPORTED 0x41

//Command not allowed
#define ERR_CME_SIM_COMMAND_IMCOMPATIBLE 0x42
#define ERR_CME_SIM_SECURITY_STATUS_NO_SATISFIED 0x43
#define ERR_CME_SIM_AUTH_PIN_METHOD_BLOCKED 0x44
#define ERR_CME_SIM_REFERENCED_DATA_INVALIDATED 0x45
#define ERR_CME_SIM_USE_CONDITIONS_NOT_SATISFIED 0x46
#define ERR_CME_SIM_COMMAND_NOT_ALLOWED 0x47
#define ERR_CME_SIM_COMMAND_NOT_ALLOWED_SECURITY_NOT_SATISFIED 0x48

//Wrong parameters
#define ERR_CME_SIM_INCORRECT_PARA_IN_DATA_FIELD 0x49
#define ERR_CME_SIM_FUNCTION_NOT_SUPPORTED 0x4a
#define ERR_CME_SIM_FILE_NOT_FOUND 0x4B
#define ERR_CME_SIM_RECORD_NOT_FOUND 0x4C
#define ERR_CME_SIM_NOT_ENOUGH_MEMORY 0x4D
#define ERR_CME_SIM_INCORRECT_PARA_P1_P2 0x4E
#define ERR_CME_SIM_LC_INCONSISTENT_WITH_P1_P2 0x4F
#define ERR_CME_SIM_REFERENCED_DATA_NOT_FOUND 0x50

//Application errors
#define ERR_CME_SIM_AUTH_ERROR_APPLICATION_SPECIFIC 0x51
#define ERR_CME_SIM_SECURITY_SESSION_ASSOCIATION_EXPIRED 0x52

//#define EV_CFW_SIM_LISTCOUNT_PBK_ENTRY_RSP        16        ///< for List counter function event id
#define ERR_SCL_QUEUE_FULL 0x23 ///<
#define ERR_CFW_INVALID_COMMAND 1
//end

#define SIM_PBK_NAME_SIZE 50 ///< Lixp updated,20071225
#define SIM_PBK_NUMBER_SIZE 10
#define SIM_PBK_EXTR_SIZE (SIM_PBK_NUMBER_SIZE + SIM_PBK_NAME_SIZE)

typedef struct _CFW_SIM_PBK_ENTRY_INFO
{
    uint8_t *pNumber;
    uint8_t *pFullName;
    uint8_t iFullNameSize;
    uint8_t nNumberSize;
    uint8_t nType;
    uint16_t phoneIndex;
    uint8_t *pRecordData;
} CFW_SIM_PBK_ENTRY_INFO;

typedef struct _CFW_SIM_PBK_ENTRY_INFO_V2
{
    uint8_t pNumber[SIM_PBK_NUMBER_SIZE];
    uint8_t pFullName[SIM_PBK_NAME_SIZE];
    uint8_t iFullNameSize;
    uint8_t nNumberSize;
    uint8_t nType;
    uint16_t phoneIndex;
    uint8_t *pRecordData;
} CFW_SIM_PBK_ENTRY_INFO_V2;

typedef struct _CFW_SIM_PUCT_INFO
{
    uint8_t iCurrency[3]; ///< currency code
    uint16_t iEPPU;
    uint8_t iEX;
    bool iSign; ///< the sign of EX,1 is the negetive sign
    uint8_t padding;
} CFW_SIM_PUCT_INFO;

#define CFW_SIM_PBKMODE_ADN 0
#define CFW_SIM_PBKMODE_FDN 1

uint32_t CFW_SimGetPbkMode(
    uint16_t nUTI);

uint32_t CFW_SimSetPbkMode(
    uint8_t nPbkMode,
    uint16_t nUTI);

typedef struct _CFW_PBK_STORAGE
{
    uint16_t usedSlot;
    uint16_t totalSlot;
    uint16_t storageId;
    uint8_t padding[2];
} CFW_PBK_STORAGE;

//add by wulc  begin
typedef struct _CFW_SPN
{
    uint8_t flag;
    uint8_t spn[16];
} CFW_SPN;
//add by wulc end

#define CFW_STY_FAC_TYPE_SC 0x5343 ///< SIM (lock SIM cards). SIM requests password upon ME power up and when this lock command is issued. "SC" lock is protected with SIM PIN1. The number can be modified with this function.
#define CFW_STY_FAC_TYPE_PS 0x5053 ///< Phone locked to SIM (device code). The "PS" password may either be individually specified by the client or, depending on the subscription, supplied from the provider (e.g. with a prepaid mobile).
#define CFW_STY_FAC_TYPE_P2 0x5032 ///< SIM PIN2.
#define CFW_STY_FAC_TYPE_PF 0x5046 ///< lock Phone to the very first SIM card
#define CFW_STY_FAC_TYPE_PN 0x504E ///< Network Personalisation
#define CFW_STY_FAC_TYPE_PU 0x5055 ///< Network-subset Personalisation
#define CFW_STY_FAC_TYPE_PP 0x5050 ///< Service-Provider Personalisation
#define CFW_STY_FAC_TYPE_PC 0x5043 ///< Corporate Personalisation
#define CFW_STY_FAC_TYPE_FD 0x4644 ///< the phone numbers stored to the "FD" memory can be dialled (depending on the SIM card, usually up to 7 numbers). If PIN2 authentication has not been performed during the current session, PIN2 is requested as <passwd>.

#define CFW_STY_AUTH_READY 0                 ///< Phone is not waiting for any password.
#define CFW_STY_AUTH_PIN1_READY 1            ///< Phone is not waiting for PIN1 password.
#define CFW_STY_AUTH_SIMPIN 2                ///< Phone is waiting for the SIM Personal Identification Number (PIN)
#define CFW_STY_AUTH_SIMPUK 3                ///< Phone is waiting for the SIM Personal Unlocking Key (PUK).
#define CFW_STY_AUTH_PHONE_TO_SIMPIN 4       ///< Phone is waiting for the phone-to-SIM card password.
#define CFW_STY_AUTH_PHONE_TO_FIRST_SIMPIN 5 ///< Phone is waiting for the phone-to-first-SIM card PIN.
#define CFW_STY_AUTH_PHONE_TO_FIRST_SIMPUK 6 ///< Phone is waiting for the phone-to-first-SIM card PUK.
#define CFW_STY_AUTH_SIMPIN2 7               ///< Phone is waiting for the SIM PIN2.
#define CFW_STY_AUTH_SIMPUK2 8               ///< Phone is waiting for the SIM PUK2.
#define CFW_STY_AUTH_NETWORKPIN 9            ///< Phone is waiting for the network personalization PIN.
#define CFW_STY_AUTH_NETWORKPUK 10           ///< Phone is waiting for the network personalization PUK.
#define CFW_STY_AUTH_NETWORK_SUBSETPIN 11    ///< Phone is waiting for the network subset personalization PIN.
#define CFW_STY_AUTHNETWORK_SUBSETPUK 12     ///< Phone is waiting for the network subset personalization PUK.
#define CFW_STY_AUTH_PROVIDERPIN 13          ///< Phone is waiting for the service provider personalization PIN.
#define CFW_STY_AUTH_PROVIDERPUK 14          ///< Phone is waiting for the service provider personalization PUK.
#define CFW_STY_AUTH_CORPORATEPIN 15         ///< Phone is waiting for the corporate personalization PIN.
#define CFW_STY_AUTH_CORPORATEPUK 16         ///< Phone is waiting for the corporate personalization PUK.
#define CFW_STY_AUTH_NOSIM 17                ///< No SIM inserted.
//
//add by lixp
//
#define CFW_STY_AUTH_PIN1BLOCK 18
#define CFW_STY_AUTH_PIN2BLOCK 19
#define CFW_STY_AUTH_PIN1_DISABLE 20
#define CFW_STY_AUTH_SIM_PRESENT 21
#define CFW_STY_AUTH_SIM_PHASE 22
#define CFW_STY_AUTH_SIM_END 23
#define CFW_STY_AUTH_PIN2_DISABLE 24
#define CFW_STY_AUTH_GENERALPIN 25
#define CFW_STY_AUTH_TOTALPIN 26

typedef struct _CFW_PROFILE
{
    //b0  == FDN
    //b1  == BDN
    //b2  == CC
    //0: Not Support,1: Support
    uint16_t nStart_Profile;

    uint8_t nSat_ProfileLen;
    uint8_t pSat_ProfileData[34]; ///<   3gpp 11.14
} CFW_PROFILE;

typedef struct _CFW_UsimEfStatus
{
    uint8_t efStructure; // 1:Transparent ; 2: Linear Fixed ; 6: Cyclic
    uint16_t fileId;
    uint16_t fileSize;
    uint8_t recordLength;
    uint8_t numberOfRecords;
} CFW_UsimEfStatus;

typedef struct CFW_SIM_GET_STATUS
{
    uint8_t nFileID;
    uint8_t nStatus;
} CFW_SIM_GET_STATUS;

uint32_t CFW_SimGetDFStatus(uint8_t nFile, uint16_t nUTI, CFW_SIM_ID nSimID);

void CFW_UsimDecodeEFFcp(
    uint8_t *RespData,
    CFW_UsimEfStatus *pSimEfStatus);

uint32_t CFW_UsimDecodeEFFcpV2(
    uint8_t *RespData,
    uint16_t iLen,
    CFW_UsimEfStatus *pSimEfStatus);

uint32_t CFW_SimChangePassword(
    uint16_t nFac,
    uint8_t *pBufOldPwd,
    uint8_t nOldPwdSize,
    uint8_t *pBufNewPwd,
    uint8_t nNewPwdSize,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

/*! \brief function retrieves the status of phone authentication.

The result should be returned through the EV_CFW_STY_GET_AUTH_STATUS_RSP event:

\rst
+------+-------+-----------------------------+-------------------------------------------------------------------------------------------------+-------------+
| UTI  | nType |           nParam1           |                                             nParam2                                             | Description |
+======+=======+=============================+=================================================================================================+=============+
| nUTI | 0x00  | Specify the authentication  | Retrieves the number of attempts still available for entering a required password(PIN, PUK etc) | OK          |
|      |       | status needed, which is one | \ (LOUINT16(nParam2)&0x0F) = PIN1 Count.                                                        |             |
|      |       | of the constants which      | \ ((LOUINT16(nParam2)&0xF0)>>4) = PUK1Count.                                                    |             |
|      |       | described in Remarks        | \ ((LOUINT16(nParam2)&0xF00)>>8) = PIN2 Count.                                                  |             |
|      |       | session below.              | \ ((LOUINT16(nParam2)&0xF000)>>12) = PUK2 Count.                                                |             |
+------+-------+-----------------------------+-------------------------------------------------------------------------------------------------+-------------+
| nUTI | 0xF0  | Error code                  | 0                                                                                               | Error       |
+------+-------+-----------------------------+-------------------------------------------------------------------------------------------------+-------------+
\endrst

when the type of notification event is 0xF0, the param1 can be one of the following value:
    - \a CFW_STY_AUTH_READY Phone is not waiting for PIN1 and PIN2 password.
    - \a CFW_STY_AUTH_PIN1_READY Phone is not waiting for PIN1 password.
    - \a CFW_STY_AUTH_SIMPIN Phone is waiting for the SIM Personal Identification Number (PIN1)
    - \a CFW_STY_AUTH_SIMPUK Phone is waiting for the SIM Personal Unlocking Key (PUK1).
    - \a CFW_STY_AUTH_SIMPIN2 Phone is waiting for the SIM PIN2.
    - \a CFW_STY_AUTH_SIMPUK2 Phone is waiting for the SIM PUK2.
    - \a CFW_STY_AUTH_NOSIM No SIM inserted.


\param [in] nUTI    UTI operation
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
                    - \a ERR_CME_OPERATION_NOT_ALLOWED This function don't complete, and you execute the same operations such as call this function again. or SIM is initializing.
                    - \a ERR_NO_MORE_MEMORY No any more memory to allocate.
                    - \a ERR_CFW_NOT_EXIST_FREE_UTI No any available free UTI in the system.
                    - \a ERR_CFW_UTI_IS_BUSY The UTI is busy, maybe the UTI is used by other instance.
                    - \a ERR_CME_UNKNOWN Unknown error occur.
*/
uint32_t CFW_SimGetAuthenticationStatus(
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

/*! \brief  function enter PIN authentication.

The result should be returned through the EV_CFW_SIM_ENTER_AUTH_RSP event:

\rst
+------+-------+------------+---------+------------------------------------------------+
| UTI  | nType |  nParam1   | nParam2 |                  Description                   |
+======+=======+============+=========+================================================+
| nUTI | 0x00  | 0          | 0       | Entry auth OK                                  |
+------+-------+------------+---------+------------------------------------------------+
| nUTI | 0x00  | 1          | 0       | Phone is not waiting for PIN1 or PIN2 password |
+------+-------+------------+---------+------------------------------------------------+
| nUTI | 0x00  | 18         | 0       | PIN1 BLOCK                                     |
+------+-------+------------+---------+------------------------------------------------+
| nUTI | 0x00  | 19         | 0       | PIN2 BLOCK                                     |
+------+-------+------------+---------+------------------------------------------------+
| nUTI | 0xF0  | error code | 0       | Error                                          |
+------+-------+------------+---------+------------------------------------------------+
\endrst

\param [in] pPin    Pointer to a string containing the current password  in ASCII string format. This parameter is shown below in detail:
\rst
+---------+------------------------------------------------------+------------------------------------------------+
| nOption |                         pPin                         |                    pNewPin                     |
+=========+======================================================+================================================+
| 0       | process by CFW, decide to select PIN1 or PIN2.       | process by CFW, decide to select PIN1 or PIN2. |
+---------+------------------------------------------------------+------------------------------------------------+
| 1       | SIM PIN1                                             | NULL(ignored)                                  |
+---------+------------------------------------------------------+------------------------------------------------+
| 1       | PUK,such as SIM PUK or PHSIM PUK or PH-FSIM PUK      | new pin password                               |
+---------+------------------------------------------------------+------------------------------------------------+
| 2       | SIM PIN2                                             | NULL(ignored)                                  |
+---------+------------------------------------------------------+------------------------------------------------+
| 2       | PUK2, such as SIM PUK2 or PHSIM PUK2 or PH-FSIM PUK2 | new pin password                               |
+---------+------------------------------------------------------+------------------------------------------------+
\endrst
\param [in] nPinSize    The size of pPin in bytes, normally this size is 4 Byte for PIN, and 8 Byte for PUK.
\param [in] pNewPin     If the requested code was a PUK, specify a new password or restore the former disabled password, else this parameter is ignored, you can specify pNewPin=NULL.
\param [in] nNewPinSize The size of nNewPinSize in bytes, normally this size is 4 Byte for PIN, and 8 Byte for PUK. If pNewPin is NULL, this parameter is ignored.
\param [in] nOption     The option to enter pin, see above.
\param [in] nUTI        Specify the UTI (Unique Transaction Identifier) to support multi-application in parallel. This parameter will be returned in notification event (see below). You also can get a free UTI by CFW_GetFreeUTI function if you application is a dynamical module.
\return \a ERR_SUCCESS on success, or error code
                    - \a ERR_CFW_INVALID_PARAMETER  The input parameter(nOption) is invalid.
                    - \a ERR_CME_OPERATION_NOT_ALLOWED This function don't complete, and you execute other operations such as call CFW_SimEnterAuthentication.
                    - \a ERR_NO_MORE_MEMORY No any more memory to allocate for SIM management.
                    - \a ERR_CFW_NOT_EXIST_FREE_UTI No any available free UTI in the system.
                    - \a ERR_CFW_UTI_IS_BUSY The UTI is busy, maybe the UTI is used by other instance.
                    - \a ERR_CME_UNKNOWN Unknown error occur.
*/
uint32_t CFW_SimEnterAuthentication(
    uint8_t *pPin,
    uint8_t nPinSize,
    uint8_t *pNewPin,
    uint8_t nNewPinSize,
    uint8_t nOption,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

/*! \brief function locks and unlocks a SIM related facility.

\param [in] nFac  Specify the facility to check its status. The facility type can be one of the following value:
                - \a CFW_STY_FAC_TYPE_SC SIM (lock SIM cards). SIM requests password upon ME power up and when this lock command is issued. "SC" lock is protected with SIM PIN1.
                                         The number can be modified with CFW_SimChangePassword function.
                - \a CFW_STY_FAC_TYPE_FD the phone numbers stored to the "FD" memory can be dialled (depending on the SIM card, usually up to 7 numbers). If PIN2 authentication has not been performed during the current session, PIN2 is requested as <passwd>.
\param [in] pBufPwd   Point to the buffer containing the password in ASCII string format If you don't care password, set this pBufPwd to NULL.
\param [in] nPwdSize  The size of pBufPwdin bytes. If pBufPwd is NULL, this parameter is ignored by this function.
\param [in] nMode     0 to unlock, 1 to lock.
\param [in] nUTI      Specify the UTI (Unique Transaction Identifier) to support multi-application in parallel. This parameter will be returned in notification event (see below). You also can get a free UTI by CFW_GetFreeUTI function if you application is a dynamical module.
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_SimSetFacilityLock(
    uint16_t nFac,
    uint8_t *pBufPwd,
    uint8_t nPwdSize,
    uint8_t nMode,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

/*! \brief function gets the status of a facility.

The result should be returned through the EV_CFW_SIM_GET_FACILITY_LOCK_RSP event:

\rst
+------+-------+-----------------------------+-------------------------------------------------------------------------------------------------+-------------+
| UTI  | nType |           nParam1           |                                             nParam2                                             | Description |
+======+=======+=============================+=================================================================================================+=============+
| nUTI | 0x00  | LOUINT8=contain the status: | Retrieves the number of attempts still available for entering a required password(PIN, PUK etc) | OK          |
|      |       | \ 0:not active;             | \ (LOUINT16(nParam2)&0x0F) = PIN1 Count.                                                        |             |
|      |       | \ 1:active.                 | \ ((LOUINT16(nParam2)&0xF0)>>4) = PUK1Count.                                                    |             |
|      |       |                             | \ ((LOUINT16(nParam2)&0xF00)>>8) = PIN2 Count.                                                  |             |
|      |       |                             | \ ((LOUINT16(nParam2)&0xF000)>>12) = PUK2 Count.                                                |             |
+------+-------+-----------------------------+-------------------------------------------------------------------------------------------------+-------------+
| nUTI | 0xF0  | Error code                  | 0                                                                                               | Error       |
+------+-------+-----------------------------+-------------------------------------------------------------------------------------------------+-------------+
\endrst

\param [in] nFac  Specify the facility to check its status. The facility type can be one of the following value:
                - \a CFW_STY_FAC_TYPE_SC SIM (lock SIM cards). SIM requests password upon ME power up and when this lock command is issued. "SC" lock is protected with SIM PIN1.
                                         The number can be modified with CFW_SimChangePassword function.
                - \a CFW_STY_FAC_TYPE_FD the phone numbers stored to the "FD" memory can be dialled (depending on the SIM card, usually up to 7 numbers). If PIN2 authentication has not been performed during the current session, PIN2 is requested as <passwd>.
\param [in] nUTI    UTI operation
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_SimGetFacilityLock(
    uint16_t nFac,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

/*! \brief function retrieves the International Mobile Subscriber Identity(IMSI).

The result should be returned through the EV_CFW_SIM_GET_PROVIDER_ID_RSP event:

\rst
+-----+-------+-----------------------------------------------------------------------+---------------+-------------+
| UTI | nType |                                nParam1                                |    nParam2    | Description |
+=====+=======+=======================================================================+===============+=============+
| NA  | 0x00  | Pointer to the buffer to hold the IMSI number in ASCII string format. | buffer length | OK          |
+-----+-------+-----------------------------------------------------------------------+---------------+-------------+
| NA  | 0xF0  | Error code                                                            | 0             | Error       |
+-----+-------+-----------------------------------------------------------------------+---------------+-------------+
\endrst

\param [in] nUTI    UTI operation
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_SimGetProviderId(
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

/*! \brief function stores the PLMNs in SIM, this API can be used to delete the PLMNs in SIM.

The result should be returned through the EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSPevent:

\rst
+------+-------+------------+---------+-------------+
| UTI  | nType |  nParam1   | nParam2 | Description |
+======+=======+============+=========+=============+
| nUTI | 0x00  | 0          | 0       | OK          |
+------+-------+------------+---------+-------------+
| nUTI | 0xF0  | Error code | 0       | Error       |
+------+-------+------------+---------+-------------+
\endrst

\param [in] nUTI    UTI operation
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
                - \a ERR_CFW_INVALID_PARAMETER  The input parameter is invalid.
                - \a ERR_CFW_NOT_EXIST_FREE_UTI Not any available free UTI in the system.
                - \a ERR_CFW_UTI_IS_BUSY        The UTI is busy, maybe the UTI is used by other instance.
                - \a ERR_CME_MEMORY_FULL        Not any more memory to use.
                - \a ERR_CME_UNKNOWN            Unknown error occurred.
*/
uint32_t CFW_SimSetPrefOperatorList(
    uint8_t *pOperatorList,
    uint8_t nSize,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

/*! \brief function gets the status of a facility.

The result should be returned through the EV_CFW_SIM_GET_PREF_OPT_LIST_MAXNUM_RSP event:

\rst
+------+-------+-----------------------------------------------------------------------------+---------+-------------+
| UTI  | nType |                                   nParam1                                   | nParam2 | Description |
+======+=======+=============================================================================+=========+=============+
| nUTI | 0x00  | The maximum number of PLMNs that can be stored in this SIM card, always >=8 | 0       | OK          |
+------+-------+-----------------------------------------------------------------------------+---------+-------------+
| nUTI | 0xF0  | Error code                                                                  | 0       | Error       |
+------+-------+-----------------------------------------------------------------------------+---------+-------------+
\endrst

\param [in] nUTI    UTI operation
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
                    - \a    ERR_CFW_INVALID_PARAMETER  The input parameter(nFac) is invalid.
                    - \a    ERR_CFW_NOT_EXIST_FREE_UTI No any available free UTI in the system.
                    - \a    ERR_CFW_UTI_IS_BUSY The UTI is busy, maybe the UTI is used by other instance.
                    - \a    ERR_CME_MEMORY_FULL No any more memory to use.
                    - \a    ERR_CME_UNKNOWN Unknown error occur.
*/
uint32_t CFW_SimGetPrefOperatorListMaxNum(
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

/*! \brief function gets all PLMNs stored in SIM.

The result should be returned through the EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP event:

\rst
+------+-------+---------------------------------------------------------------------+--------------------------+-------------+
| UTI  | nType |                               nParam1                               |         nParam2          | Description |
+======+=======+=====================================================================+==========================+=============+
| nUTI | 0x00  | Pointer to the buffer contained all PLMNs in the format like below: | The number of all PLMNs. | OK          |
| nUTI | 0x00  | 0x04060000000F04060000010F                                          | i.e. 2*6                 |             |
+------+-------+---------------------------------------------------------------------+--------------------------+-------------+
| nUTI | 0xF0  | Error code                                                          | 0                        | Error       |
+------+-------+---------------------------------------------------------------------+--------------------------+-------------+
\endrst

\param [in] nUTI    UTI operation
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
                - \a ERR_CFW_NOT_EXIST_FREE_UTI No any available free UTI in the system.
                - \a ERR_CFW_UTI_IS_BUSY The UTI is busy, maybe the UTI is used by other instance.
                - \a ERR_CME_MEMORY_FULL No any more memory to use.
                - \a ERR_CME_UNKNOWN Unknown error occur.
*/
uint32_t CFW_SimGetPrefOperatorList(
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

/*! \brief The CFW_SimAddPbkEntry function retrieves an entry from the current phone book.

The result should be returned asynchronously through the EV_CFW_SIM_ADD_PBK_RSP event which should below structure:

\rst
+------+-------+-------------------------------------------+----------------------------+-------------+
| UTI  | nType |                  nParam1                  |          nParam2           | Description |
+======+=======+===========================================+============================+=============+
|      |       | Specify the index referenced by nParam1   |                            |             |
|      |       | which indicate the memory location of the | nStorage(The same as input |             |
| nUTI | 0x00  | stored phonebook entry.if nStorage=       | parameter)                 | OK          |
|      |       | CFW_PBK_SIM_LAST_DIALLING,nParam1=0.      |                            |             |
+------+-------+-------------------------------------------+----------------------------+-------------+
|      |       |                                           | LOUINT16=Specify the index |             |
| nUTI | 0xF0  | SIM_Errors_Code                           | of the entry to add   .    | Error       |
+------+-------+-------------------------------------------+----------------------------+-------------+
\endrst

\param [in] nStorage    Specify the current phonebook memory storage. This parameter can be one of the following value:
\rst
+---------------------------+------------------------------------+
|      Memory Storage       |            Description             |
+===========================+====================================+
|                           | SIM phonebook(storage depends on   |
| CFW_PBK_SIM               | SIM Card).                         |
+---------------------------+------------------------------------+
|                           | SIM fix dialing phonebook (FD      |
| CFW_PBK_SIM_FIX_DIALLING  | Phonebook storage).                |
+---------------------------+------------------------------------+
|                           | SIM last-dialing-phonebook (LD     |
| CFW_PBK_SIM_LAST_DIALLING | Phonebook storage).                |
+---------------------------+------------------------------------+
| CFW_PBK_ON                | Own numbers (MSISDNs) list.        |
+---------------------------+------------------------------------+
|                           | The phonebook storage is specified |
| CFW_PBK_AUTO              | by CFW_CfgSetPbkStorage function   |
+---------------------------+------------------------------------+
\endrst
\param [in] pEntryInfo   Pointer to CFW_SIM_PBK_ENTRY_INFO structure to hold the phonebook entry info.
\param [in] nUTI    Specify the UTI (Unique Transaction Identifier) operation, which support multi-application in parallel. This parameter will
be return in notification event (see below). You also can get a free UTI by CFW_GetFreeUTI function if you application is a dynamical
module. This parameter should be less than 255 for all upper layer application.
\param [in] nSimID  SIM ID
\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+----------------------------+-----------------------------------------+
|         Error Code         |               Description               |
+============================+=========================================+
| ERR_CFW_INVALID_PARAMETER  | The input parameter(nIndex) is invalid. |
+----------------------------+-----------------------------------------+
| ERR_CFW_NOT_EXIST_FREE_UTI | No any available free UTI in the system |
+----------------------------+-----------------------------------------+
|                            | The UTI is busy, maybe the UTI is used  |
| ERR_CFW_UTI_IS_BUSY        | by other instance.                      |
+----------------------------+-----------------------------------------+
| ERR_CME_MEMORY_FULL        | No any more memory to allocate          |
+----------------------------+-----------------------------------------+
| ERR_CME_UNKNOWN            | Unknown error occur.                    |
+----------------------------+-----------------------------------------+
\endrst
*/

uint32_t CFW_SimAddPbkEntry(
    uint8_t nStorage,
    CFW_SIM_PBK_ENTRY_INFO *pEntryInfo,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

/*! \brief The CFW_SimAddPbkEntry_V2 function retrieves an entry from the current phone book.

The result should be returned asynchronously through the EV_CFW_SIM_ADD_PBK_RSP event which should below structure:

\rst
+------+-------+-------------------------------------------+----------------------------+-------------+
| UTI  | nType |                  nParam1                  |          nParam2           | Description |
+======+=======+===========================================+============================+=============+
|      |       | Specify the index referenced by nParam1   |                            |             |
|      |       | which indicate the memory location of the | nStorage(The same as input |             |
| nUTI | 0x00  | stored phonebook entry.if nStorage=       | parameter)                 | OK          |
|      |       | CFW_PBK_SIM_LAST_DIALLING,nParam1=0.      |                            |             |
+------+-------+-------------------------------------------+----------------------------+-------------+
|      |       |                                           | LOUINT16=Specify the index |             |
| nUTI | 0xF0  | SIM_Errors_Code                           | of the entry to add   .    | Error       |
+------+-------+-------------------------------------------+----------------------------+-------------+
\endrst

\param [in] nStorage    Specify the current phonebook memory storage. This parameter can be one of the following value:
\rst
+---------------------------+------------------------------------+
|      Memory Storage       |            Description             |
+===========================+====================================+
|                           | SIM phonebook(storage depends on   |
| CFW_PBK_SIM               | SIM Card).                         |
+---------------------------+------------------------------------+
|                           | SIM fix dialing phonebook (FD      |
| CFW_PBK_SIM_FIX_DIALLING  | Phonebook storage).                |
+---------------------------+------------------------------------+
|                           | SIM last-dialing-phonebook (LD     |
| CFW_PBK_SIM_LAST_DIALLING | Phonebook storage).                |
+---------------------------+------------------------------------+
| CFW_PBK_ON                | Own numbers (MSISDNs) list.        |
+---------------------------+------------------------------------+
|                           | The phonebook storage is specified |
| CFW_PBK_AUTO              | by CFW_CfgSetPbkStorage function   |
+---------------------------+------------------------------------+
\endrst
\param [in] pEntryInfo   Pointer to CFW_SIM_PBK_ENTRY_INFO_V2 structure to hold the phonebook entry info.
\param [in] nUTI    Specify the UTI (Unique Transaction Identifier) operation, which support multi-application in parallel. This parameter will
be return in notification event (see below). You also can get a free UTI by CFW_GetFreeUTI function if you application is a dynamical
module. This parameter should be less than 255 for all upper layer application.
\param [in] nSimID  SIM ID
\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+----------------------------+-----------------------------------------+
|         Error Code         |               Description               |
+============================+=========================================+
| ERR_CFW_INVALID_PARAMETER  | The input parameter(nIndex) is invalid. |
+----------------------------+-----------------------------------------+
| ERR_CFW_NOT_EXIST_FREE_UTI | No any available free UTI in the system |
+----------------------------+-----------------------------------------+
|                            | The UTI is busy, maybe the UTI is used  |
| ERR_CFW_UTI_IS_BUSY        | by other instance.                      |
+----------------------------+-----------------------------------------+
| ERR_CME_MEMORY_FULL        | No any more memory to allocate          |
+----------------------------+-----------------------------------------+
| ERR_CME_UNKNOWN            | Unknown error occur.                    |
+----------------------------+-----------------------------------------+
\endrst
*/
uint32_t CFW_SimAddPbkEntry_V2(
    uint8_t nStorage,
    CFW_SIM_PBK_ENTRY_INFO_V2 *pEntryInfo,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

/*! \brief The CFW_SimDeletePbkEntry function deletes an entry from the current phone book by pass the specified index.

the result should be returned through the EV_CFW_SIM_DELETE_PBK_ENTRY_RSP event:

\rst
+------+-------+----------------------------+----------------------------+-------------+
| UTI  | nType |          nParam1           |          nParam2           | Description |
+======+=======+============================+============================+=============+
|      |       | LOUINT16=Specify the index |                            |             |
| nUTI | 0x00  | of the entry to delete.    | 0                          | OK          |
+------+-------+----------------------------+----------------------------+-------------+
|      |       |                            | LOUINT16=Specify the index |             |
| nUTI | 0xF0  | SIM_Errors_Code            | of the entry to delete.    | Error       |
+------+-------+----------------------------+----------------------------+-------------+
\endrst

\param [in] nStorage Specify the current phonebook memory storage. This parameter can be one of the following value:
\rst
+---------------------------+------------------------------------+
|      Memory Storage       |            Description             |
+===========================+====================================+
|                           | SIM phonebook(storage depends on   |
| CFW_PBK_SIM               | SIM Card).                         |
+---------------------------+------------------------------------+
|                           | SIM fix dialing phonebook (FD      |
| CFW_PBK_SIM_FIX_DIALLING  | Phonebook storage).                |
+---------------------------+------------------------------------+
|                           | SIM last-dialing-phonebook (LD     |
| CFW_PBK_SIM_LAST_DIALLING | Phonebook storage).                |
+---------------------------+------------------------------------+
| CFW_PBK_ON                | Own numbers (MSISDNs) list.        |
+---------------------------+------------------------------------+
|                           | The phonebook storage is specified |
| CFW_PBK_AUTO              | by CFW_CfgSetPbkStorage function   |
+---------------------------+------------------------------------+
\endrst
\param [in] nIndex  Specify the location number where reading.
\param [in] nUTI    Specify the UTI (Unique Transaction Identifier) operation, which support multi-application in parallel. This parameter will
be return in notification event (see below). You also can get a free UTI by CFW_GetFreeUTI function if you application is a dynamical
module. This parameter should be less than 255 for all upper layer application.
\param [in] nSimID  SIM ID
\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+----------------------------+-----------------------------------------+
|         Error Code         |               Description               |
+============================+=========================================+
| ERR_CFW_INVALID_PARAMETER  | The input parameter(nIndex) is invalid. |
+----------------------------+-----------------------------------------+
| ERR_CFW_NOT_EXIST_FREE_UTI | No any available free UTI in the system |
+----------------------------+-----------------------------------------+
|                            | The UTI is busy, maybe the UTI is used  |
| ERR_CFW_UTI_IS_BUSY        | by other instance.                      |
+----------------------------+-----------------------------------------+
| ERR_CME_MEMORY_FULL        | No any more memory to allocate          |
+----------------------------+-----------------------------------------+
| ERR_CME_UNKNOWN            | Unknown error occur.                    |
+----------------------------+-----------------------------------------+
\endrst
*/
uint32_t CFW_SimDeletePbkEntry(
    uint8_t nStorage,
    uint16_t nIndex,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);
/*! \brief The CFW_SimGetPbkEntry function retrieves an entry from the current phone book.

the result should be returned through the EV_CFW_SIM_GET_PBK_ENTRY_RSP event:

\rst
+------+-------+-----------------+-------------------------------------+-------------+
| UTI  | nType |     nParam1     |               nParam2               | Description |
+======+=======+=================+=====================================+=============+
|      |       |                 | HIUINT16=The size of structure      |             |
| nUTI | 0x00  | 0               | LOUINT16=The index of SIM pbk entry | OK          |
+------+-------+-----------------+-------------------------------------+-------------+
| nUTI | 0xF0  | SIM_Errors_Code | 0                                   | Error       |
+------+-------+-----------------+-------------------------------------+-------------+
\endrst

\param [in] nStorage    Specify the current phonebook memory storage. This parameter can be one of the following value:
\rst
+---------------------------+------------------------------------+
|      Memory Storage       |            Description             |
+===========================+====================================+
|                           | SIM phonebook(storage depends on   |
| CFW_PBK_SIM               | SIM Card).                         |
+---------------------------+------------------------------------+
|                           | SIM fix dialing phonebook (FD      |
| CFW_PBK_SIM_FIX_DIALLING  | Phonebook storage).                |
+---------------------------+------------------------------------+
|                           | SIM last-dialing-phonebook (LD     |
| CFW_PBK_SIM_LAST_DIALLING | Phonebook storage).                |
+---------------------------+------------------------------------+
| CFW_PBK_ON                | Own numbers (MSISDNs) list.        |
+---------------------------+------------------------------------+
| CFW_PBK_EN                | Emergency Call storage             |
+---------------------------+------------------------------------+
|                           | The phonebook storage is specified |
| CFW_PBK_AUTO              | by CFW_CfgSetPbkStorage function   |
+---------------------------+------------------------------------+
\endrst
\param [in] nIndex  Specify the location number where reading.
\param [in] nUTI    Specify the UTI (Unique Transaction Identifier) operation, which support multi-application in parallel. This parameter will
be return in notification event (see below). You also can get a free UTI by CFW_GetFreeUTI function if you application is a dynamical
module. This parameter should be less than 255 for all upper layer application.
\param [in] nSimID  SIM ID
\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+----------------------------+-----------------------------------------+
|         Error Code         |               Description               |
+============================+=========================================+
| ERR_CFW_INVALID_PARAMETER  | The input parameter(nIndex) is invalid. |
+----------------------------+-----------------------------------------+
| ERR_CFW_NOT_EXIST_FREE_UTI | No any available free UTI in the system |
+----------------------------+-----------------------------------------+
|                            | The UTI is busy, maybe the UTI is used  |
| ERR_CFW_UTI_IS_BUSY        | by other instance.                      |
+----------------------------+-----------------------------------------+
| ERR_CME_MEMORY_FULL        | No any more memory to allocate          |
+----------------------------+-----------------------------------------+
\endrst
*/
uint32_t CFW_SimGetPbkEntry(
    uint8_t nStorage,
    uint16_t nIndex,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);
/*! \brief The CFW_SimListPbkEntries function retrieves entries from the current phone book by passed the start and end index.

The result should be returned through the EV_CFW_SIM_LIST_PBK_ENTRY_RSP event:

\rst
+------+-------+------------------------+------------------+-------------+
| UTI  | nType |        nParam1         |     nParam2      | Description |
+======+=======+========================+==================+=============+
|      |       | Pointer to the         |                  |             |
| nUTI | 0x00  | CFW_SIM_PBK_ENTRY_INFO | counter of this  | OK          |
|      |       | structure array.       | structure array. |             |
+------+-------+------------------------+------------------+-------------+
| nUTI | 0xF0  | SIM_Errors_Code        | 0                | Error       |
+------+-------+------------------------+------------------+-------------+
\endrst

\param [in] nStorage Specify the current phonebook memory storage. This parameter can be one of the following value:
\rst
+---------------------------+------------------------------------+
|      Memory Storage       |            Description             |
+===========================+====================================+
|                           | SIM phonebook(storage depends on   |
| CFW_PBK_SIM               | SIM Card).                         |
+---------------------------+------------------------------------+
|                           | SIM fix dialing phonebook (FD      |
| CFW_PBK_SIM_FIX_DIALLING  | Phonebook storage).                |
+---------------------------+------------------------------------+
|                           | SIM last-dialing-phonebook (LD     |
| CFW_PBK_SIM_LAST_DIALLING | Phonebook storage).                |
+---------------------------+------------------------------------+
| CFW_PBK_ON                | Own numbers (MSISDNs) list.        |
+---------------------------+------------------------------------+
| CFW_PBK_EN                | Emergency Call storage             |
+---------------------------+------------------------------------+
|                           | The phonebook storage is specified |
| CFW_PBK_AUTO              | by CFW_CfgSetPbkStorage function   |
+---------------------------+------------------------------------+
\endrst
\param [in] nIndexStart  Specify the location number where reading starts.
\param [in] nIndexEnd    Specify the location number where reading ends.
\param [in] nUTI    Specify the UTI (Unique Transaction Identifier) operation, which support multi-application in parallel. This parameter will
be return in notification event (see below). You also can get a free UTI by CFW_GetFreeUTI function if you application is a dynamical
module. This parameter should be less than 255 for all upper layer application.
\param [in] nSimID  SIM ID
\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+----------------------------+-----------------------------------------+
|         Error Code         |               Description               |
+============================+=========================================+
| ERR_CFW_INVALID_PARAMETER  | The input parameter(nIndex) is invalid. |
+----------------------------+-----------------------------------------+
| ERR_CFW_NOT_EXIST_FREE_UTI | No any available free UTI in the system |
+----------------------------+-----------------------------------------+
|                            | The UTI is busy, maybe the UTI is used  |
| ERR_CFW_UTI_IS_BUSY        | by other instance.                      |
+----------------------------+-----------------------------------------+
| ERR_CME_UNKNOWN            | Unknown error occur.                    |
+----------------------------+-----------------------------------------+
\endrst
*/
uint32_t CFW_SimListPbkEntries(
    uint8_t nStorage,
    uint16_t nIndexStart,
    uint16_t nIndexEnd,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);
/*! \brief The CFW_SimListPbkEntries function retrieves entries from the current phone book by passed the startindex.

The result should be returned through the EV_CFW_SIM_LIST_COUNT_PBK_ENTRY_RSP event:

\rst
+------+-------+------------------------+------------------+-------------+
| UTI  | nType |        nParam1         |     nParam2      | Description |
+======+=======+========================+==================+=============+
|      |       | Pointer to the         |                  |             |
| nUTI | 0x00  | CFW_SIM_PBK_ENTRY_INFO | counter of this  | OK          |
|      |       | structure array.       | structure array. |             |
+------+-------+------------------------+------------------+-------------+
| nUTI | 0xF0  | SIM_Errors_Code        | 0                | Error       |
+------+-------+------------------------+------------------+-------------+
\endrst

\param [in] nStorage Specify the current phonebook memory storage. This parameter can be one of the following value:
\rst
+---------------------------+----------------------------------+
|      Memory Storage       |           Description            |
+===========================+==================================+
|                           | SIM phonebook(storage depends on |
| CFW_PBK_SIM               | SIM Card).                       |
+---------------------------+----------------------------------+
|                           | SIM fix dialing phonebook (FD    |
| CFW_PBK_SIM_FIX_DIALLING  | Phonebook storage).              |
+---------------------------+----------------------------------+
|                           | SIM last-dialing-phonebook (LD   |
| CFW_PBK_SIM_LAST_DIALLING | Phonebook storage).              |
+---------------------------+----------------------------------+
| CFW_PBK_ON                | Own numbers (MSISDNs) list.      |
+---------------------------+----------------------------------+
| CFW_PBK_EN                | Emergency Call storage           |
+---------------------------+----------------------------------+
\endrst
\param [in] nIndexStart  Specify the location number where reading starts.
\param [in] nCount   The number of PbkEntries.
\param [in] nUTI    Specify the UTI (Unique Transaction Identifier) operation, which support multi-application in parallel. This parameter will
be return in notification event (see below). You also can get a free UTI by CFW_GetFreeUTI function if you application is a dynamical
module. This parameter should be less than 255 for all upper layer application.
\param [in] nSimID  SIM ID
\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+----------------------------+-----------------------------------------+
|         Error Code         |               Description               |
+============================+=========================================+
| ERR_CFW_INVALID_PARAMETER  | The input parameter(nIndex) is invalid. |
+----------------------------+-----------------------------------------+
| ERR_CFW_NOT_EXIST_FREE_UTI | No any available free UTI in the system |
+----------------------------+-----------------------------------------+
|                            | The UTI is busy, maybe the UTI is used  |
| ERR_CFW_UTI_IS_BUSY        | by other instance.                      |
+----------------------------+-----------------------------------------+
| ERR_CME_UNKNOWN            | Unknown error occur.                    |
+----------------------------+-----------------------------------------+
\endrst
*/
uint32_t CFW_SimListCountPbkEntries(
    uint8_t nStorage,
    uint16_t nIndexStart,
    uint16_t nCount,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);
/*! \brief The CFW_SimGetPbkStrorageInfo gets the current phone book storage information.

The result should be returned through the EV_CFW_SIM_GET_PBK_STRORAGE_INFO_RSP event:

\rst
+------+-------+-----------------------+------------------------+-------------+
| UTI  | nType |        nParam1        |        nParam2         | Description |
+======+=======+=======================+========================+=============+
|      |       | Pointer to the        |                        |             |
| nUTI | 0x00  | CFW_PBK_STRORAGE_INFO | size of this structure | OK          |
|      |       | structure array.      |                        |             |
+------+-------+-----------------------+------------------------+-------------+
| nUTI | 0xF0  | SIM_Errors_Code       | 0                      | Error       |
+------+-------+-----------------------+------------------------+-------------+
\endrst

\param [in] nStorage Specify the current phonebook memory storage. This parameter can be one of the following value:
\rst
+---------------------------+----------------------------------+
|      Memory Storage       |           Description            |
+===========================+==================================+
|                           | SIM phonebook(storage depends on |
| CFW_PBK_SIM               | SIM Card).                       |
+---------------------------+----------------------------------+
|                           | SIM fix dialing phonebook (FD    |
| CFW_PBK_SIM_FIX_DIALLING  | Phonebook storage).              |
+---------------------------+----------------------------------+
|                           | SIM last-dialing-phonebook (LD   |
| CFW_PBK_SIM_LAST_DIALLING | Phonebook storage).              |
+---------------------------+----------------------------------+
| CFW_PBK_ON                | Own numbers (MSISDNs) list.      |
+---------------------------+----------------------------------+
| CFW_PBK_EN                | Emergency Call storage           |
+---------------------------+----------------------------------+
\endrst
\param [in] nUTI    Specify the UTI (Unique Transaction Identifier) operation, which support multi-application in parallel. This parameter will
be return in notification event (see below). You also can get a free UTI by CFW_GetFreeUTI function if you application is a dynamical
module. This parameter should be less than 255 for all upper layer application.
\param [in] nSimID  SIM ID
\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+----------------------------+-----------------------------------------+
|         Error Code         |               Description               |
+============================+=========================================+
| ERR_CFW_INVALID_PARAMETER  | The input parameter(nIndex) is invalid. |
+----------------------------+-----------------------------------------+
| ERR_CFW_NOT_EXIST_FREE_UTI | No any available free UTI in the system |
+----------------------------+-----------------------------------------+
|                            | The UTI is busy, maybe the UTI is used  |
| ERR_CFW_UTI_IS_BUSY        | by other instance.                      |
+----------------------------+-----------------------------------------+
| ERR_CME_UNKNOWN            | Unknown error occur.                    |
+----------------------------+-----------------------------------------+
| ERR_NO_MORE_MEMORY         | No more memory to allocated             |
+----------------------------+-----------------------------------------+
\endrst
*/
uint32_t CFW_SimGetPbkStrorageInfo(
    uint8_t nStorage,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);
/*! \brief The CFW_SimGetPbkStorage gets the current phone storage using statsus.

The result should be returned through the EV_CFW_SIM_GET_PBK_STRORAGE_RSP event:

\rst
+------+-------+--------------------------------+------------------------+-------------+
| UTI  | nType |            nParam1             |        nParam2         | Description |
+======+=======+================================+========================+=============+
|      |       | Pointer to the CFW_PBK_STORAGE |                        |             |
| nUTI | 0x00  | structure array.               | size of this structure | OK          |
+------+-------+--------------------------------+------------------------+-------------+
| nUTI | 0xF0  | SIM_Errors_Code                | 0                      | Error       |
+------+-------+--------------------------------+------------------------+-------------+
\endrst

\param [in] nStorage Specify the current phonebook memory storage. This parameter can be one of the following value:
\rst
+---------------------------+----------------------------------+
|      Memory Storage       |           Description            |
+===========================+==================================+
|                           | SIM phonebook(storage depends on |
| CFW_PBK_SIM               | SIM Card).                       |
+---------------------------+----------------------------------+
|                           | SIM fix dialing phonebook (FD    |
| CFW_PBK_SIM_FIX_DIALLING  | Phonebook storage).              |
+---------------------------+----------------------------------+
|                           | SIM last-dialing-phonebook (LD   |
| CFW_PBK_SIM_LAST_DIALLING | Phonebook storage).              |
+---------------------------+----------------------------------+
| CFW_PBK_ON                | Own numbers (MSISDNs) list.      |
+---------------------------+----------------------------------+
| CFW_PBK_EN                | Emergency Call storage           |
+---------------------------+----------------------------------+
\endrst
\param [in] nUTI    Specify the UTI (Unique Transaction Identifier) operation, which support multi-application in parallel. This parameter will
be return in notification event (see below). You also can get a free UTI by CFW_GetFreeUTI function if you application is a dynamical
module. This parameter should be less than 255 for all upper layer application.
\param [in] nSimID  SIM ID
\return
If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the following error code may be returned.
\rst
+----------------------------+-----------------------------------------+
|         Error Code         |               Description               |
+============================+=========================================+
| ERR_CFW_INVALID_PARAMETER  | The input parameter(nIndex) is invalid. |
+----------------------------+-----------------------------------------+
| ERR_CFW_NOT_EXIST_FREE_UTI | No any available free UTI in the system |
+----------------------------+-----------------------------------------+
|                            | The UTI is busy, maybe the UTI is used  |
| ERR_CFW_UTI_IS_BUSY        | by other instance.                      |
+----------------------------+-----------------------------------------+
| ERR_CME_UNKNOWN            | Unknown error occur.                    |
+----------------------------+-----------------------------------------+
| ERR_NO_MORE_MEMORY         | No more memory to allocated             |
+----------------------------+-----------------------------------------+
\endrst
*/
uint32_t CFW_SimGetPbkStorage(
    uint8_t nStorage,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);
void CFW_SimSetProiorityResetFlag(
    uint8_t nResetFlag);

uint32_t CFW_SimWritePreferPLMN(uint8_t index, uint8_t *operator, uint8_t nSize, uint16_t nUTI, CFW_SIM_ID nSimID);

uint32_t CFW_SimReadPreferPLMN(uint16_t nUTI, CFW_SIM_ID nSimID);

uint32_t CFW_SimSetSaveFileFlag(
    uint8_t nFlag);

uint8_t CFW_SimGetSaveFileFlag(
    void);
uint32_t CFW_SimSetSaveFileList(
    uint8_t FileList[8],
    uint8_t Num,
    CFW_SIM_ID nSimID);
uint32_t CFW_SimGetFileUpdateCount(
    uint8_t nFile,
    CFW_SIM_ID nSimID);
uint32_t CFW_SimUpdateSaveFile(
    uint8_t FileList[8],
    uint8_t num,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

uint32_t CFW_SatActivation(
    uint8_t nMode,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

uint32_t CFW_SatResponse(
    uint8_t nCmdType,
    uint8_t nStatus,
    uint8_t nItemId,
    void *pInputString,
    uint8_t InputStrLen,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

uint32_t CFW_SatGetInformation(
    uint8_t nCmdType,
    void **pCmdData,
    CFW_SIM_ID nSimID);

uint8_t CFW_SatGetCurCMD(CFW_SIM_ID nSimID);
void CFW_GetMNCLen(uint8_t *pLen, CFW_SIM_ID nSimID);

uint32_t CFW_CfgSimGetMeProfile(CFW_PROFILE *pMeProfile);
uint32_t CFW_CfgSimSetMeProfile(CFW_PROFILE *pMeProfile);

uint32_t CFW_CfgSetCdrxNoSleepEnable(uint8_t bEnable, CFW_SIM_ID nSimID);
uint8_t CFW_CfgGetCdrxNoSleepEnable(CFW_SIM_ID nSimID);
uint32_t CFW_CfgSetPeriodTauAftLocalRel(uint8_t bEnable, CFW_SIM_ID nSimID);
uint8_t CFW_CfgGetPeriodTauAftLocalRel(CFW_SIM_ID nSimID);

uint32_t CFW_CfgSetPocEnable(uint8_t bEnable);
uint8_t CFW_CfgGetPocEnable(void);
uint32_t CFW_CfgSetEnhancePwrSweepNonCE(uint8_t bEnable, CFW_SIM_ID nSimID);
uint8_t CFW_CfgGetEnhancePwrSweepNonCE(CFW_SIM_ID nSimID);
uint32_t CFW_NwSetSimSwich(uint8_t bOnGoing);
uint8_t CFW_NwGetSimSwitch(void);
uint32_t CFW_NwSetPocSimIndex(CFW_SIM_ID nSimID);
typedef struct
{
    uint8_t Bitmap;
    uint8_t reserved;
    uint16_t Treselection;
    uint16_t TreselectionB;
    uint8_t ThreshAQ;
    uint8_t ThreshAP;
    uint8_t ThreshASINR;
    uint8_t ThreshBQ;
    uint8_t ThreshBP;
    uint8_t ThreshBSINR;
    uint16_t n;
    uint16_t reserved2;
} CFW_EMOD_POC_PARA_T;

uint32_t CFW_EmodGetPocPara(CFW_EMOD_POC_PARA_T *pParam, CFW_SIM_ID nSimID);
uint32_t CFW_EmodSetPocPara(CFW_EMOD_POC_PARA_T *pParam, CFW_SIM_ID nSimID);

void CFW_EmodSetPSMThd(uint32_t nMinute);
uint32_t CFW_EmodGetPSMThd(void);

uint16_t CFW_EmodGetLpsFlag(void);

uint32_t CFW_SimGetICCID(uint16_t nUTI, CFW_SIM_ID nSimID);

// SAT

typedef struct _CFW_SAT_ITEM_LIST
{
    uint8_t nItemID;
    uint8_t nItemLen;
    uint8_t padding[2];
    uint8_t *pItemStr;
    struct _CFW_SAT_ITEM_LIST *pNextItem;
} CFW_SAT_ITEM_LIST;

uint32_t CFW_SatGetStatus(
    uint16_t nUTI);

uint8_t CFW_SimGetFileType(uint8_t FileID);

#define CFW_SIM_REFRESH_MAX_FILE_NUM 40
typedef struct
{
    uint8_t file_num;                               //the file number in the file list
    uint8_t file_arr[CFW_SIM_REFRESH_MAX_FILE_NUM]; //the file array
} CFW_SAT_REFRESH_FILE_CHG_REQ_T;

typedef struct
{
    uint8_t file_name;  //the file_name
    uint8_t record_num; //the record number of the file
} CFW_SIM_FILE_INFO_T;

typedef struct
{
    uint8_t result;
    uint8_t file_num;                                            //the file number in the file list
    CFW_SIM_FILE_INFO_T file_info[CFW_SIM_REFRESH_MAX_FILE_NUM]; //the file info
} CFW_SAT_REFRESH_FILE_CHG_CNF_T;

uint32_t CFW_SatRefreshFileChange(
    CFW_SAT_REFRESH_FILE_CHG_REQ_T FileChangeReq,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

// CC Network Error code
#define CFW_CC_CAUSE_NOUSED_CAUSE 0
#define CFW_CC_CAUSE_UNASSIGNED_NUMBER 1
#define CFW_CC_CAUSE_NO_ROUTE_IN_TRANSIT_NETWORK 2
#define CFW_CC_CAUSE_NO_ROUTE_TO_DESTINATION 3
#define CFW_CC_CAUSE_CHANNEL_UNACCEPTABLE 6
#define CFW_CC_CAUSE_CALL_AWARDED 7
#define CFW_CC_CAUSE_OPERATOR_DETERMINED_BARRING 8
#define CFW_CC_CAUSE_NORMAL_CALL_CLEARING 16
#define CFW_CC_CAUSE_USER_BUSY 17
#define CFW_CC_CAUSE_NO_USER_RESPONDING 18
#define CFW_CC_CAUSE_USER_ALERTING_NO_ANSWER 19
#define CFW_CC_CAUSE_CALL_REJECTED 21
#define CFW_CC_CAUSE_NUMBER_CHANGED 22
#define CFW_CC_CAUSE_PRE_EMPTION 25
#define CFW_CC_CAUSE_NON_SELECTED_USER_CLEARING 26
#define CFW_CC_CAUSE_DESTINATION_OUT_OF_ORDER 27
#define CFW_CC_CAUSE_INVALID_NUMBER_FORMAT 28
#define CFW_CC_CAUSE_FACILITY_REJECTED 29
#define CFW_CC_CAUSE_RESPONSE_TO_STATUS_ENQUIRY 30
#define CFW_CC_CAUSE_NORMAL_UNSPECIFIED 31
#define CFW_CC_CAUSE_NO_CIRCUIT_CHANNEL_AVAILABLE 34
#define CFW_CC_CAUSE_NETWORK_OUT_OF_ORDER 38
#define CFW_CC_CAUSE_TEMPORARY_FAILURE 41
#define CFW_CC_CAUSE_SWITCHING_EQUIPMENT_CONGESTION 42
#define CFW_CC_CAUSE_ACCESS_INFORMATION_DISCARDED 43
#define CFW_CC_CAUSE_CHANNEL_UNAVAILABLE 44
#define CFW_CC_CAUSE_RESOURCES_UNAVAILABLE 47
#define CFW_CC_CAUSE_QOS_UNAVAILABLE 49
#define CFW_CC_CAUSE_FACILITY_NOT_SUBSCRIBED 50
#define CFW_CC_CAUSE_MT_CALLS_BARRED 55
#define CFW_CC_CAUSE_BC_NOT_AUTHORIZED 57
#define CFW_CC_CAUSE_BC_NOT_PRESENTLY_AVAILABLE 58
#define CFW_CC_CAUSE_SERVICE_UNAVAILABLE 63
#define CFW_CC_CAUSE_BEARER_SERVICE_NOT_IMPLEMENTED 65
#define CFW_CC_CAUSE_CHANNEL_NOT_IMPLEMENTED 66
#define CFW_CC_CAUSE_ACM_EQUAL_TO_OR_GREATER_THAN_ACMMAX 68
#define CFW_CC_CAUSE_REQUESTED_FACILITY_NOT_IMPLEMENTED 69
#define CFW_CC_CAUSE_ONLY_RESTRICTED_DIGITAL_INFORMATION_BC_AVAILABLE 70
#define CFW_CC_CAUSE_SERVICE_NOT_IMPLEMENTED 71
#define CFW_CC_CAUSE_INVALID_TI_VALUE 81
#define CFW_CC_CAUSE_CHANNEL_DOES_NOT_EXIST 82
#define CFW_CC_CAUSE_CALL_IDENTITY_DOESNT_EXIST 83
#define CFW_CC_CAUSE_CALL_IDENTITY_IN_USE 84
#define CFW_CC_CAUSE_NO_CALL_SUSPENDED 85
#define CFW_CC_CAUSE_CALL_CLEARED 86
#define CFW_CC_CAUSE_USER_NOT_MEMBER_OF_CUG 87
#define CFW_CC_CAUSE_INCOMPATIBLE_DESTINATION 88
#define CFW_CC_CAUSE_INVALID_TRANSIT_NETWORK 91
#define CFW_CC_CAUSE_SEMANTICALLY_INCORRECT_MESSAGE 94
#define CFW_CC_CAUSE_INVALID_MESSAGE 95
#define CFW_CC_CAUSE_INVALID_MANDATORY_INFORMATION 96
#define CFW_CC_CAUSE_MESSAGE_TYPE_NOT_IMPLEMENTED 97
#define CFW_CC_CAUSE_MESSAGE_TYPE_NOT_COMPATIBLE 98
#define CFW_CC_CAUSE_IE_NOT_IMPLEMENTED 99
#define CFW_CC_CAUSE_CONDITIONAL_IE_ERROR 100
#define CFW_CC_CAUSE_MESSAGE_NOT_COMPATIBLE 101
#define CFW_CC_CAUSE_RECOVERY_ON_TIMER_EXPIRY 102
#define CFW_CC_CAUSE_PROTOCOL_ERROR_UNSPECIFIED 111
#define CFW_CC_CAUSE_INTERWORKING_UNSPECIFIED 127
#define CFW_CM_CAUSE_SUCCESS 255

// CC local error
#define CFW_CC_ERR_NORMAL_RELEASE 0
#define CFW_CC_ERR_LOW_LAYER_FAIL 1
#define CFW_CC_ERR_REQUEST_IMPOSSIBLE 2
#define CFW_CC_ERR_INCOMING_CALL_BARRED 3
#define CFW_CC_ERR_OUTGOING_CALL_BARRED 4
#define CFW_CC_ERR_CALL_HOLD_REJECTED 5
#define CFW_CC_ERR_CALL_RTRV_REJECTED 6
#define CFW_CC_ERR_CC_TIMER_EXPIRY 7
#define CFW_CC_ERR_CC_PROTOCOL_ERROR 8
#define CFW_CC_ERR_NOT_USED 0xff

// Crss error
#define CFW_CRSS_ERR_RETURN_ERROR 0x0002
#define CFW_CRSS_ERR_GENERAL_PROBLEM_UNRECOGNIZED_COMPONENT 0x8000
#define CFW_CRSS_ERR_GENERAL_PROBLEM_MISTYPED_COMPONENT 0x8001
#define CFW_CRSS_ERR_GENERAL_PROBLEM_BADLY_STRUCTURED_COMPONENT 0x8002
#define CFW_CRSS_ERR_INVOKE_PROBLEM_DUPLICATE_INVOKE_ID 0x8100
#define CFW_CRSS_ERR_INVOKE_PROBLEM_UNRECOGNIZED_OPERATION 0x8101
#define CFW_CRSS_ERR_INVOKE_PROBLEM_MISTYPED_PARAMETER 0x8102
#define CFW_CRSS_ERR_INVOKE_PROBLEM_RESOURCE_LIMITATION 0x8103
#define CFW_CRSS_ERR_INVOKE_PROBLEM_INITIATING_RELEASE 0x8104
#define CFW_CRSS_ERR_INVOKE_PROBLEM_UNRECOGNIZED_LINKED_ID 0x8105
#define CFW_CRSS_ERR_INVOKE_PROBLEM_LINKED_RESPONSE_UNEXPECTED 0x8106
#define CFW_CRSS_ERR_INVOKE_PROBLEM_UNEXPECTED_LINKED_OPERATION 0x8107
#define CFW_CRSS_ERR_RETURN_RESULT_PROBLEM_UNRECOGNIZED_INVOKE_ID 0x8200
#define CFW_CRSS_ERR_RETURN_RESULT_PROBLEM_RETURN_RESULT_UNEXPECTED 0x8201
#define CFW_CRSS_ERR_RETURN_RESULT_PROBLEM_MISTYPED_PARAMETER 0x8202
#define CFW_CRSS_ERR_RETURN_ERROR_PROBLEM_UNRECOGNIZED_INVOKE_ID 0x8300
#define CFW_CRSS_ERR_RETURN_ERROR_PROBLEM_RETURN_ERROR_UNEXPECTED 0x8301
#define CFW_CRSS_ERR_RETURN_ERROR_PROBLEM_UNRECOGNIZED_ERROR 0x8302
#define CFW_CRSS_ERR_RETURN_ERROR_PROBLEM_UNEXPECTED_ERROR 0x8303
#define CFW_CRSS_ERR_RETURN_ERROR_PROBLEM_MISTYPED_PARAMETER 0x8304

//SS code
#define CFW_SS_NO_CODE
#define CFW_SS_CLIP 30     ///<  CLIP
#define CFW_SS_CLIR 31     ///<  CLIR
#define CFW_SS_COLP 76     ///<  COLP
#define CFW_SS_COLR 77     ///<  COLR
#define CFW_SS_CNAP 300    ///<  CNAP
#define CFW_SS_CFA 2       ///<  All forwarding
#define CFW_SS_CFU 21      ///<  CF unconditional
#define CFW_SS_CD 66       ///<  call deflection
#define CFW_SS_CFC 4       ///<  CF conditional
#define CFW_SS_CFB 67      ///<  CF on MS busy
#define CFW_SS_CFNRY 61    ///<  CF on MS no reply
#define CFW_SS_CFNRC 62    ///<  CF on MS not reachable
#define CFW_SS_USSD_V2 100 ///<  USSD version 2 services  supports
// all USSD Operations except
// ss_ProcessUnstructuredSSData
#define CFW_SS_USSD_V1 101 ///<  USSD version 1 services  supports
// ss_ProcessUnstructuredSSData
// Operations Only
#define CFW_SS_ECT 96                     ///<  explicit call transfer
#define CFW_SS_CW 43                      ///<  call waiting
#define CFW_SS_HOLD 102                   ///<  call hold  (defined by CIITEC)
#define CFW_SS_MPTY 103                   ///<  multiparty (defined by CIITEC)
#define CFW_SS_AOCI 104                   ///<  advice of charge information (defined by CIITEC)
#define CFW_SS_AOCC 105                   ///<  advice of charge charging    (defined by CIITEC)
#define CFW_SS_UUS1 361                   ///<  uus1
#define CFW_SS_UUS2 362                   ///<  uus2
#define CFW_SS_UUS3 363                   ///<  uus3
#define CFW_SS_AllBarringSS 330           ///<  all call barring
#define CFW_SS_BarringOfOutgoingCalls 333 ///<  barring of MO call
#define CFW_SS_BAOC 33                    ///<  barring of all MO call
#define CFW_SS_BOIC 331                   ///<  barring of international MO call
#define CFW_SS_BOIC_ExHC 332              ///<  barring of international MO call
// except those going to the home plmn
#define CFW_SS_BarringOfIncomingCalls 353 ///<  barring of MT call
#define CFW_SS_BAIC 35                    ///<  barring of all MT call
#define CFW_SS_BIC_ROAM 351               ///<  barring of MT call when roaming
// outside of home plmn

//SS operation code
#define CFW_SS_REGISTER 10 ///<  register a SS valid for CF
#define CFW_SS_ERASURE 11  ///<  erase a SS valid for CF
#define CFW_SS_ACTIVATE 12 ///<  activate a SS valid for all
// SS operations except line
// identification
#define CFW_SS_DEACTIVATE 13 ///<  deactivate a SS valid for all
// SS operations except line
// identification
#define CFW_SS_INTERROGATE 14 ///<  interrogate a SS
#define CFW_SS_NOTIFY 16
#define CFW_SS_REGISTERPASSWORD 17       ///<  register a password valid for CB
#define CFW_SS_GETCALLBARRINGPASSWORD 18 ///<  this operation is invoked by
// network only
#define CFW_SS_PROCESSUNSTRUCTUREDSSDATA 19 ///<  This operation is used only
// for USSD Version 1
#define CFW_SS_PROCESSUNSTRUCTUREDSSREQUEST 59 ///<  This operation is used only
// for USSD Version 2
#define CFW_SS_UNSTRUCTUREDSSREQUEST 60 ///<  This operation is used only
// for USSD Version 2
#define CFW_SS_UNSTRUCTUREDSSNOTIFY 61 ///<  This operation is used only
// for USSD Version 2
#define CFW_SS_CALLDEFLECTION 117
#define CFW_SS_USERUSERSERVICE 118
#define CFW_SS_SPLITMPTY 121
#define CFW_SS_RETRIEVEMPTY 122
#define CFW_SS_HOLDMPTY 123
#define CFW_SS_BUILDMPTY 124
#define CFW_SS_FORWARDCHARGEADVICE 125
#define CFW_SS_EXPLICITCT 126

//
// PBK Services
//

#define CFW_PBK_DATA_MAX_SIZE 20
#define PBK_SIM_NUMBER_SIZE SIM_PBK_NUMBER_SIZE
#define CFW_PBK_NAME_MAX_SIZE 20
#define CFW_PBK_NUMBER_MAX_SIZE 20 ///<  BCD

#define PBK_HINT_SIZE 16

typedef struct _CFW_PBK_ENTRY_SIMPLE_HRD
{
    uint16_t nPosition;
    uint8_t nStorageId;
    uint8_t nNameSize;
    uint8_t nNumberSize;
    uint8_t nNumberType;
    uint8_t padding[2];
    uint8_t *pName;
    uint8_t *pNumber;
} CFW_PBK_ENTRY_SIMPLE_HRD;

typedef struct _CFW_PBK_ENTRY_HRD
{
    uint16_t nPosition;
    uint16_t nAppIndex;
    uint8_t *pName;
    uint8_t *pNumber;
    uint8_t nNameSize;
    uint8_t nNumberSize;
    uint8_t nNumberType;
    uint8_t nStorageId;
    uint32_t nItemGroupType;
    uint32_t nGroup;
} CFW_PBK_ENTRY_HRD;

typedef struct _CFW_PBK_ENTRY_ITEM
{
    uint32_t nPosition;
    uint32_t nItemType;
    uint32_t nItemId;
    uint8_t *pItemName;
    uint8_t *pItemData;
    uint8_t nItemNameSize;
    uint8_t nItemDataSize;
    uint8_t nDataType;
    uint8_t padding;
} CFW_PBK_ENTRY_ITEM;

typedef struct _CFW_PBK_DETAIL_ENTRY
{
    CFW_PBK_ENTRY_HRD sHeader;
    uint32_t iItemNum;
    CFW_PBK_ENTRY_ITEM *pItemArray;
} CFW_PBK_DETAIL_ENTRY;

#define CFW_PBK_SEARCH_NAME_FIRST_MATCH 0x01
#define CFW_PBK_SEARCH_NAME_FULL_MATCH 0x02
#define CFW_PBK_SEARCH_NAME_PY_MATCH 0x04
#define CFW_PBK_SEARCH_NUM_FIRST_MATCH 0x08
#define CFW_PBK_SEARCH_NUM_NEXT_MATCH 0x10
#define CFW_PBK_SEARCH_ENTRY_SIMPLE_INFO 0x20
#define CFW_PBK_SEARCH_ENTRY_HEADER_INFO 0x40
#define CFW_PBK_SEARCH_ENTRY_FULL_INFO 0x80

#define CFW_PBK_OPTION_ENTRY_SIMPLE_HEADER 0x01
#define CFW_PBK_OPTION_ENTRY_FULL_HEADER 0x02
#define CFW_PBK_OPTION_ENTRY_ITEM 0x04

#define CFW_PBK_COPY_ME2SM 0x00
#define CFW_PBK_COPY_SM2ME 0x01
#define CFW_PBK_COPY_ME2SM_STEP 0x02
#define CFW_PBK_COPY_SM2ME_STEP 0x04

#define CFW_PBK_ITEM_TYPE_PHONE_NUMBER 0x1 ///< Specify the phone number such as mobile number or home number or office numberW
#define CFW_PBK_ITEM_TYPE_EAMIL 0x2        //
#define CFW_PBK_ITEM_TYPE_ADDERSS 0x4      //
#define CFW_PBK_ITEM_TYPE_WEBSITE 0x8      //
#define CFW_PBK_ITEM_TYPE_STRING 0x10      //
#define CFW_PBK_ITEM_TYPE_RES_ID 0x20      ///<  Specify the resource identifier
#define CFW_PBK_ITEM_TYPE_RES_PATH 0x40    ///<  Specify the resource identifier or resource file path such as icon name.

typedef struct _CFW_PBK_CAPACITY_INFO
{
    uint16_t nTotalNum;
    uint8_t nPhoneNumberLen;
    uint8_t nTextLen;
    uint16_t nUsedNum;
    uint8_t nStorageId;
    uint8_t padding;
} CFW_PBK_CAPACITY_INFO;

typedef struct _CFW_PBK_CALLLOG_ENTRY
{
    uint8_t *pNumber;
    uint8_t nNumberSize;
    uint8_t nNumberType;
    uint16_t nPosition;
    uint8_t nTimes;
    uint8_t nStorageId;
    uint16_t nDuration;
    uint32_t nStartTime;
} CFW_PBK_CALLLOG_ENTRY;

#ifdef CFW_PBK_SYNC_VER

// Synchronous Function for PBK.
typedef struct _CFW_PBK_OUT_PARAM
{
    uint8_t nType;
    uint8_t padding[3];
    uint32_t nParam1;
    uint32_t nParam2;
} CFW_PBK_OUT_PARAM;

uint32_t CFW_PbkAddEntry(
    CFW_PBK_ENTRY_HRD *pEntryHrd,
    CFW_PBK_OUT_PARAM *pOutParam);

uint32_t CFW_PbkUpdateEntry(
    CFW_PBK_ENTRY_HRD *pEntryHrd,
    CFW_PBK_OUT_PARAM *pOutParam);

uint32_t CFW_PbkDeleteEntry(
    uint8_t nStorageId,
    uint16_t nPosition,
    CFW_PBK_OUT_PARAM *pOutParam);

uint32_t CFW_PbkDeleteBatchEntries(
    uint16_t nStartPos,
    uint16_t nCount,
    uint8_t nStorageId,
    CFW_PBK_OUT_PARAM *pOutParam);

uint32_t CFW_PbkFindEntry(
    uint8_t *pString,
    uint8_t nStringSize,
    uint16_t nStartPos,
    uint16_t nCount,
    uint8_t nStorageId,
    uint16_t nGroup,
    uint16_t nOption,
    CFW_PBK_OUT_PARAM *pOutParam);

uint32_t CFW_PbkListEntries(
    uint16_t nStartPos,
    uint16_t nPageCount,
    uint16_t nSkipPage,
    uint8_t nStorageId,
    uint16_t nGroup,
    uint8_t nOption,
    CFW_PBK_OUT_PARAM *pOutParam);

uint32_t CFW_PbkGetEntry(
    uint16_t nPosition,
    uint8_t nStorageId,
    uint8_t nOption,
    CFW_PBK_OUT_PARAM *pOutParam);

uint32_t CFW_PbkCopyEntries(
    uint16_t nStartPos,
    uint16_t nCount,
    uint8_t nOption,
    uint16_t nUTI);

uint32_t CFW_PbkAddEntryItem(
    CFW_PBK_ENTRY_ITEM *pEntryItem,
    uint8_t nCount,
    CFW_PBK_OUT_PARAM *pOutParam);

uint32_t CFW_PbkGetEntryItems(
    uint16_t nPosition,
    uint32_t nItemGroupType,
    CFW_PBK_OUT_PARAM *pOutParam);

uint32_t CFW_PbkDeleteEntryItem(
    uint16_t nItemId,
    /*uint32_t nItemGroupType,*/
    CFW_PBK_OUT_PARAM *pOutParam);

uint32_t CFW_PbkGetCapacityInfo(
    uint8_t nStorageId,
    CFW_PBK_CAPACITY_INFO *pCapInfo);

uint32_t CFW_PbkGetCalllogCapacityInfo(
    uint8_t nStorageId,
    CFW_PBK_CAPACITY_INFO *pCapInfo);

uint32_t CFW_PbkAddCallLogEntry(
    CFW_PBK_CALLLOG_ENTRY *pEntryInfo,
    CFW_PBK_OUT_PARAM *pOutParam);

uint32_t CFW_PbkGetCallLogEntry(
    uint16_t nPosition,
    uint8_t nStorageId,
    CFW_PBK_OUT_PARAM *pOutParam);

uint32_t CFW_PbkListCallLogEntries(
    uint16_t nStartPosition,
    uint16_t nCount,
    uint8_t nStorage,
    CFW_PBK_OUT_PARAM *pOutParam);

uint32_t CFW_PbkDeleteBatchCallLogEntries(
    uint16_t nStartPos,
    uint16_t nCount,
    uint8_t nStorageId,
    CFW_PBK_OUT_PARAM *pOutParam);

uint32_t CFW_PbkGetCalllogCapacityInfo(
    uint8_t nStorageId,
    CFW_PBK_CAPACITY_INFO *pCapInfo);

#else

uint32_t CFW_PbkAddEntry(
    CFW_PBK_ENTRY_HRD *pEntryHrd,
    uint16_t nUTI);

uint32_t CFW_PbkUpdateEntry(
    CFW_PBK_ENTRY_HRD *pEntryHrd,
    uint16_t nUTI);

uint32_t CFW_PbkDeleteEntry(
    uint8_t nStorageId,
    uint16_t nPosition,
    uint16_t nUTI);

uint32_t CFW_PbkDeleteBatchEntries(
    uint16_t nStartPos,
    uint16_t nCount,
    uint8_t nStorageId,
    uint16_t nUTI);

uint32_t CFW_PbkFindEntry(
    uint8_t *pString,
    uint8_t nStringSize,
    uint16_t nStartPos,
    uint16_t nCount,
    uint8_t nStorageId,
    uint16_t nGroup,
    uint16_t nOption,
    uint16_t nUTI);

uint32_t CFW_PbkListEntries(
    uint16_t nStartPos,
    uint16_t nPageCount,
    uint16_t nSkipPage,
    uint8_t nStorageId,
    uint16_t nGroup,
    uint8_t nOption,
    uint16_t nUTI);

uint32_t CFW_PbkGetEntry(
    uint16_t nPosition,
    uint8_t nStorageId,
    uint8_t nOption,
    uint16_t nUTI);

uint32_t CFW_PbkCopyEntries(
    uint16_t nStartPos,
    uint16_t nCount,
    uint8_t nOption,
    uint16_t nUTI);

uint32_t CFW_PbkAddEntryItem(
    CFW_PBK_ENTRY_ITEM *pEntryItem,
    uint8_t nCount,
    uint16_t nUTI);

uint32_t CFW_PbkGetEntryItems(
    uint16_t nPosition,
    uint32_t nItemGroupType,
    uint16_t nUTI);

uint32_t CFW_PbkDeleteEntryItem(
    uint16_t nItemId,
    uint16_t nUTI);

uint32_t CFW_PbkGetCapacityInfo(
    uint8_t nStorageId,
    CFW_PBK_CAPACITY_INFO *pCapInfo);

uint32_t CFW_PbkGetCalllogCapacityInfo(
    uint8_t nStorageId,
    CFW_PBK_CAPACITY_INFO *pCapInfo);

uint32_t CFW_PbkAddCallLogEntry(
    CFW_PBK_CALLLOG_ENTRY *pEntryInfo,
    uint16_t nUTI);

uint32_t CFW_PbkGetCallLogEntry(
    uint16_t nPosition,
    uint16_t nUTI);

uint32_t CFW_PbkListCallLogEntries(
    uint16_t nStartPosition,
    uint16_t nCount,
    uint8_t nStorage,
    uint16_t nUTI);

uint32_t CFW_PbkDeleteBatchCallLogEntries(
    uint16_t nStartPos,
    uint16_t nCount,
    uint8_t nStorageId,
    uint16_t nUTI);

uint32_t CFW_PbkGetCalllogCapacityInfo(
    uint8_t nStorageId,
    CFW_PBK_CAPACITY_INFO *pCapInfo);

uint32_t CFW_PbkCancel(
    uint16_t nOption,
    uint16_t nUTI);

#endif

//
// GPRS
//
#define CFW_GPRS_DETACHED 0 ///< detached
#define CFW_GPRS_ATTACHED 1 ///< attached

#define CFW_GPRS_DEACTIVED 0 ///< deactivated
#define CFW_GPRS_ACTIVED 1   ///< activated

// error define
#define ERR_CFW_GPRS_INVALID_CID 0
#define ERR_CFW_GPRS_HAVE_ACTIVED 1
#define ERR_CFW_GPRS_HAVE_DEACTIVED 2 //add wuys 2008-06-03

// state self defined
#define CFW_GPRS_STATE_IDLE 0
#define CFW_GPRS_STATE_CONNECTING 2
#define CFW_GPRS_STATE_DISCONNECTING 3
#define CFW_GPRS_STATE_ACTIVE 4
#define CFW_GPRS_STATE_MODIFYING 5
#define CFW_GPRS_STATE_EPS_ALLOCATING 6
#define CFW_GPRS_STATE_ERROR 7

// Packet Data Protocol type
#define CFW_GPRS_PDP_TYPE_X25 0    ///<  x.25
#define CFW_GPRS_PDP_TYPE_IP 1     ///<  IP
#define CFW_GPRS_PDP_TYPE_IPV6 2   ///<  IP Version 6
#define CFW_GPRS_PDP_TYPE_IPV4V6 3 ///< DualStack
#define CFW_GPRS_PDP_TYPE_UNUSED 4 ///< according to TS24.301 R14 9.9.4.10, shall be interpreted as "IPV6" if received by the network, here reserved
#define CFW_GPRS_PDP_TYPE_OSPIH 1  ///<  internet Hosted Octect Stream Protocol
#define CFW_GPRS_PDP_TYPE_PPP 6    ///<  Point to Point Protocol
/*added by zuohuaxu 20161115, begin*/
#if (defined LTE_NBIOT_SUPPORT) || (defined LTE_SUPPORT)
#define CFW_GPRS_PDP_TYPE_NONIP 5 /*value set according to TS24.301 R14 9.9.4.10*/
#endif
/*added by zuohuaxu 20161115, end*/
// PDP data compression
#define CFW_GPRS_PDP_D_COMP_OFF 0 ///<  off (default if value is omitted)
#define CFW_GPRS_PDP_D_COMP_ON 1  ///<  on(manufacturer preferred compression)
#define CFW_GPRS_PDP_D_COMP_V42 2 ///<  V.42bis
#define CFW_GPRS_PDP_D_COMP_V44 3 ///<  V.44

// PDP header compression
#define CFW_GPRS_PDP_H_COMP_OFF 0     ///<  off (default if value is omitted)
#define CFW_GPRS_PDP_H_COMP_ON 1      ///<  on (manufacturer preferred compression)
#define CFW_GPRS_PDP_H_COMP_RFC1144 2 ///<  RFC1144 (applicable for SNDCP only)
#define CFW_GPRS_PDP_H_COMP_RFC2507 3 ///<  RFC2507
#define CFW_GPRS_PDP_H_COMP_RFC3095 4 ///<  RFC3095 (applicable for PDCP only)

//L2P
#define CFW_GPRS_L2P_NULL 0 ///<  NULL (Obsolete)
#define CFW_GPRS_L2P_PPP 1  ///<  PPP
#define CFW_GPRS_L2P_PAD 2  ///<  PAD (Obsolete)
#define CFW_GPRS_L2P_X25 3  ///<  x25 (Obsolete)

// Automatic response to a network request for PDP context activation
#define CFW_GPRS_AUTO_RESPONSE_PACKET_DOMAIN_OFF 0 ///<  turn off automatic response for Packet Domain only
#define CFW_GPRS_AUTO_RESPONSE_PACKET_DOMAIN_ON 1  ///<  turn on automatic response for Packet Domain only
#define CFW_GPRS_AUTO_RESPONSE_PS_ONLY 2           ///<  modem compatibility mode, Packet Domain only
#define CFW_GPRS_AUTO_RESPONSE_PS_CS 3             ///<  modem compatibility mode, Packet Domain and circuit switched calls (default)

#define THE_APN_MAX_LEN 99
#define THE_PDP_ADDR_MAX_LEN 21
#define THE_APN_USER_MAX_LEN 64
#define THE_APN_PWD_MAX_LEN 64
#define THE_PDP_DNS_MAX_LEN 21 ///< 0-3: IPV4  4-19:IPV6
#define THE_PDP_DNS_NUM 2

#if defined(CFW_GPRS_SUPPORT) || defined(DOXYGEN)

typedef struct _CFW_GPRS_PDPCONT_INFO
{
    uint8_t nPdpType;
    uint8_t nDComp;
    uint8_t nHComp;
    uint8_t nApnSize;
    uint8_t nIpType;
//#ifdef CFW_VOLTE_SUPPORT //quectel modify
    uint8_t nPcscfType[MAX_PCSCF_ADDR_NUMBER];
    uint8_t nPcscfNumber;
    uint8_t PcscfAddr[16 * MAX_PCSCF_ADDR_NUMBER];
    uint8_t nIsEmergency;
//#endif //quectel modify
    uint8_t nDnsType;
    uint8_t nPdpDnsSize;
    uint8_t *pPdpDns;
    uint8_t nPdpAddrSize;
    uint8_t nApnUserSize;
    uint8_t nApnPwdSize;
    uint8_t padding[1];
    uint8_t *pApnUser;
    uint8_t *pApnPwd;
    uint8_t *pApn;
    uint8_t *pPdpAddr;
    uint8_t nNSLPI;
#if (defined LTE_NBIOT_SUPPORT) || (defined LTE_SUPPORT)
    uint8_t PdnType; ///<  IPV4/V6/Non-IP
#endif
    uint8_t nAuthProt;
} CFW_GPRS_PDPCONT_INFO;

typedef struct
{
    uint8_t nPdpType;
    uint8_t nDComp;
    uint8_t nHComp;
    uint8_t nApnSize;
    uint8_t nIpType;
    uint8_t nPcscfType[MAX_PCSCF_ADDR_NUMBER];
    uint8_t nPcscfNumber;
    uint8_t PcscfAddr[16 * MAX_PCSCF_ADDR_NUMBER];
    uint8_t nIsEmergency;
    uint8_t nDnsType;
    uint8_t nPdpDnsSize;
    uint8_t pPdpDns[THE_PDP_DNS_MAX_LEN * THE_PDP_DNS_NUM];
    uint8_t nPdpAddrSize;
    uint8_t nApnUserSize;
    uint8_t nApnPwdSize;
    uint8_t pApnUser[THE_APN_USER_MAX_LEN];
    uint8_t pApnPwd[THE_APN_PWD_MAX_LEN];
    uint8_t pApn[THE_APN_MAX_LEN];
    uint8_t pPdpAddr[THE_PDP_ADDR_MAX_LEN];
    uint8_t nNSLPI;
    uint8_t PdnType; ///<  IPV4/V6/Non-IP
    uint8_t nAuthProt;
} CFW_GPRS_PDPCONT_INFO_V2;

typedef struct
{
    uint8_t nApnSize;
    uint8_t pApn[THE_APN_MAX_LEN];
} CFW_GPRS_DYNAMIC_APN_INFO;

typedef struct
{
    uint8_t nPdpAddrSize;
    uint8_t pPdpAddr[THE_PDP_ADDR_MAX_LEN];
} CFW_GPRS_DYNAMIC_IP_INFO;

typedef struct
{
    uint8_t nPdpType;
    uint8_t nAuthProt;
    uint8_t nApnSize;
    uint8_t nApnUserSize;
    uint8_t nApnPwdSize;
    uint8_t pApnUser[THE_APN_USER_MAX_LEN];
    uint8_t pApnPwd[THE_APN_PWD_MAX_LEN];
    uint8_t pApn[THE_APN_MAX_LEN];
} CFW_GPRS_DFTPDN_INFO;

typedef struct
{
    uint8_t Cid;
    uint8_t nPdpType;
    uint8_t PDPAddLen;
    uint8_t PDPAddress[THE_PDP_ADDR_MAX_LEN];
    uint8_t APNLen;
    uint8_t AccessPointName[THE_APN_MAX_LEN];
} CFW_GPRS_PDPCONT_DFTPDN_INFO;

typedef struct _CFW_GPRS_QOS
{
    uint8_t nPrecedence;
    uint8_t nDelay;
    uint8_t nReliability;
    uint8_t nPeak;
    uint8_t nMean;
    uint8_t padding[3];
#if (defined LTE_NBIOT_SUPPORT) || (defined LTE_SUPPORT)
    uint16_t nTrafficClass;
    uint16_t nMaximumbitrateUL;
    uint16_t nMaximumbitrateDL;
    uint16_t nGuaranteedbitrateUL;
    uint16_t nGuaranteedbitrateDL;
    uint16_t nDeliveryOrder;
    uint16_t nMaximumSDUSize;
    uint16_t nDeliveryOfErroneousSDU;
    uint16_t nTransferDelay;
    uint16_t nTrafficHandlingPriority;
    uint16_t nSDUErrorRatio;
    uint16_t nResidualBER;
#endif
} CFW_GPRS_QOS;

struct CFW_SAT_CHANNEL
{
    uint8_t nStatus[2];
    uint16_t nBufferSize;
    uint8_t nBearerType;
    CFW_GPRS_QOS Qos;
    uint8_t nPacketType;
};
typedef struct _CFW_EQOS
{
    uint8_t nQci;
    uint32_t nDlGbr;
    uint32_t nUlGbr;
    uint32_t nDlMbr;
    uint32_t nUlMbr;
} CFW_EQOS;

typedef struct _CFW_TFT
{
    uint8_t uLength;
    uint8_t uTFT[255]; ///< 257 - 3 should be OK.
} CFW_TFT;

typedef struct _CFW_TFT_SET
{
    uint16_t Pf_BitMap;            /*Pos: 0, 1, 2, 3, ...  */
    uint8_t Pf_ID;                 /*Bit 0 - 0x8000  */
    uint8_t Pf_EvaPreIdx;          /*Bit 1 - 0x4000 */
    uint8_t Pf_RmtAddrIPv4[8];     /*Bit 2 - 0x2000 */
    uint8_t Pf_RmtAddrIPv6[32];    /*Bit 3 - 0x1000 */
    uint8_t Pf_PortNum_NH;         /*Bit 4 - 0x0800 */
    uint16_t Pf_LocalPortRangeMin; /*Bit 5 - 0x0400 */
    uint16_t Pf_LocalPortRangeMax;
    uint16_t Pf_RemotPortRangeMin; /*Bit 6 - 0x0200 */
    uint16_t Pf_RemotPortRangeMax;
    uint32_t Pf_SPI;              /*Bit 7 - 0x0100 */
    uint8_t Pf_TOS_TC[2];         /*Bit 8 - 0x0080 */
    uint32_t Pf_FlowLabel;        /*Bit 9 - 0x0040 */
    uint8_t Pf_Direct;            /*Bit 10 - 0x0020 */
    uint8_t Pf_LocalAddrIPv4[8];  /*Bit 11 - 0x0010 */
    uint8_t Pf_LocalAddrIPv6[32]; /*Bit 12 - 0x0008 */

    struct _CFW_TFT_SET *pNext;
} CFW_TFT_SET;

/* Packet Fileter component */
typedef struct _CFW_PF_CPNT
{
    /*  IPv4 remote address type:               8 octet (a four octet IPv4 address field and a four octet IPv4 address mask field)
    IPv4 local address type:                8 octet (a four octet IPv4 address field and a four octet IPv4 address mask field)
    IPv6 remote address type:               32 octet (a sixteen octet IPv6 address field and a sixteen octet IPv6 address mask field)
    IPv6 remote address/prefix length type: 17 octet(a sixteen octet IPv6 address field and one octet prefix length field)
    IPv6 local address/prefix length type:  17 octet(a sixteen octet IPv6 address field and one octet prefix length field)
    Protocol identifier/Next header type:   one octet
    Single local port type:                 two octet
    Local port range type:                  4 octet (a two octet port range low limit field and a two octet port range high limit field)
    Single remote port type:                two octet
    Remote port range type:                 4 octet (a two octet port range low limit field and a two octet port range high limit field)
    Security parameter index type:          four octet
    Type of service/Traffic class type:     2 octet (a one octet Type-of-Service/Traffic Class field and a one octet Type-of-Service/Traffic Class mask field)
    Flow label type:                        three octet
*/
    uint8_t uCpntType;     /* Packet filter component type identifier */
    uint8_t uCpntLen;      /* Packet filter component Length */
    uint8_t uCpntCont[32]; /* Packet filter component Content */

    struct _CFW_PF_CPNT *pNext;
} CFW_PF_CPNT;

/* One Packet Fileter */
typedef struct _CFW_TFT_PF
{
    uint8_t uEBI;
    uint8_t uPF_Dir; /* Packet filter direction */
    uint8_t uPF_ID;  /* Packet filter identifier */
    uint8_t uPF_Pri; /* Packet filter evaluation precedence */

    CFW_PF_CPNT *Cpnt; /* Packet filter Component */

    /* Next Component */
    struct _CFW_TFT_PF *pNext;
} CFW_TFT_PF;

typedef struct
{
    uint8_t Ebi;
    uint8_t LinkEbi;
    uint8_t EbiDeact[7];
    uint8_t EbiDeactNum;
    uint8_t TftOpCode;
    CFW_TFT_PF *pTftPf;
} CFW_TFT_PARSE;

typedef struct CFW_DATA_ATTRIBUTE_Tag
{
    uint8_t rai;
    uint8_t exception_data;
#define CFW_PSDATAOFF_EXEMPTED_SERVICE_TYPE_NORMAL 0x00
#define CFW_PSDATAOFF_EXEMPTED_SERVICE_TYPE_DM 0x01
#define CFW_PSDATAOFF_EXEMPTED_SERVICE_TYPE_BIP 0x02
#define CFW_PSDATAOFF_EXEMPTED_SERVICE_TYPE_RESERVED 0x04
    uint16_t exempted_service_type;
} CFW_DATA_ATTRIBUTE_T;

typedef struct _CFW_GPRS_DATA
{
    uint16_t nDataLength;
#if (defined LTE_NBIOT_SUPPORT) || (defined LTE_SUPPORT)
    CFW_DATA_ATTRIBUTE_T attribute;
#endif
    uint8_t padding;
    uint8_t pData[1];
} CFW_GPRS_DATA;
//synchronization function
// AT+CGDCONT
uint32_t CFW_GprsSetPdpCxt(
    uint8_t nCid,
    CFW_GPRS_PDPCONT_INFO *pPdpCont,
    CFW_SIM_ID nSimID); ///<  [in]nCid, [in]pPdpCont

uint32_t CFW_GprsGetPdpCxt(
    uint8_t nCid,
    CFW_GPRS_PDPCONT_INFO *pPdpCont,
    CFW_SIM_ID nSimID); ///<  [in]nCid, [out]pPdpCont

uint32_t CFW_GprsSetPdpCxtV2(
    uint8_t nCid,
    const CFW_GPRS_PDPCONT_INFO_V2 *pPdpCont,
    CFW_SIM_ID nSimID); ///<  [in]nCid, [in]pPdpCont

uint32_t CFW_GprsGetPdpCxtV2(
    uint8_t nCid,
    CFW_GPRS_PDPCONT_INFO_V2 *pPdpCont,
    CFW_SIM_ID nSimID); ///<  [in]nCid, [out]pPdpCont

void CFW_GprsSetDefaultPdnMode(uint32_t mode, CFW_SIM_ID nSimID);

uint32_t CFW_GprsGetDefaultPdnMode(CFW_SIM_ID nSimID);

uint32_t CFW_GprsSetDynamicAPN(uint8_t nCid, const CFW_GPRS_DYNAMIC_APN_INFO *pDyncApn, CFW_SIM_ID nSimID);

uint32_t CFW_GprsRemovePdpCxt(
    uint8_t nCid,
    CFW_SIM_ID nSimID);

uint8_t CFW_GprsGetDefaultPdnInfo(CFW_GPRS_PDPCONT_DFTPDN_INFO *PdnInfo, CFW_SIM_ID nSimID);
uint32_t CFW_GprsSendCtxCfg_V2(CFW_GPRS_DFTPDN_INFO *pDftPdn, CFW_SIM_ID nSimID);
uint32_t CFW_GprsGetDynamicAPN(uint8_t nCid, CFW_GPRS_DYNAMIC_APN_INFO *pDyncApn, CFW_SIM_ID nSimID);
uint32_t CFW_GprsGetDynamicIP(uint8_t nCid, CFW_GPRS_DYNAMIC_IP_INFO *pDyncIP, CFW_SIM_ID nSimID);

typedef enum
{
    PDP_CTX_TYPE_XCAP,
    PDP_CTX_TYPE_IMS,
    PDP_CTX_TYPE_IA,
    PDP_CTX_TYPE_NUM,
    PDP_CTX_TYPE_INVALID = 0xFFFFFFFF
} CFW_INTERNAL_PDP_CTX_TYPE;

typedef struct
{
    uint8_t nFlag;
    CFW_GPRS_PDPCONT_INFO_V2 PdpCtx;
} CFW_INTERNAL_PDP_CTX;

uint32_t CFW_GprsSetInternalPdpCtx(
    CFW_INTERNAL_PDP_CTX_TYPE nType,
    const CFW_GPRS_PDPCONT_INFO_V2 *pPdpCont,
    CFW_SIM_ID nSimID);

CFW_GPRS_PDPCONT_INFO_V2 *CFW_GprsGetInternalPdpCtx(
    CFW_INTERNAL_PDP_CTX_TYPE nType,
    CFW_SIM_ID nSimID);

// AT+CGQMIN
// This command allows the TE to specify a Min Quality of Service Profile
// that is used when the MT sends an Activate PDP Context Request message
// to the network.
uint32_t CFW_GprsSetMinQos(uint8_t nCid, CFW_GPRS_QOS *pQos, CFW_SIM_ID nSimID);

uint32_t CFW_GprsGetMInQos(uint8_t nCid, CFW_GPRS_QOS *pQos, CFW_SIM_ID nSimID);

// AT+CGQREQ
// This command allows the TE to specify a Quality of Service Profile
// that is used when the MT sends an Activate PDP Context Request message
// to the network.
uint32_t CFW_GprsSetReqQos(
    uint8_t nCid,
    CFW_GPRS_QOS *pQos,
    CFW_SIM_ID nSimID); ///<  [in]nCid, [in]pQos

uint32_t CFW_GprsGetReqQos(
    uint8_t nCid,
    CFW_GPRS_QOS *pQos,
    CFW_SIM_ID nSimID); ///<  [in]nCid, [out]pQos

// AT+CGEQMIN
//This command allows the TE to specify a minimum acceptable profile, which is
//checked by the MT against the negotiated profile returned in the PDP context
//establishment and PDP contect modification procedures.
uint32_t CFW_GprsSetReqQosUmts(
    uint8_t nCid,
    CFW_GPRS_QOS *pQos,
    CFW_SIM_ID nSimID); // [in]nCid, [in]pQos

uint32_t CFW_GprsGetReqQosUmts(
    uint8_t nCid,
    CFW_GPRS_QOS *pQos,
    CFW_SIM_ID nSimID); // [in]nCid, [out]pQos

//AT+CGPADDR
//get the address of the selected Pdp context of the gprs network
//[in]nCid: [1-7]
// Specify the PDP Context Identifier (a numeric) which specifies a particular PDP context definition.
//[in/out]nLength: [in] nLength should be the length that malloc the pdpAdd.
//                 [out] nLength should be the length of the Pdp address.
//[out]pPdpAdd: Point to the buffer to hold the PDP address.
// return: If the function succeeds, the return value is ERR_SUCCESS,
// if the function fails, the following error code may be returned.
// ERR_CFW_INVALID_PARAMETER, ERR_CME_UNKNOWN,

uint32_t CFW_GprsGetPdpAddr(
    uint8_t nCid,
    uint8_t *nLength,
    uint8_t pPdpAdd[THE_PDP_ADDR_MAX_LEN],
    CFW_SIM_ID nSimID);

// using net parameter
//AT+CGREG
// get the status of the gprs network.
// [out]pStatus: Pointer to CFW_NW_STATUS_INFO structure to contain the network status information
// return: If the function succeeds, the return value is ERR_SUCCESS,
// if the function fails, the following error code may be returned.
// ERR_CFW_INVALID_PARAMETER, ERR_CME_UNKNOWN,
uint32_t CFW_GprsGetstatus(
    CFW_NW_STATUS_INFO *pStatus,
    CFW_SIM_ID nSimID); //

uint8_t CFW_GprsGetPsType(uint8_t nSimID);

/*! \brief retrieves the state of the GPRS service
\param [out] pState    Point to uint8_t to hold the state of GPRS attachment.
                - \a CFW_GPRS_DEACTIVED (0)
                - \a CFW_GPRS_ACTIVED (1)
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_GetGprsAttState(
    uint8_t *pState,
    CFW_SIM_ID nSimID);

/*! \brief retrieves the state of the GPRS Service according to the selected cid
\param [in] nCid    Specify the PDP Context Identifier (a numeric) which specifies
                a particular PDP context definition.
\param [out] pState    Point to uint8_t to hold the state of GPRS activation.
                - \a CFW_GPRS_DEACTIVED (0)
                - \a CFW_GPRS_ACTIVED (1)
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_GetGprsActState(
    uint8_t nCid,
    uint8_t *pState,
    CFW_SIM_ID nSimID);

/*! \brief activate or deactivate the specified PDP context.

The result should be returned through the following \a EV_CFW_GPRS_ACT_RSP event:

\rst
+------+--------------------+------------+---------+-------------+
| UTI  |       nType        |  nParam1   | nParam2 | Description |
+======+====================+============+=========+=============+
| nUTI | CFW_GPRS_DEACTIVED | 0          | 0       | OK          |
|      | CFW_GPRS_ACTIVED   |            |         |             |
+------+--------------------+------------+---------+-------------+
| nUTI | 0xF0/0xF1          | Error code | nCid    | Error       |
+------+--------------------+------------+---------+-------------+
\endrst

\param [in] nState  Specify the state of PDP context activation
                - \a CFW_GPRS_DEACTIVED (0)
                - \a CFW_GPRS_ACTIVED (1)
\param [in] nCid    Specify the PDP Context Identifier (a numeric) which specifies
                a particular PDP context definition.
\param [in] nUTI    UTI operation
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_GprsAct(uint8_t nState, uint8_t nCid, uint16_t nUTI, CFW_SIM_ID nSimID);

uint32_t CFW_GprsActEx(
    uint8_t nState,
    uint8_t nCid,
    uint16_t nUTI,
    CFW_SIM_ID nSimID,
    bool SavedInApp); ///<  [in] nCid  // notify: success or fail

uint32_t CFW_AttDetach(
    uint8_t nState,
    uint16_t nUTI,
    uint8_t AttDetachType,
    CFW_SIM_ID nSimID);

void CFW_SetPsDataAttribute(CFW_DATA_ATTRIBUTE_T *pAttribute, uint8_t rai, uint8_t exception_data, uint16_t exempted_service_type);

uint32_t CFW_SetDnsServerbyPdp(uint8_t nCid, uint8_t nSimID, uint8_t Pro_DnsIp[4], uint8_t Sec_DnsIp[4]);
uint32_t CFW_SetDnsServerbyPdpV2(uint8_t nCid, uint8_t nSimID, uint8_t Pro_DnsIp[16], uint8_t Sec_DnsIp[16]);
uint32_t CFW_getDnsServerbyPdp(uint8_t nCid, uint8_t nSimID);
uint32_t *getDNSServer(uint8_t nCid, uint8_t nSimID);
uint8_t CFW_ImsGetCevdp(uint8_t nSimID);
#ifdef DEDICATED_BEARER_SUPPORT
uint32_t CFW_GprsSecPdpAct(uint8_t nState, uint8_t nCid, uint8_t nPCid, uint16_t nUTI, CFW_SIM_ID nSimID);
#endif
uint32_t CFW_GprsSetPsm(uint8_t psmEnable, uint8_t bitmap, uint8_t t3324, uint8_t t3412, uint8_t nSimID);
uint32_t CFW_GprsSetCtxTFT(uint8_t nCid, CFW_TFT_SET *pTft, uint8_t nSimID);
uint32_t CFW_GprsSetCtxEQos(uint8_t nCid, CFW_EQOS *pQos, uint8_t nSimID);
uint32_t CFW_GprsGetCtxEQos(uint8_t nCid, CFW_EQOS *pQos, CFW_SIM_ID nSimID);
uint32_t CFW_GprsCtxEpsModify(uint16_t nUTI, uint8_t nCid, CFW_SIM_ID nSimID);
uint32_t CFW_GprsGetNetEQos(uint8_t nCid, CFW_EQOS *pQos, CFW_SIM_ID nSimID);

/*! \Manual acceptance of a network request for PDP context activation 'A'

The result should be returned through the \a EV_CFW_GPRS_CXT_ACTIVE_IND_NOTIFY event:

\rst
+------+-----------+------------+---------+-------------+
| UTI  |   nType   |  nParam1   | nParam2 | Description |
+======+===========+============+=========+=============+
| nUTI | 0         | 0          | 0       | OK          |
+------+-----------+------------+---------+-------------+
| nUTI | 0xF0/0xF1 | Error code | 0       | Error       |
+------+-----------+------------+---------+-------------+
\endrst

\param [in] nUTI    Specify the UTI (Unique Transaction Identifier) operation, which support multi-application in parallel. This parameter will
be return in notification event (see below). You also can get a free UTI by CFW_GetFreeUTI function if you application is a dynamical
module. This parameter should be less than 255 for all upper layer application.
\param [in] nCid  Specify the PDP Context Identifier (a numeric) which specifies a particular PDP context definition.
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_GprsManualAcc(
    uint16_t nUTI,
    uint8_t nCid,
    CFW_SIM_ID nSimID);

/*! \Manual rejection of a network request for PDP context activation 'H'
The function is used to reject the request of a network for GPRS PDP context activation
which has been signaled to the TE by the RING or CRING unsolicited result code.
\param [in] nUTI    UTI operation
\param [in] nCid  a numeric parameter which specifies a particular PDP context definition
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_GprsManualRej(
    uint16_t nUTI,
    uint8_t nCid,
    CFW_SIM_ID nSimID);

/*! \The function is used to modify the specified PDP context with QoS
parameter. After this function has completed, the MT returns to V.25ter online data
state. If the requested modification for any specified context cannot be
achieved, an event with ERROR information is returned.
\param [in] nCid  a numeric parameter which specifies a particular PDP context definition
\param [in] nUTI    UTI operation
\param [in] QOS    Point to CFW_GPRS_QOS structure to set the Quality of Service
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_GprsCtxModify(
    uint16_t nUTI,
    uint8_t nCid,
    CFW_GPRS_QOS *Qos,
    CFW_SIM_ID nSimID);

/*! \This message is sent by MMI to accept the PDP context modification requested by
the network in an api_PdpCtxModifyInd message.

\param [in] nUTI    UTI operation
\param [in] nCid  a numeric parameter which specifies a particular PDP context definition
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_GprsCtxModifyAcc(
    uint16_t nUTI,
    uint8_t nCid,
    CFW_SIM_ID nSimID);

// return: If the function succeeds, the return value is ERR_SUCCESS,
// if the function fails, the following error code may be returned.
// ERR_CFW_INVALID_PARAMETER, ERR_CME_UNKNOWN,
// event: NO
uint32_t CFW_GprsCtxModifyRej(
    uint16_t nUTI,
    uint8_t nCid,
    CFW_SIM_ID nSimID);

/*! \The function is used to send PDP data to the network. The implementation
of this function is always called by upper protocol

\param [in] nCid  a numeric parameter which specifies a particular PDP context definition
\param [in] pGprsData    point to the data that need to send
\param [in] rai  the value of the release assistance indication, refer 3GPP TS 24.301 [83] subclause 9.9.4.25.
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_GprsSendData(
    uint8_t nCid,
    CFW_GPRS_DATA *pGprsData,
    uint8_t rai,
    CFW_SIM_ID nSimID);

/*! \The function is used to send PDP data length to the network.

\param [in] nSimID  SIM ID
\param [in] nCid  a numeric parameter which specifies a particular PDP context definition
\param [in] nDataLen  the value of PDP data length to the network
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_GprsSendDataV2(uint8_t nSim, uint8_t nCid, uint16_t nDataLen);

uint32_t CFW_PsSendData(
    uint8_t nCid,
    uint32_t pDataOffset,
    CFW_SIM_ID nSimID);

/*! \brief attach or dettach from GPRS service

The result should be returned through the \a EV_CFW_GPRS_ATT_RSP event:

\rst
+------+-------------------+------------+---------+-------------+
| UTI  |       nType       |  nParam1   | nParam2 | Description |
+======+===================+============+=========+=============+
| nUTI | CFW_GPRS_ATTACHED | 0          | 0       | OK          |
|      | CFW_GPRS_DETACHED |            |         |             |
+------+-------------------+------------+---------+-------------+
| nUTI | 0xF0/0xF1         | Error code | 0       | Error       |
+------+-------------------+------------+---------+-------------+
\endrst

\param [in] nState  Specify the state of GPRS attachment. It can be
                - \a CFW_GPRS_DETACHED (0) dettach
                - \a CFW_GPRS_ATTACHED (1) attach
\param [in] nUTI    UTI operation
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_GprsAtt(uint8_t nState, uint16_t nUTI, CFW_SIM_ID nSimID);

uint32_t CFW_GetGprsSum(int32_t *upsum, int32_t *downsum, CFW_SIM_ID nSimID);
uint32_t CFW_ClearGprsSum(CFW_SIM_ID nSimID);

uint32_t CFW_ImsSsUtAddrSet(uint8_t nCid, uint8_t nSize, uint8_t *paddr, CFW_SIM_ID nSimID);

uint8_t CFW_GprsSetSmsSeviceMode(uint8_t nService);
uint8_t CFW_GprsGetSmsSeviceMode(void);

/*! \The function is used to check the sim card is test card or normal card.

\param [in] nSimID  SIM ID
\return \a 0 for normal card, or 1 for test card.
*/
uint32_t CFW_CheckTestSim(CFW_SIM_ID nSimID);

#endif
//
//Tcpip
//

//add for DNS
typedef enum e_resolv_result
{
    RESOLV_QUERY_INVALID,
    RESOLV_QUERY_QUEUED,
    RESOLV_COMPLETE,
    RESOLV_ENUM_FILL = 0x7fffffff
} RESOLV_RESULT;

#define RESOLV_NETWORK_ERROR 0x01
#define RESOLV_HOST_NO_FOUND 0x02

// Shell
//
uint32_t CFW_MeChangePassword(
    uint16_t nFac,
    uint8_t *pBufPwd,
    uint8_t nPwdSize,
    uint8_t nMode,
    uint16_t nUTI);

uint32_t CFW_MeGetFacilityLock(
    uint16_t nFac,
    uint16_t nUTI);

uint32_t CFW_MeSetFacilityLock(
    uint16_t nFac,
    uint8_t *pBufPwd,
    uint8_t nPwdSize,
    uint8_t nMode,
    uint16_t nUTI);

#ifdef CFW_GPRS_SUPPORT
//Errors related to a failure to perform an Attach
#define ERR_CFW_GPRS_ILLEGAL_MS 103                                ///< Illegal MS (#3)
#define ERR_CFW_GPRS_ILLEGAL_ME 106                                ///< Illegal ME (#6)
#define ERR_CFW_GPRS_GPRS_SERVICES_NOT_ALLOWED 107                 ///< GPRS services not allowed (#7)
#define ERR_CFW_GPRS_PLMN_NOT_ALLOWED 111                          ///< PLMN not allowed (#11)
#define ERR_CFW_GPRS_LOCATION_AREA_NOT_ALLOWED 112                 ///< Location area not allowed (#12)
#define ERR_CFW_GPRS_ROAMING_NOT_ALLOWED_IN_THIS_LOCATION_AREA 113 ///< Roaming not allowed in this location area (#13)

//Errors related to a failure to Activate a Context
#define ERR_CFW_GPRS_SERVICE_OPTION_NOT_SUPPORTED 132            ///< service option not supported (#32)
#define ERR_CFW_GPRS_REQUESTED_SERVICE_OPTION_NOT_SUBSCRIBED 133 ///<  requested service option not subscribed (#33)
#define ERR_CFW_GPRS_SERVICE_OPTION_TEMPORARILY_OUT_OF_ORDER 134 ///<  service option temporarily out of order (#34)
#define ERR_CFW_GPRS_PDP_AUTHENTICATION_FAILURE 149              ///<  PDP authentication failure
#define ERR_CFW_GPRS_UNSPECIFIED 148                             ///<  unspecified GPRS error
#endif
//
// Some functions need be supported by OS Kernerl(KL) based on the current target platform(MIPS).
//
typedef uint32_t (*PFN_KL_SENDMESSAGDE2APP)(uint32_t nEventId, void *pData, uint32_t nSize);
typedef uint32_t (*PFN_KL_SENDMESSAGDE_EX)(uint32_t nMailBox, uint32_t nEventId, void *pData, uint32_t nSize);
typedef void *(*PFN_KL_MALLOC)(uint32_t nSize);
typedef bool (*PFN_KL_FREE)(void *nAddr);
typedef bool (*PFN_KL_IS_VALIDHEAP)(void *pData, uint32_t nSize);
typedef bool (*PFN_AUD_START_SPK_MIC)(void);
typedef bool (*PFN_AUD_STOP_SPK_MIC)(void);

typedef struct _CFW_PFN_AUD_CBS
{
    PFN_AUD_START_SPK_MIC m_pfnAudStartSpkMic;
    PFN_AUD_STOP_SPK_MIC m_pfnAudStopSpkMic;
} CFW_PFN_AUD_CBS;

typedef struct _CFW_PFN_KL_CBS
{
    PFN_KL_SENDMESSAGDE2APP m_pfnKlSendMsg2App;
    PFN_KL_SENDMESSAGDE_EX m_pfnKlSendMsgEx;
    PFN_KL_MALLOC m_pfnKlMalloc;
    PFN_KL_FREE m_pfnKlFree;
    PFN_KL_IS_VALIDHEAP m_pfnKlIsValidHeap;
} CFW_PFN_KL_CBS;

//
//add by lixp
//
typedef struct _CFW_SAT_MENU_RSP_LIST
{
    uint8_t nComID;
    uint8_t nComQualifier;
    uint16_t nAlphaLen;
    uint8_t *pAlphaStr;
    CFW_SAT_ITEM_LIST *pItemList; ///< Link list
} CFW_SAT_MENU_RSP_LIST, CFW_SAT_MENU_RSP;

#define CFW_PBK_HINT_MAX_SIZE 16

typedef struct _CFW_SMS_OUT_PARAM
{
    uint8_t nType;
    uint8_t padding[3];
    uint32_t nParam1;
    uint32_t nParam2;
} CFW_SMS_OUT_PARAM;
// CFW Init status phase.
#define CFW_INIT_STATUS_NO_SIM (0x01)
#define CFW_INIT_STATUS_SIM (0x02)
#define CFW_INIT_STATUS_SAT (0x04)
#define CFW_INIT_STATUS_SIMCARD (0x08)
#define CFW_INIT_STATUS_SMS (0x10)
#define CFW_INIT_STATUS_PBK (0x20)
#define CFW_INIT_STATUS_NET (0x40)
#define CFW_INIT_STATUS_AUTODAIL (0x80)
#define CFW_INIT_STATUS_NO_MEMORY (0x81)
#define CFW_INIT_STATUS_SIM_DROP (0x82)
#define CFW_INIT_STATUS_SIM_COMPLTED (0xF0)
//
// CFW Init status
//
#define CFW_INIT_SIM_CARD_BLOCK 0x4
#define CFW_INIT_SIM_WAIT_PIN1 0x8
#define CFW_INIT_SIM_WAIT_PUK1 0x10
#define CFW_INIT_SIM_PIN1_READY 0x20

#define CFW_INIT_SIM_WAIT_PS 0x40
#define CFW_INIT_SIM_WAIT_PF 0x80

#define CFW_INIT_SIM_CARD_BAD 0x100
#define CFW_INIT_SIM_CARD_ABNORMITY 0x200
#define CFW_INIT_SIM_TEST_CARD 0x400
#define CFW_INIT_SIM_NORMAL_CARD 0x800
#define CFW_INIT_SIM_CARD_CHANGED 0x1000
#define CFW_INIT_SIM_SAT 0x2000

//
// CFW Exit status
//
#define CFW_EXIT_STATUS_NETWORK_DETACHMENT (0x01)
#define CFW_EXIT_STATUS_SIM_POWER_OFF (0x02)
#define CFW_EXIT_STATUS_CFW_EXIT (0x10)

/************************************************ENGINERRING TEST MODE**************************************************/
//
//add  for emod at 20060819
//

typedef struct _CFW_EMOD_BATTERY_CHARGE_INFO
{
    uint8_t nBcs; ///< Point to the unsigned 8-bit variable  to retrieve the connection status of battery pack.
    //This parameter can be one of the following:
    //0 No charging adapter is connected
    //1 Charging adapter is connected
    //2 Charging adapter is connected, charging in progress
    //3 Charging adapter is connected, charging has finished
    //4 Charging error, charging is interrupted
    //5 False charging temperature, charging is interrupted while temperature is beyond allowed range
    uint8_t nBcl; //Point to the unsigned 8-bit variable to retrieve the battery level. This parameter can be
    //one of the following:
    //0 battery is exhausted, or does not have a battery connected
    // 20, 40, 60, 80, 100 percent of remaining capacity.
    //The percent is not accurate ,but a estimated expression.
    uint16_t nMpc; //Point to the unsigned 16-bit variable to retrieve the average power consumption.
    //This parameter can be one of the following value:
    //0-5000 Value (0...5000) of average power consumption (mean value over a couple of seconds) in mA.
} CFW_EMOD_BATTERY_CHARGE_INFO;

typedef struct _CFW_EMOD_BATTERY_INFO
{
    PM_BATTERY_INFO nBatState;
    CFW_EMOD_BATTERY_CHARGE_INFO nChargeInfo;
    uint8_t nChemistry; //
} CFW_EMOD_BATTERY_INFO;

bool CFW_EmodGetBatteryInfo(CFW_EMOD_BATTERY_INFO *pBatInfo);

void CFW_EmodAudioTestStart(uint8_t type); //type value: 0 is normal; 1 is mic AUX and speak AUX; 2 is mic loud and speak loud.

void CFW_EmodAudioTestEnd(void);

bool CFW_EmodClearUserInfo(void);                                              //return TRUE is success,and FALSE is failure.
void CFW_EmodGetIMEI(uint8_t pImei[16], uint8_t *pImeiLen, CFW_SIM_ID nSimID); //pImei is the buffer that
void CFW_EmodSaveIMEI(uint8_t *pImei, uint8_t imeiLen, CFW_SIM_ID nSimID);
uint32_t CFW_EmodSupportDisableIdLte(
    uint8_t bSupport,
    CFW_SIM_ID nSimId);

void CFW_EmodSetTracePort(uint8_t port);
uint8_t CFW_EmodGetTracePort(void);
void CFW_EmodSetTraceFlag(uint8_t bStop);
uint8_t CFW_EmodGetTraceFlag(void);

/************************************
Check the Gsm calibration mark bit
return: uint32_t
     GSM_calib_afc  --bit0
     GSM850_calib_agc  --bit1
     GSM850_calib_apc  --bit2
     GSM900_calib_agc  --bit3
     GSM900_calib_apc  --bit4
     PCS_calib_agc  --bit5
     PCS_calib_apc  --bit6
     DCS_calib_agc  --bit7
     DCS_calib_apc  --bit8
     GSM_FT_flag  --bit9
*************************************/
uint32_t CFW_EmodGetGsmCalibInfo(void);

/************************************
Check the Lte calibration mark bit
return: uint32_t
    LTE_calib_afc  --bit0
    LTE_TDD_calib_agc  --bit1
    LTE_TDD_calib_apc  --bit2
    LTE_FDD_calib_agc  --bit3
    LTE_FDD_calib_apc  --bit4
    LTE_FT_flag  --bit9
************************************/
uint32_t CFW_EmodGetLteCalibInfo(void);

uint32_t CFW_EmodGetNvVersion(void);

uint8_t *CFW_EmodGetNvData(
    uint8_t dataType,
    uint8_t nSim);

int32_t CFW_EmodSetNvData(
    uint8_t dataType,
    uint16_t offset,
    uint16_t dataLength,
    uint8_t *data,
    uint8_t nSim);

int32_t CFW_EmodSaveNvData(
    uint8_t dataType,
    uint8_t nSim);

int32_t CFW_EmodSpecialNvProcess(
    uint8_t operType,
    uint8_t dataType,
    uint16_t offset,
    uint16_t dataLength,
    uint8_t *data,
    uint8_t nSim);

uint32_t CFW_EmodEngineerCmd(
    uint32_t module,
    uint32_t cmd,
    uint32_t v1,
    uint32_t v2,
    uint32_t v3);

//
//bind
//

typedef struct _CFW_EMOD_BIND_SIM
{
    uint8_t nIndex;
    uint8_t nImsiLen;
    uint8_t pImsiNum[15];
    uint8_t pad[3];
} CFW_EMOD_BIND_SIM;

typedef struct _CFW_EMOD_BIND_NW
{
    uint8_t nIndex;
    uint8_t pNccNum[3];
} CFW_EMOD_BIND_NW;

typedef struct _CFW_EMOD_READ_BIND_SIM
{
    uint8_t nNumber;
    uint32_t nPointer;
    uint8_t pad[3];
} CFW_EMOD_READ_BIND_SIM;

typedef struct _CFW_EMOD_READ_BIND_NW
{
    uint8_t nNumber;
    uint32_t nPointer;
    uint8_t pad[3];
} CFW_EMOD_READ_BIND_NW;

void CFW_EmodReadBindSIM(CFW_EMOD_READ_BIND_SIM *pReadBindSim);

void CFW_EmodReadBindNW(CFW_EMOD_READ_BIND_NW *pReadBindNw);

void CFW_EmodGetBindInfo(bool *bSim, bool *bNetWork); //ture: had been bond.false:not bind

uint32_t CFW_EmodSetBindInfo(bool bSim, bool bNetWork);

typedef struct _CFW_EMOD_UPDATE_RETURN
{
    uint32_t nErrCode;
    uint8_t nIndex;
} CFW_EMOD_UPDATE_RETURN;

CFW_EMOD_UPDATE_RETURN CFW_EmodUpdateSimBind(
    uint8_t nIndex,
    uint8_t *pBindSimNum,
    uint8_t nLen);

CFW_EMOD_UPDATE_RETURN CFW_EmodUpdateNwBind(
    uint8_t nIndex,
    uint8_t *pBindNwNum,
    uint8_t nLen);

/**
 * byte pool information
 */
typedef struct
{
    uint32_t avail_size; ///< available size. The actual allocatable size may be less than this
} CFW_BytePoolInfo_t;
/**
 * block pool information
 */
typedef struct
{
    uint32_t avail_blocks; ///< available blocks.
    uint32_t block_size;
} CFW_BlockPoolInfo_t;

#define CFW_BLOCK_MEM_NUM 10

typedef struct
{
    CFW_BytePoolInfo_t ByteMemInfo;
    uint8_t nBlocks;
    CFW_BlockPoolInfo_t BlockMeminfo[CFW_BLOCK_MEM_NUM];
} CFW_AllMemInfo_t;
uint32_t CFW_EmodGetAllMemoryInfo(CFW_AllMemInfo_t *pMemInfo);

//SN number
typedef struct _CFW_EMOD_LOCT_PRODUCT_INFO
{
    uint8_t nBoardSNLen;
    uint8_t nBoardSN[64];
    uint8_t nPhoneSNLen;
    uint8_t nPhoneSN[64];
} CFW_EMOD_LOCT_PRODUCT_INFO;

typedef struct _CFW_EMOD_RF_CALIB_INFO
{

    uint16_t nLowDacLimit[6];
    uint16_t nLowVoltLimit[6];
    uint32_t nName;
    int16_t nPcl2dbmArfcn_d[30];
    int16_t nPcl2dbmArfcn_g[30];
    int16_t nPcl2dbmArfcn_p[30];
    int16_t nProfiledBmMax_dp;
    int16_t nProfiledBmMax_g;
    int16_t nProfiledBmMin_dp;
    int16_t nProfiledBmMin_g;
    //  uint16_t nProfile_dp[1024];
    //  uint16_t nProfile_g[1024];
    uint16_t nProfileInterp_dp[16];
    uint16_t nProfileInterp_g[16];
    uint16_t nRampHigh[32];
    uint16_t nRampLow[32];
    uint16_t nRampSwap_dp;
    uint16_t nRampSwap_g;
    int32_t nRaram[15];
    int16_t nTimes[15];
} CFW_EMOD_RF_CALIB_INFO;

void CFW_EmodGetRFCalibrationInfo(CFW_EMOD_RF_CALIB_INFO *pRfCalib);

typedef struct
{
    uint8_t atFuncIndex;
    uint8_t value;
    uint8_t rsrp_flag;
    uint8_t rsrp;
    uint8_t rsrq_flag;
    uint8_t rsrq;
    uint8_t gsmBand;
    uint8_t gsmNum;
    uint16_t gsmArfcn[3];
    uint8_t lteNum;
    uint32_t lteArfcn[8];
} CFW_EmodSpLteDummyPara_t;

typedef struct
{
    uint8_t paramIndex;
    uint8_t enable;
    uint16_t value1;
    uint16_t value2;
} CFW_EmodRrtmParam_t;

typedef struct
{
    uint32_t earfcn;
    uint8_t prior;
    uint8_t thresh_h;
    uint8_t thresh_l;
} CFW_EmodGrrLteFreq_t;

typedef struct
{
    uint8_t index;
    uint8_t value;
} CFW_EmodRrdmParam_t;

typedef struct
{
    uint16_t index;
    uint16_t value;
} CFW_EmodL1Param_t;

typedef struct
{
    uint8_t type;
} CFW_EmodSpCleanInfo_t;
//
//  test mode of outfield
//

#define CFW_TSM_CURRENT_CELL 0x01
#define CFW_TSM_NEIGHBOR_CELL 0x02
#define CFW_TSM_PLOT_LTE 0x03
#define CFW_TSM_END_EMOD 0x04

/*********************************************************************************************************/
typedef struct _CFW_TSM_CURR_CELL_INFO
{
    uint16_t nTSM_Arfcn; //Absolute Radio Frequency Channel Number ,[0..1023].
    uint8_t nTSM_Rla;    // Averaged measure
    uint8_t nTSM_LAI[5]; //Location Area Identification.
    uint8_t nTSM_RAC;    //Routing Area Code ( GPRS only),  [0..255].
#if (defined LTE_NBIOT_SUPPORT) || (defined LTE_SUPPORT)
    uint8_t nTSM_CellID[4]; //Cell Identity.
#else
    uint8_t nTSM_CellID[2]; //Cell Identity.
#endif
    uint8_t nTSM_Bsic;      //Synchronisation code,[0..63].
    bool nTSM_BCCHDec;      //Indicates if the BCCH info has been decoded on the cell.
    uint8_t nTSM_NetMode;   //Network Mode of Operation (GPRS only),[0..2].
    uint8_t nTSM_AvRxLevel; //Average Rx level in Abs Val of dBm ,[0..115].
    uint8_t nTSM_MaxTxPWR;  //Max allowed transmit power, [0..31].
    int8_t nTSM_C1Value;    //Cell quality ,[-33..110].
    uint8_t nTSM_BER;       //Bit Error Rate [0..255],255 = non significant.
    uint8_t nTSM_TimeADV;   //Timing Advance (valid in GSM connected mode only).[0..255] 255 = non significant.
    uint8_t nTSM_CCCHconf;  //ccch configuration
    //0:CCCH_1_CHANNEL_WITH_SDCCH meanning 1 basic physical channel used for CCCH, combined with SDCCH
    //1:CCCH_1_CHANNEL  meanning 1 basic physical channel used for CCCH, not combined with SDCCH
    //2:CCCH_2_CHANNEL  meanning 2 basic physical channel used for CCCH, not combined with SDCCH
    //3:CCCH_3_CHANNEL  meanning 3 basic physical channel used for CCCH, not combined with SDCCH
    //4:CCCH_4_CHANNEL  meanning 4 basic physical channel used for CCCH, not combined with SDCCH
    //0xFF:INVALID_CCCH_CHANNEL_NUM meanning Invalid value
    uint8_t nTSM_BsPaMfrms;    // BS-PA-MFRMS
    uint8_t nTSM_RxQualFull;   //receive quality full  [0~7]
    uint8_t nTSM_RxQualSub;    //receive quality sub   [0~7]
    int16_t nTSM_C2;           //Value of the cell reselection criterium   [-96~173]
    uint8_t nTSM_CurrBand;     //current band used,possible value see below
    uint8_t nTSM_CurrChanType; //Current channel type
    //0:CHANNEL_TYPE_SDCCH meanning Current channel type is SDCCH
    //1:CHANNEL_TYPE_TCH_H0 meanning Current channel type is TCH ,half rate,Subchannel 0
    //2:CHANNEL_TYPE_TCH_H1 meanning Current channel type is TCH ,half rate,Subchannel 1
    //3:CHANNEL_TYPE_TCH_F meanningCurrent channel type is TCH ,full rate
    //0xff:INVALID_CHANNEL_TYPE meanning Invalid channel type
    uint32_t nTSM_CurrChanMode; //current channel mode ,only valid in decicated mode
    //API_SIG_ONLY  1<<0    signalling only
    //API_SPEECH_V1 1<<1    speech full rate or half rate version 1
    //API_SPEECH_V2 1<<2    speech full rate or half rate version 2
    //API_SPEECH_V3 1<<3    speech full rate or half rate version 3
    //API_43_14_KBS 1<<4    data, 43.5kbit/s dwnlnk, 14.5kbit/s uplnk
    //API_29_14_KBS 1<<5    data, 29.0kbit/s dwnlnk, 14.5kbit/s uplnk
    //API_43_29_KBS 1<<6    data, 43.5kbit/s dwnlnk, 29.0kbit/s uplnk
    //API_14_43_KBS 1<<7    data, 14.5kbit/s dwnlnk, 43.5kbit/s uplnk
    //API_14_29_KBS 1<<8    data, 14.5kbit/s dwnlnk, 29.0kbit/s uplnk
    //API_29_43_KBS 1<<9    data, 29.0kbit/s dwnlnk, 43.5kbit/s uplnk
    //API_43_KBS    1<<10   data, 43.5kbit/s
    //API_32_KBS    1<<11   data, 32.0kbit/s
    //API_29_KBS    1<<12   data, 29.0kbit/s
    //API_14_KBS    1<<13   data, 14.5kbit/s
    //API_12_KBS    1<<14   data, 12.0kbit/s
    //API_6_KBS     1<<15   data, 6.0kbit/s
    //API_3_KBS     1<<16   data, 3.6kbit/s
    //API_INVALID_CHAN_MODE 0xFFFFFFFF  Invalid channel mode
    //9:BAND_GSM900 meanning Band GSM900P, GSM900E and GSM900R
    //18:BAND_DCS1800 meanning Band DCS1800
    //19:BAND_PCS1900 meanning Band PCS 1900
    //0xFF:BAND_INVALID meanning Invalid band value
    uint8_t nTSM_MaxRetrans;  //max number of random access retransmission    1,2,4,7
    uint8_t nTSM_BsAgBlkRes;  //block number kept for AGCH in common channel  [0~7]
    uint8_t nTSM_AC[2];       //Access Class  [0~255]
    uint8_t nTSM_RxLevAccMin; //receive level access minimum  [0~63]
    bool bTSM_EC;             //Indicates if emergency call has been allowed
    bool bTSM_SI7_8;          //Indicates if SI7 and 8 are broadcast
    bool bTSM_ATT;            //Indicates if IMSI attach has been allowed
    uint8_t nTSM_RxDbm;       //indicates current rx power
    uint8_t nTSM_TxDbm;       //indicates current tx power
    uint8_t nTSM_Maio;
    uint8_t nTSM_Hsn;
    uint8_t nTSM_RxLevSub;
    uint8_t nTSM_RxLevFull;
    uint8_t nTSM_RxLev;
    uint8_t nTSM_Dtx_used;
    uint8_t pad[3];
} CFW_TSM_CURR_CELL_INFO;

typedef struct _CFW_TSM_CURR_CELL_INFO_V2
{
    uint16_t nTSM_Arfcn; //Absolute Radio Frequency Channel Number ,[0..1023].
    uint8_t nTSM_Rla;    // Averaged measure
    uint8_t nTSM_LAI[5]; //Location Area Identification.
    uint8_t nTSM_RAC;    //Routing Area Code ( GPRS only),  [0..255].
#if (defined LTE_NBIOT_SUPPORT) || (defined LTE_SUPPORT)
    uint8_t nTSM_CellID[4]; //Cell Identity.
#else
    uint8_t nTSM_CellID[2]; //Cell Identity.
#endif
    uint8_t nTSM_Bsic;      //Synchronisation code,[0..63].
    bool nTSM_BCCHDec;      //Indicates if the BCCH info has been decoded on the cell.
    uint8_t nTSM_NetMode;   //Network Mode of Operation (GPRS only),[0..2].
    uint8_t nTSM_AvRxLevel; //Average Rx level in Abs Val of dBm ,[0..115].
    uint8_t nTSM_MaxTxPWR;  //Max allowed transmit power, [0..31].
    int8_t nTSM_C1Value;    //Cell quality ,[-33..110].
    uint8_t nTSM_BER;       //Bit Error Rate [0..255],255 = non significant.
    uint8_t nTSM_TimeADV;   //Timing Advance (valid in GSM connected mode only).[0..255] 255 = non significant.
    uint8_t nTSM_CCCHconf;  //ccch configuration
    //0:CCCH_1_CHANNEL_WITH_SDCCH meanning 1 basic physical channel used for CCCH, combined with SDCCH
    //1:CCCH_1_CHANNEL  meanning 1 basic physical channel used for CCCH, not combined with SDCCH
    //2:CCCH_2_CHANNEL  meanning 2 basic physical channel used for CCCH, not combined with SDCCH
    //3:CCCH_3_CHANNEL  meanning 3 basic physical channel used for CCCH, not combined with SDCCH
    //4:CCCH_4_CHANNEL  meanning 4 basic physical channel used for CCCH, not combined with SDCCH
    //0xFF:INVALID_CCCH_CHANNEL_NUM meanning Invalid value
    uint8_t nTSM_BsPaMfrms;    // BS-PA-MFRMS
    uint8_t nTSM_RxQualFull;   //receive quality full  [0~7]
    uint8_t nTSM_RxQualSub;    //receive quality sub   [0~7]
    int16_t nTSM_C2;           //Value of the cell reselection criterium   [-96~173]
    uint8_t nTSM_CurrBand;     //current band used,possible value see below
    uint8_t nTSM_CurrChanType; //Current channel type
    //0:CHANNEL_TYPE_SDCCH meanning Current channel type is SDCCH
    //1:CHANNEL_TYPE_TCH_H0 meanning Current channel type is TCH ,half rate,Subchannel 0
    //2:CHANNEL_TYPE_TCH_H1 meanning Current channel type is TCH ,half rate,Subchannel 1
    //3:CHANNEL_TYPE_TCH_F meanningCurrent channel type is TCH ,full rate
    //0xff:INVALID_CHANNEL_TYPE meanning Invalid channel type
    uint32_t nTSM_CurrChanMode; //current channel mode ,only valid in decicated mode
    //API_SIG_ONLY  1<<0    signalling only
    //API_SPEECH_V1 1<<1    speech full rate or half rate version 1
    //API_SPEECH_V2 1<<2    speech full rate or half rate version 2
    //API_SPEECH_V3 1<<3    speech full rate or half rate version 3
    //API_43_14_KBS 1<<4    data, 43.5kbit/s dwnlnk, 14.5kbit/s uplnk
    //API_29_14_KBS 1<<5    data, 29.0kbit/s dwnlnk, 14.5kbit/s uplnk
    //API_43_29_KBS 1<<6    data, 43.5kbit/s dwnlnk, 29.0kbit/s uplnk
    //API_14_43_KBS 1<<7    data, 14.5kbit/s dwnlnk, 43.5kbit/s uplnk
    //API_14_29_KBS 1<<8    data, 14.5kbit/s dwnlnk, 29.0kbit/s uplnk
    //API_29_43_KBS 1<<9    data, 29.0kbit/s dwnlnk, 43.5kbit/s uplnk
    //API_43_KBS    1<<10   data, 43.5kbit/s
    //API_32_KBS    1<<11   data, 32.0kbit/s
    //API_29_KBS    1<<12   data, 29.0kbit/s
    //API_14_KBS    1<<13   data, 14.5kbit/s
    //API_12_KBS    1<<14   data, 12.0kbit/s
    //API_6_KBS     1<<15   data, 6.0kbit/s
    //API_3_KBS     1<<16   data, 3.6kbit/s
    //API_INVALID_CHAN_MODE 0xFFFFFFFF  Invalid channel mode
    //9:BAND_GSM900 meanning Band GSM900P, GSM900E and GSM900R
    //18:BAND_DCS1800 meanning Band DCS1800
    //19:BAND_PCS1900 meanning Band PCS 1900
    //0xFF:BAND_INVALID meanning Invalid band value
    uint8_t nTSM_MaxRetrans;  //max number of random access retransmission    1,2,4,7
    uint8_t nTSM_BsAgBlkRes;  //block number kept for AGCH in common channel  [0~7]
    uint8_t nTSM_AC[2];       //Access Class  [0~255]
    uint8_t nTSM_RxLevAccMin; //receive level access minimum  [0~63]
    bool bTSM_EC;             //Indicates if emergency call has been allowed
    bool bTSM_SI7_8;          //Indicates if SI7 and 8 are broadcast
    bool bTSM_ATT;            //Indicates if IMSI attach has been allowed
    uint8_t nTSM_RxDbm;       //indicates current rx power
    uint8_t nTSM_TxDbm;       //indicates current tx power
    uint8_t nTSM_Maio;
    uint8_t nTSM_Hsn;
    uint8_t nTSM_RxLevSub;
    uint8_t nTSM_RxLevFull;
    uint8_t nTSM_RxLev;
    uint8_t nTSM_Dtx_used;
    uint8_t pad[3];
    uint8_t nTSM_ts; //number format
} CFW_TSM_CURR_CELL_INFO_V2;

typedef struct _CFW_TSM_NEIGHBOR_CELL_INFO
{
    uint16_t nTSM_Arfcn;    //Absolute Radio Frequency Channel Number ,[0..1023].
    uint8_t nTSM_CellID[2]; //Cell Identity.
    uint8_t nTSM_AvRxLevel; //Average Rx level in Abs Val of dBm ,[0..115].==RSSI
    uint8_t nTSM_Bsic;      //Synchronisation code,[0..63].
    bool nTSM_BCCHDec;      //Indicates if the BCCH info has been decoded on the cell.
    uint8_t nTSM_MaxTxPWR;  //Max allowed transmit power, [0..31].
    uint8_t nTSM_RxLevAM;   //RxLev Access Min in RxLev [0..63].
    int8_t nTSM_C1Value;    //Cell quality ,[-33..110].see 05.08
    uint8_t nTSM_LAI[5];    //Location Area Identification.
    int16_t nTSM_C2;        //Value of the cell reselection criterium   [-96~173]
    int16_t nTSM_C31;
    int16_t nTSM_C32;
} CFW_TSM_NEIGHBOR_CELL_INFO;

typedef struct _CFW_TSM_ALL_NEBCELL_INFO
{
    uint8_t nTSM_NebCellNUM;                     ///< neighbor cell number.
    CFW_TSM_NEIGHBOR_CELL_INFO nTSM_NebCell[10]; ///< the max of the neighbor cell is 6.
    uint8_t pad[3];
} CFW_TSM_ALL_NEBCELL_INFO;

typedef struct _CFW_TSM_NEIGHBOR_CELL_INFO_V2
{
    uint16_t nTSM_Arfcn;    //Absolute Radio Frequency Channel Number ,[0..1023].
    uint8_t nTSM_CellID[2]; //Cell Identity.
    uint8_t nTSM_AvRxLevel; //Average Rx level in Abs Val of dBm ,[0..115].==RSSI
    uint8_t nTSM_Bsic;      //Synchronisation code,[0..63].
    bool nTSM_BCCHDec;      //Indicates if the BCCH info has been decoded on the cell.
    uint8_t nTSM_MaxTxPWR;  //Max allowed transmit power, [0..31].
    uint8_t nTSM_RxLevAM;   //RxLev Access Min in RxLev [0..63].
    int8_t nTSM_C1Value;    //Cell quality ,[-33..110].see 05.08
    uint8_t nTSM_LAI[5];    //Location Area Identification.
    int16_t nTSM_C2;        //Value of the cell reselection criterium   [-96~173]
    int16_t nTSM_C31;
    int16_t nTSM_C32;
    uint32_t nTSM_EArfcn;
    int16_t nTSM_CellIdentity;
    int16_t nTSM_RSRP;
    int16_t nTSM_RSRQ;
} CFW_TSM_NEIGHBOR_CELL_INFO_V2;

typedef struct _CFW_TSM_ALL_NEBCELL_INFO_V2
{
    uint8_t nTSM_NebCellNUM; ///< neighbor cell number.
    uint8_t LTEAdjCellNb;
    CFW_TSM_NEIGHBOR_CELL_INFO_V2 nTSM_NebCell[20]; ///< the max of the neighbor cell is 6.
    uint8_t pad[3];
} CFW_TSM_ALL_NEBCELL_INFO_V2;

typedef struct _CFW_TSM_FUNCTION_SELECT
{
    bool nServingCell;  ///< true:enable the function ,false:disable.
    bool nNeighborCell; ///< true:enable the function ,false:disable.
    uint8_t nCurrentNetMode;
    uint16_t nCurrentUti;
    uint8_t pad[2];
} CFW_TSM_FUNCTION_SELECT;

typedef struct _CFW_TUE_STATUS_INFO
{
    uint32_t dlEarfcn;
    uint16_t pcid;
    int16_t rsrp;
    int16_t rsrq;
    uint16_t sinr;
    uint16_t mcl;
    uint16_t ulMcs;
    uint16_t dlMcs;
    uint16_t mpdcchRepNum;
    uint16_t puschRepNum;
    uint16_t pdschRepNum;
    uint16_t ulInitialBler;
    uint16_t dlInitialBler;
    uint32_t ulRbNum;
    uint32_t dlRbNum;
    uint32_t ulRlcRate;
    uint32_t dlRlcRate;
    uint32_t ulTbs;
    uint32_t dlTbs;
} CFW_TUE_STATUS_INFO;

void CFW_GetStatusInfo(CFW_TUE_STATUS_INFO *nStatusInfo, CFW_SIM_ID nSimID);
bool CFW_GetScanRssiInfo(uint8_t band, uint8_t index, uint16_t *pFreqsNums, int16_t rssi[400], CFW_SIM_ID nSimID);
/***************************************API FUNCTION OF TSM***********************************************/
uint32_t CFW_GetAoHandle(uint8_t nIndex, uint32_t nState, uint32_t nServiceId, CFW_SIM_ID nSimID);
uint32_t CFW_EmodOutfieldTestStart(CFW_TSM_FUNCTION_SELECT *pSelecFUN, uint16_t nUTI, CFW_SIM_ID nSimID);
uint32_t CFW_EmodOutfieldTestStartV2(CFW_TSM_FUNCTION_SELECT *pSelecFUN, uint16_t nUTI, CFW_SIM_ID nSimID);

uint32_t CFW_EmodOutfieldTestEnd(uint16_t nUTI, CFW_SIM_ID nSimID);
uint32_t CFW_GetCellInfo(CFW_TSM_CURR_CELL_INFO *pCurrCellInfo, CFW_TSM_ALL_NEBCELL_INFO *pNeighborCellInfo, CFW_SIM_ID nSimID);
uint32_t CFW_GetCellInfoV2(CFW_TSM_CURR_CELL_INFO_V2 *pCurrCellInfo, CFW_TSM_ALL_NEBCELL_INFO_V2 *pNeighborCellInfo, CFW_SIM_ID nSimID);
uint8_t *CFW_EmodGetBaseBandVersionV1(void);

uint32_t CFW_EmodSyncInfoTest(bool bStart, uint16_t nUTI, CFW_SIM_ID nSimID);

#define CFW_SYNC_INFO_IND 0x01
#define CFW_POWER_LIST_IND 0x02

#define CFW_EMOD_POWER_LIST_COUNT 50

typedef struct
{
    uint16_t nArfcn;
    uint16_t nRssi;
} CFW_EmodPowerInfo;

typedef struct
{
    CFW_EmodPowerInfo nPowerList[CFW_EMOD_POWER_LIST_COUNT];
    uint8_t nCount;
} CFW_EmodPowerListInd;

typedef enum
{
    PHY_VERSION,
    STACK_VERSION,
    PHY_CFG_VERSION,
    CSW_VERSION,
    MMI_VERSION,
    SOFT_VERSION_ENUM_FILL = 0x7fffffff
} SOFT_VERSION;

typedef enum
{
    XCV_VERSION,
    PA_VERSION,
    SW_VERSION,
    CHIP_VERSION,
    FLASH_VERSION,
    HW_VERSION_ENUM_FILL = 0x7fffffff
} HW_VERSION;

typedef struct _Soft_Version
{
    uint8_t nVersion[30];
    uint32_t nDate;
} Soft_Version;

typedef struct _HW_Version
{
    uint8_t nVersion[30];
} HW_Version;

uint32_t reg_GetRegSize(void);
int32_t REG_Read(uint32_t ulAddrOffset, uint8_t *pBuffer, uint32_t ulBytesToRead, uint32_t *pBytesRead);
int32_t REG_Write(uint32_t ulAddrOffset, const uint8_t *pBuffer, uint32_t ulBytesToWrite, uint32_t *pBytesWritten);

//hameina[+]20081031 for bug 10176
//
void CFW_CcSetCSSI(bool nCSSI);
void CFW_CcSetCSSU(bool nCSSU);

uint32_t CFW_GetFreeCID(uint8_t *pCID, CFW_SIM_ID nSimID);
uint32_t CFW_ReleaseCID(uint8_t nCID, CFW_SIM_ID nSimID);
uint32_t CFW_SetCID(uint8_t nCID, CFW_SIM_ID nSimID);
uint8_t CFW_isPdpActiveAvailable(CFW_SIM_ID nSimID);

//#ifdef CFW_EXTENDED_API
uint32_t cfw_SimReadElementaryFile(uint16_t nUTI, uint32_t fileId, CFW_SIM_ID nSimID);
uint32_t CFW_SimGetACMMax(uint16_t nUTI, CFW_SIM_ID nSimID);
uint32_t CFW_SimSetACMMax(uint32_t iACMMaxValue, uint8_t *pPin2, uint8_t nPinSize, uint16_t Nuti, CFW_SIM_ID nSimID);
uint32_t CFW_SimGetACM(uint16_t nUTI, CFW_SIM_ID nSimID);
uint32_t CFW_SimSetACM(uint32_t iCurValue, uint8_t *pPin2, uint8_t nPinSize, uint16_t Nuti, CFW_SIM_ID nSimID);
uint32_t CFW_SimGetPUCT(uint16_t nUTI, CFW_SIM_ID nSimID);
uint32_t CFW_SimSetPUCT(CFW_SIM_PUCT_INFO *pPUCT, uint8_t *pPin2, uint8_t nPinSize, uint16_t nUTI, CFW_SIM_ID nSimID);
void CFW_EmodSetLteLockFreq(uint8_t number, uint32_t *freq, CFW_SIM_ID nSimId);
void CFW_EmodGetLteLockFreq(uint8_t *number, uint32_t freq[9], CFW_SIM_ID nSimId);
void CFW_EmodSetLteLockBand(uint8_t number, uint16_t *band, CFW_SIM_ID nSimId);
void CFW_EmodGetLteLockBand(uint8_t *number, uint16_t band[20], CFW_SIM_ID nSimId);
void CFW_EmodSetLteLockBandV2(uint8_t number, uint16_t *band, CFW_SIM_ID nSimId);
uint32_t CFW_EmodGetLteBand(uint8_t *number, uint16_t band[24], CFW_SIM_ID nSimId);
uint32_t CFW_EmodSetLteBand(uint8_t number, uint16_t *band, CFW_SIM_ID nSimId);
uint16_t CFW_GetBandFromFreq(uint32_t freq);
void CFW_EmodGetLteLockBandV2(uint8_t *number, uint16_t band[5], CFW_SIM_ID nSimId);
void CFW_EmodGetLteLockTddOrFdd(uint8_t *tddfdd, CFW_SIM_ID nSimId);
void CFW_EmodSetLteLockTddOrFdd(uint8_t tddfdd, CFW_SIM_ID nSimId);
void CFW_EmodSetLteLockCell(uint16_t Cell, CFW_SIM_ID nSimId);
void CFW_EmodGetLteLockCell(uint16_t *Cell, CFW_SIM_ID nSimId);
void CFW_EmodGetCEMode(uint8_t *pMode, CFW_SIM_ID nSimID);
void CFW_EmodSetCEMode(uint8_t nMode, CFW_SIM_ID nSimID);
void CFW_EmodGetVoicePrefer(uint8_t *pVoicePrefer, CFW_SIM_ID nSimID);
void CFW_EmodSetVoicePrefer(uint8_t nVoicePrefer, CFW_SIM_ID nSimID);

//Add by wcf, 2015/1/21, process polling timer
#ifdef _SIM_HOT_PLUG_
void CFW_SetPollingTimer(uint8_t PollingInterval, CFW_SIM_ID nSimID);
void CFW_GetPollingTimer(uint8_t *pPollingInterval, CFW_SIM_ID nSimID);
#endif
//End, process polling timer

void CSW_FacilityCrssDeflec(
    uint8_t nIndex,
    CFW_SIM_ID nSimId,
    uint8_t DTN[20], ///< Deflec number
    uint8_t DTNLen,
    uint8_t DTSA[21], ///< sub number
    uint8_t DTSALen);

#define ICCID_LENGTH 10
uint8_t *CFW_GetICCID(CFW_SIM_ID nSimID);

#define FIRST_MATCH 0xA5
#define ALL_MATCH 0x5A
#define WILDCARD_MATCH 0x55

#define SEEK_SIM_ADN 0
#define SEEK_SIM_FDN 1
#define SEEK_SIM_MSISDN 2
#define SEEK_SIM_BDN 3
#define SEEK_SIM_SDN 4
#define SEEK_SIM_LND 5

#define SEEK_SIM_NAME 3
#define SEEK_SIM_NUMBER 4
#define SEEK_SIM_USED 5

typedef struct _CFW_SIM_PBK_INFO
{
    uint16_t nIndex;
    uint8_t nType;
    uint8_t nLengthAl;
    uint8_t nLengthNb;
    uint8_t nAlpha[40];
    uint8_t nNumber[20];
} CFW_SIM_PBK_INFO;

typedef struct _CFW_SIM_SEARCH_PBK
{
    uint16_t nMatchRecords; ///< is the number of the LV
    uint16_t nUsedRecords;  ///< is the number of the LV
    uint16_t nTotalRecords;
    uint8_t nData[0]; ///< nData is composed of two bytes index, one byte type,
    //one byte length of name and name
    //one byte length of numbre and number
} CFW_SIM_SEARCH_PBK;

uint32_t CFW_CfgNwGetOperatorIdEx(
    uint8_t **pOperatorId,
    uint8_t *pOperatorName,
    uint8_t format,
    uint32_t *idx);

uint32_t CFW_CfgNwGetOperatorNameEx(
    uint8_t pOperatorId[6],
    uint8_t **pOperatorName,
    uint8_t **pOperatorShortName);

uint32_t CFW_NwDeRegisterEx(
    uint16_t nUTI,
    uint8_t detachType,
    bool poweroff,
    CFW_SIM_ID nSimID);

typedef enum _CFW_CALL_CLIR_E
{
    CFW_CALL_CLIR_INVOCATION = 1,
    CFW_CALL_CLIR_SUPPRESSION,
    CFW_CALL_CLIR_NETWORK_DECIDE
} CFW_CALL_CLIR_E;

typedef enum CFW_VOICE_MAIL_TYPE_E
{
    CFW_VOICE_MAIL,
    CFW_VOICE_MAIL_FAX,
    CFW_VOICE_MAIL_EMAIL,
    CFW_VOICE_MAIL_OTHER,
    CFW_VOICE_MAIL_VERSION_ENUM_FILL = 0x7fffffff
} CFW_VOICE_MAIL_TYPE_E;

typedef struct _CFW_SIM_INFO_VOICEMAIL
{
    uint8_t alpha_id_len;
    uint8_t mailbox_number_len;
    uint8_t ccp_id;
    bool is_ext_exist;
    uint8_t ext_id;
    bool is_ccp_exist;
    bool is_ton_npi_exist;
    uint8_t alpha_id[20];
    uint8_t mailbox_number[10];
    uint8_t ton;
    uint8_t npi;
    CFW_VOICE_MAIL_TYPE_E vm_type;
    bool is_voicemail_exist;
    uint8_t line_index;
} CFW_SIM_INFO_VOICEMAIL;

typedef struct _CFW_SIM_INFO_READVOICEMAILINFO
{
    uint8_t n_PrevStatus;
    uint8_t n_CurrStatus;
    uint8_t nTryCount;
    uint8_t nCurrentFile;
    uint8_t nTotalRecordNumber;
    uint8_t RecordLength;
    uint8_t nStatus;
    uint8_t nCurrInd;
    uint8_t nDataSize;
    uint8_t nLine_index;
    CFW_VOICE_MAIL_TYPE_E nVoiceMailType;
    CFW_SIM_INFO_VOICEMAIL vm_content;
    uint8_t pattern[5];
} CFW_SIM_INFO_READVOICEMAILINFO;

typedef struct _CFW_SIM_SMS_PARAMETERS
{
    uint8_t nSMSCInfo[12];
    uint8_t nPID;
    uint8_t nDCS;
    uint8_t nVP;
    uint8_t nFlag;
} CFW_SIM_SMS_PARAMETERS;

typedef struct _TM_ALARM
{
    uint32_t nFileTime;
    uint8_t *pText;
    uint8_t nTextLength;
    uint8_t nRecurrent;
    uint8_t padding[2];
    uint16_t nIndex;
    uint16_t nType;
} TM_ALARM;

uint32_t CFW_CcCallMultipartyFunction(uint8_t nFun, CFW_SIM_ID nSimId);
uint32_t CFW_SimReadVoiceMailInfo(uint8_t nLineId, CFW_VOICE_MAIL_TYPE_E nType, uint16_t nUTI, CFW_SIM_ID nSimID);
uint32_t CFW_SimUpdateVoiceMailInfo(uint8_t nLineId, CFW_VOICE_MAIL_TYPE_E nType, CFW_SIM_INFO_VOICEMAIL *pVoiceMail, uint16_t nUTI, CFW_SIM_ID nSimID);

typedef struct
{
    uint16_t success;   ///< success times
    uint16_t fail;      ///< failure times
    uint16_t passRatio; ///< success rate
    uint32_t delay;     ///< delay time,unit:ms
} cfw_statistic_t;

typedef struct
{
    uint16_t dropOn2g; ///< drop times on gsm
    uint16_t dropOn4g; ///< drop times on lte
} cfw_dropTimes_t;

typedef struct
{
    uint32_t timeOn2g;     ///< time on gsm registered
    uint32_t timeOn4g;     ///< time on lte registered
    uint32_t timeOnVolte;  ///< time on volte registered
    uint32_t timeOnUnknow; ///< time on no RAT or drop
    uint32_t allTheTime;   ///< total time from power on
} cfw_attachTime_t;

typedef struct
{
    cfw_statistic_t handoverStatistic[8]; ///< 0:GToGHO,1:GToGCCO,2:GToLCCO,3:LToGCCO,4:GToLHO,5:LToLHO,6:LToGHO,7:SRVCC
} cfw_handoverInfo_t;

typedef struct
{
    cfw_statistic_t reselStatistic[4]; ///< 0:GToLResel,1:GToLResel,2:LToLResel,3:LToGResel
} cfw_reselectInfo_t;

typedef struct
{
    uint8_t lteCipherFlag; ///< default value 1
    uint8_t bsrvccFlag;    ///< default value 1
    uint8_t emfbiFlag;     ///< default value 1
    uint8_t ipv6Flag;      ///< default value 1
    uint8_t asrvccFlag;    ///< default value 1
    uint8_t esrvccFlag;    ///< default value 1
} cfw_capInfoLte_t;

typedef struct
{
    uint32_t arfcn;
    uint16_t bsic; ///< if not decode BSIC,apply 0xffff
    uint16_t rxlev;
} cfw_gsmNcell_t;

typedef struct
{
    uint8_t validFlag; //0: plmn/enodeBID/cellId/tac are invalid, 1:plmn/enodeBID/cellId/tac are valid
    uint8_t plmn[3];
    uint32_t enodeBID;            //first 20 bits of SIB1 cell identity
    uint16_t cellId;              //default value 0xff
    uint16_t tac;                 //serving tac(from SIB1)
} cfw_lteCellAccessRelatedInfo_t; //CellAccessRelatedInfo

typedef struct
{
    uint32_t frequency;
    uint16_t pcid; ///< if not decode BSIC,apply 0xffff
    uint16_t bandWidth;
    int16_t rsrp;
    int16_t rsrq;
    int16_t Srxlev;
    cfw_lteCellAccessRelatedInfo_t cellAccRelInfo;
} cfw_lteNcell_t;

typedef struct
{
    uint8_t plmn[3];
    uint8_t imsi[16];    ///< imsi[0]:length of IMSI
    uint8_t roamingFlag; ///< 1:roaming,0:non roaming
    uint16_t bandInfo;   ///< band1-band256
    uint32_t dlEarfcn;   ///< 0-262143, invalid value 0xffffffff
    uint16_t pcid;
    int16_t rsrp;
    int16_t rsrq;
    int16_t Srxlev;
    uint16_t tsMode;    ///< default value 0xffff
    uint16_t bandwidth; ///< default value 0xffff
    uint16_t SA;        ///< default value 0xffff
    uint16_t SSP;       ///< default value 0xffff
    uint16_t SINR;
    uint32_t ulMcs; ///< default value 0xffff
    uint32_t dlMcs; ///< default value 0xffff
    uint8_t ulBler;
    uint8_t dlBler;
    uint32_t enodeBID; ///< first 20 bits of SIB1 cell identity
    uint16_t cellId;   ///< default value 0xff
    uint16_t tac;      //serving tac(from SIB1)
    uint16_t netMode;  //0--tdd  1--fdd
    int16_t rxPower;   //Dl receiving power; Real value(Unit:dbm)=rxPower divide 16.
    int16_t txPower;   //UE transmits power, -46~23 dbm
    int16_t rssi;
} cfw_lteScell_t;

typedef struct
{
    cfw_lteScell_t lteScell;
    uint16_t lteNcellNum;
    cfw_lteNcell_t lteNcell[9];
    uint16_t gsmNcellNum;
    cfw_gsmNcell_t gsmNcell[5];
    cfw_capInfoLte_t capInfoLte;
} cfw_nwCapLte_t;

typedef struct
{
    uint32_t vamos;
    uint32_t diversityGsm;
    uint8_t Plmn[3];
    bool Roaming;
    uint8_t CellId[2];
    uint16_t Arfcn;
    uint8_t Bsic;
    uint8_t Rla;
    uint8_t Lai[5];
} cfw_ueCapGsm_t;

typedef struct
{
    cfw_ueCapGsm_t ueCapabilityGsm;
    cfw_nwCapLte_t nwCapabilityLte;
} CFW_NET_INFO_T;

typedef struct
{
    uint8_t validFlag;     /*0: invalid; 1:valid */
    uint8_t reselPriority; /*小区重选优先级，范围0~7，无效值0xFF。*/
    uint8_t threshXLow;    /*低优先级小区重选门限threshX-Low，范围0~62.*/
    uint8_t threshXHigh;   /*高优先级小区重选门限threshX-High，范围0~62.*/
    uint8_t nccPermit;     /* NCC PERMIT，范围0~255.*/
    uint8_t reserver[3];
} cfw_gsmCellReselInfo_t;

typedef struct
{
    uint32_t arfcn;
    uint16_t bsic; ///< if not decode BSIC,apply 0xffff
    uint16_t rxlev;
    int16_t srxlev;
    uint16_t reserved;
    cfw_gsmCellReselInfo_t gsmCellReselInfo;
} cfw_gsmNcell_t_V2;

typedef struct
{
    uint8_t validFlag;     /*0: invalid; 1:valid */
    uint8_t reselPriority; /*0~7, invalid value:255*/
    uint8_t threshXLow;    /*0~62*/
    uint8_t threshXHigh;   /*0~62*/
} cfw_lteCellReselInfo_t;

typedef struct
{
    uint32_t frequency;
    uint16_t pcid; ///< if not decode BSIC,apply 0xffff
    uint16_t bandWidth;
    int16_t rsrp;
    int16_t rsrq;
    int16_t Srxlev;
    uint16_t sinr;     /*小区的SINR，单位db。*/
    int16_t rssi;      /*小区的RSSI，单位dbm。*/
    uint16_t cellType; /*0:intra lte ncell, 1:inter lte ncell */
    cfw_lteCellAccessRelatedInfo_t cellAccRelInfo;
    cfw_lteCellReselInfo_t cellReselInfo; /* 小区的重选相关信息*/
} cfw_lteNcell_t_V2;

typedef struct
{
    uint8_t plmn[3];
    uint8_t imsi[16];    ///< imsi[0]:length of IMSI
    uint8_t roamingFlag; ///< 1:roaming,0:non roaming
    uint16_t bandInfo;   ///< band1-band256
    uint32_t dlEarfcn;   ///< 0-262143, invalid value 0xffffffff
    uint16_t pcid;
    int16_t rsrp;
    int16_t rsrq;
    int16_t Srxlev;
    uint16_t tsMode;    ///< default value 0xffff
    uint16_t bandwidth; ///< default value 0xffff
    uint16_t SA;        ///< default value 0xffff
    uint16_t SSP;       ///< default value 0xffff
    uint16_t SINR;
    uint32_t ulMcs; ///< default value 0xffff
    uint32_t dlMcs; ///< default value 0xffff
    uint8_t ulBler;
    uint8_t dlBler;
    uint32_t enodeBID; ///< first 20 bits of SIB1 cell identity
    uint16_t cellId;   ///< default value 0xff
    uint16_t tac;      //serving tac(from SIB1)
    uint16_t netMode;  //0--tdd  1--fdd
    int16_t rxPower;   //Dl receiving power; Real value(Unit:dbm)=rxPower divide 16.
    int16_t txPower;   //UE transmits power, -46~23 dbm
    int16_t rssi;
    uint16_t ulBandWidth;      /*服务小区ul_bandwidth, default value 0xffff*/
    uint16_t threshServingLow; /*服务小区低优先级重选门限threshServingLow；范围0~62。*/
    uint16_t sNonIntraSearch;  /*服务小区异频测量门限s-NonIntraSearch；范围0~62。*/
    uint16_t sIntraSearch;     /*服务小区同频测量门限s-IntraSearch；范围0~62。*/
} cfw_lteScell_t_V2;           // 服务小区

typedef struct
{
    uint32_t frequency;
    uint16_t freqType; /*0:intra lte freq, 1:inter lte freq*/
    uint16_t reserverd;
    cfw_lteCellReselInfo_t cellReselInfo;
} cfw_lteFreq_t;

typedef struct
{
    cfw_lteScell_t_V2 lteScell;
    uint16_t lteNcellNum;
    cfw_lteNcell_t_V2 lteNcell[9];
    uint16_t gsmNcellNum;
    cfw_gsmNcell_t_V2 gsmNcell[5];
    cfw_capInfoLte_t capInfoLte;
    uint16_t state; /*0:search 1: limit serve 2:no connect 3:connect*/
    uint16_t lteNFreqNum;
    cfw_lteFreq_t lteNFreq[9]; // No cell frequency
} cfw_nwCapLte_t_V2;

typedef struct
{
    cfw_ueCapGsm_t ueCapabilityGsm;
    cfw_nwCapLte_t_V2 nwCapabilityLte;
} CFW_NET_INFO_T_V2;

typedef struct
{
    cfw_reselectInfo_t reselectInfo;
    cfw_handoverInfo_t handoverInfo;
    cfw_attachTime_t attachTime;
    cfw_dropTimes_t dropTimes;
} CFW_NET_INFO_STATISTIC_T;

uint32_t CFW_GetNetinfo(CFW_NET_INFO_T *pNetinfo, CFW_SIM_ID nSimID);
uint32_t CFW_GetLETRealTimeInfo(CFW_NET_INFO_T *pNetinfo, CFW_SIM_ID nSimID);
uint32_t CFW_GetLETRealTimeInfoV2(CFW_NET_INFO_T_V2 *pNetinfo, CFW_SIM_ID nSimID);
uint32_t CFW_GetLETRealTimeInfoV3(CFW_NET_INFO_T_V2 *pNetinfo, CFW_SIM_ID nSimID);
bool CFW_CheckSupportLteBand(uint16_t nBand, CFW_SIM_ID nSimID);
bool CFW_CheckSupportLteFreq(uint32_t nFreq, uint8_t dlOrUl, CFW_SIM_ID nSimID);

#ifdef CONFIG_SOC_6760
void Cfw_SetNl1cSigMod(uint8_t mode);
void Cfw_SetNl1cDlParas(uint16_t *dlps);
void Cfw_SetNl1cDlRcv(uint8_t enable);
void Cfw_SetNl1cUlParas(uint16_t *ulps);
void Cfw_SetNl1cUlRcv(uint8_t enable);
void Cfw_SetNl1cAPC(uint8_t flag, int16_t val);
void Cfw_SetNl1cAGC(uint8_t flag, int16_t val);
void Cfw_SetCarrierNum(uint16_t carrier);
#endif

bool CFW_GprsSendDataAvaliable(uint8_t nSimID);
uint32_t CFW_GprsSendData(uint8_t nCid, CFW_GPRS_DATA *pGprsData, uint8_t rai, CFW_SIM_ID nSimID);

extern uint32_t CFW_SwitchUSBShareNetwork(bool flag);

extern uint32_t CSW_SetAudioZspVqeCalibParam(uint8_t *resultcode, uint8_t *hasMsg,
                                             uint8_t *resultMsg, uint8_t nMode, uint8_t nPath, uint8_t nCtrl, uint8_t *nParam, uint16_t nParamLength);

extern uint32_t CSW_SetAudioCodecCalibParam(uint8_t *resultcode, uint8_t *hasMsg,
                                            uint8_t *resultMsg, uint8_t nMode, uint8_t nPath, uint8_t nCtrl, uint8_t *nParam, uint16_t nParamLength);

extern uint32_t CSW_SetAudioZspPreProcessCalibParam(uint8_t *resultcode, uint8_t *hasMsg,
                                                    uint8_t *resultMsg, uint8_t nMode, uint8_t nPath, uint8_t nCtrl, uint8_t *nParam, uint16_t nParamLength);

extern uint32_t CSW_SetAudioZspPostProcessCalibParam(uint8_t *resultcode, uint8_t *hasMsg,
                                                     uint8_t *resultMsg, uint8_t nMode, uint8_t nPath, uint8_t nCtrl, uint8_t *nParam, uint16_t nParamLength);

extern uint32_t CSW_WriteCalibParamToFlash(uint8_t *resultcode, uint8_t *hasMsg,
                                           uint8_t *resultMsg, uint8_t nPath, uint8_t nCtrl, uint8_t *nParam, uint16_t nParamLength);

extern uint32_t CSW_ExportCalibOrImportCalibFlashParam(uint8_t *resultcode, uint8_t *hasMsg,
                                                       uint8_t *resultMsg, uint8_t nPath, uint8_t nCtrl, uint16_t nOffset, uint16_t nlength, uint8_t *nParam, uint16_t nParamLength);

extern uint32_t CSW_VersionControl(uint8_t *resultcode, uint8_t *hasMsg,
                                   uint8_t *resultMsg, uint8_t nPath, uint8_t nCtrl, uint8_t *nParam, uint16_t nParamLength);

extern uint32_t CSW_DumpPcmDataToTflash(uint8_t *resultcode, uint8_t *hasMsg,
                                        uint8_t *resultMsg, uint8_t nPath, uint8_t nCtrl, uint8_t *nParam, uint16_t nParamLength);

extern uint32_t CSW_SetAudioZspNxpCalibParam(uint8_t *resultcode, uint8_t *hasMsg,
                                             uint8_t *resultMsg, uint8_t nMode, uint8_t nPath, uint8_t nCtrl, uint8_t *nParam, uint16_t nParamLength);

extern uint32_t CFW_SmsMemoryFullReq(uint8_t nCause, CFW_SIM_ID nSimID);
extern uint32_t CFW_SmsMMAReq(CFW_SIM_ID nSimID);

#define CFW_USIM_SOFT_KEY_LEN 16
#define CFW_USIM_SOFT_OP_LEN 16
#define CFW_USIM_SOFT_OPC_LEN 16
#define CFW_AUTH_C_PARAMTER_LENGTH 16
#define CFW_USIM_SOFT_IMSI_LEN 16

typedef struct
{
    uint32_t enSoftUsimAlg;
    uint8_t aucKey[CFW_USIM_SOFT_KEY_LEN];
    uint8_t aucOp[CFW_USIM_SOFT_KEY_LEN];
} CFW_USIM_SOFT_AUTH_PARAM_STRU;

typedef struct
{
    uint8_t R1;
    uint8_t R2;
    uint8_t R3;
    uint8_t R4;
    uint8_t R5;
    uint8_t aucRsv[3];
    uint8_t C1[CFW_AUTH_C_PARAMTER_LENGTH];
    uint8_t C2[CFW_AUTH_C_PARAMTER_LENGTH];
    uint8_t C3[CFW_AUTH_C_PARAMTER_LENGTH];
    uint8_t C4[CFW_AUTH_C_PARAMTER_LENGTH];
    uint8_t C5[CFW_AUTH_C_PARAMTER_LENGTH];
} CFW_SSIM_AMRC_STRU;

typedef struct
{
    uint32_t bitOpcSoftUsimEnable : 1; // 1: VALID; 0: INVALID*, default : 0
    uint32_t bitOpRsv : 31;
    uint8_t aucOpc[CFW_USIM_SOFT_OPC_LEN];
} CFW_USIM_SOFT_OPC_PARAM_STRU;

typedef struct
{
    uint8_t mncLen;
    uint8_t imsi[CFW_USIM_SOFT_IMSI_LEN];
    CFW_USIM_SOFT_AUTH_PARAM_STRU usim_soft_auth_param_stru;
    CFW_USIM_SOFT_OPC_PARAM_STRU auth_opc;
    CFW_SSIM_AMRC_STRU sim_amrc_stru;
} CFW_SSIM_FILE_INFO_T;

uint32_t CFW_SaveSsimFileInfo(CFW_SSIM_FILE_INFO_T ssim_file_info, CFW_SIM_ID nSimID);
uint32_t CFW_GenerateSoftSimImsi(uint8_t nPlmn[7], uint8_t nImei[16], uint8_t nImsi[16], uint8_t nKey[33]);

uint32_t CFW_SetGcfTest(CFW_SIM_ID nSimID, bool nFlag);

uint8_t CFW_SmsGetSmsType(CFW_SIM_ID nSimID);

uint32_t CFW_SwitchPort(uint8_t nFlag);

#ifdef __cplusplus
}
#endif

#define SIM_STATE_IDLE 0
#define SIM_STATE_TPDUCMD 1
#define SIM_STATE_CHANNEL 2
#define SIM_STATE_SELECT_APP 3
#define SIM_STATE_STATUS 4
#define SIM_MAX_TPDU_LEN 512

typedef struct CFW_SIM_TPDU
{
    uint8_t nTPDU[SIM_MAX_TPDU_LEN];
    uint8_t nChannel;
    uint16_t nLength;
    uint8_t nStatus;
} CFW_SIM_TPDU;

uint32_t CFW_SimTPDUCommand(uint8_t *TPDU, uint16_t Length, uint8_t Channel, uint16_t nUTI, CFW_SIM_ID nSimID);

#define OPEN_CHANNEL_CMD 0x00
#define CLOSE_CHANNEL_CMD 0x80
#define OPEN_CMD_LENGTH 0x01
#define CLOSE_CMD_LENGTH 0x00
typedef struct CFW_SIM_CHANNEL
{
    uint8_t nCommand;
    uint8_t nChannel;
    uint8_t nStatus;
} CFW_SIM_CHANNEL;
uint32_t CFW_SimManageChannel(uint8_t command, uint8_t channel, uint16_t nUIT, CFW_SIM_ID nSimID);

typedef struct CFW_SIM_SELECT_APP
{
    uint8_t aid[16];
    uint8_t length;
    uint8_t channel;
    uint8_t nStatus;
} CFW_SIM_SELECT_APP;
uint32_t CFW_SimSelectApplication(uint8_t *aid, uint8_t length, uint8_t channel, uint16_t nUIT, CFW_SIM_ID nSimID);

uint32_t CFW_SimTPDUCommand(uint8_t *TPDU, uint16_t length, uint8_t channel, uint16_t nUTI, CFW_SIM_ID nSimID);

uint32_t CFW_GetRFTemperature(uint32_t *temp);

uint32_t CFW_SetLTEFreqPwrRange(uint16_t freqlow, uint16_t freqhigh, uint16_t power);
uint32_t CFW_GetLTEFreqPwrValue(uint32_t freq);

uint32_t CFW_SetRFFreqPwrRange(uint16_t mode, uint16_t band, uint16_t powerlow, uint16_t powerhigh);
uint32_t CFW_GetRFFreqPwrRange(uint16_t mode, uint16_t band, uint16_t *powerlow, uint16_t *powerhigh);

uint32_t CFW_SetSimFileUpdateCountMode(uint8_t op_mode, CFW_SIM_ID nSimID);
uint16_t CFW_GetSimFileUpdateCount(uint8_t nFileID, uint8_t bReal, CFW_SIM_ID nSimID);
uint32_t CFW_SetStaticScene(uint8_t iScene, CFW_SIM_ID nSimID);
uint32_t CFW_GetStaticScene(uint8_t *iScene, CFW_SIM_ID nSimID);
uint32_t CFW_SetRatePriority(uint8_t iPriority, CFW_SIM_ID nSimID);
uint32_t CFW_GetRatePriority(uint8_t *iPriority, CFW_SIM_ID nSimID);

typedef struct _CFW_APNS_UNAME_UPWD
{
    char apn[THE_APN_MAX_LEN];
    char username[THE_APN_USER_MAX_LEN];
    char password[THE_APN_PWD_MAX_LEN];
} CFW_APNS_UNAME_UPWD;

typedef struct
{
    uint8_t nTimeValue;
    uint8_t nMaxCount;
} CFW_PDN_TIMER_MAXCOUNT_INFO;

uint32_t CFW_SetPdnActTimerAndMaxCount(CFW_PDN_TIMER_MAXCOUNT_INFO pdnTimerAndMaxCount, uint8_t nRat, CFW_SIM_ID nSimID);

uint32_t CFW_GetPdnActTimerAndMaxCount(CFW_PDN_TIMER_MAXCOUNT_INFO *pdnTimerAndMaxCount, uint8_t nRat, CFW_SIM_ID nSimID);

uint32_t CFW_SetPdnDeactTimerAndMaxCount(CFW_PDN_TIMER_MAXCOUNT_INFO pdnTimerAndMaxCount, uint8_t nRat, CFW_SIM_ID nSimID);

uint32_t CFW_GetPdnDeactTimerAndMaxCount(CFW_PDN_TIMER_MAXCOUNT_INFO *pdnTimerAndMaxCount, uint8_t nRat, CFW_SIM_ID nSimID);

uint32_t CFW_SetCSPagingFlag(bool nFlag, CFW_SIM_ID nSimID);

uint32_t CFW_GetCSPagingFlag(uint8_t *nFlag, CFW_SIM_ID nSimID);

extern uint32_t CSW_SetAndGetMicGain(uint8_t *resultcode, uint8_t *hasMsg, uint8_t *resultMsg, uint8_t mode, uint8_t nPath, uint8_t nCtrl, uint8_t *nParam, uint16_t nParamLength);

// Application Id that resides on the UICC see TS 31.110 for coding

typedef struct CFW_UsimAidTag
{
    uint8_t data[16]; // AID value
    uint8_t length;   // AID length
} CFW_UsimAid;

typedef struct _NETWORK_REJECTION
{
    uint8_t tag; //LOCATION_INFO_TAG         location information
                 //ROUTING_AREA_ID_TAG       Routing area identification
                 //TRACKING_AREA_ID_TAG      Tracking area identification
    uint8_t MNCaMCC[3];
    uint8_t LACoTAC[2];
    uint8_t RAC;
    uint8_t CELL[4];
    uint8_t access;      //0 GSM, 03 UTRACE(3G), 08 E-UTRAN(4G)
    uint8_t type;        //update, attach, rejection
    uint8_t code;        //rejection code
    uint8_t extend_code; // exended rejection code
} NETWORK_REJECTION;

typedef struct _CFW_SAT_SETUP_IDLE_RSP
{
    uint8_t nComID;
    uint8_t nComQualifier;
    uint8_t nIdleTextLen;
    uint8_t nIdleTextSch;
    uint8_t *pIdleTextString;
    uint8_t nIconQualifier;
    uint8_t nIconIdentifier;
    uint8_t nTextAttrLength;
    uint8_t nFrameIdentifier;
    uint8_t *pTextAttribute;
} CFW_SAT_SETUP_IDLE_RSP;

uint32_t CFW_SimWakeup(uint16_t nUTI, CFW_SIM_ID nSimID, bool flag);
uint32_t CFW_SimCloseEx(uint16_t nUTI, CFW_SIM_ID nSimID, bool flag);
uint32_t SimPollReq(uint8_t nTimerOut, CFW_SIM_ID nSimID);
uint32_t SimPollOffReq(CFW_SIM_ID nSimID);
uint32_t CFW_SimInitStage1(CFW_SIM_ID nSimID);
uint32_t CFW_SimInitStage3(CFW_SIM_ID nSimID);
//#if ((defined CONFIG_SOC_6760) || (defined CONFIG_SOC_8811))
//uint32_t CFW_SimGetMeProfile(CFW_PROFILE **pMeProfile, CFW_SIM_ID nSimID);
//#else
uint32_t CFW_SimGetMeProfile(CFW_PROFILE *pMeProfile, CFW_SIM_ID nSimID);
//#endif
uint32_t SimSelectApplicationReq(uint8_t *AID, uint8_t AIDLength, CFW_SIM_ID nSimID);
uint32_t SimSelectApplicationReqV2(uint8_t *AID, uint8_t AIDLength, uint8_t ChannelID, CFW_SIM_ID nSimID);

uint32_t CFW_SimTPDUCommand(uint8_t *TPDU, uint16_t Length, uint8_t Channel, uint16_t nUTI, CFW_SIM_ID nSimID);

uint32_t CFW_SmsSendMessage(CFW_DIALNUMBER *pNumber, uint8_t *pData, uint16_t nDataSize, uint16_t nUTI, CFW_SIM_ID nSimId);
void CFW_InvalideTmsiPTmis(CFW_SIM_ID nSimID);
void CFW_GetUsimAID(uint8_t *aid, uint8_t *length, CFW_SIM_ID nSimID);

bool CFW_SatGetCurrentCmd(uint8_t *nCmd, uint8_t *nQualifier, CFW_SIM_ID nSimID);
uint8_t CFW_SatGetSetupEventList(uint8_t *pEventList, uint8_t *pNum, CFW_SIM_ID nSimID);
void CFW_SatSetEventList(uint32_t nEventList, CFW_SIM_ID nSimID);
uint32_t CFW_SatGetEventList(CFW_SIM_ID nSimID);

bool CFW_GetSATIndFormate(void);
bool CFW_SatGetSendSmsData(uint8_t *pPduData, uint8_t *pDataLen, CFW_SIM_ID nSimID);
bool CFW_SatGetSendSmsSCInfo(uint8_t *pSCAddress, uint8_t *pSCLen, uint8_t *pType, CFW_SIM_ID nSimID);
bool CFW_SatGetRefreshFilesList(uint8_t *pFilesList, uint8_t *pFileNum, CFW_SIM_ID nSimID);
uint32_t CFW_SimRefreshFiles(uint8_t *pFiles, uint8_t nFileNum, uint16_t nUTI, CFW_SIM_ID nSimID);
bool CFW_SimGetLanguage(uint8_t *pLanguage, uint8_t *pNum, CFW_SIM_ID nSimID);
bool CFW_SatGetSetupIdleModeText(uint8_t *pText, uint8_t *pLength, uint8_t *pScheme, CFW_SIM_ID nSimID);
bool CFW_SatGetPlayToneText(uint8_t *pText, uint8_t *pLength, CFW_SIM_ID nSimID);
bool CFW_SatGetPlayToneDuration(uint8_t *pTone, uint8_t *pUnit, uint8_t *pInterval, CFW_SIM_ID nSimID);
bool CFW_SatGetPollIntervalDuration(uint8_t *pUnit, uint8_t *pInterval, CFW_SIM_ID nSimID);
bool CFW_SatGetTimerManagement(uint8_t *pTimerID, uint8_t *pHour, uint8_t *pMinute, uint8_t *pSecond, CFW_SIM_ID nSimID);
bool CFW_SatGetReceiveData(uint8_t *pChannelDataLength, CFW_SIM_ID nSimID);
bool CFW_SatGetSendData(uint8_t *pData, uint8_t *pDataLength, CFW_SIM_ID nSimID);

bool CFW_SatGetOpenChannelUserInfo(uint8_t *pUser, uint8_t *pUserLen, uint8_t *pPasswd, uint8_t *pPasswdLen, CFW_SIM_ID nSimID);
bool CFW_SatGetOpenChannelAPN(uint8_t *pAPN, uint8_t *pApnLen, CFW_SIM_ID nSimID);
bool CFW_SatGetOpenChannelDestAddress(uint8_t *pAddress, uint8_t *pAddressLen, uint8_t *pAddressType, CFW_SIM_ID nSimID);
bool CFW_SatGetOpenChannelNetInfoV2(uint16_t *pBufferSize, uint8_t *pBearerType, uint8_t *pTranType, uint16_t *pTranPort, CFW_SIM_ID nSimID);
bool CFW_SatGetOpenChannelNetInfo(uint8_t *pBufferSize, uint8_t *pBearerType, uint8_t *pTranType, uint8_t *pTranPort, CFW_SIM_ID nSimID);
bool CFW_SatGetDisplayText(uint8_t *pText, uint8_t *pTextLength, uint8_t *nScheme, CFW_SIM_ID nSimID);

uint32_t CFW_SimGetEID(uint16_t nUTI, CFW_SIM_ID nSimID);
uint32_t CFW_GprsHostAddress(uint8_t *pIPAddress, uint8_t *nLength, uint8_t *nIPType, uint8_t nCid, CFW_SIM_ID nSimID);

uint32_t SimSendStatusReq(uint8_t nMode, CFW_SIM_ID nSimID);

#define CFW_MAX_IMSI_ARR_LEN 8
#ifdef CONFIG_QUEC_PROJECT_FEATURE
//CID 4287051,4287049,:
//该宏在atSimlockGetNetworkLocks, atSimlockGetNetworkSubsetLocks, atSimlockGetSPLocks, atSimlockGetCorporateLocks, atSimlockGetUserLocks 中使用.
//可能会导致pSimLockData->user_locks.locks[i]越界,需要修改为NETWORK_LOCKS_T结构体中locks成员数组的大小.
#define CFW_MAX_PERSONALISATIONS 3   
#else
#define CFW_MAX_PERSONALISATIONS 10
#endif

typedef struct _CFW_SIM_LOCK_PLMN_T
{
    uint16_t mcc;
    uint16_t mnc;
    uint16_t mnc_digit_num;
    uint16_t pending;
} CFW_SIM_LOCK_PLMN_T;

typedef struct _CFW_NETWORK_LOCKS_T
{
    uint8_t numLocks;
    uint8_t reserved; // Byte alignment
    uint8_t pending[2];
    CFW_SIM_LOCK_PLMN_T locks[CFW_MAX_PERSONALISATIONS];
} CFW_NETWORK_LOCKS_T;

typedef struct _CFW_NETWORK_SUBSET_LOCK_DATA_T
{
    CFW_SIM_LOCK_PLMN_T plmn;
    uint8_t network_subset[2]; // IMSI digits 6 and 7
    uint8_t pending[2];
} CFW_NETWORK_SUBSET_LOCK_DATA_T;

typedef struct _CFW_NETWORK_SUBSET_LOCKS_T
{
    uint8_t numLocks;
    uint8_t reserved; // Byte alignment
    uint8_t pending[2];
    CFW_NETWORK_SUBSET_LOCK_DATA_T locks[CFW_MAX_PERSONALISATIONS];
} CFW_NETWORK_SUBSET_LOCKS_T;

typedef struct _CFW_SP_LOCK_DATA_T
{
    CFW_SIM_LOCK_PLMN_T plmn;
    uint8_t sp;       // GID1 1 byte
    uint8_t reserved; // Byte alignment
    uint8_t pending[2];
} CFW_SP_LOCK_DATA_T;

typedef struct _CFW_SP_LOCKS_T
{
    uint8_t numLocks;
    uint8_t reserved; // Byte alignment
    uint8_t pending[2];
    CFW_SP_LOCK_DATA_T locks[CFW_MAX_PERSONALISATIONS];
} CFW_SP_LOCKS_T;

typedef struct _CFW_CORPORATE_LOCK_DATA_T
{
    CFW_SIM_LOCK_PLMN_T plmn;
    uint8_t sp;        // GID1 1 byte
    uint8_t corporate; // GID2 1 byte
    uint8_t pending[2];
} CFW_CORPORATE_LOCK_DATA_T;

typedef struct _CFW_CORPORATE_LOCKS_T
{
    uint8_t numLocks;
    uint8_t reserved; // Byte alignment
    uint8_t pending[2];
    CFW_CORPORATE_LOCK_DATA_T locks[CFW_MAX_PERSONALISATIONS];
} CFW_CORPORATE_LOCKS_T;

typedef struct _CFW_IMSI_T
{
    uint8_t imsi_len;
    uint8_t pending[3];
    uint8_t imsi_val[CFW_MAX_IMSI_ARR_LEN];
} CFW_IMSI_T;

typedef CFW_IMSI_T CFW_USER_LOCK_DATA_T;

typedef struct _CFW_USER_LOCKS2_T
{
    uint8_t numLocks;
    uint8_t pending[3];
    CFW_USER_LOCK_DATA_T locks[3];
} CFW_USER_LOCKS2_T;

typedef struct
{
    uint32_t SIM_lock_status;
    uint32_t max_num_trials;
    CFW_NETWORK_LOCKS_T network_locks;
    CFW_NETWORK_SUBSET_LOCKS_T network_subset_locks;
    CFW_SP_LOCKS_T SP_locks;
    CFW_CORPORATE_LOCKS_T corporate_locks;
    CFW_USER_LOCKS2_T user_locks;
} CFW_SRV_SIMLOCKDATA_T;

bool Cfw_SetTimezone(int8_t timezone);

typedef struct T_RADIO_STATS_RPT_TAG
{
    int16_t signal_power;       //<signal power in centibels>
    int16_t total_power;        //<total power in centibels>
    int16_t cur_TX_power;       //<current TX power level in centibels >
    uint64_t ttl_TX_time;       //<total TX time since last reboot in millisecond>
    uint64_t ttl_RX_time;       //<total RX time since last reboot in millisecond>
    uint32_t last_SIB1_cell_id; //<last SIB1 cell ID>, 0xffffffff is invalid, lsb 28bits are valid
    uint8_t last_ECL_val;       //<last ECL value>
    int16_t last_snr_val;       //<last snr value>
    uint16_t last_earfcn;       //<last earfcn value>
    uint16_t last_pci_val;      //<last pci value>
    int16_t rsrq;               //<rsrq in centibels>
    uint8_t operator_mode;      //<operator mode in MIB>
} CFW_UE_RADIO_STATS_RPT;

typedef struct T_CELL_STATS_RPT_TAG
{
    uint16_t earfcn;      //<earfcn>           absolute radio-frequency channel number
    uint16_t phy_cell_id; //<physical cell id> physical id of the cell
    uint8_t pri_cell;     //<primary cell>     1 indicates the current serving cell, 0xff is invalid
    int16_t rsrp;         //<rsrp>             reference signal received power
    int16_t rsrq;         //<rsrq>             reference signal received quality
    int16_t rssi;         //<rssi>             received signal strength indicator, 0x7fff is invalid
    int16_t snr;          //<snr>              signal to noise ratio, 0x7f is invalid
    int8_t offset;        /*NB-IoT channel number?篓陇??篓庐篓虏EARFCN娄士??篓掳??隆杞柯郝μ�??娄士D篓庐D-0.5隆杞縜篓垄?隆陇?隆锟铰¤娇???篓潞娄士娄食?2隆a?a隆饪★拷篓垄?娄士?娄士隆鑻�                            篓篓??娄獭陇??隆膦÷�!3篓娄??篓潞娄士娄台潞隆锟铰癮3y篓掳?2隆杩￠瓖
                            -20,-18,-16,-14,-12,-10,-8,-6,-4,-2,-1,0,2,4,6,8,10,12,14,16,18?隆鑻�                            */
    uint8_t band;
} CFW_UE_CELL_STATS_RPT;

typedef struct T_BLER_STATS_RPT_TAG
{
    uint16_t rlc_ul_bler;         //<rlc_ul_bler> RLC layer block error rate (uplink). permille
    uint16_t rlc_dl_bler;         //<rlc_dl_bler> RLC layer block error rate (downlink). permille
    uint16_t mac_ul_bler;         //<mac_ul_bler> physical layer block error rate (uplink). permille
    uint16_t mac_dl_bler;         //<mac_dl_bler> physical layer block error rate (downlink).permille
    uint64_t ttl_bytes_tx;        //<total bytes transmitted>
    uint64_t ttl_bytes_rx;        //<total bytes received>
    uint32_t tb_num_tx;           //<transport blocks sent>
    uint32_t tb_num_rx;           //<transport blocks received>
    uint32_t tb_num_retx;         //<transport blocks retransmitted>
    uint32_t ttl_ack_nack_msg_rx; //<total ack/nack messages received>
} CFW_UE_BLER_STATS_RPT;

typedef struct T_UE_THROUPUT_STATS_RPT_TAG
{
    uint32_t rlc_ul_bps; //<rlc_ul> RLC layer throughput (uplink). Integer bps
    uint32_t rlc_dl_bps; //<rlc_dl> RLC layer throughput (downlink). Integer bps
    uint32_t mac_ul_bps; //<mac_ul> physical layer throughput (uplink). Integer bps
    uint32_t mac_dl_bps; //<mac_dl> physical layer throughput (downlink). Integer bps
} CFW_UE_THROUPUT_STATS_RPT;

#define CFW_MAX_CELL_STATS_RPT_NUM 5
typedef struct T_UE_STATS_RPT_TAG
{
    uint8_t cellNum;
    CFW_UE_RADIO_STATS_RPT radioStats;
    CFW_UE_CELL_STATS_RPT cellStats[CFW_MAX_CELL_STATS_RPT_NUM];
    CFW_UE_BLER_STATS_RPT blerStats;
    CFW_UE_THROUPUT_STATS_RPT throughputStats;
} CFW_UE_STATS_RPT;

uint32_t CFW_SetPlmn(CFW_NV_PLMN nNvPlmn, CFW_SIM_ID nSimID);
uint32_t CFW_GetPlmn(CFW_NV_PLMN *nNvPlmn, CFW_SIM_ID nSimID);

uint32_t CFW_SmsListFree(void *pListResult, uint8_t nCount, uint8_t nIndex, CFW_SIM_ID nSimID);
uint16_t CFW_SetRauAfterCallFlag(uint16_t flag);
uint16_t CFW_GetRauAfterCallFlag();

uint32_t CFW_SetNvUpdateFlag(uint32_t nFlag, CFW_SIM_ID nSimID);

#define NV_NW_SIM_PLMN 0
#define NV_NW_SIM_BA_COUNT 1
#define NV_NW_SIM_BA_COUNT_PENDING 2
#define NV_NW_SIM_ARFCE_LIST 3
#define NV_NW_SIM_STORE_LAST 4
#define NV_SMS_SIM_PARAM_MR 5
#define NV_GPRS_SIM_DOWN_SUM 6
#define NV_GPRS_SIM_UP_SUM 7

uint32_t CFW_SetSignalQuatityFirst(uint32_t signalQuatityFirst, CFW_SIM_ID nSimID);
uint32_t CFW_GetSignalQuatityFirst(uint32_t *signalQuatityFirst, CFW_SIM_ID nSimID);

uint32_t CFW_EnableLpsLock(bool nEnable, CFW_SIM_ID nSimID);

#if defined RPM_SUPPORT && defined(CONFIG_SOC_8910)
uint8_t sdb_nvGetDefaultPdnSettingApn(uint8_t *outputApn, uint8_t simFlag);
bool sdb_rpmCheckAllowRebootingPdpActiving(uint8_t *apn, uint32_t apnLen, uint8_t trb, uint8_t addFx, uint32_t sim_index);
bool sdb_rpmAllowPdpActivating(uint8_t *apn, uint32_t apnLen, uint8_t trb, uint32_t sim_index);
uint8_t sdb_rpmGetRpmFlag(uint32_t sim_index);
void sdb_rpmSetRpmFlag(uint8_t rpmFlag, uint32_t sim_index);
void sdb_rpmWriteStatic(uint32_t sim_index);
void sdb_rpmWriteDynamic(uint32_t sim_index);
void sdb_rpmWriteDynamicEx(int32_t BeReBoot, uint32_t sim_index);
void sdb_rpmResetRpmFunction(uint32_t sim_index);
uint8_t sdb_rpmGetRpmN1(uint32_t sim_index);
void sdb_rpmSetRpmN1(uint8_t rpmN1, uint32_t sim_index);
uint8_t sdb_rpmGetRpmT1(uint32_t sim_index);
void sdb_rpmSetRpmT1(uint8_t rpmT1, uint32_t sim_index);
uint8_t sdb_rpmGetRpmFX(uint8_t fx, uint32_t sim_index);
void sdb_rpmSetRpmFX(uint8_t fx, uint8_t rpmValue, uint32_t sim_index);
uint8_t sdb_rpmGetRpmLRX(uint8_t lrx, uint32_t sim_index);
void sdb_rpmSetRpmLRX(uint8_t lrx, uint8_t rpmValue, uint32_t sim_index);
uint8_t sdb_rpmGetRpmCBR1(uint32_t sim_index);
uint8_t sdb_rpmGetRpmCR1(uint32_t sim_index);
void sdb_rpmSetRpmCR1(uint8_t value, uint32_t sim_index);
void sdb_rpmGetRpmCpdpx(uint8_t *apn, uint8_t apnLen, uint8_t *cpdp1, uint8_t *cpdp2, uint8_t *cpdp3, uint8_t *cpdp4, uint32_t sim_index);

uint32_t CFW_RpmNvGetDefaultPdnSettingApn(uint8_t *outputApn, uint8_t *outApnLen, CFW_SIM_ID nSimID);
uint32_t CFW_RpmCheckAllowRebootingPdpActiving(bool *outFlag, uint8_t *apn, uint32_t apnLen, uint8_t trb, uint8_t addFx, CFW_SIM_ID nSimID);
uint32_t CFW_RpmAllowPdpActivating(bool *outFlag, uint8_t *apn, uint32_t apnLen, uint8_t trb, CFW_SIM_ID nSimID);
uint32_t CFW_RpmGetRpmFlag(uint8_t *outFlag, CFW_SIM_ID nSimID);
uint32_t CFW_RpmSetRpmFlag(uint8_t rpmFlag, CFW_SIM_ID nSimID);
uint32_t CFW_RpmWriteStatic(CFW_SIM_ID nSimID);
uint32_t CFW_RpmWriteDynamic(CFW_SIM_ID nSimID);
uint32_t CFW_RpmWriteDynamicEx(int32_t BeReBoot, CFW_SIM_ID nSimID);
//only called while USIM changed or AT configuration updated
uint32_t CFW_RpmResetRpmFunction(CFW_SIM_ID nSimID);
uint32_t CFW_RpmGetRpmN1(uint8_t *rpmN1, CFW_SIM_ID nSimID);
uint32_t CFW_RpmSetRpmN1(uint8_t rpmN1, CFW_SIM_ID nSimID);
uint32_t CFW_RpmGetRpmT1(uint8_t *rpmT1, CFW_SIM_ID nSimID);
uint32_t CFW_RpmSetRpmT1(uint8_t rpmT1, CFW_SIM_ID nSimID);
uint32_t CFW_RpmGetRpmFx(uint8_t *rpmFx, uint8_t fx, CFW_SIM_ID nSimID);
uint32_t CFW_RpmSetRpmFx(uint8_t fx, uint8_t rpmValue, CFW_SIM_ID nSimID);
uint32_t CFW_RpmGetRpmLRX(uint8_t *rpmLRX, uint8_t lrx, CFW_SIM_ID nSimID);
uint32_t CFW_RpmSetRpmLRX(uint8_t lrx, uint8_t rpmValue, CFW_SIM_ID nSimID);
uint32_t CFW_RpmGetRpmCBR1(uint8_t *rpmCBR1, CFW_SIM_ID nSimID);
uint32_t CFW_RpmGetRpmCR1(uint8_t *rpmCR1, CFW_SIM_ID nSimID);
uint32_t CFW_RpmSetRpmCR1(uint8_t value, CFW_SIM_ID nSimID);
uint32_t CFW_RpmGetRpmCpdpx(uint8_t *apn, uint8_t apnLen, uint8_t *cpdp1, uint8_t *cpdp2, uint8_t *cpdp3, uint8_t *cpdp4, CFW_SIM_ID nSimID);
uint32_t CFW_rpmNvGetLastRat(uint8_t *nLastRat, CFW_SIM_ID nSimId);
uint32_t CFW_rpmNvSetLastRat(uint8_t rat, CFW_SIM_ID nSimId);

#endif

typedef enum
{
    CSCLK_NO_PM = 0,
    CSCLK_CHECK_DTR = 1,
    CSCLK_PM1 = 2,
    CSCLK_PM2 = 5,
    CSCLK_PM3 = 9,
    CSCLK_INVALID = 0x7fffffff
} CSCLK_PM_ENUM;

uint32_t CFW_EmodSetPowerMode(CSCLK_PM_ENUM nValue, CFW_SIM_ID nSimID);

#if 1 //NBIOT_POSITION_SUPPORT
typedef struct
{
    uint16_t bearing;
    uint16_t horizontal_speed;
} T_Horizontal_Velocity;

typedef struct
{
    uint8_t d;
    uint8_t vertical_speed;
    uint16_t bearing;
    uint16_t horizontal_speed;
} T_Horizontal_With_Vertical_Velocity;

typedef struct
{
    uint8_t uncertainty_speed;
    uint16_t bearing;
    uint16_t horizontal_speed;
} T_Horizontal_Velocity_With_Uncertainty;

typedef struct
{
    uint8_t d;
    uint8_t vertical_speed;
    uint8_t horizontal_uncertainty_speed;
    uint8_t vertical_uncertainty_speed;
    uint16_t bearing;
    uint16_t horizontal_speed;
} T_Horizontal_With_Vertical_Velocity_Uncertainty;

typedef struct
{
    uint8_t velocity_type;
    union {
        T_Horizontal_Velocity vel0;
        T_Horizontal_With_Vertical_Velocity vel1;
        T_Horizontal_Velocity_With_Uncertainty vel2;
        T_Horizontal_With_Vertical_Velocity_Uncertainty vel3;
    } u;
} T_CFW_Lcs_Molr_Vel_Res;

typedef struct
{
    uint8_t sign_of_latitue;
    uint32_t degrees_of_latitue;
    uint32_t degrees_of_longtitue;
} T_Ellipsoid_Basic_Shape;

typedef T_Ellipsoid_Basic_Shape T_Ellipsoid_Point;

typedef struct
{
    T_Ellipsoid_Basic_Shape basic_shape;
    uint8_t uncertainty_code;

} T_Ellipsoid_Point_With_Uncert_Circle;

typedef struct
{
    T_Ellipsoid_Basic_Shape basic_shape;
    uint8_t uncertainty_semi_major;
    uint8_t uncertainty_semi_minor;
    uint8_t orientation_of_major_axis;
    uint8_t confidence;
} T_Ellipsoid_Point_With_Uncert_Ellipse;

typedef struct
{
    T_Ellipsoid_Basic_Shape basic_shape[15];
} T_Polygon;

typedef struct
{
    T_Ellipsoid_Basic_Shape basic_shape;
    uint8_t d;
    uint16_t altitude;
} T_Ellipsoid_Point_With_Altitude;

typedef struct
{
    T_Ellipsoid_Basic_Shape basic_shape;
    uint8_t d;
    uint16_t altitude;
    uint8_t uncertainty_semi_major;
    uint8_t uncertainty_semi_minor;
    uint8_t orientation_of_major_axis;
    uint8_t uncertainty_altitude;
    uint8_t confidence;
} T_Ellipsoid_Point_With_Alti_Uncert_Ellipse;

typedef struct
{
    T_Ellipsoid_Basic_Shape basic_shape;
    uint16_t inner_radius;
    uint8_t uncertainty_radius;
    uint8_t offset_angle;
    uint8_t included_angle;
    uint8_t confidence;
} T_Ellipsoid_Arc;

typedef struct
{
    uint8_t type_of_shape;
    uint8_t polygon_num_of_points;
    union {
        T_Ellipsoid_Point shape0;
        T_Ellipsoid_Point_With_Uncert_Circle shape1;
        T_Ellipsoid_Point_With_Uncert_Ellipse shape3;
        T_Polygon shape5;
        T_Ellipsoid_Point_With_Altitude shape8;
        T_Ellipsoid_Point_With_Alti_Uncert_Ellipse shape9;
        T_Ellipsoid_Arc shape10;
    } u;
} T_CFW_Lcs_Molr_Loc_Res;

typedef struct
{
} T_CFW_Lcs_Molr_Addloc_Res;

typedef struct
{
    uint8_t bit_mask;
    union {
        T_CFW_Lcs_Molr_Loc_Res loc;
        T_CFW_Lcs_Molr_Addloc_Res addLoc;
    } u;
    T_CFW_Lcs_Molr_Vel_Res vel_res;
} T_CFW_Lcs_Molr_Res;

typedef struct
{
    uint8_t nIndex;
    void *pMsg;
    T_CFW_Lcs_Molr_Res MolrRes;
} CFW_SS_LCS_INFO;

typedef struct _CFW_LCS_MTLR_INFO
{
    uint8_t handle_id;
    uint8_t notification_type;
    uint8_t location_type;
    bool clientExternalIdPresent;
    uint8_t clientExternalId[9];
    bool clientNamePresent;
    uint8_t clientName[63];
} CFW_LCS_MTLR_INFO;

typedef struct _CFW_LCS_MOLR_INFO
{
    uint8_t method;
    uint8_t hor_acc_set;
    uint8_t hor_acc;
    uint8_t ver_req;
    uint8_t ver_acc_set;
    uint8_t ver_acc;
    uint8_t vel_req;
    uint8_t rep_mode;
    uint8_t interval;
    uint8_t shape_rep;
    uint8_t plane;
    uint16_t timeout;
    uint8_t nmea_rep[100];
    uint8_t third_party_address[100];
} CFW_LCS_MOLR_INFO;

uint32_t CFW_LcsMtlrRsp(uint8_t handle_id, uint8_t response, uint16_t nUTI, CFW_SIM_ID nSimID);
uint32_t CFW_LcsMolrReq(CFW_LCS_MOLR_INFO *molrInfo, uint16_t nUTI, CFW_SIM_ID nSimID);

#endif
#define CFW_AUTO_ACT_LTE_CID 5

bool CFW_UpdateNV(bool bForce);
bool CFW_SatGetOpenChannelBearerDes(CFW_GPRS_QOS *Qos, uint8_t *pdp_type, CFW_SIM_ID nSimID);
bool CFW_SatGetChannelID(uint8_t *channel, CFW_SIM_ID nSimID);

typedef struct _CFW_SUSP_FUNC_SELECT
{
    uint8_t nMaxDurTimeUnit;
    uint8_t nMaxDurLenOfTime;
    uint8_t nMinDurTimeUnit;
    uint8_t nMinDurLenOfTime;
} CFW_SUSP_FUNC_SELECT;

uint32_t CFW_SimSetResume(uint8_t *nResumeToken, uint16_t nUTI, CFW_SIM_ID nSimID);
uint32_t CFW_SimSetSuspend(CFW_SUSP_FUNC_SELECT *pSelParam, uint16_t nUTI, CFW_SIM_ID nSimID);

uint32_t CFW_nvSetCopsTimer(uint8_t nValue);
uint32_t CFW_nvGetCopsTimer(void);

#ifdef CONFIG_SOC_8811
uint32_t CFW_AutoGprsAct(CFW_SIM_ID nSim, uint8_t uCid_set);
#endif

#ifndef CONFIG_SOC_8811
/*! \brief function retrieves the loss Cover Timer value.

\return \a loss Cover Timer value
*/
uint32_t CFW_GetLCTimer(void);

/*! \brief function set the loss Cover Timere value.

\param [in]  value loss Cover Timer value.
\return \a ERR_SUCCESS on success
*/
uint32_t CFW_SetLCTimer(
    uint32_t value);
/*! \brief function retrieves the loss Cover Timer value.

\return \a loss Cover Timer value
*/
uint32_t CFW_GetLCTimerEx(
    CFW_SIM_ID nSimID);

/*! \brief function set the loss Cover Timere value.

\param [in]  value loss Cover Timer value.
\return \a ERR_SUCCESS on success
*/
uint32_t CFW_SetLCTimerEx(
    uint32_t value,
    CFW_SIM_ID nSimID);

/*! \brief function retrieves the limite timer value.

\return \a limite timer value
*/
uint32_t CFW_GetLimiteTimer(
    CFW_SIM_ID nSimID);

/*! \brief function set the limite timer value.

\param [in]  value limite timer value.
\return \a ERR_SUCCESS on success
*/
uint32_t CFW_SetLimiteTimer(
    uint32_t value,
    CFW_SIM_ID nSimID);

/*! \brief function retrieves the lossCoverTiParam loopCout value.

\return \a lossCoverTiParam loopCout value
*/
uint32_t CFW_GetLctLoopCout(
    CFW_SIM_ID nSimID);

/*! \brief function set the lossCoverTiParam loopCout value.

\param [in]  value lossCoverTiParam loopCout value.
\return \a ERR_SUCCESS on success
*/
uint32_t CFW_SetLctLoopCout(
    uint32_t value,
    CFW_SIM_ID nSimID);

/*! \brief function retrieves the limitedTiParam loopCout  value.

\return \a limitedTiParam loopCout  value
*/
uint32_t CFW_GetLmtLoopCout(
    CFW_SIM_ID nSimID);

/*! \brief function set the limitedTiParam loopCout value.

\param [in]  value limitedTiParam loopCout value.
\return \a ERR_SUCCESS on success
*/
uint32_t CFW_SetLmtLoopCout(
    uint32_t value,
    CFW_SIM_ID nSimID);

/*! \brief function retrieves the lossCoverTiParam maxLen value.

\return \a lossCoverTiParam maxLen value
*/
uint32_t CFW_GetLctMaxLen(
    CFW_SIM_ID nSimID);

/*! \brief function set the lossCoverTiParam maxLen value.

\param [in]  value lossCoverTiParam maxLen value.
\return \a ERR_SUCCESS on success
*/
uint32_t CFW_SetLctMaxLen(
    uint32_t value,
    CFW_SIM_ID nSimID);

/*! \brief function retrieves the limitedTiParam maxLen  value.

\return \a limitedTiParam maxLen  value
*/
uint32_t CFW_GetLmtMaxLen(
    CFW_SIM_ID nSimID);

uint32_t CFW_NwSetNetTAU(
    CFW_SIM_ID nSimID);
/*! \brief function set the limitedTiParam maxLen value.

\param [in]  value limitedTiParam maxLen value.
\return \a ERR_SUCCESS on success
*/
uint32_t CFW_SetLmtMaxLen(
    uint32_t value,
    CFW_SIM_ID nSimID);
#endif

uint8_t CFW_GprsGetNsapi(uint8_t nCid, CFW_SIM_ID nSimID);

#ifdef CONFIG_AT_CUS_SUPPORT
uint32_t CFW_CfgGcSetSMS(CFW_COMM_MODE bMode, CFW_SIM_ID nSimID);
uint32_t CFW_CfgGcGetSMS(CFW_COMM_MODE *bMode, CFW_SIM_ID nSimID);
uint32_t CFW_CfgGcSetBip(CFW_COMM_MODE bMode, CFW_SIM_ID nSimID);
uint32_t CFW_CfgGcGetBip(CFW_COMM_MODE *bMode, CFW_SIM_ID nSimID);
#endif

//arbitration module
typedef struct _CFW_WIFISCAN_AP_RESULT
{
    uint32_t bssid_low;  ///< mac address low
    uint16_t bssid_high; ///< mac address high
    uint8_t channel;     ///< channel id
    int8_t rssival;      ///< signal strength
    uint16_t reserved;
    uint16_t ssidlen; // ssid len, unit byte
    uint8_t ssid[32];
} CFW_WIFISCAN_AP_RESULT;

typedef struct _CFW_WIFISCAN_INFO
{
    uint16_t eventCount;
    uint16_t finished;
    uint16_t wifiApNum; //number of AP(s) that have been scanned.
    uint16_t maxApNum;
    CFW_WIFISCAN_AP_RESULT *wifiScanResult;
} CFW_WIFISCAN_INFO;

typedef struct _CFW_WIFISCAN_REQ
{
    bool wswitch;         //start or stop wifiscan
    uint16_t nChNum;      // number of channels scanned
    uint16_t nMaxNodeNum; //maximum number of scan
    uint16_t nCh[13];     //channel id (1-13)
} CFW_WIFISCAN_REQ;

/**
 * @brief wifiscan start api, wifi will be automatically turned off after scanning.
 *
 * @param wifiscan_param: struct from CFW_WIFISCAN_REQ.
 * @param nUTI: UTI operation.
 * @param nSim: sim index.
 *
 * @return 1 success, others: fail
 */
uint32_t CFW_WifiScanStart(CFW_WIFISCAN_REQ wifiscan_param, uint16_t nUTI, CFW_SIM_ID nSimID);

/**
 * @brief wifiscan stop api.
 *
 * @param nUTI: UTI operation.
 * @param nSim: sim index.
 *
 * @return 1 success, others: fail
 */
uint32_t CFW_WifiScanStop(uint16_t nUTI, CFW_SIM_ID nSimID);

/**
 * @brief Interface for handshaking with LTE, 
 *            When satrting GNSS service, 
 *            LTE needs to be notified with this interface.
 *
 * @param nServiceType: 0 hot gnss service, 1 cold gnss service
 * @param nUTI: UTI operation.
 * @param nSim: sim index.
 *
 * @return 1 success, others: fail
 */
uint32_t CFW_GnssStartHandshakeStack(uint8_t nServiceType, uint16_t nUTI, CFW_SIM_ID nSimID);

/**
 * @brief Interface for handshaking with LTE, 
 *            When stoping GNSS service, 
 *            LTE needs to be notified with this interface.
 *
 * @param nUTI: UTI operation.
 * @param nSim: sim index.
 *
 * @return 1 success, others: fail
 */
uint32_t CFW_GnssStopHandshakeStack(uint16_t nUTI, CFW_SIM_ID nSimID);

/**
 * @brief When the WiFi scan is completed, 
 *            this function is used to obtain the scan result.
 *            Each AP contains a set of scan result.
 *     Be sure to use this function to obtain the scan results. 
 *     Memory leakage will occur on the no side.(This function 
 *     will release the memory occupied by the scan results)
 *
 * @param listAddr: The first address whrer the scan results are stored.
 * @param index: Number of AP(s) scanned.
 * @param end: Identify whether it is the last AP, 1 yes, 0 no.
 * @param scan_result: Hotspot information contained in an AP.
 *
 * @return 1 listAddr is null, 0: get AP information
 */
uint32_t CFW_LocGetWifiscanRes(void *listAddr, uint8_t index, uint8_t end, CFW_WIFISCAN_AP_RESULT *scan_result);

/**
 * @brief Set GNSS and LTE service priority.
 *         After setting, you need to restart to take effect.
 *
 * @param locPriority:
 *          0: gnss cold > lte signal > gnss hot > lte data
 *          1: gnss cold > lte signal > lte data > gnss hot
 *          2: gnss cold > gnss hot > lte signal > lte data
 *          3: lte signal > lte data > gnss cold > gnss hot
 * @param nSim: sim index.
 *
 * @return 1 success, others: fail
 */
void CFW_nvSetLocPriority(uint8_t locPriority, CFW_SIM_ID nSimID);

/**
 * @brief Set GNSS and LTE service priority.
 *         During setting, it's required to ensure that GNSS is off.
 *
 * @param locPriority:
 *          0: gnss cold > lte signal > gnss hot > lte data
 *          1: gnss cold > lte signal > lte data > gnss hot
 *          2: gnss cold > gnss hot > lte signal > lte data
 *          3: lte signal > lte data > gnss cold > gnss hot
 * @param nSim: sim index.
 *
 * @return 1 success, others: fail
 */
uint32_t CFW_nvSetLocPriority_V2(uint8_t locPriority, uint16_t nTUI, CFW_SIM_ID nSimID);

/**
 * @brief Get GNSS and LTE service priority.
 *
 * @param nSim: sim index.
 *
 * @return GNSS and LTE service Priority value.
 */
uint8_t CFW_nvGetLocPriority(CFW_SIM_ID nSimID);

//wifi sensitivity test
#define AT_WIFI_SENTEST_MAX_CNT 10
typedef struct _CFW_WIFISENTEST_RES
{
    uint8_t mode;                             //0: signle mod 1:con mod
    uint8_t ch_id;                            // channel no
    uint16_t loop_cnt;                        // for con mod,  executed loop cnt
    uint16_t max_loop;                        // for con mod, max loop cnt
    uint16_t result[AT_WIFI_SENTEST_MAX_CNT]; // test result (beacon frame counter), for single mod, the result fixed in u16_result[0]
    uint16_t resvered;
} CFW_WIFISENTEST_RES;

typedef struct _CFW_WIFISENTEST_PARAM
{
    uint8_t mode;       //0: signle mod 1:con mod
    uint8_t ch_id;      // channel no
    uint16_t loop_time; //unit ms,  one loop duration
    uint16_t max_loop;  //max loop cnt
    uint16_t resvered;
} CFW_WIFISENTEST_PARAM;

void CFW_WifiSentestOn(void);
void CFW_WifiSentestOff(void);
uint32_t CFW_WifiSentestSetParam(CFW_WIFISENTEST_PARAM *sentest_cfg);
uint32_t CFW_WifiSentestGetSingleRes(CFW_WIFISENTEST_RES *sentest_res);

uint32_t CFW_nvSetNasTimer(uint8_t nType, uint8_t nName, uint16_t nValue, CFW_SIM_ID nSimID);
uint32_t CFW_nvGetNasTimer(uint8_t nType, uint8_t nName, CFW_SIM_ID nSimID);
uint32_t CFW_nvSetNwSearchPrior(uint8_t mode, uint32_t enable, uint8_t bandNum, uint8_t *bandList, CFW_SIM_ID nSimID);
uint32_t CFW_nvGetNwSearchPrior(uint32_t *enable, uint8_t *bandNum, uint8_t *bandList, CFW_SIM_ID nSimID);

uint32_t CFW_SimPollReq(uint32_t nTimerOut, CFW_SIM_ID nSimID);
uint32_t CFW_NwSetLocalRelease(CFW_SIM_ID nSimID);

bool CFW_SatGetRawData(uint8_t *pRawData, uint16_t *nRawDataLen, CFW_SIM_ID nSimID);
uint32_t CFW_SatTerminalResponse(uint8_t *data, uint16_t length, uint16_t nUTI, CFW_SIM_ID nSimID);
uint32_t CFW_SatTerminalEnvelope(uint8_t *data, uint16_t length, uint16_t nUTI, CFW_SIM_ID nSimID);

bool CFW_SatGetMenuTitle(uint8_t *title, uint8_t *title_length, CFW_SIM_ID nSimID);
bool CFW_SatGetMenuItemByID(uint8_t item_id, uint8_t *item, uint8_t *item_length, CFW_SIM_ID nSimID);
bool CFW_SatGetMenuCurrentItem(uint8_t *item, uint8_t *item_length, uint8_t *item_id, CFW_SIM_ID nSimID);
bool CFW_SatGetMenuFirstItem(uint8_t *item, uint8_t *item_length, uint8_t *item_id, CFW_SIM_ID nSimID);
bool CFW_SatGetMenuNextItem(uint8_t *item, uint8_t *item_length, uint8_t *item_id, CFW_SIM_ID nSimID);

bool CFW_SatGetSubMenuTitle(uint8_t *title, uint8_t *title_length, CFW_SIM_ID nSimID);
bool CFW_SatGetSubMenuItemByID(uint8_t item_id, uint8_t *item, uint8_t *item_length, CFW_SIM_ID nSimID);
bool CFW_SatGetSubMenuCurrentItem(uint8_t *item, uint8_t *item_length, uint8_t *item_id, CFW_SIM_ID nSimID);
bool CFW_SatGetSubMenuFirstItem(uint8_t *item, uint8_t *item_length, uint8_t *item_id, CFW_SIM_ID nSimID);
bool CFW_SatGetSubMenuNextItem(uint8_t *item, uint8_t *item_length, uint8_t *item_id, CFW_SIM_ID nSimID);

bool CFW_SatGetSendSmsServiceCenterAddr(uint8_t *addr, uint8_t *addr_length, uint8_t *addr_type, CFW_SIM_ID nSimID);
bool CFW_SatGetSendSmsTPDU(uint8_t *tpdu, uint8_t *tpdu_length, CFW_SIM_ID nSimID);
bool CFW_SatGetSendSmsAlpha(uint8_t *alpha, uint8_t *alpha_length, CFW_SIM_ID nSimID);

bool CFW_SatGetDisplayText(uint8_t *text, uint8_t *text_length, uint8_t *dcs, CFW_SIM_ID nSimID);
bool CFW_SatGetGetInKeyText(uint8_t *text, uint8_t *text_length, uint8_t *dcs, CFW_SIM_ID nSimID);

bool CFW_SatGetGetInputText(uint8_t *text, uint8_t *text_length, uint8_t *dcs, CFW_SIM_ID nSimID);
bool CFW_SatGetGetInputLengthRange(uint8_t *max_len, uint8_t *min_len, CFW_SIM_ID nSimID);
bool CFW_SatGetGetInputDefaultText(uint8_t *text, uint8_t *text_length, uint8_t *dcs, CFW_SIM_ID nSimID);

//use for stage parameter
#define SAT_SETUP_CALL_CONFIRM_TEXT 0x01
#define SAT_SETUP_CALL_SETUP_TEXT 0x02
bool CFW_SatGetSetupCallText(uint8_t stage, uint8_t *text, uint8_t *text_length, CFW_SIM_ID nSimID);
//use for which parameter
#define SAT_SETUP_CALL_ADDRESS 0x01
#define SAT_SETUP_CALL_SUBADDRESS 0x02
bool CFW_SatGetSetupCallAddr(uint8_t which, uint8_t *addr, uint8_t *addr_length, uint8_t *addr_type, CFW_SIM_ID nSimID);
bool CFW_SatGetSetupCallCapCfg(uint8_t *cfg, uint8_t *cfg_length, uint8_t *unit, uint8_t *interval, CFW_SIM_ID nSimID);

bool CFW_SatGetSendSSDisplayInfo(uint8_t *text, uint8_t *text_length, uint8_t *icon_identifier,
                                 uint8_t *icon_qualifier, CFW_SIM_ID nSimID);
bool CFW_SatGetSendSSInfo(uint8_t *ss, uint8_t *ss_length, uint8_t *ss_type, CFW_SIM_ID nSimID);

bool CFW_SatGetSendUSSDDisplayInfo(uint8_t *text, uint8_t *text_length, uint8_t *icon_identifier,
                                   uint8_t *icon_qualifier, CFW_SIM_ID nSimID);
bool CFW_SatGetSendUSSDInfo(uint8_t *ussd, uint8_t *ussd_length, uint8_t *dcs, CFW_SIM_ID nSimID);

bool CFW_SatGetSendDTMFInfo(uint8_t *text, uint8_t *text_length, uint8_t *dtmf, uint8_t *dtmf_length, CFW_SIM_ID nSimID);
bool CFW_SatGetSendDTMFText(uint8_t *text, uint8_t *text_length, CFW_SIM_ID nSimID);
bool CFW_SatGetSendDTMF(uint8_t *dtmf, uint8_t *dtmf_length, CFW_SIM_ID nSimID);

bool CFW_SatGetSetupIdleMode(uint8_t *mode_text, uint8_t *mode_text_length, uint8_t *dcs,
                             uint8_t *icon_identifier, uint8_t *icon_qualifier, CFW_SIM_ID nSimID);
bool CFW_SatGetLanguageNotification(uint8_t language[2], CFW_SIM_ID nSimID);
bool CFW_SatGetPlayTone(uint8_t *tone, uint8_t *unit, uint8_t *interval, CFW_SIM_ID nSimID);

#define UICC_TYPE_SIM 0
#define UICC_TYPE_USIM 1
uint32_t CFW_SimSetMeProfile(CFW_PROFILE *pMeProfile, uint8_t uicc_type);
#define UICC_SAT_ENABLE 0
#define UICC_SAT_DISABLE 1
uint32_t CFW_SatSetFacility(uint8_t status, CFW_SIM_ID nSimID);
uint8_t CFW_SatGetFacility(CFW_SIM_ID nSimID);
uint32_t CFW_StackInit(CFW_SIM_ID nSimID);

#ifdef FUNC_SUPPORT_SIMLOCK
#define CFW_SIM_LOCK_ENCRYPTED_KEY_MAX_LEN 32 /* MAX length of control key in encrypted data format */

typedef struct
{
    uint8_t nck[CFW_SIM_LOCK_ENCRYPTED_KEY_MAX_LEN];
    uint8_t nsck[CFW_SIM_LOCK_ENCRYPTED_KEY_MAX_LEN];
    uint8_t spck[CFW_SIM_LOCK_ENCRYPTED_KEY_MAX_LEN];
    uint8_t cck[CFW_SIM_LOCK_ENCRYPTED_KEY_MAX_LEN];
    uint8_t pck[CFW_SIM_LOCK_ENCRYPTED_KEY_MAX_LEN];
} cfw_simlock_encrypt_keyinfo_t;

typedef struct
{
    cfw_simlock_encrypt_keyinfo_t pin_keys;
    cfw_simlock_encrypt_keyinfo_t puk_keys;
} cfw_simlock_encrypt_keys_t;

uint32_t CFW_SimlockDataProcess(char *pData, uint16_t nDataSize, uint8_t nFlag, CFW_SIM_ID nSimID);
uint32_t CFW_SimlockEncryptKeys(char *pData, uint16_t nDataSize, char encrypt_data[800], uint16_t en_len, CFW_SIM_ID nSimID);
uint32_t CFW_SimlockImeiProcess(char *pData, uint16_t nDataSize, uint8_t nFlag, uint16_t nUTI, CFW_SIM_ID nSimID);
uint32_t CFW_SimlockSetHuk(char *pData, uint16_t nDataSize, CFW_SIM_ID nSimID);
uint32_t CFW_SimlockFacauthPubkey(char *pubkey, uint16_t nPubkeySize, char *sign, uint16_t nSignSize, CFW_SIM_ID nSimID);
uint32_t CFW_SimlockAuthStart(char data[512], uint32_t *len, CFW_SIM_ID nSimID);
uint32_t CFW_SimlockAuthEnd(char *data, uint32_t len, CFW_SIM_ID nSimID);
uint32_t CFW_SimlockToolGetUid(uint8_t uidData[128], uint8_t len, CFW_SIM_ID nSimID);
uint32_t CFW_SimlockDataCheckUnlock(CFW_SIM_ID nSimID);
uint32_t CFW_SimlockDataCheckLock(char *data, uint16_t len, uint8_t nFlag, CFW_SIM_ID nSimID);
uint32_t CFW_SimlockImeiVerify(uint16_t nUTI, CFW_SIM_ID nSimID);
uint32_t CFW_SimlockSetVersion(char *data, uint16_t len, CFW_SIM_ID nSimID);
#endif

uint32_t CFW_SetMoSmsRetryCount(uint8_t smc_count, uint8_t smr_count, CFW_SIM_ID nSimID);
uint32_t CFW_CbSetType(uint8_t nCbType, CFW_SIM_ID nSimID);

bool CFW_DecodeCDMAImsi(uint8_t *pImsi, uint8_t *pCdmaImsiM);
uint32_t CFW_SimGetCdmaImsi(uint16_t nUTI, CFW_SIM_ID nSimID);

typedef struct
{
    uint16_t arfcn;
    uint8_t lai[5];
    uint8_t bsic;
} cfw_GsmCell_t;

typedef struct
{
    uint8_t num;
    cfw_GsmCell_t BlackCellList[CFW_MAX_BLACK_CELL_NUM]; //synchronize with protocol stack
} CFW_NW_BLKLIST_GSM_INFO;

uint32_t CFW_CusPrivatePsm(uint8_t flag, uint16_t time, CFW_SIM_ID nSimID);

typedef struct _CFW_NET_DL_PSM_PARAM
{
    bool rrcConnection;
    uint32_t t3412Len;
    uint32_t t3412RemainLen;
    uint32_t t3324Len;
    uint32_t t3324RemainLen;
} CFW_NET_DL_PSM_PARAM_T;

uint32_t CFW_GetNetDLPsmParam(uint16_t nUti, CFW_SIM_ID nSimID);

#define CFW_MAX_TAC_NUM 2

typedef enum _CFW_ENG_EVT_RPT_ENUM
{
    CFW_ENG_EVENT_INIT_ACC_RPT,
    CFW_ENG_EVENT_MAC_ACC_FAIL_RPT,
    CFW_ENG_EVENT_T310_EXPIRE_RPT,
    CFW_ENG_EVENT_RLC_MAX_NUM_RETRAN_RPT,
    CFW_ENG_EVENT_RRC_STATUS_RPT,

    CFW_ENG_EVENT_RRC_EST_REQ_RPT,
    CFW_ENG_EVENT_RRC_EST_FAIL_RPT,

    CFW_ENG_EVENT_INTRA_LTE_HO_REQ_RPT,
    CFW_ENG_EVENT_INTRA_LTE_HO_FAIL_RPT,
    CFW_ENG_EVENT_INTRA_LTE_HO_SUCC_RPT,

    CFW_ENG_EVENT_IRAT_HO_REQ_RPT,
    CFW_ENG_EVENT_IRAT_HO_FAIL_RPT,
    CFW_ENG_EVENT_IRAT_HO_SUCC_RPT,
    CFW_ENG_EVENT_IRAT_CCO_REQ_RPT,
    CFW_ENG_EVENT_IRAT_CCO_FAIL_RPT,
    CFW_ENG_EVENT_IRAT_CCO_SUCC_RPT,
    CFW_ENG_EVENT_IRAT_REDIREC_REQ_RPT,
    CFW_ENG_EVENT_IRAT_REDIREC_FAIL_RPT,
    CFW_ENG_EVENT_IRAT_REDIREC_SUCC_RPT,

    CFW_ENG_EVENT_IRAT_GSM_CCO_LTE_REQ,
    CFW_ENG_EVENT_IRAT_GSM_CCO_LTE_FAIL,
    CFW_ENG_EVENT_IRAT_GSM_CCO_LTE_SUCCESS,
    CFW_ENG_EVENT_IRAT_GSM_REDIREC_REQ_RPT,
    CFW_ENG_EVENT_IRAT_GSM_REDIREC_FAIL_RPT,
    CFW_ENG_EVENT_IRAT_GSM_REDIREC_SUCC_RPT,
} CFW_ENG_EVT_RPT_ENUM_T;

typedef enum _CFW_EVENT_RPT_MODE_TYPE
{
    CFW_ENG_EVENT_RPT_MODE_TYPE_LTE,
    CFW_ENG_EVENT_RPT_MODE_TYPE_GSM
} CFW_EVENT_RPT_MODE_TYPE;

typedef enum _CFW_EVENT_RPT_RRC_STATUS
{
    CFW_ENG_EVENT_RPT_RRC_RELEASED,
    CFW_ENG_EVENT_RPT_RRC_CONNECTED
} CFW_EVENT_RPT_RRC_STATUS;

typedef enum _CFW_EVENT_RRC_EST_FAIL_CAUSE
{
    CFW_ENG_EVENT_RRC_EST_FAIL_ACC_BAR,
    CFW_ENG_EVENT_RRC_EST_FAIL_T300_EXPIRE,
    CFW_ENG_EVENT_RRC_EST_FAIL_ABORT,
    CFW_ENG_EVENT_RRC_EST_FAIL_REJECT
} CFW_EVENT_RRC_EST_FAIL_CAUSE;

typedef struct _CFW_PLMN_TAG
{
    uint8_t mcc[2]; //Mobile Colour Code (octet 1, octet 2 bits 1 to 4)
    uint8_t mnc;    //Mobile Network Code (octet 3, octet 2bits 5 to 8);
    uint8_t reserved;
} CFW_PLMN_TAG;

typedef struct _CFW_ENGEVTRPT_CELL_INFO_Tag
{
    uint8_t tac[CFW_MAX_TAC_NUM];
    uint16_t pci;
    uint32_t cellIdentity;
    uint32_t dlEarfcn;
    CFW_PLMN_TAG plmn;
} CFW_ENGEVTRPT_CELL_INFO;

typedef struct CFW_ENGEVTRPT_INITACC_RPT_Tag
{
    CFW_EVENT_RPT_MODE_TYPE mode;
    uint16_t accResult;
    uint16_t reserved;
    CFW_ENGEVTRPT_CELL_INFO cellInfo;
} CFW_ENGEVTRPT_INITACC_RPT;

typedef struct CFW_ENGEVTRPT_MACACCFAIL_RPT_Tag
{
    CFW_ENGEVTRPT_CELL_INFO cellInfo;
} CFW_ENGEVTRPT_MACACCFAIL_RPT;

typedef struct CFW_ENGEVTRPT_T310EXPI_RPT_Tag
{
    CFW_ENGEVTRPT_CELL_INFO cellInfo;
} CFW_ENGEVTRPT_T310EXPI_RPT;

typedef struct CFW_ENGEVTRPT_RLCMAXRETRAN_RPT_Tag
{
    CFW_ENGEVTRPT_CELL_INFO cellInfo;
} CFW_ENGEVTRPT_RLCMAXRETRAN_RPT;

typedef struct CFW_ENGEVTRPT_RRCSTATUS_RPT_Tag
{
    CFW_EVENT_RPT_RRC_STATUS rrcStatus;
    CFW_ENGEVTRPT_CELL_INFO cellInfo;
} CFW_ENGEVTRPT_RRCSTATUS_RPT;

typedef struct CFW_ENGEVTRPT_RRCESTREQ_RPT_Tag
{
    CFW_ENGEVTRPT_CELL_INFO cellInfo;
} CFW_ENGEVTRPT_RRCESTREQ_RPT;

typedef struct CFW_ENGEVTRPT_RRCESTFAIL_RPT_Tag
{
    CFW_EVENT_RRC_EST_FAIL_CAUSE failCause;
    CFW_ENGEVTRPT_CELL_INFO cellInfo;
} CFW_ENGEVTRPT_RRCESTFAIL_RPT;

typedef struct CFW_ENGEVTRPT_INTRALTE_HO_RPT_Tag
{
    uint16_t targetPci;
    uint16_t reserved;
    uint32_t targetDlEarfcn;
    CFW_ENGEVTRPT_CELL_INFO orgCellInfo;
} CFW_ENGEVTRPT_INTRALTE_HO_RPT;

#define CFW_MAX_REDIRECTION_GSM_CELL_NUM 32

typedef struct _CFW_ENGEVTRPT_IRAT_ORGLTE_INFO
{
    uint8_t mode;
    uint8_t reserved[3];
    CFW_ENGEVTRPT_CELL_INFO cellInfo;
} CFW_ENGEVTRPT_IRAT_ORGLTE_INFO;

typedef struct _CFW_ENGEVTRPT_IRAT_TARGETGSM_INFO
{
    uint8_t mode;
    uint8_t gsmBandIndicator;
    uint8_t bsic;
    uint8_t reserved;
    uint16_t dlArfcn;
    uint8_t reserved1[2];
} CFW_ENGEVTRPT_IRAT_TARGETGSM_INFO;

typedef struct _CFW_ENGEVTRPT_IRAT_REDIREC_GSMCELL_INFO
{
    uint8_t bsic;
    uint8_t nccPermitted;
    uint16_t arfcn;
} CFW_ENGEVTRPT_IRAT_REDIREC_GSMCELL_INFO;

typedef struct _CFW_ENGEVTRPT_IRAT_REDIREC_TARGETGSM_LIST
{
    uint8_t mode;
    uint8_t gsmCellNum;
    uint16_t reserved;
    CFW_ENGEVTRPT_IRAT_REDIREC_GSMCELL_INFO gsmCell[CFW_MAX_REDIRECTION_GSM_CELL_NUM];
} CFW_ENGEVTRPT_IRAT_REDIREC_TARGETGSM_LIST;

typedef struct CFW_ENGEVTRPT_IRAT_L2G_RPT_Tag
{
    CFW_ENGEVTRPT_IRAT_ORGLTE_INFO orgLteCellInfo;
    CFW_ENGEVTRPT_IRAT_TARGETGSM_INFO targetGsmCellInfo;
} CFW_ENGEVTRPT_IRAT_L2G_RPT;

typedef struct CFW_ENGEVTRPT_IRAT_L2G_REDIREC_RPT_Tag
{
    CFW_ENGEVTRPT_IRAT_ORGLTE_INFO orgLteCellInfo;
    CFW_ENGEVTRPT_IRAT_REDIREC_TARGETGSM_LIST targetGsmCellList;
} CFW_ENGEVTRPT_IRAT_L2G_REDIREC_RPT;

typedef struct
{
    uint8_t Arfcn;
    uint8_t Plmn[3];
    uint8_t BSIC;
    uint8_t LAC[2];
    uint8_t CellId[2];
} cfw_gsmSouceCellInfo_t;

typedef struct
{
    uint16_t Arfcn;
    uint8_t Plmn[3];
    uint8_t BSIC;
    uint8_t LAC[2];
    uint8_t CellId[2];
} cfw_gsmSouceCellInfo_v2_t;

typedef struct
{
    uint32_t Earfcn;
    uint32_t Pci;
} cfw_lteTargetCellInfo_t;

#define CFW_MAX_LTE_CELL_NUM 16

typedef struct
{
    cfw_gsmSouceCellInfo_t GsmSourceCell;
    uint8_t NumOfTargetCell;
    cfw_lteTargetCellInfo_t LteTargetCell[CFW_MAX_LTE_CELL_NUM];
} cfw_GsmChangeToLteInd_t;

typedef struct
{
    cfw_gsmSouceCellInfo_v2_t GsmSourceCell;
    uint8_t NumOfTargetCell;
    cfw_lteTargetCellInfo_t LteTargetCell[CFW_MAX_LTE_CELL_NUM];
} cfw_GsmChangeToLteInd_v2_t;

typedef struct
{
    uint8_t eventId;
    uint8_t reserved[3];
    union {
        CFW_ENGEVTRPT_INITACC_RPT initAccInfo;
        CFW_ENGEVTRPT_MACACCFAIL_RPT macAccFailInfo;
        CFW_ENGEVTRPT_T310EXPI_RPT t310ExpiredInfo;
        CFW_ENGEVTRPT_RLCMAXRETRAN_RPT rlcMaxNumRetranInfo;
        CFW_ENGEVTRPT_RRCSTATUS_RPT rrcStatusInfo;
        CFW_ENGEVTRPT_RRCESTREQ_RPT rrcEstReqInfo;
        CFW_ENGEVTRPT_RRCESTFAIL_RPT rrcEstFailInfo;
        CFW_ENGEVTRPT_INTRALTE_HO_RPT intraLteHoInfo;
        CFW_ENGEVTRPT_IRAT_L2G_RPT IratHoL2GInfo;
        CFW_ENGEVTRPT_IRAT_L2G_REDIREC_RPT IratRedirectionL2GInfo;
        cfw_GsmChangeToLteInd_t GsmChangeToLte;
    };
} CFW_ENG_EVENT_RPT;

typedef struct
{
    uint8_t eventId;
    uint8_t reserved[3];
    union {
        CFW_ENGEVTRPT_INITACC_RPT initAccInfo;
        CFW_ENGEVTRPT_MACACCFAIL_RPT macAccFailInfo;
        CFW_ENGEVTRPT_T310EXPI_RPT t310ExpiredInfo;
        CFW_ENGEVTRPT_RLCMAXRETRAN_RPT rlcMaxNumRetranInfo;
        CFW_ENGEVTRPT_RRCSTATUS_RPT rrcStatusInfo;
        CFW_ENGEVTRPT_RRCESTREQ_RPT rrcEstReqInfo;
        CFW_ENGEVTRPT_RRCESTFAIL_RPT rrcEstFailInfo;
        CFW_ENGEVTRPT_INTRALTE_HO_RPT intraLteHoInfo;
        CFW_ENGEVTRPT_IRAT_L2G_RPT IratHoL2GInfo;
        CFW_ENGEVTRPT_IRAT_L2G_REDIREC_RPT IratRedirectionL2GInfo;
        cfw_GsmChangeToLteInd_v2_t GsmChangeToLte;
    };
} CFW_ENG_EVENT_RPT_V2;

#define CFW_L2_WB_MAX_DRB_NUM 8

typedef struct CFW_ENGSTATISRPT_RLC_INSTSTATIS_Tag
{
    uint8_t drbId;
    uint8_t reserved[3];
    uint16_t ulAmDataPduNackNum;
    uint16_t ulAmDataPduAckNum;
    uint32_t ulAmDataPduRetrRate;
    uint32_t ulAmNewPduByte;
    uint32_t ulAmDataPduNackByte;
    uint32_t ulAmDataPduAckByte;
    uint32_t sendBufferRemain;
    uint32_t dlAmPduByte;
} CFW_ENGSTATISRPT_RLC_INSTSTATIS;

typedef struct CFW_ENGSTATISRPT_PDCP_INSTSTATIS_Tag
{
    uint8_t drbId;
    uint8_t reserved[3];
    uint32_t ulPacketDiscardRate;
} CFW_ENGSTATISRPT_PDCP_INSTSTATIS;

typedef struct CFW_ENGSTATISRPT_EL2_RPT_Tag
{
    CFW_ENGSTATISRPT_RLC_INSTSTATIS rlcInstStatis[CFW_L2_WB_MAX_DRB_NUM];
    CFW_ENGSTATISRPT_PDCP_INSTSTATIS pdcpInstStatis[CFW_L2_WB_MAX_DRB_NUM];
} CFW_ENGSTATISRPT_EL2_RPT;

typedef struct CFW_ENGSTATISRPT_EL1_RPT_Tag
{
    int16_t ta;
    uint16_t phr;
} CFW_ENGSTATISRPT_EL1_RPT;

typedef struct
{
    uint8_t el1ValidFlag;
    CFW_ENGSTATISRPT_EL1_RPT el1StatisInfo;
} CFW_ENG_STATIS_RPT_EL1;

typedef struct
{
    uint8_t el1ValidFlag;
    uint8_t el2ValidFlag;
    CFW_ENGSTATISRPT_EL1_RPT el1StatisInfo;
    CFW_ENGSTATISRPT_EL2_RPT el2StatisInfo;
} CFW_ENG_STATIS_RPT;

typedef struct
{
    uint8_t atEl2StatisType;
    uint32_t atEl2Threshold;
    uint32_t atPhyTimeThreshold;
} CFW_STATIS_INFO_CFG_RPT;
uint32_t CFW_NwErrcStatisCfg(CFW_STATIS_INFO_CFG_RPT cfg_rpt, CFW_SIM_ID nSimID);
uint32_t CFW_NwGetStatisticRpt(CFW_ENG_STATIS_RPT *pEngStatisRpt, CFW_SIM_ID nSimID);
typedef struct
{
    int16_t thresholdOffsetP;   //Setup value of the idle measurement threshold offset for Srxlev
    int16_t thresholdOffsetQ;   //Setup value of the idle measurement threshold offset for Squal
    int16_t neighborOffsetRSRP; //Setup value for the RSRP offset of neighbor cells in idle measurement
    int16_t neighborOffsetRSRQ; //Setup value for the RSRQ offset of neighbor cells in idle measurement
} CFW_MEAS_OFFSET_T;
uint32_t CFW_ErrcSetMeasThreshold(CFW_MEAS_OFFSET_T meas_cfg, CFW_SIM_ID nSim);
uint32_t CFW_ErrcGetMeasThreshold(uint16_t nUTI, CFW_SIM_ID nSim);

typedef struct
{
    uint8_t state;
} CFW_RACH_RESULT_IND;

#define CFW_MAX_STORED_USR_EARFCN_NUM 16

typedef struct CFW_StoredUsrFreqInfo_Tag
{
    uint32_t arfcn;
    uint8_t reserved;
    uint8_t plmn[3];
} CFW_StoredUsrFreqInfo;

typedef struct CFW_UsrHisFreqInfo_Tag
{
    uint8_t arfcnNum;
    uint8_t reserved[3];
    CFW_StoredUsrFreqInfo storedUsrFreqInfo[CFW_MAX_STORED_USR_EARFCN_NUM];
} CFW_UsrHisFreqInfo;

typedef struct
{
    uint8_t bitmask;
    uint8_t emmMsgType;
    uint8_t esmMsgType;
    uint8_t emmCause;
    uint8_t esmCause;
    uint8_t reserved[3];
} CFW_NW_EMM_FAILED_PROCEDURE_T;

uint32_t CFW_SetUsrHisFreqInfo(CFW_StoredUsrFreqInfo pHisFreqInfo, uint8_t bOperFlag, CFW_SIM_ID nSimID);
uint32_t CFW_GetUsrHisFreqInfo(CFW_UsrHisFreqInfo *pHisFreqInfo, CFW_SIM_ID nSimID);
uint8_t CFW_GetDefaultPcoControl(CFW_SIM_ID nSimID);                  //return 1:PDN req with Attach no set pco; 0: PDN req with Attach no change
uint32_t CFW_SetDefaultPcoControl(uint8_t nValue, CFW_SIM_ID nSimID); //value 1:PDN req with Attach no set pco; 0: PDN req with Attach no change

uint32_t CFW_SetVocLoopType(uint16_t type, CFW_SIM_ID nSimID);
uint32_t CFW_GprsSendDefaultCtx(uint8_t nSimID); //config default apn,pdptype,authprot,username,password
#ifdef CONFIG_SOC_8811
void CFW_DefaultEpsBearerRejFromAP(uint8_t uCause);
#endif
uint32_t CFW_SetInitStack(bool status, CFW_SIM_ID nSimID);
uint32_t CFW_SimResetStatus(CFW_SIM_ID nSimID);

#endif //if cfw.h
