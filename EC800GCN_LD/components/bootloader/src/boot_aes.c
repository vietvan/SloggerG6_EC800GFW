/* Copyright (C) 2017 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "boot_aes.h"
#include "boot_platform.h"

enum
{
    AES_STATECOLS = 4, /* columns in the state & expanded key */
    AES_KEYCOLS = 4,   /* columns in a key */
    AES_ROUNDS = 10,   /* rounds in encryption */

    AES_KEY_LENGTH = 128 / 8,
    AES_EXPAND_KEY_LENGTH = 4 * AES_STATECOLS * (AES_ROUNDS + 1),
};

/* combined Xtimes2[Sbox[]] */
static const uint8_t x2_sbox[256] = {
    0xc6, 0xf8, 0xee, 0xf6, 0xff, 0xd6, 0xde, 0x91,
    0x60, 0x02, 0xce, 0x56, 0xe7, 0xb5, 0x4d, 0xec,
    0x8f, 0x1f, 0x89, 0xfa, 0xef, 0xb2, 0x8e, 0xfb,
    0x41, 0xb3, 0x5f, 0x45, 0x23, 0x53, 0xe4, 0x9b,
    0x75, 0xe1, 0x3d, 0x4c, 0x6c, 0x7e, 0xf5, 0x83,
    0x68, 0x51, 0xd1, 0xf9, 0xe2, 0xab, 0x62, 0x2a,
    0x08, 0x95, 0x46, 0x9d, 0x30, 0x37, 0x0a, 0x2f,
    0x0e, 0x24, 0x1b, 0xdf, 0xcd, 0x4e, 0x7f, 0xea,
    0x12, 0x1d, 0x58, 0x34, 0x36, 0xdc, 0xb4, 0x5b,
    0xa4, 0x76, 0xb7, 0x7d, 0x52, 0xdd, 0x5e, 0x13,
    0xa6, 0xb9, 0x00, 0xc1, 0x40, 0xe3, 0x79, 0xb6,
    0xd4, 0x8d, 0x67, 0x72, 0x94, 0x98, 0xb0, 0x85,
    0xbb, 0xc5, 0x4f, 0xed, 0x86, 0x9a, 0x66, 0x11,
    0x8a, 0xe9, 0x04, 0xfe, 0xa0, 0x78, 0x25, 0x4b,
    0xa2, 0x5d, 0x80, 0x05, 0x3f, 0x21, 0x70, 0xf1,
    0x63, 0x77, 0xaf, 0x42, 0x20, 0xe5, 0xfd, 0xbf,
    0x81, 0x18, 0x26, 0xc3, 0xbe, 0x35, 0x88, 0x2e,
    0x93, 0x55, 0xfc, 0x7a, 0xc8, 0xba, 0x32, 0xe6,
    0xc0, 0x19, 0x9e, 0xa3, 0x44, 0x54, 0x3b, 0x0b,
    0x8c, 0xc7, 0x6b, 0x28, 0xa7, 0xbc, 0x16, 0xad,
    0xdb, 0x64, 0x74, 0x14, 0x92, 0x0c, 0x48, 0xb8,
    0x9f, 0xbd, 0x43, 0xc4, 0x39, 0x31, 0xd3, 0xf2,
    0xd5, 0x8b, 0x6e, 0xda, 0x01, 0xb1, 0x9c, 0x49,
    0xd8, 0xac, 0xf3, 0xcf, 0xca, 0xf4, 0x47, 0x10,
    0x6f, 0xf0, 0x4a, 0x5c, 0x38, 0x57, 0x73, 0x97,
    0xcb, 0xa1, 0xe8, 0x3e, 0x96, 0x61, 0x0d, 0x0f,
    0xe0, 0x7c, 0x71, 0xcc, 0x90, 0x06, 0xf7, 0x1c,
    0xc2, 0x6a, 0xae, 0x69, 0x17, 0x99, 0x3a, 0x27,
    0xd9, 0xeb, 0x2b, 0x22, 0xd2, 0xa9, 0x07, 0x33,
    0x2d, 0x3c, 0x15, 0xc9, 0x87, 0xaa, 0x50, 0xa5,
    0x03, 0x59, 0x09, 0x1a, 0x65, 0xd7, 0x84, 0xd0,
    0x82, 0x29, 0x5a, 0x1e, 0x7b, 0xa8, 0x6d, 0x2c};

