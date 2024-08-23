// Copyright 2018 GoldenRiver Technology Co., Ltd. All rights reserved.
#pragma once

#include <stdint.h>

#define DRC_SERVICE_PORT "com.goldenrivertek.vtrust"
#define DRC_MSG_BUF_SIZE 0x1000

enum {
    DRC_SERVICE_CMD_INVALID = 0x0,
    DRC_SERVICE_CMD_DEV_CHECK = 0x1,
};

enum {
    DRC_SERVICE_RET_OK = 0,
    DRC_SERVICE_RET_CMD_UNKNOWN = -1,
    DRC_SERVICE_RET_INVALID_PARAMS = -2,
};

enum {
    DRC_LEVEL_1A = 0,
    DRC_LEVEL_1B = 1,
    DRC_LEVEL_2A = 2,
};

/**
 * struct drc_req - request structure for drc service
 * @cmd: cmd for drc service
 */
struct drc_req {
    uint32_t cmd;
    uint32_t params[4];
    uint8_t payload[0];
};

/**
 * struct security_rsp - response structure for security service
 * @ret: return code for security service
 */
struct drc_rsp {
    int32_t ret;
    uint8_t payload[0];
};
