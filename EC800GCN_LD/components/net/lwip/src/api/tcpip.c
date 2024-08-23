/**
 * @file
 * Sequential API Main thread module
 *
 */

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
#include "quec_proj_config.h"
#include "lwip/opt.h"

extern bool sys_post_event_to_APS(void *msg, uint32_t msg_type);

#if !NO_SYS /* don't build if not configured for use in lwipopts.h */

#include "lwip/priv/tcpip_priv.h"
#include "lwip/sys.h"
#include "lwip/memp.h"
#include "lwip/mem.h"
#include "lwip/init.h"
#include "lwip/ip.h"
#include "lwip/pbuf.h"
#include "lwip/etharp.h"
#include "netif/ethernet.h"

#define TCPIP_MSG_VAR_REF(name)     API_VAR_REF(name)
#define TCPIP_MSG_VAR_DECLARE(name) API_VAR_DECLARE(struct tcpip_msg, name)
#define TCPIP_MSG_VAR_ALLOC(name)   API_VAR_ALLOC(struct tcpip_msg, MEMP_TCPIP_MSG_API, name, ERR_MEM)
#define TCPIP_MSG_VAR_FREE(name)    API_VAR_FREE(MEMP_TCPIP_MSG_API, name)

/* global variables */
#if !USE_CUSTOMER_THREAD
static tcpip_init_done_fn tcpip_init_done;
static void *tcpip_init_done_arg;
static sys_mbox_t tcpip_mbox;

#endif

#if LWIP_TCPIP_CORE_LOCKING
/** The global semaphore to lock the stack. */
sys_mutex_t lock_tcpip_core;
#endif /* LWIP_TCPIP_CORE_LOCKING */

static void tcpip_thread_handle_msg(struct tcpip_msg *msg);

#if !LWIP_TIMERS
/* wait for a message with timers disabled (e.g. pass a timer-check trigger into tcpip_thread) */
#define TCPIP_MBOX_FETCH(mbox, msg) sys_mbox_fetch(mbox, msg)
#else /* !LWIP_TIMERS */
/* wait for a message, timeouts are processed while waiting */
#define TCPIP_MBOX_FETCH(mbox, msg) tcpip_timeouts_mbox_fetch(mbox, msg)
/**
 * Wait (forever) for a message to arrive in an mbox.
 * While waiting, timeouts are processed.
 *
 * @param mbox the mbox to fetch the message from
 * @param msg the place to store the message
 */
static void
tcpip_timeouts_mbox_fetch(sys_mbox_t *mbox, void **msg)
{
  u32_t sleeptime, res;

again:
  LWIP_ASSERT_CORE_LOCKED();

  sleeptime = sys_timeouts_sleeptime();
  if (sleeptime == SYS_TIMEOUTS_SLEEPTIME_INFINITE) {
    UNLOCK_TCPIP_CORE();
    sys_arch_mbox_fetch(mbox, msg, 0);
    LOCK_TCPIP_CORE();
    return;
  } else if (sleeptime == 0) {
    sys_check_timeouts();
    /* We try again to fetch a message from the mbox. */
    goto again;
  }

  UNLOCK_TCPIP_CORE();
  res = sys_arch_mbox_fetch(mbox, msg, sleeptime);
  LOCK_TCPIP_CORE();
  if (res == SYS_ARCH_TIMEOUT) {
    /* If a SYS_ARCH_TIMEOUT value is returned, a timeout occurred
       before a message could be fetched. */
    sys_check_timeouts();
    /* We try again to fetch a message from the mbox. */
    goto again;
  }
}
#endif /* !LWIP_TIMERS */

/**
 * The main lwIP thread. This thread has exclusive access to lwIP core functions
 * (unless access to them is not locked). Other threads communicate with this
 * thread using message boxes.
 *
 * It also starts all the timers to make sure they are running in the right
 * thread context.
 *
 * @param arg unused argument
 */
 #if !USE_CUSTOMER_THREAD

