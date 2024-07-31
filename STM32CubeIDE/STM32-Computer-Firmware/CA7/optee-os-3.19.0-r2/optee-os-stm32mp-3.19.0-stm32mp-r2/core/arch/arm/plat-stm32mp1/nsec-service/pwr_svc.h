/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2017-2018, STMicroelectronics
 */

#ifndef __PWR_SVC_H__
#define __PWR_SVC_H__

#include <stm32mp1_smc.h>

#ifdef CFG_STM32_PWR_SIP
uint32_t pwr_scv_handler(uint32_t x1, uint32_t x2, uint32_t x3);
#else
static inline uint32_t pwr_scv_handler(uint32_t x1 __unused,
				       uint32_t x2 __unused,
				       uint32_t x3 __unused)
{
	return STM32_SIP_SVC_FAILED;
}
#endif

#endif /* __PWR_SVC_H__*/
