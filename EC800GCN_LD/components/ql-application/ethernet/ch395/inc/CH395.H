#ifndef __CH395_H__
#define __CH395_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "ql_api_ethernet.h"

typedef void (*ch395_gpio_cb_t)(void* ctx);

ch395_gpio_cb_t ch395_get_gpio_cb(void);

bool ch395_register(ql_ethernet_ctx_s *ctx);

#ifdef __cplusplus
}
#endif
#endif

/************************** endfile **************************************/
