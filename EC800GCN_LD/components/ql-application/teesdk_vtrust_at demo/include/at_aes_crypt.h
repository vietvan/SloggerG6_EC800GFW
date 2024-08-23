/*
 * Copyright (C) 2015-2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <assert.h>
#include <stdint.h>
//#include <lk/compiler.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <openssl/rand.h>

#define DEBUG_MAC_VALUES 0
#define AT_HMAC_SIZE 16

#ifndef STATIC_ASSERT
#define STATIC_ASSERT(e) _Static_assert(e, #e)
#endif
struct key {
    uint8_t byte[16];
};

struct mac {
    uint8_t byte[16];
};

struct iv {
    uint8_t byte[16];
};

typedef struct at_crypt_data_type {
    void* data;
    int size;
} at_crypt_data;

#define IV_INITIAL_ZERO_VALUE(iv) \
    {                             \
        { 0 }                     \
    }

#if DEBUG_MAC_VALUES
#define UINT8_16_PRINTF_STR \
    "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"
#define UINT8_16_PRINTF_ARGS(var)                                           \
    var[0], var[1], var[2], var[3], var[4], var[5], var[6], var[7], var[8], \
            var[9], var[10], var[11], var[12], var[13], var[14], var[15]
#else
#define UINT8_16_PRINTF_STR "%c"
#define UINT8_16_PRINTF_ARGS(var) '*'
#endif

#define MAC_PRINTF_STR UINT8_16_PRINTF_STR
#define MAC_PRINTF_ARGS(var) UINT8_16_PRINTF_ARGS((var)->byte)
#define IV_PRINTF_STR UINT8_16_PRINTF_STR
#define IV_PRINTF_ARGS(var) UINT8_16_PRINTF_ARGS((var)->byte)

int at_aes_hmac_encrypt(const struct key* key,
                        at_crypt_data* data_in,
                        at_crypt_data* data_out,
                        const struct iv* iv_in);

int at_aes_hmac_decrypt(const struct key* key,
                        at_crypt_data* data_in,
                        at_crypt_data* data_out,
                        const struct iv* iv_in);

int at_aes_encrypt(const struct key* key,
                   at_crypt_data* data_in,
                   at_crypt_data* data_out,
                   const struct iv* iv_in);

int at_aes_decrypt(const struct key* key,
                   at_crypt_data* data_in,
                   at_crypt_data* data_out,
                   const struct iv* iv_in);