
#include <at_client.h>
#include <nbl_fwk_ree/err.h>
#include <nbl_fwk_ree/log.h>
#include <nbl_fwk_ree/service.h>
#include <nbl_fwk_ree/stream.h>
#include <osi_api.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define LOG_TAG "vtrust_at_client"
extern osiMutex_t *vtee_lock;

int vtrust_at_client(void *in, size_t in_size, void *out, size_t *out_size) {
  osiMutexLock(vtee_lock);
  int rc;
  nbl_chn_t *chn;
  char *srv_name = NULL;
  if (AT_TEST_SRV) {
    srv_name = AT_SRV_TEST_NAME;
  } else {
    srv_name = AT_SRV_NAME;
  }
  if ((!in) || (!out) || (in_size > MSG_SIZE_MAX)) {
    rc = NBL_ERR_INVALID_ARGS;
    goto failed;
  }

  NBL_LOGI("at srv name %s\n", srv_name);
  rc = nbl_chn_open(srv_name, 0, &chn);
  if (rc != NBL_NO_ERROR) {
    NBL_LOGE("Connect to %s failed.\n", srv_name);
    rc = NBL_ERR_IO;
    goto connect_failed;
  }
  NBL_LOGI("in size:%d,in:%s", in_size, in);
  rc = nbl_chn_write_buf(chn, in, in_size);
  if (rc != in_size) {
    NBL_LOGE("at servier write buffer failed (%d).\n", rc);
    rc = NBL_ERR_IO;
    goto write_buf_failed;
  }
  rc = nbl_chn_read_buf(chn, out, *out_size, NBL_TIMEOUT_INFINITY);
  if (rc > *out_size) {
    rc = NBL_ERR_IO;
    NBL_LOGE("at servier read buffer failed (%d).\n", rc);
    goto read_buf_failed;
  }
  *out_size = rc;
  NBL_LOGI("buf size: %d read buf:%s\n", *out_size, out);
  rc = NBL_NO_ERROR;
read_buf_failed:
write_buf_failed:
  nbl_chn_close(chn);
connect_failed:
failed:
  osiMutexUnlock(vtee_lock);
  return rc;
}