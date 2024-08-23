/*============================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
 =============================================================================*/
/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


WHEN        WHO            WHAT, WHERE, WHY
----------  ------------   ----------------------------------------------------

=============================================================================*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "ql_log.h"
#include "ql_api_osi.h"
#include "ql_osi_def.h"
#include "ch395_adapter.h"

#include "CH395CMD.H"
#include "CH395INC.H"
#include "CH395.H"

#define CH395_LOG_LEVEL                     QL_LOG_LEVEL_INFO
#define CH395_DEMO_LOG(msg, ...)			QL_LOG(CH395_LOG_LEVEL, "ch395_log", msg, ##__VA_ARGS__)

#define CH395_ERROR_RETRY_TIMES     5

#define CH395_BUFFER_SIZE           1514       //max mac frame len
#define CH395_SOCKET_INDEX          0

struct _CH395_SYS
{
    UINT8   IPAddr[4];                                           /* CH395IP地址 32bit*/
    UINT8   GWIPAddr[4];                                         /* CH395网关地址 32bit*/
    UINT8   MASKAddr[4];                                         /* CH395子网掩码 32bit*/
    UINT8   MacAddr[6];                                          /* CH395MAC地址 48bit*/
    UINT8   PHYStat;                                             /* CH395 PHY状态码 8bit*/
    UINT8   MackFilt;                                            /* CH395 MAC过滤，默认为接收广播，接收本机MAC 8bit*/
    UINT32  RetranCount;                                         /* 重试次数 默认为10次*/
    UINT32  RetranPeriod;                                        /* 重试周期,单位MS,默认200MS */
    UINT8   IntfMode;                                            /* 接口模式 */
    UINT8   UnreachIPAddr[4];                                    /* 不可到达IP */
    UINT16  UnreachPort;                                         /* 不可到达端口 */
};

struct _SOCK_INF
{
    UINT8  IPAddr[4];                                           /* socket目标IP地址 32bit*/
    UINT8  MacAddr[6];                                          /* socket目标地址 48bit*/
    UINT8  ProtoType;                                           /* 协议类型 */
    UINT8  ScokStatus;                                          /* socket状态，参考scoket状态定义 */
    UINT8  TcpMode;                                             /* TCP模式 */
    UINT32 IPRAWProtoType;                                      /* IPRAW 协议类型 */
    UINT16 DesPort;                                             /* 目的端口 */
    UINT16 SourPort;                                            /* 目的端口 */
    UINT16 SendLen;                                             /* 发送数据长度 */
    UINT16 RemLen;                                              /* 剩余长度 */
    UINT8  *pSend;                                              /* 发送指针 */
};

/* 常用变量定义 */
UINT8 MyBuffer[CH395_BUFFER_SIZE];                              /* 数据缓冲区 */
UINT8 SendBuffer[CH395_BUFFER_SIZE];
UINT32 SendBufferLen = 0;
struct _SOCK_INF SockInf;                                       /* 保存Socket信息 */
struct _CH395_SYS CH395Inf;                                     /* 保存CH395信息 */

/* CH395相关定义 */
UINT8 CH395IPAddr[4] = {192,168,1,1};                         /* CH395IP地址 */
UINT8 CH395GWIPAddr[4] = {192,168,1,1};                        /* CH395网关 */
UINT8 CH395IPMask[4] = {255,255,255,0};                        /* CH395子网掩码 */

ql_task_t           ch395_task = NULL;
ql_sem_t            ch395_connect_sem =NULL;
ql_ethernet_mode_e  ch395_mode = QL_ETHERNET_MODE_NONE;

bool isConnect = false;

