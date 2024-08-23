// Copyright 2018 GoldenRiver Technology Co., Ltd. All rights reserved.
#pragma once

#include <stdint.h>

#define NBL_NSC_SRV_MSG_MAX_SIZE 4080
#define NBL_NSC_LITE_PROXY_PORT "com.goldenriver.vtrust.teelite.nsc_proxy"
#define NBL_NSC_LITE_SERVICE_PORT "com.goldenriver.vtrust.teelite.nsc_srv"
#define NBL_NSC_SRV_COUNT 2
#define SECURITY_SERVICE_REQ_MAX_PAYLOAD (NBL_NSC_SRV_MSG_MAX_SIZE - sizeof(struct nsc_req))
#define SECURITY_SERVICE_RSP_MAX_PAYLOAD (NBL_NSC_SRV_MSG_MAX_SIZE - sizeof(struct nsc_rsp))

enum {
    NSC_SERVICE_CMD_INVALID = 0x0,
    NSC_SERVICE_CMD_FORWARD_REQUEST = 0x1,
    NSC_SERVICE_CMD_PROXY_INIT = 0x10001,
    NSC_SERVICE_CMD_PROXY_DEINIT = 0x10002,
    NSC_SERVICE_CMD_PROXY_REQUEST = 0x10003,
};

enum {
    NSC_SERVICE_RET_OK = 0,
    NSC_SERVICE_RET_CMD_UNKNOWN = -1,
    NSC_SERVICE_RET_INVALID_PARAMS = -2,
    NSC_SERVICE_RET_NOT_READY = -3,
    NSC_SERVICE_RET_FORWARD_REQUEST_FAILED = -4,
    NSC_SERVICE_RET_FORWARD_RESPONSE_FAILED = -5,
    NSC_SERVICE_RET_IO_ERROR = -5,
    NSC_SERVICE_RET_PROXY_RESPONSE = 0x10004,
};

enum {
    NSC_SERVICE_SUBCMD_INVALID = 0x0,
    NSC_SERVICE_SUBCMD_GET_REE_TIME = 0x10001,
    NSC_SERVICE_SUBCMD_HTTPS_GET = 0x10002,
    NSC_SERVICE_SUBCMD_HTTPS_POST = 0x10003,
    NSC_SERVICE_SUBCMD_GET_SIM_INFO = 0x10004,
};


/**
 * struct nsc_req - request structure for nsc service
 * @cmd: cmd for nsc service
 */
struct nsc_req {
    uint32_t cmd;
    uint32_t params[4];
    uint8_t payload[0];
};

/**
 * struct nsc_rsp - response structure for nsc service
 * @ret: return code for nsc service
 */
struct nsc_rsp {
    int32_t ret;
    uint8_t payload[0];
};

struct sim_info {
    uint8_t imsi[16];
    uint8_t iccid[21];
};
