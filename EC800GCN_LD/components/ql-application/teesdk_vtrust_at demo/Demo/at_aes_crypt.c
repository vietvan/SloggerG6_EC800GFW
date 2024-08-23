/*
 * Copyright (C) 2014-2016 The Android Open Source Project
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

#include "at_aes_crypt.h"
#include "at_log.h"

/*
 * The OpenSSL 1.1.0 API requires we allocate these dynamically. Cache them
 * globally to avoid alocator thrash and the potential for another dynamic
 * failure.
 */
static EVP_CIPHER_CTX *cipher_ctx;
static HMAC_CTX *hmac_ctx;

static void crypt_init(void) {
  cipher_ctx = EVP_CIPHER_CTX_new();
  assert(cipher_ctx);
}

static void hmac_init(void) {
  hmac_ctx = HMAC_CTX_new();
  assert(hmac_ctx);
}

static void crypt_shutdown(void) {
  EVP_CIPHER_CTX_free(cipher_ctx);
  cipher_ctx = NULL;
}

static void hmac_shutdown(void) {
  HMAC_CTX_free(hmac_ctx);
  hmac_ctx = NULL;
}

/**
 * crypt - Helper function for encrypt and decrypt.
 * @key:            Key object.
 * @data_in_out:    Data to encrypt or decrypt.
 * @data_size:      Number of bytes in @data_in_out.
 * @iv:             Initialization vector to use for Cipher Block Chaining.
 * @padding:        Have padding or not.
 * @encrypt:        %true to select encrypt, %false to select decrypt.
 *
 * Return: 0 on success, -1 if an error was detected.
 */
static int crypt(const struct key *key, at_crypt_data *in, at_crypt_data *out,
                 const struct iv *iv, bool padding, bool encrypt) {
  int evp_ret;
  const EVP_CIPHER *cipher;
  int out_data_size;
  size_t key_len;

  /*
   * Make sure iv is large enough. Current implementation allows static
   * check.
   * TODO: Switch to runtime check for selcted cipher if EVP_MAX_IV_LENGTH
   * changes to cover larger ivs used by other cipers.
   */
  STATIC_ASSERT(sizeof(*iv) >= EVP_MAX_IV_LENGTH);

  cipher = EVP_aes_128_cbc();
  key_len = EVP_CIPHER_key_length(cipher);
  if (key_len > sizeof(*key)) {
    ATLOGE("key too small for selected cipher, %zd < %zd\n", sizeof(*key),
           key_len);
    evp_ret = 0;
    goto err;
  }

  assert(cipher_ctx);
  EVP_CIPHER_CTX_reset(cipher_ctx);

  evp_ret =
      EVP_CipherInit_ex(cipher_ctx, cipher, NULL, key->byte, iv->byte, encrypt);
  if (!evp_ret) {
    ATLOGE("EVP_CipherInit_ex failed\n");
    goto err;
  }

  evp_ret = EVP_CIPHER_CTX_set_padding(cipher_ctx, padding);
  if (!evp_ret) {
    ATLOGE("EVP_CIPHER_CTX_set_padding failed\n");
    goto err;
  }

  evp_ret =
      EVP_CipherUpdate(cipher_ctx, out->data, &out->size, in->data, in->size);
  if (!evp_ret) {
    ATLOGE("EVP_CipherUpdate failed\n");
    goto err;
  }

  out_data_size = out->size;
  evp_ret =
      EVP_CipherFinal_ex(cipher_ctx, out->data + out_data_size, &out_data_size);
  if (!evp_ret) {
    ATLOGE("EVP_CipherFinal_ex failed\n");
    goto err;
  }
  out->size += out_data_size;

err:
  return evp_ret ? 0 : -1;
}

/**
 * calculate_mac - Calulate keyed-hash message authentication code (HMAC SHA256)
 * @key:            Key object.
 * @mac:            Mac object to return calulated mac in.
 * @data:           Data to calculate mac for.
 * @data_size:      Number of bytes in @data.
 *
 * Return: 0 on success, -1 if an error was detected.
 */
static int calculate_mac(const struct key *key, struct mac *mac,
                         const void *data, size_t data_size) {
  int hmac_ret;
  unsigned int md_len;
  unsigned char mac_buf[EVP_MAX_MD_SIZE];

  assert(hmac_ctx);
  HMAC_CTX_reset(hmac_ctx);

  hmac_ret = HMAC_Init_ex(hmac_ctx, key, sizeof(*key), EVP_sha256(), NULL);
  if (!hmac_ret) {
    fprintf(stderr, "HMAC_Init_ex failed\n");
    goto err;
  }

  hmac_ret = HMAC_Update(hmac_ctx, data, data_size);
  if (!hmac_ret) {
    fprintf(stderr, "HMAC_Update failed\n");
    goto err;
  }

  hmac_ret = HMAC_Final(hmac_ctx, mac_buf, &md_len);
  if (!hmac_ret) {
    fprintf(stderr, "HMAC_Final failed\n");
    goto err;
  }
  if (md_len < sizeof(*mac)) {
    fprintf(stderr, "bad md_len %d < %zd\n", md_len, sizeof(*mac));
    hmac_ret = 0;
    goto err;
  }
  memcpy(mac, mac_buf, sizeof(*mac));

err:
  return hmac_ret ? 0 : -1;
}