UINT8  CH395Init(void);
UINT8 CH395SocketInitOpen(void);
/**********************************************************************************
* Function Name  : mStopIfError
* Description    : 调试使用，显示错误代码，并停机
* Input          : iError 错误代码
* Output         : None
* Return         : None
**********************************************************************************/
void mStopIfError(UINT8 iError)
{
    if (iError == CMD_ERR_SUCCESS) return;                           /* 操作成功 */
    uint8_t i = 0;
    switch(iError)
    {   
        //硬件错误
        case CH395_ERR_UNKNOW:
        {
            i = 0;
            while (i < CH395_ERROR_RETRY_TIMES) 
            {
                CH395_DEMO_LOG("CH395 Error: %02X\n", (UINT16)iError);                 /* 显示错误 */
                ql_rtos_task_sleep_ms(2000);
                //Hardware reset
                CH395CMDReset();
                iError = CH395Init();                                                 /* 初始化CH395芯片 */
                if(iError == CMD_ERR_SUCCESS)
                {
                    break;
                }
            }
            break;
        }
        //CH395内部socket错误
        case CH395_ERR_OPEN:
        {
            i = 0;
            while(i < CH395_ERROR_RETRY_TIMES)
            {
                CH395_DEMO_LOG("CH395 Error: %02X\n", (UINT16)iError);                 /* 显示错误 */
                CH395CloseSocket(CH395_SOCKET_INDEX);
                ql_rtos_task_sleep_ms(2000);
                //socket reopen
                iError = CH395SocketInitOpen();
                if(iError == CMD_ERR_SUCCESS)
                {
                    break;
                }
            }
            break;
        }
        default:
        {
            i = CH395_ERROR_RETRY_TIMES;
            break;
        }
    }
    //Error,never return.Otherwise return.
    if(i >= CH395_ERROR_RETRY_TIMES)
    {
        while(1)
        {
            CH395_DEMO_LOG("CH395 Error: %02X\n", (UINT16)iError);
            ql_rtos_task_sleep_ms(10000);
        }
    }
}

/**********************************************************************************
* Function Name  : InitCH395InfParam
* Description    : 初始化CH395Inf参数
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void InitCH395InfParam(void)
{
    memset(&CH395Inf,0,sizeof(CH395Inf));                            /* 将CH395Inf全部清零*/
    memcpy(CH395Inf.IPAddr,CH395IPAddr,sizeof(CH395IPAddr));         /* 将IP地址写入CH395Inf中 */
    memcpy(CH395Inf.GWIPAddr,CH395GWIPAddr,sizeof(CH395GWIPAddr));   /* 将网关IP地址写入CH395Inf中 */
    memcpy(CH395Inf.MASKAddr,CH395IPMask,sizeof(CH395IPMask));       /* 将子网掩码写入CH395Inf中 */
}

/**********************************************************************************
* Function Name  : InitSocketParam
* Description    : 初始化socket
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void InitSocketParam(void)
{
    memset(&SockInf,0,sizeof(SockInf));                              /* 将SockInf[0]全部清零*/
    SockInf.ProtoType = PROTO_TYPE_MAC_RAW;                          /* MAC RAW模式 */
}

/**********************************************************************************
* Function Name  : CH395SocketInitOpen
* Description    : 配置CH395 socket 参数，初始化并打开socket
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
UINT8 CH395SocketInitOpen(void)
{
    UINT8 i;
     /* socket 0为MAC RAW模式 */
    CH395SetSocketProtType(CH395_SOCKET_INDEX,SockInf.ProtoType);                     /* 设置socket 0协议类型 */
    i = CH395OpenSocket(CH395_SOCKET_INDEX);                                          /* 打开socket 0 */
    return i;
}

/**********************************************************************************
* Function Name  : CH395SocketInterrupt
* Description    : CH395 socket 中断,在全局中断中被调用
* Input          : sockindex
* Output         : None
* Return         : None
**********************************************************************************/
void CH395SocketInterrupt(UINT8 sockindex)
{
    UINT8  sock_int_socket;
    UINT16 len;

    sock_int_socket = CH395GetSocketInt(sockindex);                   /* 获取socket 的中断状态 */
    CH395_DEMO_LOG("ch395_sock_int_socket:0x%x",sock_int_socket);
    if(sock_int_socket & SINT_STAT_SENBUF_FREE)                       /* 发送缓冲区空闲，可以继续写入要发送的数据 */
    {

    }
    if(sock_int_socket & SINT_STAT_SEND_OK)                           /* 发送完成中断 */
    {

    }
    if(sock_int_socket & SINT_STAT_RECV)                              /* 接收中断 */
    {
        len = CH395GetRecvLength(sockindex);                          /* 获取当前缓冲区内数据长度 */
        CH395_DEMO_LOG("receive len = %d",len);
        if(len == 0)
        {
            return;
        }
        if(len > CH395_BUFFER_SIZE)
        {
            len = CH395_BUFFER_SIZE;
        }                                                             /*MyBuffer缓冲区长度为1514，*/
        CH395GetRecvData(sockindex,len,MyBuffer);                     /* 读取数据 */
        ql_ethernet_data_input(MyBuffer,len);
    }
    if(sock_int_socket & SINT_STAT_CONNECT)                          /* 连接中断，仅在TCP模式下有效*/
    {

    }
    if(sock_int_socket & SINT_STAT_DISCONNECT)                        /* 断开中断，仅在TCP模式下有效 */
    {

    }
    if(sock_int_socket & SINT_STAT_TIM_OUT)                           /* 超时中断，仅在TCP模式下有效 */
    {   

    }
}