/* combined Xtimes3[Sbox[]] */
static const uint8_t x3_sbox[256] = {
    0xa5, 0x84, 0x99, 0x8d, 0x0d, 0xbd, 0xb1, 0x54,
    0x50, 0x03, 0xa9, 0x7d, 0x19, 0x62, 0xe6, 0x9a,
    0x45, 0x9d, 0x40, 0x87, 0x15, 0xeb, 0xc9, 0x0b,
    0xec, 0x67, 0xfd, 0xea, 0xbf, 0xf7, 0x96, 0x5b,
    0xc2, 0x1c, 0xae, 0x6a, 0x5a, 0x41, 0x02, 0x4f,
    0x5c, 0xf4, 0x34, 0x08, 0x93, 0x73, 0x53, 0x3f,
    0x0c, 0x52, 0x65, 0x5e, 0x28, 0xa1, 0x0f, 0xb5,
    0x09, 0x36, 0x9b, 0x3d, 0x26, 0x69, 0xcd, 0x9f,
    0x1b, 0x9e, 0x74, 0x2e, 0x2d, 0xb2, 0xee, 0xfb,
    0xf6, 0x4d, 0x61, 0xce, 0x7b, 0x3e, 0x71, 0x97,
    0xf5, 0x68, 0x00, 0x2c, 0x60, 0x1f, 0xc8, 0xed,
    0xbe, 0x46, 0xd9, 0x4b, 0xde, 0xd4, 0xe8, 0x4a,
    0x6b, 0x2a, 0xe5, 0x16, 0xc5, 0xd7, 0x55, 0x94,
    0xcf, 0x10, 0x06, 0x81, 0xf0, 0x44, 0xba, 0xe3,
    0xf3, 0xfe, 0xc0, 0x8a, 0xad, 0xbc, 0x48, 0x04,
    0xdf, 0xc1, 0x75, 0x63, 0x30, 0x1a, 0x0e, 0x6d,
    0x4c, 0x14, 0x35, 0x2f, 0xe1, 0xa2, 0xcc, 0x39,
    0x57, 0xf2, 0x82, 0x47, 0xac, 0xe7, 0x2b, 0x95,
    0xa0, 0x98, 0xd1, 0x7f, 0x66, 0x7e, 0xab, 0x83,
    0xca, 0x29, 0xd3, 0x3c, 0x79, 0xe2, 0x1d, 0x76,
    0x3b, 0x56, 0x4e, 0x1e, 0xdb, 0x0a, 0x6c, 0xe4,
    0x5d, 0x6e, 0xef, 0xa6, 0xa8, 0xa4, 0x37, 0x8b,
    0x32, 0x43, 0x59, 0xb7, 0x8c, 0x64, 0xd2, 0xe0,
    0xb4, 0xfa, 0x07, 0x25, 0xaf, 0x8e, 0xe9, 0x18,
    0xd5, 0x88, 0x6f, 0x72, 0x24, 0xf1, 0xc7, 0x51,
    0x23, 0x7c, 0x9c, 0x21, 0xdd, 0xdc, 0x86, 0x85,
    0x90, 0x42, 0xc4, 0xaa, 0xd8, 0x05, 0x01, 0x12,
    0xa3, 0x5f, 0xf9, 0xd0, 0x91, 0x58, 0x27, 0xb9,
    0x38, 0x13, 0xb3, 0x33, 0xbb, 0x70, 0x89, 0xa7,
    0xb6, 0x22, 0x92, 0x20, 0x49, 0xff, 0x78, 0x7a,
    0x8f, 0xf8, 0x80, 0x17, 0xda, 0x31, 0xc6, 0xb8,
    0xc3, 0xb0, 0x77, 0x11, 0xcb, 0xfc, 0xd6, 0x3a};

