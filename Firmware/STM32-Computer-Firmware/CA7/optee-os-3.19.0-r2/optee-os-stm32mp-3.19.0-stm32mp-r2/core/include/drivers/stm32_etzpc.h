/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2014-2021, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2018-2021, STMicroelectronics
 */

#ifndef __STM32_ETZPC_H__
#define __STM32_ETZPC_H__

#include <drivers/clk.h>
#include <util.h>
#include <types_ext.h>

enum etzpc_decprot_attributes {
	ETZPC_DECPROT_S_RW = 0,
	ETZPC_DECPROT_NS_R_S_W = 1,
	ETZPC_DECPROT_MCU_ISOLATION = 2,
	ETZPC_DECPROT_NS_RW = 3,
	ETZPC_DECPROT_MAX = 4,
};

struct stm32_etzpc_platdata {
	const char *name;
	uintptr_t base;
	struct clk *clk;
	uint8_t *periph_cfg;
	uint16_t *tzma_cfg;
};

#define ETZPC_TZMA_ALL_SECURE		GENMASK_32(9, 0)
#define ETZPC_TZMA_ALL_NO_SECURE	0x0

#ifdef CFG_STM32_ETZPC
void stm32_etzpc_init(paddr_t base);
#else
static inline void stm32_etzpc_init(paddr_t __unused base) {}
#endif
#endif /*__STM32_ETZPC_H__*/
