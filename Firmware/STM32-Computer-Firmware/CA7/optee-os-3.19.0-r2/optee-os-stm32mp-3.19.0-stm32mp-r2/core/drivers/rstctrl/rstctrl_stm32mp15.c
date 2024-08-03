// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2018-2022, STMicroelectronics
 */

#include <drivers/rstctrl.h>
#include <drivers/stm32_rstctrl.h>
#include <dt-bindings/reset/stm32mp1-resets.h>
#include <kernel/dt.h>

static TEE_Result reset_assert_mcu(struct rstctrl *rstctrl,
				   unsigned int to_us __unused)
{
	/* MCU reset can only be written */
	return stm32_reset_assert_clr_offset(rstctrl, 0);
}

static TEE_Result reset_deassert_mcu(struct rstctrl *rstctrl __unused,
				     unsigned int to_us __unused)
{
	/* MCU reset deasserts by its own */
	return TEE_SUCCESS;
}

static struct rstctrl_ops stm32_rstctrl_mcu_ops = {
	.assert_level = reset_assert_mcu,
	.deassert_level = reset_deassert_mcu,
};

static struct rstctrl_ops *stm32_reset_mp15_find_ops(unsigned int id)
{
	switch (id) {
	case MCU_R:
		return &stm32_rstctrl_mcu_ops;
	case MCU_HOLD_BOOT_R:
		return &stm32_rstctrl_inverted_ops;
	default:
		return &stm32_rstctrl_clr_offset_ops;
	}
}

const struct stm32_reset_data stm32mp15_reset = {
	.reset_get_ops = stm32_reset_mp15_find_ops
};

static const struct dt_device_match stm32_rstctrl_match_table[] = {
	{
		.compatible = "st,stm32mp1-rcc",
		.compat_data = &stm32mp15_reset,
	},
	{
		.compatible = "st,stm32mp1-rcc-secure",
		.compat_data = &stm32mp15_reset,
	},
	{ }
};

DEFINE_DT_DRIVER(stm32mp15_rstctrl_dt_driver) = {
	.name = "stm32mp15_rstctrl",
	.type = DT_DRIVER_RSTCTRL,
	.match_table = stm32_rstctrl_match_table,
	.probe = stm32_rstctrl_provider_probe,
};