/**********************************************************************************
* Function Name  : CH395GlobalInterrupt
* Description    : CH395全局中断函数
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void CH395GlobalInterrupt(void)
{
   UINT16  init_status;

   UINT8  buf[10]; 
 
    init_status = CH395CMDGetGlobIntStatus_ALL();
    CH395_DEMO_LOG("ch395_init_status0x%x",init_status);
    if(init_status & GINT_STAT_UNREACH)                              /* 不可达中断，读取不可达信息 */
    {
        CH395CMDGetUnreachIPPT(buf);                                
    }
    if(init_status & GINT_STAT_IP_CONFLI)                            /* 产生IP冲突中断，建议重新修改CH395的 IP，并初始化CH395*/
    {
    }
    if(init_status & GINT_STAT_PHY_CHANGE)                           /* 产生PHY改变中断*/
    {
        CH395_DEMO_LOG("Init status : GINT_STAT_PHY_CHANGE\n");
        ql_event_t event;
        event.id = QUEC_ETHERNET_APP_STATUS_CHECK;
        ql_rtos_event_send(ch395_task, &event);
    }
    if(init_status & GINT_STAT_SOCK0)
    {
        CH395SocketInterrupt(0);                                     /* 处理socket 0中断*/
    }
    if(init_status & GINT_STAT_SOCK1)                                
    {
        CH395SocketInterrupt(1);                                     /* 处理socket 1中断*/
    }
    if(init_status & GINT_STAT_SOCK2)                                
    {
        CH395SocketInterrupt(2);                                     /* 处理socket 2中断*/
    }
    if(init_status & GINT_STAT_SOCK3)                                
    {
        CH395SocketInterrupt(3);                                     /* 处理socket 3中断*/
    }
    if(init_status & GINT_STAT_SOCK4)
    {
        CH395SocketInterrupt(4);                                     /* 处理socket 4中断*/
    }
    if(init_status & GINT_STAT_SOCK5)                                
    {
        CH395SocketInterrupt(5);                                     /* 处理socket 5中断*/
    }
    if(init_status & GINT_STAT_SOCK6)                                
    {
        CH395SocketInterrupt(6);                                     /* 处理socket 6中断*/
    }
    if(init_status & GINT_STAT_SOCK7)                                
    {
        CH395SocketInterrupt(7);                                     /* 处理socket 7中断*/
    }
}

/**********************************************************************************
* Function Name  : CH395Init
* Description    : 配置CH395的IP,GWIP,MAC等参数，并初始化
* Input          : None
* Output         : None
* Return         : 函数执行结果
**********************************************************************************/

UINT8  CH395Init(void)
{
    CH395_DEMO_LOG("CH395init");
    UINT8 i;
    i = CH395CMDCheckExist(0x65);                      
    if(i != 0x9a)return CH395_ERR_UNKNOW;                            /* 测试命令，如果无法通过返回0XFA */
                                                                     /* 返回0XFA一般为硬件错误或者读写时序不对 */
    CH395CMDGetMACAddr(CH395Inf.MacAddr);
    CH395_DEMO_LOG("CH395EVT:get mac %02x:%02x:%02x:%02x:%02x:%02x",
    CH395Inf.MacAddr[0],CH395Inf.MacAddr[1],CH395Inf.MacAddr[2],
    CH395Inf.MacAddr[3],CH395Inf.MacAddr[4],CH395Inf.MacAddr[5]);
    i = CH395CMDInitCH395();                                         /* 初始化CH395芯片 */
    return i;
}

/**********************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
**********************************************************************************/
void ch395_output(uint8_t * data,uint32_t len)
{
    if(len > CH395_BUFFER_SIZE || len == 0)
    {
        CH395_DEMO_LOG("CH395_drop");
        return;
    }
    memcpy((void*)SendBuffer,(void*)data,len);
    SendBufferLen = len;
    ql_event_t event;
    event.id = QUEC_ETHERNET_APP_DATA_OUTPUT;
    event.param1 = (uint32_t)SendBuffer;
    event.param2 = (uint32_t)SendBufferLen;
    ql_rtos_event_send(ch395_task, &event);
    CH395_DEMO_LOG("CH395_output");
}

void _gpioint_cb(void *ctx)
{
    //INT# from 1 to 0
    ql_event_t event;
    event.id = QUEC_ETHERNET_APP_INT;
    ql_rtos_event_send(ch395_task, &event);
    CH395_DEMO_LOG("CH395_gpioint_cb");
}