/* forward s-box */
static const uint8_t sbox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5,
    0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0,
    0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc,
    0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a,
    0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0,
    0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b,
    0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85,
    0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
    0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17,
    0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88,
    0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c,
    0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9,
    0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6,
    0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
    0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94,
    0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68,
    0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16};

/*
 * encrypt/decrypt columns of the key
 * n.b. you can replace this with byte-wise xor if you wish.
 */
static void _addRoundKey(uint32_t *state, uint32_t *key)
{
    int idx;

    for (idx = 0; idx < 4; idx++)
        state[idx] ^= key[idx];
}

static const uint8_t rcon[11] = {
    0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};
/* produce AES_STATECOLS bytes for each round */
static void _aesExpandKey(uint8_t *key, uint8_t *expkey)
{
    uint8_t tmp0, tmp1, tmp2, tmp3, tmp4;
    uint32_t idx;

    memcpy(expkey, key, AES_KEYCOLS * 4);

    for (idx = AES_KEYCOLS; idx < AES_STATECOLS * (AES_ROUNDS + 1); idx++)
    {
        tmp0 = expkey[4 * idx - 4];
        tmp1 = expkey[4 * idx - 3];
        tmp2 = expkey[4 * idx - 2];
        tmp3 = expkey[4 * idx - 1];
        if (!(idx % AES_KEYCOLS))
        {
            tmp4 = tmp3;
            tmp3 = sbox[tmp0];
            tmp0 = sbox[tmp1] ^ rcon[idx / AES_KEYCOLS];
            tmp1 = sbox[tmp2];
            tmp2 = sbox[tmp4];
        }
        else if ((AES_KEYCOLS > 6) && (idx % AES_KEYCOLS == 4))
        {
            tmp0 = sbox[tmp0];
            tmp1 = sbox[tmp1];
            tmp2 = sbox[tmp2];
            tmp3 = sbox[tmp3];
        }

        expkey[4 * idx + 0] = expkey[4 * idx - 4 * AES_KEYCOLS + 0] ^ tmp0;
        expkey[4 * idx + 1] = expkey[4 * idx - 4 * AES_KEYCOLS + 1] ^ tmp1;
        expkey[4 * idx + 2] = expkey[4 * idx - 4 * AES_KEYCOLS + 2] ^ tmp2;
        expkey[4 * idx + 3] = expkey[4 * idx - 4 * AES_KEYCOLS + 3] ^ tmp3;
    }
}

