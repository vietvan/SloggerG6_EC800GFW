/******************************************************************************
 ** File Name:      mn_gps_signal_def.h                                                 *
 ** Author:         CUIFANG DENG                                              *
 ** Date:           05/31/2017                                               *
 ** Copyright:      Copyright 2001-2017 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:                                                              *                                                      
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 05/31/2017     cuifang.deng      Create.                                   *
 ******************************************************************************/
//#ifdef _MNGPS_SUPPORT_
#if !defined(_MN_GPS_SIGNAL_DEF_H)
#define _MN_GPS_SIGNAL_DEF_H

#include "sci_types.h"
#include "os_api.h"
#include "mn_type.h"
#include "mn_gps_type.h"

typedef enum
{
    MN_GPS_NOT_RUN,
    MN_GPS_NOT_FIX,
    MN_GPS_2D_FIX,
    MN_GPS_3D_FIX
} MN_GPS_STATUS_E;

typedef enum
{
    MN_GPS_HOT_MODE,
    MN_GPS_COLD_MODE,
    MN_GPS_WARM_MODE,
    MN_GPS_FACTORY_MODE
} MN_GPS_RESET_MODE_E,
    MN_GPS_START_MODE_E;

// following signal only have signal header
typedef xSignalHeaderRec SIGNALS_NO_PARA_T;

typedef struct
{
    SIGNAL_VARS
    uint8 type; //bit0: START_GPS     bit1: START_BDS      bit2: START_GLONASS
    MN_GPS_START_MODE_E start_mode;
} MN_GPS_START_REQ_T;

typedef struct
{
    SIGNAL_VARS
    MN_GPS_RESET_MODE_E reset_mode;
} MN_GPS_RESET_REQ_T;
typedef struct
{
    SIGNAL_VARS
    MN_GPS_DATA_SOCKET_E dataSocket_type;
} MN_GPS_DATA_SOCKET_IND_T;
typedef struct
{
    SIGNAL_VARS
    MN_GPS_GNSS_MODE_E gnss_mode;
} MN_GPS_SET_GNSS_MODE_REQ_T;

typedef struct
{
    SIGNAL_VARS
    MN_GPS_STATUS_E status;
} MN_GPS_STATUS_CNF_T;

typedef struct
{
    SIGNAL_VARS
    uint16 type; // value 0- disable ouput;  BIT0=1--GGA,  BIT1=1--GLL,  BIT2=1--GSA,  BIT3=1--GSV,  BIT4=1--RMC, BIT5=1--VTG
    uint32 time_interval;
} MN_GPS_OUTPUT_REQ_T;

typedef struct
{
    SIGNAL_VARS
    MN_SYS_SIM_E dual_sys;
    uint32 netid; //value 5~11.   0:invalid
} MN_GPS_PDP_INFO_REQ_T;

typedef struct
{
    SIGNAL_VARS
    MN_SYS_SIM_E dual_sys;
    BOOLEAN is_report_location_info;
    BOOLEAN is_report_satellite_info;
} MN_GPS_REPORT_REQ_T;

typedef struct
{
    SIGNAL_VARS
    MN_SYS_SIM_E dual_sys;
    uint8 type;
} MN_GPS_ASSERT_REQ_T;

typedef struct
{
    SIGNAL_VARS
    BOOLEAN is_ok;
} MN_GPS_DOWNLOAD_CNF_T,
    MN_GPS_INIT_CNF_T,
    MN_GPS_START_CNF_T,
    MN_GPS_STOP_CNF_T,
    MN_GPS_RESET_CNF_T,
    MN_GPS_OUTPUT_CNF_T,
    MN_GPS_POWEROFF_CNF_T,
    MN_GPS_SET_GNSS_MODE_CNF_T,
    MN_GPS_DEL_AID_DATA_CNF_T,
    MN_GPS_PDP_INFO_CNF_T,
    MN_GPS_RUN_CNF_T,
    MN_GPS_REPORT_CNF_T;

