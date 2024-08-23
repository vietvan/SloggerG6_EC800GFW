/*
 * Copyright (c) 2008-2014 Travis Geiselbrecht
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef NBL_ERR_H_
#define NBL_ERR_H_

#ifdef __cplusplus
extern "C" {
#endif

/// \nbl_group{ nbl_fwk_ree }

#define NBL_NO_ERROR                (0)
#define NBL_ERR_GENERIC             (-1)
#define NBL_ERR_NOT_FOUND           (-2)
#define NBL_ERR_NOT_READY           (-3)
#define NBL_ERR_NO_MSG              (-4)
#define NBL_ERR_NO_MEMORY           (-5)
#define NBL_ERR_ALREADY_STARTED     (-6)
#define NBL_ERR_NOT_VALID           (-7)
#define NBL_ERR_INVALID_ARGS        (-8)
#define NBL_ERR_NOT_ENOUGH_BUFFER   (-9)
#define NBL_ERR_NOT_SUSPENDED       (-10)
#define NBL_ERR_OBJECT_DESTROYED    (-11)
#define NBL_ERR_NOT_BLOCKED         (-12)
#define NBL_ERR_TIMED_OUT           (-13)
#define NBL_ERR_ALREADY_EXISTS      (-14)
#define NBL_ERR_CHANNEL_CLOSED      (-15)
#define NBL_ERR_OFFLINE             (-16)
#define NBL_ERR_NOT_ALLOWED         (-17)
#define NBL_ERR_BAD_PATH            (-18)
#define NBL_ERR_ALREADY_MOUNTED     (-19)
#define NBL_ERR_IO                  (-20)
#define NBL_ERR_NOT_DIR             (-21)
#define NBL_ERR_NOT_FILE            (-22)
#define NBL_ERR_RECURSE_TOO_DEEP    (-23)
#define NBL_ERR_NOT_SUPPORTED       (-24)
#define NBL_ERR_TOO_BIG             (-25)
#define NBL_ERR_CANCELLED           (-26)
#define NBL_ERR_NOT_IMPLEMENTED     (-27)
#define NBL_ERR_CHECKSUM_FAIL       (-28)
#define NBL_ERR_CRC_FAIL            (-29)
#define NBL_ERR_CMD_UNKNOWN         (-30)
#define NBL_ERR_BAD_STATE           (-31)
#define NBL_ERR_BAD_LEN             (-32)
#define NBL_ERR_BUSY                (-33)
#define NBL_ERR_THREAD_DETACHED     (-34)
#define NBL_ERR_I2C_NACK            (-35)
#define NBL_ERR_ALREADY_EXPIRED     (-36)
#define NBL_ERR_OUT_OF_RANGE        (-37)
#define NBL_ERR_NOT_CONFIGURED      (-38)
#define NBL_ERR_NOT_MOUNTED         (-39)
#define NBL_ERR_FAULT               (-40)
#define NBL_ERR_NO_RESOURCES        (-41)
#define NBL_ERR_BAD_HANDLE          (-42)
#define NBL_ERR_ACCESS_DENIED       (-43)
#define NBL_ERR_PARTIAL_WRITE       (-44)

#define NBL_ERR_CREATE_HANDLE       (-1001)
#define NBL_ERR_CREATE_CHILD_HANDLE (-1002)
#define NBL_ERR_MMAP                (-1003)
#define NBL_ERR_OUT_OF_MEM          (-1004)
#define NBL_ERR_CONFLICT_ARGS       (-1005)
#define NBL_ERR_TA_RUN_FAILED       (-1006)

#ifdef __cplusplus
}
#endif

#endif /* NBL_ERR_H_ */