static void
tcpip_thread(void *arg)
{
  struct tcpip_msg *msg;
  LWIP_UNUSED_ARG(arg);

  LWIP_MARK_TCPIP_THREAD();

  LOCK_TCPIP_CORE();
  #if !USE_CUSTOMER_THREAD
  if (tcpip_init_done != NULL) {
    tcpip_init_done(tcpip_init_done_arg);
  }
  #endif
  tcpip_thread_handle_msg(msg);
#if !USE_CUSTOMER_THREAD
  while (1) {                          /* MAIN Loop */
    LWIP_TCPIP_THREAD_ALIVE();
    /* wait for a message, timeouts are processed while waiting */
    TCPIP_MBOX_FETCH(&tcpip_mbox, (void **)&msg);
    if (msg == NULL) {
      LWIP_DEBUGF(TCPIP_DEBUG, (0x100076c0, "tcpip_thread: invalid message: NULL\n"));
      LWIP_ASSERT("tcpip_thread: invalid message", 0);
      continue;
    }
    tcpip_thread_handle_msg(msg);
  }
  #endif
}
 #else
void tcpip_thread(void *arg)
{
  struct tcpip_msg *msg = (struct tcpip_msg *)arg;
  if (msg == NULL) {
    LWIP_DEBUGF(TCPIP_DEBUG, (0x100076c0, "tcpip_thread: invalid message: NULL\n"));
    return;
  }
  LOCK_TCPIP_CORE();
  tcpip_thread_handle_msg(msg);
  UNLOCK_TCPIP_CORE();

}
#endif

/* Handle a single tcpip_msg
 * This is in its own function for access by tests only.
 */
static void
tcpip_thread_handle_msg(struct tcpip_msg *msg)
{
  switch (msg->type) {
#if !LWIP_TCPIP_CORE_LOCKING
    case TCPIP_MSG_API:
      LWIP_DEBUGF(TCPIP_DEBUG, (0x08101154, "tcpip_thread: API message %p\n", (void *)msg));
      msg->msg.api_msg.function(msg->msg.api_msg.msg);
      break;
    case TCPIP_MSG_API_CALL:
      LWIP_DEBUGF(TCPIP_DEBUG, (0x100076c1, "tcpip_thread: API CALL message %p\n", (void *)msg));
      msg->msg.api_call.arg->err = msg->msg.api_call.function(msg->msg.api_call.arg);
      sys_sem_signal(msg->msg.api_call.sem);
      break;
    case TCPIP_MSG_CALLBACK_STATIC_WAIT:
      LWIP_DEBUGF(TCPIP_DEBUG, (0x1000972d, "tcpip_thread: CALLBACK WAIT message %p\n", (void *)msg));
      msg->msg.cb_wait.function(msg->msg.cb_wait.ctx);
      sys_sem_signal(msg->msg.cb_wait.sem);
      break;
#endif /* !LWIP_TCPIP_CORE_LOCKING */

#if !LWIP_TCPIP_CORE_LOCKING_INPUT
    case TCPIP_MSG_INPKT:
      LWIP_DEBUGF(TCPIP_DEBUG, (0x100076c2, "tcpip_thread: PACKET %p\n", (void *)msg));
      if (msg->msg.inp.input_fn(msg->msg.inp.p, msg->msg.inp.netif) != ERR_OK) {
        pbuf_free(msg->msg.inp.p);
      }
      memp_free(MEMP_TCPIP_MSG_INPKT, msg);
      break;
#endif /* !LWIP_TCPIP_CORE_LOCKING_INPUT */

#if LWIP_TCPIP_TIMEOUT && LWIP_TIMERS
    case TCPIP_MSG_TIMEOUT:
      LWIP_DEBUGF(TCPIP_DEBUG, (0x100076c3, "tcpip_thread: TIMEOUT %p\n", (void *)msg));
      sys_timeout(msg->msg.tmo.msecs, msg->msg.tmo.h, msg->msg.tmo.arg);
      memp_free(MEMP_TCPIP_MSG_API, msg);
      break;
    case TCPIP_MSG_UNTIMEOUT:
      LWIP_DEBUGF(TCPIP_DEBUG, (0x100076c4, "tcpip_thread: UNTIMEOUT %p\n", (void *)msg));
      sys_untimeout(msg->msg.tmo.h, msg->msg.tmo.arg);
      memp_free(MEMP_TCPIP_MSG_API, msg);
      break;
#endif /* LWIP_TCPIP_TIMEOUT && LWIP_TIMERS */

    case TCPIP_MSG_CALLBACK:
      LWIP_DEBUGF(TCPIP_DEBUG, (0x08101156, "tcpip_thread: CALLBACK %p\n", (void *)msg));
      msg->msg.cb.function(msg->msg.cb.ctx);
      memp_free(MEMP_TCPIP_MSG_API, msg);
      break;

    case TCPIP_MSG_CALLBACK_STATIC:
      LWIP_DEBUGF(TCPIP_DEBUG, (0x100076c5, "tcpip_thread: CALLBACK_STATIC %p\n", (void *)msg));
      msg->msg.cb.function(msg->msg.cb.ctx);
      break;

    default:
      LWIP_DEBUGF(TCPIP_DEBUG, (0x100076c6, "tcpip_thread: invalid message: %d\n", msg->type));
      LWIP_ASSERT("tcpip_thread: invalid message", 0);
      break;
  }
}

