// Copyright 2018 GoldenRiver Technology Co., Ltd. All rights reserved.
#ifndef NBL_COMMAND_H_
#define NBL_COMMAND_H_

#include <stdint.h>
#include <string.h>

#include <nbl_fwk_ree/common.h>
#include <nbl_fwk_ree/err.h>

#ifdef __cplusplus
extern "C" {
#endif

/// \nbl_group{ nbl_fwk_ree }

/// Max shared memory count in one transfer.
#define NBL_STM_MAX_SHM_COUNT 4

/// Shared memory type is normal memory.
#define NBL_SHM_TYPE_NORMAL 0x0ull

/// Shared memory type is device memory.
#define NBL_SHM_TYPE_DEVICE 0x1ull

/// Shared memory type field mask.
#define NBL_SHM_TYPE_MASK 0x7ull


/// Shared memory scope field shift count.
#define NBL_SHM_SCOPE_SHIFT 3

/// Shared memory use default scope.
/// In REE default scope is #NBL_SHM_SCOPE_REE.
/// In HEE default scope is #NBL_SHM_SCOPE_HEE.
/// In TEE default scope is #NBL_SHM_SCOPE_TEE.
#define NBL_SHM_SCOPE_DEFAULT (0x0ull)

/// Shared memory can be accessed in REE, HEE and TEE.
#define NBL_SHM_SCOPE_REE (0x1ull << NBL_SHM_SCOPE_SHIFT)

/// Shared memory can be accessed in HEE and TEE.
#define NBL_SHM_SCOPE_HEE (0x2ull << NBL_SHM_SCOPE_SHIFT)

/// Shared memory can be accessed in TEE.
#define NBL_SHM_SCOPE_TEE (0x3ull << NBL_SHM_SCOPE_SHIFT)

/// Shared memory scope field mask.
#define NBL_SHM_SCOPE_MASK (0x7ull << NBL_SHM_SCOPE_SHIFT)


/// Shared memory permission field shift count.
#define NBL_SHM_PERM_SHIFT (NBL_SHM_SCOPE_SHIFT + 3)

/// Shared memory can be read and write.
#define NBL_SHM_PERM_RW (0x0ull << NBL_SHM_PERM_SHIFT)

/// Shared memory is read-only.
#define NBL_SHM_PERM_RO (0x1ull << NBL_SHM_PERM_SHIFT)

/// Shared memory permission field mask.
#define NBL_SHM_PERM_MASK (0x7ull << NBL_SHM_PERM_SHIFT)

/**
 * \brief Allocates a block of shared memory to this process.
 *
 * The allocated shared memory is auto mapped in this method. So it is no need to
 * call nbl_shm_map() and nbl_shm_unmap() in the owner process.
 *
 * \param[in] chn Channel context that is used to send the shared memory.
 * \param[in] size Request shared memory size.
 * \param[in] flags Request shared memory mapping flags.
 * \param[out] out_ref Pointer used for receiving shared memory.
 * \param[out] out_base Pointer used for receiving shared memory mapped base address.
 * \return #NBL_NO_ERROR if success. #NBL_ERR_NO_MEMORY if there is no memory to
 *         allocated. #NBL_ERR_INVALID_ARGS if size is 0. #NBL_ERR_INVALID_ARGS if
 *         out_ref or out_base is NULL.
 * \see nbl_shm_free()
 * \see NBL_SHM_TYPE_NORMAL NBL_SHM_TYPE_DEVICE NBL_SHM_SCOPE_DEFAULT NBL_SHM_SCOPE_REE
 *      NBL_SHM_SCOPE_HEE NBL_SHM_SCOPE_TEE NBL_SHM_PERM_RW NBL_SHM_PERM_RO
 */
int nbl_shm_alloc(nbl_chn_t *chn, size_t size, uint64_t flags,
		nbl_shm_t **out_ref, void **out_base);

/**
 * \brief Frees a block of shared memory.
 *
 * The allocated shared memory is auto unmapped in this method. So it is no need to
 * call nbl_shm_map() and nbl_shm_unmap() in the owner process.
 *
 * \param[in] ref Shared memory pointer.
 */
void nbl_shm_free(nbl_shm_t *ref);

/**
 * \brief Gets a shared memory data size.
 *
 * Shared memory data size does not equal shared memory size. In some OS shared
 * memory size must be aligned to page size. So shared memory size is usually
 * larger than shared memory data size.
 *
 * \param[in] ref Shared memory pointer.
 * \param[out] out_size Shared memory data size that is set in nbl_shm_alloc().
 * \return #NBL_NO_ERROR if success. #NBL_ERR_INVALID_ARGS if ref or out_size is
 *         NULL.
 */
int nbl_shm_get_data_size(nbl_shm_t *ref, size_t *out_size);

/**
 * \brief Gets a shared memory's data pointer.
 *
 * If the caller process is not the owner of the shared memory, #nbl_shm_map must
 * be called before this method is invoked.
 *
 * \param[in] ref Shared memory pointer.
 * \param[out] out_ptr Pointer used for receiving shared memory data pointer.
 * \return #NBL_NO_ERROR if success. #NBL_ERR_INVALID_ARGS if ref or out_size if
 *         NULL. #NBL_ERR_NOT_READY if caller process is not the owner and the shared
 *         memory is not mapped.
 * \see nbl_shm_map
 */
int nbl_shm_get_data_ptr(nbl_shm_t *ref, void **out_ptr);

/**
 * \brief Creates a stream.
 *
 * Stream can serialized data for channel. It supports integer, memory buffer,
 * shared memory. A stream contains two buffers, message buffer and handle buffer.
 * Message buffer is used for transferring normal message. Handle buffer is used
 * for transferring special message(e.g. shared memory).
 *
 * \return Stream pointer #nbl_stm_t if success, NULL otherwise.
 * \see nbl_stm_destroy()
 */
nbl_stm_t *nbl_stm_create();

/**
 * \brief Destroys a stream.
 *
 * Frees message buffer and handle buffer which are created in nbl_stm_create().
 * \param[in] stream Stream pointer.
 */
void nbl_stm_destroy(nbl_stm_t *stream);

/**
 * \brief Adds an integer to a stream.
 *
 * -# Copies an integer value to the stream message buffer.
 * -# Adds sizeof(int) to stream message buffer offset.
 *
 * \param[in] stream Stream pointer.
 * \param[in] value A integer value.
 * \return #NBL_NO_ERROR if success. #NBL_ERR_INVALID_ARGS if stream is NULL. #NBL_ERR_OUT_OF_MEM
 *         if message buffer offset is greater than message buffer size.
 * \see nbl_stm_next_int()
 */
int nbl_stm_add_int(nbl_stm_t *stream, int value);

/**
 * \brief Adds data to a stream.
 *
 * -# Copies data to the stream message buffer.
 * -# Adds data size to stream message buffer offset.
 *
 * \param[in] stream Stream pointer.
 * \param[in] size Size of data.
 * \param[in] data Data pointer.
 * \return #NBL_NO_ERROR if success. #NBL_ERR_INVALID_ARGS if stream is NULL. #NBL_ERR_OUT_OF_MEM
 *         if message buffer offset is greater than message buffer size.
 * \see nbl_stm_next_buf()
 */
int nbl_stm_add_buf(nbl_stm_t *stream, int size, const void *data);

/**
 * \brief Adds data to a stream with message buffer pointer.
 *
 * -# Adds data size to stream message buffer offset.
 * -# Gets message buffer current pointer.
 *
 * \param[in] stream Stream pointer.
 * \param[in] size Size of data.
 * \param[out] out_data_ptr Pointer used for receiving message buffer current pointer.
 * \return #NBL_NO_ERROR if success. #NBL_ERR_INVALID_ARGS if stream or out_data_ptr
 *         is NULL. #NBL_ERR_OUT_OF_MEM if message buffer offset is greater than
 *         message buffer size.
 * \see nbl_stm_next_buf_ptr()
 */
int nbl_stm_add_buf_ptr(nbl_stm_t *stream, int size, void **out_data_ptr);

/**
 * \brief Adds a shared memory to a stream.
 *
 * -# Copies shared memory handle to stream handle buffer.
 * -# Increases stream handle buffer offset.
 * -# Copies shared memory info to stream message buffer.
 * -# Increases stream message buffer offset.
 *
 * \param[in] stream Stream pointer.
 * \param[in] shrmem Shared memory pointer.
 * \return #NBL_NO_ERROR if success. #NBL_ERR_INVALID_ARGS if stream or shrmem is
 *         NULL. #NBL_ERR_OUT_OF_RANGE if stream handle buffer offset greater than
 *         #NBL_STM_MAX_SHM_COUNT.#NBL_ERR_OUT_OF_MEM if message buffer offset
 *         is greater than message buffer size.
 * \see nbl_stm_next_shm()
 * \note The method can only be invoked by the shared memory owner.
 */
int nbl_stm_add_shm(nbl_stm_t *stream, nbl_shm_t *shrmem);

/**
 * \brief Gets current integer value from a stream.
 *
 * -# Copies stream message buffer current integer value to *out_value.
 * -# Increases stream message buffer offset.
 *
 * \param[in] stream Stream pointer.
 * \param[out] out_value Pointer used for receiving integer value.
 * \return #NBL_NO_ERROR if success. #NBL_ERR_INVALID_ARGS if stream or out_value is
 *         NULL. #NBL_ERR_OUT_OF_MEM if message buffer offset is greater than message
 *         buffer size.
 * \see nbl_stm_add_int()
 */
int nbl_stm_next_int(nbl_stm_t *stream, int *out_value);

/**
 * \brief Gets current data from a stream.
 *
 * -# Sets data size to *out_size.
 * -# Copies stream message buffer current data to *out_value.
 * -# Increases stream message buffer offset.
 *
 * \param[in] stream Stream pointer.
 * \param[out] out_size Pointer used for receiving size of the current data in stream.
 * \param[out] out_data Pointer used for receiving the current data in stream.
 * \return #NBL_NO_ERROR if success. #NBL_ERR_INVALID_ARGS if stream or out_size or
 *         out_data is NULL. #NBL_ERR_OUT_OF_MEM if message buffer offset is greater
 *         than message buffer size.
 * \see nbl_stm_add_buf()
 */
int nbl_stm_next_buf(nbl_stm_t *stream, int *out_size, void *out_data);

/**
 * \brief Gets current message buffer pointer from a stream.
 *
 * -# Sets data size to *out_size.
 * -# Copies stream message buffer current pointer to *out_data_ptr.
 * -# Increases stream message buffer offset.
 *
 * \param[in] stream Stream pointer.
 * \param[out] out_size Pointer used for receiving size of the current data in stream.
 * \param[out] out_data_ptr Pointer used for receiving stream messsge buffer current
 *             pointer.
 * \return #NBL_NO_ERROR if success. #NBL_ERR_INVALID_ARGS if stream or out_size or
 *         out_data_ptr is NULL. #NBL_ERR_OUT_OF_MEM if message buffer offset is greater
 *         than message buffer size.
 * \see nbl_stm_add_buf_ptr()
 */
int nbl_stm_next_buf_ptr(nbl_stm_t *stream, int *out_size, void **out_data_ptr);

/**
 * \brief Resets stream.
 *
 * -# Resets stream handle buffer offset to 0.
 * -# Resets stream message buffer offset to 0.
 *
 * \param[in] stream Stream pointer.
 * \return #NBL_NO_ERROR if success. #NBL_ERR_INVALID_ARGS if stream is NULL.
 */
int nbl_stm_reset(nbl_stm_t *stream);

#ifdef __cplusplus
}
#endif

#endif /* NBL_COMMAND_H_ */
