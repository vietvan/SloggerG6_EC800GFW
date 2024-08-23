// Copyright 2018 GoldenRiver Technology Co., Ltd. All rights reserved.
#define OSI_LOCAL_LOG_LEVEL OSI_LOG_LEVEL_DEBUG
#define OSI_LOG_TAG_V OSI_MAKE_LOG_TAG('V', 'T', 'D', 'V')

#include <stdio.h>
#include <string.h>

#include <osi_api.h>
//#include <http_api.h>
#include <netutils.h>

#include <drc/drc.h>
#include <interface/nbl_nsc/nbl_nsc_lite.h>
#include <nbl_fwk_ree/err.h>
#include <nbl_fwk_ree/log.h>
#include <nbl_fwk_ree/service.h>
#include <security/nbl_security.h>

#define LOG_TAG "vtee driver"
#define VTEE_WORK_QUEUE_STACK_SIZE 4096

struct drc_param {
  int cmd;
  uint32_t params[4];
  int ret;
};

static int initialized;
static osiWorkQueue_t *vtee_wq;
osiMutex_t *vtee_lock;

static int nsc_msg_handler(uint32_t param0, uint32_t param1, uint32_t param2,
                           uint32_t param3, const void *payload,
                           size_t payload_size, int32_t *ret, void *out_buf,
                           size_t buf_size) {
  int rc;
  int rsp_payload_size;

  switch (param0) {
  case NSC_SERVICE_SUBCMD_GET_SIM_INFO: {
    struct sim_info *sim = out_buf;
    uint8_t len;
    memset(sim, 0, sizeof(*sim));

    len = sizeof(sim->imsi);
    rc = getSimImsi(param1, sim->imsi, &len);
    if (!rc || len != 15) {
      *ret = NSC_SERVICE_RET_NOT_READY;
      rsp_payload_size = 0;
      break;
    }

    len = sizeof(sim->iccid);
    rc = getSimIccid(param1, sim->iccid, &len);
    if (!rc || len != 20) {
      *ret = NSC_SERVICE_RET_NOT_READY;
      rsp_payload_size = 0;
      break;
    }

    *ret = NSC_SERVICE_RET_OK;
    rsp_payload_size = sizeof(*sim);
    break;
  }
  case NSC_SERVICE_SUBCMD_HTTPS_GET: {
    // TODO implement http get
    OSI_LOGI(0, "nsc_msg_handler param: %d %d %d %d\n", param0, param1, param2,
             param3);

    int receive_count = 0;
    for (int i = 0; i < payload_size; i++) {
      if (((char *)payload)[i] == i % 256) {
        receive_count++;
      }
    }
    OSI_LOGI(0, "nsc_msg_handler payload_size: %ld receive_count:%d\n",
             payload_size, receive_count);

    OSI_LOGI(0, "nsc_msg_handler buf_size: %ld\n", buf_size);
    *ret = 0x12345678;
    for (int i = 0; i < buf_size; i++) {
      ((char *)out_buf)[i] = 0xaa;
    }
    rsp_payload_size = buf_size;
    break;
  }
  case NSC_SERVICE_SUBCMD_HTTPS_POST: {
    // TODO implement http post
    *ret = NSC_SERVICE_RET_CMD_UNKNOWN;
    rsp_payload_size = 0;
    break;
  }
  default:
    *ret = NSC_SERVICE_RET_CMD_UNKNOWN;
    rsp_payload_size = 0;
  }
  return rsp_payload_size;
}

void drvVTEEInit(void) {
  if (initialized) {
    return;
  }

  nbl_register_nsc_handler(nsc_msg_handler);
  nbl_fwk_init();

  vtee_wq = osiWorkQueueCreate("vtee-works", 1, OSI_PRIORITY_NORMAL,
                               VTEE_WORK_QUEUE_STACK_SIZE);
  if (!vtee_wq) {
    OSI_LOGE(0, "Initialize vtee work queue failed.\n");
    goto create_wq_failed;
  }

  vtee_lock = osiMutexCreate();
  if (!vtee_lock) {
    OSI_LOGE(0, "Initialize vtee lock failed.\n");
    goto create_lock_failed;
  }

  initialized = 1;

  return;

create_lock_failed:
create_wq_failed:
  nbl_fwk_deinit();
  return;
}

void drvVTEEDeinit(void) {
  if (vtee_lock) {
    osiMutexDelete(vtee_lock);
    vtee_lock = NULL;
  }
  if (vtee_wq) {
    osiWorkQueueDelete(vtee_wq);
    vtee_wq = NULL;
  }
  nbl_fwk_deinit();
  nbl_register_nsc_handler(NULL);
  initialized = 0;
}

static void handle_drc_work(void *ctx) {
  struct drc_param *param = (struct drc_param *)ctx;

  osiMutexLock(vtee_lock);

  switch (param->cmd) {
  case DRC_SERVICE_CMD_DEV_CHECK:
    param->ret = nbl_drc_dev_check_sync(param->params[0]);
    break;
  default:
    param->ret = -1;
  }

  osiMutexUnlock(vtee_lock);
}

