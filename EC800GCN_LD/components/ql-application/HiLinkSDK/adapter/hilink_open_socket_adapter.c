/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: ���������Socket�ӿں��� (���豸����ʵ��)
 */
#include "hilink_open_socket_adapter.h"
//#include "hilink_log.h" //Quectel larson.li �ṩ��SDK��δ������ͷ�ļ�

/* HiLink Socket������ */
typedef enum {
    HILINK_SOCKET_NO_ERROR = 0,
    HILINK_SOCKET_NULL_PTR = -1,
    HILINK_SOCKET_CREAT_UDP_FD_FAILED = -2,
    HILINK_SOCKET_SEND_UDP_PACKET_FAILED = -3,
    HILINK_SOCKET_READ_UDP_PACKET_FAILED = -4,
    HILINK_SOCKET_TCP_CONNECTING = -5,
    HILINK_SOCKET_TCP_CONNECT_FAILED = -6,
    HILINK_SOCKET_SEND_TCP_PACKET_FAILED = -7,
    HILINK_SOCKET_READ_TCP_PACKET_FAILED = -8,
    HILINK_SOCKET_REMOVE_UDP_FD_FAILED = -9,
    HILINK_SOCKET_SELECT_TIMEOUT = -10,
    HILINK_SOCKET_SELECT_ERROR = -11
} hilink_socket_error_t;

/*
 * ��ȡԶ��������
 * ����hostName Զ����������, url
 * ����ipList ���Զ������IP��ַ�б������
 * ����num ���Զ������IP��ַ�б������Ĵ�С
 * ����0 �ɹ�, ���� ʧ��
 * ע��Contiki OS�¸ýӿ�Ϊ�첽��ȡ, ʧ�ܺ����ٴλ�ȡ
 */
int HILINK_GetHostByName(const char *hostName, char ipList[][MAX_IP_LEN], int num)
{
    return 0;
}

/*
 * ����UDPͨѶ�׽���
 * ����localPort UDPͨ�Ŷ˿ں�
 * ����ֵ: ����ֵС��0 ����ʧ��, ���ڵ���0 UDP�׽���fd
 */
int HILINK_CreateUdp(unsigned short localPort)
{
    return 0;
}

/*
 * �ر�UDPͨѶ�׽���
 * �������int fd UDPͨѶ�׽���
 */
void HILINK_RemoveUdp(int fd)
{
    return;
}

/*
 * ����UDP��������
 * ����fd UDPͨѶ�׽���
 * ����buf ���͵�����
 * ����len ���ݳ���
 * ����remoteIp Ŀ��ip
 * ����remotePort Ŀ�Ķ˿�
 * ����ֵС��0 ����ʧ��, ���ڵ���0 ʵ�ʷ��͵��ֽ���
 */
int HILINK_SendUdp(int fd, const unsigned char *buf, unsigned short len, const char *remoteIp,
    unsigned short remotePort)
{
    return 0;
}

/*
 * UDP��ȡ����
 * ����ֵС��0 ��ȡʧ��, ���ڵ���0 ʵ�ʶ�ȡ���ֽ���
 */
int HILINK_RecvUdp(UdpRecvParam *udpRecvParam)
{
    return 0;
}

/*
 * ����TCP����
 * ����dst Ŀ��IP
 * ����port Ŀ�Ķ˿�
 * ����ֵС��0 ����ʧ��, ���ڵ���0 TCP�׽���
 */
int HILINK_ConnectTcp(const char *dst, unsigned short port)
{
    return 0;
}

/*
 * TCP����״̬��ȡ
 * ����fd TCP�׽���
 * ����0 ��������; ��0 ����ʧ��, ����μ� hilink_socket_error_t����
 */
int HILINK_GetTcpState(int fd)
{
    return 0;
}

/*
 * �Ͽ�TCP����
 * ����fd TCP�׽���
 * ������� int fd  TCP �׽���
 */
void HILINK_DisconnectTcp(int fd)
{
    return;
}

/*
 * TCP��������
 * ����fd TCPͨѶ�׽���
 * ����buf ���͵�����
 * ����len ���ݳ���
 * ����ֵС��0 ����ʧ��, ���ڵ���0 ʵ�ʷ��͵��ֽ���
 */
int HILINK_SendTcp(int fd, const unsigned char *buf, unsigned short len)
{
    return 0;
}

/*
 * ����TCP��ȡ����
 * ����fd TCPͨѶ�׽���
 * ����buf ������ݵĻ���
 * ����len Ҫ��ȡ�ĳ���
 * ����ֵС��0 ��ȡʧ��, ���ڵ���0 ʵ�ʶ�ȡ���ֽ���
 */
int HILINK_RecvTcp(int fd, unsigned char *buf, unsigned short len)
{
    return 0;
}

/*
 * �����������ֽ���ת�����ֽ���
 * ����ns �����ֽ���
 * ���������ֽ�������
 */
unsigned short HILINK_Ntohs(unsigned short ns)
{
    return 0;
}

/*
 * �����������ֽ���ת�����ֽ���
 * ����hs �����ֽ���
 * ���������ֽ�������
 */
unsigned short HILINK_Htons(unsigned short hs)
{
    return 0;
}

/*
 * ��ȡTCP���ͻ�����ʣ�໺�峤��
 * ����fd TCPͨѶ�׽���
 * ����taskType 0��ʾ������ϵͳ��m2m�����ڶ�����task��;1��ʾ������ϵͳ��m2m����������������ͬ��task��
 * ����bufLeft ���ͻ�����ʣ�໺�峤��
 * ����dataMax ����payload����󳤶�
 * ����0��ʾ�ɹ�������������ʾʧ��
 */
int HILINK_GetLeftTcpSendbuf(int fd, int *taskType, unsigned int *bufLeft, unsigned int *dataMax)
{
    return 0;
}

/*
 * �Ƴ������ŵ�
 * ����fd UDPͨѶ�׽���
 * ����0��ʾ�ɹ�������������ʾʧ��
 */
int HILINK_DeleteMultiGroupUdp(int fd)
{
    return 0;
}

/*
 * ��ȡ�������ʧ��ʱ�Ĵ�����errno
 * ����fd
 */
int HILINK_GetNetWorkError(int fd)
{
    return 0;
}

/*
 * ��ȡsocket״̬
 * ����fd TCP�׽���
 * ����flag TCP�׽���״̬,����1�ɶ�״̬;2��д״̬;4�쳣״̬
 * ����usec ״̬���ȴ�ʱ��,���˺�����Ҫ�ڸ�ʱ�������ڷ���,��λ΢��
 * ����0��ʾ�ɹ�������������ʾʧ��
 */
int HILINK_GetSocketState(int fd, unsigned int flag, unsigned int usec)
{
    return 0;
}

