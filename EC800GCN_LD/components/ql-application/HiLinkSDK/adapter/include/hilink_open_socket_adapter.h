/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: ϵͳ���������Socket�ӿ�(���豸����ʵ��)
 */
#ifndef HILINK_OPEN_SOCKET_ADAPTER_H
#define HILINK_OPEN_SOCKET_ADAPTER_H

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_IP_LEN   40

/* udp��ȡ���ݲ����ṹ�� */
typedef struct {
    int fd;                 /* UDPͨѶ�׽��� */
    unsigned char *buf;     /* ������ݵĻ��� */
    unsigned short len;     /* Ҫ��ȡ�ĳ��� */
    char *remoteIp;              /* Զ��ip */
    unsigned short remoteIpLen;  /* Զ��ip���� */
    unsigned short *remotePort;   /* Զ�˶˿� */
} UdpRecvParam;

/*
 * ��ȡԶ��������
 * ����hostName Զ����������, url
 * ����ipList ���Զ������IP��ַ�б������
 * ����num ���Զ������IP��ַ�б������Ĵ�С
 * ����0 �ɹ�, ���� ʧ��
 * ע��Contiki OS�¸ýӿ�Ϊ�첽��ȡ, ʧ�ܺ����ٴλ�ȡ
 */
int HILINK_GetHostByName(const char *hostName, char ipList[][MAX_IP_LEN], int num);

/*
 * ����UDPͨѶ�׽���
 * ����localPort UDPͨ�Ŷ˿ں�
 * ����ֵ: ����ֵС��0 ����ʧ��, ���ڵ���0 UDP�׽���fd
 */
int HILINK_CreateUdp(unsigned short localPort);

/*
 * �ر�UDPͨѶ�׽���
 * �������int fd UDPͨѶ�׽���
 */
void HILINK_RemoveUdp(int fd);

/*
 * ����UDP��������
 * ����fd UDPͨѶ�׽���
 * ����buf ���͵�����
 * ����len ���ݳ���
 * ����remoteIp Ŀ��IP
 * ����remotePort Ŀ�Ķ˿�
 * ����ֵС��0 ����ʧ��, ���ڵ���0 ʵ�ʷ��͵��ֽ���
 */
int HILINK_SendUdp(int fd, const unsigned char *buf, unsigned short len, const char *remoteIp,
    unsigned short remotePort);

/*
 * UDP��ȡ����
 * ����udpRecvParam udp��ȡ���ݲ����ṹ��
 */
int HILINK_RecvUdp(UdpRecvParam *udpRecvParam);

/*
 * ����TCP����
 * ����dst Ŀ��IP
 * ����port Ŀ�Ķ˿�
 * ����ֵС��0 ����ʧ��, ���ڵ���0 TCP�׽���
 */
int HILINK_ConnectTcp(const char *dst, unsigned short port);

/*
 * TCP����״̬��ȡ
 * ����fd TCP�׽���
 * ����0 ��������; ��0 ����ʧ��, ����μ� hilink_socket_error_t����
 */
int HILINK_GetTcpState(int fd);

/*
 * �Ͽ�TCP����
 * ����fd TCP�׽���
 * ������� int fd  TCP �׽���
 */
void HILINK_DisconnectTcp(int fd);

/*
 * TCP��������
 * ����fd TCPͨѶ�׽���
 * ����buf ���͵�����
 * ����len ���ݳ���
 * ����ֵС��0 ����ʧ��, ���ڵ���0 ʵ�ʷ��͵��ֽ���
 */
int HILINK_SendTcp(int fd, const unsigned char *buf, unsigned short len);

/*
 * ����TCP��ȡ����
 * ����fd TCPͨѶ�׽���
 * ����buf ������ݵĻ���
 * ����len Ҫ��ȡ�ĳ���
 * ����ֵС��0 ��ȡʧ��, ���ڵ���0 ʵ�ʶ�ȡ���ֽ���
 */
int HILINK_RecvTcp(int fd, unsigned char *buf, unsigned short len);

/*
 * �����������ֽ���ת�����ֽ���
 * ����hs �����ֽ���
 * ���������ֽ�������
 */
unsigned short HILINK_Htons(unsigned short hs);

/*
 * �����������ֽ���ת�����ֽ���
 * ����ns �����ֽ���
 * ���������ֽ�������
 */
unsigned short HILINK_Ntohs(unsigned short ns);

/*
 * ��ȡTCP���ͻ�����ʣ�໺�峤��
 * ����fd TCPͨѶ�׽���
 * ����taskType 0��ʾ������ϵͳ��m2m�����ڶ�����task��;1��ʾ������ϵͳ��m2m����������������ͬ��task��
 * ����bufLeft ���ͻ�����ʣ�໺�峤��
 * ����dataMax ����payload����󳤶�
 * ����0��ʾ�ɹ�������������ʾʧ��
 */
int HILINK_GetLeftTcpSendbuf(int fd, int *taskType, unsigned int *bufLeft, unsigned int *dataMax);

/*
 * �Ƴ������ŵ�
 * ����fd UDPͨѶ�׽���
 * ����0��ʾ�ɹ�������������ʾʧ��
 */
int HILINK_DeleteMultiGroupUdp(int fd);

/*
 * ��ȡ�������ʧ��ʱ�Ĵ�����errno
 * ����fd
 */
int HILINK_GetNetWorkError(int fd);

/*
 * ��ȡsocket״̬
 * ����fd TCP�׽���
 * ����flag TCP״̬,����1�ɶ�״̬;2��д״̬;4�쳣״̬
 * ����usec ״̬���ȴ�ʱ��,���˺�����Ҫ�ڸ�ʱ�������ڷ���,��λ΢��
 * ����0��ʾ�ɹ�������������ʾʧ��
 */
int HILINK_GetSocketState(int fd, unsigned int flag, unsigned int usec);

#ifdef __cplusplus
}
#endif

#endif