/**
 * at_aes_hmac_encrypt - Encrypt data using AES-128-CBC and HMAC.
 * @key:            Key object.
 * @data_in:        Data to encrypt.
 * @data_out:       Data to encrypt for.
 * @iv_in:          Initialization vector to use for Cipher Block Chaining.
 *
 * Return: 0 on success, -1 if an error was detected.
 */
int at_aes_hmac_encrypt(const struct key *key, at_crypt_data *data_in,
                        at_crypt_data *data_out, const struct iv *iv_in) {
  if (key == NULL || data_in == NULL || data_out == NULL || iv_in == NULL) {
    ATLOGE("encrypt param error.\n");
    goto param_err;
  }

  at_crypt_data crypt_out;

  crypt_out.size = data_in->size / 16 * 16 +
                   (1 + (data_in->size % 16 & INT_MAX) ? 1 : 0) * 16;
  crypt_out.data = (uint8_t *)malloc(sizeof(uint8_t) * crypt_out.size + 1);
  if (crypt_out.data == NULL) {
    ATLOGE("malloc error.\n");
    goto malloc_err;
  }
  memset(crypt_out.data, 0, crypt_out.size + 1);

  data_out->size = crypt_out.size + AT_HMAC_SIZE;
  data_out->data = (uint8_t *)malloc(sizeof(uint8_t) * data_out->size + 1);
  if (data_out->data == NULL) {
    ATLOGE("malloc error.\n");
    goto malloc_err;
  }
  memset(data_out->data, 0, data_out->size + 1);

  crypt_init();
  int rc = crypt(key, data_in, &crypt_out, iv_in, true, true);
  if (rc < 0) {
    ATLOGE("crypt faild.\n");
    goto crypt_err;
  }

  hmac_init();
  struct mac mac;
  rc = calculate_mac(key, &mac, crypt_out.data, crypt_out.size);
  if (rc < 0) {
    ATLOGE("calculate_mac faild.\n");
    goto hash_err;
  }

  memcpy(data_out->data, mac.byte, AT_HMAC_SIZE);
  memcpy(data_out->data + AT_HMAC_SIZE, crypt_out.data, crypt_out.size);
  data_out->size = crypt_out.size + AT_HMAC_SIZE;

  hmac_shutdown();
  crypt_shutdown();
  if (crypt_out.data != NULL) {
    free(crypt_out.data);
  }

  return 0;

hash_err:
  hmac_shutdown();
crypt_err:
  crypt_shutdown();
  if (data_out->data != NULL) {
    free(data_out->data);
  }
malloc_err:
  if (crypt_out.data != NULL) {
    free(crypt_out.data);
  }
param_err:

  return -1;
}

/**
 * at_aes_hmac_decrypt - Decrypt data using AES-128-CBC and verify HMAC.
 * @key:            Key object.
 * @data_in:        Data to decrypt.
 * @data_out:       Data to decrypt for.
 * @iv_in:          Initialization vector to use for Cipher Block Chaining.
 *
 * Return: 0 on success, -1 if an error was detected.
 */