#ifdef TCPIP_THREAD_TEST
/** Work on queued items in single-threaded test mode */
int
tcpip_thread_poll_one(void)
{
  int ret = 0;
 #if !USE_CUSTOMER_THREAD
  struct tcpip_msg *msg;

  if (sys_arch_mbox_tryfetch(&tcpip_mbox, (void **)&msg) != SYS_MBOX_EMPTY) {
    LOCK_TCPIP_CORE();
    if (msg != NULL) {
      tcpip_thread_handle_msg(msg);
      ret = 1;
    }
    UNLOCK_TCPIP_CORE();
  }
  #endif
  return ret;
}
#endif

/**
 * Pass a received packet to tcpip_thread for input processing
 *
 * @param p the received packet
 * @param inp the network interface on which the packet was received
 * @param input_fn input function to call
 */
err_t
tcpip_inpkt(struct pbuf *p, struct netif *inp, netif_input_fn input_fn)
{
#if LWIP_TCPIP_CORE_LOCKING_INPUT
  err_t ret;
  LWIP_DEBUGF(TCPIP_DEBUG, (0x100076c7, "tcpip_inpkt: PACKET %p/%p\n", (void *)p, (void *)inp));
  LOCK_TCPIP_CORE();
  ret = input_fn(p, inp);
  UNLOCK_TCPIP_CORE();
  return ret;
#else /* LWIP_TCPIP_CORE_LOCKING_INPUT */
  struct tcpip_msg *msg;

#if !USE_CUSTOMER_THREAD
  LWIP_ASSERT("Invalid mbox", sys_mbox_valid_val(tcpip_mbox));
#endif
  msg = (struct tcpip_msg *)memp_malloc(MEMP_TCPIP_MSG_INPKT);
  if (msg == NULL) {
    return ERR_MEM;
  }

  msg->type = TCPIP_MSG_INPKT;
  msg->msg.inp.p = p;
  msg->msg.inp.netif = inp;
  msg->msg.inp.input_fn = input_fn;
  #if !USE_CUSTOMER_THREAD
  if (sys_mbox_trypost(&tcpip_mbox, msg) != ERR_OK) {
    memp_free(MEMP_TCPIP_MSG_INPKT, msg);
    return ERR_MEM;
  }
  #else
  if (!sys_post_event_to_APS(msg, TCPIP_MSG_INPKT)) {
    memp_free(MEMP_TCPIP_MSG_INPKT, msg);
    return ERR_MEM;
  }
  #endif
  return ERR_OK;
#endif /* LWIP_TCPIP_CORE_LOCKING_INPUT */
}

