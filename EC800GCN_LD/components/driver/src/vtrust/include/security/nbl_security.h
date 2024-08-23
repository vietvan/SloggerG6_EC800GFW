// Copyright 2018 GoldenRiver Technology Co., Ltd. All rights reserved.
#pragma once
#include <stdint.h>

int vtee_read_efuse_sync(uint32_t block_index, uint8_t *out_buf);
int vtee_write_efuse_sync(uint32_t block_index, const uint8_t *buf);
