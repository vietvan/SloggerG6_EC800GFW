// Copyright 2018 GoldenRiver Technology Co., Ltd. All rights reserved.

#ifndef NBL_COMMON_H_
#define NBL_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

/// \nbl_group{ nbl_fwk_ree }

/**
 * \brief Channel context for channel management.
 *
 * \see nbl_chn_open
 * \see nbl_chn_set_priv
 * \see nbl_chn_get_priv
 * \see nbl_chn_write_buf
 * \see nbl_chn_read_buf
 * \see nbl_chn_write_stm
 * \see nbl_chn_read_stm
 * \see nbl_chn_write_stm_sync
 * \see nbl_chn_close
 */
typedef struct nbl_chn nbl_chn_t;

/**
 * \brief Shared memory structure.
 *
 * Shared memory is used for transferring mass of data between CA and HA, CA and
 * TA, HA and HA, HA and TA, TA and TA.
 */
typedef struct nbl_shm nbl_shm_t;

/**
 * \brief Stream structure.
 *
 * Stream is used for serializing data. It contains a message buffer and handle
 * buffer. It records the current offset of two buffers.
 */
typedef struct nbl_stm nbl_stm_t;

#define NBL_API_LEVEL 1
#define NBL_MSEC 1000000ULL

int nbl_api_level();

#ifdef __cplusplus
}
#endif

#endif /* NBL_COMMON_H_ */
