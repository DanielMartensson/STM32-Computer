/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2018-2022, STMicroelectronics
 */

#ifndef __STM32_TIM_H
#define __STM32_TIM_H

struct stm32_timer_platdata {
	const char *name;
	int phandle;
	uintptr_t base;
	struct clk *clock;
	int irq;
	uint8_t mode;
};

TEE_Result stm32_timer_get_platdata(struct stm32_timer_platdata *pdata __unused);
#endif /* __STM32_TIM_H */
