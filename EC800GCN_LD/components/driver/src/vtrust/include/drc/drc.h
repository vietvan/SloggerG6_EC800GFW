// Copyright 2018 GoldenRiver Technology Co., Ltd. All rights reserved.
#pragma once

#include <interface/drc/drc.h>

/**
 * \brief Check whether device is in secure state.
 *
 * This function may connects to drc server and wait until get response.
 *
 * param[in] drc_level DRC device check level.See #DRC_LEVEL_1A, #DRC_LEVEL_1B and #DRC_LEVEL_2A.
 *
 * \return 1 if device is in secure state, 0 otherwise.
 */
int nbl_drc_dev_check_sync(int drc_level);