/**
 * @ingroup lwip_os
 * Pass a received packet to tcpip_thread for input processing with
 * ethernet_input or ip_input. Don't call directly, pass to netif_add()
 * and call netif->input().
 *
 * @param p the received packet, p->payload pointing to the Ethernet header or
 *          to an IP header (if inp doesn't have NETIF_FLAG_ETHARP or
 *          NETIF_FLAG_ETHERNET flags)
 * @param inp the network interface on which the packet was received
 */
#ifdef CONFIG_QUEC_PROJECT_FEATURE_GPRS_DATA_TRANSFER
extern bool ql_gprs_data_transfer_get_support(void);
extern unsigned char ql_gprs_data_transfer_get_sim_cid(void);
extern void ql_gprs_data_transfer_input(void *payload, unsigned short tot_len);
#endif
err_t
tcpip_input(struct pbuf *p, struct netif *inp)
{
#ifdef CONFIG_QUEC_PROJECT_FEATURE_GPRS_DATA_TRANSFER
  if(ql_gprs_data_transfer_get_support() == 1)
  {
	  if(inp->sim_cid == ql_gprs_data_transfer_get_sim_cid())
	  {
		  ql_gprs_data_transfer_input(p->payload,p->tot_len);
          pbuf_free(p);
		  return ERR_OK;
	  }
  }
#endif

#if LWIP_ETHERNET
  if (inp->flags & (NETIF_FLAG_ETHARP | NETIF_FLAG_ETHERNET)) {
    return tcpip_inpkt(p, inp, ethernet_input);
  } else
#endif /* LWIP_ETHERNET */
    return tcpip_inpkt(p, inp, ip_input);
}

/**
 * @ingroup lwip_os
 * Call a specific function in the thread context of
 * tcpip_thread for easy access synchronization.
 * A function called in that way may access lwIP core code
 * without fearing concurrent access.
 * Blocks until the request is posted.
 * Must not be called from interrupt context!
 *
 * @param function the function to call
 * @param ctx parameter passed to f
 * @return ERR_OK if the function was called, another err_t if not
 *
 * @see tcpip_try_callback
 */
err_t
tcpip_callback(tcpip_callback_fn function, void *ctx)
{
  struct tcpip_msg *msg;
#if !USE_CUSTOMER_THREAD
  LWIP_ASSERT("Invalid mbox", sys_mbox_valid_val(tcpip_mbox));
#endif
  msg = (struct tcpip_msg *)memp_malloc(MEMP_TCPIP_MSG_API);
  if (msg == NULL) {
    return ERR_MEM;
  }

  msg->type = TCPIP_MSG_CALLBACK;
  msg->msg.cb.function = function;
  msg->msg.cb.ctx = ctx;
#if !USE_CUSTOMER_THREAD
  sys_mbox_post(&tcpip_mbox, msg);
#else
  if (!sys_post_event_to_APS(msg, TCPIP_MSG_CALLBACK)) {
    return ERR_MEM;
  }
#endif
  return ERR_OK;
}

/**
 * @ingroup lwip_os
 * Call a specific function in the thread context of
 * tcpip_thread for easy access synchronization.
 * A function called in that way may access lwIP core code
 * without fearing concurrent access.
 * Does NOT block when the request cannot be posted because the
 * tcpip_mbox is full, but returns ERR_MEM instead.
 * Can be called from interrupt context.
 *
 * @param function the function to call
 * @param ctx parameter passed to f
 * @return ERR_OK if the function was called, another err_t if not
 *
 * @see tcpip_callback
 */
