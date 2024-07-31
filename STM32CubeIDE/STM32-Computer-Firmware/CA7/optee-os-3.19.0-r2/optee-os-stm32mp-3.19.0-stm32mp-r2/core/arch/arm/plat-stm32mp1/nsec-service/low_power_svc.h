/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2018-2021, STMicroelectronics
 */

#ifndef LOW_POWER_SVC_H
#define LOW_POWER_SVC_H

#include <sm/sm.h>
#include <stdint.h>

#include "stm32mp1_smc.h"

#ifdef CFG_STM32_LOWPOWER_SIP
uint32_t pm_domain_scv_handler(uint32_t x1, uint32_t x2);
#else
static inline uint32_t pm_domain_scv_handler(uint32_t x1 __unused,
					     uint32_t x2 __unused)
{
	return STM32_SIP_SVC_FAILED;
}
#endif /* CFG_STM32_LOWPOWER_SIP */
#endif /* LOW_POWER_SVC_H */
