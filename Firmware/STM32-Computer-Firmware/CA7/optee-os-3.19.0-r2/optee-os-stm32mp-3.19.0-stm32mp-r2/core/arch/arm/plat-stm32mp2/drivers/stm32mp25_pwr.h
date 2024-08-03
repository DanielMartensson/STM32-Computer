/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022, STMicroelectronics
 */

#ifndef __STM32MP25_PWR_H
#define __STM32MP25_PWR_H

#include <kernel/interrupt.h>
#include <tee_api_types.h>
#include <types_ext.h>
#include <util.h>

#ifdef CFG_STM32_PWR_IRQ
/*
 * Flags for PWR wakeup event management
 * PWR_WKUP_FLAG_RISING - Detect event on signal rising edge
 * PWR_WKUP_FLAG_FALLING - Detect event on signal falling edge
 * PWR_WKUP_FLAG_THREADED - Notify event in the threaded context
 */
#define PWR_WKUP_FLAG_RISING	0
#define PWR_WKUP_FLAG_FALLING	BIT(0)
#define PWR_WKUP_FLAG_THREADED	BIT(1)

/* wakeup-pins irq chip */
enum pwr_wkup_pins {
	PWR_WKUP_PIN1 = 0,
	PWR_WKUP_PIN2,
	PWR_WKUP_PIN3,
	PWR_WKUP_PIN4,
	PWR_WKUP_PIN5,
	PWR_WKUP_PIN6,
	PWR_NB_WAKEUPPINS
};

enum pwr_monitoring {
	PWR_MON_V08CAP_TEMP,
	PWR_MON_VCORE,
	PWR_MON_VCPU,
};

TEE_Result
stm32mp25_pwr_itr_alloc_add(size_t it, itr_handler_t handler, uint32_t flags,
			    void *data, struct itr_handler **phdl);

void stm32mp25_pwr_itr_enable(size_t it);
void stm32mp25_pwr_itr_disable(size_t it);

TEE_Result
stm32mp25_pwr_irq_probe(const void *fdt, int node, int interrupt);
#endif /* CFG_STM32_PWR_IRQ */

vaddr_t stm32_pwr_base(void);

void stm32mp_pwr_monitoring_enable(enum pwr_monitoring monitoring);

#endif /*__STM32MP25_PWR_H*/
