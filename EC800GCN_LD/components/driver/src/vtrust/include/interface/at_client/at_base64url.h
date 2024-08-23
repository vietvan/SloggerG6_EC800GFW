/*!
 * Copyrights
 *
 * 
 */

#ifndef AT_BASE64URL_H
#define AT_BASE64URL_H

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

int at_base64url_decode(const char *input, size_t inlen, uint8_t **output, size_t *outlen);
int at_base64url_encode(const uint8_t *input, size_t inlen, char **output, size_t *outlen);

#endif
