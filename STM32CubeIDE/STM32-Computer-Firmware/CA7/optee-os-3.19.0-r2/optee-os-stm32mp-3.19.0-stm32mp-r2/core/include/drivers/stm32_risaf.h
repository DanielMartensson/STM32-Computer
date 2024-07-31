/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022, STMicroelectronics - All Rights Reserved
 */

#ifndef __DRIVERS_STM32_RISAF_H__
#define __DRIVERS_STM32_RISAF_H__

#include <tee_api_types.h>
#include <trace.h>
#include <types_ext.h>

TEE_Result stm32_risaf_reconfigure(paddr_t base);

/**
 * stm32_risaf_reconfigure_region() - Allows to reconfigure a previously
 *				    configured memory region.
 * @addr:	Base address of the region
 * @len:	Size of the region
 * @config:	When @config points to a NULL reference, the function grants
 *		access to OP-TEE  compartment and returns in @config a pointer
 *		to the previously apply configuration, to be later used in a
 *		call to the same API function.
 *		When @config points to a non-NULL reference, that reference is
 *		expected to be obtained from a previous call to this function
 *		and contains the configuration to be applied. In such case, the
 *		function set @config output value to a NULL reference.
 */
TEE_Result stm32_risaf_reconfigure_region(uintptr_t addr, size_t len,
					  void **conf);

void stm32_risaf_clear_illegal_access_flags(void);

#ifdef CFG_TEE_CORE_DEBUG
void stm32_risaf_dump_erroneous_data(void);
#else /* CFG_TEE_CORE_DEBUG */
static inline void stm32_risaf_dump_erroneous_data(void)
{
}
#endif /* CFG_TEE_CORE_DEBUG */

#endif /*__DRIVERS_STM32_RISAF_H__*/
