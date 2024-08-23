/* Copyright (C) 2021 RDA Technologies Limited and/or its affiliates("RDA").
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

#ifndef __LWIP_NAT_H__
#define __LWIP_NAT_H__

#include "lwip/err.h"
#include "lwip/ip_addr.h"
#include "lwip/opt.h"

#if IP_NAT

/** Timer interval at which to call ip_nat_tmr() */
#define LWIP_NAT_TMR_INTERVAL_SEC        (30)

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct netif;
struct pbuf;

typedef struct ip4_nat_entry
{
  ip4_addr_t    source_net;
  ip4_addr_t    source_netmask;
  ip4_addr_t    dest_net;
  ip4_addr_t    dest_netmask;
  struct netif *out_if;
  struct netif *in_if;
  uint16_t port_index;
} ip4_nat_entry_t;


void set_nat_enable(uint32_t natCfg);
bool get_nat_enabled(uint8_t nSimId, uint8_t nCid);
uint32_t get_nat_cfg(void);
void  ip4_nat_init(void);
void  ip4_nat_tmr(void);
u8_t  ip4_nat_input(struct pbuf *p);
u8_t  ip4_nat_out(struct pbuf *p);

err_t ip4_nat_add(const ip4_nat_entry_t *new_entry);
void  ip4_nat_remove(const ip4_nat_entry_t *remove_entry);
uint32_t get_nat_enable(void);

#include "lwip/netif.h"
ip4_nat_entry_t *add_static_route(ip4_addr_t dest_net, ip4_addr_t dest_netmask, struct netif *lan_netif, struct netif *wan_netif);
void remove_static_route(ip4_nat_entry_t *nat_entry);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* IP_NAT */

#endif /* __LWIP_NAT_H__ */
