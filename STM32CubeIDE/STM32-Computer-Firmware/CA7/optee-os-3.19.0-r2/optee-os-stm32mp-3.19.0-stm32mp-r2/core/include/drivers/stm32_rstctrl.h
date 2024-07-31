/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2018-2022, STMicroelectronics
 */

#include <drivers/rstctrl.h>

struct stm32_reset_data {
	struct rstctrl_ops * (*reset_get_ops)(unsigned int id);
};

TEE_Result stm32_reset_assert(struct rstctrl *rstctrl, unsigned int to_us);
TEE_Result stm32_reset_deassert(struct rstctrl *rstctrl, unsigned int to_us);

TEE_Result stm32_reset_assert_clr_offset(struct rstctrl *rstctrl,
					 unsigned int to_us);
TEE_Result stm32_reset_deassert_clr_offset(struct rstctrl *rstctrl,
					   unsigned int to_us);

extern struct rstctrl_ops stm32_rstctrl_ops;
extern struct rstctrl_ops stm32_rstctrl_inverted_ops;
extern struct rstctrl_ops stm32_rstctrl_clr_offset_ops;

TEE_Result stm32_rstctrl_provider_probe(const void *fdt, int offs,
					const void *compat_data);
