/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: ���ɰ�ȫ����������Ҳ���ʵ�֣�ֱ�ӵ���
 */
#ifndef HILINK_SEC_RANDOM_H
#define HILINK_SEC_RANDOM_H

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*RandEntropy)(unsigned int *entropyValue);

/*
 * ע��Ӳ����Դ,���Ҳ���ʵ�ָýӿڣ�ֻ����øýӿ�
 */
int HILINK_RegisterRandomEntropy(RandEntropy randEntropy);

/*
 * ���ģ����Ӳ����Դ���ڵ���HILINK_RegisterRandomEntropy�ӿ�ע����Դ��
 * ���øýӿ����ɰ�ȫ�����, �䷶ΧΪ[0,2^32),���Ҳ���ʵ�ָýӿڣ�ֻ����øýӿ�
 * ����securityRandom���ڱ������ɵİ�ȫ�����
 * ����ֵ: �ɹ�����0, ʧ�ܷ��ط�0ֵ
 */
int HILINK_SecRandom(unsigned int *securityRandom);

#ifdef __cplusplus
}
#endif

#endif