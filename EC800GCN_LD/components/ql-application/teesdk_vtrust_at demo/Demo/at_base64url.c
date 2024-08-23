#include "at_log.h"
#include "at_base64url.h"

// defines
#define B64_BYTE1(ptr) (((*ptr) & 0xfc) >> 2)
#define B64_BYTE2(ptr) ((((*ptr) & 0x03) << 4) | ((*(ptr + 1) & 0xf0) >> 4))
#define B64_BYTE3(ptr) (((*(ptr + 1) & 0x0f) << 2) | ((*(ptr + 2) & 0xc0) >> 6))
#define B64_BYTE4(ptr) (*(ptr + 2) & 0x3f)

// internal data

static const char *ALPHABET_B64U = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

static const uint8_t TEBAHPLA_B64[]
    = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0x3e, 0xff, 0x3e, 0xff, 0x3f, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
        0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0x1a, 0x1b, 0x1c,
        0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30,
        0x31, 0x32, 0x33, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

// internal functions
////////////////////////////////////////////////////////////////////////////////
int at_base64url_decode(const char *input, size_t inlen, uint8_t **output, size_t *outlen)
{
    if ((NULL == input) || (NULL == output) || (NULL == outlen))
    {
        ATLOGE("param error.\n");
        return -1;
    }

    // return empty string on 0 length input
    if (0 == inlen)
    {
        uint8_t *retVal = (uint8_t *)malloc(sizeof(uint8_t));
        if (NULL == retVal)
        {
            ATLOGE("malloc error.\n");
            return -1;
        }

        retVal[0] = 0;
        *output = retVal;
        *outlen = 0;
        return 0;
    }

    // extra validation -- inlen is a multiple of 4
    if (inlen % 4 == 1)
    {
        ATLOGE("param error.\n");;
        return -1;
    }

    // rlen takes a best guess on size;
    // might be too large for base64url, but never too small.
    size_t rlen = ((inlen * 3) >> 2) + 3;
    uint8_t *buffer = (uint8_t *)malloc(sizeof(uint8_t) * rlen);
    if (NULL == buffer)
    {
        ATLOGE("malloc error.\n");;
        return -1;
    }
    (void)memset(buffer, 0, rlen);

    size_t idx = 0;
    size_t pos = 0;
    size_t shift = 0;
    uint32_t packed = 0;
    while (inlen > idx)
    {
        uint8_t val;
        val = input[idx];
        if ('=' == val)
        {
            break;
        }
        else if ('+' == val || '/' == val)
        {
            ATLOGE("param error.\n");
            goto b64_decode_failed;
        }

        val = TEBAHPLA_B64[val];
        if (0xff == val)
        {
            ATLOGE("param error.\n");
            free(buffer);
            return -1;
        }
        idx++;

        packed = packed | (val << (18 - (6 * shift++)));
        if (4 == shift)
        {
            buffer[pos++] = (packed >> 16) & 0xff;
            buffer[pos++] = (packed >> 8) & 0xff;
            buffer[pos++] = packed & 0xff;
            shift = 0;
            packed = 0;
        }
    }

    assert(shift != 1);
    assert(shift != 4);

    if (shift == 3)
    {
        buffer[pos++] = (packed >> 16) & 0xff;
        buffer[pos++] = (packed >> 8) & 0xff;
    }

    if (shift == 2)
    {
        buffer[pos++] = (packed >> 16) & 0xff;
    }

    *output = buffer;
    *outlen = pos;
    assert(*outlen <= rlen);
    return 0;

b64_decode_failed:
    if (NULL != buffer)
    {
        free(buffer);
    }
    return -1;
}

////////////////////////////////////////////////////////////////////////////////
int at_base64url_encode(const uint8_t *input, size_t inlen, char **output, size_t *outlen)
{  
    if ((inlen > 0 && NULL == input) || (NULL == output) || (NULL == outlen))
    {
        ATLOGE("param error.\n");
        return -1;
    }

    // return empty string on 0 length input
    if (!inlen)
    {
        char *retVal = (char *)malloc(sizeof(char));
        if (!retVal)
        {
            ATLOGE("malloc error.\n");
            return -1;
        }
        retVal[0] = '\0';
        *output = retVal;
        *outlen = 0;
        return 0;
    }

    size_t rlen = (((inlen + 2) / 3) << 2);
    char *base;

    base = (char *)malloc(sizeof(char) * (rlen + 1));
    if (NULL == base)
    {
        ATLOGE("malloc error.\n");
        return -1;
    }
    (void)memset(base, 0, rlen + 1);

    size_t pos = 0, idx = 0;
    while ((idx + 2) < inlen)
    {
        base[pos++] = ALPHABET_B64U[0x3f & (input[idx] >> 2)];
        base[pos++] = ALPHABET_B64U[(0x3f & (input[idx] << 4)) | (0x3f & (input[idx + 1] >> 4))];
        base[pos++] = ALPHABET_B64U[(0x3f & (input[idx + 1] << 2)) | (0x3f & (input[idx + 2] >> 6))];
        base[pos++] = ALPHABET_B64U[0x3f & input[idx + 2]];
        idx += 3;
    }

    if (idx < inlen)
    {
        if ((inlen - 1) == idx)
        {
            base[pos++] = ALPHABET_B64U[0x3f & (input[idx] >> 2)];
            base[pos++] = ALPHABET_B64U[0x3f & (input[idx] << 4)];
        }
        else
        {
            base[pos++] = ALPHABET_B64U[0x3f & (input[idx] >> 2)];
            base[pos++] = ALPHABET_B64U[(0x3f & (input[idx] << 4)) | (0x3f & (input[idx + 1] >> 4))];
            base[pos++] = ALPHABET_B64U[0x3f & (input[idx + 1] << 2)];
        }
        rlen = pos;
    }
    base[rlen] = '\0';

    *output = base;
    *outlen = rlen;

    return 0;
}