ch395_gpio_cb_t ch395_get_gpio_cb(void)
{
    return _gpioint_cb;
}

bool ch395_wait_for_connect()
{
    while(1)
    {                                                                 /* 等待以太网连接成功*/
        if(CH395CMDGetPHYStatus() == PHY_DISCONN)                     /* 查询CH395是否连接 */
        {
            if(isConnect)
            {
                isConnect = false;
            }
            ql_rtos_task_sleep_ms(1000);
            CH395_DEMO_LOG("CH395 wait for connect");            /* 未连接则等待1s后再次查询 */
        }
        else 
        {
            CH395_DEMO_LOG("CH395 Connect Ethernet");               /* CH395芯片连接到以太网，此时会产生中断 */
            isConnect = true;
            break;
        }
    }
    return true;
}

void ch395_thread(void * arg)
{
    UINT8 i;
    CH395_DEMO_LOG("CH395 Demo");
    CH395CMDReset();
    ql_rtos_task_sleep_ms(200);
    InitCH395InfParam();                                             /* 初始化CH395相关变量 */
    i = CH395Init();                                                 /* 初始化CH395芯片 */
    mStopIfError(i);

    if(ch395_wait_for_connect())
    {
        InitSocketParam();                                        /* 初始化socket相关变量 */
        i = CH395SocketInitOpen();
        mStopIfError(i);
        CH395_DEMO_LOG("CH395 Connect");

        ql_ethernet_ctx_s ctx = 
        {
            .ip4 =          CH395Inf.IPAddr,
            .gw =           CH395Inf.GWIPAddr,
            .netmask =      CH395Inf.MASKAddr,
            .mac =          CH395Inf.MacAddr,
            .mode =         ch395_mode,
            .cb =           ch395_output,
        };
        if(QL_ETHERNET_SUCCESS != ql_ethernet_register(&ctx))
        {
            CH395_DEMO_LOG("CH395 register err");
            goto exit;
        }
        ql_rtos_semaphore_release(ch395_connect_sem);
    }

	while(1)
	{
        ql_event_t event;
        if(ql_event_try_wait(&event) != 0)
        {
            continue;
        }

        switch(event.id)
        {
            case QUEC_ETHERNET_APP_INT:
                CH395GlobalInterrupt();
                break;
            case QUEC_ETHERNET_APP_DATA_OUTPUT:
                CH395SendData(CH395_SOCKET_INDEX,(uint8_t*)event.param1,event.param2);
                break;
            case QUEC_ETHERNET_APP_STATUS_CHECK:
                CH395_DEMO_LOG("CH395 QUEC_ETHERNET_APP_STATUS_CHECK");
                if(ch395_wait_for_connect())
                {
                    CH395_DEMO_LOG("CH395 Connect");
                }
                break;
            default:break;
        }
	}
exit:
    ql_rtos_semaphore_delete(ch395_connect_sem);
    ql_rtos_task_delete(NULL);
}

void ch395_demo_init(void)
{
    QlOSStatus err = QL_OSI_SUCCESS;

    err = ql_rtos_task_create(&ch395_task, 4*1024, APP_PRIORITY_NORMAL, "Ch395_task", ch395_thread, NULL, 10);
    if(err != QL_OSI_SUCCESS)
    {
        CH395_DEMO_LOG("task created failed");
        return;
    }
}

bool ch395_register(ql_ethernet_ctx_s *ctx)
{
    if(!ctx)
    {
        CH395_DEMO_LOG("ctx null");
        return false;
    }
    if(ctx->ip4)
    {
        memcpy((void*)CH395IPAddr,ctx->ip4,sizeof(CH395IPAddr));
    }
    if(ctx->gw)
    {
        memcpy((void*)CH395GWIPAddr,ctx->gw,sizeof(CH395GWIPAddr));
    }
    if(ctx->netmask)
    {
        memcpy((void*)CH395IPMask,ctx->netmask,sizeof(CH395IPMask));
    }
    if(ctx->mode < QL_ETHERNET_MODE_MAX)
    {
        ch395_mode = ctx->mode;
    }
    osi_errcode_e ret = ql_rtos_semaphore_create(&ch395_connect_sem, 0);
    if(ret != QL_OSI_SUCCESS)
    {
        CH395_DEMO_LOG("ch395_connect_sem created failed, ret = 0x%x", ret);
        return false;
    }
    ch395_demo_init();
    if(ql_rtos_semaphore_wait(ch395_connect_sem, 15000))
    {
        CH395_DEMO_LOG("ch395_connect_sem time out");
        return false;
    }
    return true;
}
