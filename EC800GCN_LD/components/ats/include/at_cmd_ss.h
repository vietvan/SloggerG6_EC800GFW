#ifndef __AT_CMD_SS_H__
#define __AT_CMD_SS_H__

#include "ats_config.h"
/*
    for test parse string
*/
//#ifdef CFW_VOLTE_SUPPORT //quectel modify
#define MAX_LENGTH_NUMBER_CCFCU 41
#define AT_CMD_CCFCU_OPTYPE_VOICECALL 1
//#endif //quectel modify
#define AT_MAX_DIGITS_USSD 183
#define AT_MAX_CC_NUM 7
#define MAX_PARMS 5
#define MAXPASSWORDLEN 4

#define CHR_STAR ('*')
#define CHR_HASH ('#')

#define CW_DISABLE 0
#define CW_ENABLE 1

typedef enum
{
    SSMMI_STAR = 0,
    SSMMI_HASH,
    SSMMI_STARHASH,
    SSMMI_STARSTAR,
    SSMMI_HASHHASH,
    SSMMI_USS
} SSOpType;

typedef enum //                           Basic Service group number
{
    Adp_ALL_BS_AND_TS = 0, // 1-12
    //         TELESERVICES
    Adp_ALL_TELESERVICES = 10,            // 1-6 + 12
    Adp_TELEPHONY = 11,                   // 1
    Adp_ALL_DATA_TELESERVICES = 12,       // 2-6
    Adp_FACSIMILE_SERVICES = 13,          // 6
    Adp_SHORT_MESSAGE_SERVICES = 16,      // 2
    Adp_ALL_TELESERVICES_EXCEPT_SMS = 19, // 1 + 3-6 + 12
    //         VOICE GROUP SERVICES
    Adp_VOICE_GROUP_CALL_SERVICE = 17,
    Adp_VOICE_BROADCAST_SERVICE = 18,
    //         BEARER SERVICE
    Adp_ALL_BEARER_SERVICES = 20,     // 7-11
    Adp_ALL_ASYNC_SERVICES = 21,      // 7
    Adp_ALL_SYNC_SERVICES = 22,       // 8
    Adp_ALL_DATA_CIRCUIT_SYNC = 24,   // 8
    Adp_ALL_DATA_CIRCUIT_ASYNC = 25,  // 7
    Adp_ALL_GPRS_BEARER_SERVICES = 29 // 13
} Adp_bs_code_enum;

typedef enum //                        RegisterSS EraseSS ActivateSS DeactivateSS InterrogateSS
{
    SS_ALLSS = 0x00,
    SS_ALLLINEIDENTIFICATIONSS = 0x10,
    SS_CLIP = 0x11, //                                           11.14
    SS_CLIR = 0x12, //                                           12.14
    SS_COLP = 0x13, //                                           13.14
    SS_COLR = 0x14, //                                           14.14
    SS_MCI = 0x15,
    SS_ALLNAMEIDENTIFICATIONSS = 0x18,
    SS_CNAP = 0x19,                //                                           19.14
    SS_ALLFORWARDINGSS = 0x20,     //20.10      20.11   20.12      20.13        20.14
    SS_CFU = 0x21,                 //21.10      21.11   21.12      21.13        21.14
    SS_ALLCONDFORWARDINGSS = 0x28, //28.10      28.11   28.12      28.13        28.14
    SS_CFB = 0x29,                 //29.10      29.11   29.12      29.13        29.14
    SS_CFNRY = 0x2A,               //2A.10      2A.11   2A.12      2A.13        2A.14
    SS_CFNRC = 0x2B,               //2B.10      2B.11   2B.12      2B.13        2B.14
    SS_CD = 0x24,
    SS_ALLCALLOFFERINGSS = 0x30,
    SS_ECT = 0x31,
    SS_MAH = 0x32,
    SS_ALLCALLCOMPLETIONSS = 0x40,
    SS_CW = 0x41,     //                   41.12      41.13        41.14
    SS_HOLD = 0x42,   //                              ERASECCENTRY
    SS_CCBS_A = 0x43, //                              43.77        43.14
    SS_CCBS_B = 0x44,
    SS_MC = 0x45,
    SS_ALLMULTIPARTYSS = 0x50,
    SS_MULTIPTY = 0x51,
    SS_ALLCOMMUNITYOFINTEREST_SS = 0x60,
    SS_CUG = 0x61,
    SS_ALLCHARGINGSS = 0x70,
    SS_AOCI = 0x71,
    SS_AOCC = 0x72,
    SS_ALLADDITIONALINFOTRANSFERSS = 0x80,
    SS_UUS1 = 0x81,
    SS_UUS2 = 0x82,
    SS_UUS3 = 0x83,                   //REGISTERPW GETPW
    SS_ALLBARRINGSS = 0x90,           //90.17      90.18   90.12      90.13        90.14
    SS_BARRINGOFOUTGOINGCALLS = 0x91, //91.17      91.18   91.12      91.13        91.14
    SS_BAOC = 0x92,                   //92.17      92.18   92.12      92.13        92.14
    SS_BOIC = 0x93,                   //93.17      93.18   93.12      93.13        93.14
    SS_BOICEXHC = 0x94,               //94.17      94.18   94.12      94.13        94.14
    SS_BARRINGOFINCOMINGCALLS = 0x99, //99.17      99.18   99.12      99.13        99.14
    SS_BAIC = 0x9A,                   //9A.17      9A.18   9A.12      9A.13        9A.14
    SS_BICROAM = 0x9B,                //9B.17      9B.18   9B.12      9B.13        9B.14
    SS_ALLCALLPRIORITYSS = 0xA0,
    SS_EMLPP = 0xA1, //A1.10                                      A1.14
    SS_ALLLCSPRIVACYEXCEPTION = 0xB0,
    SS_UNIVERSAL = 0xB1,
    SS_CALLRELATED = 0xB2,
    SS_CALLUNRELATED = 0xB3,
    SS_PLMNOPERATOR = 0xB4,
    SS_ALLMOLR_SS = 0xC0,
    SS_BASICSELFLOCATION = 0xC1,
    SS_AUTONOMOUSSELFLOCATION = 0xC2,
    SS_TRANSFERTOTHIRDPARTY = 0xC3,
    SS_ALLPLMN_SPECIFICSS = 0xF0,
    SS_PLMN_SPECIFICSS_1 = 0xF1,
    SS_PLMN_SPECIFICSS_2 = 0xF2,
    SS_PLMN_SPECIFICSS_3 = 0xF3,
    SS_PLMN_SPECIFICSS_4 = 0xF4,
    SS_PLMN_SPECIFICSS_5 = 0xF5,
    SS_PLMN_SPECIFICSS_6 = 0xF6,
    SS_PLMN_SPECIFICSS_7 = 0xF7,
    SS_PLMN_SPECIFICSS_8 = 0xF8,
    SS_PLMN_SPECIFICSS_9 = 0xF9,
    SS_PLMN_SPECIFICSS_A = 0xFA,
    SS_PLMN_SPECIFICSS_B = 0xFB,
    SS_PLMN_SPECIFICSS_C = 0xFC,
    SS_PLMN_SPECIFICSS_D = 0xFD,
    SS_PLMN_SPECIFICSS_E = 0xFE,
    SS_PLMN_SPECIFICSS_F = 0xFF
} ss_code_enum;

