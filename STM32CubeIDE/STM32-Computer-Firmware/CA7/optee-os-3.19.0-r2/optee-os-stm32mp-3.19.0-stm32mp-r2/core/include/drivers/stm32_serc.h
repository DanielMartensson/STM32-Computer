/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2024, STMicroelectronics
 */
#ifndef __DRIVERS_STM32_SERC_H__
#define __DRIVERS_STM32_SERC_H__

#include <types_ext.h>

struct serc_driver_data {
	uint32_t version;
	uint8_t num_ilac;
};

struct stm32_serc_platdata {
	uintptr_t base;
	struct clk *clock;
	int irq;
};

/* Helper to print and handle SERC ILACs */
#ifdef CFG_STM32_SERC
void stm32_serc_handle_ilac(void);
#else /* CFG_STM32_SERC */
static inline void stm32_serc_handle_ilac(void) { };
#endif /* CFG_STM32_SERC */

#endif /* __DRIVERS_STM32_SERC_H__ */