typedef SIGNALS_NO_PARA_T MN_GPS_ERR_IND_T;

typedef struct
{
    SIGNAL_VARS
    MN_GPS_STATUS_E fix_status; //¶¨Î»×´Ì¬
} MN_GPS_READ_STATUS_CNF_T;

typedef struct
{
    uint16 wYear;
    uint16 wMonth;
    uint16 wDay;
    uint16 wHour;
    uint16 wMinute;
    uint16 wSecond;
} TimeStamp;

typedef struct
{
    float longitude;     // ¾­¶È
    float latitude;      // Î³¶È
    float altitude;      // ¸ß¶È
    TimeStamp utc_time;  // ÊÀ½çÊ±¼ä
    uint32 ttff_time;    // µÚÒ»´Î¶¨Î»ËùÓÃÊ±¼ä£¬µ¥Î»s
    uint8 satellite_num; // ÎÀÐÇÊýÄ¿
    float speed;         // ¶ÔµØËÙ¶È
    float course;        // ¶ÔµØ·½Ïò
    uint16 clockdrift;
    SCI_TICK_TIME_T m_nSystemTimeStamp; // RTC or any other system time
    uint32 m_nUTCTime;                  // second
    uint8 m_nUncertaintySemiMajor;
    uint8 m_nUncertaintySemiMinor;
    float m_nBearing;            // increments of 1 measured clockwise from the North.
    uint16 m_nDirection;         // UP=0, Down=1
    float m_nHorizontalVelocity; // m/s
    float m_nVerticalVelocity;   // m/s
    uint32 fix_flag;
    float m_nHorizontalAccuracy;
    float m_nPDOP; // Î»ÖÃ¾«¶ÈÒò×Ó
    float m_nHDOP; // Ë®Æ½¾«¶ÈÒò×Ó
    float m_nVDOP; // ´¹Ö±¾«¶ÈÒò×Ó
} MN_GPS_INFO_T;

typedef struct
{
    uint16 m_nSatelliteIdentifier; // SV PRN
    uint16 m_cn0;                  // 2CN0ÐÅÔë±È
    uint16 m_nElevation;           // Ñö½Ç
    uint16 m_nAzimuth;             // ·½Î»½Ç
    uint8 m_IsUsed;
} MN_GPS_SATELLITE_INFO_T;

typedef struct
{
    uint8 satellite_num;
    MN_GPS_SATELLITE_INFO_T satellite_info[32];
} MN_GPS_SATELLITE_INFO_LIST_T;

typedef struct
{
    SIGNAL_VARS
    MN_GPS_INFO_T gps_info;
    MN_GPS_SATELLITE_INFO_LIST_T satellite_list_info;
} MN_GPS_READ_INFO_CNF_T;

typedef struct
{
    SIGNAL_VARS
    MN_GPS_INFO_T gps_info;
} MN_GPS_LOCATION_INFO_IND_T;

typedef struct
{
    SIGNAL_VARS
    MN_GPS_SATELLITE_INFO_LIST_T satellite_list_info;
} MN_GPS_SATELLITE_INFO_IND_T;

typedef struct
{
    SIGNAL_VARS
    uint16 length;
    uint16 type;                           // BIT0=1--GGA,  BIT1=1--GLL,  BIT2=1--GSA,  BIT3=1--GSV,  BIT4=1--RMC, BIT5=1--VTG
    uint8 nmea_data[NMEA_PACKET_DATA_LEN]; // NMEA packet buffer
} MN_GPS_OUTPUT_INFO_IND_T;

typedef struct
{
    SIGNAL_VARS
    float longitude;    //¾­¶È
    float latitude;     //Î³¶È
    TimeStamp utc_time; // ÊÀ½çÊ±¼ä
    uint32 ttff_time;   //µÚÒ»´Î¶¨Î»ËùÓÃÊ±¼ä£¬µ¥Î»s
    uint16 cn0;
} MN_GPS_FIX_IND_T;

#endif /* end of _MN_GPS_SIGNAL_DEF_H */
//#endif
