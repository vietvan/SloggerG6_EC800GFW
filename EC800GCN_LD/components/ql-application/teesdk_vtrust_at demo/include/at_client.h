#ifndef _AT_CLIENT_H_
#define _AT_CLIENT_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AT_SRV_NAME "com.grt.nebula.at.srv"
#define MSG_SIZE_MAX 1024
#define FILE_NAME_SIZE 50
#define DEVICE_ID_SIZE 16

enum at_command {
    AT_COMMAND_GET_KEY = 1,
    AT_COMMAND_GET_RAND,
    AT_COMMAND_GET_DEVICE_ID,
    AT_COMMAND_GET_REE_TIME,
    AT_COMMAND_GET_TEE_TIME,
    AT_COMMAND_STORAGE_WRITE_FILE,
    AT_COMMAND_STORAGE_READ_FILE,
    AT_COMMAND_END = 0xFF,
};

typedef struct {
    uint32_t seconds;
    uint32_t millis;
} nbl_time_t;

#endif