/* recombine and mix each row in a column */
static void _mixSubColumns(uint8_t *state)
{
    uint8_t tmp[4 * AES_STATECOLS];

    /* mixing column 0 */
    tmp[0] = x2_sbox[state[0]] ^ x3_sbox[state[5]] ^
             sbox[state[10]] ^ sbox[state[15]];
    tmp[1] = sbox[state[0]] ^ x2_sbox[state[5]] ^
             x3_sbox[state[10]] ^ sbox[state[15]];
    tmp[2] = sbox[state[0]] ^ sbox[state[5]] ^
             x2_sbox[state[10]] ^ x3_sbox[state[15]];
    tmp[3] = x3_sbox[state[0]] ^ sbox[state[5]] ^
             sbox[state[10]] ^ x2_sbox[state[15]];

    /* mixing column 1 */
    tmp[4] = x2_sbox[state[4]] ^ x3_sbox[state[9]] ^
             sbox[state[14]] ^ sbox[state[3]];
    tmp[5] = sbox[state[4]] ^ x2_sbox[state[9]] ^
             x3_sbox[state[14]] ^ sbox[state[3]];
    tmp[6] = sbox[state[4]] ^ sbox[state[9]] ^
             x2_sbox[state[14]] ^ x3_sbox[state[3]];
    tmp[7] = x3_sbox[state[4]] ^ sbox[state[9]] ^
             sbox[state[14]] ^ x2_sbox[state[3]];

    /* mixing column 2 */
    tmp[8] = x2_sbox[state[8]] ^ x3_sbox[state[13]] ^
             sbox[state[2]] ^ sbox[state[7]];
    tmp[9] = sbox[state[8]] ^ x2_sbox[state[13]] ^
             x3_sbox[state[2]] ^ sbox[state[7]];
    tmp[10] = sbox[state[8]] ^ sbox[state[13]] ^
              x2_sbox[state[2]] ^ x3_sbox[state[7]];
    tmp[11] = x3_sbox[state[8]] ^ sbox[state[13]] ^
              sbox[state[2]] ^ x2_sbox[state[7]];

    /* mixing column 3 */
    tmp[12] = x2_sbox[state[12]] ^ x3_sbox[state[1]] ^
              sbox[state[6]] ^ sbox[state[11]];
    tmp[13] = sbox[state[12]] ^ x2_sbox[state[1]] ^
              x3_sbox[state[6]] ^ sbox[state[11]];
    tmp[14] = sbox[state[12]] ^ sbox[state[1]] ^
              x2_sbox[state[6]] ^ x3_sbox[state[11]];
    tmp[15] = x3_sbox[state[12]] ^ sbox[state[1]] ^
              sbox[state[6]] ^ x2_sbox[state[11]];

    memcpy(state, tmp, sizeof(tmp));
}

/*
 * Exchanges columns in each of 4 rows
 * row0 - unchanged, row1- shifted left 1,
 * row2 - shifted left 2 and row3 - shifted left 3
 */
static void _shiftRows(uint8_t *state)
{
    uint8_t tmp;

    /* just substitute row 0 */
    state[0] = sbox[state[0]];
    state[4] = sbox[state[4]];
    state[8] = sbox[state[8]];
    state[12] = sbox[state[12]];

    /* rotate row 1 */
    tmp = sbox[state[1]];
    state[1] = sbox[state[5]];
    state[5] = sbox[state[9]];
    state[9] = sbox[state[13]];
    state[13] = tmp;

    /* rotate row 2 */
    tmp = sbox[state[2]];
    state[2] = sbox[state[10]];
    state[10] = tmp;
    tmp = sbox[state[6]];
    state[6] = sbox[state[14]];
    state[14] = tmp;

    /* rotate row 3 */
    tmp = sbox[state[15]];
    state[15] = sbox[state[11]];
    state[11] = sbox[state[7]];
    state[7] = sbox[state[3]];
    state[3] = tmp;
}

/* encrypt one 128 bit block */
static void _aesEncrypt(uint8_t *in, uint8_t *expkey, uint8_t *out)
{
    uint8_t state[AES_STATECOLS * 4];
    uint32_t round;

    memcpy(state, in, AES_STATECOLS * 4);
    _addRoundKey((uint32_t *)state, (uint32_t *)expkey);

    for (round = 1; round < AES_ROUNDS + 1; round++)
    {
        if (round < AES_ROUNDS)
            _mixSubColumns(state);
        else
            _shiftRows(state);

        _addRoundKey((uint32_t *)state,
                     (uint32_t *)expkey + round * AES_STATECOLS);
    }

    memcpy(out, state, sizeof(state));
}

void aesEncryptObj(void *buf, uint32_t len, void *key)
{
    unsigned int num_aes_blocks;
    unsigned char key_schedule[AES_EXPAND_KEY_LENGTH];
    int i;
    unsigned char *src, *dst;
    _aesExpandKey((uint8_t *)key, key_schedule);

    num_aes_blocks = (len + AES_KEY_LENGTH - 1) / AES_KEY_LENGTH;
    src = dst = (unsigned char *)buf;

    for (i = 0; i < num_aes_blocks; i++)
    {
        _aesEncrypt(src, key_schedule, dst);

        /* Update pointers for next loop. */
        src += AES_KEY_LENGTH;
        dst += AES_KEY_LENGTH;
    }
}
