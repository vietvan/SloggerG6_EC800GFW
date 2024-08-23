/**
 * @file
 * DNS API
 */

/**
 * lwip DNS resolver header file.

 * Author: Jim Pettinato
 *   April 2007

 * ported from uIP resolv.c Copyright (c) 2002-2003, Adam Dunkels.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef LWIP_HDR_DNS_H
#define LWIP_HDR_DNS_H

#include "quec_proj_config.h"

#include "lwip/opt.h"


#if LWIP_DNS

#include "lwip/ip_addr.h"
#include "lwip/err.h"

#ifdef __cplusplus
extern "C" {
#endif

/** DNS timer period */
#define DNS_TMR_INTERVAL          1000
#define Specificserver_DNS_TMR_INTERVAL    1000
#ifdef CONFIG_QUEC_PROJECT_FEATURE_DNS
#define DNS_MAX_ADDR_ANSWER         8	//IPV4 4个 IPV6 4个
#else
#define DNS_MAX_ADDR_ANSWER         4
#endif

/* DNS resolve types: */
#define LWIP_DNS_ADDRTYPE_IPV4      0
#define LWIP_DNS_ADDRTYPE_IPV6      1
#define LWIP_DNS_ADDRTYPE_IPV4_IPV6 2 /* try to resolve IPv4 first, try IPv6 if IPv4 fails only */
#define LWIP_DNS_ADDRTYPE_IPV6_IPV4 3 /* try to resolve IPv6 first, try IPv4 if IPv6 fails only */
#if LWIP_IPV4 && LWIP_IPV6
#ifndef LWIP_DNS_ADDRTYPE_DEFAULT
#define LWIP_DNS_ADDRTYPE_DEFAULT   LWIP_DNS_ADDRTYPE_IPV4_IPV6
#endif
#elif LWIP_IPV4
#define LWIP_DNS_ADDRTYPE_DEFAULT   LWIP_DNS_ADDRTYPE_IPV4
#else
#define LWIP_DNS_ADDRTYPE_DEFAULT   LWIP_DNS_ADDRTYPE_IPV6
#endif

#if DNS_LOCAL_HOSTLIST
/** struct used for local host-list */
struct local_hostlist_entry {
  /** static hostname */
  const char *name;
  /** static host address in network byteorder */
  ip_addr_t addr;
  struct local_hostlist_entry *next;
};
#define DNS_LOCAL_HOSTLIST_ELEM(name, addr_init) {name, addr_init, NULL}
#if DNS_LOCAL_HOSTLIST_IS_DYNAMIC
#ifndef DNS_LOCAL_HOSTLIST_MAX_NAMELEN
#define DNS_LOCAL_HOSTLIST_MAX_NAMELEN  DNS_MAX_NAME_LENGTH
#endif
#define LOCALHOSTLIST_ELEM_SIZE ((sizeof(struct local_hostlist_entry) + DNS_LOCAL_HOSTLIST_MAX_NAMELEN + 1))
#endif /* DNS_LOCAL_HOSTLIST_IS_DYNAMIC */
#endif /* DNS_LOCAL_HOSTLIST */

#if LWIP_IPV4
extern const ip_addr_t dns_mquery_v4group;
#endif /* LWIP_IPV4 */
#if LWIP_IPV6
extern const ip_addr_t dns_mquery_v6group;
#endif /* LWIP_IPV6 */

/* DNS table entry states */
typedef enum {
  DNS_STATE_UNUSED           = 0,
  DNS_STATE_NEW              = 1,
  DNS_STATE_ASKING           = 2,
  DNS_STATE_DONE             = 3
} dns_state_enum_t;

