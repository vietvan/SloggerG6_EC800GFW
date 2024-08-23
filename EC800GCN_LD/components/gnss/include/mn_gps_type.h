/******************************************************************************
 ** File Name:      mn_gps_type.h                                                 *
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
 ** 06/01/2017     cuifang.deng      Create.                                   *
 ******************************************************************************/
//#ifdef _MNGPS_SUPPORT_
#if !defined(_MN_GPS_TYPE_H)
#define _MN_GPS_TYPE_H

#include "sci_types.h"
#include "os_api.h"
#include "mn_type.h"
#include "gps_pc_mode.h"

typedef enum
{
    MNGPS_NOT_RUN,
    MNGPS_NOT_FIX,
    MNGPS_2D_FIX,
    MNGPS_3D_FIX
} MNGPS_STATUS_E;

typedef enum
{
    MNGPS_HOT_MODE,  //������
    MNGPS_WARM_MODE, //������
    MNGPS_COLD_MODE, //������
    MNGPS_FACTORY_MODE
} MNGPS_RESET_MODE_E,
    MNGPS_START_MODE_E;
typedef enum
{
    MN_GPS_SETUP_DATA_SOCKET,
    MN_GPS_RELEASE_DATA_SOCKET
} MN_GPS_DATA_SOCKET_E;

typedef uint8 MNGPS_START_TYPE_E;
typedef enum
{
    MNGPS_START_GPS_BD2_GALILEO = (CPMODE_GPS_MASK | CPMODE_BD2_MASK | CPMODE_GAL_MASK), //19
    MNGPS_START_GPS = CPMODE_GPS_MASK,                                                   //GPS only                        1
    MNGPS_START_BDS = CPMODE_BD2_MASK,                                                   //BeiDou Only                     2
    MNGPS_START_GLONASS = CPMODE_GLO_MASK,                                               //GLONASS  only         4
    MNGPS_START_B1C = CPMODE_B1C_MASK,                                                   //B1C only                            8
    MNGPS_START_GALILEO = CPMODE_GAL_MASK,                                               //GALILEO  only	     16
    MNGPS_START_GPS_BDS = (CPMODE_GPS_MASK | CPMODE_BD2_MASK),                           //GPS+BeiDou                3
    MNGPS_START_GPS_GLONASS = (CPMODE_GPS_MASK | CPMODE_GLO_MASK),                       //GPS+Glonass     5
    MNGPS_START_GPS_B1C = (CPMODE_GPS_MASK | CPMODE_B1C_MASK),                           //9
    MNGPS_START_GPS_GALILEO = (CPMODE_GPS_MASK | CPMODE_GAL_MASK),                       //17
    //MNGPS_START_BD2_GLONASS = (CPMODE_BD2_MASK|CPMODE_GLO_MASK),                    //6
    MNGPS_START_BD2_GALILEO = (CPMODE_BD2_MASK | CPMODE_GAL_MASK),                                                 //18
    MNGPS_START_B1C_GALILEO = (CPMODE_B1C_MASK | CPMODE_GAL_MASK),                                                 //24
    MNGPS_START_GLONASS_GALILEO = (CPMODE_GLO_MASK | CPMODE_GAL_MASK),                                             //20
    MNGPS_START_GPS_B1C_GLONASS = (CPMODE_GPS_MASK | CPMODE_B1C_MASK | CPMODE_GLO_MASK),                           //13
    MNGPS_START_GPS_B1C_GALILEO = (CPMODE_GPS_MASK | CPMODE_B1C_MASK | CPMODE_GAL_MASK),                           //25
    MNGPS_START_GPS_GLONASS_GALILEO = (CPMODE_GPS_MASK | CPMODE_GLO_MASK | CPMODE_GAL_MASK),                       //21
    MNGPS_START_GPS_B1C_GLONASS_GALILEO = (CPMODE_GPS_MASK | CPMODE_B1C_MASK | CPMODE_GLO_MASK | CPMODE_GAL_MASK), //29
    MN_GPS_GNSS_MODE_MAX
} MN_GPS_GNSS_MODE_E;

typedef struct
{
    SIGNAL_VARS
    BOOLEAN is_ok;
} APP_MN_GPS_SETGNSSMODE_CNF_T,
    APP_MN_GPS_DEL_AID_DATA_CNF_T;

typedef struct
{
    SIGNAL_VARS
    MN_GPS_DATA_SOCKET_E dataSocket_type;
} APP_MN_GPS_DATA_SOCKET_IND_T;