err_t
tcpip_try_callback(tcpip_callback_fn function, void *ctx)
{
  struct tcpip_msg *msg;
#if !USE_CUSTOMER_THREAD
  LWIP_ASSERT("Invalid mbox", sys_mbox_valid_val(tcpip_mbox));
#endif
  msg = (struct tcpip_msg *)memp_malloc(MEMP_TCPIP_MSG_API);
  if (msg == NULL) {
    return ERR_MEM;
  }

  msg->type = TCPIP_MSG_CALLBACK;
  msg->msg.cb.function = function;
  msg->msg.cb.ctx = ctx;

#if !USE_CUSTOMER_THREAD
  if (sys_mbox_trypost(&tcpip_mbox, msg) != ERR_OK) {
    memp_free(MEMP_TCPIP_MSG_API, msg);
    return ERR_MEM;
  }
#else
  sys_post_event_to_APS(msg, TCPIP_MSG_CALLBACK);
#endif
  return ERR_OK;
}

#if LWIP_TCPIP_TIMEOUT && LWIP_TIMERS
/**
 * call sys_timeout in tcpip_thread
 *
 * @param msecs time in milliseconds for timeout
 * @param h function to be called on timeout
 * @param arg argument to pass to timeout function h
 * @return ERR_MEM on memory error, ERR_OK otherwise
 */
err_t
tcpip_timeout(u32_t msecs, sys_timeout_handler h, void *arg)
{
  struct tcpip_msg *msg;
#if !USE_CUSTOMER_THREAD
  LWIP_ASSERT("Invalid mbox", sys_mbox_valid_val(tcpip_mbox));
#endif
  msg = (struct tcpip_msg *)memp_malloc(MEMP_TCPIP_MSG_API);
  if (msg == NULL) {
    return ERR_MEM;
  }

  msg->type = TCPIP_MSG_TIMEOUT;
  msg->msg.tmo.msecs = msecs;
  msg->msg.tmo.h = h;
  msg->msg.tmo.arg = arg;
#if !USE_CUSTOMER_THREAD
  sys_mbox_post(&tcpip_mbox, msg);
#else
  sys_post_event_to_APS(msg, TCPIP_MSG_TIMEOUT);
#endif
  return ERR_OK;
}

/**
 * call sys_untimeout in tcpip_thread
 *
 * @param h function to be called on timeout
 * @param arg argument to pass to timeout function h
 * @return ERR_MEM on memory error, ERR_OK otherwise
 */
err_t
tcpip_untimeout(sys_timeout_handler h, void *arg)
{
  struct tcpip_msg *msg;
  
#if !USE_CUSTOMER_THREAD
  LWIP_ASSERT("Invalid mbox", sys_mbox_valid_val(tcpip_mbox));
#endif

  msg = (struct tcpip_msg *)memp_malloc(MEMP_TCPIP_MSG_API);
  if (msg == NULL) {
    return ERR_MEM;
  }

  msg->type = TCPIP_MSG_UNTIMEOUT;
  msg->msg.tmo.h = h;
  msg->msg.tmo.arg = arg;
  
#if !USE_CUSTOMER_THREAD
  sys_mbox_post(&tcpip_mbox, msg);
#else
  sys_post_event_to_APS(&msg, TCPIP_MSG_UNTIMEOUT);
#endif
  return ERR_OK;
}
#endif /* LWIP_TCPIP_TIMEOUT && LWIP_TIMERS */


/**
 * Sends a message to TCPIP thread to call a function. Caller thread blocks on
 * on a provided semaphore, which ist NOT automatically signalled by TCPIP thread,
 * this has to be done by the user.
 * It is recommended to use LWIP_TCPIP_CORE_LOCKING since this is the way
 * with least runtime overhead.
 *
 * @param fn function to be called from TCPIP thread
 * @param apimsg argument to API function
 * @param sem semaphore to wait on
 * @return ERR_OK if the function was called, another err_t if not
 */
