// Copyright 2018 GoldenRiver Technology Co., Ltd. All rights reserved.
#define OSI_LOCAL_LOG_LEVEL OSI_LOG_LEVEL_DEBUG
// #define OSI_LOG_TAG OSI_MAKE_LOG_TAG('V', 'T', 'E', 'E')

#include <osi_api.h>
#include <osi_log.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "command.h"
#include <interface/at_client/at_client.h>
#include <nbl_fwk_ree/err.h>
#include <nbl_fwk_ree/log.h>
#include <nbl_fwk_ree/service.h>
#include <nbl_fwk_ree/stream.h>
#include <vtrust.h>
#define LOG_TAG "Simple REE Client"
#define SRV_A_NAME "com.nebula.simple.tee.a"
#define SRV_B_NAME "com.nebula.simple.tee.b"
#define BUF_MSG "Hello World from REE!"

// static int _run_service_a_demo()
// {
//     int rc = 0;
//     rc = open_chn_map();
//     if (rc != NBL_NO_ERROR)
//     {
//         NBL_LOGE("open_chn_map to %d failed.\n", rc);
//         goto connect_failed;
//     }

//     rc = read_write_stm_test();
//     if (rc != NBL_NO_ERROR)
//     {
//         NBL_LOGE("read_write_stm_test to %d failed.\n", rc);
//         goto connect_failed;
//     }

//     rc = close_chn_map();
//     if (rc != NBL_NO_ERROR)
//     {
//         NBL_LOGE("open_chn_map to %d failed.\n", rc);
//         goto connect_failed;
//     }
//         NBL_LOGI("_run_service_a_demo OK.\n");
//         return 0;
// connect_failed:
// 	return 1;
// }

/*static int _run_service_b_demo() {
  int rc;

  nbl_chn_t *chn;
  char *srv_name = SRV_B_NAME;
  char buf[256];
  OSI_LOGI(0, "_run_service_b_demo.\n");
  rc = nbl_chn_open(srv_name, 0, &chn);
  if (rc != NBL_NO_ERROR) {
    NBL_LOGE("Connect to %s failed.\n", srv_name);
    goto connect_failed;
  }
  OSI_LOGI(0, "nbl_chn_open.\n");
  rc = nbl_chn_write_buf(chn, BUF_MSG, strlen(BUF_MSG) + 1);
  if (rc != strlen(BUF_MSG) + 1) {
    NBL_LOGE("Service B Client write buffer failed (%d).\n", rc);
    goto write_buf_failed;
  }
  OSI_LOGI(0, "nbl_chn_write_buf.\n");
  rc = nbl_chn_read_buf(chn, buf, sizeof(buf), NBL_TIMEOUT_INFINITY);
  if (rc != strlen(BUF_MSG) + 1) {
    OSI_LOGI(0, "nbl_chn_read_buf failed .\n");
    NBL_LOGE("Service B Client read buffer failed (%d).\n", rc);
    goto read_buf_failed;
  } else {
    NBL_LOGI("Service B read buffer (%s).\n", buf);
  }
  nbl_chn_close(chn);
  OSI_LOGI(0, "nbl_chn_read_buf.\n");
  NBL_LOGI("_run_service_b_demo OK.\n");
  return 0;
read_buf_failed:
write_buf_failed:
  nbl_chn_close(chn);
connect_failed:
  return 1;
}*/

// static int _run_tee_api_test()
// {
//     int rc;

//     nbl_chn_t *chn;
//     char *srv_name = SRV_B_NAME;
//     char buf[256];
//     rc = nbl_chn_open(srv_name, 0, &chn);
//     if (rc != NBL_NO_ERROR)
//     {
//         NBL_LOGE("Connect to %s failed.\n", srv_name);
//         goto connect_failed;
//     }

//     rc = nbl_chn_write_buf(chn, BUF_MSG, strlen(BUF_MSG) + 1);
//     if (rc != strlen(BUF_MSG) + 1)
//     {
//         NBL_LOGE("Service B Client write buffer failed (%d).\n", rc);
//         goto write_buf_failed;
//     }
//     NBL_LOGE("nbl_chn_write_buf OK.\n");
//     rc = nbl_chn_read_buf(chn, buf, sizeof(buf), NBL_TIMEOUT_INFINITY);
//     if (rc != strlen(BUF_MSG) + 1)
//     {
//         NBL_LOGE("Service B Client read buffer failed (%d).\n", rc);
//         goto read_buf_failed;
//     }
//     else
//     {
//         if(memcmp(buf,BUF_MSG,strlen(BUF_MSG)) != 0)
//         {
//             return 1;
//         }
//     }
//     NBL_LOGE("nbl_chn_read_buf OK.\n");
//     nbl_chn_close(chn);
//     // rc = open_chn_map();
//     // if (rc != NBL_NO_ERROR)
//     // {
//     //     NBL_LOGE("open_chn_map to %d failed.\n", rc);
//     //     goto connect_failed;
//     // }
//     // NBL_LOGE("open_chn_map OK.\n");
//     // rc = read_write_test();
//     // if (rc != NBL_NO_ERROR)
//     // {
//     //     NBL_LOGE("read_write_stm_test to %d failed.\n", rc);
//     //     goto connect_failed;
//     // }
//     // NBL_LOGE("read_write_test OK.\n");
//     // rc = close_chn_map();
//     // if (rc != NBL_NO_ERROR)
//     // {
//     //     NBL_LOGE("close_chn_map to %d failed.\n", rc);
//     //     goto connect_failed;
//     // }
//     // NBL_LOGE("close_chn_map OK.\n");
//     return 0;
// read_buf_failed:
// write_buf_failed:
//     nbl_chn_close(chn);
// connect_failed:
//     return 1;
// }
extern osiMutex_t *vtee_lock;
void vtrust_tee_test(void) {
  int rc;
  nbl_chn_t *chn;
  char *srv_name = SRV_B_NAME;
  char buf[256];
  for (;;) {
    osiThreadSleep(3000);
    osiMutexLock(vtee_lock);
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
    osiMutexUnlock(vtee_lock);
    //_run_service_b_demo();
  }
read_buf_failed:
write_buf_failed:
  nbl_chn_close(chn);
connect_failed:
  nbl_chn_close(chn);
  osiMutexUnlock(vtee_lock);
  NBL_LOGI("TEE API TEST FAIL.\n");
  osiThreadExit();
}
