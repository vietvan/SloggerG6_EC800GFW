/* Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
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
#include <at_aes_crypt.h>
#include <at_base64url.h>
#include <at_client.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define LOG_TAG "vtrust_at_client"

char app_uuid[40] = {"186b54c8-8bd0-11ed-a8b1-cb652ea15773"};

uint8_t app_request_id[4] = {0x12, 0x43, 0x55, 0x78};

static uint8_t app_cek[16] = {0};

struct iv iv_temp = {
        .byte = {1, 67, 43, 87, 46, 67, 34, 56, 78, 97, 56, 78, 24, 16, 45, 73},
};

struct key aes_key = {
        .byte = {20, 47, 89, 56, 23, 47, 89, 52, 12, 47, 89, 56, 123, 7, 85,
                 46},
};

static int at_send_recv(void* in, size_t in_size, void* out, size_t* out_size) {
    int rc;

    nbl_chn_t* chn;
    char* srv_name = AT_SRV_NAME;

    if ((!in) || (!out) || (in_size > MSG_SIZE_MAX)) {
        rc = NBL_ERR_INVALID_ARGS;
        goto failed;
    }
    rc = nbl_chn_open(srv_name, 0, &chn);
    if (rc != NBL_NO_ERROR) {
        NBL_LOGE("Connect to %s failed.\n", srv_name);
        rc = NBL_ERR_IO;
        goto connect_failed;
    }

    rc = nbl_chn_write_buf(chn, in, in_size);
    if (rc != in_size) {
        NBL_LOGE("Service B Client write buffer failed (%d).\n", rc);
        rc = NBL_ERR_IO;
        goto write_buf_failed;
    }
    rc = nbl_chn_read_buf(chn, out, *out_size, NBL_TIMEOUT_INFINITY);
    if (rc > *out_size) {
        rc = NBL_ERR_IO;
        goto read_buf_failed;
    }
    *out_size = rc;
    NBL_LOGI("buf size: %zu read buf:%s\n", *out_size, out);
    rc = NBL_NO_ERROR;
read_buf_failed:
write_buf_failed:
    nbl_chn_close(chn);
connect_failed:
failed:
    return rc;
}

static void get_cek(uint8_t* cek) {
    memcpy(cek, app_cek, 16);
}

static void get_requset_id(uint8_t* requset_id, size_t size) {
    memcpy(requset_id, app_request_id, size);
}

static void get_uuid(char* uuid, size_t size) {
    memcpy(uuid, app_uuid, size);
}

static void bytes_to_hexstring(uint8_t* in,
                               size_t in_size,
                               char* out,
                               size_t out_size) {
    if (in == NULL || out == NULL || out_size < ((in_size * 2) + 1)) {
        return;
    }
    char* ptr = out;
    for (size_t i = 0; i < in_size; i++) {
        sprintf(ptr, "%02x", in[i]);
        ptr = ptr + 2;
    }
}

static void string_to_dec(char* in,
                          size_t in_size,
                          void* out,
                          size_t out_size) {
    uint8_t flg = 0;
    int temp = 0;

    if (in == NULL || out == NULL) {
        return;
    }
    if (*in == '-') {
        in++;
        in_size--;
        flg = 1;
    }

    for (int i = 0; i < in_size; i++) {
        temp = temp * 10 + (*in - '0');
        in++;
    }

    if (flg) {
        temp = -1 * temp;
    }
    memset(out, 0, out_size);
    if (out_size >= sizeof(temp)) {
        memcpy(out, &temp, sizeof(temp));
    } else {
        memcpy(out, &temp, out_size);
    }
}

static int at_command_encrypt(char* in,
                              size_t in_size,
                              char* out,
                              size_t* out_size) {
    int rc;
    char* cbase64url_out = NULL;
    size_t base64url_size;
    struct key cek;
    if ((in == NULL) || (out == NULL)) {
        *out_size = 0;
        return -1;
    }

    at_crypt_data crypt_in = {
            .data = in,
            .size = in_size,
    };
    at_crypt_data crypt_out;
    get_cek(cek.byte);
    rc = at_aes_hmac_encrypt(&cek, &crypt_in, &crypt_out, &iv_temp);
    if (rc < 0) {
        sprintf(out, "%s", "app aes encrypt error");
        *out_size = 0;
        return -1;
    }

    rc = at_base64url_encode((uint8_t*)crypt_out.data, crypt_out.size,
                             &cbase64url_out, &base64url_size);
    free(crypt_out.data);
    if (rc < 0) {
        sprintf(out, "%s", "app aes encrypt error");
        *out_size = 0;
        return -1;
    }
    memcpy(out, cbase64url_out, base64url_size);
    *out_size = base64url_size;
    free(cbase64url_out);
    return 0;
}

static int at_command_decrypt(char* in,
                              size_t in_size,
                              char* out,
                              size_t* out_size) {
    int rc;
    uint8_t* ubase64url_out = NULL;
    size_t base64url_size;
    struct key cek;
    if ((in == NULL) || (out == NULL)) {
        *out_size = 0;
        return -1;
    }

    rc = at_base64url_decode(in, in_size, &ubase64url_out, &base64url_size);
    if (rc < 0) {
        sprintf(out, "%s", "app aes decrypt error");
        *out_size = strlen(out);
        return -1;
    }
    at_crypt_data crypt_in = {
            .data = ubase64url_out,
            .size = base64url_size,
    };
    at_crypt_data crypt_out;
    get_cek(cek.byte);
    rc = at_aes_hmac_decrypt(&cek, &crypt_in, &crypt_out, &iv_temp);
    free(ubase64url_out);
    if (rc < 0) {
        sprintf(out, "%s", "app aes decrypt error");
        *out_size = strlen(out);
        return -1;
    }
    memcpy(out, crypt_out.data, crypt_out.size);
    free(crypt_out.data);
    return 0;
}

static int at_get_key(uint8_t* requset_id,
                      char* uuid,
                      uint8_t at_command,
                      char* out,
                      size_t* out_size) {
    char requset_id_string[8 + 1] = {0};
    if (*out_size > MSG_SIZE_MAX) {
        sprintf(out, "%s", "out size long");
        return -1;
    }
    bytes_to_hexstring(requset_id, 4, requset_id_string, 9);
    memset(out, 0, *out_size);
    sprintf(out,
            "%s,"
            "%s,"
            "%d",
            requset_id_string, uuid, at_command);
    *out_size = strlen(out);
    return 0;
}

static int at_get_rand(uint8_t* requset_id,
                       char* uuid,
                       uint8_t at_command,
                       size_t size,
                       char* out,
                       size_t* out_size) {
    int rc;
    char cry_in[10] = {0};
    char requset_id_string[8 + 1] = {0};
    size_t encrypt_out_size;
    if ((requset_id == NULL) | (uuid == NULL) || (out == NULL) ||
        (*out_size > MSG_SIZE_MAX)) {
        return -1;
    }

    memset(out, 0, *out_size);
    bytes_to_hexstring(requset_id, 4, requset_id_string, 9);
    sprintf(out,
            "%s,"
            "%s,",
            requset_id_string, uuid);

    sprintf((char*)cry_in,
            "%d,"
            "%d",
            at_command, (int)size);
    rc = at_command_encrypt(cry_in, strlen(cry_in),
                            out + strlen(requset_id_string) + strlen(uuid) + 2,
                            &encrypt_out_size);
    if (rc < 0) {
        *out_size = encrypt_out_size;
        return -1;
    }

    *out_size = strlen(requset_id_string) + strlen(uuid) + 2 + encrypt_out_size;
    return 0;
}

static int at_get_device_id(uint8_t* requset_id,
                            char* uuid,
                            uint8_t at_command,
                            char* out,
                            size_t* out_size) {
    int rc;
    char cry_in[10] = {0};
    char requset_id_string[8 + 1] = {0};
    size_t encrypt_out_size;
    if ((requset_id == NULL) | (uuid == NULL) || (out == NULL) ||
        (*out_size > MSG_SIZE_MAX)) {
        return -1;
    }

    memset(out, 0, *out_size);
    bytes_to_hexstring(requset_id, 4, requset_id_string, 9);
    sprintf(out,
            "%s,"
            "%s,",
            requset_id_string, uuid);

    sprintf((char*)cry_in, "%d", at_command);
    rc = at_command_encrypt(cry_in, strlen(cry_in),
                            out + strlen(requset_id_string) + strlen(uuid) + 2,
                            &encrypt_out_size);
    if (rc < 0) {
        *out_size = encrypt_out_size;
        return -1;
    }

    *out_size = strlen(requset_id_string) + strlen(uuid) + 2 + encrypt_out_size;
    return 0;
}

static int at_get_ree_time(uint8_t* requset_id,
                           char* uuid,
                           uint8_t at_command,
                           char* out,
                           size_t* out_size) {
    int rc;
    char cry_in[10] = {0};
    char requset_id_string[8 + 1] = {0};
    size_t encrypt_out_size;
    if ((requset_id == NULL) | (uuid == NULL) || (out == NULL) ||
        (*out_size > MSG_SIZE_MAX)) {
        return -1;
    }

    memset(out, 0, *out_size);
    bytes_to_hexstring(requset_id, 4, requset_id_string, 9);
    sprintf(out,
            "%s,"
            "%s,",
            requset_id_string, uuid);

    sprintf((char*)cry_in, "%d", at_command);
    rc = at_command_encrypt(cry_in, strlen(cry_in),
                            out + strlen(requset_id_string) + strlen(uuid) + 2,
                            &encrypt_out_size);
    if (rc < 0) {
        *out_size = encrypt_out_size;
        return -1;
    }

    *out_size = strlen(requset_id_string) + strlen(uuid) + 2 + encrypt_out_size;
    return 0;
}

static int at_get_tee_time(uint8_t* requset_id,
                           char* uuid,
                           uint8_t at_command,
                           char* out,
                           size_t* out_size) {
    int rc;
    char cry_in[10] = {0};
    char requset_id_string[8 + 1] = {0};
    size_t encrypt_out_size;
    if ((requset_id == NULL) | (uuid == NULL) || (out == NULL) ||
        (*out_size > MSG_SIZE_MAX)) {
        return -1;
    }

    memset(out, 0, *out_size);
    bytes_to_hexstring(requset_id, 4, requset_id_string, 9);
    sprintf(out,
            "%s,"
            "%s,",
            requset_id_string, uuid);

    sprintf((char*)cry_in, "%d", at_command);
    rc = at_command_encrypt(cry_in, strlen(cry_in),
                            out + strlen(requset_id_string) + strlen(uuid) + 2,
                            &encrypt_out_size);
    if (rc < 0) {
        *out_size = encrypt_out_size;
        return -1;
    }

    *out_size = strlen(requset_id_string) + strlen(uuid) + 2 + encrypt_out_size;
    return 0;
}

static int at_storge_write(uint8_t* requset_id,
                           char* uuid,
                           uint8_t at_command,
                           char* name,
                           size_t size,
                           size_t offset,
                           char* data,
                           char* out,
                           size_t* out_size) {
    int rc;
    char cry_in[1024] = {0};
    char requset_id_string[8 + 1] = {0};
    size_t encrypt_out_size;

    if ((requset_id == NULL) | (uuid == NULL) || (out == NULL) ||
        (*out_size > MSG_SIZE_MAX)) {
        return -1;
    }

    memset(out, 0, *out_size);
    bytes_to_hexstring(requset_id, 4, requset_id_string, 9);
    sprintf(out,
            "%s,"
            "%s,",
            requset_id_string, uuid);

    sprintf((char*)cry_in,
            "%d,"
            "%s,"
            "%u,"
            "%d,"
            "%s",
            at_command, name, (unsigned int)size, (unsigned int)offset, data);
    rc = at_command_encrypt(cry_in, strlen(cry_in),
                            out + strlen(requset_id_string) + strlen(uuid) + 2,
                            &encrypt_out_size);
    if (rc < 0) {
        *out_size = encrypt_out_size;
        return -1;
    }

    *out_size = strlen(requset_id_string) + strlen(uuid) + 2 + encrypt_out_size;
    return 0;
}

static int at_storge_read(uint8_t* requset_id,
                          char* uuid,
                          uint8_t at_command,
                          char* name,
                          size_t size,
                          size_t offset,
                          char* out,
                          size_t* out_size) {
    int rc;
    char cry_in[1024] = {0};
    char requset_id_string[8 + 1] = {0};
    size_t encrypt_out_size;

    if ((requset_id == NULL) | (uuid == NULL) || (out == NULL) ||
        (*out_size > MSG_SIZE_MAX)) {
        return -1;
    }

    memset(out, 0, *out_size);
    bytes_to_hexstring(requset_id, 4, requset_id_string, 9);
    sprintf(out,
            "%s,"
            "%s,",
            requset_id_string, uuid);

    sprintf((char*)cry_in,
            "%d,"
            "%s,"
            "%u,"
            "%d",
            at_command, name, (unsigned int)size, (unsigned int)offset);
    rc = at_command_encrypt(cry_in, strlen(cry_in),
                            out + strlen(requset_id_string) + strlen(uuid) + 2,
                            &encrypt_out_size);
    if (rc < 0) {
        *out_size = encrypt_out_size;
        return -1;
    }

    *out_size = strlen(requset_id_string) + strlen(uuid) + 2 + encrypt_out_size;
    return 0;
}

static int at_command_data(char* in,
                           size_t in_size,
                           void* out,
                           size_t* out_size) {
    uint8_t at_command;
    char* paragma1 = NULL;
    char* paragma2 = NULL;
    char* paragma3 = NULL;
    size_t data_size = 0;
    paragma1 = strstr(in, ",");
    NBL_LOGI("at_command string:%s\n", in);
    if (!paragma1) {
        string_to_dec(in, in_size, &at_command, sizeof(at_command));
    } else {
        string_to_dec(in, paragma1 - in, &at_command, sizeof(at_command));
    }

    NBL_LOGI("at_command:%d\n", at_command);
    switch (at_command) {
    case AT_COMMAND_GET_KEY:
        if (paragma1) {
            uint8_t* ubase64url_out = NULL;
            size_t base64url_size;
            at_base64url_decode(paragma1 + 1, in + in_size - (paragma1 + 1),
                                &ubase64url_out, &base64url_size);

            at_crypt_data cek_crypt_in = {
                    .data = ubase64url_out,
                    .size = base64url_size,
            };
            at_crypt_data cek_crypt_out;

            int rc = at_aes_hmac_decrypt(&aes_key, &cek_crypt_in,
                                         &cek_crypt_out, &iv_temp);
            free(ubase64url_out);
            if (rc < 0) {
                sprintf(out, "%s", "app aes decrypt error");
                return -1;
            }

            memcpy(&app_cek, cek_crypt_out.data, cek_crypt_out.size);
            memcpy(out, cek_crypt_out.data, cek_crypt_out.size);
            *out_size = cek_crypt_out.size;
            free(cek_crypt_out.data);
        }
        break;

    case AT_COMMAND_GET_RAND:
        paragma2 = strstr(paragma1 + 1, ",");
        if (paragma2) {
            string_to_dec(paragma1 + 1, paragma2 - (paragma1 + 1), &data_size,
                          sizeof(data_size));
            memcpy(out, paragma2 + 1, data_size);
            *out_size = data_size;
        } else {
            return -1;
        }
        break;

    case AT_COMMAND_GET_DEVICE_ID:
        paragma2 = strstr(paragma1 + 1, ",");
        if (paragma2) {
            string_to_dec(paragma1 + 1, paragma2 - (paragma1 + 1), &data_size,
                          sizeof(data_size));
            memcpy(out, paragma2 + 1, data_size);
            *out_size = data_size;
        } else {
            return -1;
        }
        break;

    case AT_COMMAND_GET_REE_TIME:
        paragma2 = strstr(paragma1 + 1, ",");
        if (paragma2) {
            string_to_dec(paragma1 + 1, paragma2 - (paragma1 + 1), &data_size,
                          sizeof(data_size));
            memcpy(out, paragma2 + 1, data_size);
            nbl_time_t* ree_time = (nbl_time_t*)out;
            *out_size = data_size;
        } else {
            return -1;
        }
        break;

    case AT_COMMAND_GET_TEE_TIME:
        paragma2 = strstr(paragma1 + 1, ",");
        if (paragma2) {
            string_to_dec(paragma1 + 1, paragma2 - (paragma1 + 1), &data_size,
                          sizeof(data_size));
            memcpy(out, paragma2 + 1, data_size);
            nbl_time_t* tee_time = (nbl_time_t*)out;
            *out_size = data_size;
        } else {
            return -1;
        }
        break;

    case AT_COMMAND_STORAGE_WRITE_FILE:
        *out_size = 0;
        break;

    case AT_COMMAND_STORAGE_READ_FILE:
        paragma2 = strstr(paragma1 + 1, ",");
        if (paragma2) {
            string_to_dec(paragma1 + 1, paragma2 - (paragma1 + 1), &data_size,
                          sizeof(data_size));
            paragma3 = strstr(paragma2 + 1, ",");
            if (paragma3) {
                size_t file_size = 0;
                string_to_dec(paragma2 + 1, paragma3 - (paragma2 + 1),
                              &file_size, sizeof(file_size));
                memcpy(out, &data_size, sizeof(data_size));
                memcpy(out + sizeof(data_size), &file_size, sizeof(file_size));
                memcpy(out + sizeof(data_size) + sizeof(file_size),
                       paragma3 + 1, data_size);
                *out_size = sizeof(data_size) + sizeof(file_size) + data_size;
            } else {
                return -1;
            }
        } else {
            return -1;
        }
        break;
    default:
        sprintf(out, "%s", "no find at command");
        *out_size = strlen(out);
        return -1;
        break;
    }
    return 0;
}

static int at_data_process(char* recv_msg,
                           size_t msg_size,
                           void* out,
                           size_t* out_size) {
    int rc;
    char* paragma1 = NULL;
    char* paragma2 = NULL;
    char* paragma3 = NULL;
    char res_request_id_string[8 + 1] = {0};
    uint8_t request_id[4];
    char requset_id_string[8 + 1] = {0};
    struct key cek;
    int8_t res;
    size_t paragma3_size;
    uint8_t* ubase64url_out = NULL;
    size_t base64url_size;
    paragma1 = strstr(recv_msg, ",");
    paragma2 = strstr(paragma1 + 1, ",");

    memset(out, 0, *out_size);
    if ((!paragma1) || (!paragma2)) {
        sprintf(out, "%s", "app recv paragma error");
        return -1;
    }

    memcpy(res_request_id_string, recv_msg, paragma1 - recv_msg);
    get_requset_id(request_id, sizeof(request_id));
    bytes_to_hexstring(request_id, 4, requset_id_string, 9);
    string_to_dec(paragma1 + 1, paragma2 - (paragma1 + 1), &res, sizeof(res));
    if (strcmp(requset_id_string, res_request_id_string) == 0) {
        if (res == 0) {
            paragma3 = strstr(paragma2 + 1, ",");
            if (paragma3) {
                paragma3_size = paragma3 - (paragma2 + 1);
            } else {
                paragma3_size = (char*)recv_msg + msg_size - (paragma2 + 1);
            }
            if (paragma3_size > 1) {
                at_base64url_decode(paragma2 + 1,
                                    recv_msg + msg_size - (paragma2 + 1),
                                    &ubase64url_out, &base64url_size);
                at_crypt_data crypt_in = {
                        .data = ubase64url_out,
                        .size = base64url_size,
                };

                at_crypt_data crypt_out;
                get_cek(cek.byte);
                rc = at_aes_hmac_decrypt(&cek, &crypt_in, &crypt_out, &iv_temp);
                free(ubase64url_out);
                if (rc < 0) {
                    sprintf(out, "%s", "app aes decrypt error");
                    *out_size = strlen(out);
                    return -1;
                }
                rc = at_command_data(crypt_out.data, crypt_out.size, out,
                                     out_size);
                if (rc < 0) {
                    return -1;
                }
                free(crypt_out.data);
            } else {
                rc = at_command_data(paragma2 + 1,
                                     recv_msg + msg_size - (paragma2 + 1), out,
                                     out_size);
                if (rc < 0) {
                    return -1;
                }
            }
        } else {
            memcpy(out, paragma2 + 1, recv_msg + msg_size - (paragma2 + 1));
            *out_size = strlen(out);
            return -1;
        }
    } else {
        sprintf(out, "%s", "tee requset id error");
        return -1;
    }
    return 0;
}

int app_at_msg(uint8_t at_command,
               void* in,
               size_t in_size,
               void* out,
               size_t* out_size) {
    char uuid[40];
    uint8_t request_id[4];
    int rc = -1;
    if (*out_size > MSG_SIZE_MAX) {
        return -1;
    }
    memset(in, 0, in_size);
    memset(out, 0, *out_size);
    get_uuid(uuid, sizeof(uuid));
    get_requset_id(request_id, sizeof(request_id));
    switch (at_command) {
    case AT_COMMAND_GET_KEY:
        rc = at_get_key(request_id, uuid, AT_COMMAND_GET_KEY, in, &in_size);
        break;
    case AT_COMMAND_GET_RAND:
        rc = at_get_rand(request_id, uuid, AT_COMMAND_GET_RAND, 32, in,
                         &in_size);
        break;
    case AT_COMMAND_GET_DEVICE_ID:
        rc = at_get_device_id(request_id, uuid, AT_COMMAND_GET_DEVICE_ID, in,
                              &in_size);
        break;
    case AT_COMMAND_GET_REE_TIME:
        rc = at_get_ree_time(request_id, uuid, AT_COMMAND_GET_REE_TIME, in,
                             &in_size);
        break;
    case AT_COMMAND_GET_TEE_TIME:
        rc = at_get_tee_time(request_id, uuid, AT_COMMAND_GET_TEE_TIME, in,
                             &in_size);
        break;
    case AT_COMMAND_STORAGE_WRITE_FILE:
        rc = at_storge_write(request_id, uuid, AT_COMMAND_STORAGE_WRITE_FILE,
                             "test", strlen("nihao"), 0, "nihao", in, &in_size);
        break;
    case AT_COMMAND_STORAGE_READ_FILE:
        rc = at_storge_read(request_id, uuid, AT_COMMAND_STORAGE_READ_FILE,
                            "test", strlen("nihao"), 0, in, &in_size);
        break;
    default:
        sprintf(out, "%s", "app no find at command");
        *out_size = strlen(out);
        return -1;
        break;
    }

    if (rc < 0) {
        return rc;
    }
    NBL_LOGI("data size: %zu,in data:%s", in_size, in);

    char msg_recv[1024];
    size_t mgs_recv_size = 1024;
    memset(msg_recv, 0, mgs_recv_size);
    rc = at_send_recv(in, in_size, msg_recv, &mgs_recv_size);
    if (rc < 0) {
        sprintf(out, "%s", "at send recv falied");
        *out_size = strlen(out);
        return rc;
    }
    rc = at_data_process(msg_recv, mgs_recv_size, out, out_size);
    if (rc < 0) {
        return rc;
    }
    return rc;
}

char out[1024];
char in[1024];

int main(void) {
    int rc;
    int i;
    size_t in_size;
    size_t out_size;
    in_size = 1024;
    out_size = 1024;

    while (1) {
        rc = app_at_msg(AT_COMMAND_GET_KEY, in, in_size, out, &out_size);
        if (rc < 0) {
            NBL_LOGI("out:%s\n", out);
        } else {
            for (i = 0; i < out_size; i++) {
                NBL_LOGI("key:0x%02x\n", out[i]);
            }
        }

        rc = app_at_msg(AT_COMMAND_GET_RAND, in, in_size, out, &out_size);
        if (rc < 0) {
            NBL_LOGI("out:%s\n", out);
        } else {
            for (i = 0; i < out_size; i++) {
                NBL_LOGI("rand:%x\n", out[i]);
            }
        }
        rc = app_at_msg(AT_COMMAND_GET_RAND, in, in_size, out, &out_size);
        if (rc < 0) {
            NBL_LOGI("out:%s\n", out);
        } else {
            for (i = 0; i < out_size; i++) {
                NBL_LOGI("rand:%x\n", out[i]);
            }
        }
        rc = app_at_msg(AT_COMMAND_GET_DEVICE_ID, in, in_size, out, &out_size);
        if (rc < 0) {
            NBL_LOGI("out:%s\n", out);
        } else {
            for (i = 0; i < out_size; i++) {
                NBL_LOGI("device id:0x%02x\n", out[i]);
            }
        }
        rc = app_at_msg(AT_COMMAND_GET_REE_TIME, in, in_size, out, &out_size);
        if (rc < 0) {
            NBL_LOGI("out:%s\n", out);
        } else {
            nbl_time_t* ree_time = (nbl_time_t*)out;
            NBL_LOGI(" ree time: %d.%d\n", ree_time->seconds, ree_time->millis);
        }
        rc = app_at_msg(AT_COMMAND_GET_TEE_TIME, in, in_size, out, &out_size);
        if (rc < 0) {
            NBL_LOGI("out:%s\n", out);
        } else {
            nbl_time_t* tee_time = (nbl_time_t*)out;
            NBL_LOGI(" tee time: %d.%d\n", tee_time->seconds, tee_time->millis);
        }

        rc = app_at_msg(AT_COMMAND_STORAGE_WRITE_FILE, in, in_size, out,
                        &out_size);
        if (rc < 0) {
            NBL_LOGI("out:%s\n", out);
        } else {
            NBL_LOGI("out:%s\n", out);
        }
        rc = app_at_msg(AT_COMMAND_STORAGE_READ_FILE, in, in_size, out,
                        &out_size);
        if (rc < 0) {
            NBL_LOGI("%s\n", out);
        } else {
            size_t read_size = 0;
            size_t file_size = 0;
            memcpy(&read_size, out, sizeof(read_size));
            memcpy(&file_size, out, sizeof(file_size));
            NBL_LOGI("out size:%zu,read size:%zu,filesize:%zu,out:%s\n ",
                     out_size, read_size, file_size,
                     out + sizeof(read_size) + sizeof(file_size));
        }
        for (int i = 0; i < 5000; i++) {
            for (int j = 0; j < 65535; j++) {
            }
        }
    }
    return 0;
}