err_t
tcpip_send_msg_wait_sem(tcpip_callback_fn fn, void *apimsg, sys_sem_t *sem)
{
  sys_arch_printf("in tcpip_send_msg_wait_sem");
#if LWIP_TCPIP_CORE_LOCKING
  LWIP_UNUSED_ARG(sem);
  LOCK_TCPIP_CORE();
  sys_arch_printf("in tcpip_send_msg_wait_sem before fn");
  fn(apimsg);
  sys_arch_printf("in tcpip_send_msg_wait_sem after fn");
  UNLOCK_TCPIP_CORE();
  return ERR_OK;
#else /* LWIP_TCPIP_CORE_LOCKING */
  TCPIP_MSG_VAR_DECLARE(msg);

  LWIP_ASSERT("semaphore not initialized", sys_sem_valid(sem));
#if !USE_CUSTOMER_THREAD
  LWIP_ASSERT("Invalid mbox", sys_mbox_valid_val(tcpip_mbox));
#endif
  TCPIP_MSG_VAR_ALLOC(msg);
  TCPIP_MSG_VAR_REF(msg).type = TCPIP_MSG_API;
  TCPIP_MSG_VAR_REF(msg).msg.api_msg.function = fn;
  TCPIP_MSG_VAR_REF(msg).msg.api_msg.msg = apimsg;
#if !USE_CUSTOMER_THREAD
  sys_mbox_post(&tcpip_mbox, &TCPIP_MSG_VAR_REF(msg));
 #else
  sys_arch_printf("in tcpip_send_msg_wait_sem sys_post_event_to_APS");
  if (!sys_post_event_to_APS(&TCPIP_MSG_VAR_REF(msg), TCPIP_MSG_API)) {
    TCPIP_MSG_VAR_FREE(msg);
    return ERR_MEM;
  }
#endif
  sys_arch_sem_wait(sem, 0);
  TCPIP_MSG_VAR_FREE(msg);
  return ERR_OK;
#endif /* LWIP_TCPIP_CORE_LOCKING */
}

/**
 * Synchronously calls function in TCPIP thread and waits for its completion.
 * It is recommended to use LWIP_TCPIP_CORE_LOCKING (preferred) or
 * LWIP_NETCONN_SEM_PER_THREAD.
 * If not, a semaphore is created and destroyed on every call which is usually
 * an expensive/slow operation.
 * @param fn Function to call
 * @param call Call parameters
 * @return Return value from tcpip_api_call_fn
 */
err_t
tcpip_api_call(tcpip_api_call_fn fn, struct tcpip_api_call_data *call)
{
#if LWIP_TCPIP_CORE_LOCKING
  err_t err;
  LOCK_TCPIP_CORE();
  err = fn(call);
  UNLOCK_TCPIP_CORE();
  return err;
#else /* LWIP_TCPIP_CORE_LOCKING */
  TCPIP_MSG_VAR_DECLARE(msg);

#if !LWIP_NETCONN_SEM_PER_THREAD
  err_t err = sys_sem_new(&call->sem, 0);
  if (err != ERR_OK) {
    return err;
  }
#endif /* LWIP_NETCONN_SEM_PER_THREAD */
#if !USE_CUSTOMER_THREAD
  LWIP_ASSERT("Invalid mbox", sys_mbox_valid_val(tcpip_mbox));
#endif
  TCPIP_MSG_VAR_ALLOC(msg);
  TCPIP_MSG_VAR_REF(msg).type = TCPIP_MSG_API_CALL;
  TCPIP_MSG_VAR_REF(msg).msg.api_call.arg = call;
  TCPIP_MSG_VAR_REF(msg).msg.api_call.function = fn;
#if LWIP_NETCONN_SEM_PER_THREAD
  TCPIP_MSG_VAR_REF(msg).msg.api_call.sem = LWIP_NETCONN_THREAD_SEM_GET();
#else /* LWIP_NETCONN_SEM_PER_THREAD */
  TCPIP_MSG_VAR_REF(msg).msg.api_call.sem = &call->sem;
#endif /* LWIP_NETCONN_SEM_PER_THREAD */
#if !USE_CUSTOMER_THREAD
  sys_mbox_post(&tcpip_mbox, &TCPIP_MSG_VAR_REF(msg));
#else
  if (!sys_post_event_to_APS(&TCPIP_MSG_VAR_REF(msg), TCPIP_MSG_API_CALL)) {
     TCPIP_MSG_VAR_FREE(msg);
#if !LWIP_NETCONN_SEM_PER_THREAD
     sys_sem_free(&call->sem);
#endif /* LWIP_NETCONN_SEM_PER_THREAD */
     return ERR_MEM;
  }
#endif
  sys_arch_sem_wait(TCPIP_MSG_VAR_REF(msg).msg.api_call.sem, 0);
  TCPIP_MSG_VAR_FREE(msg);

#if !LWIP_NETCONN_SEM_PER_THREAD
  sys_sem_free(&call->sem);
#endif /* LWIP_NETCONN_SEM_PER_THREAD */

  return call->err;
#endif /* LWIP_TCPIP_CORE_LOCKING */
}