typedef struct
{
    SIGNAL_VARS
    MN_SYS_SIM_E dual_sys;
    BOOLEAN is_ok;
} APP_MN_GPS_DOWNLOAD_CNF_T,
    APP_MN_GPS_INIT_CNF_T,
    APP_MN_GPS_START_CNF_T,
    APP_MN_GPS_STOP_CNF_T,
    APP_MN_GPS_RESET_CNF_T,
    APP_MN_GPS_OUTPUT_CNF_T,
    APP_MN_GPS_POWEROFF_CNF_T,
    APP_MN_GPS_REPORT_CNF_T;

typedef struct
{
    SIGNAL_VARS
    MN_SYS_SIM_E dual_sys;
} APP_MN_GPS_ERR_IND_T;

typedef struct
{
    SIGNAL_VARS
    MN_SYS_SIM_E dual_sys;
    float longitude;  //����
    float latitude;   //γ��
    uint32 ttff_time; //��һ�ζ�λ����ʱ�䣬��λs
} APP_MN_GPS_FIX_IND_T;

typedef struct
{
    SIGNAL_VARS
    MN_SYS_SIM_E dual_sys;
    MNGPS_STATUS_E fix_status; //��λ״̬
} APP_MN_GPS_READ_STATUS_CNF_T;

typedef struct
{
    uint16 year;
    uint16 month;
    uint16 day;
    uint16 hour;
    uint16 minute;
    uint16 second;
} MNGPS_TIMESTAMP_T;

typedef struct
{
    float longitude;            //����
    float latitude;             //γ��
    float altitude;             //�߶�
    MNGPS_TIMESTAMP_T utc_time; // ����ʱ��
    uint32 ttff_time;           //��һ�ζ�λ����ʱ�䣬��λs
    uint8 satellite_num;        //������Ŀ
    float speed;                //�Ե��ٶ�
    float course;               //�Եط���
    uint16 cno;
    SCI_TICK_TIME_T m_nSystemTimeStamp; //RTC or any other system time
    uint32 m_nUTCTime;                  //second
    uint8 m_nUncertaintySemiMajor;
    uint8 m_nUncertaintySemiMinor;
    float m_nBearing;            // increments of 1 measured clockwise from the North.
    uint16 m_nDirection;         //  UP=0, Down=1
    float m_nHorizontalVelocity; // m/s
    float m_nVerticalVelocity;   // m/s
    uint32 fix_flag;
    float m_nHorizontalAccuracy;
    float m_nPDOP; //λ�þ�������
    float m_nHDOP; //ˮƽ��������
    float m_nVDOP; //��ֱ��������
} MNGPS_INFO_T;

typedef struct
{
    uint16 m_nSatelliteIdentifier; // SV PRN
    uint16 m_cn0;                  // 2CN0�����
    uint16 m_nElevation;           // ����
    uint16 m_nAzimuth;             // ��λ��
    uint8 m_IsUsed;
} MNGPS_SATELLITE_INFO_T;

typedef struct
{
    uint8 satellite_num;
    MNGPS_SATELLITE_INFO_T satellite_info[32];
} MNGPS_SATELLITE_INFO_LIST_T;

typedef struct
{
    SIGNAL_VARS
    MN_SYS_SIM_E dual_sys;
    MNGPS_INFO_T gps_info;
    MNGPS_SATELLITE_INFO_LIST_T satellite_list_info;
} APP_MN_GPS_READ_INFO_CNF_T;

typedef struct
{
    SIGNAL_VARS
    MN_SYS_SIM_E dual_sys;
    MNGPS_INFO_T gps_info;
} APP_MN_GPS_LOC_INFO_IND_T;

typedef struct
{
    SIGNAL_VARS
    MN_SYS_SIM_E dual_sys;
    MNGPS_SATELLITE_INFO_LIST_T satellite_list_info;
} APP_MN_GPS_SATELLITE_INFO_IND_T;

typedef struct
{
    SIGNAL_VARS
    MN_SYS_SIM_E dual_sys;
    uint16 length;
    uint16 type;           // BIT0=1--GGA,  BIT1=1--GLL,  BIT2=1--GSA,  BIT3=1--GSV,  BIT4=1--RMC, BIT5=1--VTG
    uint8 nmea_data[1024]; //NMEA��ʽ����
} APP_MN_GPS_OUTPUT_INFO_IND_T;

typedef struct
{
    BOOLEAN is_gps_power_on;       //gps�Ƿ��
    BOOLEAN is_fix_on;             //gps��λ�Ƿ��
    MNGPS_RESET_MODE_E reset_mode; //����ģʽ
    uint16 ouput_type;             //�������
    uint32 time_interval;
} MNGPS_STATUS_INFO_T;

#endif /* end of _MN_TYPE_H */
//#endif
