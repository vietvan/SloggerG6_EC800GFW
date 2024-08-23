// Copyright 2018 GoldenRiver Technology Co., Ltd. All rights reserved.
#ifndef NBL_REE_SERVICE_H_
#define NBL_REE_SERVICE_H_

#include <stdint.h>

#include <nbl_fwk_ree/common.h>
#include <nbl_fwk_ree/stream.h>
#include <nbl_fwk_ree/err.h>
#include <nbl_fwk_ree/uuid.h>
#include <nbl_fwk_ree/log.h>

#ifdef __cplusplus
extern "C" {
#endif

/// \nbl_group{ nbl_fwk_ree }

/* Expected limits: should be in sync with kernel settings */
#define NBL_SRV_MAX_BUF_SIZE  4080 ///< Max size of per port buffer
#define NBL_SRV_MAX_BUF_COUNT     32 ///< Max number of per port buffers

#define NBL_SRV_MAX_NAME_LEN    64 ///< Max length of port path name
#define NBL_SRV_DEFAULT_BUF_NUM 8  ///< Default number of per port buffers

/**
 * \brief Common enumerations of Nebula framework.
 */
enum nbl_common {
	NBL_TIMEOUT_INFINITY = -1, ///< Used for waiting for ever.
};

/**
 * \brief NSC service handler.
 *
 * TA can send NSC request to NSC lite service. NSC lite service will route TA's
 * NSC request to this handler.
 *
 * \param[in] param0 param 0. This is sub command.
 * \param[in] param1 param 1.
 * \param[in] param2 param 2.
 * \param[in] param3 param 3.
 * \param[in] payload request payload.
 * \param[in] payload_size request payload size.
 * \param[in] ret Return code which will be returned to TA.
 * \param[in] out_buf Data buffer which will be returned to TA.
 * \param[in] buf_size out_buf size.
 * \return response payload size.
 */
typedef int (*nsc_msg_handler_t)(
        uint32_t param0, uint32_t param1, uint32_t param2, uint32_t param3,
        const void *payload, size_t payload_size,
        int32_t *ret, void *out_buf, size_t buf_size);

/**
 * \brief Opens a channel to a service with its name.
 *
 * \param[in] srv_name Service name.
 * \param[in] timeout A timeout value in milliseconds. A value of #NBL_TIMEOUT_INFINITY
 *            is an infinite timeout.
 * \param[out] out_channel Pointer that is used for receiving #nbl_chn_t.
 * \return #NBL_NO_ERROR if success. #NBL_ERR_INVALID_ARGS if port_name or out_channel
 *         is NULL. #NBL_ERR_CHANNEL_CLOSED if the target service refuse to accept.
 * \note timeout can only set 0 at this moment.
 */
int nbl_chn_open(const char *srv_name, long timeout,
		nbl_chn_t **out_channel);

/**
 * \brief Sets user defined data to a channel's private area.
 *
 * Nebula framework will not touch channel's private area. The life-cycle of the
 * data in this area must be managed by user.
 *
 * \param[in] chan The channel context.
 * \param[in] priv Pointer of the user defined data.
 * \see nbl_chn_get_priv()
 */
void nbl_chn_set_priv(nbl_chn_t *chan, void *priv);

/**
 * \brief Gets user defined data from a channel's private area.
 *
 * \param[in] chan The channel context.
 * \return Pointer of the user defined data.
 * \see nbl_chn_set_priv()
 */
void *nbl_chn_get_priv(nbl_chn_t *chan);

/**
 * \brief Writes data to channel.
 *
 * This operation is asynchronous and copies data to channel's message buffer.
 * The data use user-defined protocol.
 *
 * \param[in] chan The channel context.
 * \param[in] data Data pointer.
 * \param[in] data_size Data size.
 * \return #NBL_NO_ERROR if success. #NBL_ERR_INVALID_ARGS if chan or data is NULL.
 */
int nbl_chn_write_buf(nbl_chn_t *chan, void *data,
		uint32_t data_size);

/**
 * \brief Bound channel to specific CPU cores.
 *
 * \param[in] chan The channel context.
 * \param[in] cpumask Target cpu bit mask.
 * \return #NBL_NO_ERROR if success. #NBL_ERR_INVALID_ARGS if chan or data is NULL.
 */
int nbl_chn_set_cpumask(nbl_chn_t *chan, uint32_t cpumask);

/**
 * \brief Read data from channel.
 *
 * This operation can be synchronous if timeout is specified #NBL_TIMEOUT_INFINITY.
 * Otherwise the operation is asynchronous. The operation reads data from channel's
 * message receive buffer and copies the data to a user defined buffer.
 *
 * \param[in] chan The channel context.
 * \param[out] data Data pointer.
 * \param[in] data_size Size of the data that is to be read.
 * \param[in] timeout A timeout value in milliseconds. A value of #NBL_TIMEOUT_INFINITY
 *            is an infinite timeout.
 * \return #NBL_NO_ERROR if success. #NBL_ERR_INVALID_ARGS if chan or data is NULL.
 *         #NBL_ERR_TIMED_OUT if timeout occurs.
 * \note The data_size must be equal with or larger than the size of data that is
 *       in channel's message receive buffer.
 */
int nbl_chn_read_buf(nbl_chn_t *chan, void *data, uint32_t data_size,
		long timeout);

/**
 * \brief Writes stream to channel.
 *
 * This operation is asynchronous. Stream #nbl_chn_t contains serialized
 * data.
 *
 * \param[in] chan The channel context.
 * \param[in] stream Stream which contains serialized data.
 * \return #NBL_NO_ERROR if success. #NBL_ERR_INVALID_ARGS if chan or stream is NULL.
 */
int nbl_chn_write_stm(nbl_chn_t *chan, nbl_stm_t *stream);

/**
 * \brief Reads stream from channel.
 *
 * This operation can be synchronous if timeout is specified #NBL_TIMEOUT_INFINITY.
 * Otherwise this operation is asynchronous. The operation reads data from channel's
 * message receive buffer and serializes the data to a specified stream.
 *
 * \param[in] chan The channel context.
 * \param[in] stream Stream that is used for receive serialized data.
 * \param[in] timeout A timeout value in milliseconds. A value of #NBL_TIMEOUT_INFINITY
 *            is an infinite timeout.
 * \return #NBL_NO_ERROR if success. #NBL_ERR_INVALID_ARGS if chan or stream is NULL.
 *         #NBL_ERR_TIMED_OUT if timeout occurs.
 */
int nbl_chn_read_stm(nbl_chn_t *chan, nbl_stm_t *stream, long timeout);


/**
 * \brief Writes stream to channel and wait for response stream.
 *
 * This operation is synchronous. Stream #nbl_chn_t contains serialized
 * data.
 *
 * \param[in] chan The channel context.
 * \param[in] request The stream that is used for writing data to channel.
 * \param[in] response The stream that is usde for reading data from channel.
 * \param[in] timeout A timeout value in milliseconds. A value of #NBL_TIMEOUT_INFINITY
 *            is an infinite timeout.
 * \return #NBL_NO_ERROR if success. #NBL_ERR_INVALID_ARGS if any of chan, request
 *         and response is NULL.
 */
int nbl_chn_write_stm_sync(nbl_chn_t *chan, nbl_stm_t *request,
		nbl_stm_t *response, long timeout);

/**
 * \brief Closes channel.
 *
 * \param[in] chan The channel context.
 */
void nbl_chn_close(nbl_chn_t *chan);

/**
 * \brief Register NSC handler.
 */
void nbl_register_nsc_handler(nsc_msg_handler_t handler);

/**
 * \brief Initialize nbl_fwk.
 *
 * Initialize nbl_fwk. Must be called before using nbl_fwk functions.
 *
 * \return #NBL_NO_ERROR if success.
 */
int nbl_fwk_init(void);

/**
 * \brief Deinitialize nbl_fwk.
 *
 * Destroy all resources created in nbl_fwk_init().
 */
void nbl_fwk_deinit(void);
#ifdef __cplusplus
}
#endif

#endif /* NBL_REE_SERVICE_H_ */
