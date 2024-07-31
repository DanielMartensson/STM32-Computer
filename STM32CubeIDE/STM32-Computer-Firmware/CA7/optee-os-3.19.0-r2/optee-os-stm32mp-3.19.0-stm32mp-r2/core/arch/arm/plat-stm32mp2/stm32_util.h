/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022, STMicroelectronics
 */

#ifndef __STM32_UTIL_H__
#define __STM32_UTIL_H__

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <types_ext.h>

#include <drivers/stm32mp2_rcc_util.h>

static inline void stm32mp_register_secure_periph_iomem(vaddr_t base __unused)
{
}

static inline void stm32mp_register_non_secure_periph_iomem(vaddr_t base
							    __unused) { }

/*
 * Generic spinlock function that bypass spinlock if MMU is disabled or
 * lock is NULL.
 */
uint32_t may_spin_lock(unsigned int *lock);
void may_spin_unlock(unsigned int *lock, uint32_t exceptions);
#ifdef CFG_STM32_CPU_OPP
bool stm32mp_supports_cpu_opp(uint32_t opp_id);
#endif /*CFG_STM32_CPU_OPP*/

void __noreturn do_reset(const char *str);

TEE_Result stm32_activate_internal_tamper(int id);

/**
 * stm32_rif_reconfigure_mem_region() - Allows to reconfigure a previously
 *					configured memory region.
 * @fdt:	Device tree file to work on
 * @phandle:	Phandle of the memory region
 * @config:	When @config points to a NULL reference, the function grants
 *		access to OP-TEE  compartment and returns in @config a pointer
 *		to the previously apply configuration, to be later used in a
 *		call to the same API function.
 *		When @config points to a non-NULL reference, that reference is
 *		expected to be obtained from a previous call to this function
 *		and contains the configuration to be applied. In such case, the
 *		function set @config output value to a NULL reference.
 */
TEE_Result stm32_rif_reconfigure_mem_region(const void *fdt, int phandle,
					    void **config);

#endif /*__STM32_UTIL_H__*/