/**
 * @ingroup lwip_os
 * Allocate a structure for a static callback message and initialize it.
 * The message has a special type such that lwIP never frees it.
 * This is intended to be used to send "static" messages from interrupt context,
 * e.g. the message is allocated once and posted several times from an IRQ
 * using tcpip_callbackmsg_trycallback().
 * Example usage: Trigger execution of an ethernet IRQ DPC routine in lwIP thread context.
 *
 * @param function the function to call
 * @param ctx parameter passed to function
 * @return a struct pointer to pass to tcpip_callbackmsg_trycallback().
 *
 * @see tcpip_callbackmsg_trycallback()
 * @see tcpip_callbackmsg_delete()
 */
struct tcpip_callback_msg *
tcpip_callbackmsg_new(tcpip_callback_fn function, void *ctx)
{
  struct tcpip_msg *msg = (struct tcpip_msg *)memp_malloc(MEMP_TCPIP_MSG_API);
  if (msg == NULL) {
    return NULL;
  }
  msg->type = TCPIP_MSG_CALLBACK_STATIC;
  msg->msg.cb.function = function;
  msg->msg.cb.ctx = ctx;
  return (struct tcpip_callback_msg *)msg;
}

/**
 * @ingroup lwip_os
 * Free a callback message allocated by tcpip_callbackmsg_new().
 *
 * @param msg the message to free
 *
 * @see tcpip_callbackmsg_new()
 */
void
tcpip_callbackmsg_delete(struct tcpip_callback_msg *msg)
{
  memp_free(MEMP_TCPIP_MSG_API, msg);
}

/**
 * @ingroup lwip_os
 * Try to post a callback-message to the tcpip_thread tcpip_mbox.
 *
 * @param msg pointer to the message to post
 * @return sys_mbox_trypost() return code
 *
 * @see tcpip_callbackmsg_new()
 */
err_t
tcpip_callbackmsg_trycallback(struct tcpip_callback_msg *msg)
{
#if !USE_CUSTOMER_THREAD
  LWIP_ASSERT("Invalid mbox", sys_mbox_valid_val(tcpip_mbox));
  return sys_mbox_trypost(&tcpip_mbox, msg);
#else
  sys_post_event_to_APS(msg, TCPIP_MSG_CALLBACK_STATIC);
  return ERR_OK;
#endif  
}

/**
 * @ingroup lwip_os
 * Try to post a callback-message to the tcpip_thread mbox.
 * Same as @ref tcpip_callbackmsg_trycallback but calls sys_mbox_trypost_fromisr(),
 * mainly to help FreeRTOS, where calls differ between task level and ISR level.
 *
 * @param msg pointer to the message to post
 * @return sys_mbox_trypost_fromisr() return code (without change, so this
 *         knowledge can be used to e.g. propagate "bool needs_scheduling")
 *
 * @see tcpip_callbackmsg_new()
 */
err_t
tcpip_callbackmsg_trycallback_fromisr(struct tcpip_callback_msg *msg)
{
#if !USE_CUSTOMER_THREAD
  LWIP_ASSERT("Invalid mbox", sys_mbox_valid_val(tcpip_mbox));
  return sys_mbox_trypost_fromisr(&tcpip_mbox, msg);
#else
  sys_post_event_to_APS(msg, TCPIP_MSG_CALLBACK_STATIC);
  return ERR_OK;
#endif  
}