int vtee_dev_check(int drc_level, int timeout) {
  int rc = 0;

  if (!initialized) {
    return -1;
  }

  struct drc_param *param = (struct drc_param *)calloc(1, sizeof(*param));
  if (!param) {
    rc = -1;
    goto create_param_failed;
  }

  param->cmd = DRC_SERVICE_CMD_DEV_CHECK;
  param->params[0] = drc_level;

  osiWork_t *drc_work = osiWorkCreate(handle_drc_work, NULL, param);
  if (!drc_work) {
    rc = -2;
    goto create_work_failed;
  }
  if (timeout == -1) {
    timeout = OSI_WAIT_FOREVER;
  }

  rc = osiWorkEnqueue(drc_work, vtee_wq);
  if (!rc) {
    rc = -3;
    goto enque_work_failed;
  }

  rc = osiWorkWaitFinish(drc_work, timeout);
  if (!rc) {
    rc = -4;
    goto exec_work_failed;
  }

  rc = param->ret;

exec_work_failed:
enque_work_failed:
  osiWorkDelete(drc_work);
  drc_work = NULL;
create_work_failed:
  free(param);
  param = NULL;
create_param_failed:
  return rc;
}

int vtee_read_efuse(uint32_t block_index, uint8_t *out_buf) {
  int rc = 0;

  if (!initialized) {
    return 0;
  }

  osiMutexLock(vtee_lock);

  rc = vtee_read_efuse_sync(block_index, out_buf);
  if (rc == NBL_NO_ERROR) {
    rc = 1;
  } else {
    rc = 0;
  }

  osiMutexUnlock(vtee_lock);

  return rc;
}

int vtee_write_efuse(uint32_t block_index, const uint8_t *buf) {
  int rc = 0;

  if (!initialized) {
    return 0;
  }

  osiMutexLock(vtee_lock);

  rc = vtee_write_efuse_sync(block_index, buf);
  if (rc == NBL_NO_ERROR) {
    rc = 1;
  } else {
    rc = 0;
  }

  osiMutexUnlock(vtee_lock);

  return rc;
}

/*#include "command.h"

#define LOG_TAG "Simple REE Client"
#define SRV_A_NAME "com.nebula.simple.tee.a"
#define SRV_B_NAME "com.nebula.simple.tee.b"
#define BUF_MSG "Hello World from REE!"

int vtrust_tee_verification() {
  int rc;
  nbl_chn_t *chn;
  char *srv_name = SRV_B_NAME;
  char buf[256];
  rc = nbl_chn_open(srv_name, 0, &chn);
  if (rc != NBL_NO_ERROR) {
    NBL_LOGE("Connect to %s failed.\n", srv_name);
    goto connect_failed;
  }
  // NBL_LOGE("nbl_chn_open OK.\n");
  rc = nbl_chn_write_buf(chn, BUF_MSG, strlen(BUF_MSG) + 1);
  if (rc != strlen(BUF_MSG) + 1) {
    NBL_LOGE("Service B Client write buffer failed (%d).\n", rc);
    goto write_buf_failed;
  }
  // NBL_LOGE("nbl_chn_write_buf OK.\n");
  rc = nbl_chn_read_buf(chn, buf, sizeof(buf), NBL_TIMEOUT_INFINITY);
  if (rc != strlen(BUF_MSG) + 1) {
    NBL_LOGE("Service B Client read buffer failed (%d).\n", rc);
    goto read_buf_failed;
  } else {
    if (memcmp(buf, BUF_MSG, strlen(BUF_MSG)) != 0) {
      NBL_LOGI("TEE API TEST FAIL.\n");
      return;
    }
  }
  // NBL_LOGE("nbl_chn_read_buf OK.\n");
  nbl_chn_close(chn);

  // NBL_LOGE("nbl_chn_close OK.\n");
  rc = open_chn_map();
  if (rc != NBL_NO_ERROR) {
    NBL_LOGE("open_chn_map to %d failed.\n", rc);
    goto connect_failed;
  }

  // NBL_LOGE("open_chn_map OK.\n");
  rc = read_write_test1();
  if (rc != NBL_NO_ERROR) {
    NBL_LOGE("read_write_test1 to %d failed.\n", rc);
    goto connect_failed;
  }
  // NBL_LOGE("read_write_test1 OK.\n");
  rc = close_chn_map();
  if (rc != NBL_NO_ERROR) {
    NBL_LOGE("close_chn_map to %d failed.\n", rc);
    goto connect_failed;
  }
  // NBL_LOGE("close_chn_map OK.\n");

  rc = open_chn_map();
  if (rc != NBL_NO_ERROR) {
    NBL_LOGE("open_chn_map to %d failed.\n", rc);
    goto connect_failed;
  }
  rc = read_write_test2();
  if (rc != NBL_NO_ERROR) {
    NBL_LOGE("read_write_test2 to %d failed.\n", rc);
    goto connect_failed;
  }
  // NBL_LOGE("read_write_test2 OK.\n");
  rc = close_chn_map();
  if (rc != NBL_NO_ERROR) {
    NBL_LOGE("close_chn_map to %d failed.\n", rc);
    goto connect_failed;
  }
  // NBL_LOGE("close_chn_map OK.\n");

  NBL_LOGE("TEE API TEST OK.\n");
  return 0;
read_buf_failed:
write_buf_failed:
  nbl_chn_close(chn);
connect_failed:
  nbl_chn_close(chn);
  NBL_LOGI("TEE API TEST FAIL.\n");
  return -1;
}*/