typedef enum
{
    SS_OP_REGISTERSS = 10,                    //          CISS
    SS_OP_ERASESS = 11,                       //          CISS
    SS_OP_ACTIVATESS = 12,                    //          CISS
    SS_OP_DEACTIVATESS = 13,                  //          CISS
    SS_OP_INTERROGATESS = 14,                 //          CISS
    SS_OP_NOTIFYSS = 16,                      // CRSS
    SS_OP_REGISTERPASSWORD = 17,              //          CISS
    SS_OP_GETPASSWORD = 18,                   //          CISS
    SS_OP_PROCESSUNSTRUCTUREDSS_DATA = 19,    // CRSS AND CISS
    SS_OP_FORWARDCHECKSS_INDICATION = 38,     //          CISS
    SS_OP_PROCESSUNSTRUCTUREDSS_REQUEST = 59, //          CISS
    SS_OP_UNSTRUCTUREDSS_REQUEST = 60,        //          CISS
    SS_OP_UNSTRUCTUREDSS_NOTIFY = 61,         //          CISS
    SS_OP_ERASECCENTRY = 77,                  //          CISS
    SS_OP_LCS_MOLR = 115,                     //          CISS
    SS_OP_LCS_LOCATIONNOTIFICATION = 116,     //          CISS
    SS_OP_CALLDEFLECTION = 117,               // CRSS
    SS_OP_USERUSERSERVICE = 118,              // CRSS
    SS_OP_ACCESSREGISTERCCENTRY = 119,        // CRSS
    SS_OP_FORWARDCUG_INFO = 120,              // CRSS
    SS_OP_SPLITMPTY = 121,                    // CRSS
    SS_OP_RETRIEVEMPTY = 122,                 // CRSS
    SS_OP_HOLDMPTY = 123,                     // CRSS
    SS_OP_BUILDMPTY = 124,                    // CRSS
    SS_OP_FORWARDCHARGEADVICE = 125,          // CRSS
    SS_OP_EXPLICITCT = 126                    // CRSS
} l4_op_code_enum;

typedef struct
{
    SSOpType type;
    uint8_t parmStart[MAX_PARMS]; /* start position in user string */
    uint8_t parmLen[MAX_PARMS];
} SSParsedParameters;

typedef struct
{
    uint8_t *pzMmiStr;
    ss_code_enum SSCode;
} ATCodeToSsCode;

#define AT_MAX_CC_ADDR_LEN 41

typedef struct
{
    uint8_t input[AT_MAX_DIGITS_USSD];
    uint8_t dcs;
    uint8_t length;
    uint8_t ip_string[11];
    bool IsEmergencyNo;
} at_ss_parsing_string_req_struct;

bool AT_IsSSString(const char *s, CFW_SIM_ID nSimID);
bool AT_GetCallInfo(CFW_CC_CURRENT_CALL_INFO *pCallInfo, uint8_t CallStatus, CFW_SIM_ID nSimID);
bool AT_SS_Adaption_ParseStringREQ(at_ss_parsing_string_req_struct *pReq, uint8_t nDLCI, CFW_SIM_ID nSimID);
uint32_t AT_SS_QUERYERR(uint8_t nType, uint16_t nErrCode);

// =====================================================
//
// GSMBCD码,如果转化奇数个数字的话，将会在对应高位补F
// 支持'*''#''p'
// 13811189836 --> 0x31 0x18 0x11 0x98 0x38 0xF6
// return: output should >= nNumberLen/2+1
// ======================================================
uint16_t AT_SsAsciiToGsmBcd(const char *pNumber, uint8_t nNumberLen, uint8_t *pBCD);

#if !defined(CONFIG_ATR_NBONLY_API_SUPPORT) && !defined(CONFIG_SOC_8850)
uint8_t SS_ClassToCFWClass(uint8_t nclass);
#endif //CONFIG_ATR_NBONLY_API_SUPPORT

#endif
