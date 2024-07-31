/* SPDX-License-Identifier: (GPL-2.0 OR BSD-3-Clause) */
/*
 * Copyright (c) 2020-2022, STMicroelectronics
 */
#ifndef __STM32_LPTIMER_H
#define __STM32_LPTIMER_H

#ifndef TFM_ENV
/* optee */
#include <types_ext.h>
#endif

struct lptimer_ext_input_cfg {
	/* 2 input mux could be available */
	uint8_t mux[2];
	uint8_t polarity;
	uint8_t num;
};

struct lptimer_ext_trigger_cfg {
	uint8_t mux;
	uint8_t polarity;
	uint8_t num;
};

struct lptimer_driver_data {
	uint32_t version;
	uint8_t nb_ext_input;
	uint8_t nb_ext_trigger;
	uint8_t encoder:1;
};

#define LPTIMER_MODE_COUNTER BIT(0)

struct stm32_lptimer_platdata {
	const char *name;
	int phandle;
	uintptr_t base;
	struct clk *clock;
	int irq;
	struct lptimer_ext_trigger_cfg ext_trigger;
	struct lptimer_ext_input_cfg ext_input;
	uint8_t mode;
};

TEE_Result stm32_lptimer_get_platdata(struct stm32_lptimer_platdata *pdata);
int stm32_lptimer_init(void);
#endif /* __STM32_LPTIMER_H */
