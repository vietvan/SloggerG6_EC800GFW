// Copyright 2018 GoldenRiver Technology Co., Ltd. All rights reserved.
#ifndef VTRUST_H_
#define VTRUST_H_

#include <interface/drc/drc.h>
#include <stdint.h>

void drvVTEEInit(void);
void drvVTEEDeinit(void);
void vtrust_tee_test(void);

/**
 * \brief Check whether device is in secure state.
 *
 * This function may connects to drc server and wait until get response.
 *
 * param[in] drc_level DRC device check level.See #DRC_LEVEL_1A, #DRC_LEVEL_1B
 * and #DRC_LEVEL_2A. param[in] timeout timeout milliseconds. Use -1 wait for
 * ever.
 *
 * \return 1 if device is in secure state, 0 otherwise, < 0 if error occurs.
 */
int vtee_dev_check(int drc_level, int timeout);
int vtee_read_efuse(uint32_t block_index, uint8_t *out_buf);
int vtee_write_efuse(uint32_t block_index, const uint8_t *buf);
int vtrust_at_client(void *in, size_t in_size, void *out, size_t *out_size);

#endif /* VTRUST_H_ */