int at_aes_hmac_decrypt(const struct key *key, at_crypt_data *data_in,
                        at_crypt_data *data_out, const struct iv *iv_in) {
  if (key == NULL || data_in == NULL || data_out == NULL || iv_in == NULL) {
    ATLOGE("encrypt param error.\n");
    goto param_err;
  }

  if((data_in->size/16 < 2) || (data_in->size%16 != 0)  || (data_in->size == 0)) {
    ATLOGE("data_in size error.\n");
    goto param_err;
  }

  hmac_init();
  struct mac mac;
  int rc = calculate_mac(key, &mac, data_in->data + AT_HMAC_SIZE,
                         data_in->size - AT_HMAC_SIZE);
  if (rc < 0) {
    ATLOGE("calculate_mac faild.\n");
    goto hash_err;
  }

  rc = memcmp(data_in->data, mac.byte, AT_HMAC_SIZE);
  if (rc != 0) {
    ATLOGE("hash error.\n");
    goto hash_err;
  }

  int decrypt_osize = data_in->size - AT_HMAC_SIZE;
  uint8_t *decrypt_odata = (uint8_t *)malloc(sizeof(uint8_t) * decrypt_osize);
  if (decrypt_odata == NULL) {
    ATLOGE("malloc error.\n");
    goto malloc_err;
  }
  memset(decrypt_odata, 0, decrypt_osize);

  data_out->size = data_in->size - AT_HMAC_SIZE;
  data_out->data = (uint8_t *)malloc(sizeof(uint8_t) * data_out->size);
  if (data_out->data == NULL) {
    ATLOGE("malloc error.\n");
    goto malloc_err;
  }

  at_crypt_data decrypt_in;
  at_crypt_data decrypt_out;

  decrypt_in.data = data_in->data + AT_HMAC_SIZE;
  decrypt_in.size = data_in->size - AT_HMAC_SIZE;
  decrypt_out.data = decrypt_odata;
  decrypt_out.size = decrypt_osize;

  crypt_init();
  rc = crypt(key, &decrypt_in, &decrypt_out, iv_in, true, false);
  if (rc < 0) {
    ATLOGE("crypt faild.\n");
    goto decrypt_err;
  }

  memset(data_out->data, 0, data_out->size);
  memcpy(data_out->data, decrypt_out.data, decrypt_out.size);
  data_out->size = decrypt_out.size;

  hmac_shutdown();
  crypt_shutdown();
  if (decrypt_odata != NULL) {
    free(decrypt_odata);
  }

  return 0;

decrypt_err:
  crypt_shutdown();
  if (data_out->data != NULL) {
    free(data_out->data);
  }
malloc_err:
  if (decrypt_odata != NULL) {
    free(decrypt_odata);
  }
hash_err:
  hmac_shutdown();
param_err:

  return -1;
}

/**
 * at_aes_encrypt - Encrypt data using AES-128-CBC.
 * @key:            Key object.
 * @data_in:        Data to encrypt.
 * @data_out:       Data to encrypt for.
 * @iv_in:          Initialization vector to use for Cipher Block Chaining.
 *
 * Return: 0 on success, -1 if an error was detected.
 */
int at_aes_encrypt(const struct key *key, at_crypt_data *data_in,
                   at_crypt_data *data_out, const struct iv *iv_in) {
  if (key == NULL || data_in == NULL || data_out == NULL || iv_in == NULL) {
    ATLOGE("encrypt param error.\n");
    goto param_err;
  }

  if((data_in->size%16 != 0) || (data_in->size == 0)) {
    ATLOGE("data_in size error.\n");
    goto param_err;
  }

  data_out->size = data_in->size;
  data_out->data = (uint8_t *)malloc(sizeof(uint8_t) * data_out->size);

  if (data_out->data == NULL) {
    ATLOGE("malloc error.\n");
    goto malloc_err;
  }
  memset(data_out->data, 0, data_out->size);

  crypt_init();
  int rc = crypt(key, data_in, data_out, iv_in, false, true);

  if (rc < 0) {
    ATLOGE("crypt faild.\n");
    goto crypt_err;
  }

  crypt_shutdown();

  return 0;

crypt_err:
  crypt_shutdown();
  if (data_out->data != NULL) {
    free(data_out->data);
  }
malloc_err:
param_err:

  return -1;
}

/**
 * at_aes_decrypt - Decrypt data using AES-128-CBC.
 * @key:            Key object.
 * @data_in:        Data to decrypt.
 * @data_out:       Data to decrypt for.
 * @iv_in:          Initialization vector to use for Cipher Block Chaining.
 *
 * Return: 0 on success, -1 if an error was detected.
 */
int at_aes_decrypt(const struct key *key, at_crypt_data *data_in,
                   at_crypt_data *data_out, const struct iv *iv_in) {
  if (key == NULL || data_in == NULL || data_out == NULL || iv_in == NULL) {
    ATLOGE("encrypt param error.\n");
    goto param_err;
  }

  if((data_in->size%16 != 0) || (data_in->size == 0)) {
    ATLOGE("data_in size error.\n");
    goto param_err;
  }

  data_out->size = data_in->size;
  data_out->data = (uint8_t *)malloc(sizeof(uint8_t) * data_out->size);
  if (data_out->data == NULL) {
    ATLOGE("malloc error.\n");
    goto malloc_err;
  }
  memset(data_out->data, 0, data_out->size);

  crypt_init();
  int rc = crypt(key, data_in, data_out, iv_in, false, false);
  if (rc < 0) {
    ATLOGE("crypt faild.\n");
    goto decrypt_err;
  }

  crypt_shutdown();

  return 0;

decrypt_err:
  crypt_shutdown();
  if (data_out->data != NULL) {
    free(data_out->data);
  }
malloc_err:
param_err:

  return -1;
}
