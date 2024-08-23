/*
 * Copyright (C) 2014 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef TRYSTY_IRQ_H_
#define TRYSTY_IRQ_H_

#define SEND_NOP_EVENT      20

extern osiThread_t *secirq_proc_task;

int trusty_irq_call_notify();
int trusty_irq_init();
void process_security_irq(void);

#endif /* TRYSTY_IRQ_H_ */