/** DNS table entry */
struct dns_table_entry {
  u32_t ttl;
  ip_addr_t ipaddr[DNS_MAX_ADDR_ANSWER];
  u16_t txid;
  u16_t simcid;
  u8_t  state;
  u8_t  server_idx;
  u8_t  tmr;
  u8_t  retries;
  u8_t  seqno;
#if ((LWIP_DNS_SECURE & LWIP_DNS_SECURE_RAND_SRC_PORT) != 0)
  u8_t pcb_idx;
#endif
  char name[DNS_MAX_NAME_LENGTH];
#if LWIP_IPV4 && LWIP_IPV6
  u8_t reqaddrtype;
#endif /* LWIP_IPV4 && LWIP_IPV6 */
#if LWIP_DNS_SUPPORT_MDNS_QUERIES
  u8_t is_mdns;
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_DNS
  int dns_type;		//期望返回dns地址类型
  int fir_num;		//针对获取IPV4/IPV6地址类型,记录第一个类型获取地址的数量
  u32_t fir_ttl;	//针对获取IPV4/IPV6地址类型,记录第一个类型获取的ttl
  int in_enqueue;   //执行dns_enqueue函数次数
#endif
};



/** Callback which is invoked when a hostname is found.
 * A function of this type must be implemented by the application using the DNS resolver.
 * @param name pointer to the name that was looked up.
 * @param ipaddr pointer to an ip_addr_t containing the IP address of the hostname,
 *        or NULL if the name could not be found (or on any other error).
 * @param callback_arg a user-specified callback argument passed to dns_gethostbyname
*/
#ifdef CONFIG_QUEC_PROJECT_FEATURE_DNS
typedef void (*dns_found_callback)(const char *name, const uint32_t ttl, const ip_addr_t *ipaddr, void *callback_arg);
#else
typedef void (*dns_found_callback)(const char *name, const ip_addr_t *ipaddr, void *callback_arg);
#endif

void             dns_init(void);
u8_t             dns_tmr(void);
u8_t             specificserver_dns_tmr(void);
void             dns_setserver(u8_t simid, u8_t cid, u8_t numdns, const ip_addr_t *dnsserver);
const ip_addr_t* dns_getserver(u8_t simid, u8_t cid, u8_t numdns);
extern void             wifi_dns_setserver(u8_t numdns, const ip_addr_t *dnsserver);
extern const ip_addr_t* wifi_dns_getserver(u8_t numdns);

err_t            dns_gethostbyname(const char *hostname, ip_addr_t *addr,
                                   dns_found_callback found, void *callback_arg);
#ifdef CONFIG_QUEC_PROJECT_FEATURE_DNS
err_t            dns_gethostbyname_addrtype(uint16_t simcid, const char *hostname, ip_addr_t *addr,
                                   dns_found_callback found, void *callback_arg,
                                   u8_t dns_addrtype);
#else
err_t            dns_gethostbyname_addrtype(const char *hostname, ip_addr_t *addr,
                                   dns_found_callback found, void *callback_arg,
                                   u8_t dns_addrtype);
#endif
err_t            specificserver_dns_gethostbyname_addrtype(const char *hostname, ip_addr_t *addr, dns_found_callback found,
                                   void *callback_arg, u8_t dns_addrtype);
#ifdef CONFIG_QUEC_PROJECT_FEATURE_DNS
err_t            dns_getallhostbyname_addrtype(uint16_t simcid, const char *hostname, uint32_t *ttl, ip_addr_t *addr, dns_found_callback found,
                           void *callback_arg, u8_t dns_addrtype);
#else
err_t            dns_getallhostbyname_addrtype(const char *hostname, ip_addr_t *addr, dns_found_callback found,
                           void *callback_arg, u8_t dns_addrtype);
#endif
void dns_clean_entries(void);
void dns_clean_entry_byname(char *hostname);
const struct dns_table_entry * dns_get_table(char* hostname, u8_t dns_addrtype);

#if DNS_LOCAL_HOSTLIST
size_t         dns_local_iterate(dns_found_callback iterator_fn, void *iterator_arg);
err_t          dns_local_lookup(const char *hostname, ip_addr_t *addr, u8_t dns_addrtype);
#if DNS_LOCAL_HOSTLIST_IS_DYNAMIC
int            dns_local_removehost(const char *hostname, const ip_addr_t *addr);
err_t          dns_local_addhost(const char *hostname, const ip_addr_t *addr);
#endif /* DNS_LOCAL_HOSTLIST_IS_DYNAMIC */
#endif /* DNS_LOCAL_HOSTLIST */

#ifdef __cplusplus
}
#endif

#endif /* LWIP_DNS */

#endif /* LWIP_HDR_DNS_H */
