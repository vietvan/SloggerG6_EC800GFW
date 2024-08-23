#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vtrust.h>
#include <nbl_fwk_ree/service.h>
#include <nbl_fwk_ree/stream.h>
#include <nbl_fwk_ree/log.h>
#include <nbl_fwk_ree/err.h>
#include "command.h"

#define LOG_TAG "Simple REE Client"
#define SRV_GRT_CRYPTO_SRV_TEXT_MAX 1024
#define SRV_A_NAME "com.nebula.simple.tee.a"

enum {
  CMD_MAP_SHM = 1,
  CMD_UNMAP_SHM = 2,
  CMD_STM_TEST = 3,
  CMD_SEND_INT = 4,
  CMD_SEND_DATA = 5,
};

static nbl_chn_t *chn;
static nbl_stm_t *request;
static nbl_stm_t *response;
// static nbl_shm_t *shm = NULL;
static uint8_t shm_base[SRV_GRT_CRYPTO_SRV_TEXT_MAX];
static int ack;

int open_chn_map(void) {
  const char *srv_name = SRV_A_NAME;
  int rc = nbl_chn_open(srv_name, 0, &chn);
  if (rc != NBL_NO_ERROR) {
    NBL_LOGE("Connect to %s failed %d.\n", srv_name,rc);
    goto connect_failed;
  }
  request = nbl_stm_create();
  response = nbl_stm_create();
  (void)memset(shm_base, 0U, SRV_GRT_CRYPTO_SRV_TEXT_MAX);

  int cmd = CMD_MAP_SHM;
  nbl_stm_add_int(request, cmd);
  rc = nbl_chn_write_stm_sync(chn, request, response, NBL_TIMEOUT_INFINITY);
  if (rc != NBL_NO_ERROR) {
    NBL_LOGE("Invoke command (%d) failed.\n", cmd);
  } else {
    nbl_stm_next_int(response, &ack);
  }
  return rc;
connect_failed:
  return rc;
}

int close_chn_map(void) {
  int cmd = CMD_UNMAP_SHM;
  nbl_stm_add_int(request, cmd);
  int rc = nbl_chn_write_stm_sync(chn, request, response, NBL_TIMEOUT_INFINITY);
  if (rc != NBL_NO_ERROR) {
    NBL_LOGE("Invoke command (%d) failed.\n", cmd);
  } else {
    nbl_stm_next_int(response, &ack);
  }

  nbl_stm_destroy(request);
  nbl_stm_destroy(response);
  nbl_chn_close(chn);

  return NBL_NO_ERROR;
}

int read_write_test1(void) {
  int cmd = CMD_STM_TEST;
  char data[] = "Hello Tee!!!";
  int data_size = strlen(data);

  nbl_stm_add_int(request, cmd);
  nbl_stm_add_buf_ptr(request, data_size, (void*)&data);
  int rc = nbl_chn_write_stm_sync(chn, request, response, NBL_TIMEOUT_INFINITY);
  if (rc != NBL_NO_ERROR) {
    NBL_LOGE("Invoke command (%d) failed.\n", cmd);
    return NBL_ERR_GENERIC;
  } else {
    nbl_stm_next_int(response, &cmd);
    memset(data,0x00,data_size);
    data_size = 0;
    nbl_stm_next_buf_ptr(response, &data_size, (void*)&data);
    nbl_stm_next_int(response, &rc);
    if (rc != NBL_NO_ERROR) {
      NBL_LOGE("nbl_stm_next_int failed %d\n", rc);
    }
  }
  // NBL_LOGE("nbl_chn_write_stm_sync OK.\n");
  return NBL_NO_ERROR;
}

int read_write_test2(void) {
  int cmd = CMD_SEND_INT;
  int out_data_size = 0;
  nbl_stm_reset(request);
  nbl_stm_add_int(request, cmd);
  int data_size = 0x10;
  nbl_stm_add_int(request, data_size);
  nbl_chn_write_stm(chn, request);
  // NBL_LOGE("nbl_chn_write_stm OK.\n");
  nbl_stm_reset(response);
  int rc = nbl_chn_read_stm(chn, response, NBL_TIMEOUT_INFINITY);
  if (rc != NBL_NO_ERROR) {
		NBL_LOGE("Read stream failed (%d).\n", rc);
    return NBL_ERR_GENERIC;
	}else{
    nbl_stm_next_int(response, &out_data_size);
    if(out_data_size != data_size)
    {
      return NBL_ERR_GENERIC;
    }
  }
  // NBL_LOGE("nbl_chn_read_stm OK.\n");
  return NBL_NO_ERROR;
}