/**
 * Sends a message to TCPIP thread to call a function. Caller thread blocks
 * until the function returns.
 * It is recommended to use LWIP_TCPIP_CORE_LOCKING (preferred) or
 * LWIP_NETCONN_SEM_PER_THREAD.
 * If not, a semaphore is created and destroyed on every call which is usually
 * an expensive/slow operation.
 *
 * @param function the function to call
 * @param ctx parameter passed to f
 * @return ERR_OK if the function was called, another err_t if not
 */
err_t
tcpip_callback_wait(tcpip_callback_fn function, void *ctx)
{
#if LWIP_TCPIP_CORE_LOCKING
  LOCK_TCPIP_CORE();
  function(ctx);
  UNLOCK_TCPIP_CORE();
  return ERR_OK;
#else /* LWIP_TCPIP_CORE_LOCKING */
  err_t err;
  sys_sem_t sem;
  struct tcpip_msg msg;

  LWIP_ASSERT("Invalid mbox", sys_mbox_valid_val(tcpip_mbox));

  err = sys_sem_new(&sem, 0);
  if (err != ERR_OK) {
    return err;
  }

  msg.type = TCPIP_MSG_CALLBACK_STATIC_WAIT;
  msg.msg.cb_wait.function = function;
  msg.msg.cb_wait.ctx = ctx;
  msg.msg.cb_wait.sem = &sem;
  sys_mbox_post(&tcpip_mbox, &msg);
  sys_arch_sem_wait(&sem, 0);
  sys_sem_free(&sem);
  return ERR_OK;
#endif /* LWIP_TCPIP_CORE_LOCKING */
}

/**
 * @ingroup lwip_os
 * Initialize this module:
 * - initialize all sub modules
 * - start the tcpip_thread
 *
 * @param initfunc a function to call when tcpip_thread is running and finished initializing
 * @param arg argument to pass to initfunc
 */
void
tcpip_init(tcpip_init_done_fn initfunc, void *arg)
{
  lwip_init();
#if !USE_CUSTOMER_THREAD
  tcpip_init_done = initfunc;
  tcpip_init_done_arg = arg;
  if (sys_mbox_new(&tcpip_mbox, TCPIP_MBOX_SIZE) != ERR_OK) {
    LWIP_ASSERT("failed to create tcpip_thread mbox", 0);
  }
#endif
#if LWIP_TCPIP_CORE_LOCKING
  if (sys_mutex_new(&lock_tcpip_core) != ERR_OK) {
    LWIP_ASSERT("failed to create lock_tcpip_core", 0);
  }
#endif /* LWIP_TCPIP_CORE_LOCKING */
#if !USE_CUSTOMER_THREAD
  sys_thread_new(TCPIP_THREAD_NAME, tcpip_thread, NULL, TCPIP_THREAD_STACKSIZE, TCPIP_THREAD_PRIO);
#endif
#if !LWIP_TIMERS

  sys_timeouts_init();
#endif
}

/**
 * Simple callback function used with tcpip_callback to free a pbuf
 * (pbuf_free has a wrong signature for tcpip_callback)
 *
 * @param p The pbuf (chain) to be dereferenced.
 */
static void
pbuf_free_int(void *p)
{
  struct pbuf *q = (struct pbuf *)p;
  pbuf_free(q);
}

/**
 * A simple wrapper function that allows you to free a pbuf from interrupt context.
 *
 * @param p The pbuf (chain) to be dereferenced.
 * @return ERR_OK if callback could be enqueued, an err_t if not
 */
err_t
pbuf_free_callback(struct pbuf *p)
{
  return tcpip_try_callback(pbuf_free_int, p);
}

/**
 * A simple wrapper function that allows you to free heap memory from
 * interrupt context.
 *
 * @param m the heap memory to free
 * @return ERR_OK if callback could be enqueued, an err_t if not
 */
err_t
mem_free_callback(void *m)
{
  return tcpip_try_callback(mem_free, m);
}

#endif /* !NO_SYS */
